#include "AsterUI/AsterSwitch.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

namespace AsterUI {

    AsterSwitch::AsterSwitch(QWidget* parent)
        : QAbstractButton(parent)
        , m_handlePosition(0.0)
    {
        setCheckable(true);
        init();
    }

    AsterSwitch::~AsterSwitch() = default;

    void AsterSwitch::init() {
        setCursor(Qt::PointingHandCursor);
        m_animGroup = new QParallelAnimationGroup(this);
        
        // 默认颜色
        m_uncheckedColor = QColor(0, 0, 0, 64); // Unchecked gray (Alpha 25%)
        m_checkedColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Primary);

        // 初始颜色
        m_backgroundColor = m_uncheckedColor;
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
    }

    void AsterSwitch::leaveEvent(QEvent* event) {
        QAbstractButton::leaveEvent(event);
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

        // 1. 绘制轨道 (Track)
        // 轨道高度占满控件，圆角为高度的一半
        QRectF trackRect = rect();
        painter.setPen(Qt::NoPen);
        painter.setBrush(m_backgroundColor);
        painter.drawRoundedRect(trackRect, height() / 2.0, height() / 2.0);

        // 2. 计算手柄 (Handle) 位置和大小
        // 留出 2px 的边距 (Padding)，让手柄悬浮在轨道内
        qreal padding = 2.0;
        qreal handleSize = height() - 2 * padding;
        qreal trackWidth = width();
        
        // 计算手柄的 X 坐标范围
        // 最小 X: padding
        // 最大 X: width - padding - handleSize
        qreal minX = padding;
        qreal maxX = trackWidth - padding - handleSize;
        
        // 根据当前动画位置 (0.0 ~ 1.0) 插值计算实际 X
        qreal currentX = minX + (maxX - minX) * m_handlePosition;
        
        QRectF handleRect(currentX, padding, handleSize, handleSize);

        // 3. 绘制手柄阴影 (简单的偏移半透明圆)
        // 阴影向下偏移 1px，颜色为半透明黑
        painter.setBrush(QColor(0, 0, 0, 40)); 
        painter.drawEllipse(handleRect.translated(0, 1)); 

        // 4. 绘制手柄本体 (纯白)
        painter.setBrush(Qt::white);
        painter.drawEllipse(handleRect);
    }

}
