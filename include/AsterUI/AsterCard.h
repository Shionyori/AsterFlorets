#pragma once

#include "Global.h"
#include <QFrame>

namespace AsterUI {

    class ASTERUI_EXPORT AsterCard : public QFrame {
        Q_OBJECT
        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
        Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius)
        Q_PROPERTY(bool hoverable READ isHoverable WRITE setHoverable)

    public:
        explicit AsterCard(QWidget* parent = nullptr);
        ~AsterCard() override;

        // 属性
        QColor backgroundColor() const { return m_backgroundColor; }
        void setBackgroundColor(const QColor& color);

        int borderRadius() const { return m_borderRadius; }
        void setBorderRadius(int radius);

        bool isHoverable() const { return m_hoverable; }
        void setHoverable(bool hoverable);

        // 标题支持 (可选)
        void setTitle(const QString& title);
        QString title() const { return m_title; }

    protected:
        void paintEvent(QPaintEvent* event) override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;

    private:
        void init();
        void updateStyle();

    private:
        QColor m_backgroundColor;
        int m_borderRadius = -1;
        bool m_hoverable = false;
        bool m_isHovered = false;
        QString m_title;
    };

}
