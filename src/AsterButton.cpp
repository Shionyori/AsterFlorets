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

    AsterButton::AsterButton(const QString& text, Type type, QWidget* parent)
        : AsterButton(parent)
    {
        setText(text);
        setType(type);
    }

    AsterButton::AsterButton(QWidget* parent)
        : QAbstractButton(parent)
        , m_type(Type::Default)
        , m_colorAnimationGroup(new QParallelAnimationGroup(this))
        , m_rippleAnimation(new QPropertyAnimation(this, "")) // 占位，后续自定义
        , m_loadingTimer(new QTimer(this))
    {
        setCursor(Qt::PointingHandCursor);
        
        initAnimations();

        updateStyle();
    }

    AsterButton::~AsterButton() = default;

    void AsterButton::initAnimations() {
        // 1. Loading 动画定时器
        connect(m_loadingTimer, &QTimer::timeout, this, [this]() {
            m_loadingAngle += 10.0;
            if (m_loadingAngle >= 360.0) m_loadingAngle = 0.0;
            update();
        });
        m_loadingTimer->setInterval(30); // ~30fps

        // 2. 初始化波纹动画
        auto rippleAnim = new QVariantAnimation(this);
        rippleAnim->setDuration(400); // 初始值，实际使用时会动态计算
        rippleAnim->setEasingCurve(QEasingCurve::OutQuad);
        connect(rippleAnim, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
            qreal progress = value.toReal();
            m_ripple.radius = m_ripple.maxRadius * progress;
            m_ripple.opacity = 0.4 * (1.0 - progress);  // 透明度从 0.4 渐变到 0
            update();
        });
        connect(rippleAnim, &QVariantAnimation::finished, this, [this]() {
            m_ripple.active = false;
            update();
        });
        m_rippleAnimation = reinterpret_cast<QPropertyAnimation*>(rippleAnim); // Hacky cast for storage

        // 3. 初始化按压缩放动画
        auto scaleAnim = new QVariantAnimation(this);
        scaleAnim->setDuration(100);
        scaleAnim->setEasingCurve(QEasingCurve::OutQuad);
        connect(scaleAnim, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
            m_scale = value.toReal();
            update();
        });
        m_scaleAnimation = reinterpret_cast<QPropertyAnimation*>(scaleAnim);
    }
    
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

    void AsterButton::setPaddingHorizontal(int h) {
        if (m_paddingH != h) {
            m_paddingH = h;
            updateGeometry();
            update();
        }
    }

    void AsterButton::setPaddingVertical(int v) {
        if (m_paddingV != v) {
            m_paddingV = v;
            updateGeometry();
            update();
        }
    }

    void AsterButton::setMinWidth(int w) {
        if (m_minWidth != w) {
            m_minWidth = w;
            updateGeometry();
            update();
        }
    }

    void AsterButton::setMinHeight(int h) {
        if (m_minHeight != h) {
            m_minHeight = h;
            updateGeometry();
            update();
        }
    }

    void AsterButton::setCustomWidth(int w) {
        if (m_customWidth != w) {
            m_customWidth = w;
            updateGeometry();
            update();
        }
    }

    void AsterButton::setCustomHeight(int h) {
        if (m_customHeight != h) {
            m_customHeight = h;
            updateGeometry();
            update();
        }
    }

    void AsterButton::setCustomSize(int w, int h) {
        if (m_customWidth != w || m_customHeight != h) {
            m_customWidth = w;
            m_customHeight = h;
            updateGeometry();
            update();
        }
    }

    void AsterButton::setHoverAnimationDuration(int duration) {
        m_hoverDuration = duration;
    }

    QSize AsterButton::sizeHint() const {
        // 基础尺寸计算
        QFontMetrics fm(font());
        
        int w = (m_customWidth >= 0) ? m_customWidth : (fm.horizontalAdvance(text()) + m_paddingH);
        int h = (m_customHeight >= 0) ? m_customHeight : (fm.height() + m_paddingV);
        
        return QSize(qMax(w, m_minWidth), qMax(h, m_minHeight));    // 最小尺寸
    }

    void AsterButton::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 应用按压缩放 (以中心为原点)
        if (!qFuzzyCompare(m_scale, 1.0)) {
            painter.translate(width() / 2.0, height() / 2.0);
            painter.scale(m_scale, m_scale);
            painter.translate(-width() / 2.0, -height() / 2.0);
        }

        drawBackground(painter); // 包含背景和边框
        drawRipple(painter);
        drawContent(painter);    // 文本和 Loading
    }

    void AsterButton::drawBackground(QPainter& painter) {
        auto theme = AsterTheme::instance();
        int radius = theme->borderRadius();

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
    }

    void AsterButton::drawContent(QPainter& painter) {
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
        
        // 按下缩放动画 (缩小)
        auto anim = static_cast<QVariantAnimation*>(m_scaleAnimation);
        anim->stop();
        anim->setStartValue(m_scale);
        anim->setEndValue(0.96);
        anim->setDuration(100);
        anim->start();
    }

    void AsterButton::mouseReleaseEvent(QMouseEvent* event) {
        QAbstractButton::mouseReleaseEvent(event);
        
        // 松开恢复动画
        auto anim = static_cast<QVariantAnimation*>(m_scaleAnimation);
        anim->stop();
        anim->setStartValue(m_scale);
        anim->setEndValue(1.0);
        anim->setDuration(150);
        anim->setEasingCurve(QEasingCurve::OutBack); 
        anim->start();
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
            anim->setDuration(m_hoverDuration);
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
        
        // 计算四个顶点到点击位置的最远距离作为最大半径
        // 这样可以避免渲染不必要的巨大圆形，大幅提升长条形按钮的渲染性能
        const QPoint corners[4] = {
            rect().topLeft(), rect().topRight(),
            rect().bottomLeft(), rect().bottomRight()
        };
        
        qreal maxDist = 0.0;
        for (const auto& corner : corners) {
            maxDist = qMax(maxDist, QLineF(pos, corner).length());
        }
        m_ripple.maxRadius = maxDist;
        
        auto anim = static_cast<QVariantAnimation*>(m_rippleAnimation);
        anim->stop();
        
        // 动态计算适宜的波纹 duration
        // 基于最大扩散距离计算，速度更均匀
        int duration = 300 + static_cast<int>(m_ripple.maxRadius * 0.5);
        // 放宽持续时间限制，保证长距离动画的平滑度
        duration = qMin(qMax(duration, 300), 1000);
        
        anim->setDuration(duration);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->start();
    }

}
