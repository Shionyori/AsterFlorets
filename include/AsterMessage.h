#pragma once

#include "Global.h"
#include <QObject>
#include <QString>

class QWidget;

namespace AsterUI {

    class ASTERUI_EXPORT AsterMessage : public QObject {
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
         * @brief Show a message globally (or attached to parent)
         * @param content Message text
         * @param duration Display time in ms. 0 to keep open.
         * @param parent Specific parent. If null, tries to find active window.
         */
        static void show(Type type, const QString& content, int duration = 3000, QWidget* parent = nullptr);

        static void info(const QString& content, int duration = 3000, QWidget* parent = nullptr);
        static void success(const QString& content, int duration = 3000, QWidget* parent = nullptr);
        static void warning(const QString& content, int duration = 3000, QWidget* parent = nullptr);
        static void error(const QString& content, int duration = 3000, QWidget* parent = nullptr);
    };

}
