#include "AsterDrawer.h"
#include "AsterTheme.h"
#include "AsterTitle.h"
#include "AsterButton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QScreen>
#include <QWindow>
#include <QDebug>

namespace AsterFlorets {

    AsterDrawer::AsterDrawer(QWidget* parent)
        : QWidget(parent)
    {
        // Drawer covers the whole parent area (usually window) acting as modal overlay
        // If parent is null, it might need to act as a window, but typically Drawer is attached to a main window.
        hide();
        if (parent) {
            resize(parent->size());
            setAttribute(Qt::WA_DeleteOnClose, false);
        }
        
        // Transparent background for the widget itself, we paint mask manully
        setAttribute(Qt::WA_TranslucentBackground);
        
        // The sliding container
        m_container = new QWidget(this);
        m_container->setAutoFillBackground(true);
        QPalette p = m_container->palette();
        p.setColor(QPalette::Window, AsterTheme::instance()->color(AsterTheme::ColorRole::Background));
        m_container->setPalette(p);
        
        // Shadow
        // ... (Shadow implementation simplified)

        // Layout for container
        auto* mainVLayout = new QVBoxLayout(m_container);
        mainVLayout->setContentsMargins(0, 0, 0, 0);
        mainVLayout->setSpacing(0);

        // Header
        m_header = new QWidget(m_container);
        auto* headerLayout = new QHBoxLayout(m_header);
        headerLayout->setContentsMargins(24, 16, 24, 16);
        AsterTitle* titleLabel = new AsterTitle("Drawer Title", 1); 
        // Need to link titleLabel to m_title property really, simplified for now
        // And a close button
        AsterButton* closeBtn = new AsterButton("X"); 
        closeBtn->setType(AsterButton::Type::Text);
        connect(closeBtn, &AsterButton::clicked, this, &AsterDrawer::close);

        headerLayout->addWidget(titleLabel);
        headerLayout->addStretch();
        headerLayout->addWidget(closeBtn);
        
        mainVLayout->addWidget(m_header);

        // Content Area
        m_containerLayout = new QVBoxLayout();
        m_containerLayout->setContentsMargins(24, 24, 24, 24);
        mainVLayout->addLayout(m_containerLayout);
        mainVLayout->addStretch();

        // Animation
        m_anim = new QPropertyAnimation(m_container, "geometry", this);
        m_anim->setDuration(300);
        m_anim->setEasingCurve(QEasingCurve::OutCubic);
        connect(m_anim, &QPropertyAnimation::finished, this, &AsterDrawer::onAnimationFinished);

        if (parent) {
            parent->installEventFilter(this);
        }
    }

    AsterDrawer::~AsterDrawer() {
    }

    void AsterDrawer::setTitle(const QString& title) {
        m_title = title;
        if (m_header) {
            // Find the label and update text (Simplified access)
            if (auto l = m_header->findChild<AsterTitle*>()) {
                l->setText(title);
            }
        }
    }

    QString AsterDrawer::title() const {
        return m_title;
    }

    void AsterDrawer::setContent(QWidget* content) {
        if (m_content) {
            m_containerLayout->removeWidget(m_content);
            m_content->hide(); 
            // Optional: delete old content? Usually setContent implies ownership transfer or replacement
        }
        m_content = content;
        if (m_content) {
            m_containerLayout->insertWidget(0, m_content);
            m_content->show();
        }
    }

    QWidget* AsterDrawer::contentWidget() const {
        return m_content;
    }

    void AsterDrawer::open() {
        if (m_isOpen) return;
        m_isOpen = true;
        
        // Ensure we cover the parent
        if (parentWidget()) {
            resize(parentWidget()->size());
            raise();
        }
        show();

        // Calculate start and end geometries
        QRect endRect = rect(); // Base
        QRect startRect = rect();

        if (m_position == Position::Right) {
            endRect.setLeft(width() - m_drawerSize);
            startRect.setLeft(width()); // Outside right
        } else if (m_position == Position::Left) {
            endRect.setWidth(m_drawerSize);
            startRect.setRight(0); // Outside left
        } else if (m_position == Position::Top) {
             endRect.setHeight(m_drawerSize);
             startRect.setBottom(0);
        } else {
             endRect.setTop(height() - m_drawerSize);
             startRect.setTop(height());
        }

        m_container->setGeometry(startRect);
        m_anim->setStartValue(startRect);
        m_anim->setEndValue(endRect);
        m_anim->start();
        
        emit opened();
    }

    void AsterDrawer::close() {
        if (!m_isOpen) return;
        
        QRect startRect = m_container->geometry();
        QRect endRect = startRect;

        if (m_position == Position::Right) {
            endRect.setLeft(width());
        } else if (m_position == Position::Left) {
            endRect.setRight(0);
        } else if (m_position == Position::Top) {
            endRect.setBottom(0);
        } else {
            endRect.setTop(height());
        }

        m_anim->setStartValue(startRect);
        m_anim->setEndValue(endRect);
        m_anim->start();

        m_isOpen = false;
        // Don't emit closed() yet, wait for anim
    }

    void AsterDrawer::onAnimationFinished() {
        if (!m_isOpen) {
            hide();
            emit closed();
        }
    }

    void AsterDrawer::toggle() {
        if (m_isOpen) close();
        else open();
    }

    void AsterDrawer::setPosition(Position pos) {
        m_position = pos;
    }

    AsterDrawer::Position AsterDrawer::position() const {
        return m_position;
    }
    
    void AsterDrawer::setMaskClosable(bool maskClosable) {
        m_maskClosable = maskClosable;
    }

    bool AsterDrawer::isMaskClosable() const {
        return m_maskClosable;
    }
    
    void AsterDrawer::setDrawerWidth(int width) {
        m_drawerSize = width;
    }

    int AsterDrawer::drawerWidth() const {
        return m_drawerSize;
    }
    
    bool AsterDrawer::eventFilter(QObject* watched, QEvent* event) {
        if (watched == parentWidget() && event->type() == QEvent::Resize) {
            resize(parentWidget()->size());
            if (m_isOpen) {
                // Adjust container geometry if fully open
                 QRect r = rect();
                 if (m_position == Position::Right) r.setLeft(width() - m_drawerSize);
                 else if (m_position == Position::Left) r.setWidth(m_drawerSize);
                 else if (m_position == Position::Top) r.setHeight(m_drawerSize);
                 else r.setTop(height() - m_drawerSize);
                 m_container->setGeometry(r);
            }
        }
        return QWidget::eventFilter(watched, event);
    }
    
    void AsterDrawer::mousePressEvent(QMouseEvent* event) {
        if (m_maskClosable && !m_container->geometry().contains(event->pos())) {
            close();
        }
    }

    void AsterDrawer::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        if (!m_isOpen && m_anim->state() == QAbstractAnimation::Stopped) return;

        QPainter p(this);
        // Paint Mask
        QColor maskColor = Qt::black;
        // Opacity should ideally be animated too, but fixed for now
        maskColor.setAlphaF(0.45); 
        p.fillRect(rect(), maskColor);
    }
    
    void AsterDrawer::resizeEvent(QResizeEvent* event) {
        QWidget::resizeEvent(event);
        if (m_isOpen && m_anim->state() == QAbstractAnimation::Stopped) {
            // Re-layout container
             QRect r = rect();
             if (m_position == Position::Right) r.setLeft(width() - m_drawerSize);
             else if (m_position == Position::Left) r.setWidth(m_drawerSize);
             else if (m_position == Position::Top) r.setHeight(m_drawerSize);
             else r.setTop(height() - m_drawerSize);
             m_container->setGeometry(r);
        }
    }

}
