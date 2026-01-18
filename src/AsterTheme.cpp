#include "AsterUI/AsterTheme.h"
#include <QApplication>
#include <QPalette>

namespace AsterUI {

    AsterTheme* AsterTheme::instance() {
        static AsterTheme* _instance = new AsterTheme(qApp);
        return _instance;
    }

    AsterTheme::AsterTheme(QObject* parent) : QObject(parent), m_isDarkMode(false) {
        initLightMode();
    }

    QColor AsterTheme::color(ColorRole role) const {
        return m_colors.value(role, Qt::red); // 默认红色以便发现错误
    }

    QString AsterTheme::colorString(ColorRole role) const {
        return color(role).name();
    }

    int AsterTheme::borderRadius(Size size) const {
        switch (size) {
            case Size::Small: return 2;
            case Size::Default: return 4; // Ant Design 默认圆角
            case Size::Large: return 8;
            default: return 4;
        }
    }

    int AsterTheme::spacing(Size size) const {
        switch (size) {
            case Size::Small: return 8;
            case Size::Default: return 16;
            case Size::Large: return 24;
            default: return 16;
        }
    }

    QFont AsterTheme::font(Size size) const {
        QFont f = QApplication::font();
        switch (size) {
            case Size::Small: f.setPixelSize(12); break;
            case Size::Default: f.setPixelSize(14); break;
            case Size::Large: f.setPixelSize(16); break;
        }
        return f;
    }

    void AsterTheme::setDarkMode(bool dark) {
        if (m_isDarkMode == dark) return;
        m_isDarkMode = dark;
        if (m_isDarkMode) {
            initDarkMode();
        } else {
            initLightMode();
        }
        emit themeChanged();
    }

    bool AsterTheme::isDarkMode() const {
        return m_isDarkMode;
    }

    void AsterTheme::initLightMode() {
        // Shion Yorigami Theme (Light)
        // 核心色：蓝紫色 (Indigo/Violet)
        
        // Primary Palette
        m_colors[ColorRole::Primary]       = QColor("#5E5CE6"); // Shion Violet
        m_colors[ColorRole::PrimaryHover]  = QColor("#7A78EA"); // Lighter
        m_colors[ColorRole::PrimaryActive] = QColor("#4A48B8"); // Darker

        // Functional
        m_colors[ColorRole::Success] = QColor("#52C41A");
        m_colors[ColorRole::Warning] = QColor("#FAAD14");
        m_colors[ColorRole::Error]   = QColor("#FF4D4F");
        m_colors[ColorRole::Info]    = QColor("#1890FF");

        // Neutral
        m_colors[ColorRole::Text]          = QColor(0, 0, 0, 217); // 85%
        m_colors[ColorRole::TextSecondary] = QColor(0, 0, 0, 166); // 65%
        m_colors[ColorRole::Placeholder]   = QColor(0, 0, 0, 115); // 45%
        m_colors[ColorRole::TextDisabled]  = QColor(0, 0, 0, 64); // 25%
        
        m_colors[ColorRole::Background]    = QColor("#FFFFFF");
        m_colors[ColorRole::Surface]       = QColor("#FFFFFF");
        m_colors[ColorRole::Border]        = QColor("#D9D9D9");
        m_colors[ColorRole::Divider]       = QColor("#F0F0F0");

        // Layout Areas (Empty States)
        m_colors[ColorRole::LayoutHeader]   = QColor("#F5F7FA");
        m_colors[ColorRole::LayoutSider]    = QColor("#F5F7FA");
        m_colors[ColorRole::LayoutContent]  = QColor("#F5F7FA"); 
        m_colors[ColorRole::LayoutFooter]   = QColor("#F5F7FA");

        // Tag Colors
        m_colors[ColorRole::TagBackground]  = QColor("#F5F5F5"); // Default Tag Background
        m_colors[ColorRole::TagBorder]      = QColor("#D9D9D9"); // Default Tag Border

        m_colors[ColorRole::Mask]           = QColor(0, 0, 0, 115); // 45% Black
    }

    void AsterTheme::initDarkMode() {
        // TODO: Implement Dark Mode based on Shion's dark aura
        // Placeholder for now
        initLightMode(); 
    }

}
