#include "AsterUI/AsterButton.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QVariantAnimation>

#include <QTimer>

namespace AsterUI {

    AsterButton::AsterButton(const QString& text, QWidget* parent) 
        : AsterButton(parent) 
    {
        setText(text);
    }

    AsterButton::AsterButton(QWidget* parent)
        : QAbstractButton(parent)
        , m_type(Type::Default)
        , m_colorAnimationGroup(new QParallelAnimationGroup(this))
        , m_rippleAnimation(new QPropertyAnimation(this, "")) // 占位，后续自定义
        , m_loadingTimer(new QTimer(this))
    {
        setCursor(Qt::PointingHandCursor);
        
        // Loading 动画定时器
        connect(m_loadingTimer, &QTimer::timeout, this, [this]() {
            m_loadingAngle += 10.0;
            if (m_loadingAngle >= 360.0) m_loadingAngle = 0.0;
            update();
        });
        m_loadingTimer->setInterval(30); // ~30fps

        // 初始化波纹动画
        auto rippleAnim = new QVariantAnimation(this);
        rippleAnim->setDuration(400);
        rippleAnim->setEasingCurve(QEasingCurve::OutQuad);
        connect(rippleAnim, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
            qreal progress = value.toReal();
            m_ripple.radius = width() * 1.5 * progress; // 扩散到宽度的1.5倍
            m_ripple.opacity = 0.4 * (1.0 - progress);  // 透明度从 0.4 渐变到 0
            update();
        });
        connect(rippleAnim, &QVariantAnimation::finished, this, [this]() {
            m_ripple.active = false;
            update();
        });
        m_rippleAnimation = reinterpret_cast<QPropertyAnimation*>(rippleAnim); // Hacky cast for storage

