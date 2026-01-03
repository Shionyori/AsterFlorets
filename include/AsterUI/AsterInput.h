#pragma once

#include "Global.h"
#include <QLineEdit>
#include <QIcon>
#include <QPropertyAnimation>

namespace AsterUI {

    class ASTERUI_EXPORT AsterInput : public QLineEdit {
        Q_OBJECT
        Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)

    public:
        explicit AsterInput(QWidget* parent = nullptr);
        explicit AsterInput(const QString& placeholder, QWidget* parent = nullptr);
        ~AsterInput() override;

        // 清除按钮
        bool isClearable() const;
        void setClearable(bool clearable);

        // 动画属性
        QColor borderColor() const { return m_borderColor; }
        void setBorderColor(const QColor& color);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void focusInEvent(QFocusEvent* event) override;
        void focusOutEvent(QFocusEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;

    private:
        void init();
        void updateClearButtonPosition();
        void animateBorder(bool focused);

    private:
        bool m_clearable;
        QAction* m_clearAction;
        
        QColor m_borderColor;
        QPropertyAnimation* m_borderAnimation;
    };

}
