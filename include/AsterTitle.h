#pragma once

#include "AsterGlobal.h"
#include <QLabel>

namespace AsterUI {

class ASTERUI_EXPORT AsterTitle : public QLabel {
    Q_OBJECT
    Q_PROPERTY(int level READ level WRITE setLevel)

public:
    explicit AsterTitle(const QString &text, int level = 1, QWidget *parent = nullptr);
    explicit AsterTitle(int level = 1, QWidget *parent = nullptr);

    int level() const;
    void setLevel(int level);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void updateStyle();
    int m_level = 1;
};

}
