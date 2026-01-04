#include "AsterUI/AsterDivider.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>
#include <QFontMetrics>

namespace AsterUI {

    AsterDivider::AsterDivider(Qt::Orientation orientation, QWidget* parent)
        : QWidget(parent)
        , m_orientation(orientation)
        , m_alignment(Qt::AlignCenter)
    {
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    }

    AsterDivider::AsterDivider(const QString& text, Qt::Alignment alignment, QWidget* parent)
        : QWidget(parent)
        , m_orientation(Qt::Horizontal)
        , m_text(text)
        , m_alignment(alignment)
    {
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    }

    AsterDivider::~AsterDivider() = default;

    void AsterDivider::setOrientation(Qt::Orientation orientation) {
        if (m_orientation != orientation) {
            m_orientation = orientation;
            update();
            updateGeometry();
        }
    }

    void AsterDivider::setText(const QString& text) {
        if (m_text != text) {
            m_text = text;
            update();
            updateGeometry();
        }
    }

    void AsterDivider::setAlignment(Qt::Alignment alignment) {
        if (m_alignment != alignment) {
            m_alignment = alignment;
            update();
        }
    }

    QSize AsterDivider::sizeHint() const {
        if (m_orientation == Qt::Vertical) {
            return QSize(1, 24); // Default height for vertical divider
        } else {
            if (m_text.isEmpty()) {
                return QSize(24, 1); // Default width for horizontal divider
            } else {
                QFontMetrics fm(font());
                int textWidth = fm.horizontalAdvance(m_text);
                return QSize(textWidth + 32, fm.height()); // Padding
            }
        }
    }

    void AsterDivider::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        auto theme = AsterTheme::instance();
        QColor borderColor = theme->color(AsterTheme::ColorRole::Border);
        
        painter.setPen(borderColor);

        if (m_orientation == Qt::Vertical) {
            int x = width() / 2;
            painter.drawLine(x, 0, x, height());
        } else {
            int y = height() / 2;
            if (m_text.isEmpty()) {
                painter.drawLine(0, y, width(), y);
            } else {
                QFontMetrics fm(font());
                int textWidth = fm.horizontalAdvance(m_text);
                int textHeight = fm.height();
                int spacing = 16; // Space between line and text

                int textX = 0;
                if (m_alignment & Qt::AlignLeft) {
                    textX = 24; // Margin left
                } else if (m_alignment & Qt::AlignRight) {
                    textX = width() - textWidth - 24; // Margin right
                } else {
                    textX = (width() - textWidth) / 2;
                }

                // Draw left line
                if (textX > spacing) {
                    painter.drawLine(0, y, textX - spacing, y);
                }

                // Draw text
                painter.setPen(theme->color(AsterTheme::ColorRole::Text));
                painter.drawText(textX, 0, textWidth, height(), Qt::AlignVCenter, m_text);

                // Draw right line
                painter.setPen(borderColor);
                if (textX + textWidth + spacing < width()) {
                    painter.drawLine(textX + textWidth + spacing, y, width(), y);
                }
            }
        }
    }

}
