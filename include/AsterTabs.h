#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QIcon>
#include <QList>

class QStackedWidget;
class QScrollArea;

namespace AsterUI {

    // Forward declaration of internal tab bar
    class AsterTabBar;

    class ASTERUI_EXPORT AsterTabs : public QWidget {
        Q_OBJECT
        Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)

    public:
        explicit AsterTabs(QWidget* parent = nullptr);
        ~AsterTabs() override;

        // Add a tab with a page widget and label
        int addTab(QWidget* page, const QString& label);
        int addTab(QWidget* page, const QIcon& icon, const QString& label);
        
        // Remove tab at index
        void removeTab(int index);

        // Accessors
        int count() const;
        int currentIndex() const;
        QWidget* currentWidget() const;
        QWidget* widget(int index) const;
        
        QString tabText(int index) const;
        void setTabText(int index, const QString& text);

    public Q_SLOTS:
        void setCurrentIndex(int index);
        void setCurrentWidget(QWidget* widget);

    Q_SIGNALS:
        void currentChanged(int index);

    private:
        AsterTabBar* m_tabBar;
        QStackedWidget* m_stack;
    };

}
