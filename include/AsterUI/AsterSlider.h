#pragma once

#include "Global.h"
#include <QAbstractSlider>
#include <QColor>

namespace AsterUI {

    class ASTERUI_EXPORT AsterSlider : public QAbstractSlider {
        Q_OBJECT
        Q_PROPERTY(QColor activeColor READ activeColor WRITE setActiveColor)
        Q_PROPERTY(QColor inactiveColor READ inactiveColor WRITE setInactiveColor)
        Q_PROPERTY(QColor handleColor READ handleColor WRITE setHandleColor)

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

    private:
        QColor m_activeColor;
        QColor m_inactiveColor;
        QColor m_handleColor;
        
        bool m_isHovered = false;
        bool m_isDragging = false;
        int m_handleRadius = 7;
    };

}
