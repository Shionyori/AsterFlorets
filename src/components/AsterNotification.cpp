#include "AsterNotification.h"
#include "AsterTheme.h"
#include "AsterIcon.h"
#include "AsterButton.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QApplication>
#include <QScreen>
#include <QStyle>
#include <QMutex>

namespace AsterFlorets {

    class AsterNotificationWidget;
    static QList<AsterNotificationWidget*> g_activeNotifications;

    class AsterNotificationWidget : public QWidget {
    public:
        AsterNotificationWidget(const QString& title, const QString& message, AsterNotification::Type type, int duration, QWidget* parent)
            : QWidget(nullptr) // Always top-level tool
        {
            if (parent) {
                QWidget* top = parent->window();
                if (top) setParent(top, Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
            } else {
                setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::NoDropShadowWindowHint);
            }
            setAttribute(Qt::WA_TranslucentBackground);
            setAttribute(Qt::WA_ShowWithoutActivating);
            setAttribute(Qt::WA_DeleteOnClose);

            // Fixed width for notifications
            setFixedWidth(384); 

            // Main Layout
            auto mainLayout = new QHBoxLayout(this);
            mainLayout->setContentsMargins(10, 10, 10, 10); // Shadow margin

            // Content Container (Card)
            auto contentWidget = new QWidget(this);
            contentWidget->setObjectName("NotifyContent");
            auto bg = AsterTheme::instance()->colorString(AsterTheme::ColorRole::Surface);
            contentWidget->setStyleSheet(QString(
                "#NotifyContent { background-color: %1; border-radius: 8px; border: 1px solid %2; }"
            ).arg(bg).arg(AsterTheme::instance()->colorString(AsterTheme::ColorRole::Divider)));

            auto shadow = new QGraphicsDropShadowEffect(this);
            shadow->setBlurRadius(24);
            shadow->setOffset(0, 8);
            shadow->setColor(QColor(0, 0, 0, 20));
            contentWidget->setGraphicsEffect(shadow);

            mainLayout->addWidget(contentWidget);

            // Inner Layout
            auto innerLayout = new QHBoxLayout(contentWidget);
            innerLayout->setContentsMargins(16, 16, 16, 16);
            innerLayout->setSpacing(16);
            innerLayout->setAlignment(Qt::AlignTop);

            // Icon
            auto iconWidget = new AsterIcon(getIcon(type), contentWidget);
            iconWidget->setColor(getIconColor(type));
            iconWidget->setFixedSize(24, 24);
            innerLayout->addWidget(iconWidget);

            // Text Group
            auto textLayout = new QVBoxLayout();
            textLayout->setSpacing(4);
            
            auto titleLabel = new QLabel(title, contentWidget);
            titleLabel->setStyleSheet(QString("font-weight: bold; font-size: 14px; color: %1;").arg(
                AsterTheme::instance()->colorString(AsterTheme::ColorRole::Text)
            ));
            titleLabel->setWordWrap(true);
            textLayout->addWidget(titleLabel);

            if (!message.isEmpty()) {
                auto msgLabel = new QLabel(message, contentWidget);
                msgLabel->setStyleSheet(QString("font-size: 13px; color: %1;").arg(
                    AsterTheme::instance()->colorString(AsterTheme::ColorRole::TextSecondary)
                ));
                msgLabel->setWordWrap(true);
                textLayout->addWidget(msgLabel);
            }
            
            innerLayout->addLayout(textLayout, 1); // Expand text area

            // Close Button (Top-Right)
            auto closeBtn = new AsterButton(contentWidget);
            closeBtn->setFixedSize(20, 20);
            closeBtn->setText("×"); // Simple close symbol
            closeBtn->setType(AsterButton::Type::Text);
            // closeBtn->setIcon(window()->style()->standardIcon(QStyle::SP_TitleBarCloseButton)); // Optional: Use icon instead of text
            closeBtn->setStyleSheet("background: transparent; border: none; font-size: 16px; color: #999;");
            
            // Align close button to top-right
            // Using a VLayout for the button to stick to top
            auto btnVLayout = new QVBoxLayout(); 
            btnVLayout->setContentsMargins(0, 0, 0, 0);
            btnVLayout->addWidget(closeBtn);
            btnVLayout->addStretch();
            
            innerLayout->addLayout(btnVLayout);

            connect(closeBtn, &AsterButton::clicked, this, &AsterNotificationWidget::closeAnim);
            
            // Adjust size to fit content
            adjustSize();

            // Auto close
            if (duration > 0) {
                QTimer::singleShot(duration, this, &AsterNotificationWidget::closeAnim);
            }

            connect(this, &QObject::destroyed, [this](){
                g_activeNotifications.removeAll(this);
                // Defer reposition to allow list update
                QTimer::singleShot(0, [](){ AsterNotificationWidget::repositionAll(); });
            });
        }
        
