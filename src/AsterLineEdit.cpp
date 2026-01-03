#include "AsterUI/AsterLineEdit.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QAction>
#include <QStyle>

namespace AsterUI {

    AsterLineEdit::AsterLineEdit(QWidget* parent)
        : QLineEdit(parent)
    {
        init();
    }

    AsterLineEdit::AsterLineEdit(const QString& placeholder, QWidget* parent)
        : QLineEdit(parent)
    {
        setPlaceholderText(placeholder);
        init();
    }

    AsterLineEdit::~AsterLineEdit() = default;

    void AsterLineEdit::init() {
        setAttribute(Qt::WA_MacShowFocusRect, false);
        setAttribute(Qt::WA_TranslucentBackground); // 支持圆角透明
        
        auto theme = AsterTheme::instance();
        m_borderColor = theme->color(AsterTheme::ColorRole::Border);
        m_clearable = false;
        m_clearAction = nullptr;
        m_isHovered = false;

        // 样式设置：移除原生边框
        setFrame(false);
        
        // 设置内边距 (考虑到背景内缩，稍微增加边距)
        setTextMargins(10, 0, 10, 0);
        
        // 关键：设置背景透明，以便在 paintEvent 中绘制自定义背景
        // 同时设置文字颜色
        QPalette p = palette();
        p.setColor(QPalette::Base, Qt::transparent);
        p.setColor(QPalette::Text, theme->color(AsterTheme::ColorRole::Text));
        p.setColor(QPalette::PlaceholderText, theme->color(AsterTheme::ColorRole::Placeholder));
        setPalette(p);

        // 动画初始化
        m_borderAnimation = new QPropertyAnimation(this, "borderColor", this);
        m_borderAnimation->setDuration(200);
        m_borderAnimation->setEasingCurve(QEasingCurve::OutQuad);

        // 监听文本变化以控制清除按钮显隐
        connect(this, &QLineEdit::textChanged, this, [this](const QString& text) {
            if (m_clearAction) {
                m_clearAction->setVisible(!text.isEmpty());
            }
        });
    }

    bool AsterLineEdit::isClearable() const {
        return m_clearable;
    }

    void AsterLineEdit::setClearable(bool clearable) {
        if (m_clearable == clearable) return;
        m_clearable = clearable;

        if (m_clearable) {
            if (!m_clearAction) {
                QIcon clearIcon = style()->standardIcon(QStyle::SP_LineEditClearButton);
                m_clearAction = addAction(clearIcon, QLineEdit::TrailingPosition);
                connect(m_clearAction, &QAction::triggered, this, &QLineEdit::clear);
            }
            m_clearAction->setVisible(!text().isEmpty());
        } else {
            if (m_clearAction) {
                removeAction(m_clearAction);
                delete m_clearAction;
                m_clearAction = nullptr;
            }
        }
    }

    void AsterLineEdit::setBorderRadius(int radius) {
        m_borderRadius = radius;
        update();
    }

    void AsterLineEdit::setStatus(Status status) {
        if (m_status == status) return;
        m_status = status;
        animateBorder(hasFocus());
    }

    void AsterLineEdit::setBorderColor(const QColor& color) {
        m_borderColor = color;
        update();
    }

    void AsterLineEdit::paintEvent(QPaintEvent* event) {
        // 1. 绘制自定义背景和边框
        {
            QPainter painter(this);
            if (painter.isActive()) {
                painter.setRenderHint(QPainter::Antialiasing);

                auto theme = AsterTheme::instance();
                // 统一使用 6px 圆角
                int radius = (m_borderRadius >= 0) ? m_borderRadius : 6;

                // 内缩 1.5px
                QRectF r = rect();
                r.adjust(1.5, 1.5, -1.5, -1.5);

                QPainterPath path;
                path.addRoundedRect(r, radius, radius);

                // 背景
                painter.fillPath(path, theme->color(AsterTheme::ColorRole::Surface));

                // 边框颜色
                QColor borderColor = m_borderColor;
                if (m_status == Status::Error) {
                    borderColor = theme->color(AsterTheme::ColorRole::Error);
                } else if (m_status == Status::Warning) {
                    borderColor = theme->color(AsterTheme::ColorRole::Warning);
                } else if (m_isHovered && !hasFocus()) {
                    borderColor = theme->color(AsterTheme::ColorRole::Primary);
                } else if (hasFocus()) {
                    borderColor = theme->color(AsterTheme::ColorRole::Primary);
                }

                // Focus Glow (先画光晕)
                if (hasFocus()) {
                    QColor glowColor = borderColor;
                    glowColor.setAlpha(40);
                    QPen glowPen(glowColor);
                    glowPen.setWidth(4);
                    painter.setPen(glowPen);
                    painter.setBrush(Qt::NoBrush);
                    painter.drawPath(path);
                }

                // 绘制边框
                QPen pen(borderColor);
                pen.setWidth(1);
                painter.setPen(pen);
                painter.setBrush(Qt::NoBrush);
                painter.drawPath(path);
            }
        }

        // 2. 绘制文字 (调用基类)
        QLineEdit::paintEvent(event);
    }

    void AsterLineEdit::focusInEvent(QFocusEvent* event) {
        QLineEdit::focusInEvent(event);
        animateBorder(true);
    }

    void AsterLineEdit::focusOutEvent(QFocusEvent* event) {
        QLineEdit::focusOutEvent(event);
        animateBorder(false);
    }

    void AsterLineEdit::enterEvent(QEnterEvent* event) {
        m_isHovered = true;
        update();
        QLineEdit::enterEvent(event);
    }

    void AsterLineEdit::leaveEvent(QEvent* event) {
        m_isHovered = false;
        update();
        QLineEdit::leaveEvent(event);
    }

    void AsterLineEdit::resizeEvent(QResizeEvent* event) {
        QLineEdit::resizeEvent(event);
    }

    void AsterLineEdit::updateClearButtonPosition() {
        // QLineEdit handles action layout automatically
    }

    void AsterLineEdit::animateBorder(bool focused) {
        auto theme = AsterTheme::instance();
        QColor start = m_borderColor;
        QColor end;

        if (m_status == Status::Error) {
            end = theme->color(AsterTheme::ColorRole::Error);
        } else if (m_status == Status::Warning) {
            end = theme->color(AsterTheme::ColorRole::Warning);
        } else {
            end = focused ? theme->color(AsterTheme::ColorRole::Primary) 
                          : theme->color(AsterTheme::ColorRole::Border);
        }

        m_borderAnimation->stop();
        m_borderAnimation->setStartValue(start);
        m_borderAnimation->setEndValue(end);
        m_borderAnimation->start();
    }

}
