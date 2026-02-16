#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QString>
#include <QIcon>

// Forward declarations for Qt classes
class QLabel;
class QVBoxLayout;

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterResult : public QWidget {
        Q_OBJECT
        Q_PROPERTY(Status status READ status WRITE setStatus)
        Q_PROPERTY(QString title READ title WRITE setTitle)
        Q_PROPERTY(QString subTitle READ subTitle WRITE setSubTitle)

    public:
        enum class Status {
            Success,
            Error,
            Info,
            Warning,
            NotFound,
            Forbidden,
            ServerError
        };
        Q_ENUM(Status)

        explicit AsterResult(QWidget* parent = nullptr);
        ~AsterResult() override;

        void setStatus(Status status);
        Status status() const;

        void setTitle(const QString& title);
        QString title() const;

        void setSubTitle(const QString& subTitle);
        QString subTitle() const;

        void setExtra(QWidget* widget); // Add buttons/actions

    protected:
        // No longer override paintEvent, usage Layout
        // void paintEvent(QPaintEvent* event) override;

    private:
        Status m_status = Status::Info;
        
        // UI Components
        QWidget* m_iconWidget = nullptr;
        QLabel* m_titleLabel = nullptr;
        QLabel* m_subLabel = nullptr;
        QWidget* m_extraWidget = nullptr;
        QVBoxLayout* m_layout = nullptr;

        void updateIcon();
    };

}
