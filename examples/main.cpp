#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QPushButton>
#include "AsterUI/AsterUI.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qDebug() << "AsterUI Version:" << AsterUI::version();

    // 获取主题实例
    auto theme = AsterUI::AsterTheme::instance();

    QMainWindow window;
    window.setWindowTitle("AsterUI Demo - Shion Theme");
    window.resize(800, 600);
    
    // 设置背景色
    QPalette pal = window.palette();
    pal.setColor(QPalette::Window, theme->color(AsterUI::AsterTheme::ColorRole::Background));
    window.setPalette(pal);

    QWidget *centralWidget = new QWidget(&window);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setSpacing(theme->spacing(AsterUI::AsterTheme::Size::Large));
    
    // 标题
    QLabel *label = new QLabel("Welcome to AsterUI Demo", centralWidget);
    label->setAlignment(Qt::AlignCenter);
    
    // 使用主题字体和颜色
    QFont titleFont = theme->font(AsterUI::AsterTheme::Size::Large);
    titleFont.setBold(true);
    label->setFont(titleFont);
    
    // 手动设置样式表测试颜色 (后续会封装到控件中)
    QString labelStyle = QString("color: %1;").arg(theme->colorString(AsterUI::AsterTheme::ColorRole::Primary));
    label->setStyleSheet(labelStyle);
    
    layout->addWidget(label);

    // 测试按钮 (原生)
    QPushButton *btn = new QPushButton("Native Button (Test Theme Color)", centralWidget);
    QString btnStyle = QString(
        "QPushButton { "
        "  background-color: %1; "
        "  color: white; "
        "  border-radius: %2px; "
        "  padding: 8px 16px;"
        "}"
        "QPushButton:hover { background-color: %3; }"
    )
    .arg(theme->colorString(AsterUI::AsterTheme::ColorRole::Primary))
    .arg(theme->borderRadius(AsterUI::AsterTheme::Size::Default))
    .arg(theme->colorString(AsterUI::AsterTheme::ColorRole::PrimaryHover));
    
    btn->setStyleSheet(btnStyle);
    layout->addWidget(btn);

    layout->addStretch();

    window.setCentralWidget(centralWidget);
    window.show();

    return app.exec();
}
