#include "AsterMenu.h"
#include "AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include <QPropertyAnimation>

namespace AsterFlorets {

    // ========================================================================
    // AsterMenuItem
    // ========================================================================

    AsterMenuItem::AsterMenuItem(const QString& key, const QString& title, const QIcon& icon, QWidget* parent)
        : QWidget(parent)
        , m_key(key)
        , m_title(title)
        , m_icon(icon)
    {
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        setFixedHeight(50); // Standard item height
        setCursor(Qt::PointingHandCursor);

        m_hoverAnim = new QPropertyAnimation(this, "hoverProgress", this);
        m_hoverAnim->setDuration(150);
        m_hoverAnim->setEasingCurve(QEasingCurve::OutQuad);
    }

    AsterMenuItem::AsterMenuItem(QWidget* parent)
        : AsterMenuItem("", "", QIcon(), parent)
    {
    }

    QString AsterMenuItem::key() const {
        return m_key;
    }

    void AsterMenuItem::setKey(const QString& key) {
        m_key = key;
    }

    QString AsterMenuItem::title() const {
        return m_title;
    }

    void AsterMenuItem::setTitle(const QString& title) {
        m_title = title;
        update();
    }

    QIcon AsterMenuItem::icon() const {
        return m_icon;
    }

    void AsterMenuItem::setIcon(const QIcon& icon) {
        m_icon = icon;
        update();
    }

    bool AsterMenuItem::isSelected() const {
        return m_selected;
    }

    void AsterMenuItem::setSelected(bool selected) {
        if (m_selected != selected) {
            m_selected = selected;
            update();
        }
    }

    void AsterMenuItem::setHoverProgress(qreal p) {
        m_hoverProgress = p;
        update();
    }

    void AsterMenuItem::enterEvent(QEnterEvent* event) {
        QWidget::enterEvent(event);
        m_hoverAnim->stop();
        m_hoverAnim->setEndValue(1.0);
        m_hoverAnim->start();
    }

    void AsterMenuItem::leaveEvent(QEvent* event) {
        QWidget::leaveEvent(event);
        m_hoverAnim->stop();
        m_hoverAnim->setEndValue(0.0);
        m_hoverAnim->start();
    }

    void AsterMenuItem::mousePressEvent(QMouseEvent* event) {
        QWidget::mousePressEvent(event);
        emit clicked(m_key);
    }

    void AsterMenuItem::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        auto theme = AsterTheme::instance();
        
        // 1. Determine State & Colors
        QColor primary = theme->color(AsterTheme::ColorRole::Primary);
        QColor textNorm = theme->color(AsterTheme::ColorRole::Text);
        
        // Check Parent Mode
        bool isVertical = true;
        if (auto menu = qobject_cast<AsterMenu*>(parentWidget())) {
            isVertical = (menu->mode() == AsterMenu::Mode::Vertical);
        }

        // Layout Constants: Compact (Minimal margins)
        // User requested: "Don't have margin by default" -> 0 or 2px
        // Let's use 2px horizontal margin to prevent touching edge, 
        // effectively making "pill" very wide.
        int radius = 6;
        int marginX = isVertical ? 2 : 2; 
        int marginY = isVertical ? 2 : 2;
        
        QRect r = rect();
        QRect bgRect = r.adjusted(marginX, marginY, -marginX, -marginY);

        bool isDarkBg = false; 

        // 2. Draw Background
        if (m_selected) {
            // -- Selected State: Modern Gradient Pill --
            isDarkBg = true;
            
            // A. Shadow (Simulated) - reduced offset for compact look
            if (isVertical) {
                QColor shadowColor = primary;
                shadowColor.setAlpha(40);
                QRect shadowRect = bgRect.translated(0, 2);
                painter.setPen(Qt::NoPen);
                painter.setBrush(shadowColor);
                painter.drawRoundedRect(shadowRect, radius, radius);
            }

            // B. Gradient Background
            QLinearGradient gradient(bgRect.topLeft(), bgRect.bottomRight());
            gradient.setColorAt(0.0, primary.lighter(110));
            gradient.setColorAt(1.0, primary);
            
            painter.setBrush(gradient);
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(bgRect, radius, radius);

        } else if (m_hoverProgress > 0.0) {
             // -- Hover State: Animated Opacity --
             QColor hoverBg = theme->color(AsterTheme::ColorRole::Text);
             // Max opacity 0.08
             hoverBg.setAlphaF(0.08 * m_hoverProgress);
             
             painter.setBrush(hoverBg);
             painter.setPen(Qt::NoPen);
             painter.drawRoundedRect(bgRect, radius, radius);
        }

        // 3. Draw Content
        QColor textColor = (m_selected && isDarkBg) ? Qt::white : textNorm;
        QColor iconColor = textColor; // Icon auto-tints to match text

        // Content Position
        // Padding inside the pill, slightly reduced for compact
        int contentPadding = 10; 
        int xPos = bgRect.left() + contentPadding;
        int const contentH = height();
        // int const availWidth = bgRect.width() - (contentPadding * 2);