        // Static helper to calculate target geometry
        static QPoint calculateTargetPos(int index, int width) {
            int rightMargin = 24;
            int topMargin = 24;
            int spacing = 16;
            
            QRect screenGeom;
            // Use parent of first notification if available, else Active Window, else Screen
            // Note: If using multiple windows, notifications should probably stick to the window that triggered them.
            // But g_activeNotifications is global. This is a simplification.
            QWidget* refWidget = nullptr;
            if (!g_activeNotifications.isEmpty() && g_activeNotifications.first()) {
                 refWidget = g_activeNotifications.first()->parentWidget();
            }
            
            if (refWidget && refWidget->window()) {
                 // Map window geometry to global roughly?
                 // window->geometry() is usually global for top-level windows.
                 screenGeom = refWidget->window()->geometry();
            } else if (auto active = QApplication::activeWindow()) {
                screenGeom = active->geometry();
            } else {
                screenGeom = QGuiApplication::primaryScreen()->availableGeometry();
            }

            int currentY = screenGeom.top() + topMargin;
            
            // Calculate Y based on preceding notifications
            for (int i=0; i < index && i < g_activeNotifications.count(); ++i) {
                auto w = g_activeNotifications[i];
                if (w) currentY += w->height() + spacing - 20; 
            }
            
            int x = screenGeom.right() - width - rightMargin;
            return QPoint(x, currentY);
        }

        static void repositionAll() {
            int i = 0;
            for (auto notify : g_activeNotifications) {
                if (!notify) continue;
                QPoint target = calculateTargetPos(i++, notify->width());
                
                // Only animate if significant change or if not animating?
                // For simplicity, just animate always
                auto anim = new QPropertyAnimation(notify, "pos");
                anim->setStartValue(notify->pos());
                anim->setEndValue(target);
                anim->setDuration(250);
                anim->setEasingCurve(QEasingCurve::OutQuad);
                anim->start(QAbstractAnimation::DeleteWhenStopped);
            }
        }

        void showAnim() {
             // Initial setup was done in show() now
             // We just animate opacity and slight X-slide
             
            setWindowOpacity(0.0);
            
            // Target is current pos (set by show())
            QPoint targetPos = pos();
            // Start slightly to the right
            move(targetPos.x() + 40, targetPos.y());
            show();
            
            auto group = new QParallelAnimationGroup(this);
            
            auto opacityAnim = new QPropertyAnimation(this, "windowOpacity");
            opacityAnim->setStartValue(0.0);
            opacityAnim->setEndValue(1.0);
            opacityAnim->setDuration(300);
            opacityAnim->setEasingCurve(QEasingCurve::OutQuad);
            group->addAnimation(opacityAnim);

            auto posAnim = new QPropertyAnimation(this, "pos");
            posAnim->setStartValue(pos());
            posAnim->setEndValue(targetPos);
            posAnim->setDuration(300);
            posAnim->setEasingCurve(QEasingCurve::OutBack);
            group->addAnimation(posAnim);

            group->start(QAbstractAnimation::DeleteWhenStopped);
        }

        void closeAnim() {
            auto anim = new QPropertyAnimation(this, "windowOpacity");
            anim->setStartValue(windowOpacity());
            anim->setEndValue(0.0);
            anim->setDuration(250);
            connect(anim, &QPropertyAnimation::finished, this, &QWidget::close);
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        }
        
    private:
        QIcon getIcon(AsterNotification::Type type) {
            QStyle::StandardPixmap sp = QStyle::SP_MessageBoxInformation;
            switch(type) {
                case AsterNotification::Type::Info: sp = QStyle::SP_MessageBoxInformation; break;
                case AsterNotification::Type::Success: sp = QStyle::SP_DialogApplyButton; break; 
                case AsterNotification::Type::Warning: sp = QStyle::SP_MessageBoxWarning; break;
                case AsterNotification::Type::Error: sp = QStyle::SP_MessageBoxCritical; break;
            }
             return QApplication::style()->standardIcon(sp);
        }
        
        QColor getIconColor(AsterNotification::Type type) {
            auto theme = AsterTheme::instance();
            using T = AsterNotification::Type;
            switch(type) {
                case T::Info: return theme->color(AsterTheme::ColorRole::Primary);
                case T::Success: return theme->color(AsterTheme::ColorRole::Success);
                case T::Warning: return theme->color(AsterTheme::ColorRole::Warning);
                case T::Error: return theme->color(AsterTheme::ColorRole::Error);
            }
            return theme->color(AsterTheme::ColorRole::Primary);
        }
    };

    void AsterNotification::show(const QString& title, const QString& message, Type type, int duration, QWidget* parent) {
        auto notify = new AsterNotificationWidget(title, message, type, duration, parent);
        
        // Setup initial position (invisible)
        // Add to list first to calculate correct stack position
        g_activeNotifications.append(notify);
        
        // Calculate Target
        QPoint target = AsterNotificationWidget::calculateTargetPos(g_activeNotifications.count()-1, notify->width());
        
        // Move to target immediately (but invisible)
        notify->move(target);
        
        // Force reposition others if needed? No, just the new one needs placement primarily.
        // Others are already placed. RepositionAll might be needed if window moved? 
        // Let's call repositionAll later if size changes, but for now just placement.
        
        // Trigger show animation (will slide in from targetX + offset)
        notify->showAnim();
    }

    void AsterNotification::info(const QString& title, const QString& message, int duration, QWidget* parent) {
        show(title, message, Type::Info, duration, parent);
    }
    void AsterNotification::success(const QString& title, const QString& message, int duration, QWidget* parent) {
        show(title, message, Type::Success, duration, parent);
    }
    void AsterNotification::warning(const QString& title, const QString& message, int duration, QWidget* parent) {
        show(title, message, Type::Warning, duration, parent);
    }
    void AsterNotification::error(const QString& title, const QString& message, int duration, QWidget* parent) {
        show(title, message, Type::Error, duration, parent);
    }

}
