#pragma once

#include "AsterGlobal.h"
#include <QAbstractSlider>
#include <QColor>
#include <QPropertyAnimation>

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterSlider : public QAbstractSlider {
        Q_OBJECT
        Q_PROPERTY(QColor activeColor READ activeColor WRITE setActiveColor)
        Q_PROPERTY(QColor inactiveColor READ inactiveColor WRITE setInactiveColor)
        Q_PROPERTY(QColor handleColor READ handleColor WRITE setHandleColor)
        Q_PROPERTY(double handleBorderOpacity READ handleBorderOpacity WRITE setHandleBorderOpacity)

    public:
        explicit AsterSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
        explicit AsterSlider(QWidget* parent = nullptr);
        ~AsterSlider() override;

        QColor activeColor() const { return m_activeColor; }
        void setActiveColor(const QColor& color);

        QColor inactiveColor() const { return m_inactiveColor; }
        void setInactiveColor(const QColor& color);

        QColor handleColor() const { return m_handleColor; }
        void setHandleColor(const QColor& color);

        double handleBorderOpacity() const { return m_handleBorderOpacity; }
        void setHandleBorderOpacity(double opacity) {
            if (m_handleBorderOpacity != opacity) {
                m_handleBorderOpacity = opacity;
                update();
            }
        }

    protected:
        void paintEvent(QPaintEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;
        QSize sizeHint() const override;

    private:
        void init();
        int pixelPosToValue(int pos) const;
        int valueToPixelPos(int val) const;
        QRect handleRect() const;
        void animateHover(bool hovered);

    private:
        QColor m_activeColor;
        QColor m_inactiveColor;
        QColor m_handleColor;
        
        bool m_isHovered = false;
        bool m_isDragging = false;
        int m_handleRadius = 7;

        double m_handleBorderOpacity = 0.0;
        QPropertyAnimation* m_hoverAnim = nullptr;
    };

}
