#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QPixmap>
#include <QIcon>

namespace AsterUI {

    class ASTERUI_EXPORT AsterAvatar : public QWidget {
        Q_OBJECT
        Q_PROPERTY(int size READ size WRITE setSize)
        Q_PROPERTY(Shape shape READ shape WRITE setShape)
        Q_PROPERTY(QString text READ text WRITE setText)

    public:
        enum class Shape {
            Circle,
            Square
        };
        Q_ENUM(Shape)

        enum class Size {
            Small,
            Default,
            Large,
            Custom
        };
        Q_ENUM(Size)

        explicit AsterAvatar(QWidget* parent = nullptr);
        explicit AsterAvatar(const QString& text, QWidget* parent = nullptr);
        explicit AsterAvatar(const QPixmap& image, QWidget* parent = nullptr);
        ~AsterAvatar() override;

        void setShape(Shape shape);
        Shape shape() const { return m_shape; }

        void setSize(Size size);
        void setSize(int size); // Custom size in pixels
        int size() const;

        void setText(const QString& text);
        QString text() const { return m_text; }

        void setImage(const QPixmap& image);
        QPixmap image() const { return m_image; }

        void setIcon(const QIcon& icon);
        QIcon icon() const { return m_icon; }

        void setBackgroundColor(const QColor& color);
        QColor backgroundColor() const { return m_backgroundColor; }

        void setTextColor(const QColor& color);
        QColor textColor() const { return m_textColor; }

    protected:
        void paintEvent(QPaintEvent* event) override;
        QSize sizeHint() const override;

    private:
        void init();
        void updateSize();

        Shape m_shape = Shape::Circle;
        Size m_sizeType = Size::Default;
        int m_customSize = 40;
        
        QString m_text;
        QPixmap m_image;
        QIcon m_icon;
        
        QColor m_backgroundColor;
        QColor m_textColor;
    };

}
