#include "AsterSlider.h"
#include "AsterTheme.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>

namespace AsterUI {

    AsterSlider::AsterSlider(Qt::Orientation orientation, QWidget* parent)
        : QAbstractSlider(parent)
    {
        setOrientation(orientation);
        init();
    }

    AsterSlider::AsterSlider(QWidget* parent)
        : QAbstractSlider(parent)
    {
        setOrientation(Qt::Horizontal);
        init();
    }

    AsterSlider::~AsterSlider() = default;

    void AsterSlider::init() {
        auto theme = AsterTheme::instance();
        m_activeColor = theme->color(AsterTheme::ColorRole::Primary);
        m_inactiveColor = theme->color(AsterTheme::ColorRole::Border); 
        m_handleColor = theme->color(AsterTheme::ColorRole::Surface);
        
        setMinimum(0);
        setMaximum(100);
        setSingleStep(1);
        setPageStep(10);
        
        setCursor(Qt::PointingHandCursor);

        m_handleBorderOpacity = 0.0;

        m_hoverAnim = new QPropertyAnimation(this, "handleBorderOpacity", this);
        m_hoverAnim->setDuration(200);
        m_hoverAnim->setEasingCurve(QEasingCurve::OutCubic);
    }

    void AsterSlider::setActiveColor(const QColor& color) {
        m_activeColor = color;
        update();
    }

    void AsterSlider::setInactiveColor(const QColor& color) {
        m_inactiveColor = color;
        update();
    }

    void AsterSlider::setHandleColor(const QColor& color) {
        m_handleColor = color;
        update();
    }

    QSize AsterSlider::sizeHint() const {
        if (orientation() == Qt::Horizontal) {
            return QSize(150, 24);
        } else {
            return QSize(24, 150);
        }
    }

    int AsterSlider::valueToPixelPos(int val) const {
        int len = (orientation() == Qt::Horizontal) ? width() : height();
        int availableLen = len - 2 * m_handleRadius;
        
        double ratio = (double)(val - minimum()) / (maximum() - minimum());
        
        if (orientation() == Qt::Vertical) {
            // Vertical: bottom is min, top is max usually, or top-down?
            // Standard Qt vertical slider: bottom is min.
            // Let's follow standard: 0 at bottom.
            return height() - m_handleRadius - (int)(ratio * availableLen);
        } else {
            return m_handleRadius + (int)(ratio * availableLen);
        }
    }

    int AsterSlider::pixelPosToValue(int pos) const {
        int len = (orientation() == Qt::Horizontal) ? width() : height();
        int availableLen = len - 2 * m_handleRadius;
        
        int relativePos;
        if (orientation() == Qt::Vertical) {
            relativePos = height() - m_handleRadius - pos;
        } else {
            relativePos = pos - m_handleRadius;
        }
        
        double ratio = (double)relativePos / availableLen;
        ratio = qBound(0.0, ratio, 1.0);
        
        return minimum() + (int)(ratio * (maximum() - minimum()));
    }

    QRect AsterSlider::handleRect() const {
        int pos = valueToPixelPos(value());
        if (orientation() == Qt::Horizontal) {
            return QRect(pos - m_handleRadius, (height() - 2 * m_handleRadius) / 2, 
                         2 * m_handleRadius, 2 * m_handleRadius);
        } else {
            return QRect((width() - 2 * m_handleRadius) / 2, pos - m_handleRadius,
                         2 * m_handleRadius, 2 * m_handleRadius);
        }
    }

    void AsterSlider::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        int trackThickness = 4;
        int center = (orientation() == Qt::Horizontal) ? height() / 2 : width() / 2;
        
        // 1. Draw Inactive Track (Background)
        painter.setPen(Qt::NoPen);
        painter.setBrush(m_inactiveColor);
        
        if (orientation() == Qt::Horizontal) {
            painter.drawRoundedRect(m_handleRadius, center - trackThickness / 2, 
                                    width() - 2 * m_handleRadius, trackThickness, 
                                    trackThickness / 2, trackThickness / 2);
        } else {
            painter.drawRoundedRect(center - trackThickness / 2, m_handleRadius,
                                    trackThickness, height() - 2 * m_handleRadius,
                                    trackThickness / 2, trackThickness / 2);
        }

        // 2. Draw Active Track (Progress)
        painter.setBrush(m_activeColor);
        int handlePos = valueToPixelPos(value());
        
        if (orientation() == Qt::Horizontal) {
            int startX = m_handleRadius;
            int w = handlePos - startX;
            if (w > 0) {
                painter.drawRoundedRect(startX, center - trackThickness / 2, 
                                        w, trackThickness, 
                                        trackThickness / 2, trackThickness / 2);
            }
        } else {
            int startY = height() - m_handleRadius;
            int h = startY - handlePos;
            if (h > 0) {
                painter.drawRoundedRect(center - trackThickness / 2, handlePos,
                                        trackThickness, h,
                                        trackThickness / 2, trackThickness / 2);
            }
        }

        // 3. Draw Handle
        QRect hRect = handleRect();
        
        // Shadow
        painter.setBrush(QColor(0, 0, 0, 40));
        painter.drawEllipse(hRect.translated(0, 2));
        
        // Handle Body
        painter.setBrush(m_handleColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(hRect);

        // Border Ring (Hover/Drag effect)
        double opacity = m_handleBorderOpacity;
        if (m_isDragging) opacity = 1.0;

        if (opacity > 0.001) {
            QColor borderColor = m_activeColor;
            borderColor.setAlphaF(opacity);
            QPen pen(borderColor);
            pen.setWidth(2);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(hRect.adjusted(1, 1, -1, -1));
        }
    }

    void AsterSlider::mousePressEvent(QMouseEvent* event) {
        if (event->button() == Qt::LeftButton) {
            m_isDragging = true;
            animateHover(true); // Ensure visual feedback immediately
            int newVal = pixelPosToValue(orientation() == Qt::Horizontal ? event->pos().x() : event->pos().y());
            setValue(newVal);
            update();
            event->accept();
        } else {
            QAbstractSlider::mousePressEvent(event);
        }
    }

    void AsterSlider::mouseMoveEvent(QMouseEvent* event) {
        if (m_isDragging) {
            int newVal = pixelPosToValue(orientation() == Qt::Horizontal ? event->pos().x() : event->pos().y());
            setValue(newVal);
            update();
            event->accept();
        } else {
            QAbstractSlider::mouseMoveEvent(event);
        }
    }

    void AsterSlider::mouseReleaseEvent(QMouseEvent* event) {
        if (event->button() == Qt::LeftButton) {
            m_isDragging = false;
            // If we are still inside, keep hover effect, else fade out
            bool stillHovered = rect().contains(mapFromGlobal(QCursor::pos()));
            animateHover(stillHovered);
            update();
            event->accept();
        } else {
            QAbstractSlider::mouseReleaseEvent(event);
        }
    }

    void AsterSlider::enterEvent(QEnterEvent* event) {
        m_isHovered = true;
        animateHover(true);
        QAbstractSlider::enterEvent(event);
    }

    void AsterSlider::leaveEvent(QEvent* event) {
        m_isHovered = false;
        if (!m_isDragging) {
            animateHover(false);
        }
        QAbstractSlider::leaveEvent(event);
    }

    void AsterSlider::animateHover(bool hovered) {
        m_hoverAnim->stop();
        m_hoverAnim->setStartValue(m_handleBorderOpacity);
        m_hoverAnim->setEndValue(hovered ? 1.0 : 0.0);
        m_hoverAnim->setDuration(600); // Even slower for smoother effect
        m_hoverAnim->start();
    }

}
