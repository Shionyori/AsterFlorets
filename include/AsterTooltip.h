#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QEvent>

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterTooltip : public QObject {
        Q_OBJECT

    public:
        // Helper to install tooltip on a widget
        static void install(QWidget* widget, const QString& text);
        
        explicit AsterTooltip(QWidget* parent, const QString& text);
        ~AsterTooltip() override;

        bool eventFilter(QObject* watched, QEvent* event) override;

    private:
        void showTooltip();
        void hideTooltip();
        void updatePosition();

        QWidget* m_target;
        QString m_text;
        QWidget* m_popup = nullptr; // The floating widget
        QLabel* m_label = nullptr;
    };

}
