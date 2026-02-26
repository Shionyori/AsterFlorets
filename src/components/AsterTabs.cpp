#include "AsterTabs.h"
#include "AsterTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QVariantAnimation>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QDebug>

namespace AsterFlorets {

    struct TabItem {
        QString label;
        QIcon icon;
        QRect rect; // Calculated during layout/paint
        bool enabled = true;
    };

    // ========================================================================
    // AsterTabBar (Internal)
    // ========================================================================
    class AsterTabBar : public QWidget {
        Q_OBJECT
        Q_PROPERTY(qreal inkPos READ inkPos WRITE setInkPos)
        Q_PROPERTY(qreal inkWidth READ inkWidth WRITE setInkWidth)

    public:
        explicit AsterTabBar(QWidget* parent = nullptr) : QWidget(parent) {
            setFixedHeight(46); // Standard tab bar height
            setMouseTracking(true);
            setFocusPolicy(Qt::StrongFocus);
            setAccessibleName("Aster Tabs");
            setAccessibleDescription("Use arrow keys to switch tabs");
            
            // Animation for sliding underline
            m_anim = new QParallelAnimationGroup(this);
            m_posAnim = new QPropertyAnimation(this, "inkPos");
            m_posAnim->setDuration(250);
            m_posAnim->setEasingCurve(QEasingCurve::OutQuad);
            
            m_widthAnim = new QPropertyAnimation(this, "inkWidth");
            m_widthAnim->setDuration(250);
            m_widthAnim->setEasingCurve(QEasingCurve::OutQuad);

            m_anim->addAnimation(m_posAnim);
            m_anim->addAnimation(m_widthAnim);
        }

        void addTab(const QIcon& icon, const QString& label) {
            TabItem item;
            item.icon = icon;
            item.label = label;
            m_tabs.append(item);
            updateLayout();

            if (m_tabs.size() == 1) {
                m_currentIndex = 0;
                m_inkPos = m_tabs[0].rect.x();
                m_inkWidth = m_tabs[0].rect.width();
            }

            update();
        }

        void removeTab(int index) {
            if (index >= 0 && index < m_tabs.size()) {
                m_tabs.removeAt(index);
                if (m_currentIndex >= m_tabs.size()) {
                    m_currentIndex = qMax(0, m_tabs.size() - 1);
                    emit currentChanged(m_currentIndex);
                }
                updateLayout();
                update();
            }
        }

        int currentIndex() const { return m_currentIndex; }

        void setCurrentIndex(int index) {
            if (index == m_currentIndex) return;
            if (index < 0 || index >= m_tabs.size()) return;
            
            m_currentIndex = index;
            emit currentChanged(m_currentIndex);
            animateInk();
            update();
        }

        void setTabText(int index, const QString& text) {
             if (index >= 0 && index < m_tabs.size()) {
                 m_tabs[index].label = text;
                 updateLayout();
                 update();
             }
        }
        
        QString tabText(int index) const {
             if (index >= 0 && index < m_tabs.size()) {
                 return m_tabs[index].label;
             }
             return QString();
        }

        // Properties for animation
        qreal inkPos() const { return m_inkPos; }
        void setInkPos(qreal p) { m_inkPos = p; update(); }
        
        qreal inkWidth() const { return m_inkWidth; }
        void setInkWidth(qreal w) { m_inkWidth = w; update(); }

    signals:
        void currentChanged(int index);

