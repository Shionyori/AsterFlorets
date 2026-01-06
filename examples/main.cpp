#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QStyle>
#include <QDebug>
#include "AsterUI/AsterUI.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    auto theme = AsterUI::AsterTheme::instance();
    
    // Debug Theme Colors
    qDebug() << "Theme Background Color:" << theme->color(AsterUI::AsterTheme::ColorRole::Background).name();
    qDebug() << "Theme Surface Color:" << theme->color(AsterUI::AsterTheme::ColorRole::Surface).name();

    QMainWindow window;
    window.setWindowTitle("AsterUI Demo - Components");
    window.resize(800, 600);
    
    QPalette pal = window.palette();
    pal.setColor(QPalette::Window, theme->color(AsterUI::AsterTheme::ColorRole::Background));
    window.setPalette(pal);

    AsterUI::AsterScrollArea *scrollArea = new AsterUI::AsterScrollArea();
    
    QWidget *contentWidget = new QWidget();
    // Set palette for content widget to match window background
    contentWidget->setPalette(pal);
    contentWidget->setAutoFillBackground(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setAlignment(Qt::AlignTop); // Ensure content starts at the top

    // Section: Typography
    // Titles
    mainLayout->addWidget(new AsterUI::AsterTitle("H1. Main Title", 1));
    mainLayout->addWidget(new AsterUI::AsterTitle("H2. Sub Title", 2));
    mainLayout->addWidget(new AsterUI::AsterTitle("H3. Section Title", 3));
    mainLayout->addWidget(new AsterUI::AsterTitle("H4. Subset Title", 4));
    mainLayout->addWidget(new AsterUI::AsterTitle("H5. Small Title", 5));

    // Texts
    mainLayout->addWidget(new AsterUI::AsterDivider(Qt::Horizontal));
    
    auto *textDefault = new AsterUI::AsterText("Default Text: AsterUI is a modern UI library.");
    mainLayout->addWidget(textDefault);

    auto *textSecondary = new AsterUI::AsterText("Secondary Text: Provides additional information.");
    textSecondary->setType(AsterUI::AsterText::Type::Secondary);
    mainLayout->addWidget(textSecondary);

    auto *textSuccess = new AsterUI::AsterText("Success Text: Operation completed successfully.");
    textSuccess->setType(AsterUI::AsterText::Type::Success);
    mainLayout->addWidget(textSuccess);

    auto *textWarning = new AsterUI::AsterText("Warning Text: Please check your configuration.");
    textWarning->setType(AsterUI::AsterText::Type::Warning);
    mainLayout->addWidget(textWarning);

    auto *textDanger = new AsterUI::AsterText("Danger Text: System error occurred.");
    textDanger->setType(AsterUI::AsterText::Type::Danger);
    mainLayout->addWidget(textDanger);

    mainLayout->addWidget(new AsterUI::AsterDivider(Qt::Horizontal));

    // Section: Avatars
    QLabel *avatarLabel = new QLabel("Avatars", contentWidget);
    avatarLabel->setFont(theme->font(AsterUI::AsterTheme::Size::Large));
    // avatarLabel->setStyleSheet("margin-top: 20px;"); // Removed top margin for the first element
    mainLayout->addWidget(avatarLabel);

    QHBoxLayout *avatarLayout = new QHBoxLayout();
    avatarLayout->setAlignment(Qt::AlignLeft);
    avatarLayout->setSpacing(20);

    // Default Circle Avatar
    auto *avatar1 = new AsterUI::AsterAvatar("U");
    avatarLayout->addWidget(avatar1);

    // Square Avatar
    auto *avatar2 = new AsterUI::AsterAvatar("Admin");
    avatar2->setShape(AsterUI::AsterAvatar::Shape::Square);
    avatar2->setBackgroundColor(theme->color(AsterUI::AsterTheme::ColorRole::Success));
    avatarLayout->addWidget(avatar2);

    // Large Avatar
    auto *avatar3 = new AsterUI::AsterAvatar("L");
    avatar3->setSize(AsterUI::AsterAvatar::Size::Large);
    avatar3->setBackgroundColor(theme->color(AsterUI::AsterTheme::ColorRole::Warning));
    avatarLayout->addWidget(avatar3);

    // Small Avatar
    auto *avatar4 = new AsterUI::AsterAvatar("S");
    avatar4->setSize(AsterUI::AsterAvatar::Size::Small);
    avatar4->setBackgroundColor(theme->color(AsterUI::AsterTheme::ColorRole::Error));
    avatarLayout->addWidget(avatar4);

    mainLayout->addLayout(avatarLayout);

    // Section: Buttons
    QLabel *btnLabel = new QLabel("Buttons", contentWidget);
    btnLabel->setFont(theme->font(AsterUI::AsterTheme::Size::Large));
    btnLabel->setStyleSheet("margin-top: 20px;");
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

    // Section: Icons
    QLabel *iconLabel = new QLabel("Icons", contentWidget);
    iconLabel->setFont(theme->font(AsterUI::AsterTheme::Size::Large));
    iconLabel->setStyleSheet("margin-top: 20px;");
    mainLayout->addWidget(iconLabel);

    QHBoxLayout *iconLayout = new QHBoxLayout();
    iconLayout->setAlignment(Qt::AlignLeft);
    iconLayout->setSpacing(20);

    // Standard Icon (using standard pixmap as placeholder for now since we don't have assets)
    QStyle *style = window.style();
    
    auto *icon1 = new AsterUI::AsterIcon(style->standardIcon(QStyle::SP_ComputerIcon));
    icon1->setIconSize(QSize(24, 24));
    iconLayout->addWidget(icon1);

    auto *icon2 = new AsterUI::AsterIcon(style->standardIcon(QStyle::SP_MessageBoxWarning));
    icon2->setIconSize(QSize(24, 24));
    iconLayout->addWidget(icon2);

    mainLayout->addLayout(iconLayout);

    // Section: LineEdit
    QLabel *inputLabel = new QLabel("Inputs", contentWidget);
    inputLabel->setFont(theme->font(AsterUI::AsterTheme::Size::Large));
    mainLayout->addWidget(inputLabel);

    QVBoxLayout *inputLayout = new QVBoxLayout();
    
    // Basic LineEdit
    auto *inputBasic = new AsterUI::AsterTextInput("Basic Input");
    inputBasic->setFixedHeight(30);
    inputLayout->addWidget(inputBasic);

    // Clearable LineEdit
    auto *inputClearable = new AsterUI::AsterTextInput("Clearable Input");
    inputClearable->setFixedHeight(30);
    inputClearable->setClearable(true);
    inputLayout->addWidget(inputClearable);

    mainLayout->addLayout(inputLayout);

    // Section: TextEdit
    QLabel *textEditLabel = new QLabel("TextEdit", contentWidget);
    textEditLabel->setFont(theme->font(AsterUI::AsterTheme::Size::Large));
    mainLayout->addWidget(textEditLabel);

    auto *textEdit = new AsterUI::AsterTextArea();
    textEdit->setPlaceholderText("Enter multi-line text here...");
    textEdit->setFixedHeight(100);
    mainLayout->addWidget(textEdit);

    // Section: Toggles
    QLabel *toggleLabel = new QLabel("Toggles", contentWidget);
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

    // Radio Buttons
    QHBoxLayout *radioLayout = new QHBoxLayout();
    radioLayout->setAlignment(Qt::AlignLeft);
    
    auto *radio1 = new AsterUI::AsterRadio("Radio 1");
    radio1->setChecked(true);
    radioLayout->addWidget(radio1);
    
    auto *radio2 = new AsterUI::AsterRadio("Radio 2");
    radioLayout->addWidget(radio2);
    
    mainLayout->addLayout(radioLayout);

    // Divider
    mainLayout->addWidget(new AsterUI::AsterDivider(Qt::Horizontal));

    // Section: Cards
    QLabel *cardLabel = new QLabel("Cards", contentWidget);
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
    QLabel *tagLabel = new QLabel("Tags", contentWidget);
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
    QLabel *sliderLabel = new QLabel("Sliders", contentWidget);
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
    QLabel *selectLabel = new QLabel("Selects", contentWidget);
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

    // Section: Feedback
    mainLayout->addWidget(new AsterUI::AsterDivider("Feedback", Qt::AlignLeft));
    
    auto *progress = new AsterUI::AsterProgress();
    progress->setValue(60);
    mainLayout->addWidget(progress);

    mainLayout->addStretch();

    scrollArea->setWidget(contentWidget);
    window.setCentralWidget(scrollArea);
    window.show();

    return app.exec();
}
