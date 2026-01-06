#include "AsterUI/AsterText.h"
#include "AsterUI/AsterTheme.h"
#include <QPainter>

namespace AsterUI {

AsterText::AsterText(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    updateStyle();
    connect(AsterTheme::instance(), &AsterTheme::themeChanged, this, &AsterText::updateStyle);
}

AsterText::AsterText(QWidget *parent)
    : AsterText("", parent)
{
}

AsterText::Type AsterText::type() const
{
    return m_type;
}

void AsterText::setType(Type type)
{
    if (m_type != type) {
        m_type = type;
        updateStyle();
    }
}

void AsterText::updateStyle()
{
    auto theme = AsterTheme::instance();
    QColor color;

    switch (m_type) {
        case Type::Default:
            color = theme->color(AsterTheme::ColorRole::Text);
            break;
        case Type::Secondary:
            color = theme->color(AsterTheme::ColorRole::TextSecondary);
            break;
        case Type::Success:
            color = theme->color(AsterTheme::ColorRole::Success);
            break;
        case Type::Warning:
            color = theme->color(AsterTheme::ColorRole::Warning);
            break;
        case Type::Danger:
            color = theme->color(AsterTheme::ColorRole::Error);
            break;
        case Type::Disabled:
            color = theme->color(AsterTheme::ColorRole::TextDisabled);
            break;
    }

    QPalette pal = palette();
    pal.setColor(QPalette::WindowText, color);
    setPalette(pal);
    
    // Ensure font matches theme if not overridden
    if (!testAttribute(Qt::WA_SetFont)) {
        setFont(theme->font(AsterTheme::Size::Default));
    }
}

void AsterText::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
}

}
