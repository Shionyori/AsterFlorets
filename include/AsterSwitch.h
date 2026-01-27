#pragma once

#include "AsterGlobal.h"
#include <QAbstractButton>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

namespace AsterUI {

    class ASTERUI_EXPORT AsterSwitch : public QAbstractButton {
        Q_OBJECT
        Q_PROPERTY(qreal handlePosition READ handlePosition WRITE setHandlePosition)
        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
        Q_PROPERTY(QColor checkedColor READ checkedColor WRITE setCheckedColor)
        Q_PROPERTY(QColor uncheckedColor READ uncheckedColor WRITE setUncheckedColor)
        Q_PROPERTY(double glowOpacity READ glowOpacity WRITE setGlowOpacity)

    public:
        explicit AsterSwitch(QWidget* parent = nullptr);
        ~AsterSwitch() override;

        QSize sizeHint() const override;
        QSize minimumSizeHint() const override;

        // 动画属性
        qreal handlePosition() const { return m_handlePosition; }
        void setHandlePosition(qreal pos);

        QColor backgroundColor() const { return m_backgroundColor; }
        void setBackgroundColor(const QColor& color);

        double glowOpacity() const { return m_glowOpacity; }
        void setGlowOpacity(double opacity);

        // 自定义颜色
        QColor checkedColor() const { return m_checkedColor; }
        void setCheckedColor(const QColor& color);

        QColor uncheckedColor() const { return m_uncheckedColor; }
        void setUncheckedColor(const QColor& color);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void checkStateSet() override;
        void nextCheckState() override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;

    private:
        void init();
        void startAnimation(bool checked);
        void animateGlow(bool hovered);

    private:
        qreal m_handlePosition; // 0.0 (Left/Off) to 1.0 (Right/On)
        QColor m_backgroundColor;
        double m_glowOpacity;
        
        QColor m_checkedColor;
        QColor m_uncheckedColor;
        
        QParallelAnimationGroup* m_animGroup;
        QPropertyAnimation* m_glowAnim;
        bool m_isHovered = false;
    };

}