    protected:
        void paintEvent(QPaintEvent* event) override {
            QPainter p(this);
            p.setRenderHint(QPainter::Antialiasing);

            auto theme = AsterTheme::instance();
            QColor primary = theme->color(AsterTheme::ColorRole::Primary);
            QColor textPrim = theme->color(AsterTheme::ColorRole::Text);
            QColor textSec = theme->color(AsterTheme::ColorRole::TextSecondary);
            QColor border = theme->color(AsterTheme::ColorRole::Divider);

            // Draw bottom border line
            p.setPen(QPen(border, 1));
            p.drawLine(0, height() - 1, width(), height() - 1);

            // Draw Tabs
            QFont font = p.font();
            font.setPixelSize(14);
            
            for (int i = 0; i < m_tabs.size(); ++i) {
                const auto& item = m_tabs[i];
                QRect r = item.rect;
                
                bool isSelected = (i == m_currentIndex);
                bool isHovered = (i == m_hoverIndex);

                // Text Color
                QColor tc = textSec;
                if (isSelected) {
                    tc = primary;
                } else if (isHovered) {
                    tc = primary.lighter(120); // Hover color
                }
                
                p.setPen(tc);
                
                // Font weight
                // if (isSelected) font.setBold(true); else font.setBold(false);
                p.setFont(font);

                const int iconSize = 16;
                const int gap = 6;
                const bool hasIcon = !item.icon.isNull();
                const int textWidth = QFontMetrics(font).horizontalAdvance(item.label);
                const int contentWidth = hasIcon ? (iconSize + gap + textWidth) : textWidth;

                int contentX = r.x() + (r.width() - contentWidth) / 2;

                if (hasIcon) {
                    QRect iconRect(contentX, r.y() + (r.height() - iconSize) / 2, iconSize, iconSize);
                    item.icon.paint(&p, iconRect);
                    contentX += iconSize + gap;
                }

                QRect textRect(contentX, r.y(), textWidth, r.height());
                p.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, item.label);

                if (hasFocus() && isSelected) {
                    QColor focusColor = primary;
                    focusColor.setAlpha(70);
                    QPen focusPen(focusColor);
                    focusPen.setWidth(2);
                    p.setPen(focusPen);
                    p.setBrush(Qt::NoBrush);
                    QRect focusRect = r.adjusted(3, 8, -3, -8);
                    p.drawRoundedRect(focusRect, 5, 5);
                }
            }

            // Draw Ink Bar
            if (m_tabs.isEmpty()) return;

            qreal barHeight = 2.0;
            QRectF barRect(m_inkPos, height() - barHeight, m_inkWidth, barHeight);
            
            p.setBrush(primary);
            p.setPen(Qt::NoPen);
            p.drawRoundedRect(barRect, 1, 1);
        }

        void resizeEvent(QResizeEvent* event) override {
            updateLayout();
            // Reset ink position without animation on resize to avoid jumping
            if (m_currentIndex >= 0 && m_currentIndex < m_tabs.size()) {
                QRect r = m_tabs[m_currentIndex].rect;
                m_inkPos = r.x();
                m_inkWidth = r.width();
            }
            QWidget::resizeEvent(event);
        }

        void mouseMoveEvent(QMouseEvent* event) override {
            int oldHover = m_hoverIndex;
            m_hoverIndex = -1;
            for (int i = 0; i < m_tabs.size(); ++i) {
                if (m_tabs[i].rect.contains(event->pos())) {
                    m_hoverIndex = i;
                    setCursor(Qt::PointingHandCursor);
                    break;
                }
            }
            if (m_hoverIndex == -1) setCursor(Qt::ArrowCursor);
            if (oldHover != m_hoverIndex) update();
        }

        void leaveEvent(QEvent* event) override {
            if (m_hoverIndex != -1) {
                m_hoverIndex = -1;
                update();
            }
            QWidget::leaveEvent(event);
        }

        void mousePressEvent(QMouseEvent* event) override {
            for (int i = 0; i < m_tabs.size(); ++i) {
                if (m_tabs[i].rect.contains(event->pos())) {
                    if (i != m_currentIndex) {
                        setCurrentIndex(i);
                    }
                    setFocus(Qt::MouseFocusReason);
                    break;
                }
            }
        }

        void keyPressEvent(QKeyEvent* event) override {
            if (m_tabs.isEmpty()) {
                QWidget::keyPressEvent(event);
                return;
            }

            int target = m_currentIndex;
            switch (event->key()) {
                case Qt::Key_Left:
                case Qt::Key_Up:
                    target = qMax(0, m_currentIndex - 1);
                    break;
                case Qt::Key_Right:
                case Qt::Key_Down:
                    target = qMin(m_tabs.size() - 1, m_currentIndex + 1);
                    break;
                case Qt::Key_Home:
                    target = 0;
                    break;
                case Qt::Key_End:
                    target = m_tabs.size() - 1;
                    break;
                case Qt::Key_Return:
                case Qt::Key_Enter:
                case Qt::Key_Space:
                    setCurrentIndex(m_currentIndex);
                    event->accept();
                    return;
                default:
                    QWidget::keyPressEvent(event);
                    return;
            }

            if (target != m_currentIndex) {
                setCurrentIndex(target);
            }
            event->accept();
        }

