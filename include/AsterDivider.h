#pragma once

#include "AsterGlobal.h"
#include <QWidget>

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterDivider : public QWidget {
        Q_OBJECT
        Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
        Q_PROPERTY(QString text READ text WRITE setText)
        Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)

    public:
        explicit AsterDivider(Qt::Orientation orientation = Qt::Horizontal, QWidget* parent = nullptr);
        explicit AsterDivider(const QString& text, Qt::Alignment alignment = Qt::AlignCenter, QWidget* parent = nullptr);
        ~AsterDivider() override;

        Qt::Orientation orientation() const { return m_orientation; }
        void setOrientation(Qt::Orientation orientation);

        QString text() const { return m_text; }
        void setText(const QString& text);

        Qt::Alignment alignment() const { return m_alignment; }
        void setAlignment(Qt::Alignment alignment);

        QSize sizeHint() const override;

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        Qt::Orientation m_orientation;
        QString m_text;
        Qt::Alignment m_alignment;
    };

}
