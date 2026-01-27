#include "AsterRow.h"
#include "AsterColumn.h"
#include <QEvent>

namespace AsterFlorets {

    AsterRow::AsterRow(QWidget* parent)
        : QWidget(parent)
    {
        m_layout = new QHBoxLayout(this);
        m_layout->setContentsMargins(0, 0, 0, 0);
        m_layout->setSpacing(0);
        
        // Allow transparency
        setAttribute(Qt::WA_StyledBackground, false); 
        setAttribute(Qt::WA_TranslucentBackground, true);
    }

    AsterRow::~AsterRow() = default;

    int AsterRow::gutter() const {
        return m_gutter;
    }

    void AsterRow::setGutter(int gutter) {
        if (m_gutter != gutter) {
            m_gutter = gutter;
            updateSpacing();
        }
    }

    void AsterRow::updateSpacing() {
        if (m_layout) {
            m_layout->setSpacing(m_gutter);
        }
    }

    void AsterRow::addColumn(AsterColumn* col) {
        if (!col) return;
        m_layout->addWidget(col, col->span());
    }

    void AsterRow::addWidget(QWidget* w) {
        // If it's an AsterColumn, respect its span
        if (auto col = qobject_cast<AsterColumn*>(w)) {
            addColumn(col);
        } else {
            // Treat as span 1 or just add
            m_layout->addWidget(w);
        }
    }

    bool AsterRow::event(QEvent* event) {
        return QWidget::event(event);
    }

    void AsterRow::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
    }

}
