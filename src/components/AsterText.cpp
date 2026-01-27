#include "AsterText.h"
#include "AsterTheme.h"
#include <QPainter>

namespace AsterFlorets {

AsterText::AsterText(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    // Enable word wrap by default to ensure text flows correctly in layouts
    setWordWrap(true);
    // Ensure the label tries to be at least as tall as its content
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    
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