        updateStyle();
    }

    AsterButton::~AsterButton() = default;

    AsterButton::Type AsterButton::type() const {
        return m_type;
    }

    void AsterButton::setType(Type type) {
        if (m_type == type) return;
        m_type = type;
        updateStyle();
        update();
    }

    void AsterButton::setBackgroundColor(const QColor& color) {
        m_backgroundColor = color;
        update();
    }

    void AsterButton::setBorderColor(const QColor& color) {
        m_borderColor = color;
        update();
    }

    void AsterButton::setTextColor(const QColor& color) {
        m_textColor = color;
        update();
    }

    void AsterButton::setBorderRadius(int radius) {
        m_borderRadius = radius;
        update();
    }

    void AsterButton::setLoading(bool loading) {
        if (m_loading == loading) return;
        m_loading = loading;
        setEnabled(!loading); // Loading 时禁用交互
        
        if (m_loading) {
            m_loadingTimer->start();
        } else {
            m_loadingTimer->stop();
        }
        update();
    }

    QSize AsterButton::sizeHint() const {
        // 基础尺寸计算
        QFontMetrics fm(font());
        int w = fm.horizontalAdvance(text()) + 32; // Padding horizontal
        int h = fm.height() + 12;                  // Padding vertical
        return QSize(qMax(w, 60), qMax(h, 32));    // 最小尺寸
    }

    void AsterButton::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        auto theme = AsterTheme::instance();
        int radius = theme->borderRadius();

        // 1. 绘制背景和边框
        // 注意：Qt 的 drawRect/drawPath 描边是居中对齐的，如果直接用 rect()，
        // 边缘的 0.5px 会被切掉。需要向内收缩半个像素。
        QRectF r = rect();
        r.adjust(0.5, 0.5, -0.5, -0.5);

        QPainterPath path;
        path.addRoundedRect(r, radius, radius);

        // 填充背景
        if (m_backgroundColor.isValid() && m_backgroundColor.alpha() > 0) {
            painter.fillPath(path, m_backgroundColor);
        }

        // 绘制波纹 (在背景之上，文字之下)
        drawRipple(painter);

        // 绘制边框
        if (m_type == Type::Dashed) {
            QPen pen(m_borderColor);
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);
            painter.drawPath(path);
        } else if (m_borderColor.isValid() && m_borderColor.alpha() > 0) {
            QPen pen(m_borderColor);
            painter.setPen(pen);
            painter.drawPath(path);
        }

        // 2. 绘制内容 (Spinner + Text)
        painter.setPen(m_textColor);
        painter.setFont(font());
        
        if (m_loading) {
            // 计算总宽度：Spinner + Spacing + Text
            int spinnerSize = 14;
            int spacing = 8;
            int textWidth = painter.fontMetrics().horizontalAdvance(text());
            int totalWidth = spinnerSize + spacing + textWidth;
            
            int startX = (width() - totalWidth) / 2;
            int centerY = height() / 2;
            
            // 绘制 Spinner
            painter.save();
            painter.translate(startX + spinnerSize / 2, centerY);
            painter.rotate(m_loadingAngle);
            painter.setPen(QPen(m_textColor, 2));
            painter.drawArc(-spinnerSize/2, -spinnerSize/2, spinnerSize, spinnerSize, 0, 270 * 16);
            painter.restore();
            
            // 绘制文字
            QRect textRect(startX + spinnerSize + spacing, 0, textWidth, height());
            painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text());
        } else {
            painter.drawText(rect(), Qt::AlignCenter, text());
        }
    }

    void AsterButton::drawRipple(QPainter& painter) {
        if (!m_ripple.active) return;

        painter.save();
        QPainterPath path;
        path.addRoundedRect(rect(), AsterTheme::instance()->borderRadius(), AsterTheme::instance()->borderRadius());
        painter.setClipPath(path); // 限制波纹在按钮内部

        QColor rippleColor = (m_type == Type::Primary) ? Qt::white : AsterTheme::instance()->color(AsterTheme::ColorRole::Primary);
        rippleColor.setAlphaF(m_ripple.opacity);

        painter.setBrush(rippleColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(m_ripple.center), m_ripple.radius, m_ripple.radius);
        painter.restore();
    }

    void AsterButton::enterEvent(QEnterEvent* event) {
        QAbstractButton::enterEvent(event);
        startHoverAnimation(true);
    }

    void AsterButton::leaveEvent(QEvent* event) {
        QAbstractButton::leaveEvent(event);
        startHoverAnimation(false);
    }

    void AsterButton::mousePressEvent(QMouseEvent* event) {
        QAbstractButton::mousePressEvent(event);
        startRippleAnimation(event->pos());
    }

    void AsterButton::mouseReleaseEvent(QMouseEvent* event) {
        QAbstractButton::mouseReleaseEvent(event);
    }

    void AsterButton::updateStyle() {
        auto theme = AsterTheme::instance();
        
        // 确定初始颜色状态
        QColor bg, border, text;

        switch (m_type) {
            case Type::Primary:
                bg = theme->color(AsterTheme::ColorRole::Primary);
                border = bg;
                text = Qt::white;
                break;
            case Type::Default:
                bg = theme->color(AsterTheme::ColorRole::Surface);
                border = theme->color(AsterTheme::ColorRole::Border);
                text = theme->color(AsterTheme::ColorRole::Text);
                break;
            case Type::Dashed:
                bg = theme->color(AsterTheme::ColorRole::Surface);
                border = theme->color(AsterTheme::ColorRole::Border);
                text = theme->color(AsterTheme::ColorRole::Text);
                break;
            case Type::Text:
                bg = Qt::transparent;
                border = Qt::transparent;
                text = theme->color(AsterTheme::ColorRole::Text);
                break;
            case Type::Link:
                bg = Qt::transparent;
                border = Qt::transparent;
                text = theme->color(AsterTheme::ColorRole::Primary);
                break;
        }

        // 直接设置，不动画 (用于初始化或类型切换)
        m_backgroundColor = bg;
        m_borderColor = border;
        m_textColor = text;
        update();
    }

    void AsterButton::startHoverAnimation(bool hovered) {
        auto theme = AsterTheme::instance();
        
        QColor targetBg, targetBorder, targetText;

        // 计算目标颜色
        switch (m_type) {
            case Type::Primary:
                targetBg = hovered ? theme->color(AsterTheme::ColorRole::PrimaryHover) : theme->color(AsterTheme::ColorRole::Primary);
                targetBorder = targetBg;
                targetText = Qt::white;
                break;
            case Type::Default:
            case Type::Dashed:
                targetBg = theme->color(AsterTheme::ColorRole::Surface);
                targetBorder = hovered ? theme->color(AsterTheme::ColorRole::PrimaryHover) : theme->color(AsterTheme::ColorRole::Border);
                targetText = hovered ? theme->color(AsterTheme::ColorRole::PrimaryHover) : theme->color(AsterTheme::ColorRole::Text);
                break;
            case Type::Text:
                targetBg = hovered ? QColor(0, 0, 0, 15) : Qt::transparent; // 轻微背景
                targetBorder = Qt::transparent;
                targetText = theme->color(AsterTheme::ColorRole::Text);
                break;
            case Type::Link:
                targetBg = Qt::transparent;
                targetBorder = Qt::transparent;
                targetText = hovered ? theme->color(AsterTheme::ColorRole::PrimaryHover) : theme->color(AsterTheme::ColorRole::Primary);
                break;
        }

        // 停止之前的动画
        m_colorAnimationGroup->stop();
        m_colorAnimationGroup->clear();

        auto createAnim = [this](const QByteArray& prop, const QColor& start, const QColor& end) {
            auto anim = new QPropertyAnimation(this, prop);
            anim->setDuration(200);
            anim->setStartValue(start);
            anim->setEndValue(end);
            return anim;
        };

        if (m_backgroundColor != targetBg) 
            m_colorAnimationGroup->addAnimation(createAnim("backgroundColor", m_backgroundColor, targetBg));
        
        if (m_borderColor != targetBorder)
            m_colorAnimationGroup->addAnimation(createAnim("borderColor", m_borderColor, targetBorder));
            
        if (m_textColor != targetText)
            m_colorAnimationGroup->addAnimation(createAnim("textColor", m_textColor, targetText));

        m_colorAnimationGroup->start();
    }

    void AsterButton::startRippleAnimation(const QPoint& pos) {
        m_ripple.center = pos;
        m_ripple.active = true;
        
        auto anim = static_cast<QVariantAnimation*>(m_rippleAnimation);
        anim->stop();
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->start();
    }

}
