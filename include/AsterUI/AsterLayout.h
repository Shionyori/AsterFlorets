#pragma once

#include "Global.h"
#include <QWidget>
#include <QBoxLayout>

namespace AsterUI {

class ASTERUI_EXPORT AsterHeader : public QWidget {
    Q_OBJECT
public:
    explicit AsterHeader(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

class ASTERUI_EXPORT AsterFooter : public QWidget {
    Q_OBJECT
public:
    explicit AsterFooter(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

class ASTERUI_EXPORT AsterContent : public QWidget {
    Q_OBJECT
public:
    explicit AsterContent(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

class ASTERUI_EXPORT AsterSider : public QWidget {
    Q_OBJECT
public:
    explicit AsterSider(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

class ASTERUI_EXPORT AsterLayout : public QWidget {
    Q_OBJECT
public:
    enum class Mode {
        HeaderFirst, // Header spans top (Sider is below Header)
        SiderFirst   // Sider spans left (Header is to the right of Sider)
    };
    Q_ENUM(Mode)

    explicit AsterLayout(QWidget *parent = nullptr);
    ~AsterLayout() override;

    // Slot setters
    void setHeader(QWidget *widget, int height = -1);
    void setSider(QWidget *widget, int width = -1);
    void setContent(QWidget *widget);
    void setFooter(QWidget *widget, int height = -1);

    // Dimension setters (applied to current widgets)
    void setHeaderHeight(int height); // height <= 0 means auto/expand
    void setSiderWidth(int width);    // width <= 0 means auto/expand
    void setFooterHeight(int height); // height <= 0 means auto/expand

    // Layout configuration
    void setMode(Mode mode);
    Mode mode() const;

    // Optional: Global spacing for the layout
    void setSpacing(int spacing);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void init();
    void updateLayout();

    QGridLayout *m_layout = nullptr;
    Mode m_mode = Mode::HeaderFirst;

    QWidget *m_header = nullptr;
    QWidget *m_sider = nullptr;
    QWidget *m_content = nullptr;
    QWidget *m_footer = nullptr;

    int m_headerHeight = -1;
    int m_siderWidth = -1;
    int m_footerHeight = -1;
};

}
