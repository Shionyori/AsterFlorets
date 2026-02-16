#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QList>

namespace AsterFlorets {

    class AsterCollapsePanel;

    class AsterFlorets_EXPORT AsterCollapse : public QWidget {
        Q_OBJECT
        Q_PROPERTY(bool accordion READ isAccordion WRITE setAccordion)

    public:
        explicit AsterCollapse(QWidget* parent = nullptr);
        ~AsterCollapse() override;

        void addPanel(const QString& title, QWidget* content);
        void removePanel(int index);
        
        void setAccordion(bool accordion);
        bool isAccordion() const;

    private slots:
        void onPanelToggled(bool expanded);

    private:
        QVBoxLayout* m_layout;
        QList<AsterCollapsePanel*> m_panels;
        bool m_accordion = false;
    };

}
