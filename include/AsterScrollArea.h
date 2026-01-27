#pragma once

#include "Global.h"
#include <QScrollArea>
#include "AsterSpace.h"

namespace AsterUI {

    class ASTERUI_EXPORT AsterScrollArea : public QScrollArea {
        Q_OBJECT
    public:
        explicit AsterScrollArea(QWidget* parent = nullptr);
        ~AsterScrollArea() override;

        // Proxy to internal AsterSpace container
        void addWidget(QWidget* widget);
        void addStretch(int stretch = 0);
        void setSpacing(int spacing);
        void setDirection(Qt::Orientation direction);
        void setContentsMargins(int left, int top, int right, int bottom);
        void setContentsMargins(const QMargins &margins);

        // Access internal container for advanced usage
        AsterSpace* container() const;

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        AsterSpace* m_container = nullptr;
    };

}
