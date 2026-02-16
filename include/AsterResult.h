#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QString>
#include <QIcon>

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
        void paintEvent(QPaintEvent* event) override;

    private:
        Status m_status = Status::Info;
        QString m_title;
        QString m_subTitle;
        QWidget* m_extra = nullptr;
        // Icons are resource based usually, we will draw placeholders
    };

}
