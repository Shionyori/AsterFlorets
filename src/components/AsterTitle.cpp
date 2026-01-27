#include "AsterTitle.h"
#include "AsterTheme.h"

namespace AsterUI {

AsterTitle::AsterTitle(const QString &text, int level, QWidget *parent)
    : QLabel(text, parent), m_level(level)
{
    updateStyle();
    connect(AsterTheme::instance(), &AsterTheme::themeChanged, this, &AsterTitle::updateStyle);
}

AsterTitle::AsterTitle(int level, QWidget *parent)
    : AsterTitle("", level, parent)
{
}

int AsterTitle::level() const
{
    return m_level;
}

void AsterTitle::setLevel(int level)
{
    if (m_level != level && level >= 1 && level <= 5) {
        m_level = level;
        updateStyle();
    }
}

void AsterTitle::updateStyle()
{
    auto theme = AsterTheme::instance();
    
    // Set color
    QPalette pal = palette();
    pal.setColor(QPalette::WindowText, theme->color(AsterTheme::ColorRole::Text));
    setPalette(pal);

    // Set Font Size based on Level
    QFont f = theme->font(AsterTheme::Size::Default);
    f.setBold(true);
    
    // Sizes based on Ant Design specification or similar
    // H1: 38px, H2: 30px, H3: 24px, H4: 20px, H5: 16px
    // Assuming base font is ~14px (Default) or 16px (Large)
    // Let's use pixel size for consistency across platforms or point size.
    // 14pt approx 18px.
    
    switch (m_level) {
        case 1: f.setPixelSize(38); break;
        case 2: f.setPixelSize(30); break;
        case 3: f.setPixelSize(24); break;
        case 4: f.setPixelSize(20); break;
        case 5: f.setPixelSize(16); break;
        default: f.setPixelSize(16); break;
    }

    setFont(f);
}

void AsterTitle::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
}

}
