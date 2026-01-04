#include "AsterUI/AsterCheckBox.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>
#include <QPainterPath>

namespace AsterUI {

    AsterCheckBox::AsterCheckBox(const QString& text, QWidget* parent)
        : AsterCheckBox(parent)
    {
        setText(text);
    }

    AsterCheckBox::AsterCheckBox(QWidget* parent)
        : QCheckBox(parent)
        , m_checkScale(0.0)
        , m_isHovered(false)
    {
        init();
    }

    AsterCheckBox::~AsterCheckBox() = default;

    void AsterCheckBox::init() {
        setCursor(Qt::PointingHandCursor);
        m_animGroup = new QParallelAnimationGroup(this);
        
        connect(this, &QCheckBox::stateChanged, this, [this](int state) {
            startAnimation(state);
        });

        updateColors();
    }

    void AsterCheckBox::setCheckScale(double scale) {
        m_checkScale = scale;
        update();
    }

    void AsterCheckBox::setBorderColor(const QColor& color) {
        m_borderColor = color;
        update();
    }

    QSize AsterCheckBox::sizeHint() const {
        QSize size = QCheckBox::sizeHint();
        if (size.height() < 16) size.setHeight(16);
        return size;
    }

    bool AsterCheckBox::hitButton(const QPoint &pos) const {
        return rect().contains(pos);
    }

    void AsterCheckBox::updateColors() {
        auto theme = AsterTheme::instance();
        int state = checkState();
        
        if (state != Qt::Unchecked) {
            m_checkScale = 1.0;
            m_borderColor = theme->color(AsterTheme::ColorRole::Primary);
        } else {
            m_checkScale = 0.0;
            m_borderColor = m_isHovered ? theme->color(AsterTheme::ColorRole::Primary) 
                                        : theme->color(AsterTheme::ColorRole::Border);
        }
        update();
    }

    // Removed checkStateSet override

    void AsterCheckBox::nextCheckState() {
        QCheckBox::nextCheckState();
    }

    void AsterCheckBox::enterEvent(QEnterEvent* event) {
        QCheckBox::enterEvent(event);
        m_isHovered = true;
        if (checkState() == Qt::Unchecked) {
            updateColors(); // Update border color on hover
        }
    }

    void AsterCheckBox::leaveEvent(QEvent* event) {
        QCheckBox::leaveEvent(event);
        m_isHovered = false;
        if (checkState() == Qt::Unchecked) {
            updateColors();
        }
    }

    void AsterCheckBox::startAnimation(int state) {
        auto theme = AsterTheme::instance();
        m_animGroup->stop();
        m_animGroup->clear();

        bool checked = (state != Qt::Unchecked);
        QColor targetBorder = checked ? theme->color(AsterTheme::ColorRole::Primary) 
                                      : (m_isHovered ? theme->color(AsterTheme::ColorRole::Primary) : theme->color(AsterTheme::ColorRole::Border));
        
        double targetScale = checked ? 1.0 : 0.0;

        auto createColorAnim = [this](const QByteArray& prop, const QColor& start, const QColor& end) {
            auto anim = new QPropertyAnimation(this, prop);
            anim->setDuration(200);
            anim->setStartValue(start);
            anim->setEndValue(end);
            return anim;
        };

        auto createScaleAnim = [this](const QByteArray& prop, double start, double end) {
            auto anim = new QPropertyAnimation(this, prop);
            anim->setDuration(200);
            anim->setStartValue(start);
            anim->setEndValue(end);
            return anim;
        };

        m_animGroup->addAnimation(createScaleAnim("checkScale", m_checkScale, targetScale));
        m_animGroup->addAnimation(createColorAnim("borderColor", m_borderColor, targetBorder));
        m_animGroup->start();
    }

    void AsterCheckBox::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        auto theme = AsterTheme::instance();
        
        // 布局参数
        int boxSize = 16;
        int spacing = 8;
        
        // 垂直居中
        int y = (height() - boxSize) / 2;
        if (y < 0) y = 0;
        QRect boxRect(0, y, boxSize, boxSize);

        // 1. 绘制复选框背景和边框
        QPainterPath path;
        path.addRoundedRect(boxRect, 2, 2); // 2px 圆角

        painter.save();
        
