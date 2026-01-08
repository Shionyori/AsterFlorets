#include "AsterUI/AsterMessage.h"
#include "AsterUI/AsterTheme.h"
#include "AsterUI/AsterIcon.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QApplication>
#include <QScreen>
#include <QStyle>
#include <QQueue>
#include <QPainter>

namespace AsterUI {

    // Forward declaration & Global list
    class AsterMessageWidget;
    static QList<AsterMessageWidget*> g_activeMessages;

    class AsterMessageWidget : public QWidget {
    public:
        AsterMessageWidget(AsterMessage::Type type, const QString& content, int duration, QWidget* parent) 
            : QWidget(nullptr)
        {
            // Set parent window hint
            if (parent) {
                QWidget* top = parent->window();
                if (top) setParent(top, Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
            } else {
                 setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::NoDropShadowWindowHint);
            }
            
            setAttribute(Qt::WA_TranslucentBackground);
            setAttribute(Qt::WA_ShowWithoutActivating);
            setAttribute(Qt::WA_DeleteOnClose);
            
            auto containerLayout = new QVBoxLayout(this);
            containerLayout->setContentsMargins(10, 10, 10, 10);
            
            auto contentWidget = new QWidget(this);
            contentWidget->setObjectName("content");
            contentWidget->setStyleSheet(QString(
                "#content { background-color: %1; border-radius: 6px; border: 1px solid %2; }"
            ).arg(AsterTheme::instance()->colorString(AsterTheme::ColorRole::Surface))
             .arg(AsterTheme::instance()->colorString(AsterTheme::ColorRole::Divider))); // Use Divider for subtle border
             
            auto shadow = new QGraphicsDropShadowEffect(this);
            shadow->setBlurRadius(16);
            shadow->setOffset(0, 4);
            shadow->setColor(QColor(0, 0, 0, 30));
            contentWidget->setGraphicsEffect(shadow);
            
            containerLayout->addWidget(contentWidget);

            auto layout = new QHBoxLayout(contentWidget);
            layout->setContentsMargins(16, 10, 16, 10);
            layout->setSpacing(10);

            // Icon
            QIcon icon = getIcon(type);
            QColor iconColor = getIconColor(type);
            
            auto iconLabel = new AsterIcon(icon, contentWidget);
            iconLabel->setColor(iconColor);
            iconLabel->setFixedSize(18, 18);
            layout->addWidget(iconLabel);

            // Text
            auto textLabel = new QLabel(content, contentWidget);
            textLabel->setStyleSheet(QString("color: %1; font-size: 14px; background: transparent; border: none;")
                .arg(AsterTheme::instance()->colorString(AsterTheme::ColorRole::Text)));
            layout->addWidget(textLabel);

            // Adjust Size immediately
            adjustSize();

            // Close Timer
            if (duration > 0) {
                QTimer::singleShot(duration, this, &AsterMessageWidget::fadeOut);
            }
            
            // Connected in constructor, but safer to do cleanup in destructor or specific close method
            connect(this, &QObject::destroyed, [this](){
                g_activeMessages.removeAll(this);
                // Use timer to reposition to avoid re-entry issues during destruction
                QTimer::singleShot(0, [](){ AsterMessageWidget::repositionAll(); });
            });
        }
        
        static void repositionAll() {
            int topMargin = 50;
            QRect screenGeom;
            
            // Determine reference geometry
            if (!g_activeMessages.isEmpty() && g_activeMessages.first()->parentWidget()) {
                screenGeom = g_activeMessages.first()->parentWidget()->geometry();
            } else if (auto active = QApplication::activeWindow()) {
                // Fix: 如果是 AsterModal (或类似的非主窗口 Modal)，我们希望 Message 显示在主窗口区域
                // 且不应该跟随 Dialog 移动或被 Dialog 限制尺寸
                 QWidget* target = active;
                 if (target->inherits("AsterUI::AsterModal") && target->parentWidget()) {
                     // 尝试向上寻找主窗口
                     if (auto parent = target->parentWidget()->window()) {
                         target = parent;
                     }
                 }
                screenGeom = target->geometry();
            } else {
                screenGeom = QGuiApplication::primaryScreen()->availableGeometry();
            }
            
            int currentY = screenGeom.top() + topMargin;
            
            for (auto msg : g_activeMessages) {
                // Ensure widget geometry is valid
                if (msg->width() < 10) msg->adjustSize();

                int x = screenGeom.left() + (screenGeom.width() - msg->width()) / 2;
                int targetY = currentY;
                
                // Animate to new position
                auto anim = new QPropertyAnimation(msg, "pos");
                anim->setStartValue(msg->pos());
                anim->setEndValue(QPoint(x, targetY));
                anim->setDuration(250);
                anim->setEasingCurve(QEasingCurve::OutQuad);
                anim->start(QAbstractAnimation::DeleteWhenStopped);
                
                currentY += msg->height() - 10; // -10 to account for shadow padding overlap if desired, or pure height
                // Note: msg->height() includes the shadow margins (20px total vertical).
                // Real visual height is height - 20. 
                // We want spacing between visual blocks.
                // currentY += (msg->contentHeight) + spacing.
            }
        }

