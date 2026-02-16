#include "AsterPopover.h"
#include "AsterTheme.h"
#include "AsterTitle.h"
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QEvent>
#include <QApplication>
#include <QScreen>
#include <QDebug>

namespace AsterFlorets {

    AsterPopover::AsterPopover(QWidget* parent)
        : QWidget(parent)
    {
        // Popover needs to be a popup window to float above others
        // Qt::Popup handles closing when clicking outside
        setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
        
        // Container holds the actual content
        m_container = new QWidget(this);
        m_container->setObjectName("AsterPopoverContainer"); // Ensure correct styling
        
        // Fix: Explicitly set background color for container
        QPalette pal = m_container->palette();
        pal.setColor(QPalette::Window, AsterTheme::instance()->color(AsterTheme::ColorRole::Surface));
        m_container->setPalette(pal);
        m_container->setAutoFillBackground(true);
        
        auto* lay = new QVBoxLayout(m_container);
        lay->setContentsMargins(12, 12, 12, 12);
        lay->setSpacing(8);
    }

    AsterPopover::~AsterPopover() = default;

    void AsterPopover::setTarget(QWidget* target) {
        if (m_target) {
            m_target->removeEventFilter(this);
        }
        m_target = target;
        if (m_target) {
            m_target->installEventFilter(this);
        }
    }

    QWidget* AsterPopover::target() const {
        return m_target;
    }

    void AsterPopover::setTitle(const QString& title) {
        m_title = title;
        // If we had a title widget already created, update it
        // But we create UI dynamically in setContent usually or init.
        // Let's enforce structure: Title (optional) + Content
    }
    
    QString AsterPopover::title() const {
        return m_title; 
    }

    void AsterPopover::setContent(QWidget* content) {
        // Clear layout
        QLayout* lay = m_container->layout();
        QLayoutItem* item;
        while ((item = lay->takeAt(0)) != nullptr) {
            delete item->widget(); // Deletes old content/titles
            delete item;
        }

        // Add Title if exists
        if (!m_title.isEmpty()) {
            AsterTitle* t = new AsterTitle(m_title, 5); // h5
            lay->addWidget(t);
            
            // Divider?
            QFrame* line = new QFrame();
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            line->setStyleSheet("background-color: #f0f0f0; border: none; max-height: 1px;");
            lay->addWidget(line);
        }

        m_content = content;
        if (m_content) {
            lay->addWidget(m_content);
            m_content->show();
        }
    }

    QWidget* AsterPopover::content() const {
        return m_content;
    }

    void AsterPopover::showPopover() {
        if (!m_target) return;
        
        // Re-construct layout if needed (simplified)
        // Usually updatePosition is called
        updatePosition();
        show();
    }

    void AsterPopover::hidePopover() {
        hide();
    }

    void AsterPopover::togglePopover() {
        if (isVisible()) hidePopover();
        else showPopover();
    }

    void AsterPopover::updatePosition() {
        if (!m_target) return;
        
        // Resize to fit content
        m_container->adjustSize();
        // Add padding for shadow/arrow
        int padding = 4;
        resize(m_container->size() + QSize(padding*2, padding*2));
        m_container->move(padding, padding);

        QPoint p = m_target->mapToGlobal(QPoint(0, 0));
        // simple top placement
        int x = p.x() + m_target->width()/2 - width()/2;
        int y = p.y() - height(); 
        
        // Simple bounds check
        if (y < 0) {
            y = p.y() + m_target->height(); // move to bottom
        }

        move(x, y);
    }
    
    bool AsterPopover::eventFilter(QObject* watched, QEvent* event) {
        if (watched == m_target) {
            if (event->type() == QEvent::MouseButtonPress) {
                togglePopover();
            }
        }
        return QWidget::eventFilter(watched, event);
    }

    void AsterPopover::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        
        // Draw shadow/border
        QRect r = m_container->geometry();
        
        p.setBrush(AsterTheme::instance()->color(AsterTheme::ColorRole::Background));
        p.setPen(QPen(AsterTheme::instance()->color(AsterTheme::ColorRole::Border), 1));
        
        p.drawRoundedRect(r, 4, 4);
    }

}
