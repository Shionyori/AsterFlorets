#include "AsterUI/AsterScrollArea.h"
#include "AsterUI/AsterTheme.h"
#include <QScrollBar>

namespace AsterUI {

    AsterScrollArea::AsterScrollArea(QWidget* parent)
        : QScrollArea(parent)
    {
        setFrameShape(QFrame::NoFrame);
        setWidgetResizable(true);
        setAttribute(Qt::WA_TranslucentBackground);
        viewport()->setAttribute(Qt::WA_TranslucentBackground);

        // Dynamic Stylesheet based on Theme
        auto theme = AsterTheme::instance();
        
        // 1. Idle State: Use Border color (Subtle Grey)
        QColor cIdle = theme->color(AsterTheme::ColorRole::Border);
        
        // 2. Hover State: Slightly darker than Idle for natural transition
        QColor cHover = cIdle.darker(120); // 20% darker

        // 3. Pressed State: Use Primary Color (Shion Violet)
        QColor cPressed = theme->color(AsterTheme::ColorRole::Primary);

        QString sIdle = cIdle.name(QColor::HexArgb);
        QString sHover = cHover.name(QColor::HexArgb);
        QString sPressed = cPressed.name(QColor::HexArgb);

        // Simple scrollbar styling
        QString style = QString(R"(
            QScrollArea {
                background: transparent;
                border: none;
            }
            
            /* Vertical ScrollBar */
            QScrollBar:vertical {
                border: none;
                background: transparent;
                width: 10px;
                margin: 0px;
            }
            QScrollBar::handle:vertical {
                background: %1;
                min-height: 30px;
                border-radius: 3px; /* Exact half of (10px - 2px*2) = 3px */
                margin: 2px; /* Creates a floating effect */
            }
            QScrollBar::handle:vertical:hover {
                background: %2;
            }
            QScrollBar::handle:vertical:pressed {
                background: %3;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }
            QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
                background: none;
            }

            /* Horizontal ScrollBar */
            QScrollBar:horizontal {
                border: none;
                background: transparent;
                height: 10px;
                margin: 0px;
            }
            QScrollBar::handle:horizontal {
                background: %1;
                min-width: 30px;
                border-radius: 3px;
                margin: 2px;
            }
            QScrollBar::handle:horizontal:hover {
                background: %2;
            }
            QScrollBar::handle:horizontal:pressed {
                background: %3;
            }
            QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
                width: 0px;
            }
            QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
                background: none;
            }
        )").arg(sIdle, sHover, sPressed);

        setStyleSheet(style);
    }

    AsterScrollArea::~AsterScrollArea() = default;

    void AsterScrollArea::paintEvent(QPaintEvent* event) {
        QScrollArea::paintEvent(event);
    }

}
