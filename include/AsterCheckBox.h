#pragma once

#include "AsterGlobal.h"
#include <QCheckBox>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterCheckBox : public QCheckBox {
        Q_OBJECT
        Q_PROPERTY(double checkScale READ checkScale WRITE setCheckScale)
        Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)

    public:
        explicit AsterCheckBox(const QString& text, QWidget* parent = nullptr);
        explicit AsterCheckBox(QWidget* parent = nullptr);
        ~AsterCheckBox() override;

        // 动画属性
        double checkScale() const { return m_checkScale; }
        void setCheckScale(double scale);

        QColor borderColor() const { return m_borderColor; }
        void setBorderColor(const QColor& color);

        QSize sizeHint() const override;
        bool hitButton(const QPoint &pos) const override;

    protected:
        void paintEvent(QPaintEvent* event) override;
        void nextCheckState() override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;

    private:
        void init();
        void startAnimation();
        void updateColors();

    private:
        double m_checkScale;
        QColor m_borderColor;
        
        QParallelAnimationGroup* m_animGroup;
        bool m_isHovered;
    };

}
