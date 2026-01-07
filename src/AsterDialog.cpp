#include "AsterUI/AsterDialog.h"
#include "AsterUI/AsterTheme.h"
#include "AsterUI/AsterButton.h"
#include "AsterUI/AsterTitle.h"
#include "AsterUI/AsterText.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>

namespace AsterUI {

    AsterDialog::AsterDialog(QWidget* parent)
        : QDialog(parent)
        , m_customContent(nullptr)
    {
        initUI();
    }

    AsterDialog::AsterDialog(const QString& title, const QString& content, QWidget* parent)
        : AsterDialog(parent)
    {
        setTitle(title);
        setContent(content);
    }

    AsterDialog::~AsterDialog() {
    }

    void AsterDialog::initUI() {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        setAttribute(Qt::WA_TranslucentBackground);
        
        // Main layout (container for shadow)
        auto rootLayout = new QVBoxLayout(this);
        rootLayout->setContentsMargins(10, 10, 10, 10);

        // Content Widget (The actual dialog background)
        auto contentWidget = new QWidget(this);
        contentWidget->setObjectName("DialogContent");
        auto shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(20);
        shadow->setOffset(0, 4);
        shadow->setColor(QColor(0, 0, 0, 40));
        contentWidget->setGraphicsEffect(shadow);
        
        rootLayout->addWidget(contentWidget);

        m_mainLayout = new QVBoxLayout(contentWidget);
        m_mainLayout->setContentsMargins(0, 0, 0, 0);
        m_mainLayout->setSpacing(0);

        // Header
        m_headerContainer = new QWidget(contentWidget);
        auto headerLayout = new QHBoxLayout(m_headerContainer);
        headerLayout->setContentsMargins(24, 20, 24, 12);
        
        m_titleLabel = new QLabel(m_headerContainer);
        auto f = m_titleLabel->font();
        f.setPixelSize(16);
        f.setBold(true);
        m_titleLabel->setFont(f);
        headerLayout->addWidget(m_titleLabel);
        // Can add close X button here
        
        m_mainLayout->addWidget(m_headerContainer);

        // Body
        m_bodyContainer = new QWidget(contentWidget);
        auto bodyLayout = new QVBoxLayout(m_bodyContainer);
        bodyLayout->setContentsMargins(24, 0, 24, 24);
        
        m_contentLabel = new QLabel(m_bodyContainer);
        m_contentLabel->setWordWrap(true);
        f = m_contentLabel->font();
        f.setPixelSize(14);
        m_contentLabel->setFont(f);
        bodyLayout->addWidget(m_contentLabel);

        m_mainLayout->addWidget(m_bodyContainer);

        // Footer
        m_footerContainer = new QWidget(contentWidget);
        auto footerLayout = new QHBoxLayout(m_footerContainer);
        footerLayout->setContentsMargins(16, 12, 16, 12);
        footerLayout->addStretch(1);

        m_btnCancel = new AsterButton("Cancel", m_footerContainer);
        connect(m_btnCancel, &AsterButton::clicked, this, &AsterDialog::reject);
        footerLayout->addWidget(m_btnCancel);

        footerLayout->addSpacing(8);

        m_btnOk = new AsterButton("OK", m_footerContainer);
        m_btnOk->setType(AsterButton::Type::Primary);
        connect(m_btnOk, &AsterButton::clicked, this, &AsterDialog::accept);
        footerLayout->addWidget(m_btnOk);

        m_mainLayout->addWidget(m_footerContainer);

        updateTheme();
        
        // Fixed width for standard dialog
        setFixedWidth(420);
    }

    void AsterDialog::updateTheme() {
        auto theme = AsterTheme::instance();
        auto bg = theme->colorString(AsterTheme::ColorRole::Surface);
        auto text = theme->colorString(AsterTheme::ColorRole::Text);
        auto border = theme->colorString(AsterTheme::ColorRole::Divider);

        QWidget* content = findChild<QWidget*>("DialogContent");
        if (content) {
            content->setStyleSheet(QString(
                "#DialogContent { background: %1; border-radius: 8px; }"
            ).arg(bg));
        }

        m_titleLabel->setStyleSheet(QString("color: %1;").arg(theme->colorString(AsterTheme::ColorRole::Text)));
        m_contentLabel->setStyleSheet(QString("color: %1;").arg(theme->colorString(AsterTheme::ColorRole::TextSecondary)));
        
        m_footerContainer->setStyleSheet(QString(
            "border-top: 1px solid %1;"
            "background: %2;"
            "border-bottom-left-radius: 8px;"
            "border-bottom-right-radius: 8px;"
        ).arg(border).arg(theme->colorString(AsterTheme::ColorRole::Background))); // Slight contrast for footer? Or same as Surface. Let's use Surface mostly but Ant uses same.
        
        // Override footer background to Surface to match
        m_footerContainer->setStyleSheet(QString(
             "border-top: 1px solid %1;"
             "border-bottom-left-radius: 8px;"
             "border-bottom-right-radius: 8px;"
        ).arg(border));
    }

