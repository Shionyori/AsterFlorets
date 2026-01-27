#include "AsterScrollArea.h"
#include "AsterTheme.h"
#include <QScrollBar>

namespace AsterUI {

    AsterScrollArea::AsterScrollArea(QWidget* parent)
        : QScrollArea(parent)
    {
        // Ensure styling works correctly (transparency, etc.)
        setAttribute(Qt::WA_StyledBackground, true);
        
        setFrameShape(QFrame::NoFrame);
        setWidgetResizable(true);
        setAttribute(Qt::WA_TranslucentBackground);
        viewport()->setAttribute(Qt::WA_TranslucentBackground);

        // Internal Container (Defaults to Vertical Space)
        m_container = new AsterSpace(Qt::Vertical, this);
        // Important: Ensure container allows being resized by ScrollArea
        m_container->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); 
        
        // // Fix: Make container transparent so it doesn't paint a black/default background over the parent content
        m_container->setAttribute(Qt::WA_TranslucentBackground);
        m_container->setAutoFillBackground(false);
        // Explicitly set transparent background to be safe against style inheritance
        m_container->setStyleSheet("background: transparent;");

        setWidget(m_container);

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

    AsterScrollArea::~AsterScrollArea() {
         // m_container is managed by QScrollArea via setWidget
    }

    void AsterScrollArea::addWidget(QWidget* widget) {
        if (m_container) m_container->addWidget(widget);
    }

    void AsterScrollArea::addStretch(int stretch) {
        if (m_container) m_container->addStretch(stretch);
    }

    void AsterScrollArea::setSpacing(int spacing) {
         if (m_container) m_container->setSize(spacing);
    }

    void AsterScrollArea::setDirection(Qt::Orientation direction) {
        if (m_container) m_container->setDirection(direction);
    }

    void AsterScrollArea::setContentsMargins(int left, int top, int right, int bottom) {
        if (m_container) m_container->layout()->setContentsMargins(left, top, right, bottom);
    }
    
    void AsterScrollArea::setContentsMargins(const QMargins &margins) {
         if (m_container) m_container->layout()->setContentsMargins(margins);
    }

    AsterSpace* AsterScrollArea::container() const {
        return m_container;
    }

    void AsterScrollArea::paintEvent(QPaintEvent* event) {
        QScrollArea::paintEvent(event);
    }

}
