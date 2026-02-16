#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QFrame>

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterPopover : public QWidget {
        Q_OBJECT
        Q_PROPERTY(QString title READ title WRITE setTitle)
        Q_PROPERTY(QWidget* content READ content WRITE setContent)
        Q_PROPERTY(bool visible READ isVisible WRITE setVisible)

    public:
        // Helper to install on a widget efficiently?
        // Or usage: popover = new AsterPopover(); popover->setTarget(btn);
        
        explicit AsterPopover(QWidget* parent = nullptr);
        ~AsterPopover() override;

        void setTarget(QWidget* target);
        QWidget* target() const;

        void setTitle(const QString& title);
        QString title() const;

        void setContent(QWidget* content); // Takes ownership of content widget
        QWidget* content() const;

        void showPopover();
        void hidePopover();
        void togglePopover();

    protected:
        bool eventFilter(QObject* watched, QEvent* event) override;
        void paintEvent(QPaintEvent* event) override;
        // void showEvent(QShowEvent* event) override;

    private:
        void updatePosition();

        QWidget* m_target = nullptr;
        QString m_title;
        QWidget* m_content = nullptr;
        QWidget* m_container; // Internal layout container
        
        // We might need a separate popup window if we want it to float over everything
        // For simplicity, let's make AsterPopover ITSELF the popup window
    };

}
