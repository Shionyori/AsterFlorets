#include "AsterUI/AsterGrid.h"
#include <QLayoutItem>

namespace AsterUI {

    AsterGrid::AsterGrid(QWidget* parent)
        : QWidget(parent)
    {
        m_layout = new QGridLayout(this);
        m_layout->setContentsMargins(0, 0, 0, 0);
        m_layout->setHorizontalSpacing(m_hSpacing);
        m_layout->setVerticalSpacing(m_vSpacing);

        setAttribute(Qt::WA_StyledBackground, false);
        setAttribute(Qt::WA_TranslucentBackground, true);
    }

    AsterGrid::AsterGrid(int columns, QWidget* parent)
        : AsterGrid(parent)
    {
        setColumnCount(columns);
    }

    AsterGrid::~AsterGrid() = default;

    int AsterGrid::columnCount() const {
        return m_columnCount;
    }

    void AsterGrid::setColumnCount(int count) {
        if (count < 1) count = 1;
        if (m_columnCount != count) {
            m_columnCount = count;
            reflow();
        }
    }

    int AsterGrid::spacing() const {
        return m_hSpacing; 
    }

    void AsterGrid::setSpacing(int spacing) {
        if (m_hSpacing != spacing || m_vSpacing != spacing) {
            m_hSpacing = spacing;
            m_vSpacing = spacing;
            m_layout->setHorizontalSpacing(m_hSpacing);
            m_layout->setVerticalSpacing(m_vSpacing);
        }
    }

    int AsterGrid::horizontalSpacing() const {
        return m_hSpacing;
    }

    void AsterGrid::setHorizontalSpacing(int spacing) {
        if (m_hSpacing != spacing) {
            m_hSpacing = spacing;
            m_layout->setHorizontalSpacing(m_hSpacing);
        }
    }

    int AsterGrid::verticalSpacing() const {
        return m_vSpacing;
    }

    void AsterGrid::setVerticalSpacing(int spacing) {
        if (m_vSpacing != spacing) {
            m_vSpacing = spacing;
            m_layout->setVerticalSpacing(m_vSpacing);
        }
    }

    void AsterGrid::addWidget(QWidget* w) {
        if (!w) return;
        m_items.append(w);
        
        // Add to current layout position
        int index = m_items.size() - 1;
        int row = index / m_columnCount;
        int col = index % m_columnCount;
        
        m_layout->addWidget(w, row, col);
    }

    void AsterGrid::removeWidget(QWidget* w) {
        if (!w) return;
        int idx = m_items.indexOf(w);
        if (idx != -1) {
            m_layout->removeWidget(w);
            w->setParent(nullptr); 
            m_items.removeAt(idx);
            reflow();
        }
    }

    void AsterGrid::clear() {        
        QLayoutItem* item;
        while ((item = m_layout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->setParent(nullptr); // Detach
                delete item->widget(); 
            }
            delete item;
        }
        m_items.clear();
    }

    void AsterGrid::reflow() {
        // Remove all from layout
        QLayoutItem* item;
        while ((item = m_layout->takeAt(0)) != nullptr) {
            delete item; // Just delete the container item, not the widget
        }

        // Re-add
        for (int i = 0; i < m_items.size(); ++i) {
            int row = i / m_columnCount;
            int col = i % m_columnCount;
            // Ensure widget is not deleted? it is in m_items.
            m_layout->addWidget(m_items[i], row, col);
        }
    }

    void AsterGrid::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
    }

}
