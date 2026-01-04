#pragma once

#include "Global.h"
#include <QRadioButton>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

namespace AsterUI {

    class ASTERUI_EXPORT AsterRadio : public QRadioButton {
        Q_OBJECT
        Q_PROPERTY(double dotScale READ dotScale WRITE setDotScale)
        Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)

    public:
        explicit AsterRadio(const QString& text, QWidget* parent = nullptr);
        explicit AsterRadio(QWidget* parent = nullptr);
        ~AsterRadio() override;

        // 动画属性
        double dotScale() const { return m_dotScale; }
        void setDotScale(double scale);

        QColor borderColor() const { return m_borderColor; }
        void setBorderColor(const QColor& color);

        QSize sizeHint() const override;
        bool hitButton(const QPoint &pos) const override;

    protected:
        void paintEvent(QPaintEvent* event) override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;
        // void checkStateSet() override; // Removed, using toggled signal

    private:
        void init();
        void startAnimation();
        void updateColors();

    private:
        double m_dotScale;
        QColor m_borderColor;
        
        QParallelAnimationGroup* m_animGroup;
        bool m_isHovered;
    };

}
