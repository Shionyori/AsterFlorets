#include "AsterSpin.h"
#include "AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <QFontMetrics>

namespace AsterFlorets {

    AsterSpin::AsterSpin(QWidget* parent)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, [this](){
            m_angle = (m_angle + 30) % 360;
            update();
        });
        updateTimer();
    }

    AsterSpin::~AsterSpin() = default;

    bool AsterSpin::isSpinning() const {
        return m_spinning;
    }

    void AsterSpin::setSpinning(bool spinning) {
        if (m_spinning != spinning) {
            m_spinning = spinning;
            updateTimer();
            update();
        }
    }

    QString AsterSpin::tip() const {
        return m_tip;
    }

    void AsterSpin::setTip(const QString& tip) {
        m_tip = tip;
        updateGeometry();
        update();
    }

    void AsterSpin::setSpinSize(Size size) {
        m_size = size;
        updateGeometry();
        update();
    }

    AsterSpin::Size AsterSpin::spinSize() const {
        return m_size;
    }

    void AsterSpin::updateTimer() {
        if (m_spinning && isVisible()) {
            if (!m_timer->isActive()) m_timer->start(80); // ~12fps
        } else {
            m_timer->stop();
        }
    }

    QSize AsterSpin::sizeHint() const {
        int d = 20; // Default icon diameter
        if (m_size == Size::Small) d = 14;
        else if (m_size == Size::Large) d = 32;

        int w = d;
        int h = d;

        if (!m_tip.isEmpty()) {
            QFontMetrics fm(font());
            int textW = fm.horizontalAdvance(m_tip);
            int textH = fm.height();
            w = qMax(w, textW);
            h += textH + 8; // gap
        }
        
        // Add some padding
        return QSize(w + 10, h + 10);
    }

    void AsterSpin::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        if (!m_spinning) return;

        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int d = 20;
        if (m_size == Size::Small) d = 14;
        else if (m_size == Size::Large) d = 32;

        // Center position
        int cx = width() / 2;
        int cy = height() / 2;
        
        // If text exists, shift circle up
        if (!m_tip.isEmpty()) {
            QFontMetrics fm(font());
            int textH = fm.height();
            cy -= (textH + 4) / 2;
        }

        auto theme = AsterTheme::instance();
        QColor color = theme->color(AsterTheme::ColorRole::Primary);

        // Draw dots (Ant Design style is 4 dots spinning)
        // Let's emulate the rotating dots
        p.translate(cx, cy);
        p.rotate(m_angle);

        int dotSize = d / 3.5; // Roughly
        int radius = d / 2 - dotSize / 2;

        for (int i = 0; i < 4; ++i) {
             p.setOpacity(0.4 + (i * 0.15)); // Gradient opacity
             p.setBrush(color);
             p.setPen(Qt::NoPen);
             p.drawEllipse(QPoint(radius, 0), dotSize / 2, dotSize / 2);
             p.rotate(90);
        }

        // Draw Tip
        if (!m_tip.isEmpty()) {
            p.resetTransform(); // Reset to verify coord
            p.setFont(font());
            p.setPen(color);
            
            // Re-calc text rect
            QFontMetrics fm(font());
            int textH = fm.height();
            int top = cy + d / 2 + 8;
            QRect textRect(0, top, width(), textH);
            p.drawText(textRect, Qt::AlignCenter, m_tip);
        }
    }

}
