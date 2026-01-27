#include "AsterSpace.h"
#include "AsterTheme.h"
#include <QPainter>
#include <QStyleOption>

namespace AsterUI {

AsterSpace::AsterSpace(QWidget *parent)
    : QWidget(parent)
{
    init();
}

AsterSpace::AsterSpace(Qt::Orientation direction, QWidget *parent)
    : QWidget(parent), m_direction(direction)
{
    init();
}

AsterSpace::~AsterSpace()
{
}

void AsterSpace::init()
{
    // Default size from theme
    m_size = AsterTheme::instance()->spacing(AsterTheme::Size::Default);

    if (m_direction == Qt::Horizontal) {
        m_layout = new QHBoxLayout(this);
    } else {
        m_layout = new QVBoxLayout(this);
    }
    
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(m_size);
    // Remove default alignment to allow expansion
    // m_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop); 
    
    // Default Size Policy

}

Qt::Orientation AsterSpace::direction() const
{
    return m_direction;
}

void AsterSpace::setDirection(Qt::Orientation direction)
{
    if (m_direction != direction) {
        m_direction = direction;
        
        m_layout->setDirection(direction == Qt::Horizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
        
        update();
    }
}

int AsterSpace::size() const
{
    return m_size;
}

void AsterSpace::setSize(int size)
{
    if (m_size != size) {
        m_size = size;
        updateSpacing();
    }
}

void AsterSpace::addWidget(QWidget *widget)
{
    if (m_layout) {
        m_layout->addWidget(widget);
    }
}

void AsterSpace::addStretch(int stretch)
{
    if (m_layout) {
        m_layout->addStretch(stretch);
    }
}

void AsterSpace::updateSpacing()
{
    if (m_layout) {
        m_layout->setSpacing(m_size);
    }
}

void AsterSpace::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // Allow stylesheets to render background
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

}
