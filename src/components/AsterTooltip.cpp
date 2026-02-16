#include "AsterTooltip.h"
#include "AsterTheme.h"
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QApplication>
#include <QScreen>
#include <QTimer>

namespace AsterFlorets {

    // ========================================================================
    // TooltipPopup (Internal Widget)
    // ========================================================================
    class TooltipPopup : public QWidget {
    public:
        TooltipPopup(const QString& text, QWidget* parent = nullptr) : QWidget(parent) {
            setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
            setAttribute(Qt::WA_TranslucentBackground);
            setAttribute(Qt::WA_ShowWithoutActivating);

            auto layout = new QVBoxLayout(this);
            layout->setContentsMargins(1, 1, 1, 1); // Space for shadow? or minimal

            m_label = new QLabel(text);
            m_label->setStyleSheet("QLabel { color: white; background-color: rgba(0,0,0,0.75); border-radius: 4px; padding: 6px 8px; }");
            // Note: simple style for now. Ant Design uses black with arrow.

            layout->addWidget(m_label);
        }
    
    private:
        QLabel* m_label;
    };


    // ========================================================================
    // AsterTooltip
    // ========================================================================

    void AsterTooltip::install(QWidget* widget, const QString& text) {
        new AsterTooltip(widget, text);
    }

    AsterTooltip::AsterTooltip(QWidget* parent, const QString& text)
        : QObject(parent), m_target(parent), m_text(text)
    {
        if (m_target) {
            m_target->installEventFilter(this);
            m_target->setMouseTracking(true); // Needed sometimes
        }
    }

    AsterTooltip::~AsterTooltip() {
        if (m_popup) delete m_popup;
    }

    bool AsterTooltip::eventFilter(QObject* watched, QEvent* event) {
        if (watched == m_target) {
            if (event->type() == QEvent::Enter) {
                showTooltip();
            } else if (event->type() == QEvent::Leave) {
                hideTooltip();
            } else if (event->type() == QEvent::Hide) {
                hideTooltip();
            }
        }
        return QObject::eventFilter(watched, event);
    }

    void AsterTooltip::showTooltip() {
        if (!m_popup) {
            m_popup = new TooltipPopup(m_text);
        }
        updatePosition();
        m_popup->show();
    }

    void AsterTooltip::hideTooltip() {
        if (m_popup) {
            m_popup->hide();
            // Optional: delete later or reuse
        }
    }

    void AsterTooltip::updatePosition() {
        if (!m_popup || !m_target) return;
        
        QPoint p = m_target->mapToGlobal(QPoint(0, 0));
        
        // Default top
        int x = p.x() + (m_target->width() - m_popup->sizeHint().width()) / 2;
        int y = p.y() - m_popup->sizeHint().height() - 4; // gap

        // Basic screen boundary check skipped for brevity
        m_popup->move(x, y);
    }

}
