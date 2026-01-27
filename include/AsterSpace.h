#pragma once

#include "Global.h"
#include <QWidget>
#include <QBoxLayout>

namespace AsterUI {

class ASTERUI_EXPORT AsterSpace : public QWidget {
    Q_OBJECT
    Q_PROPERTY(Qt::Orientation direction READ direction WRITE setDirection)
    Q_PROPERTY(int size READ size WRITE setSize)

public:
    explicit AsterSpace(QWidget *parent = nullptr);
    explicit AsterSpace(Qt::Orientation direction, QWidget *parent = nullptr);
    ~AsterSpace() override;

    Qt::Orientation direction() const;
    void setDirection(Qt::Orientation direction);

    // Size corresponds to spacing in pixels.
    // You can also use AsterTheme constants if you map them manually, 
    // but here we just take int for flexibility, defaulting to theme's default spacing.
    int size() const;
    void setSize(int size);

    void addWidget(QWidget *widget);
    void addStretch(int stretch = 0);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void init();
    void updateSpacing();

    QBoxLayout *m_layout = nullptr;
    Qt::Orientation m_direction = Qt::Horizontal;
    int m_size = 10; // Default spacing
};

}