    void AsterDialog::setTitle(const QString& title) {
        m_title = title;
        m_titleLabel->setText(title);
    }

    void AsterDialog::setContent(const QString& content) {
        m_content = content;
        m_contentLabel->setText(content);
        if (m_customContent) {
            m_customContent->hide();
        }
        m_contentLabel->show();
    }
    
    void AsterDialog::setContentWidget(QWidget* widget) {
         if (!widget) return;
         m_contentLabel->hide();
         
         if (m_customContent) {
             m_bodyContainer->layout()->removeWidget(m_customContent);
             delete m_customContent;
         }
         m_customContent = widget;
         m_bodyContainer->layout()->addWidget(widget);
         widget->show();
    }

    void AsterDialog::setOkText(const QString& text) {
        m_btnOk->setText(text);
    }

    void AsterDialog::setCancelText(const QString& text) {
        m_btnCancel->setText(text);
    }

    bool AsterDialog::confirm(QWidget* parent, const QString& title, const QString& content, const QString& okText, const QString& cancelText) {
        AsterDialog dlg(parent);
        dlg.setTitle(title);
        dlg.setContent(content);
        dlg.setOkText(okText);
        dlg.setCancelText(cancelText);
        return dlg.exec() == QDialog::Accepted;
    }

    void AsterDialog::alert(QWidget* parent, const QString& title, const QString& content, Type type) {
        AsterDialog dlg(parent);
        dlg.setTitle(title);
        dlg.setContent(content);
        dlg.m_btnCancel->hide(); // Alert usually just has OK
        
        // Potentially add icon based on Type
        // ... feature for later
        
        dlg.exec();
    }

    void AsterDialog::paintEvent(QPaintEvent* event) {
        // No custom painting needed, handled by QWidget stylesheet
        QDialog::paintEvent(event);
    }

    // Removed nativeEvent to avoid potential ABI issues or unnecessary hooks

    void AsterDialog::mousePressEvent(QMouseEvent* event) {
        if (!m_headerContainer) return;

        if (event->button() == Qt::LeftButton) {
            // Get header rect relative to Dialog
            QRect headerRect = m_headerContainer->geometry();
            // Need to map headerRect to Dialog coordinates because m_headerContainer is child of child.
            QPoint headerTopLeft = m_headerContainer->mapTo(this, QPoint(0,0));
            headerRect.moveTopLeft(headerTopLeft);
            
            if (headerRect.contains(event->pos())) {
                m_isDragging = true;
                // Use globalPosition() if available (Qt6), fallback to globalPos
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
#else
                m_dragPosition = event->globalPos() - frameGeometry().topLeft();
#endif
                event->accept();
            }
        }
    }

    void AsterDialog::mouseMoveEvent(QMouseEvent* event) {
        if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            move(event->globalPosition().toPoint() - m_dragPosition);
#else
            move(event->globalPos() - m_dragPosition);
#endif
            event->accept();
        }
    }

    void AsterDialog::mouseReleaseEvent(QMouseEvent* event) {
        if (m_isDragging) {
            m_isDragging = false;
        }
    }

    void AsterDialog::showEvent(QShowEvent* event) {
        QDialog::showEvent(event);
        
        // Defer animation to ensure layout is complete
        QTimer::singleShot(0, this, [this](){
            // Find the content widget for animation
            auto content = findChild<QWidget*>("DialogContent");
            if (!content) return;

            auto anim = new QPropertyAnimation(this, "windowOpacity");
            anim->setStartValue(0.0);
            anim->setEndValue(1.0);
            anim->setDuration(200);
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        });
    }

}
