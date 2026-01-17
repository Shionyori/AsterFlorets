#include "AsterUI/AsterBadge.h"
#include "AsterUI/AsterTheme.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QEvent>
#include <QResizeEvent>
#include <QFontMetrics>
#include <QDebug>

namespace AsterUI {

    // Internal helper widget for drawing the badge
    class BadgeIndicator : public QWidget {
    public:
        explicit BadgeIndicator(AsterBadge* parent) : QWidget(parent), m_badge(parent) {
            setAttribute(Qt::WA_TransparentForMouseEvents);
            setAttribute(Qt::WA_NoSystemBackground); 
        }

    protected:
        void paintEvent(QPaintEvent*) override {
            if (!m_badge) return;
            
            int count = m_badge->count();
            bool showZero = m_badge->showZero();
            bool dot = m_badge->isDot();
            int maxCount = m_badge->maxCount();

            if (count == 0 && !showZero && !dot) return;

            QPainter p(this);
            p.setRenderHint(QPainter::Antialiasing);

            QColor bgColor = m_badge->color();
            if (!bgColor.isValid()) {
                bgColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Error);
            }

            if (dot) {
                p.setBrush(bgColor);
                p.setPen(Qt::NoPen);
                QRectF r = rect();
                // Draw 6px dot centered
                p.drawEllipse(r.center(), 3, 3);
                return;
            }

            QString text = QString::number(count);
            if (count > maxCount) {
                text = QString::number(maxCount) + "+";
            }

            p.setBrush(bgColor);
            p.setPen(Qt::NoPen);
            p.drawRoundedRect(rect(), rect().height() / 2.0, rect().height() / 2.0);

            p.setPen(Qt::white);
            QFont f = p.font();
            f.setPixelSize(12); // Small font
            // f.setBold(true);
            p.setFont(f);
            p.drawText(rect(), Qt::AlignCenter, text);
        }

    private:
        AsterBadge* m_badge;
    };

    // ========================================================================
    // AsterBadge
    // ========================================================================

    AsterBadge::AsterBadge(QWidget* parent)
        : QWidget(parent)
    {
        // Container setup
        // Use a layout to position content with margins for badge overhang
        auto* l = new QVBoxLayout(this);
        // Top and Right margins accommodate the badge. 
        // Increased Right margin to 25 to accommodate "99+" badge width without clipping.
        l->setContentsMargins(0, 10, 25, 0); 
        l->setSpacing(0);
        
        setAttribute(Qt::WA_TranslucentBackground);
    }

    AsterBadge::AsterBadge(QWidget* contentWidget, QWidget* parent)
        : AsterBadge(parent)
    {
        setContentWidget(contentWidget);
    }

    AsterBadge::~AsterBadge() = default;

    void AsterBadge::setContentWidget(QWidget* widget)
    {
        if (m_contentWidget) {
            m_contentWidget->removeEventFilter(this);
            delete m_contentWidget;
        }
        m_contentWidget = widget;
        if (m_contentWidget) {
            layout()->addWidget(m_contentWidget);
            m_contentWidget->installEventFilter(this);
        }
        updateSize();
    }

    QWidget* AsterBadge::contentWidget() const {
        return m_contentWidget;
    }

    int AsterBadge::count() const {
        return m_count;
    }

    void AsterBadge::setCount(int count) {
        if (m_count != count) {
            m_count = count;
            updateSize();
        }
    }

    bool AsterBadge::showZero() const {
        return m_showZero;
    }

    void AsterBadge::setShowZero(bool show) {
        if (m_showZero != show) {
            m_showZero = show;
            updateSize();
        }
    }

    int AsterBadge::maxCount() const {
        return m_maxCount;
    }

    void AsterBadge::setMaxCount(int max) {
        if (m_maxCount != max) {
            m_maxCount = max;
            updateSize();
        }
    }

    bool AsterBadge::isDot() const {
        return m_dot;
    }

    void AsterBadge::setDot(bool dot) {
        if (m_dot != dot) {
            m_dot = dot;
            updateSize();
        }
    }

    QColor AsterBadge::color() const {
        return m_color;
    }

    void AsterBadge::setColor(const QColor& color) {
        if (m_color != color) {
            m_color = color;
            if(m_indicator) m_indicator->update();
        }
    }

    void AsterBadge::paintEvent(QPaintEvent* event)
    {
        QWidget::paintEvent(event);
    }

    bool AsterBadge::eventFilter(QObject* watched, QEvent* event)
    {
        if (watched == m_contentWidget) {
             // If content resizes, we might need to adjust indicator?
             // Layout handles content position, updateSize handles badge position.
             if (event->type() == QEvent::Resize || event->type() == QEvent::Move) {
                 updateSize();
             }
        }
        return QWidget::eventFilter(watched, event);
    }
    
    void AsterBadge::resizeEvent(QResizeEvent* event) {
        updateSize();
        QWidget::resizeEvent(event);
    }
    
    void AsterBadge::updateSize()
    {
        if (!m_indicator) {
            m_indicator = new BadgeIndicator(this);
        }
        
        QWidget* indicator = m_indicator;

        bool visible = (m_count > 0 || m_showZero || m_dot);
        if (!visible) {
            indicator->hide();
            return;
        }
        
        indicator->show();
        indicator->raise();

        // Calculate size and position
        int w, h;
        if (m_dot) {
            w = h = 10; // 10x10 rect for 6px dot
        } else {
            QString text = QString::number(m_count);
            if (m_count > m_maxCount) text = QString::number(m_maxCount) + "+";
            
            QFont f;
            f.setPixelSize(12);
            QFontMetrics fm(f);
            int padding = 6;
            int textWidth = fm.horizontalAdvance(text);
            h = 20; 
            w = qMax(h, textWidth + padding);
        }
        
        indicator->resize(w, h); // This triggers paintEvent on indicator
        
        // Position relative to Content Top-Right
        QRect cRect = m_contentWidget ? m_contentWidget->geometry() : rect();
        QPoint tr = cRect.topRight();
        
        // Center of badge on the corner
        // x = tr.x - w/2
        // y = tr.y - h/2
        indicator->move(tr.x() - w/2 + 3, tr.y() - h/2 + 3);
        
        indicator->update();
    }
}
