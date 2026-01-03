#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include "AsterUI/AsterUI.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    auto theme = AsterUI::AsterTheme::instance();

    QMainWindow window;
    window.setWindowTitle("AsterUI Demo - Components");
    window.resize(800, 600);
    
    QPalette pal = window.palette();
    pal.setColor(QPalette::Window, theme->color(AsterUI::AsterTheme::ColorRole::Background));
    window.setPalette(pal);

    QWidget *centralWidget = new QWidget(&window);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Section: Buttons
    QLabel *btnLabel = new QLabel("Buttons", centralWidget);
    btnLabel->setFont(theme->font(AsterUI::AsterTheme::Size::Large));
    mainLayout->addWidget(btnLabel);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignLeft);
    
    // Primary Button
    auto *btnPrimary = new AsterUI::AsterButton("Primary Button");
    btnPrimary->setType(AsterUI::AsterButton::Type::Primary);
    btnLayout->addWidget(btnPrimary);

    // Default Button
    auto *btnDefault = new AsterUI::AsterButton("Default Button");
    btnDefault->setType(AsterUI::AsterButton::Type::Default);
    btnLayout->addWidget(btnDefault);

    // Dashed Button
    auto *btnDashed = new AsterUI::AsterButton("Dashed Button");
    btnDashed->setType(AsterUI::AsterButton::Type::Dashed);
    btnLayout->addWidget(btnDashed);

    // Text Button
    auto *btnText = new AsterUI::AsterButton("Text Button");
    btnText->setType(AsterUI::AsterButton::Type::Text);
    btnLayout->addWidget(btnText);

    // Link Button
    auto *btnLink = new AsterUI::AsterButton("Link Button");
    btnLink->setType(AsterUI::AsterButton::Type::Link);
    btnLayout->addWidget(btnLink);

    mainLayout->addLayout(btnLayout);

    // Section: Inputs
    QLabel *inputLabel = new QLabel("Inputs", centralWidget);
    inputLabel->setFont(theme->font(AsterUI::AsterTheme::Size::Large));
    mainLayout->addWidget(inputLabel);

    QVBoxLayout *inputLayout = new QVBoxLayout();
    
    // Basic Input
    auto *inputBasic = new AsterUI::AsterInput("Basic Input");
    inputLayout->addWidget(inputBasic);

    // Clearable Input
    auto *inputClearable = new AsterUI::AsterInput("Clearable Input");
    inputClearable->setClearable(true);
    inputLayout->addWidget(inputClearable);

    mainLayout->addLayout(inputLayout);

    // Section: Toggles
    QLabel *toggleLabel = new QLabel("Toggles", centralWidget);
    toggleLabel->setFont(theme->font(AsterUI::AsterTheme::Size::Large));
    mainLayout->addWidget(toggleLabel);

    QHBoxLayout *toggleLayout = new QHBoxLayout();
    toggleLayout->setAlignment(Qt::AlignLeft);

    // CheckBox
    auto *checkBox = new AsterUI::AsterCheckBox("Remember me");
    toggleLayout->addWidget(checkBox);

    // Switch
    auto *switchBtn = new AsterUI::AsterSwitch();
    // 强制设置一个固定大小，排除布局问题
    switchBtn->setFixedSize(44, 22); 
    toggleLayout->addWidget(switchBtn);

    mainLayout->addLayout(toggleLayout);

    // Section: Cards
    QLabel *cardLabel = new QLabel("Cards", centralWidget);
    cardLabel->setFont(theme->font(AsterUI::AsterTheme::Size::Large));
    mainLayout->addWidget(cardLabel);

    QHBoxLayout *cardLayout = new QHBoxLayout();
    cardLayout->setAlignment(Qt::AlignLeft);

    // Basic Card
    auto *card1 = new AsterUI::AsterCard();
    card1->setTitle("Basic Card");
    card1->setFixedSize(200, 120);
    cardLayout->addWidget(card1);

    // Hoverable Card
    auto *card2 = new AsterUI::AsterCard();
    card2->setTitle("Hoverable Card");
    card2->setHoverable(true);
    card2->setFixedSize(200, 120);
    cardLayout->addWidget(card2);

    mainLayout->addLayout(cardLayout);

    mainLayout->addStretch();

    window.setCentralWidget(centralWidget);
    window.show();

    return app.exec();
}
