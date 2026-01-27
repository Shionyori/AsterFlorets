#include "AsterSelect.h"
#include "AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QListView>
#include <QStyledItemDelegate>
#include <QEvent>

namespace AsterUI {

    // 自定义委托以美化下拉列表项
    class AsterSelectDelegate : public QStyledItemDelegate {
    public:
        using QStyledItemDelegate::QStyledItemDelegate;

        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);

            // 背景绘制
            if (option.state & QStyle::State_MouseOver) {
                // 悬停背景 (浅色 Primary)
                QColor hoverColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Primary);
                hoverColor.setAlpha(20); // 10% opacity
                painter->fillRect(option.rect, hoverColor);
            } else if (option.state & QStyle::State_Selected) {
                // 选中项背景 (稍深一点)
                QColor selectedColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Primary);
                selectedColor.setAlpha(40); // 20% opacity
                painter->fillRect(option.rect, selectedColor);
            } else {
                painter->fillRect(option.rect, AsterTheme::instance()->color(AsterTheme::ColorRole::Surface));
            }

            // 文字绘制
            painter->setPen(AsterTheme::instance()->color(AsterTheme::ColorRole::Text));
            
            // 如果选中，字体加粗
            if (option.state & QStyle::State_Selected) {
                QFont font = option.font;
                font.setBold(true);
                painter->setFont(font);
                // 也可以改变文字颜色为 Primary
                painter->setPen(AsterTheme::instance()->color(AsterTheme::ColorRole::Primary));
            }
            
            QRect textRect = option.rect.adjusted(12, 0, -12, 0); // Padding
            painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, index.data().toString());

            painter->restore();
        }
        
        QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
            QSize s = QStyledItemDelegate::sizeHint(option, index);
            s.setHeight(36); // 固定高度，更舒适的点击区域
            return s;
        }
    };

    AsterSelect::AsterSelect(QWidget* parent)
        : QComboBox(parent)
    {
        init();
    }

    AsterSelect::~AsterSelect() = default;

    void AsterSelect::init() {
        auto theme = AsterTheme::instance();
        m_borderColor = theme->color(AsterTheme::ColorRole::Border);
        
        // 配置下拉视图
        auto* listView = new QListView(this);
        listView->setItemDelegate(new AsterSelectDelegate(this));
        listView->setFrameShape(QFrame::NoFrame);
        listView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        
        // 设置下拉列表样式 (圆角、阴影等通常由 Window 系统管理，Qt 样式表可以辅助)
        // 注意：QComboBox 的 popup 是一个独立的窗口
        listView->setStyleSheet(QString(
            "QListView { "
            "   outline: 0px; "
            "   background-color: %1; "
            "   border: 1px solid %2; "
            "   border-radius: 4px; "
            "   outline: none; "
            "}"
        ).arg(theme->color(AsterTheme::ColorRole::Surface).name())
         .arg(theme->color(AsterTheme::ColorRole::Border).name()));
        
        setView(listView);

        // 动画初始化
        m_borderAnimation = new QPropertyAnimation(this, "borderColor", this);
        m_borderAnimation->setDuration(200);
        m_borderAnimation->setEasingCurve(QEasingCurve::OutQuad);
        
        setCursor(Qt::PointingHandCursor);
        // 增加一点内边距，防止文字紧贴边缘 (虽然 paintEvent 会重绘，但 sizeHint 可能用到)
    }

    void AsterSelect::setBorderColor(const QColor& color) {
        m_borderColor = color;
        update();
    }

    void AsterSelect::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        auto theme = AsterTheme::instance();
        int radius = theme->borderRadius();

        // 1. 绘制背景和边框
        QRectF r = rect();
        r.adjust(0.5, 0.5, -0.5, -0.5);
        
        QPainterPath path;
        path.addRoundedRect(r, radius, radius);

        painter.fillPath(path, theme->color(AsterTheme::ColorRole::Surface));

        QPen pen(m_borderColor);
        // Removed manual override for hover/focus since m_borderColor is animated
        
        if (hasFocus()) {
             // pen.setColor(theme->color(AsterTheme::ColorRole::Primary)); // Already handled by animation
             pen.setWidth(1); 
        }
        painter.setPen(pen);
        painter.drawPath(path);
        
        // Focus Ring (Optional)
        if (hasFocus()) {
            QColor ringColor = m_borderColor; // Use animated color
            ringColor.setAlpha(40);
            QPainterPath ringPath;
            // ...
            // Let's draw a simple glow like LineEdit
            QPen glowPen(ringColor);
            glowPen.setWidth(4);
            painter.setPen(glowPen);
            painter.setBrush(Qt::NoBrush);
            painter.drawPath(path);
        }

        // 2. 绘制文字
        QRect textRect = rect().adjusted(12, 0, -30, 0); // Left padding 12, Right padding for arrow
        
        QString text = currentText();
        QColor textColor = theme->color(AsterTheme::ColorRole::Text);

        if (text.isEmpty() && !placeholderText().isEmpty()) {
            text = placeholderText();
            textColor = theme->color(AsterTheme::ColorRole::TextSecondary);
        }

        painter.setPen(textColor);
        
        // 获取显示的文本 (elided if too long)
        QString textToDraw = fontMetrics().elidedText(text, Qt::ElideRight, textRect.width());
        painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, textToDraw);

        // 3. 绘制箭头图标 (Chevron Down)
        int arrowW = 10;
        int arrowH = 6;
        int arrowX = width() - 12 - arrowW;
        int arrowY = (height() - arrowH) / 2;
        
        QPainterPath arrowPath;
        // 简单的 V 形
        arrowPath.moveTo(arrowX, arrowY);
        arrowPath.lineTo(arrowX + arrowW / 2.0, arrowY + arrowH);
        arrowPath.lineTo(arrowX + arrowW, arrowY);
        
        QPen arrowPen(theme->color(AsterTheme::ColorRole::TextSecondary));
        arrowPen.setWidth(2);
        arrowPen.setCapStyle(Qt::RoundCap);
        arrowPen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(arrowPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(arrowPath);
    }

    void AsterSelect::focusInEvent(QFocusEvent* event) {
        QComboBox::focusInEvent(event);
        animateBorder();
    }

    void AsterSelect::focusOutEvent(QFocusEvent* event) {
        QComboBox::focusOutEvent(event);
        animateBorder();
    }

    void AsterSelect::enterEvent(QEnterEvent* event) {
        m_isHovered = true;
        animateBorder();
        QComboBox::enterEvent(event);
    }

    void AsterSelect::leaveEvent(QEvent* event) {
        m_isHovered = false;
        animateBorder();
        QComboBox::leaveEvent(event);
    }

    void AsterSelect::animateBorder() {
        auto theme = AsterTheme::instance();
        QColor start = m_borderColor;
        QColor end;
        
        if (hasFocus()) {
            end = theme->color(AsterTheme::ColorRole::Primary);
        } else if (m_isHovered) {
            end = theme->color(AsterTheme::ColorRole::Primary);
        } else {
            end = theme->color(AsterTheme::ColorRole::Border);
        }

        m_borderAnimation->stop();
        m_borderAnimation->setStartValue(start);
        m_borderAnimation->setEndValue(end);
        m_borderAnimation->start();
    }

}