    private:
        void updateLayout() {
            int x = 0; // Left padding
            // Calculate widths
            QFont font;
            font.setPixelSize(14);
            QFontMetrics fm(font);
            
            int spacing = 32; // Gap between tabs
            const int iconSize = 16;
            const int iconGap = 6;

            for (int i = 0; i < m_tabs.size(); ++i) {
                int w = fm.horizontalAdvance(m_tabs[i].label) + spacing;
                if (!m_tabs[i].icon.isNull()) {
                    w += iconSize + iconGap;
                }
                m_tabs[i].rect = QRect(x, 0, w, height());
                x += w; 
            }
        }

        void animateInk() {
            if (m_currentIndex < 0 || m_currentIndex >= m_tabs.size()) return;
            
            QRect target = m_tabs[m_currentIndex].rect;
            
            // Should ink bar be full width of tab?
            // Usually ink bar is slightly smaller or matches text width. 
            // For simplicity, let's match the visual tab width (which includes padding in our calculation above)
            // Or better: match text width + constant padding.
            // Current rect calculation is: text width + 32. 
            // Let's make the ink bar width = rect width.
            
            m_anim->stop();
            
            m_posAnim->setStartValue(m_inkPos);
            m_posAnim->setEndValue((qreal)target.x());
            
            m_widthAnim->setStartValue(m_inkWidth);
            m_widthAnim->setEndValue((qreal)target.width());
            
            m_anim->start();
        }

        QList<TabItem> m_tabs;
        int m_currentIndex = 0;
        int m_hoverIndex = -1;

        // Animation state
        qreal m_inkPos = 0;
        qreal m_inkWidth = 0;
        QParallelAnimationGroup* m_anim;
        QPropertyAnimation* m_posAnim;
        QPropertyAnimation* m_widthAnim;
    };

    // ========================================================================
    // AsterTabs
    // ========================================================================

    AsterTabs::AsterTabs(QWidget* parent)
        : QWidget(parent)
    {
        auto* l = new QVBoxLayout(this);
        l->setContentsMargins(0, 0, 0, 0);
        l->setSpacing(0);

        m_tabBar = new AsterTabBar(this);
        // Connect tab bar signal
        connect(m_tabBar, &AsterTabBar::currentChanged, this, &AsterTabs::setCurrentIndex);

        m_stack = new QStackedWidget(this);
        // Stack styling: typically clear
        
        l->addWidget(m_tabBar);
        l->addWidget(m_stack);
    }

    AsterTabs::~AsterTabs() = default;

    int AsterTabs::addTab(QWidget* page, const QString& label)
    {
        return addTab(page, QIcon(), label);
    }

    int AsterTabs::addTab(QWidget* page, const QIcon& icon, const QString& label)
    {
        m_stack->addWidget(page);
        m_tabBar->addTab(icon, label);
        return m_stack->count() - 1;
    }

    void AsterTabs::removeTab(int index) {
        QWidget* w = m_stack->widget(index);
        if (w) {
            m_stack->removeWidget(w);
            m_tabBar->removeTab(index);
            // 与 QTabWidget 一致：removeTab 不负责销毁页面对象。
        }
    }

    int AsterTabs::count() const {
        return m_stack->count();
    }

    int AsterTabs::currentIndex() const {
        return m_stack->currentIndex();
    }

    QWidget* AsterTabs::currentWidget() const {
        return m_stack->currentWidget();
    }

    QWidget* AsterTabs::widget(int index) const {
        return m_stack->widget(index);
    }

    QString AsterTabs::tabText(int index) const {
        return m_tabBar->tabText(index);
    }
    
    void AsterTabs::setTabText(int index, const QString& text) {
        m_tabBar->setTabText(index, text);
    }

    void AsterTabs::setCurrentIndex(int index) {
        if (index != m_stack->currentIndex()) {
            m_stack->setCurrentIndex(index);
            m_tabBar->setCurrentIndex(index);
            emit currentChanged(index);
        }
    }

    void AsterTabs::setCurrentWidget(QWidget* widget) {
        int idx = m_stack->indexOf(widget);
        if (idx != -1) {
            setCurrentIndex(idx);
        }
    }

}

#include "AsterTabs.moc"
