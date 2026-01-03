#pragma once

#include "Global.h"
#include <QAbstractButton>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

namespace AsterUI {

    class ASTERUI_EXPORT AsterButton : public QAbstractButton {
        Q_OBJECT
        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
        Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
        Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
        Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius)
        Q_PROPERTY(bool loading READ isLoading WRITE setLoading)

    public:
        enum class Type {
            Primary,
            Default,
            Dashed,
            Text,
            Link
        };
        Q_ENUM(Type)

        explicit AsterButton(const QString& text, QWidget* parent = nullptr);
        explicit AsterButton(QWidget* parent = nullptr);
        ~AsterButton() override;

        Type type() const;
        void setType(Type type);

        // 属性访问器
        int borderRadius() const { return m_borderRadius; }
        void setBorderRadius(int radius);

        bool isLoading() const { return m_loading; }
        void setLoading(bool loading);

        // 动画属性访问器
        QColor backgroundColor() const { return m_backgroundColor; }
        void setBackgroundColor(const QColor& color);

        QColor borderColor() const { return m_borderColor; }
        void setBorderColor(const QColor& color);

        QColor textColor() const { return m_textColor; }
        void setTextColor(const QColor& color);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        
        // 尺寸提示
        QSize sizeHint() const override;

    private:
        void updateStyle();
        void startHoverAnimation(bool hovered);
        void startRippleAnimation(const QPoint& pos);

        // 绘制辅助
        void drawRipple(QPainter& painter);

    private:
        Type m_type;
        
        // 当前绘制颜色 (用于动画插值)
        QColor m_backgroundColor;
        QColor m_borderColor;
        QColor m_textColor;

        int m_borderRadius = -1; // -1 表示使用主题默认值
        bool m_loading = false;
        qreal m_loadingAngle = 0.0;
        QTimer* m_loadingTimer = nullptr;

        // 动画相关
        QParallelAnimationGroup* m_colorAnimationGroup;
        
        // 波纹效果
        struct Ripple {
            QPoint center;
            qreal radius;
            qreal opacity;
            bool active = false;
        };
        Ripple m_ripple;
        QPropertyAnimation* m_rippleAnimation;
    };

}
