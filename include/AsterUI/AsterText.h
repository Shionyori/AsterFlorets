#pragma once

#include "Global.h"
#include <QLabel>

namespace AsterUI {

class ASTERUI_EXPORT AsterText : public QLabel {
    Q_OBJECT
    Q_PROPERTY(Type type READ type WRITE setType)

public:
    enum class Type {
        Default,
        Secondary,
        Success,
        Warning,
        Danger,
        Disabled
    };
    Q_ENUM(Type)

    explicit AsterText(const QString &text, QWidget *parent = nullptr);
    explicit AsterText(QWidget *parent = nullptr);

    Type type() const;
    void setType(Type type);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void updateStyle();
    Type m_type = Type::Default;
};

}
