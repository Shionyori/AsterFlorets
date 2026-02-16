#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QFrame>
#include <QPropertyAnimation>
#include <QVBoxLayout>

namespace AsterFlorets {

    class AsterFlorets_EXPORT AsterDrawer : public QWidget {
        Q_OBJECT
        Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
        Q_PROPERTY(int drawerWidth READ drawerWidth WRITE setDrawerWidth)
        Q_PROPERTY(QString title READ title WRITE setTitle)
        Q_PROPERTY(Position position READ position WRITE setPosition)

    public:
        enum class Position {
            Right,
            Left,
            Top,
            Bottom
        };
        Q_ENUM(Position)

        explicit AsterDrawer(QWidget* parent = nullptr); // Parent should be the window
        ~AsterDrawer() override;

        void open();
        void close();
        void toggle();

        void setContent(QWidget* content);
        QWidget* contentWidget() const;

        void setTitle(const QString& title);
        QString title() const;

        void setPosition(Position pos);
        Position position() const;
        
        void setMaskClosable(bool maskClosable);
        bool isMaskClosable() const;

        // Custom width for Left/Right, height for Top/Bottom is usually content based or fixed
        void setDrawerWidth(int width);
        int drawerWidth() const;

    signals:
        void opened();
        void closed();

    protected:
        void paintEvent(QPaintEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;
        bool eventFilter(QObject* watched, QEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;

    private:
        void updateLayout();
        void onAnimationFinished();

        QWidget* m_container; // The actual drawer panel
        QVBoxLayout* m_containerLayout;
        QWidget* m_content = nullptr;
        
        // Header
        QWidget* m_header;
        
        // Mask
        QWidget* m_mask; // We draw mask in paintEvent or use a separate widget. paintEvent is easier for alpha.

        QPropertyAnimation* m_anim;
        bool m_isOpen = false;
        QString m_title;
        Position m_position = Position::Right;
        bool m_maskClosable = true;
        int m_drawerSize = 378; // Default width

        qreal m_maskOpacity = 0.0;
        int m_offset = 0; // Current animation offset
    };

}
