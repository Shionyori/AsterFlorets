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
        Q_PROPERTY(int paddingHorizontal READ paddingHorizontal WRITE setPaddingHorizontal)
        Q_PROPERTY(int paddingVertical READ paddingVertical WRITE setPaddingVertical)
        Q_PROPERTY(int minWidth READ minWidth WRITE setMinWidth)
        Q_PROPERTY(int minHeight READ minHeight WRITE setMinHeight)
        Q_PROPERTY(int customWidth READ customWidth WRITE setCustomWidth)
        Q_PROPERTY(int customHeight READ customHeight WRITE setCustomHeight)

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

        // 尺寸和边距
        int paddingHorizontal() const { return m_paddingH; }
        void setPaddingHorizontal(int h);

        int paddingVertical() const { return m_paddingV; }
        void setPaddingVertical(int v);

        int minWidth() const { return m_minWidth; }
        void setMinWidth(int w);

        int minHeight() const { return m_minHeight; }
        void setMinHeight(int h);

        // 自定义固定尺寸 (设置 -1 恢复自适应)
        int customWidth() const { return m_customWidth; }
        void setCustomWidth(int w);

        int customHeight() const { return m_customHeight; }
        void setCustomHeight(int h);
        
        void setCustomSize(int w, int h);

        // 动画配置
        void setHoverAnimationDuration(int duration);
        int hoverAnimationDuration() const { return m_hoverDuration; }

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
        void initAnimations();

        // 绘制辅助
        void drawBackground(QPainter& painter);
        void drawRipple(QPainter& painter);
        void drawContent(QPainter& painter);

    private:
        Type m_type;
        
        // 当前绘制颜色 (用于动画插值)
        QColor m_backgroundColor;
        QColor m_borderColor;
        QColor m_textColor;

        int m_borderRadius = -1; // -1 表示使用主题默认值
        bool m_loading = false;
        
        // 边距和最小尺寸
        int m_paddingH = 32;
        int m_paddingV = 12;
        int m_minWidth = 60;
        int m_minHeight = 32;
        
        int m_hoverDuration = 200; // 默认 200ms
        
        // 自定义尺寸 (-1 表示自适应)
        int m_customWidth = -1;
        int m_customHeight = -1;

        qreal m_loadingAngle = 0.0;
        QTimer* m_loadingTimer = nullptr;

        // 动画相关
        QParallelAnimationGroup* m_colorAnimationGroup;
        
        // 按下缩放效果
        qreal m_scale = 1.0;
        QPropertyAnimation* m_scaleAnimation;

        // 波纹效果
        struct Ripple {
            QPoint center;
            qreal radius;
            qreal maxRadius;
            qreal opacity;
            bool active = false;
        };
        Ripple m_ripple;
        QPropertyAnimation* m_rippleAnimation;
    };

}
