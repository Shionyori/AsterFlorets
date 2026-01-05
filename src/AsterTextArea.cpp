#include "AsterUI/AsterTextArea.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QEvent>

namespace AsterUI {

    AsterTextArea::AsterTextArea(QWidget* parent)
        : QFrame(parent)
    {
        init();
    }

    AsterTextArea::AsterTextArea(const QString& placeholder, QWidget* parent)
        : QFrame(parent)
    {
        init();
        setPlaceholderText(placeholder);
    }

    AsterTextArea::~AsterTextArea() = default;

    void AsterTextArea::init() {
        setAttribute(Qt::WA_TranslucentBackground); // 支持圆角透明
        auto theme = AsterTheme::instance();
        m_borderColor = theme->color(AsterTheme::ColorRole::Border);
        
        // Setup Layout
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(10, 6, 10, 6); // 增加一点内边距
        layout->setSpacing(0);

        // Setup Editor
        m_editor = new QPlainTextEdit(this);
        m_editor->setFrameShape(QFrame::NoFrame);
        
        // Make editor transparent background so our frame background shows through
        QPalette p = m_editor->palette();
        p.setColor(QPalette::Base, Qt::transparent);
        p.setColor(QPalette::Text, theme->color(AsterTheme::ColorRole::Text));
        p.setColor(QPalette::PlaceholderText, theme->color(AsterTheme::ColorRole::Placeholder));
        m_editor->setPalette(p);
        m_editor->viewport()->setAutoFillBackground(false);
        
        layout->addWidget(m_editor);

        // Install event filter to capture focus events from editor
        m_editor->installEventFilter(this);
        
        // Animation
        m_borderAnimation = new QPropertyAnimation(this, "borderColor", this);
        m_borderAnimation->setDuration(200);
        m_borderAnimation->setEasingCurve(QEasingCurve::OutQuad);
    }

    void AsterTextArea::setPlaceholderText(const QString& text) {
        if (m_editor) m_editor->setPlaceholderText(text);
    }

    QString AsterTextArea::placeholderText() const {
        return m_editor ? m_editor->placeholderText() : QString();
    }

    void AsterTextArea::setPlainText(const QString& text) {
        if (m_editor) m_editor->setPlainText(text);
    }

    QString AsterTextArea::toPlainText() const {
        return m_editor ? m_editor->toPlainText() : QString();
    }

    void AsterTextArea::setStatus(Status status) {
        if (m_status == status) return;
        m_status = status;
        animateBorder();
    }

    void AsterTextArea::setBorderColor(const QColor& color) {
        m_borderColor = color;
        update();
    }

    bool AsterTextArea::eventFilter(QObject* watched, QEvent* event) {
        if (watched == m_editor) {
            if (event->type() == QEvent::FocusIn) {
                m_isFocused = true;
                animateBorder();
                update();
            } else if (event->type() == QEvent::FocusOut) {
                m_isFocused = false;
                animateBorder();
                update();
            }
        }
        return QFrame::eventFilter(watched, event);
    }

    void AsterTextArea::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        auto theme = AsterTheme::instance();
        // 使用稍大的圆角 (6px)
        int radius = 6; 

        // 内缩 1px 以防边缘锯齿和容纳光晕
        QRectF r = rect();
        r.adjust(1.5, 1.5, -1.5, -1.5);

        QPainterPath path;
        path.addRoundedRect(r, radius, radius);

        // 1. 绘制背景
        painter.fillPath(path, theme->color(AsterTheme::ColorRole::Surface));

        // 2. 确定边框颜色
        QColor borderColor = m_borderColor;

        // 3. 绘制光晕 (Focus Ring) - 先画光晕，再画边框
        if (m_isFocused) {
            QColor glowColor = borderColor;
            glowColor.setAlpha(40); // 降低不透明度
            QPen glowPen(glowColor);
            glowPen.setWidth(4); // 4px 宽，实际上会有 2px 在边框外
            painter.setPen(glowPen);
            painter.setBrush(Qt::NoBrush);
            painter.drawPath(path);
        }

        // 4. 绘制边框
        QPen pen(borderColor);
        pen.setWidth(1);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path);
    }

    void AsterTextArea::enterEvent(QEnterEvent* event) {
        m_isHovered = true;
        animateBorder();
        QFrame::enterEvent(event);
    }

    void AsterTextArea::leaveEvent(QEvent* event) {
        m_isHovered = false;
        animateBorder();
        QFrame::leaveEvent(event);
    }

    void AsterTextArea::animateBorder() {
        auto theme = AsterTheme::instance();
        QColor start = m_borderColor;
        QColor end;
        
        if (m_status == Status::Error) {
            end = theme->color(AsterTheme::ColorRole::Error);
        } else if (m_status == Status::Warning) {
            end = theme->color(AsterTheme::ColorRole::Warning);
        } else if (m_isFocused) {
            end = theme->color(AsterTheme::ColorRole::Primary);
        } else if (m_isHovered) {
            end = theme->color(AsterTheme::ColorRole::Primary);
        } else {
            end = theme->color(AsterTheme::ColorRole::Border);
        }

        m_borderAnimation->stop();
        m_borderAnimation->setStartValue(start);
        m_borderAnimation->setEndValue(end);
        m_borderAnimation->start();
    }

}
