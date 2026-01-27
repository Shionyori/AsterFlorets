#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QList>
#include <QIcon>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>

namespace AsterFlorets {

    class AsterMenu;

    // --- AsterMenuItem ---
    class AsterFlorets_EXPORT AsterMenuItem : public QWidget {
        Q_OBJECT
        Q_PROPERTY(bool selected READ isSelected WRITE setSelected)
        Q_PROPERTY(qreal hoverProgress READ hoverProgress WRITE setHoverProgress)

    public:
        explicit AsterMenuItem(const QString& key, const QString& title, const QIcon& icon = QIcon(), QWidget* parent = nullptr);
        explicit AsterMenuItem(QWidget* parent = nullptr); // Default constructor for flexibility

        QString key() const;
        void setKey(const QString& key);

        QString title() const;
        void setTitle(const QString& title);

        QIcon icon() const;
        void setIcon(const QIcon& icon);

        bool isSelected() const;
        void setSelected(bool selected);

        qreal hoverProgress() const { return m_hoverProgress; }
        void setHoverProgress(qreal p);

    signals:
        void clicked(const QString& key);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;

    private:
        QString m_key;
        QString m_title;
        QIcon m_icon;
        bool m_selected = false;
        
        qreal m_hoverProgress = 0.0;
        QPropertyAnimation* m_hoverAnim = nullptr;
        
        friend class AsterMenu;
    };

    // --- AsterMenu ---
    class AsterFlorets_EXPORT AsterMenu : public QWidget {
        Q_OBJECT
        Q_PROPERTY(Mode mode READ mode WRITE setMode)
        Q_PROPERTY(QString selectedKey READ selectedKey WRITE setSelectedKey)

    public:
        enum class Mode {
            Vertical,   // Normal vertical list
            Horizontal, // Top bar
        };
        Q_ENUM(Mode)

        explicit AsterMenu(QWidget* parent = nullptr);
        ~AsterMenu() override;

        Mode mode() const;
        void setMode(Mode mode);

        QString selectedKey() const;
        void setSelectedKey(const QString& key);

        // Alignment for items inside the menu (e.g. AlignTop, AlignBottom, AlignCenter)
        void setAlignment(Qt::Alignment alignment);
        Qt::Alignment alignment() const;

        // Adds an existing item instance
        void addItem(AsterMenuItem* item);
        
        // Convenience: creates and adds an item
        AsterMenuItem* addItem(const QString& key, const QString& title, const QIcon& icon = QIcon());

    signals:
        void itemClicked(const QString& key);

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        void handleItemClick(const QString& key);
        void updateLayout();

    private:
        Mode m_mode = Mode::Vertical;
        Qt::Alignment m_alignment = Qt::AlignTop; // Default to Top
        QString m_selectedKey;
        QList<AsterMenuItem*> m_items;
        QBoxLayout* m_mainLayout = nullptr;
    };

}
