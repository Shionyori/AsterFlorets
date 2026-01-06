#include <QApplication>
#include <QMainWindow>
#include <QScrollArea>
#include <QStyle>
#include <QDebug>
#include "AsterUI/AsterUI.h"

using namespace AsterUI;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    auto theme = AsterUI::AsterTheme::instance();
    
    // Debug Theme Colors
    qDebug() << "Theme Background Color:" << theme->color(AsterUI::AsterTheme::ColorRole::Background).name();
    qDebug() << "Theme Surface Color:" << theme->color(AsterUI::AsterTheme::ColorRole::Surface).name();

    QMainWindow window;
    window.setWindowTitle("AsterUI Demo - Components");
    window.resize(1024, 800);
    
    QPalette pal = window.palette();
    pal.setColor(QPalette::Window, theme->color(AsterUI::AsterTheme::ColorRole::Background));
    window.setPalette(pal);

    AsterUI::AsterScrollArea *scrollArea = new AsterUI::AsterScrollArea();
    
    QWidget *contentWidget = new QWidget();
    contentWidget->setPalette(pal);
    contentWidget->setAutoFillBackground(true);

    // Root Layout using AsterRow/Col for high level structure
    QVBoxLayout *mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(30);
    mainLayout->setAlignment(Qt::AlignTop);

    // Header Area
    auto *headerLayout = new QVBoxLayout();
    headerLayout->setSpacing(10);
    headerLayout->addWidget(new AsterTitle("AsterUI Components", 1));
    headerLayout->addWidget(new AsterText("A modern, Qt-based UI component library inspired by Ant Design."));
    mainLayout->addLayout(headerLayout);

    mainLayout->addWidget(new AsterDivider);

    // Grid System for Layout
    auto *rootGrid = new QVBoxLayout();
    rootGrid->setSpacing(30);

    // --- Row 1: General & Layout ---
    {
        auto *row = new AsterRow();
        row->setGutter(30);

        // Col 1: Buttons
        auto *col1 = new AsterCol(12);
        auto *card = new AsterCard();
        card->setTitle("Buttons");
        
        auto *vbox = new QVBoxLayout(card);
        vbox->setSpacing(15);
        
        auto *space1 = new AsterSpace(Qt::Horizontal);
        space1->addWidget(new AsterButton("Default"));
        auto *btnPri = new AsterButton("Primary");
        btnPri->setType(AsterButton::Type::Primary);
        space1->addWidget(btnPri);
        auto *btnDash = new AsterButton("Dashed");
        btnDash->setType(AsterButton::Type::Dashed);
        space1->addWidget(btnDash);
        vbox->addWidget(space1);

        auto *space2 = new AsterSpace(Qt::Horizontal);
        auto *btnText = new AsterButton("Text Button");
        btnText->setType(AsterButton::Type::Text);
        space2->addWidget(btnText);
        auto *btnLink = new AsterButton("Link Button");
        btnLink->setType(AsterButton::Type::Link);
        space2->addWidget(btnLink);
        vbox->addWidget(space2);

        col1->addWidget(card);
        row->addCol(col1);

        // Col 2: Icons & Typography
        auto *col2 = new AsterCol(12);
        auto *card2 = new AsterCard();
        card2->setTitle("Icons & Typography");

        auto *vbox2 = new QVBoxLayout(card2);
        vbox2->setSpacing(15);

        auto *spaceIcons = new AsterSpace(Qt::Horizontal);
        spaceIcons->setSize(20);
        QStyle *style = window.style();
        auto *icon1 = new AsterIcon(style->standardIcon(QStyle::SP_ComputerIcon));
        icon1->setIconSize(QSize(24, 24));
        spaceIcons->addWidget(icon1);
        auto *icon2 = new AsterIcon(style->standardIcon(QStyle::SP_MessageBoxWarning));
        icon2->setIconSize(QSize(24, 24));
        icon2->setColor(theme->color(AsterTheme::ColorRole::Warning));
        spaceIcons->addWidget(icon2);
        vbox2->addWidget(spaceIcons);

        vbox2->addWidget(new AsterDivider);

        vbox2->addWidget(new AsterText("Default Text"));
        auto *txtSec = new AsterText("Secondary Text");
        txtSec->setType(AsterText::Type::Secondary);
        vbox2->addWidget(txtSec);
        
        col2->addWidget(card2);
        row->addCol(col2);

        rootGrid->addWidget(row);
    }

    // --- Row 2: Data Entry ---
    {
        auto *row = new AsterRow();
        row->setGutter(30);

        // Col 1: Inputs
        auto *col1 = new AsterCol(12);
        auto *card = new AsterCard();
        card->setTitle("Data Entry");
        auto *vbox = new QVBoxLayout(card);
        vbox->setSpacing(15);

        vbox->addWidget(new AsterText("Inputs", nullptr));
        auto *inputBasic = new AsterTextInput("Basic Input");
        vbox->addWidget(inputBasic);
        
        auto *inputClear = new AsterTextInput("Clearable Input");
        inputClear->setClearable(true);
        vbox->addWidget(inputClear);

        vbox->addWidget(new AsterText("Select", nullptr));
        auto *select = new AsterSelect();
        select->setPlaceholderText("Choose option...");
        select->addItems({"Option 1", "Option 2", "Option 3"});
        vbox->addWidget(select);

        col1->addWidget(card);
        row->addCol(col1);

        // Col 2: Toggles & Sliders
        auto *col2 = new AsterCol(12);
        auto *card2 = new AsterCard();
        card2->setTitle("Toggles & Controls");
        auto *vbox2 = new QVBoxLayout(card2);
        vbox2->setSpacing(15);

        auto *spaceH = new AsterSpace(Qt::Horizontal);
        spaceH->addWidget(new AsterCheckBox("CheckBox"));
        auto *switchBtn = new AsterSwitch();
        switchBtn->setFixedSize(44, 22);
        spaceH->addWidget(switchBtn);
        vbox2->addWidget(spaceH);

        auto *spaceRadio = new AsterSpace(Qt::Horizontal);
        auto *r1 = new AsterRadio("Radio A");
        r1->setChecked(true);
        spaceRadio->addWidget(r1);
        spaceRadio->addWidget(new AsterRadio("Radio B"));
        vbox2->addWidget(spaceRadio);

        vbox2->addWidget(new AsterDivider);
        
        auto *sliderH = new AsterSlider(Qt::Horizontal);
        sliderH->setValue(30);
        vbox2->addWidget(sliderH);

        col2->addWidget(card2);
        row->addCol(col2);

        rootGrid->addWidget(row);
    }

    // --- Row 3: Data Display ---
    {
        auto *row = new AsterRow();
        row->setGutter(30);

        // Col 1: Avatars & Tags
        auto *col1 = new AsterCol(12);
        auto *card = new AsterCard();
        card->setTitle("Data Display");
        auto *vbox = new QVBoxLayout(card);
        vbox->setSpacing(20);

        auto *spaceAvatar = new AsterSpace(Qt::Horizontal);
        spaceAvatar->addWidget(new AsterAvatar("U"));
        auto *av2 = new AsterAvatar("Adm");
        av2->setShape(AsterAvatar::Shape::Square);
        av2->setBackgroundColor(theme->color(AsterTheme::ColorRole::Success));
        spaceAvatar->addWidget(av2);
        vbox->addWidget(spaceAvatar);

        auto *flowTags = new AsterFlowLayout(0, 10, 10);
        flowTags->addItem(new QWidgetItem(new AsterTag("Classic")));
        
        auto *tagPri = new AsterTag("Modern UI");
        tagPri->setType(AsterTag::Type::Primary);
        flowTags->addItem(new QWidgetItem(tagPri));

        auto *tagSucc = new AsterTag("Stable");
        tagSucc->setType(AsterTag::Type::Success);
        flowTags->addItem(new QWidgetItem(tagSucc));

        auto *tagErr = new AsterTag("Beta");
        tagErr->setType(AsterTag::Type::Error);
        flowTags->addItem(new QWidgetItem(tagErr));

        QWidget *tagsWidget = new QWidget();
        tagsWidget->setLayout(flowTags);
        vbox->addWidget(tagsWidget);

        col1->addWidget(card);
        row->addCol(col1);

        // Col 2: Feedback & Layouts
        auto *col2 = new AsterCol(12);
        auto *card2 = new AsterCard();
        card2->setTitle("Feedback & Grid System");
        auto *vbox2 = new QVBoxLayout(card2);
        vbox2->setSpacing(15);

        auto *progress = new AsterProgress();
        progress->setValue(75);
        vbox2->addWidget(progress);

        vbox2->addWidget(new AsterDivider("Grid Demo", Qt::AlignCenter));

        // Mini Grid Demo
        auto *miniGrid = new QVBoxLayout();
        miniGrid->setSpacing(10);
        
        auto *r1 = new AsterRow();
        r1->setGutter(10);
        auto *c1 = new AsterCol(8); c1->addWidget(new AsterButton("8")); r1->addCol(c1);
        auto *c2 = new AsterCol(8); c2->addWidget(new AsterButton("8")); r1->addCol(c2);
        auto *c3 = new AsterCol(8); c3->addWidget(new AsterButton("8")); r1->addCol(c3);
        miniGrid->addWidget(r1);

        auto *r2 = new AsterRow();
        r2->setGutter(10);
        auto *c4 = new AsterCol(6); c4->addWidget(new AsterButton("6")); r2->addCol(c4);
        auto *c5 = new AsterCol(18); 
        auto *btnLong = new AsterButton("18 (Expanding)"); 
        btnLong->setType(AsterButton::Type::Primary); 
        c5->addWidget(btnLong); 
        r2->addCol(c5);
        miniGrid->addWidget(r2);

        vbox2->addLayout(miniGrid);

        col2->addWidget(card2);
        row->addCol(col2);

        rootGrid->addWidget(row);
    }

    mainLayout->addLayout(rootGrid);
    mainLayout->addStretch();

    // Footer
    auto *footer = new AsterText("AsterUI Demo 2026 © Open Source");
    footer->setType(AsterText::Type::Secondary);
    footer->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(footer);

    scrollArea->setWidget(contentWidget);
    window.setCentralWidget(scrollArea);
    window.show();

    return app.exec();
}