        void showAnim() {
            setWindowOpacity(0.0);
            show();
            
            auto anim = new QPropertyAnimation(this, "windowOpacity");
            anim->setStartValue(0.0);
            anim->setEndValue(1.0);
            anim->setDuration(300);
            anim->setEasingCurve(QEasingCurve::OutQuad);
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        }

        void fadeOut() {
            auto anim = new QPropertyAnimation(this, "windowOpacity");
            anim->setStartValue(windowOpacity());
            anim->setEndValue(0.0);
            anim->setDuration(250);
            anim->setEasingCurve(QEasingCurve::OutQuad);
            connect(anim, &QPropertyAnimation::finished, this, &QWidget::close); // Close triggers destroy
            anim->start(QAbstractAnimation::DeleteWhenStopped);
            
            auto posAnim = new QPropertyAnimation(this, "pos");
            posAnim->setStartValue(pos());
            posAnim->setEndValue(pos() - QPoint(0, 20));
            posAnim->setDuration(250);
            posAnim->setEasingCurve(QEasingCurve::InQuad);
            posAnim->start(QAbstractAnimation::DeleteWhenStopped);
        }

    private: 
        QIcon getIcon(AsterMessage::Type type) {
            QStyle::StandardPixmap sp = QStyle::SP_MessageBoxInformation;
            switch(type) {
                case AsterMessage::Type::Info: sp = QStyle::SP_MessageBoxInformation; break;
                case AsterMessage::Type::Success: sp = QStyle::SP_DialogApplyButton; break; 
                case AsterMessage::Type::Warning: sp = QStyle::SP_MessageBoxWarning; break;
                case AsterMessage::Type::Error: sp = QStyle::SP_MessageBoxCritical; break;
            }
             return QApplication::style()->standardIcon(sp);
        }
        
        QColor getIconColor(AsterMessage::Type type) {
            auto theme = AsterTheme::instance();
            using T = AsterMessage::Type;
            switch(type) {
                case T::Info: return theme->color(AsterTheme::ColorRole::Primary);
                case T::Success: return theme->color(AsterTheme::ColorRole::Success);
                case T::Warning: return theme->color(AsterTheme::ColorRole::Warning);
                case T::Error: return theme->color(AsterTheme::ColorRole::Error);
            }
            return theme->color(AsterTheme::ColorRole::Primary);
        }
    };

    void AsterMessage::show(Type type, const QString& content, int duration, QWidget* parent) {
        auto msg = new AsterMessageWidget(type, content, duration, parent);
        
        // Initial positioning calculation (rough)
        // Correct positioning happens in repositionAll
        // But we need to set initial Pos to something reasonable (e.g. top center - 20)
        
        QRect screenGeom;
        if (msg->parentWidget()) {
             screenGeom = msg->parentWidget()->geometry();
        } else if (auto active = QApplication::activeWindow()) {
            screenGeom = active->geometry();
        } else {
             screenGeom = QGuiApplication::primaryScreen()->availableGeometry();
        }
        
        int x = screenGeom.left() + (screenGeom.width() - msg->width()) / 2;
        int y = screenGeom.top() + 30;
        msg->move(x, y);

        g_activeMessages.append(msg);
        AsterMessageWidget::repositionAll();
        msg->showAnim();
    }

    void AsterMessage::info(const QString& content, int duration, QWidget* parent) {
        show(Type::Info, content, duration, parent);
    }
    void AsterMessage::success(const QString& content, int duration, QWidget* parent) {
        show(Type::Success, content, duration, parent);
    }
    void AsterMessage::warning(const QString& content, int duration, QWidget* parent) {
        show(Type::Warning, content, duration, parent);
    }
    void AsterMessage::error(const QString& content, int duration, QWidget* parent) {
        show(Type::Error, content, duration, parent);
    }

}
