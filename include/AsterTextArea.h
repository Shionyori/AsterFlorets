#pragma once

#include "AsterGlobal.h"
#include <QFrame>
#include <QPlainTextEdit>
#include <QPropertyAnimation>

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterTextArea : public QFrame {
        Q_OBJECT
        Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
        Q_PROPERTY(Status status READ status WRITE setStatus)

    public:
        enum class Status {
            Normal,
            Error,
            Warning
        };
        Q_ENUM(Status)

        explicit AsterTextArea(QWidget* parent = nullptr);
        explicit AsterTextArea(const QString& placeholder, QWidget* parent = nullptr);
        ~AsterTextArea() override;

        // Proxy methods for QPlainTextEdit
        void setPlaceholderText(const QString& text);
        QString placeholderText() const;
        void setPlainText(const QString& text);
        QString toPlainText() const;
        QPlainTextEdit* editor() const { return m_editor; }

        Status status() const { return m_status; }
        void setStatus(Status status);

        QColor borderColor() const { return m_borderColor; }
        void setBorderColor(const QColor& color);

    protected:
        void paintEvent(QPaintEvent* event) override;
        bool eventFilter(QObject* watched, QEvent* event) override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;

    private:
        void init();
        void animateBorder();

        QPlainTextEdit* m_editor = nullptr;
        QColor m_borderColor;
        Status m_status = Status::Normal;
        QPropertyAnimation* m_borderAnimation = nullptr;
        bool m_isHovered = false;
        bool m_isFocused = false;
    };

}