        // 背景色逻辑：
        // 如果 checkScale > 0，说明正在动画或已选中，绘制 Primary 背景
        // 但为了平滑过渡，我们可以让背景色也随 checkScale 变化，或者简单地：
        // 当 checkScale > 0 时，绘制 Primary 背景，但透明度随 checkScale 变化？
        // 或者更简单：背景始终是白色，选中时绘制一个覆盖层。
        // Ant Design 的 CheckBox 选中时是实心 Primary 色。
        
        // 绘制白色底
        painter.fillPath(path, theme->color(AsterTheme::ColorRole::Surface));
        
        // 如果有选中进度，绘制 Primary 背景覆盖
        if (m_checkScale > 0.01) {
            QColor primary = theme->color(AsterTheme::ColorRole::Primary);
            // 简单的淡入淡出效果
            // primary.setAlphaF(m_checkScale); 
            // 或者缩放效果？通常 CheckBox 是颜色淡入或缩放。
            // 这里我们用缩放效果模拟：从中心向外扩散
            
            painter.setClipPath(path);
            // 计算缩放后的矩形
            // 实际上直接用透明度可能更自然，或者直接填充
            // 让我们尝试缩放填充：
            double scale = m_checkScale;
            // 居中缩放
            double w = boxSize * scale;
            double h = boxSize * scale;
            double x = boxRect.x() + (boxSize - w) / 2.0;
            double y = boxRect.y() + (boxSize - h) / 2.0;
            
            painter.setPen(Qt::NoPen);
            painter.setBrush(primary);
            painter.drawRoundedRect(QRectF(x, y, w, h), 2 * scale, 2 * scale);
        }
        
        // 绘制边框 (内描边技巧)
        // 边框颜色已经由动画控制
        painter.setClipPath(path);
        QPen pen(m_borderColor);
        pen.setWidthF(2.0); // 2px width clipped to 1px
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path);
        painter.restore();

        // 2. 绘制勾选标记 (Checkmark)
        // 只有在有进度时绘制
        if (m_checkScale > 0.01) {
            painter.save();
            QPen checkPen(Qt::white);
            checkPen.setWidthF(1.5);
            checkPen.setCapStyle(Qt::RoundCap);
            checkPen.setJoinStyle(Qt::RoundJoin);
            painter.setPen(checkPen);

            // 裁剪区域，实现 Checkmark 的动态显示（可选，或者直接随背景缩放显示）
            // 这里我们让 Checkmark 随背景一起缩放显示即可，因为背景是 Primary，Checkmark 是白色。
            // 如果背景很小，Checkmark 也会很小。
            
            // 为了更好的视觉效果，我们可以让 Checkmark 的绘制路径也进行缩放
            // 或者简单地，只在背景足够大时绘制 Checkmark
            
            // 重新计算 Checkmark 路径
            QPainterPath checkPath;
            if (checkState() == Qt::PartiallyChecked) {
                // 绘制横线
                double lineLen = (boxRect.width() - 6) * m_checkScale;
                double cx = boxRect.center().x();
                double cy = boxRect.center().y();
                painter.drawLine(QPointF(cx - lineLen/2, cy), QPointF(cx + lineLen/2, cy));
            } else {
                // 绘制对勾
                // 原始坐标：(3.5, 8) -> (6.5, 11) -> (12.5, 4.5)
                // 中心点大约在 (8, 8)
                // 我们对这些点相对于中心进行缩放
                
                auto scalePoint = [&](double px, double py) {
                    double cx = boxRect.x() + boxSize / 2.0;
                    double cy = boxRect.y() + boxSize / 2.0;
                    return QPointF(cx + (px - boxSize/2.0) * m_checkScale, 
                                   cy + (py - boxSize/2.0) * m_checkScale);
                };
                
                checkPath.moveTo(scalePoint(3.5, 8.0));
                checkPath.lineTo(scalePoint(6.5, 11.0));
                checkPath.lineTo(scalePoint(12.5, 4.5));
                painter.drawPath(checkPath);
            }
            painter.restore();
        }

        // 3. 绘制文本
        if (!text().isEmpty()) {
            QRect textRect = rect().adjusted(boxSize + spacing, 0, 0, 0);
            painter.setPen(theme->color(AsterTheme::ColorRole::Text));
            painter.setFont(font());
            painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text());
        }
    }

}
