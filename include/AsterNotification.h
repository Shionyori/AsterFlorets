#pragma once

#include "Global.h"
#include <QObject>
#include <QString>
#include <QWidget>

namespace AsterUI {

    class ASTERUI_EXPORT AsterNotification : public QObject {
        Q_OBJECT
    public:
        enum class Type {
            Info,
            Success,
            Warning,
            Error
        };
        Q_ENUM(Type)

        /**
         * @brief Show a notification card in the top-right corner.
         * @param title The title of the notification.
         * @param message The detailed message content.
         * @param type The type (affects icon and color).
         * @param duration Duration in ms. 0 to keep open. Default 4500ms.
         * @param parent Optional parent widget/window.
         */
        static void show(const QString& title, const QString& message, Type type = Type::Info, int duration = 4500, QWidget* parent = nullptr);

        static void info(const QString& title, const QString& message, int duration = 4500, QWidget* parent = nullptr);
        static void success(const QString& title, const QString& message, int duration = 4500, QWidget* parent = nullptr);
        static void warning(const QString& title, const QString& message, int duration = 4500, QWidget* parent = nullptr);
        static void error(const QString& title, const QString& message, int duration = 4500, QWidget* parent = nullptr);
    };

}
