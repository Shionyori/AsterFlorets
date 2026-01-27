#pragma once

#include "Global.h"
#include <QWidget>

namespace AsterUI {

    class AsterFlowLayout; // Forward declaration

    class ASTERUI_EXPORT AsterFlow : public QWidget {
        Q_OBJECT
        Q_PROPERTY(int hSpacing READ hSpacing WRITE setHSpacing)
        Q_PROPERTY(int vSpacing READ vSpacing WRITE setVSpacing)

    public:
        explicit AsterFlow(QWidget* parent = nullptr);
        ~AsterFlow() override;

        int hSpacing() const;
        void setHSpacing(int spacing);

        int vSpacing() const;
        void setVSpacing(int spacing);

        void addWidget(QWidget* w);

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        AsterFlowLayout* m_layout;
    };

}
