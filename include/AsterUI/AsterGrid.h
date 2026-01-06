#pragma once

#include "Global.h"
#include <QWidget>
#include <QGridLayout>

namespace AsterUI {

    /**
     * @brief AsterGrid
     * A widget that arranges its children in a uniform grid.
     * Unlike AsterRow/AsterColumn which provide offset/span control,
     * AsterGrid is for automatically arranging N items into M columns.
     */
    class ASTERUI_EXPORT AsterGrid : public QWidget {
        Q_OBJECT
        Q_PROPERTY(int columnCount READ columnCount WRITE setColumnCount)
        Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
        Q_PROPERTY(int horizontalSpacing READ horizontalSpacing WRITE setHorizontalSpacing)
        Q_PROPERTY(int verticalSpacing READ verticalSpacing WRITE setVerticalSpacing)

    public:
        explicit AsterGrid(QWidget* parent = nullptr);
        explicit AsterGrid(int columns, QWidget* parent = nullptr);
        ~AsterGrid() override;

        int columnCount() const;
        void setColumnCount(int count);

        int spacing() const;
        void setSpacing(int spacing);

        int horizontalSpacing() const;
        void setHorizontalSpacing(int spacing);

        int verticalSpacing() const;
        void setVerticalSpacing(int spacing);

        void addWidget(QWidget* w);
        void removeWidget(QWidget* w); // Added for completeness
        void clear();

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        void reflow();

    private:
        int m_columnCount = 3;
        int m_hSpacing = 16;
        int m_vSpacing = 16;
        QGridLayout* m_layout;
        QList<QWidget*> m_items;
    };

}
