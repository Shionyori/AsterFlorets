#pragma once

#include "Global.h"
#include <QWidget>
#include <QVBoxLayout>

namespace AsterUI {

    class ASTERUI_EXPORT AsterColumn : public QWidget {
        Q_OBJECT
        Q_PROPERTY(int span READ span WRITE setSpan)

    public:
        explicit AsterColumn(QWidget* parent = nullptr);
        explicit AsterColumn(int span, QWidget* parent = nullptr);
        ~AsterColumn() override;

        int span() const;
        void setSpan(int span);

        void addWidget(QWidget* w);

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        int m_span = 24;
        QVBoxLayout* m_layout;
    };

}
