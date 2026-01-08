#pragma once

#include "Global.h"
#include <QObject>
#include <QColor>
#include <QFont>
#include <QHash>

namespace AsterUI {

    class ASTERUI_EXPORT AsterTheme : public QObject {
        Q_OBJECT

    public:
        enum class ColorRole {
            Primary,        // 主色 (Shion Blue/Purple)
            PrimaryHover,   // 主色悬停
            PrimaryActive,  // 主色按下
            
            Success,        // 成功
            Warning,        // 警告
            Error,          // 错误
            Info,           // 信息

            Text,           // 主要文本
            TextSecondary,  // 次要文本
            TextDisabled,   // 禁用文本
            Placeholder,    // 占位符文本
            
            Background,     // 背景色
            Surface,        // 表面色 (卡片、弹窗背景)
            Border,         // 边框色
            Divider,        // 分割线
            
            LayoutHeader,   // 页面布局头部区
            LayoutSider,    // 页面布局侧边区
            LayoutContent,  // 页面布局内容区
            LayoutFooter,   // 页面布局底部区

            Mask            // 遮罩层
        };
        Q_ENUM(ColorRole)

        enum class Size {
            Small,
            Default,
            Large
        };
        Q_ENUM(Size)

    public:
        static AsterTheme* instance();

        // 颜色获取
        QColor color(ColorRole role) const;
        QString colorString(ColorRole role) const; // 返回 hex 字符串

        // 尺寸获取
        int borderRadius(Size size = Size::Default) const;
        int spacing(Size size = Size::Default) const;
        QFont font(Size size = Size::Default) const;

        // 主题切换 (预留接口)
        void setDarkMode(bool dark);
        bool isDarkMode() const;

    signals:
        void themeChanged();

    private:
        explicit AsterTheme(QObject* parent = nullptr);
        void initLightMode();
        void initDarkMode();

    private:
        bool m_isDarkMode;
        QHash<ColorRole, QColor> m_colors;
    };

}
