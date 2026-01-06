#include "AsterUI/AsterGrid.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>

namespace AsterUI {

// --- AsterCol ---

AsterCol::AsterCol(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    // Don't set spacing here, let inner content decide or default to 0
    m_layout->setSpacing(0); 
}

AsterCol::AsterCol(int span, QWidget *parent)
    : AsterCol(parent)
{
    setSpan(span);
}

AsterCol::~AsterCol()
{
}

int AsterCol::span() const
{
    return m_span;
}

void AsterCol::setSpan(int span)
{
    if (span < 1) span = 1;
    if (span > 24) span = 24;
    
    if (m_span != span) {
        m_span = span;
        
        // Update parent layout stretch if attached
        if (parentWidget()) {
            AsterRow *row = qobject_cast<AsterRow*>(parentWidget());
            if (row) {
                 // Trigger row update? 
                 // Layout stretch is updated in Row's addCol currently.
                 // If we change span dynamically, we need to update stretch.
                 // But QHBoxLayout uses index to set stretch.
                 QHBoxLayout* rowLayout = qobject_cast<QHBoxLayout*>(row->layout());
                 if (rowLayout) {
                     int index = rowLayout->indexOf(this);
                     if (index >= 0) {
                         rowLayout->setStretch(index, m_span);
                     }
                 }
            }
        }
    }
}

void AsterCol::setWidget(QWidget *widget)
{
    // Clear existing? For now, just add.
    addWidget(widget);
}

void AsterCol::addWidget(QWidget *widget)
{
    m_layout->addWidget(widget);
}

void AsterCol::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // Transparent container
}


// --- AsterRow ---

AsterRow::AsterRow(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0); // Default 0, controlled by gutter
}

AsterRow::~AsterRow()
{
}

int AsterRow::gutter() const
{
    return m_gutter;
}

void AsterRow::setGutter(int gutter)
{
    if (m_gutter != gutter) {
        m_gutter = gutter;
        m_layout->setSpacing(m_gutter);
    }
}

void AsterRow::addCol(AsterCol *col)
{
    m_layout->addWidget(col);
    m_layout->setStretchFactor(col, col->span());
}

void AsterRow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}

}
