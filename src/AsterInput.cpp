#include "AsterUI/AsterInput.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QAction>
#include <QStyle>

namespace AsterUI {

    AsterInput::AsterInput(QWidget* parent)
        : QLineEdit(parent)
    {
        init();
    }

    AsterInput::AsterInput(const QString& placeholder, QWidget* parent)
        : QLineEdit(parent)
    {
        setPlaceholderText(placeholder);
        init();
    }

    AsterInput::~AsterInput() = default;

    void AsterInput::init() {
        setAttribute(Qt::WA_MacShowFocusRect, false); // Disable native focus rect on Mac
        
        auto theme = AsterTheme::instance();
        m_borderColor = theme->color(AsterTheme::ColorRole::Border);
        m_clearable = false;
        m_clearAction = nullptr;

        // 样式设置：移除原生边框，增加 Padding
        // 注意：我们通过 paintEvent 自绘边框，所以这里设为无边框
        // 但为了保证文字不被边框遮挡，需要设置 textMargins
        setTextMargins(8, 0, 8, 0);
        
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

    bool AsterInput::isClearable() const {
        return m_clearable;
    }

    void AsterInput::setClearable(bool clearable) {
        if (m_clearable == clearable) return;
        m_clearable = clearable;

        if (m_clearable) {
            if (!m_clearAction) {
                // 使用标准图标，实际项目中建议使用自定义 SVG 图标
                QIcon clearIcon = style()->standardIcon(QStyle::SP_DialogCloseButton);
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

    void AsterInput::setBorderColor(const QColor& color) {
        m_borderColor = color;
        update();
    }

    void AsterInput::paintEvent(QPaintEvent* event) {
        // 先让 QLineEdit 绘制文字和占位符
        // 为了避免原生边框干扰，我们通常在 stylesheet 中设置 border: none
        // 或者在这里不调用基类 paintEvent 而是手动绘制文字？
        // 更好的做法是：让基类绘制内容，但我们覆盖背景和边框。
        
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        auto theme = AsterTheme::instance();
        int radius = theme->borderRadius();

        // 1. 绘制背景和边框
        QPainterPath path;
        path.addRoundedRect(rect(), radius, radius);

        // Clipping 技巧绘制内描边
        painter.save();
        painter.setClipPath(path);

        // 背景
        painter.fillPath(path, theme->color(AsterTheme::ColorRole::Surface));

        // 边框
        qreal borderWidth = 2.0;
        QPen pen(m_borderColor);
        pen.setWidthF(borderWidth);
        painter.setPen(pen);
        painter.drawPath(path);

        // Focus 时的光晕效果 (Box Shadow)
        if (hasFocus()) {
            // 简单的光晕模拟：再画一圈半透明的边框
            // 由于 Clipping 限制，这里其实是在内部画，可能效果不如外部阴影
            // 如果要外部阴影，需要取消 Clip，并且 rect 稍微缩小一点留出阴影空间
            // 这里为了简单，暂且只改变边框颜色，光晕效果留待后续优化
        }

        painter.restore();

        // 2. 绘制文字内容
        // 调用基类绘制，但要注意基类可能会绘制自带的边框
        // 我们通过 setFrame(false) 来禁用原生边框
        bool originalFrame = hasFrame();
        if (originalFrame) const_cast<AsterInput*>(this)->setFrame(false);
        
        QLineEdit::paintEvent(event);
        
        if (originalFrame) const_cast<AsterInput*>(this)->setFrame(true);
    }

    void AsterInput::focusInEvent(QFocusEvent* event) {
        QLineEdit::focusInEvent(event);
        animateBorder(true);
    }

    void AsterInput::focusOutEvent(QFocusEvent* event) {
        QLineEdit::focusOutEvent(event);
        animateBorder(false);
    }

    void AsterInput::resizeEvent(QResizeEvent* event) {
        QLineEdit::resizeEvent(event);
        // 如果有自定义布局的图标，这里更新位置
    }

    void AsterInput::animateBorder(bool focused) {
        auto theme = AsterTheme::instance();
        QColor start = m_borderColor;
        QColor end = focused ? theme->color(AsterTheme::ColorRole::Primary) 
                             : theme->color(AsterTheme::ColorRole::Border);

        m_borderAnimation->stop();
        m_borderAnimation->setStartValue(start);
        m_borderAnimation->setEndValue(end);
        m_borderAnimation->start();
    }

}
