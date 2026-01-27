#include "AsterBreadcrumb.h"
#include "AsterText.h"
#include "AsterTheme.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QEvent>
#include <QVariant>
#include <QVariantAnimation>
#include <QDebug>

namespace AsterUI {

    // Helper class for clickable breadcrumb items with hover background and animation
    class BreadcrumbLabel : public AsterText {
    public:
        BreadcrumbLabel(const QString& text, QWidget* parent = nullptr) : AsterText(text, parent) {
            setContentsMargins(4, 3, 4, 2); 
            setAttribute(Qt::WA_Hover);
            setWordWrap(false);
            setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

            m_anim = new QVariantAnimation(this);
            m_anim->setDuration(200); 
            m_anim->setEasingCurve(QEasingCurve::OutQuad);
            
            // Set initial color to Placeholder
            if (auto theme = AsterTheme::instance()) {
                 QPalette p = palette();
                 p.setColor(QPalette::WindowText, theme->color(AsterTheme::ColorRole::Placeholder));
                 setPalette(p);
            }

            // Lambda to update color
            QObject::connect(m_anim, &QVariantAnimation::valueChanged, this, [this](const QVariant& val){
                 QPalette p = palette();
                 p.setColor(QPalette::WindowText, val.value<QColor>());
                 setPalette(p);
            });
        }

    protected:
        void paintEvent(QPaintEvent* event) override {
            if (m_hovered && isEnabled()) {
                QPainter p(this);
                p.setRenderHint(QPainter::Antialiasing);
                QColor bg = AsterTheme::instance()->color(AsterTheme::ColorRole::Text);
                bg.setAlpha(20); 
                p.setBrush(bg);
                p.setPen(Qt::NoPen);
                p.drawRoundedRect(rect(), 4, 4);
            }
            AsterText::paintEvent(event);
        }

        bool event(QEvent* e) override {
            if (e->type() == QEvent::HoverEnter) {
                m_hovered = true;
                animateColor(true);
            } else if (e->type() == QEvent::HoverLeave) {
                m_hovered = false;
                animateColor(false);
            }
            return AsterText::event(e);
        }

    private:
        void animateColor(bool hover) {
            m_anim->stop();
            auto theme = AsterTheme::instance();
            QColor placeholder = theme->color(AsterTheme::ColorRole::Placeholder);
            QColor textDefault = theme->color(AsterTheme::ColorRole::Text); 

            m_anim->setStartValue(palette().color(QPalette::WindowText));
            m_anim->setEndValue(hover ? textDefault : placeholder);
            m_anim->start();
        }

        bool m_hovered = false;
        QVariantAnimation* m_anim;
    };

    AsterBreadcrumb::AsterBreadcrumb(QWidget* parent)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        auto *l = new QHBoxLayout(this);
        l->setContentsMargins(0, 0, 0, 0);
        l->setSpacing(8);
        l->setAlignment(Qt::AlignLeft);
    }

    AsterBreadcrumb::~AsterBreadcrumb() = default;

    void AsterBreadcrumb::setSeparator(const QString& separator) {
        if (m_separator != separator) {
            m_separator = separator;
            rebuildLayout();
        }
    }

    void AsterBreadcrumb::addItem(const QString& text, std::function<void()> onClick) {
        m_items.append({text, onClick});
        rebuildLayout();
    }
    
    int AsterBreadcrumb::count() const {
        return m_items.size();
    }

    void AsterBreadcrumb::clear() {
        m_items.clear();
        rebuildLayout();
    }

    void AsterBreadcrumb::rebuildLayout() {
        // Clear layout
        QLayout* lay = layout();
        QLayoutItem* child;
        while ((child = lay->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->deleteLater(); // Use deleteLater to avoid issues during event processing
            }
            delete child;
        }

        for (int i = 0; i < m_items.size(); ++i) {
            const auto& item = m_items[i];
            bool isLast = (i == m_items.size() - 1);

            // Item Widget
            AsterText* label;
            
            if (!isLast) {
                 // Link style items use our custom BreadcrumbLabel for hover effect
                 label = new BreadcrumbLabel(item.text);
                 label->setCursor(Qt::PointingHandCursor);
                 
                 // Make it clickable via Breadcrumb's event filter
                 label->installEventFilter(this);
                 label->setProperty("breadcrumb_index", i);
            } else {
                 // Last item - Active (Black)
                 label = new AsterText(item.text);
                 label->setContentsMargins(6, 1, 6, 1); 
                 label->setType(AsterText::Type::Default); 
                 label->setWordWrap(false);
                 label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
            }

            lay->addWidget(label);

            // Separator
            if (!isLast) {
                auto* sep = new AsterText(m_separator);
                sep->setContentsMargins(0, 0, 0, 0);
                
                // Set separator to Placeholder color
                if (auto theme = AsterTheme::instance()) {
                    QPalette p = sep->palette();
                    p.setColor(QPalette::WindowText, theme->color(AsterTheme::ColorRole::Placeholder));
                    sep->setPalette(p);
                }

                sep->setWordWrap(false);
                sep->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                lay->addWidget(sep);
            }
        }
    }
    
    bool AsterBreadcrumb::eventFilter(QObject* watched, QEvent* event) {
        if (event->type() == QEvent::MouseButtonRelease) {
            bool ok = false;
            int idx = watched->property("breadcrumb_index").toInt(&ok);
            if (ok && idx >= 0 && idx < m_items.size()) {
                if (m_items[idx].onClick) {
                    m_items[idx].onClick();
                }
                emit itemClicked(idx, m_items[idx].text);
                return true;
            }
        }
        return QWidget::eventFilter(watched, event);
    }

}
