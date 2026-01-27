#include "AsterProgress.h"
#include "AsterTheme.h"
#include <QPainter>
#include <QPainterPath>

namespace AsterUI {

    AsterProgress::AsterProgress(QWidget* parent)
        : QProgressBar(parent)
    {
        auto theme = AsterTheme::instance();
        m_color = theme->color(AsterTheme::ColorRole::Primary);
        m_backgroundColor = theme->color(AsterTheme::ColorRole::Border); // Use Border color directly
        
        // Default height
        setFixedHeight(8);
        setTextVisible(false); // Default no text inside bar
    }

    AsterProgress::~AsterProgress() = default;

    void AsterProgress::setColor(const QColor& color) {
        m_color = color;
        update();
    }

    void AsterProgress::setBackgroundColor(const QColor& color) {
        m_backgroundColor = color;
        update();
    }

    void AsterProgress::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Draw track
        QRect rect = this->rect();
        int height = rect.height();
        int radius = height / 2;

        painter.setPen(Qt::NoPen);
        painter.setBrush(m_backgroundColor);
        painter.drawRoundedRect(rect, radius, radius);

        // Draw progress
        double percent = (double)(value() - minimum()) / (maximum() - minimum());
        if (percent > 0) {
            int progressWidth = (int)(rect.width() * percent);
            QRect progressRect = rect;
            progressRect.setWidth(progressWidth);
            
            painter.setBrush(m_color);
            painter.drawRoundedRect(progressRect, radius, radius);
        }
        
        // If text is visible and we want to draw it (QProgressBar default is center)
        // Ant Design usually puts it outside.
        // For now, we just draw the bar. If user wants text, they can use a label or we can add a property later.
    }

}
