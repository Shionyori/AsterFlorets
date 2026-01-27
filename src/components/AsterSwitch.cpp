#include "AsterSwitch.h"
#include "AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

namespace AsterFlorets {

    AsterSwitch::AsterSwitch(QWidget* parent)
        : QAbstractButton(parent)
        , m_handlePosition(0.0)
    {
        setCheckable(true);
        init();
    }

    AsterSwitch::~AsterSwitch() = default;

    void AsterSwitch::init() {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setCursor(Qt::PointingHandCursor);
        m_animGroup = new QParallelAnimationGroup(this);
        m_glowAnim = new QPropertyAnimation(this, "glowOpacity", this);
        m_glowAnim->setDuration(200);
        
        // 默认颜色
        m_uncheckedColor = AsterTheme::instance()->color(AsterTheme::ColorRole::TextDisabled); 
        m_checkedColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Primary);

        // 初始颜色
        m_backgroundColor = m_uncheckedColor;
        m_glowOpacity = 0.0;
    }

    QSize AsterSwitch::sizeHint() const {
        return QSize(44, 22); // 标准 Switch 尺寸
    }

    QSize AsterSwitch::minimumSizeHint() const {
        return sizeHint();
    }

    void AsterSwitch::setHandlePosition(qreal pos) {
        m_handlePosition = pos;
        update();
    }

    void AsterSwitch::setBackgroundColor(const QColor& color) {
        m_backgroundColor = color;
        update();
    }

    void AsterSwitch::setGlowOpacity(double opacity) {
        m_glowOpacity = opacity;
        update();
    }

    void AsterSwitch::setCheckedColor(const QColor& color) {
        m_checkedColor = color;
        if (isChecked()) {
            m_backgroundColor = color;
            update();
        }
    }

    void AsterSwitch::setUncheckedColor(const QColor& color) {
        m_uncheckedColor = color;
        if (!isChecked()) {
            m_backgroundColor = color;
            update();
        }
    }

    void AsterSwitch::mouseReleaseEvent(QMouseEvent* event) {
        // 强制手动切换状态，确保点击生效
        if (event->button() == Qt::LeftButton && hitButton(event->pos())) {
            setChecked(!isChecked());
            // QAbstractButton::mouseReleaseEvent(event); // 不需要调用基类，避免重复触发
            // 注意：setChecked 会触发 checkStateSet，从而启动动画
        } else {
            QAbstractButton::mouseReleaseEvent(event);
        }
    }

    void AsterSwitch::checkStateSet() {
        QAbstractButton::checkStateSet();
        startAnimation(isChecked());
    }

    void AsterSwitch::nextCheckState() {
        QAbstractButton::nextCheckState();
    }

    void AsterSwitch::enterEvent(QEnterEvent* event) {
        QAbstractButton::enterEvent(event);
        m_isHovered = true;
        animateGlow(true);
    }

    void AsterSwitch::leaveEvent(QEvent* event) {
        QAbstractButton::leaveEvent(event);
        m_isHovered = false;
        animateGlow(false);
    }

    void AsterSwitch::animateGlow(bool hovered) {
        m_glowAnim->stop();
        m_glowAnim->setStartValue(m_glowOpacity);
        m_glowAnim->setEndValue(hovered ? 1.0 : 0.0);
        m_glowAnim->start();
    }

    void AsterSwitch::startAnimation(bool checked) {
        m_animGroup->stop();
        
        // 清除之前的动画，防止内存泄漏或逻辑混乱
        // 注意：QParallelAnimationGroup::clear() 会删除动画对象
        if (m_animGroup->animationCount() > 0) {
            m_animGroup->clear();
        }

        // 颜色动画
        QColor targetBg = checked ? m_checkedColor : m_uncheckedColor;
        auto colorAnim = new QPropertyAnimation(this, "backgroundColor");
        colorAnim->setDuration(200);
        colorAnim->setStartValue(m_backgroundColor);
        colorAnim->setEndValue(targetBg);
        m_animGroup->addAnimation(colorAnim);

        // 位置动画
        qreal targetPos = checked ? 1.0 : 0.0;
        auto posAnim = new QPropertyAnimation(this, "handlePosition");
        posAnim->setDuration(200);
        posAnim->setEasingCurve(QEasingCurve::OutQuad);
        posAnim->setStartValue(m_handlePosition);
        posAnim->setEndValue(targetPos);
        m_animGroup->addAnimation(posAnim);

        m_animGroup->start();
    }

    void AsterSwitch::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 定义边距，为悬浮光环留出空间 (2px)
        // 这样光环可以画在轨道外部，而不是覆盖在轨道上
        qreal margin = 2.0;
        QRectF r = rect();
        QRectF trackRect = r.adjusted(margin, margin, -margin, -margin);
        qreal trackRadius = trackRect.height() / 2.0;

        // 1. 绘制悬浮光环 (Glow Ring)
        if (m_glowOpacity > 0.01) {
            QColor glowColor = m_checkedColor;
            glowColor.setAlphaF(0.15 * m_glowOpacity); // Max alpha 0.15
            
            painter.setPen(Qt::NoPen);
            painter.setBrush(glowColor);
            // 光环填满整个控件区域 (比轨道大)
            painter.drawRoundedRect(r, r.height() / 2.0, r.height() / 2.0);
        }

        // 2. 绘制轨道 (Track)
        painter.setPen(Qt::NoPen);
        painter.setBrush(m_backgroundColor);
        painter.drawRoundedRect(trackRect, trackRadius, trackRadius);

        // 3. 计算手柄 (Handle) 位置和大小
        // 留出 2px 的边距 (Padding)，让手柄悬浮在轨道内
        qreal padding = 2.0;
        qreal handleSize = trackRect.height() - 2 * padding;
        
        // 计算手柄的 X 坐标范围 (相对于 trackRect)
        qreal minX = trackRect.left() + padding;
        qreal maxX = trackRect.right() - padding - handleSize;
        
        // 根据当前动画位置 (0.0 ~ 1.0) 插值计算实际 X
        qreal currentX = minX + (maxX - minX) * m_handlePosition;
        
        QRectF handleRect(currentX, trackRect.top() + padding, handleSize, handleSize);

        // 4. 绘制手柄阴影 (简单的偏移半透明圆)
        // 阴影向下偏移 1px，颜色为半透明黑
        painter.setBrush(QColor(0, 0, 0, 40)); 
        painter.drawEllipse(handleRect.translated(0, 1)); 

        // 5. 绘制手柄本体 (纯白)
        painter.setBrush(Qt::white);
        painter.drawEllipse(handleRect);
    }

}
