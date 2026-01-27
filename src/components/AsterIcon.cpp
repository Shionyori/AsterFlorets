#include "AsterIcon.h"
#include <QPainter>
#include <QPaintEvent>

namespace AsterFlorets {

AsterIcon::AsterIcon(QWidget *parent)
    : QWidget(parent), m_iconSize(16, 16)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

AsterIcon::AsterIcon(const QIcon &icon, QWidget *parent)
    : AsterIcon(parent)
{
    m_icon = icon;
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

AsterIcon::AsterIcon(const QString &fileName, QWidget *parent)
    : AsterIcon(parent)
{
    m_icon = QIcon(fileName);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QIcon AsterIcon::icon() const
{
    return m_icon;
}

void AsterIcon::setIcon(const QIcon &icon)
{
    m_icon = icon;
    update();
}

QSize AsterIcon::iconSize() const
{
    return m_iconSize;
}

void AsterIcon::setIconSize(const QSize &size)
{
    if (m_iconSize != size) {
        m_iconSize = size;
        updateGeometry();
        update();
    }
}

QColor AsterIcon::color() const
{
    return m_color;
}

void AsterIcon::setColor(const QColor &color)
{
    m_color = color;
    update();
}

void AsterIcon::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (m_icon.isNull()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QSize drawSize = m_iconSize;
    if (drawSize.isEmpty()) {
        drawSize = size(); 
    }
    
    QPixmap pixmap = m_icon.pixmap(drawSize);
    
    if (m_color.isValid()) {
        // Create a mask from the pixmap alpha channel and fill it with color
        QPixmap colorized = QPixmap(pixmap.size());
        colorized.fill(Qt::transparent);
        
        QPainter p(&colorized);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.drawPixmap(0, 0, pixmap);
        p.setCompositionMode(QPainter::CompositionMode_SourceIn);
        p.fillRect(colorized.rect(), m_color);
        p.end();
        
        pixmap = colorized;
    }

    int x = (width() - drawSize.width()) / 2;
    int y = (height() - drawSize.height()) / 2;
    
    painter.drawPixmap(x, y, pixmap);
}

QSize AsterIcon::sizeHint() const
{
    return m_iconSize;
}

}
