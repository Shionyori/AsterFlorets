#pragma once

#include "Global.h"
#include <QWidget>
#include <QHBoxLayout>

namespace AsterUI {

    class AsterColumn;

    class ASTERUI_EXPORT AsterRow : public QWidget {
        Q_OBJECT
        Q_PROPERTY(int gutter READ gutter WRITE setGutter)

    public:
        explicit AsterRow(QWidget* parent = nullptr);
        ~AsterRow() override;

        int gutter() const;
        void setGutter(int gutter);

        void addColumn(AsterColumn* col);
        
        // Compatibility helper
        void addWidget(QWidget* w); 

    protected:
        bool event(QEvent* event) override;
        void paintEvent(QPaintEvent* event) override;

    private:
        void updateSpacing();

    private:
        int m_gutter = 0;
        QHBoxLayout* m_layout;
    };

}
