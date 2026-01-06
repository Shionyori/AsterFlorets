#include "AsterUI/AsterCard.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <QLabel>

namespace AsterUI {

    AsterCard::AsterCard(QWidget* parent)
        : QFrame(parent)
    {
        init();
    }

    AsterCard::~AsterCard() = default;

    void AsterCard::init() {
        auto theme = AsterTheme::instance();
        m_backgroundColor = theme->color(AsterTheme::ColorRole::Surface);
        
        // 优化阴影 - 更柔和
        auto shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(20);
        shadow->setColor(QColor(0, 0, 0, 12));
        shadow->setOffset(0, 4);
        setGraphicsEffect(shadow);

        // 默认内边距
        setContentsMargins(24, 24, 24, 24);
        setMinimumSize(100, 60);
    }

    void AsterCard::setBackgroundColor(const QColor& color) {
        m_backgroundColor = color;
        update();
    }

    void AsterCard::setBorderRadius(int radius) {
        m_borderRadius = radius;
        update();
    }

    void AsterCard::setHoverable(bool hoverable) {
        m_hoverable = hoverable;
        if (m_hoverable) {
            setCursor(Qt::PointingHandCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }
    }

    void AsterCard::setTitle(const QString& title) {
        m_title = title;
        if (!m_title.isEmpty()) {
            // Header height 56px
            // Content starts at 56 + 16px padding
            setContentsMargins(24, 56 + 16, 24, 24);
        } else {
            setContentsMargins(24, 24, 24, 24);
        }
        update();
    }

    void AsterCard::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        auto theme = AsterTheme::instance();
        int radius = (m_borderRadius >= 0) ? m_borderRadius : theme->borderRadius();

        // 调整绘制区域，留出边框宽度的一半防止裁切
        QRectF rect = this->rect();
        rect.adjust(0.5, 0.5, -0.5, -0.5);

        QPainterPath path;
        path.addRoundedRect(rect, radius, radius);
        
        // 1. 背景
        painter.fillPath(path, m_backgroundColor);

        // 2. 边框
        // 使用更淡的分割线颜色作为卡片边框，或者降低透明度，避免视觉过重
        QColor borderColor = theme->color(AsterTheme::ColorRole::Border);
        borderColor.setAlpha(60); // 降低不透明度
        painter.setPen(QPen(borderColor, 1));
        painter.drawPath(path);

        // 3. 标题
        if (!m_title.isEmpty()) {
            int headerHeight = 56;

            painter.setPen(theme->color(AsterTheme::ColorRole::Text));
            QFont font = painter.font();
            font.setBold(true);
            font.setPixelSize(16);
            painter.setFont(font);
            
            // 标题文本
            QRect titleRect(24, 0, width() - 48, headerHeight);
            painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, m_title);
            
            // 标题分割线
            painter.setPen(theme->color(AsterTheme::ColorRole::Divider));
            painter.drawLine(0, headerHeight, width(), headerHeight);
        }
    }

    void AsterCard::enterEvent(QEnterEvent* event) {
        QFrame::enterEvent(event);
        if (m_hoverable) {
            m_isHovered = true;
            animateHover(true);
        }
    }

    void AsterCard::leaveEvent(QEvent* event) {
        QFrame::leaveEvent(event);
        if (m_hoverable) {
            m_isHovered = false;
            animateHover(false);
        }
    }

    void AsterCard::animateHover(bool hovered) {
        auto shadow = qobject_cast<QGraphicsDropShadowEffect*>(graphicsEffect());
        if (!shadow) return;

        if (m_hoverAnimGroup) {
            m_hoverAnimGroup->stop();
            delete m_hoverAnimGroup;
        }
        m_hoverAnimGroup = new QParallelAnimationGroup(this);

        // Only animate color (opacity) for a smooth fade-in/out effect
        auto colorAnim = new QPropertyAnimation(shadow, "color");
        colorAnim->setDuration(300); 
        colorAnim->setStartValue(shadow->color());
        // Slightly darker shadow on hover
        colorAnim->setEndValue(hovered ? QColor(0, 0, 0, 60) : QColor(0, 0, 0, 20));
        colorAnim->setEasingCurve(QEasingCurve::OutCubic);

        m_hoverAnimGroup->addAnimation(colorAnim);
        m_hoverAnimGroup->start();
    }

}
