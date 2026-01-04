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
        
        // 默认阴影效果
        auto shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(15);
        shadow->setColor(QColor(0, 0, 0, 20));
        shadow->setOffset(0, 2);
        setGraphicsEffect(shadow);

        // 确保有最小尺寸
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
        // 这里可以扩展：如果设置了标题，自动在内部布局顶部添加一个 Label
        // 目前为了保持通用性，暂只存储数据，由 paintEvent 简单绘制或留给用户布局
        update();
    }

    void AsterCard::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        auto theme = AsterTheme::instance();
        int radius = (m_borderRadius >= 0) ? m_borderRadius : theme->borderRadius();

        // 绘制背景
        QRectF r = rect();
        // 如果使用了 QGraphicsEffect，父级会处理阴影，但我们需要确保背景是不透明的
        // 并且要稍微留一点边距给阴影（虽然 QGraphicsEffect 是画在 widget 外部的，但有时需要调整 margin）
        
        QPainterPath path;
        path.addRoundedRect(r, radius, radius);
        
        painter.fillPath(path, m_backgroundColor);

        // 绘制标题 (简单实现)
        if (!m_title.isEmpty()) {
            painter.setPen(theme->color(AsterTheme::ColorRole::Text));
            QFont font = painter.font();
            font.setBold(true);
            font.setPointSize(10);
            painter.setFont(font);
            
            // 标题区域
            QRect titleRect(16, 16, width() - 32, 24);
            painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, m_title);
            
            // 分割线
            painter.setPen(QColor(0, 0, 0, 20));
            painter.drawLine(0, 48, width(), 48);
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
