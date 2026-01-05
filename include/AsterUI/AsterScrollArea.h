#pragma once

#include "Global.h"
#include <QScrollArea>

namespace AsterUI {

    class ASTERUI_EXPORT AsterScrollArea : public QScrollArea {
        Q_OBJECT
    public:
        explicit AsterScrollArea(QWidget* parent = nullptr);
        ~AsterScrollArea() override;

    protected:
        void paintEvent(QPaintEvent* event) override;
    };

}
