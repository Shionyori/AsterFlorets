#pragma once

#include "Global.h"
#include <QLineEdit>
#include <QIcon>
#include <QPropertyAnimation>

namespace AsterUI {

    class ASTERUI_EXPORT AsterTextInput : public QLineEdit {
        Q_OBJECT
        Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
        Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius)
        Q_PROPERTY(Status status READ status WRITE setStatus)

    public:
        enum class Status {
            Normal,
            Error,
            Warning
        };
        Q_ENUM(Status)

        explicit AsterTextInput(QWidget* parent = nullptr);
        explicit AsterTextInput(const QString& placeholder, QWidget* parent = nullptr);
        ~AsterTextInput() override;

        // 清除按钮
        bool isClearable() const;
        void setClearable(bool clearable);

        // 属性
        int borderRadius() const { return m_borderRadius; }
        void setBorderRadius(int radius);

        Status status() const { return m_status; }
        void setStatus(Status status);

        // 动画属性
        QColor borderColor() const { return m_borderColor; }
        void setBorderColor(const QColor& color);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void focusInEvent(QFocusEvent* event) override;
        void focusOutEvent(QFocusEvent* event) override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;

    private:
        void init();
        void updateClearButtonPosition();
        void animateBorder();

    private:
        bool m_clearable;
        QAction* m_clearAction;
        
        QColor m_borderColor;
        QPropertyAnimation* m_borderAnimation;

        int m_borderRadius = -1;
        Status m_status = Status::Normal;
        bool m_isHovered = false;
    };

}
