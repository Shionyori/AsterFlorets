#pragma once

#include "Global.h"
#include <QWidget>
#include <QColor>

namespace AsterUI {

    class ASTERUI_EXPORT AsterBadge : public QWidget {
        Q_OBJECT
        Q_PROPERTY(int count READ count WRITE setCount)
        Q_PROPERTY(bool showZero READ showZero WRITE setShowZero)
        Q_PROPERTY(int maxCount READ maxCount WRITE setMaxCount)
        Q_PROPERTY(bool dot READ isDot WRITE setDot)
        Q_PROPERTY(QColor color READ color WRITE setColor)

    public:
        explicit AsterBadge(QWidget* parent = nullptr);
        explicit AsterBadge(QWidget* contentWidget, QWidget* parent);
        ~AsterBadge() override;

        void setContentWidget(QWidget* widget);
        QWidget* contentWidget() const;

        int count() const;
        void setCount(int count);

        bool showZero() const;
        void setShowZero(bool show);

        int maxCount() const;
        void setMaxCount(int max);

        bool isDot() const;
        void setDot(bool dot);

        QColor color() const;
        void setColor(const QColor& color);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;
        bool eventFilter(QObject* watched, QEvent* event) override;

    private:
        void updateSize();
        QString displayText() const;

        QWidget* m_contentWidget = nullptr;
        QWidget* m_indicator = nullptr;
        int m_count = 0;
        bool m_showZero = false;
        int m_maxCount = 99;
        bool m_dot = false;
        QColor m_color; // Default should be Error/Red
    };

}
