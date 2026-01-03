#pragma once

#include "Global.h"
#include <QWidget>
#include <QColor>

namespace AsterUI {

    class ASTERUI_EXPORT AsterTag : public QWidget {
        Q_OBJECT
        Q_PROPERTY(QString text READ text WRITE setText)
        Q_PROPERTY(bool closable READ isClosable WRITE setClosable)

    public:
        enum class Type {
            Default,
            Primary,
            Success,
            Warning,
            Error
        };
        Q_ENUM(Type)

        explicit AsterTag(const QString& text, QWidget* parent = nullptr);
        explicit AsterTag(QWidget* parent = nullptr);
        ~AsterTag() override;

        QString text() const { return m_text; }
        void setText(const QString& text);

        Type type() const { return m_type; }
        void setType(Type type);

        bool isClosable() const { return m_closable; }
        void setClosable(bool closable);

    signals:
        void closed();
        void clicked();

    protected:
        void paintEvent(QPaintEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;
        QSize sizeHint() const override;

    private:
        void init();
        void updateStyle();
        QRect closeButtonRect() const;

    private:
        QString m_text;
        Type m_type = Type::Default;
        bool m_closable = false;
        bool m_isHovered = false;
        bool m_isCloseHovered = false;

        QColor m_bgColor;
        QColor m_borderColor;
        QColor m_textColor;
    };

}
