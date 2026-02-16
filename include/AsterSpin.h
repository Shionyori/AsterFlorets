#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QString>
#include <QTimer>

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterSpin : public QWidget {
        Q_OBJECT
        Q_PROPERTY(bool spinning READ isSpinning WRITE setSpinning)
        Q_PROPERTY(QString tip READ tip WRITE setTip)
        Q_PROPERTY(Size spinSize READ spinSize WRITE setSpinSize)

    public:
        enum class Size {
            Small,
            Default,
            Large
        };
        Q_ENUM(Size)

        explicit AsterSpin(QWidget* parent = nullptr);
        ~AsterSpin() override;

        bool isSpinning() const;
        void setSpinning(bool spinning);

        QString tip() const;
        void setTip(const QString& tip);

        void setSpinSize(Size size); 
        Size spinSize() const;

        // If wrapping content (like <Spin><Content /></Spin>)
        // In Qt, this usually means AsterSpin is a container overlaid on top, or we use a layout trick.
        // For simplicity, this widget is just the spinner itself. 
        // A helper container might be needed for the "overlay" behavior.

    protected:
        void paintEvent(QPaintEvent* event) override;
        void showEvent(QShowEvent* event) override;
        void hideEvent(QHideEvent* event) override;
        QSize sizeHint() const override;

    private:
        void updateTimer();
        
        bool m_spinning = true;
        QString m_tip;
        Size m_size = Size::Default;
        int m_angle = 0;
        QTimer* m_timer;
    };

}
