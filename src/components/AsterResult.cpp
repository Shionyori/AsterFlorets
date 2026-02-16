#include "AsterResult.h"
#include "AsterTheme.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QIcon>

namespace AsterFlorets {

    // Internal Icon Widget
    class AsterResultIcon : public QWidget {
    public:
        AsterResultIcon(QWidget* parent = nullptr) : QWidget(parent) {
            setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            updateSize();
        }

        void setStatus(AsterResult::Status status) {
            if (m_status != status) {
                m_status = status;
                update();
            }
        }
        
        QSize sizeHint() const override {
            return QSize(m_size, m_size);
        }

        void setIconSize(int size) {
            if (m_size != size) {
                m_size = size;
                updateSize();
                update();
            }
        }

    protected:
        void paintEvent(QPaintEvent*) override {
            QPainter p(this);
            p.setRenderHint(QPainter::Antialiasing);

            QRect r = rect();
            QColor iconColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Primary);
            QString iconText = "i";

            switch (m_status) {
                case AsterResult::Status::Success: 
                    iconColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Success); 
                    iconText = "V"; // Checkmark
                    break;
                case AsterResult::Status::Error: 
                case AsterResult::Status::ServerError:
                    iconColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Error); 
                    iconText = "X"; // Cross
                    break;
                case AsterResult::Status::Warning:
                    iconColor = AsterTheme::instance()->color(AsterTheme::ColorRole::Warning); 
                    iconText = "!";
                    break;
                case AsterResult::Status::NotFound:
                    iconColor = Qt::gray;
                    iconText = "404";
                    break;
                case AsterResult::Status::Forbidden:
                    iconColor = Qt::gray;
                    iconText = "403";
                    break;
                 default: break;
            }

            p.setBrush(iconColor);
            p.setPen(Qt::NoPen);
            p.drawEllipse(r);
            
            p.setPen(Qt::white);
            QFont f = font();
            f.setPixelSize(m_size / 3);
            f.setBold(true);
            p.setFont(f);
            p.drawText(r, Qt::AlignCenter, iconText);
        }

    private:
        void updateSize() {
            setFixedSize(m_size, m_size);
        }
        AsterResult::Status m_status = AsterResult::Status::Info;
        int m_size = 72;
    };

    AsterResult::AsterResult(QWidget* parent)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        
        // Use Layout System
        auto* mainLay = new QVBoxLayout(this);
        mainLay->setAlignment(Qt::AlignCenter);
        mainLay->setSpacing(16);
        mainLay->setContentsMargins(24, 24, 24, 24); // Padding
        m_layout = mainLay;

        // 1. Icon
        auto* iconWrapper = new AsterResultIcon(this);
        m_iconWidget = iconWrapper;
        mainLay->addWidget(m_iconWidget, 0, Qt::AlignCenter);

        // 2. Title
        m_titleLabel = new QLabel(this);
        m_titleLabel->setAlignment(Qt::AlignCenter);
        m_titleLabel->setWordWrap(true);
        QFont ft = font();
        ft.setPixelSize(24);
        ft.setBold(true);
        m_titleLabel->setFont(ft);
        // Palette for text color
        QPalette pt = m_titleLabel->palette();
        pt.setColor(QPalette::WindowText, AsterTheme::instance()->color(AsterTheme::ColorRole::Text));
        m_titleLabel->setPalette(pt);
        mainLay->addWidget(m_titleLabel);

        // 3. SubTitle
        m_subLabel = new QLabel(this);
        m_subLabel->setAlignment(Qt::AlignCenter);
        m_subLabel->setWordWrap(true);
        QFont fs = font();
        fs.setPixelSize(14);
        m_subLabel->setFont(fs);
        QPalette ps = m_subLabel->palette();
        ps.setColor(QPalette::WindowText, AsterTheme::instance()->color(AsterTheme::ColorRole::TextSecondary));
        m_subLabel->setPalette(ps);
        mainLay->addWidget(m_subLabel);
        
        // Hide empty labels initially
        m_titleLabel->hide();
        m_subLabel->hide();
    }

    AsterResult::~AsterResult() = default;

    void AsterResult::setStatus(Status status) {
        if (m_status != status) {
            m_status = status;
            static_cast<AsterResultIcon*>(m_iconWidget)->setStatus(status);
        }
    }

    AsterResult::Status AsterResult::status() const {
        return m_status;
    }

    void AsterResult::setTitle(const QString& title) {
        m_titleLabel->setText(title);
        m_titleLabel->setVisible(!title.isEmpty());
    }

    QString AsterResult::title() const {
        return m_titleLabel->text();
    }

    void AsterResult::setSubTitle(const QString& subTitle) {
        m_subLabel->setText(subTitle);
        m_subLabel->setVisible(!subTitle.isEmpty());
    }

    QString AsterResult::subTitle() const {
        return m_subLabel->text();
    }

    void AsterResult::setExtra(QWidget* widget) {
        if (m_extraWidget) {
            m_layout->removeWidget(m_extraWidget);
            m_extraWidget->deleteLater();
        }
        m_extraWidget = widget;
        if (m_extraWidget) {
            m_layout->addWidget(m_extraWidget, 0, Qt::AlignCenter);
            m_extraWidget->show();
        }
    }

    void AsterResult::updateIcon() {
        // ...
    }

}
