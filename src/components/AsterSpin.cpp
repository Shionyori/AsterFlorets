#include "AsterSpin.h"
#include "AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <QFontMetrics>
#include <QTimer>

namespace AsterFlorets {

    AsterSpin::AsterSpin(QWidget* parent)
        : QWidget(parent)
        , m_timer(new QTimer(this))
    {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        connect(m_timer, &QTimer::timeout, this, [this](){
            // Smoother rotation: 5 degrees per tick (assuming ~30ms interval) -> ~166 deg/sec
            m_angle = (m_angle + 10) % 360; 
            update();
        });
        // Don't call updateTimer here because isVisible returns false in constructor
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
            if (!m_timer->isActive()) m_timer->start(30); // ~30fps for smooth animation
        } else {
            m_timer->stop();
        }
    }

    void AsterSpin::showEvent(QShowEvent* event) {
        QWidget::showEvent(event);
        updateTimer();
    }

    void AsterSpin::hideEvent(QHideEvent* event) {
        QWidget::hideEvent(event);
        updateTimer();
    }

    QSize AsterSpin::sizeHint() const {
        int d = 24; // Default icon diameter (Ant Design 20px-32px usually)
        if (m_size == Size::Small) d = 16;
        else if (m_size == Size::Large) d = 32;

        int w = d;
        int h = d;

        if (!m_tip.isEmpty()) {
            QFontMetrics fm(font());
            int textW = fm.horizontalAdvance(m_tip);
            int textH = fm.height();
            w = qMax(d, textW);
            h += textH + 8; // gap
        }
        
        // Add some padding
        return QSize(w + 10, h + 10);
    }

    void AsterSpin::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        // Paint even if not spinning? Usually yes, static if valid. 
        // But usually Spin component hides content if spinning or shows spinner over content.
        // Here we just draw spinner.

        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int d = 24;
        if (m_size == Size::Small) d = 16;
        else if (m_size == Size::Large) d = 32;

        // Calculate layout
        QFontMetrics fm(font());
        int textH = (!m_tip.isEmpty()) ? (fm.height() + 8) : 0;
        int totalH = d + textH;
        
        // Center position in widget
        int cx = width() / 2;
        int cy = height() / 2;
        
        // If text exists, shift spinner up slightly so entire block is centered
        if (!m_tip.isEmpty()) {
            cy = (height() - totalH) / 2 + d / 2;
        }

        auto theme = AsterTheme::instance();
        QColor color = theme->color(AsterTheme::ColorRole::Primary);

        // Draw dots (Ant Design style is 4 dots spinning)
        // 4 dots, each separate 90 degrees.
        // Dot 1: Opacity 1.0 (Brightest)
        // Dot 2: Opacity 0.7
        // Dot 3: Opacity 0.4
        // Dot 4: Opacity 0.15 (Faintest)
        
        p.save();
        p.translate(cx, cy);
        p.rotate(m_angle); // Rotate the whole group

        // Radius for dot centers
        int dotRadius = d / 2; // Distance from center
        int dotDiameter = d * 0.4; // Size of each dot relative to overall spinner size

        // Draw 4 petals/dots
        for (int i = 0; i < 4; ++i) {
             // Calculate opacity: 1.0 -> 0.7 -> 0.4 -> 0.15
             // i=0 (0 deg): 1.0
             // i=1 (90 deg): 0.7 
             // ...
             qreal opacity = 1.0 - (i * 0.25);
             if (opacity < 0.1) opacity = 0.1;
             
             p.setOpacity(opacity);
             p.setBrush(color);
             p.setPen(Qt::NoPen);
             
             // Draw circle at top (0,-r)
             // But coordinate system is rotated.
             // Let's just draw at (0, -r + padding)? 
             // Actually standard way: draw at (0, -radius), rotate 90.
             // But radius needs to account for dot size so it fits in 'd'.
             // Spinner bounds 'd' usually includes the dots.
             // So center to dot center = (d/2) - (dotDiameter/2).
             
             int actualRadius = (d / 2) - (dotDiameter / 2);
             
             p.drawEllipse(QPoint(0, -actualRadius), dotDiameter / 2, dotDiameter / 2);
             p.rotate(90);
        }
        p.restore();

        // Draw Tip
        if (!m_tip.isEmpty()) {
            p.setFont(font());
            p.setPen(color); // Or Text color? Ant Design usually uses Primary for tip too? Or text. Let's use Primary.
            
            int textTop = cy + d / 2 + 8; // Below spinner
            QRect textRect(0, textTop, width(), fm.height());
            p.drawText(textRect, Qt::AlignCenter, m_tip);
        }
    }

}