        // A. Icon
        if (!m_icon.isNull()) {
            int iconSize = 16; // Slightly smaller icon
            QRect iconRect(xPos, (contentH - iconSize) / 2, iconSize, iconSize);
            
            QPixmap pixmap = m_icon.pixmap(iconSize, iconSize);
            QPainter iconPainter(&pixmap);
            iconPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            iconPainter.fillRect(pixmap.rect(), iconColor);
            iconPainter.end();
            
            painter.drawPixmap(iconRect, pixmap);
            
            xPos += iconSize + 8; // Tighter gap
        }

        // B. Text
        painter.setPen(textColor);
        QFont f = theme->font(AsterTheme::Size::Default);
        if (m_selected) f.setBold(true);
        painter.setFont(f);
        
        int textWidth = bgRect.right() - xPos - contentPadding;
        if (textWidth > 0) {
            QRect textRect(xPos, 0, textWidth, contentH);
            painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_title);
        }
    }


    // ========================================================================
    // AsterMenu
    // ========================================================================

    AsterMenu::AsterMenu(QWidget* parent)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Expand to fill container
        updateLayout();
    }

    AsterMenu::~AsterMenu() = default;

    AsterMenu::Mode AsterMenu::mode() const {
        return m_mode;
    }

    void AsterMenu::setMode(Mode mode) {
        if (m_mode != mode) {
            m_mode = mode;
            updateLayout();
        }
    }

    QString AsterMenu::selectedKey() const {
        return m_selectedKey;
    }

    void AsterMenu::setSelectedKey(const QString& key) {
        if (m_selectedKey != key) {
            m_selectedKey = key;
            
            // Update items
            for (auto item : m_items) {
                item->setSelected(item->key() == m_selectedKey);
            }
            
            emit itemClicked(m_selectedKey);
        }
    }

    void AsterMenu::addItem(AsterMenuItem* item) {
        if (!item) return;

        item->setParent(this);
        m_items.append(item);
        
        // Connect signal
        connect(item, &AsterMenuItem::clicked, this, &AsterMenu::handleItemClick);

        // Initial state
        if (!m_selectedKey.isEmpty() && item->key() == m_selectedKey) {
            item->setSelected(true);
        }

        // If we simply addWidget, we need to respect the stretch structure.
        // It's safer to full rebuild layout when adding items dynamically to keep stretch order.
        // But for performance, if we append to end (Top Align), we must insert BEFORE the last stretch.
        
        // For simplicity now, let's just trigger updateLayout() if it's already created.
        if (m_mainLayout) {
             updateLayout();
        }
    }

    AsterMenuItem* AsterMenu::addItem(const QString& key, const QString& title, const QIcon& icon) {
        AsterMenuItem* item = new AsterMenuItem(key, title, icon, this);
        addItem(item);
        return item;
    }

    void AsterMenu::handleItemClick(const QString& key) {
        setSelectedKey(key);
    }

    void AsterMenu::setAlignment(Qt::Alignment alignment) {
        if (m_alignment != alignment) {
            m_alignment = alignment;
            updateLayout();
        }
    }

    Qt::Alignment AsterMenu::alignment() const {
        return m_alignment;
    }

    void AsterMenu::updateLayout() {
        if (m_mainLayout) {
            delete m_mainLayout; 
        }

        // Layout Constants: Tighter for integrated look
        if (m_mode == Mode::Vertical) {
            m_mainLayout = new QVBoxLayout(this);
            // User requested no margin between menu and items
            m_mainLayout->setContentsMargins(0, 8, 0, 8); // Top/Bottom padding inside the "Card"
            m_mainLayout->setSpacing(2);
        } else {
            m_mainLayout = new QHBoxLayout(this);
            m_mainLayout->setContentsMargins(8, 0, 8, 0);
            m_mainLayout->setSpacing(2);
        }

        // Apply Alignment by using stretches
        // Top/Left Stretch if Bottom/Right aligned
        if (m_alignment & (Qt::AlignBottom | Qt::AlignRight)) {
             m_mainLayout->addStretch();
        }
        else if (m_alignment & Qt::AlignVCenter || m_alignment & Qt::AlignHCenter) {
             m_mainLayout->addStretch();
        }

        // Re-add all items
        for (auto item : m_items) {
            m_mainLayout->addWidget(item);
        }

        // Bottom/Right Stretch if Top/Left aligned
        if (m_alignment & (Qt::AlignTop | Qt::AlignLeft)) {
             m_mainLayout->addStretch();
        }
        else if (m_alignment & Qt::AlignVCenter || m_alignment & Qt::AlignHCenter) {
             m_mainLayout->addStretch();
        }
    }

    void AsterMenu::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        // Paint Menu Container Background (Card-like)
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        auto theme = AsterTheme::instance();
        
        // Menu Background: Surface (White) vs Layout Background (Gray)
        QColor bgColor = theme->color(AsterTheme::ColorRole::Surface);
        
        // Optional: Border
        QColor borderColor = theme->color(AsterTheme::ColorRole::Border);
        borderColor.setAlpha(100);

        QRect r = rect();
        int radius = theme->borderRadius(AsterTheme::Size::Large);

        // Draw Shadow (Simple simulation)
        // painter.setPen(Qt::NoPen);
        // painter.setBrush(QColor(0,0,0,10));
        // painter.drawRoundedRect(r.translated(0, 2), radius, radius);

        // Draw Background
        painter.setPen(QPen(borderColor, 1));
        painter.setBrush(bgColor);
        painter.drawRoundedRect(r.adjusted(1,1,-1,-1), radius, radius);
    }

}
