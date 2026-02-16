#include "AsterResult.h"
#include "AsterTheme.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>

namespace AsterFlorets {

    AsterResult::AsterResult(QWidget* parent)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        // Central layout
        auto* layout = new QVBoxLayout(this);
        layout->setAlignment(Qt::AlignCenter);
        layout->setSpacing(24);
        
        // We handle painting mostly, but "extra" widget is added to layout
        // Actually, let's just use layout for everything to keep it simple
    }

    AsterResult::~AsterResult() = default;

    void AsterResult::setStatus(Status status) {
        if (m_status != status) {
            m_status = status;
            update();
        }
    }

    AsterResult::Status AsterResult::status() const {
        return m_status;
    }

    void AsterResult::setTitle(const QString& title) {
        m_title = title;
        update();
    }

    QString AsterResult::title() const {
        return m_title;
    }

    void AsterResult::setSubTitle(const QString& subTitle) {
        m_subTitle = subTitle;
        update();
    }

    QString AsterResult::subTitle() const {
        return m_subTitle;
    }

    void AsterResult::setExtra(QWidget* widget) {
        if (m_extra) {
            layout()->removeWidget(m_extra);
            m_extra->deleteLater();
        }
        m_extra = widget;
        if (m_extra) {
            layout()->addWidget(m_extra);
            m_extra->show();
        }
    }

    void AsterResult::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int centX = width() / 2;
        int currentY = 40; // Top padding

        // 1. Draw Icon
        int iconSize = 72;
        QRect iconRect(centX - iconSize/2, currentY, iconSize, iconSize);
        
        QColor iconColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Primary);
        QString iconText = "i";

        switch (m_status) {
            case Status::Success: 
                iconColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Success); 
                iconText = "V";
                break;
            case Status::Error: 
            case Status::ServerError:
                iconColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Error); 
                iconText = "X";
                break;
            case Status::Warning:
                iconColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Warning); 
                iconText = "!";
                break;
            case Status::NotFound:
                iconColor = Qt::gray;
                iconText = "404";
                break;
            case Status::Forbidden:
                iconColor = Qt::gray;
                iconText = "403";
                break;
             default: break;
        }

        // Placeholder Icon
        // In real app, load SVG
        p.setBrush(iconColor);
        p.setPen(Qt::NoPen);
        p.drawEllipse(iconRect);
        
        p.setPen(Qt::white);
        QFont f = font();
        f.setPixelSize(24);
        f.setBold(true);
        p.setFont(f);
        p.drawText(iconRect, Qt::AlignCenter, iconText);

        currentY += iconSize + 24;

        // 2. Draw Title
        if (!m_title.isEmpty()) {
            f.setPixelSize(24);
            f.setBold(true);
            p.setFont(f);
            p.setPen(AsterTheme::instance()->color(AsterTheme::ColorRole::Text));
            
            QRect titleRect(20, currentY, width() - 40, 40);
            p.drawText(titleRect, Qt::AlignCenter | Qt::TextWordWrap, m_title);
            
            // Measure actual height for wrapping
            currentY += 40 + 16; 
        }

        // 3. Draw SubTitle
        if (!m_subTitle.isEmpty()) {
            f.setPixelSize(14);
            f.setBold(false);
            p.setFont(f);
            p.setPen(AsterTheme::instance()->color(AsterTheme::ColorRole::TextSecondary));
            
            QRect subRect(40, currentY, width() - 80, 60);
            p.drawText(subRect, Qt::AlignTop | Qt::AlignHCenter | Qt::TextWordWrap, m_subTitle);
            
             currentY += 60 + 24;
        }

        // Extra widget position is handled by layout, but we need to ensure layout starts after painted content?
        // Actually mixed approach is bad.
        // Better: Use standard Draw for everything OR Layout for everything.
        // Let's rely on Layout for "Extra" buttons, implying they appear at bottom.
        // To make layout respect painted area, we can set spacing or custom margin on top element.
        // But since we are painting freely, layout items might overlap if we are not careful.
        // FIX: The layout contains ONLY the 'extra' widget. We set the layout's top margin to `currentY`.
        
        if (layout()) {
            layout()->setContentsMargins(0, currentY, 0, 0);
        }
    }

}
