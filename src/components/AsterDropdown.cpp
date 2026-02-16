#include "AsterDropdown.h"
#include "AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QMenu>
#include <QMouseEvent>
#include <QVariantAnimation>

namespace AsterFlorets {

    AsterDropdown::AsterDropdown(const QString& text, QWidget* parent)
        : AsterButton(text, parent)
    {
        // Dropdown usually looks like a button or a link
        // By default, let's treat it as a button that triggers a menu
        m_menu = new QMenu(this);
        // Style the menu roughly to match theme (Qt styles menus via QStyle or stylesheet generally)
        // For now, default OS menu style or minimal stylesheet
        m_menu->setStyleSheet(QString(
            "QMenu { background-color: %1; border: 1px solid %2; padding: 4px; }"
            "QMenu::item { padding: 5px 20px; color: %3; }"
            "QMenu::item:selected { background-color: %4; color: %1; }"
        ).arg(AsterTheme::instance()->color(AsterTheme::ColorRole::Background).name())
         .arg(AsterTheme::instance()->color(AsterTheme::ColorRole::Border).name())
         .arg(AsterTheme::instance()->color(AsterTheme::ColorRole::Text).name())
         .arg(AsterTheme::instance()->color(AsterTheme::ColorRole::Primary).name())
        );

        // Manual menu handling not fully implemented in AsterButton base
        // But we connect clicked to show menu
        connect(this, &AsterDropdown::clicked, this, [this](){
            if (m_menu) {
                // Show menu at bottom left of button
                QPoint p = mapToGlobal(rect().bottomLeft());
                m_menu->exec(p);
            }
        });
    }

    AsterDropdown::~AsterDropdown() = default; // m_menu is child

    void AsterDropdown::addAction(const QString& text, const std::function<void()>& handler) {
        QAction* act = m_menu->addAction(text);
        if (handler) {
            connect(act, &QAction::triggered, handler);
        }
    }

    void AsterDropdown::setMenu(QMenu* menu) {
        if (m_menu && m_menu->parent() == this) {
            delete m_menu;
        }
        m_menu = menu;
        // logic to show menu is in constructor lambda
    }

    QMenu* AsterDropdown::menu() const {
        return m_menu;
    }

    void AsterDropdown::paintEvent(QPaintEvent* event) {
        // 重写父类 paintEvent，不使用 AsterButton 自带的波纹/Scale，避免视觉冲突
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        drawBackground(painter); // 使用 AsterButton 的背景绘制逻辑

        // 绘制内容 (Left aligned text)
        painter.setPen(textColor());
        painter.setFont(font());
        
        // Icon / Spinner placeholder logic if needed, simplify for dropdown:
        int padding = 12;
        QRect textRect = rect().adjusted(padding, 0, -30, 0); // Reserve space for arrow
        painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text());

        // Simple arrow
        int arrowSize = 8;
        int x = width() - 20;
        int y = height() / 2 - arrowSize / 4;
        
        QPainterPath path;
        path.moveTo(x, y);
        path.lineTo(x + arrowSize / 2, y + arrowSize / 2);
        path.lineTo(x + arrowSize, y);
        
        QPen arrowPen(textColor(), 1.5);
        arrowPen.setCapStyle(Qt::RoundCap);
        arrowPen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(arrowPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path);
    }

    void AsterDropdown::mousePressEvent(QMouseEvent* event) {
        // 覆盖 AsterButton 的点击动画逻辑，Dropdown不需要缩放
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }
        // QAbstractButton default handling for 'down' state if needed
        QAbstractButton::mousePressEvent(event);
    }

    void AsterDropdown::mouseReleaseEvent(QMouseEvent* event) {
        QAbstractButton::mouseReleaseEvent(event);
    }

}
