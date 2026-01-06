#pragma once

#include "Global.h"
#include <QWidget>
#include <QHBoxLayout>

namespace AsterUI {

class ASTERUI_EXPORT AsterCol : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int span READ span WRITE setSpan)

public:
    explicit AsterCol(QWidget *parent = nullptr);
    explicit AsterCol(int span, QWidget *parent = nullptr);
    ~AsterCol() override;

    int span() const;
    void setSpan(int span);

    // Helper to set a single widget content
    void setWidget(QWidget *widget);

    // If using as a generic container, you can access internal layout
    // but usually we just set one widget or add simplified content.
    void addWidget(QWidget *widget);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_span = 24; // Default to full width (24 grid system)
    QVBoxLayout *m_layout = nullptr;
};

class ASTERUI_EXPORT AsterRow : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int gutter READ gutter WRITE setGutter)

public:
    explicit AsterRow(QWidget *parent = nullptr);
    ~AsterRow() override;

    int gutter() const;
    void setGutter(int gutter);

    void addCol(AsterCol *col);
    // Helper to add a plain widget wrapped in a default Col (span=1?? or auto?)
    // Better to force using AsterCol for clarity in Grid system
    
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QHBoxLayout *m_layout = nullptr;
    int m_gutter = 0;
};

}
