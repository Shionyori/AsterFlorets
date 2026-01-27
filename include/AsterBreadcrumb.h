#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QStringList>
#include <functional>

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterBreadcrumb : public QWidget {
        Q_OBJECT

    public:
        explicit AsterBreadcrumb(QWidget* parent = nullptr);
        ~AsterBreadcrumb() override;

        // Add a breadcrumb item
        // onClick is optional callback
        void addItem(const QString& text, std::function<void()> onClick = nullptr);
        
        // Set custom separator (default: "/")
        void setSeparator(const QString& separator); 

        int count() const;
        void clear();

    Q_SIGNALS:
        void itemClicked(int index, const QString& text);

    protected:
        bool eventFilter(QObject* watched, QEvent* event) override;

    private:
        void rebuildLayout();

        struct BreadcrumbItem {
            QString text;
            std::function<void()> onClick;
        };
        
        QList<BreadcrumbItem> m_items;
        QString m_separator = "/";
    };

}
