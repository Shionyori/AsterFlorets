#include "AsterFlow.h"
#include <QLayout>
#include <QList>
#include <QRect>
#include <QStyle>
#include <QWidgetItem>

namespace AsterFlorets {

    // --- Embedded AsterFlowLayout Helper Class ---
    // Moved internally to consolidate API surface.

    class AsterFlowLayout : public QLayout {
    public:
        explicit AsterFlowLayout(QWidget* parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
        ~AsterFlowLayout();

        void addItem(QLayoutItem* item) override;
        int horizontalSpacing() const;
        int verticalSpacing() const;
        Qt::Orientations expandingDirections() const override;
        bool hasHeightForWidth() const override;
        int heightForWidth(int) const override;
        int count() const override;
        QLayoutItem* itemAt(int index) const override;
        QSize minimumSize() const override;
        void setGeometry(const QRect& rect) override;
        QSize sizeHint() const override;
        QLayoutItem* takeAt(int index) override;

        void setHorizontalSpacing(int spacing);
        void setVerticalSpacing(int spacing);

    private:
        int doLayout(const QRect& rect, bool testOnly) const;
        int smartSpacing(QStyle::PixelMetric pm) const;

        QList<QLayoutItem*> itemList;
        int m_hSpace;
        int m_vSpace;
    };

    AsterFlowLayout::AsterFlowLayout(QWidget* parent, int margin, int hSpacing, int vSpacing)
        : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
    {
        setContentsMargins(margin, margin, margin, margin);
    }

    AsterFlowLayout::~AsterFlowLayout()
    {
        QLayoutItem* item;
        while ((item = takeAt(0)))
            delete item;
    }

    void AsterFlowLayout::addItem(QLayoutItem* item) {
        itemList.append(item);
    }

    int AsterFlowLayout::horizontalSpacing() const {
        if (m_hSpace >= 0) {
            return m_hSpace;
        }
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }

    int AsterFlowLayout::verticalSpacing() const {
        if (m_vSpace >= 0) {
            return m_vSpace;
        }
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }

    int AsterFlowLayout::count() const {
        return itemList.size();
    }

    QLayoutItem* AsterFlowLayout::itemAt(int index) const {
        return itemList.value(index);
    }

    QLayoutItem* AsterFlowLayout::takeAt(int index) {
        if (index >= 0 && index < itemList.size())
            return itemList.takeAt(index);
        return nullptr;
    }

    Qt::Orientations AsterFlowLayout::expandingDirections() const {
        return { };
    }

    bool AsterFlowLayout::hasHeightForWidth() const {
        return true;
    }

    int AsterFlowLayout::heightForWidth(int width) const {
        int height = doLayout(QRect(0, 0, width, 0), true);
        return height;
    }

    void AsterFlowLayout::setGeometry(const QRect& rect) {
        QLayout::setGeometry(rect);
        doLayout(rect, false);
    }

    QSize AsterFlowLayout::sizeHint() const {
        return minimumSize();
    }

    QSize AsterFlowLayout::minimumSize() const {
        QSize size;
        for (const QLayoutItem* item : itemList)
            size = size.expandedTo(item->minimumSize());

        const QMargins margins = contentsMargins();
        size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
        return size;
    }

    int AsterFlowLayout::doLayout(const QRect& rect, bool testOnly) const {
        int left, top, right, bottom;
        getContentsMargins(&left, &top, &right, &bottom);
        QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
        int x = effectiveRect.x();
        int y = effectiveRect.y();
        int lineHeight = 0;

        for (QLayoutItem* item : itemList) {
            const QWidget* wid = item->widget();
            int spaceX = horizontalSpacing();
            if (spaceX == -1)
                spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
            int spaceY = verticalSpacing();
            if (spaceY == -1)
                spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);

            int nextX = x + item->sizeHint().width() + spaceX;
            if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
                x = effectiveRect.x();
                y = y + lineHeight + spaceY;
                nextX = x + item->sizeHint().width() + spaceX;
                lineHeight = 0;
            }

            if (!testOnly)
                item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

            x = nextX;
            lineHeight = qMax(lineHeight, item->sizeHint().height());
        }
        return y + lineHeight - rect.y() + bottom;
    }

    int AsterFlowLayout::smartSpacing(QStyle::PixelMetric pm) const {
        QObject* parent = this->parent();
        if (!parent) {
            return -1;
        }
        else if (parent->isWidgetType()) {
            QWidget* pw = static_cast<QWidget*>(parent);
            return pw->style()->pixelMetric(pm, nullptr, pw);
        }
        else {
            return static_cast<QLayout*>(parent)->spacing();
        }
    }

    void AsterFlowLayout::setHorizontalSpacing(int spacing) {
        if (m_hSpace != spacing) {
            m_hSpace = spacing;
            invalidate();
        }
    }

    void AsterFlowLayout::setVerticalSpacing(int spacing) {
        if (m_vSpace != spacing) {
            m_vSpace = spacing;
            invalidate();
        }
    }


    // --- AsterFlow Implementation ---

    AsterFlow::AsterFlow(QWidget* parent)
        : QWidget(parent)
    {
        m_layout = new AsterFlowLayout(this);
        m_layout->setContentsMargins(0, 0, 0, 0); // zero margins by default
        m_layout->setHorizontalSpacing(10);
        m_layout->setVerticalSpacing(10);

        setAttribute(Qt::WA_StyledBackground, false);
        setAttribute(Qt::WA_TranslucentBackground, true);

        // Crucial for flow layout proper resizing
        QSizePolicy policy = sizePolicy();
        policy.setHeightForWidth(true);
        setSizePolicy(policy);
    }

    AsterFlow::~AsterFlow() = default;

    int AsterFlow::hSpacing() const {
        return m_layout->horizontalSpacing();
    }

    void AsterFlow::setHSpacing(int spacing) {
        m_layout->setHorizontalSpacing(spacing);
    }

    int AsterFlow::vSpacing() const {
        return m_layout->verticalSpacing();
    }

    void AsterFlow::setVSpacing(int spacing) {
        m_layout->setVerticalSpacing(spacing);
    }

    void AsterFlow::addWidget(QWidget* w) {
        m_layout->addWidget(w);
    }

    void AsterFlow::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);
    }

}
