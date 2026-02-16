#include "AsterCollapse.h"
#include "AsterTheme.h"
#include "AsterIcon.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QLabel>
#include <QToolButton>
#include <QScrollArea>

namespace AsterFlorets {

    // ========================================================================
    // AsterCollapsePanel (Internal)
    // ========================================================================
    class AsterCollapsePanel : public QWidget {
        Q_OBJECT
        Q_PROPERTY(int contentHeight READ contentHeight WRITE setContentHeight)
    public:
        AsterCollapsePanel(const QString& title, QWidget* content, QWidget* parent = nullptr)
            : QWidget(parent), m_content(content)
        {
            auto mainLayout = new QVBoxLayout(this);
            mainLayout->setContentsMargins(0, 0, 0, 0);
            mainLayout->setSpacing(0);

            // Header
            m_header = new QWidget(this);
            m_header->setFixedHeight(40);
            m_header->setCursor(Qt::PointingHandCursor);
            auto headerLayout = new QHBoxLayout(m_header);
            headerLayout->setContentsMargins(12, 0, 12, 0);
            
            // Arrow (Triangle)
            m_arrow = new QLabel(">"); // Simplified arrow
            m_arrow->setStyleSheet("color: gray; font-family: Consolas;"); 
            headerLayout->addWidget(m_arrow);

            auto titleLabel = new QLabel(title);
            QFont f = titleLabel->font();
            f.setBold(false);
            titleLabel->setFont(f);
            headerLayout->addWidget(titleLabel);
            headerLayout->addStretch();
            
            mainLayout->addWidget(m_header);

            // Content Container (ScrollArea for clipping)
            m_scrollArea = new QScrollArea(this);
            m_scrollArea->setFrameShape(QFrame::NoFrame);
            m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_scrollArea->setWidgetResizable(true);
            m_scrollArea->setFixedHeight(0); // Start collapsed
            
            // Fix: Set transparent background for ScrollArea so parent background shows through
            // Or set specifically to Surface color
            m_scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; } QWidget { background: transparent; }");
            m_scrollArea->viewport()->setStyleSheet("background: transparent;");
            
            // We need a wrapper widget for layout if we use setWidget
            QWidget* wrapper = new QWidget();
            // Fix: Ensure wrapper is transparent or has correct background
            wrapper->setStyleSheet("background: transparent;"); 
            
            auto contentLayout = new QVBoxLayout(wrapper);
            contentLayout->setContentsMargins(12, 12, 12, 12);
            if (m_content) {
                contentLayout->addWidget(m_content);
            }
            m_scrollArea->setWidget(wrapper);
            
            mainLayout->addWidget(m_scrollArea);

            // Events
            m_header->installEventFilter(this);

            // Animation
            m_anim = new QPropertyAnimation(this, "contentHeight", this);
            m_anim->setDuration(200);
            m_anim->setEasingCurve(QEasingCurve::InOutQuad);
        }

        bool isExpanded() const { return m_expanded; }

        void setExpanded(bool expanded) {
            if (m_expanded == expanded) return;
            m_expanded = expanded;
            
            // Calculate height of content
            m_scrollArea->widget()->adjustSize();
            int contentH = m_scrollArea->widget()->sizeHint().height(); // margins included in wrapper
            
            int startData = m_scrollArea->height();
            int endData = expanded ? contentH : 0;
            
            m_anim->stop();
            m_anim->setStartValue(startData);
            m_anim->setEndValue(endData);
            m_anim->start();

            // Rotate arrow?
            m_arrow->setText(expanded ? "v" : ">");
            
            emit toggled(expanded);
        }

        void setContentHeight(int h) {
            m_scrollArea->setFixedHeight(h);
        }

        int contentHeight() const {
            return m_scrollArea->height();
        }

    signals:
        void toggled(bool expanded);

    protected:
        bool eventFilter(QObject* o, QEvent* e) override {
            if (o == m_header && e->type() == QEvent::MouseButtonRelease) {
                setExpanded(!m_expanded);
                return true;
            }
            return QWidget::eventFilter(o, e);
        }

        void paintEvent(QPaintEvent* event) override {
            Q_UNUSED(event);
            QPainter p(this);
            
            // 绘制背景，防止展开过程中出现黑色伪影
            p.fillRect(rect(), AsterTheme::instance()->color(AsterTheme::ColorRole::Surface));

            // Draw bottom border
            p.setPen(QPen(AsterTheme::instance()->color(AsterTheme::ColorRole::Border), 1));
            p.drawLine(rect().bottomLeft(), rect().bottomRight());
        }

    private:
        QWidget* m_header;
        QLabel* m_arrow;
        QScrollArea* m_scrollArea; // Replaces m_contentContainer
        QWidget* m_content;
        bool m_expanded = false;
        QPropertyAnimation* m_anim;
    };


    // ========================================================================
    // AsterCollapse
    // ========================================================================

    AsterCollapse::AsterCollapse(QWidget* parent)
        : QWidget(parent)
    {
        m_layout = new QVBoxLayout(this);
        m_layout->setContentsMargins(0, 0, 0, 0);
        m_layout->setSpacing(0);
        
        // Border
        setStyleSheet(QString("AsterCollapse { border: 1px solid %1; border-radius: 4px; border-bottom: none; }")
            .arg(AsterTheme::instance()->color(AsterTheme::ColorRole::Border).name()));
    }

    AsterCollapse::~AsterCollapse() = default;

    void AsterCollapse::addPanel(const QString& title, QWidget* content) {
        auto* panel = new AsterCollapsePanel(title, content, this);
        connect(panel, &AsterCollapsePanel::toggled, this, [this, panel](bool expanded){
            if (m_accordion && expanded) {
                // Collapse others
                for (auto* p : m_panels) {
                    if (p != panel && p->isExpanded()) {
                        p->setExpanded(false);
                    }
                }
            }
        });
        m_layout->addWidget(panel);
        m_panels.append(panel);
    }

    void AsterCollapse::removePanel(int index) {
        if (index >= 0 && index < m_panels.size()) {
            auto* p = m_panels.takeAt(index);
            m_layout->removeWidget(p);
            delete p;
        }
    }

    void AsterCollapse::setAccordion(bool accordion) {
        m_accordion = accordion;
    }

    bool AsterCollapse::isAccordion() const {
        return m_accordion;
    }

    void AsterCollapse::onPanelToggled(bool expanded) {
        // Connected via lambda above
    }

}

#include "AsterCollapse.moc"
