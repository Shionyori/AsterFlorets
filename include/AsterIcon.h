#pragma once

#include "AsterGlobal.h"
#include <QWidget>
#include <QIcon>
#include <QColor>

namespace AsterFlorets {

class AsterFlorets_EXPORT AsterIcon : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    explicit AsterIcon(QWidget *parent = nullptr);
    explicit AsterIcon(const QIcon &icon, QWidget *parent = nullptr);
    explicit AsterIcon(const QString &fileName, QWidget *parent = nullptr);

    QIcon icon() const;
    void setIcon(const QIcon &icon);

    QSize iconSize() const;
    void setIconSize(const QSize &size);

    QColor color() const;
    void setColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    QIcon m_icon;
    QSize m_iconSize;
    QColor m_color;
};

}
