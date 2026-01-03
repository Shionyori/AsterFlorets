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

    // Section: Tags
    QLabel *tagLabel = new QLabel("Tags", centralWidget);
    tagLabel->setFont(theme->font(AsterUI::AsterTheme::Size::Large));
    mainLayout->addWidget(tagLabel);

    QHBoxLayout *tagLayout = new QHBoxLayout();
    tagLayout->setAlignment(Qt::AlignLeft);

    // Default Tag
    tagLayout->addWidget(new AsterUI::AsterTag("Default"));

    // Primary Tag
    auto *tagPrimary = new AsterUI::AsterTag("Primary");
    tagPrimary->setType(AsterUI::AsterTag::Type::Primary);
    tagLayout->addWidget(tagPrimary);

    // Success Tag
    auto *tagSuccess = new AsterUI::AsterTag("Success");
    tagSuccess->setType(AsterUI::AsterTag::Type::Success);
    tagLayout->addWidget(tagSuccess);

    // Warning Tag
    auto *tagWarning = new AsterUI::AsterTag("Warning");
    tagWarning->setType(AsterUI::AsterTag::Type::Warning);
    tagLayout->addWidget(tagWarning);

    // Error Tag
    auto *tagError = new AsterUI::AsterTag("Error");
    tagError->setType(AsterUI::AsterTag::Type::Error);
    tagLayout->addWidget(tagError);

    // Closable Tag
    auto *tagClosable = new AsterUI::AsterTag("Closable");
    tagClosable->setClosable(true);
    tagLayout->addWidget(tagClosable);

    mainLayout->addLayout(tagLayout);

    // Section: Sliders
    QLabel *sliderLabel = new QLabel("Sliders", centralWidget);
    sliderLabel->setFont(theme->font(AsterUI::AsterTheme::Size::Large));
    mainLayout->addWidget(sliderLabel);

    QHBoxLayout *sliderLayout = new QHBoxLayout();
    sliderLayout->setAlignment(Qt::AlignLeft);

    // Horizontal Slider
    auto *sliderH = new AsterUI::AsterSlider(Qt::Horizontal);
    sliderH->setValue(30);
    sliderH->setFixedWidth(200);
    sliderLayout->addWidget(sliderH);

    // Vertical Slider
    auto *sliderV = new AsterUI::AsterSlider(Qt::Vertical);
    sliderV->setValue(70);
    sliderV->setFixedHeight(100);
    sliderLayout->addWidget(sliderV);

    mainLayout->addLayout(sliderLayout);

    // Section: Selects
    QLabel *selectLabel = new QLabel("Selects", centralWidget);
    selectLabel->setFont(theme->font(AsterUI::AsterTheme::Size::Large));
    mainLayout->addWidget(selectLabel);

    QHBoxLayout *selectLayout = new QHBoxLayout();
    selectLayout->setAlignment(Qt::AlignLeft);

    // Basic Select
    auto *select1 = new AsterUI::AsterSelect();
    select1->setPlaceholderText("Choose an option");
    select1->addItems({"Option 1", "Option 2", "Option 3", "Long Option Text Here"});
    select1->setCurrentIndex(-1);
    select1->setFixedWidth(200);
    selectLayout->addWidget(select1);

    // Select with many items
    auto *select2 = new AsterUI::AsterSelect();
    select2->setPlaceholderText("Select Country");
    QStringList countries;
    for(int i=1; i<=20; ++i) countries << QString("Country %1").arg(i);
    select2->addItems(countries);
    select2->setFixedWidth(200);
    selectLayout->addWidget(select2);

    mainLayout->addLayout(selectLayout);

    mainLayout->addStretch();

    window.setCentralWidget(centralWidget);
    window.show();

    return app.exec();
}
