#include "AsterUI/AsterAvatar.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>
#include <QPainterPath>

namespace AsterUI {

    AsterAvatar::AsterAvatar(QWidget* parent)
        : QWidget(parent)
    {
        init();
    }

    AsterAvatar::AsterAvatar(const QString& text, QWidget* parent)
        : QWidget(parent)
    {
        init();
        setText(text);
    }

    AsterAvatar::AsterAvatar(const QPixmap& image, QWidget* parent)
        : QWidget(parent)
    {
        init();
        setImage(image);
    }

    AsterAvatar::~AsterAvatar() = default;

    void AsterAvatar::init() {
        auto theme = AsterTheme::instance();
        m_backgroundColor = theme->color(AsterTheme::ColorRole::Primary); // Default to primary color
        m_textColor = Qt::white;
        
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        updateSize();
    }

    void AsterAvatar::setShape(Shape shape) {
        m_shape = shape;
        update();
    }

    void AsterAvatar::setSize(Size size) {
        m_sizeType = size;
        updateSize();
    }

    void AsterAvatar::setSize(int size) {
        m_sizeType = Size::Custom;
        m_customSize = size;
        updateSize();
    }

    int AsterAvatar::size() const {
        return m_customSize;
    }

    void AsterAvatar::updateSize() {
        int s = 40;
        switch (m_sizeType) {
            case Size::Small: s = 32; break;
            case Size::Default: s = 40; break;
            case Size::Large: s = 64; break;
            case Size::Custom: s = m_customSize; break;
        }
        m_customSize = s;
        setFixedSize(s, s);
        update();
    }

    void AsterAvatar::setText(const QString& text) {
        m_text = text;
        m_image = QPixmap();
        m_icon = QIcon();
        update();
    }

    void AsterAvatar::setImage(const QPixmap& image) {
        m_image = image;
        m_text.clear();
        m_icon = QIcon();
        update();
    }

    void AsterAvatar::setIcon(const QIcon& icon) {
        m_icon = icon;
        m_text.clear();
        m_image = QPixmap();
        update();
    }

    void AsterAvatar::setBackgroundColor(const QColor& color) {
        m_backgroundColor = color;
        update();
    }

    void AsterAvatar::setTextColor(const QColor& color) {
        m_textColor = color;
        update();
    }

    QSize AsterAvatar::sizeHint() const {
        return QSize(m_customSize, m_customSize);
    }

    void AsterAvatar::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QRect rect = this->rect();
        
        // Create clip path for shape
        QPainterPath path;
        if (m_shape == Shape::Circle) {
            path.addEllipse(rect);
        } else {
            path.addRoundedRect(rect, 4, 4); // Slight radius for square
        }

        painter.setClipPath(path);

        if (!m_image.isNull()) {
            // Draw image
            painter.drawPixmap(rect, m_image.scaled(rect.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        } else {
            // Draw background
            painter.fillPath(path, m_backgroundColor);

            // Draw text or icon
            if (!m_text.isEmpty()) {
                painter.setPen(m_textColor);
                QFont font = painter.font();
                font.setPixelSize(m_customSize / 2); // Adjust font size relative to avatar size
                painter.setFont(font);
                painter.drawText(rect, Qt::AlignCenter, m_text.left(2).toUpper()); // Limit to 2 chars
            } else if (!m_icon.isNull()) {
                int iconSize = m_customSize * 0.6;
                QPixmap iconPix = m_icon.pixmap(iconSize, iconSize);
                int x = (width() - iconSize) / 2;
                int y = (height() - iconSize) / 2;
                painter.drawPixmap(x, y, iconPix);
            }
        }
    }

}
