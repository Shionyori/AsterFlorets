#include "AsterUI/AsterTag.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

namespace AsterUI {

    AsterTag::AsterTag(const QString& text, QWidget* parent)
        : AsterTag(parent)
    {
        setText(text);
    }

    AsterTag::AsterTag(QWidget* parent)
        : QWidget(parent)
    {
        init();
    }

    AsterTag::~AsterTag() = default;

    void AsterTag::init() {
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        updateStyle();
    }

    void AsterTag::setText(const QString& text) {
        m_text = text;
        update();
        updateGeometry();
    }

    void AsterTag::setType(Type type) {
        m_type = type;
        updateStyle();
        update();
    }

    void AsterTag::setClosable(bool closable) {
        m_closable = closable;
        setMouseTracking(closable); // 需要开启鼠标追踪以检测关闭按钮悬停
        update();
        updateGeometry();
    }

    void AsterTag::updateStyle() {
        auto theme = AsterTheme::instance();
        
        // 辅助函数：混合白色以变浅
        auto lighten = [](const QColor& color, int factor = 235) {
            int r = (color.red() * (255 - factor) + 255 * factor) / 255;
            int g = (color.green() * (255 - factor) + 255 * factor) / 255;
            int b = (color.blue() * (255 - factor) + 255 * factor) / 255;
            return QColor(r, g, b);
        };

        switch (m_type) {
            case Type::Primary:
                m_textColor = theme->color(AsterTheme::ColorRole::Primary);
                m_bgColor = lighten(m_textColor);
                m_borderColor = m_textColor;
                m_borderColor.setAlpha(80);
                break;
            case Type::Success:
                m_textColor = theme->color(AsterTheme::ColorRole::Success);
                m_bgColor = lighten(m_textColor);
                m_borderColor = m_textColor;
                m_borderColor.setAlpha(80);
                break;
            case Type::Warning:
                m_textColor = theme->color(AsterTheme::ColorRole::Warning);
                m_bgColor = lighten(m_textColor);
                m_borderColor = m_textColor;
                m_borderColor.setAlpha(80);
                break;
            case Type::Error:
                m_textColor = theme->color(AsterTheme::ColorRole::Error);
                m_bgColor = lighten(m_textColor);
                m_borderColor = m_textColor;
                m_borderColor.setAlpha(80);
                break;
            case Type::Default:
            default:
                m_textColor = theme->color(AsterTheme::ColorRole::Text);
                m_bgColor = QColor(245, 245, 245); // Light Gray
                m_borderColor = QColor(217, 217, 217);
                break;
        }
    }

    QSize AsterTag::sizeHint() const {
        QFontMetrics fm(font());
        int w = fm.horizontalAdvance(m_text) + 16; // Padding
        if (m_closable) {
            w += 20; // Space for close icon
        }
        return QSize(w, 24); // Fixed height usually
    }

    QRect AsterTag::closeButtonRect() const {
        if (!m_closable) return QRect();
        int h = height();
        int iconSize = 14;
        return QRect(width() - iconSize - 5, (h - iconSize) / 2, iconSize, iconSize);
    }

    void AsterTag::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Draw Background
        QRectF r = rect();
        r.adjust(0.5, 0.5, -0.5, -0.5);
        
        QPainterPath path;
        path.addRoundedRect(r, 4, 4); // Small radius for tags

        painter.fillPath(path, m_bgColor);
        
        QPen pen(m_borderColor);
        painter.setPen(pen);
        painter.drawPath(path);

        // Draw Text
        painter.setPen(m_textColor);
        QRect textRect = rect();
        if (m_closable) {
            textRect.adjust(0, 0, -20, 0);
        }
        painter.drawText(textRect, Qt::AlignCenter, m_text);

        // Draw Close Button
        if (m_closable) {
            QRect closeRect = closeButtonRect();
            
            if (m_isCloseHovered) {
                painter.setPen(Qt::NoPen);
                painter.setBrush(m_textColor); // Hover 时用文字色做背景
                painter.drawEllipse(closeRect);
                painter.setPen(Qt::white); // X 变白
            } else {
                painter.setPen(m_textColor);
                painter.setBrush(Qt::NoBrush);
            }

            // Draw 'X'
            int p = 4; // padding inside close rect
            painter.drawLine(closeRect.left() + p, closeRect.top() + p, closeRect.right() - p, closeRect.bottom() - p);
            painter.drawLine(closeRect.left() + p, closeRect.bottom() - p, closeRect.right() - p, closeRect.top() + p);
        }
    }

    void AsterTag::mousePressEvent(QMouseEvent* event) {
        if (m_closable && closeButtonRect().contains(event->pos())) {
            emit closed();
            hide(); // Default behavior: hide on close
        } else {
            emit clicked();
        }
    }

    void AsterTag::mouseMoveEvent(QMouseEvent* event) {
        if (m_closable) {
            bool hover = closeButtonRect().contains(event->pos());
            if (hover != m_isCloseHovered) {
                m_isCloseHovered = hover;
                update();
                setCursor(hover ? Qt::PointingHandCursor : Qt::ArrowCursor);
            }
        }
        QWidget::mouseMoveEvent(event);
    }

    void AsterTag::enterEvent(QEnterEvent* event) {
        m_isHovered = true;
        update();
        QWidget::enterEvent(event);
    }

    void AsterTag::leaveEvent(QEvent* event) {
        m_isHovered = false;
        m_isCloseHovered = false;
        update();
        QWidget::leaveEvent(event);
    }

}
