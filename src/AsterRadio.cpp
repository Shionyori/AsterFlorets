#include "AsterUI/AsterRadio.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>
#include <QPainterPath>

namespace AsterUI {

    AsterRadio::AsterRadio(const QString& text, QWidget* parent)
        : AsterRadio(parent)
    {
        setText(text);
    }

    AsterRadio::AsterRadio(QWidget* parent)
        : QRadioButton(parent)
        , m_dotScale(0.0)
        , m_isHovered(false)
    {
        init();
    }

    AsterRadio::~AsterRadio() = default;

    void AsterRadio::init() {
        setCursor(Qt::PointingHandCursor);
        m_animGroup = new QParallelAnimationGroup(this);
        
        // Use toggled signal instead of checkStateSet for better reliability with QRadioButton
        connect(this, &QRadioButton::toggled, this, [this](bool checked) {
            startAnimation(checked);
        });

        updateColors();
    }

    void AsterRadio::setDotScale(double scale) {
        m_dotScale = scale;
        update();
    }

    void AsterRadio::setBorderColor(const QColor& color) {
        m_borderColor = color;
        update();
    }

    QSize AsterRadio::sizeHint() const {
        QSize size = QRadioButton::sizeHint();
        if (size.height() < 16) size.setHeight(16);
        return size;
    }

    bool AsterRadio::hitButton(const QPoint &pos) const {
        return rect().contains(pos);
    }

    void AsterRadio::updateColors() {
        auto theme = AsterTheme::instance();
        bool checked = isChecked();
        
        if (checked) {
            m_dotScale = 1.0;
            m_borderColor = theme->color(AsterTheme::ColorRole::Primary);
        } else {
            m_dotScale = 0.0;
            m_borderColor = m_isHovered ? theme->color(AsterTheme::ColorRole::Primary) 
                                        : theme->color(AsterTheme::ColorRole::Border);
        }
        update();
    }

    // Removed checkStateSet override

    void AsterRadio::enterEvent(QEnterEvent* event) {
        QRadioButton::enterEvent(event);
        m_isHovered = true;
        if (!isChecked()) {
            updateColors();
        }
    }

    void AsterRadio::leaveEvent(QEvent* event) {
        QRadioButton::leaveEvent(event);
        m_isHovered = false;
        if (!isChecked()) {
            updateColors();
        }
    }

    void AsterRadio::startAnimation(bool checked) {
        auto theme = AsterTheme::instance();
        m_animGroup->stop();
        m_animGroup->clear();

        QColor targetBorder = checked ? theme->color(AsterTheme::ColorRole::Primary) 
                                      : (m_isHovered ? theme->color(AsterTheme::ColorRole::Primary) : theme->color(AsterTheme::ColorRole::Border));
        
        double targetScale = checked ? 1.0 : 0.0;

        auto createColorAnim = [this](const QByteArray& prop, const QColor& start, const QColor& end) {
            auto anim = new QPropertyAnimation(this, prop);
            anim->setDuration(200);
            anim->setStartValue(start);
            anim->setEndValue(end);
            return anim;
        };

        auto createScaleAnim = [this](const QByteArray& prop, double start, double end) {
            auto anim = new QPropertyAnimation(this, prop);
            anim->setDuration(200);
            anim->setStartValue(start);
            anim->setEndValue(end);
            return anim;
        };

        m_animGroup->addAnimation(createScaleAnim("dotScale", m_dotScale, targetScale));
        m_animGroup->addAnimation(createColorAnim("borderColor", m_borderColor, targetBorder));
        m_animGroup->start();
    }

    void AsterRadio::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        auto theme = AsterTheme::instance();
        
        // Layout parameters
        int boxSize = 16;
        int spacing = 8;
        
        // Vertically center
        int y = (height() - boxSize) / 2;
        if (y < 0) y = 0; // Safety check
        QRect boxRect(0, y, boxSize, boxSize);

        // 1. Draw Outer Circle (Border + Background)
        painter.save();
        QPen pen(m_borderColor);
        pen.setWidthF(1.5);
        painter.setPen(pen);
        painter.setBrush(theme->color(AsterTheme::ColorRole::Surface)); // Always white background
        painter.drawEllipse(boxRect.adjusted(1, 1, -1, -1)); // Adjust for pen width
        painter.restore();

        // 2. Draw Inner Dot
        if (m_dotScale > 0.01) {
            painter.save();
            painter.setPen(Qt::NoPen);
            // Dot color is always Primary (or Disabled color if disabled)
            painter.setBrush(theme->color(AsterTheme::ColorRole::Primary));
            
            // Max dot size: 8px
            double maxDotSize = 8.0;
            double currentSize = maxDotSize * m_dotScale;
            double offset = (boxSize - currentSize) / 2.0;
            
            QRectF dotRect(boxRect.x() + offset, boxRect.y() + offset, currentSize, currentSize);
            
            painter.drawEllipse(dotRect);
            painter.restore();
        }

        // 3. Draw Text
        if (!text().isEmpty()) {
            QRect textRect = rect().adjusted(boxSize + spacing, 0, 0, 0);
            painter.setPen(theme->color(AsterTheme::ColorRole::Text));
            painter.setFont(font());
            painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text());
        }
    }

}
