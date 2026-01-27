#pragma once

#include "AsterGlobal.h"
#include <QProgressBar>

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterProgress : public QProgressBar {
        Q_OBJECT
        Q_PROPERTY(QColor color READ color WRITE setColor)
        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

    public:
        explicit AsterProgress(QWidget* parent = nullptr);
        ~AsterProgress() override;

        QColor color() const { return m_color; }
        void setColor(const QColor& color);

        QColor backgroundColor() const { return m_backgroundColor; }
        void setBackgroundColor(const QColor& color);

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        QColor m_color;
        QColor m_backgroundColor;
    };

}
