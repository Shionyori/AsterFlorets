#pragma once

#include "AsterGlobal.h"
#include "AsterButton.h"
#include <QWidget>
#include <QMenu>

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterDropdown : public AsterButton {
        Q_OBJECT
    public:
        explicit AsterDropdown(const QString& text = "Dropdown", QWidget* parent = nullptr);
        ~AsterDropdown() override;

        // Add menu items easily
        void addAction(const QString& text, const std::function<void()>& handler = nullptr);
        
        void setMenu(QMenu* menu);
        QMenu* menu() const;

    protected:
        // paint event is handled by AsterButton already
        // But we might want to draw a down arrow icon automatically?
        void paintEvent(QPaintEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;

    private:
        QMenu* m_menu = nullptr;
    };

}
