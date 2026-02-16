#include "AsterDropdown.h"
#include "AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QMenu>
#include <QVariantAnimation>

namespace AsterFlorets {

    AsterDropdown::AsterDropdown(const QString& text, QWidget* parent)
        : AsterButton(text, parent)
    {
        // Dropdown usually looks like a button or a link
        // By default, let's treat it as a button that triggers a menu
        m_menu = new QMenu(this);
        // Style the menu roughly to match theme (Qt styles menus via QStyle or stylesheet generally)
        // For now, default OS menu style or minimal stylesheet
        m_menu->setStyleSheet(QString(
            "QMenu { background-color: %1; border: 1px solid %2; padding: 4px; }"
            "QMenu::item { padding: 5px 20px; color: %3; }"
            "QMenu::item:selected { background-color: %4; color: %1; }"
        ).arg(AsterTheme::instance()->color(AsterTheme::ColorRole::Background).name())
         .arg(AsterTheme::instance()->color(AsterTheme::ColorRole::Border).name())
         .arg(AsterTheme::instance()->color(AsterTheme::ColorRole::Text).name())
         .arg(AsterTheme::instance()->color(AsterTheme::ColorRole::Primary).name())
        );

        // Manual menu handling not fully implemented in AsterButton base
        // But we connect clicked to show menu
        connect(this, &AsterDropdown::clicked, this, [this](){
            if (m_menu) {
                // Show menu at bottom left of button
                QPoint p = mapToGlobal(rect().bottomLeft());
                m_menu->exec(p);
            }
        });
    }

    AsterDropdown::~AsterDropdown() = default; // m_menu is child

    void AsterDropdown::addAction(const QString& text, const std::function<void()>& handler) {
        QAction* act = m_menu->addAction(text);
        if (handler) {
            connect(act, &QAction::triggered, handler);
        }
    }

    void AsterDropdown::setMenu(QMenu* menu) {
        if (m_menu && m_menu->parent() == this) {
            delete m_menu;
        }
        m_menu = menu;
        // logic to show menu is in constructor lambda
    }

    QMenu* AsterDropdown::menu() const {
        return m_menu;
    }

    void AsterDropdown::paintEvent(QPaintEvent* event) {
        AsterButton::paintEvent(event);
        
        // Draw small down arrow if not present?
        // Actually typically text + icon. 
        // Let's assume user sets text. We just paint an arrow at the right if there's room.
        
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        
        // Simple arrow
        int arrowSize = 8;
        int x = width() - 16;
        int y = height() / 2 - arrowSize / 4;
        
        QPainterPath path;
        path.moveTo(x, y);
        path.lineTo(x + arrowSize / 2, y + arrowSize / 2);
        path.lineTo(x + arrowSize, y);
        
        p.setPen(QPen(AsterTheme::instance()->color(AsterTheme::ColorRole::Text), 1.5));
        p.setBrush(Qt::NoBrush);
        p.drawPath(path);
    }

}
