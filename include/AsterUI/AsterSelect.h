#pragma once

#include "Global.h"
#include <QComboBox>
#include <QPropertyAnimation>

namespace AsterUI {

    class ASTERUI_EXPORT AsterSelect : public QComboBox {
        Q_OBJECT
        Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)

    public:
        explicit AsterSelect(QWidget* parent = nullptr);
        ~AsterSelect() override;

        QColor borderColor() const { return m_borderColor; }
        void setBorderColor(const QColor& color);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void focusInEvent(QFocusEvent* event) override;
        void focusOutEvent(QFocusEvent* event) override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;

    private:
        void init();
        void animateBorder(bool focused);

    private:
        QColor m_borderColor;
        QPropertyAnimation* m_borderAnimation;
        bool m_isHovered = false;
    };

}
