#include "AsterColumn.h"
#include "AsterRow.h"
#include <QVBoxLayout>

namespace AsterFlorets {

    AsterColumn::AsterColumn(QWidget* parent)
        : QWidget(parent)
    {
        m_layout = new QVBoxLayout(this);
        m_layout->setContentsMargins(0, 0, 0, 0);
        m_layout->setSpacing(0); // Default local spacing

        setAttribute(Qt::WA_StyledBackground, false);
        setAttribute(Qt::WA_TranslucentBackground, true);

        QSizePolicy policy = sizePolicy();
        policy.setHeightForWidth(true);
        setSizePolicy(policy);
    }

    AsterColumn::AsterColumn(int span, QWidget* parent)
        : AsterColumn(parent)
    {
        setSpan(span);
    }

    AsterColumn::~AsterColumn() = default;

    int AsterColumn::span() const {
        return m_span;
    }

    void AsterColumn::setSpan(int span) {
        if (span < 1) span = 1;
        // In 24 grid system, usually max is 24.
        if (span > 24) span = 24;

        if (m_span != span) {
            m_span = span;
            
            // Try to update parent layout if already added
            if (parentWidget()) {
                // If the parent layout supports stretch/flex
                auto parentLayout = parentWidget()->layout();
                if (auto hbox = qobject_cast<QHBoxLayout*>(parentLayout)) {
                    // Find index
                    int idx = hbox->indexOf(this);
                    if (idx >= 0) {
                        hbox->setStretch(idx, m_span);
                    }
                }
            }
        }
    }

    void AsterColumn::addWidget(QWidget* w) {
        m_layout->addWidget(w);
    }

    void AsterColumn::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
    }

}
