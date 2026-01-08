#include "AsterUI/AsterLayout.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>
#include <QEvent>
#include <QStyleOption>

namespace AsterUI {

// Helper to standardise paint event (support stylesheet + palette fallback)
static void paintLayoutWidget(QWidget* widget) {
    QStyleOption opt;
    opt.initFrom(widget);
    QPainter p(widget);
    widget->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, widget);
}

// Helper to set default background from theme
static void initLayoutWidget(QWidget* widget, AsterTheme::ColorRole role) {
    widget->setAutoFillBackground(true);
    QPalette pal = widget->palette();
    pal.setColor(QPalette::Window, AsterTheme::instance()->color(role));
    widget->setPalette(pal);

    // Optional: Connect to theme change to update palette
    // This requires the widget to be QObject, passing 'this' capture is tricky with static.
    // So we'll skip dynamic update for now or implement inside classes.
}

// --- AsterHeader ---
AsterHeader::AsterHeader(QWidget *parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    initLayoutWidget(this, AsterTheme::ColorRole::LayoutHeader);
}

void AsterHeader::paintEvent(QPaintEvent *) {
    paintLayoutWidget(this);
}

// --- AsterFooter ---
AsterFooter::AsterFooter(QWidget *parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    initLayoutWidget(this, AsterTheme::ColorRole::LayoutFooter);
}

void AsterFooter::paintEvent(QPaintEvent *) {
    paintLayoutWidget(this);
}

// --- AsterContent ---
AsterContent::AsterContent(QWidget *parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // Content defaults to transparent/inherited usually, but lets set default white per theme
    // Actually, usually Content area is transparent to show Layout BG, or White card.
    // Theme says LayoutContent is White.
    initLayoutWidget(this, AsterTheme::ColorRole::LayoutContent);
}

void AsterContent::paintEvent(QPaintEvent *) {
    paintLayoutWidget(this);
}

// --- AsterSider ---
AsterSider::AsterSider(QWidget *parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    initLayoutWidget(this, AsterTheme::ColorRole::LayoutSider);
}

void AsterSider::paintEvent(QPaintEvent *) {
    paintLayoutWidget(this);
}

// --- AsterLayout ---
AsterLayout::AsterLayout(QWidget *parent) : QWidget(parent) {
    init();
}

AsterLayout::~AsterLayout() {
}

void AsterLayout::init() {
    initLayoutWidget(this, AsterTheme::ColorRole::Background);

    m_layout = new QGridLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
}

void AsterLayout::setHeader(QWidget *widget, int height) {
    if (m_header) {
        m_layout->removeWidget(m_header);
        m_header->hide();
        m_header->setParent(nullptr);
    }
    m_header = widget;
    m_headerHeight = height;
    if (m_header) {
        if (m_headerHeight > 0) m_header->setFixedHeight(m_headerHeight);
        else m_header->setMinimumHeight(0);
    }
    updateLayout();
}

void AsterLayout::setSider(QWidget *widget, int width) {
    if (m_sider) {
        m_layout->removeWidget(m_sider);
        m_sider->hide();
        m_sider->setParent(nullptr);
    }
    m_sider = widget;
    m_siderWidth = width;
    if (m_sider) {
        if (m_siderWidth > 0) m_sider->setFixedWidth(m_siderWidth);
        else m_sider->setMinimumWidth(0); 
    }
    updateLayout();
}

void AsterLayout::setContent(QWidget *widget) {
    if (m_content) {
        m_layout->removeWidget(m_content);
        m_content->hide();
        m_content->setParent(nullptr);
    }
    m_content = widget;
    updateLayout();
}

void AsterLayout::setFooter(QWidget *widget, int height) {
    if (m_footer) {
        m_layout->removeWidget(m_footer);
        m_footer->hide();
        m_footer->setParent(nullptr); 
    }
    m_footer = widget;
    m_footerHeight = height;
    if (m_footer) {
         if (m_footerHeight > 0) m_footer->setFixedHeight(m_footerHeight);
         else m_footer->setMinimumHeight(0);
    }
    updateLayout();
}

void AsterLayout::setHeaderHeight(int height) {
    m_headerHeight = height;
    if (m_header) {
        if (m_headerHeight > 0) m_header->setFixedHeight(m_headerHeight);
        else {
             m_header->setMinimumHeight(0);
             m_header->setMaximumHeight(QWIDGETSIZE_MAX);
        }
    }
}

void AsterLayout::setSiderWidth(int width) {
    m_siderWidth = width;
    if (m_sider) {
        if (m_siderWidth > 0) m_sider->setFixedWidth(m_siderWidth);
        else {
            m_sider->setMinimumWidth(0);
            m_sider->setMaximumWidth(QWIDGETSIZE_MAX);
        }
    }
}

void AsterLayout::setFooterHeight(int height) {
    m_footerHeight = height;
    if (m_footer) {
        if (m_footerHeight > 0) m_footer->setFixedHeight(m_footerHeight);
        else {
            m_footer->setMinimumHeight(0);
            m_footer->setMaximumHeight(QWIDGETSIZE_MAX);
        }
    }
}

void AsterLayout::setMode(Mode mode) {
    if (m_mode == mode) return;
    m_mode = mode;
    updateLayout();
}

AsterLayout::Mode AsterLayout::mode() const {
    return m_mode;
}

void AsterLayout::setSpacing(int spacing) {
    m_layout->setSpacing(spacing);
}

void AsterLayout::paintEvent(QPaintEvent *) {
    paintLayoutWidget(this);
}

void AsterLayout::updateLayout() {
    for (int i = m_layout->count() - 1; i >= 0; --i) {
        m_layout->takeAt(i);
    }

    if (m_mode == Mode::HeaderFirst) {
        if (m_header) m_layout->addWidget(m_header, 0, 0, 1, 2);
        
        int contentRow = 1;
        if (m_sider) m_layout->addWidget(m_sider, contentRow, 0);
        if (m_content) m_layout->addWidget(m_content, contentRow, 1);
        else if (!m_sider && m_content) m_layout->addWidget(m_content, contentRow, 0, 1, 2); 

        if (m_footer) m_layout->addWidget(m_footer, 2, 0, 1, 2);

        m_layout->setColumnStretch(0, 0);
        m_layout->setColumnStretch(1, 1);
        m_layout->setRowStretch(0, 0);
        m_layout->setRowStretch(1, 1);
        m_layout->setRowStretch(2, 0); 
    } else {
        // SiderFirst
        if (m_sider) m_layout->addWidget(m_sider, 0, 0, 3, 1);
        
        if (m_header) m_layout->addWidget(m_header, 0, 1);
        if (m_content) m_layout->addWidget(m_content, 1, 1);
        if (m_footer) m_layout->addWidget(m_footer, 2, 1);

        m_layout->setColumnStretch(0, 0);
        m_layout->setColumnStretch(1, 1);
        m_layout->setRowStretch(0, 0);
        m_layout->setRowStretch(1, 1); 
        m_layout->setRowStretch(2, 0);
    }
}

}
