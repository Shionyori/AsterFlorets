#include "AsterAlert.h"
#include "AsterTheme.h"
#include "AsterIcon.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QFontMetrics>

namespace AsterFlorets {

    AsterAlert::AsterAlert(QWidget* parent)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        setMouseTracking(true); // For hover effects on close button if any
    }

    AsterAlert::AsterAlert(const QString& title, Type type, QWidget* parent)
        : AsterAlert(parent)
    {
        m_title = title;
        m_type = type;
    }

    AsterAlert::AsterAlert(const QString& title, const QString& description, Type type, QWidget* parent)
        : AsterAlert(parent)
    {
        m_title = title;
        m_description = description;
        m_type = type;
    }

    AsterAlert::~AsterAlert() = default;

    AsterAlert::Type AsterAlert::type() const {
        return m_type;
    }

    void AsterAlert::setType(Type type) {
        if (m_type != type) {
            m_type = type;
            update();
        }
    }

    QString AsterAlert::title() const {
        return m_title;
    }

    void AsterAlert::setTitle(const QString& title) {
        if (m_title != title) {
            m_title = title;
            updateGeometry();
            update();
        }
    }

    QString AsterAlert::description() const {
        return m_description;
    }

    void AsterAlert::setDescription(const QString& description) {
        if (m_description != description) {
            m_description = description;
            updateGeometry();
            update();
        }
    }

    bool AsterAlert::isClosable() const {
        return m_closable;
    }

    void AsterAlert::setClosable(bool closable) {
        if (m_closable != closable) {
            m_closable = closable;
            update();
        }
    }

    bool AsterAlert::showIcon() const {
        return m_showIcon;
    }

    void AsterAlert::setShowIcon(bool show) {
        if (m_showIcon != show) {
            m_showIcon = show;
            updateGeometry();
            update();
        }
    }

    void AsterAlert::setCustomIcon(const QIcon& icon) {
        m_customIcon = icon;
        update();
    }

    QSize AsterAlert::sizeHint() const {
        // Calculate size based on content
        int w = 200; // Minimum width
        int h = 0;
        int padding = 12; // 8-16px usually
        int iconSize = (m_showIcon) ? 16 : 0;
        int iconGap = (m_showIcon) ? 8 : 0;
        int closeSize = (m_closable) ? 16 : 0;
        int closeGap = (m_closable) ? 8 : 0;

        int contentWidth = width() - padding * 2 - iconSize - iconGap - closeSize - closeGap;
        if (contentWidth < 0) contentWidth = 200; // Fallback for sizeHint calculation if width not set

        QFont titleFont = font();
        if (!m_description.isEmpty()) {
            titleFont.setBold(true);
        }
        QFontMetrics fmTitle(titleFont);
        
        // Simple height estimation
        int textHeight = fmTitle.height();
        if (!m_description.isEmpty()) {
            QFont descFont = font(); // Usually smaller? AntD keeps it same size just lighter color for basic
            QFontMetrics fmDesc(descFont);
            // Description can wrap
            QRect descRect = fmDesc.boundingRect(0, 0, contentWidth, 0, Qt::TextWordWrap, m_description);
            textHeight += 4; // Gap logic
            textHeight += descRect.height();
        }

        h = qMax(textHeight, iconSize) + padding * 2;
        // Ensure minimum height
        if (h < 32) h = 32;

        return QSize(w, h);
    }

    void AsterAlert::resizeEvent(QResizeEvent* event) {
        QWidget::resizeEvent(event);
        if (m_closable) {
            int closeSize = 16;
            m_closeRect = QRect(width() - 12 - closeSize, 12, closeSize, closeSize); // Top-right roughly
        }
    }

    void AsterAlert::mouseReleaseEvent(QMouseEvent* event) {
        if (m_closable && m_closeRect.contains(event->pos())) {
            emit closed();
            close(); // Or hide? Standard behavior is close.
        }
        QWidget::mouseReleaseEvent(event);
    }

    void AsterAlert::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        // Colors
        QColor bg = getBackgroundColor();
        QColor border = getBorderColor();
        QColor text = AsterTheme::instance()->color(AsterTheme::ColorRole::Text);

        // Draw background
        QRect r = rect();
        p.setBrush(bg);
        p.setPen(QPen(border, 1));
        p.drawRoundedRect(r.adjusted(0, 0, -1, -1), 4, 4);

        int leftOffset = 15; // Padding left
        int topOffset = 11;  // Padding top (adjusted for visual center with 1.5 line height logic)
        
        // Draw Icon
        if (m_showIcon) {
            int iconSize = 16; // Ant Design icon size
            // Determine icon color
            QColor iconColor = getTypeColor();
            
            // Draw dummy circle for now or load actual icon if resource system ready
            // Here we just draw a circle with the color
            if (!m_customIcon.isNull()) {
                m_customIcon.paint(&p, leftOffset, topOffset, iconSize, iconSize);
            } else {
                // Determine icon shape based on type
                // Info: 'i', Success: check, Warning: '!', Error: 'x'
                p.setPen(Qt::NoPen);
                p.setBrush(iconColor);
                p.drawEllipse(leftOffset, topOffset + 1, 14, 14);
                
                // Simple symbol inside
                p.setPen(Qt::white);
                if (m_type == Type::Success) {
                     // Checkmark simplified
                     QPainterPath path;
                     path.moveTo(leftOffset + 4, topOffset + 8);
                     path.lineTo(leftOffset + 6, topOffset + 10);
                     path.lineTo(leftOffset + 10, topOffset + 5);
                     p.drawPath(path);
                } else if (m_type == Type::Info) {
                    p.drawText(QRect(leftOffset, topOffset + 1, 14, 14), Qt::AlignCenter, "i");
                } else if (m_type == Type::Warning) {
                    p.drawText(QRect(leftOffset, topOffset + 1, 14, 14), Qt::AlignCenter, "!");
                } else {
                    p.drawText(QRect(leftOffset, topOffset + 1, 14, 14), Qt::AlignCenter, "x");
                }
            }
            leftOffset += iconSize + 8; // Gap
        }

        // Draw Text
        QFont f = font();
        if (!m_description.isEmpty()) {
            // With description
            f.setBold(true);
            p.setFont(f);
            p.setPen(text);
            
            QFontMetrics fm(f);
            int titleH = fm.height();
            p.drawText(leftOffset, topOffset + fm.ascent(), m_title);

            // Description
            f.setBold(false);
            p.setFont(f);
            // Description color is usually lighter
            p.setPen(AsterTheme::instance()->color(AsterTheme::ColorRole::TextSecondary));
            
            QRect descRect(leftOffset, topOffset + titleH + 4, width() - leftOffset - (m_closable ? 30 : 15), height() - (topOffset + titleH + 4));
            p.drawText(descRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, m_description);

        } else {
            // Title only (centered vertically if single line, but usually top aligned)
            p.setFont(f);
            p.setPen(text);
             // Center vertically if no description?
             // Actually Alert text is usually top aligned with some padding
             p.drawText(QRect(leftOffset, 0, width() - leftOffset - (m_closable ? 30 : 15), height()), Qt::AlignVCenter | Qt::AlignLeft, m_title);
        }

        // Draw Close button
        if (m_closable) {
            // Simple X
            int cx = width() - 12 - 8;
            int cy = 12 + 8; // Center of the 16x16 box
            
            p.setPen(QPen(AsterTheme::instance()->color(AsterTheme::ColorRole::TextSecondary), 1.5));
            p.drawLine(cx - 4, cy - 4, cx + 4, cy + 4);
            p.drawLine(cx - 4, cy + 4, cx + 4, cy - 4);
        }
    }

    QColor AsterAlert::getTypeColor() const {
        auto theme = AsterTheme::instance();
        switch (m_type) {
            case Type::Success: return theme->color(AsterTheme::ColorRole::Success);
            case Type::Info:    return theme->color(AsterTheme::ColorRole::Primary);
            case Type::Warning: return theme->color(AsterTheme::ColorRole::Warning);
            case Type::Error:   return theme->color(AsterTheme::ColorRole::Error);
            default:            return theme->color(AsterTheme::ColorRole::Primary);
        }
    }

    QColor AsterAlert::getBackgroundColor() const {
        auto theme = AsterTheme::instance();
        // Usually a very light version of the type color
        // For now, simpler approximation or use existing palette roles if defined
        // Ant Design uses specific palettes like blue-1, green-1 etc.
        QColor c = getTypeColor();
        c.setAlpha(30); // 12% opacity approx for background
        return c;
    }

    QColor AsterAlert::getBorderColor() const {
        // Slightly darker than bg
        QColor c = getTypeColor();
        c.setAlpha(60); 
        return c;
    }

}
