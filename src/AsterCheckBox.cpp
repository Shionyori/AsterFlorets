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
        , m_isHovered(false)
    {
        init();
    }

    AsterCheckBox::~AsterCheckBox() = default;

    void AsterCheckBox::init() {
        setCursor(Qt::PointingHandCursor);
        m_animGroup = new QParallelAnimationGroup(this);
        updateColors();
    }

    void AsterCheckBox::setIndicatorColor(const QColor& color) {
        m_indicatorColor = color;
        update();
    }

    void AsterCheckBox::setBorderColor(const QColor& color) {
        m_borderColor = color;
        update();
    }

    void AsterCheckBox::updateColors() {
        auto theme = AsterTheme::instance();
        bool checked = isChecked();
        
        if (checked) {
            m_indicatorColor = theme->color(AsterTheme::ColorRole::Primary);
            m_borderColor = theme->color(AsterTheme::ColorRole::Primary);
        } else {
            m_indicatorColor = theme->color(AsterTheme::ColorRole::Surface); // White bg
            m_borderColor = m_isHovered ? theme->color(AsterTheme::ColorRole::Primary) 
                                        : theme->color(AsterTheme::ColorRole::Border);
        }
        update();
    }

    void AsterCheckBox::checkStateSet() {
        QCheckBox::checkStateSet();
        startAnimation(isChecked());
    }

    void AsterCheckBox::nextCheckState() {
        QCheckBox::nextCheckState();
    }

    void AsterCheckBox::enterEvent(QEnterEvent* event) {
        QCheckBox::enterEvent(event);
        m_isHovered = true;
        if (!isChecked()) {
            updateColors(); // Update border color on hover
        }
    }

    void AsterCheckBox::leaveEvent(QEvent* event) {
        QCheckBox::leaveEvent(event);
        m_isHovered = false;
        if (!isChecked()) {
            updateColors();
        }
    }

    void AsterCheckBox::startAnimation(bool checked) {
        auto theme = AsterTheme::instance();
        m_animGroup->stop();
        m_animGroup->clear();

        QColor targetBg = checked ? theme->color(AsterTheme::ColorRole::Primary) : theme->color(AsterTheme::ColorRole::Surface);
        QColor targetBorder = checked ? theme->color(AsterTheme::ColorRole::Primary) : theme->color(AsterTheme::ColorRole::Border);

        auto createAnim = [this](const QByteArray& prop, const QColor& start, const QColor& end) {
            auto anim = new QPropertyAnimation(this, prop);
            anim->setDuration(200);
            anim->setStartValue(start);
            anim->setEndValue(end);
            return anim;
        };

        m_animGroup->addAnimation(createAnim("indicatorColor", m_indicatorColor, targetBg));
        m_animGroup->addAnimation(createAnim("borderColor", m_borderColor, targetBorder));
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
        QRect boxRect(0, y, boxSize, boxSize);

        // 1. 绘制复选框背景和边框
        QPainterPath path;
        path.addRoundedRect(boxRect, 2, 2); // 2px 圆角

        painter.save();
        // 填充背景
        // 如果选中，填充 Primary 色；如果未选中，填充 Surface 色 (通常是白色)
        if (checkState() != Qt::Unchecked) {
             painter.fillPath(path, theme->color(AsterTheme::ColorRole::Primary));
        } else {
             painter.fillPath(path, theme->color(AsterTheme::ColorRole::Surface));
        }
        
        // 绘制边框 (内描边技巧)
        painter.setClipPath(path);
        QPen pen(m_borderColor);
        pen.setWidthF(2.0); // 2px width clipped to 1px
        painter.setPen(pen);
        painter.drawPath(path);
        painter.restore();

        // 2. 绘制勾选标记 (Checkmark)
        // 只有在选中或部分选中时绘制
        if (checkState() != Qt::Unchecked) {
            painter.save();
            QPen checkPen(Qt::white);
            checkPen.setWidthF(1.5);
            checkPen.setCapStyle(Qt::RoundCap);
            checkPen.setJoinStyle(Qt::RoundJoin);
            painter.setPen(checkPen);

            if (checkState() == Qt::PartiallyChecked) {
                // 绘制横线
                painter.drawLine(boxRect.left() + 3, boxRect.center().y(), boxRect.right() - 3, boxRect.center().y());
            } else {
                // 绘制对勾
                // 坐标点：(3, 8) -> (7, 12) -> (13, 5)  (相对于 16x16)
                QPainterPath checkPath;
                checkPath.moveTo(boxRect.x() + 3.5, boxRect.y() + 8.0);
                checkPath.lineTo(boxRect.x() + 6.5, boxRect.y() + 11.0);
                checkPath.lineTo(boxRect.x() + 12.5, boxRect.y() + 4.5);
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
