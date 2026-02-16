#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QString>
#include <QIcon>

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterAlert : public QWidget {
        Q_OBJECT
        Q_PROPERTY(Type type READ type WRITE setType)
        Q_PROPERTY(QString title READ title WRITE setTitle)
        Q_PROPERTY(QString description READ description WRITE setDescription)
        Q_PROPERTY(bool closable READ isClosable WRITE setClosable)
        Q_PROPERTY(bool showIcon READ showIcon WRITE setShowIcon)

    public:
        enum class Type {
            Success,
            Info,
            Warning,
            Error
        };
        Q_ENUM(Type)

        explicit AsterAlert(QWidget* parent = nullptr);
        explicit AsterAlert(const QString& title, Type type = Type::Info, QWidget* parent = nullptr);
        explicit AsterAlert(const QString& title, const QString& description, Type type = Type::Info, QWidget* parent = nullptr);
        ~AsterAlert() override;

        Type type() const;
        void setType(Type type);

        QString title() const;
        void setTitle(const QString& title);

        QString description() const;
        void setDescription(const QString& description);

        bool isClosable() const;
        void setClosable(bool closable);

        bool showIcon() const;
        void setShowIcon(bool show);

        void setCustomIcon(const QIcon& icon);

        QSize sizeHint() const override;

    signals:
        void closed();

    protected:
        void paintEvent(QPaintEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;

    private:
        void updateStyle();
        QColor getTypeColor() const;
        QColor getBackgroundColor() const;
        QColor getBorderColor() const;

        Type m_type = Type::Info;
        QString m_title;
        QString m_description;
        bool m_closable = false;
        bool m_showIcon = true; // Default consistent with Ant Design (usually defaults to true for alerts with type)
        QIcon m_customIcon;
        
        // Internal state
        bool m_hoverClose = false;
        QRect m_closeRect;
    };

}
