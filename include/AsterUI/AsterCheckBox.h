#pragma once

#include "Global.h"
#include <QCheckBox>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

namespace AsterUI {

    class ASTERUI_EXPORT AsterCheckBox : public QCheckBox {
        Q_OBJECT
        Q_PROPERTY(QColor indicatorColor READ indicatorColor WRITE setIndicatorColor)
        Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)

    public:
        explicit AsterCheckBox(const QString& text, QWidget* parent = nullptr);
        explicit AsterCheckBox(QWidget* parent = nullptr);
        ~AsterCheckBox() override;

        // 动画属性
        QColor indicatorColor() const { return m_indicatorColor; }
        void setIndicatorColor(const QColor& color);

        QColor borderColor() const { return m_borderColor; }
        void setBorderColor(const QColor& color);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void checkStateSet() override;
        void nextCheckState() override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;

    private:
        void init();
        void startAnimation(bool checked);
        void updateColors();

    private:
        QColor m_indicatorColor;
        QColor m_borderColor;
        
        QParallelAnimationGroup* m_animGroup;
        bool m_isHovered;
    };

}
