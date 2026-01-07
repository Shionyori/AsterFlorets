#include <QApplication>
#include <QMainWindow>
#include <QScrollArea>
#include <QStyle>
#include <QDebug>
#include "AsterUI/AsterUI.h"

using namespace AsterUI;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto theme = AsterUI::AsterTheme::instance();

    QMainWindow window;
    window.setWindowTitle("AsterUI Demo - Component Gallery");
    window.resize(1100, 900);

    QPalette pal = window.palette();
    pal.setColor(QPalette::Window, theme->color(AsterUI::AsterTheme::ColorRole::Background));
    window.setPalette(pal);

    AsterUI::AsterScrollArea *scrollArea = new AsterUI::AsterScrollArea();

    // Root Container
    AsterSpace *rootSpace = new AsterSpace(Qt::Vertical);
    rootSpace->setPalette(pal);
    rootSpace->setAutoFillBackground(true);
    rootSpace->setContentsMargins(40, 40, 40, 40);
    rootSpace->setSize(30);

    // Header Container
    {
        auto *headerSpace = new AsterSpace(Qt::Vertical);
        headerSpace->setSize(10);
        headerSpace->addWidget(new AsterTitle("AsterUI Gallery", 1));
        headerSpace->addWidget(new AsterText("A modern C++ Qt component library inspired by Ant Design."));
        rootSpace->addWidget(headerSpace);
        rootSpace->addWidget(new AsterDivider);
    }

    // Row Container : Card 1 & Card 2
    {
        auto *row = new AsterRow();
        row->setGutter(24); // 设置行内间距

        // [Card 1] Buttons
        {
            auto *col = new AsterColumn(12);
            auto *card = new AsterCard();
            card->setTitle("Buttons");

            auto *cardSpace = new AsterSpace(Qt::Vertical);
            cardSpace->setSize(16);

            // Row of primary/default/dashed buttons
            {
                auto *r1 = new AsterSpace(Qt::Horizontal);
                r1->addWidget(new AsterButton("Default"));

                auto *btnPrimary = new AsterButton("Primary");
                btnPrimary->setType(AsterButton::Type::Primary);
                r1->addWidget(btnPrimary);

                auto *btnDash = new AsterButton("Dashed");
                btnDash->setType(AsterButton::Type::Dashed);
                r1->addWidget(btnDash);
                cardSpace->addWidget(r1);
            }

            // Row of text/link buttons
            {
                auto *r2 = new AsterSpace(Qt::Horizontal);
                auto *btnText = new AsterButton("Text Button");
                btnText->setType(AsterButton::Type::Text);
                r2->addWidget(btnText);

                auto *btnLink = new AsterButton("Link Button");
                btnLink->setType(AsterButton::Type::Link);
                r2->addWidget(btnLink);
                cardSpace->addWidget(r2);
            }

            // Select
            auto *select = new AsterSelect();
            select->setPlaceholderText("Select an option...");
            select->addItems({"Ant Design", "Material UI", "Fluent Design"});
            cardSpace->addWidget(select);

            // Checkbox and Switch Space
            auto *ToggleSpace = new AsterSpace(Qt::Horizontal);
            ToggleSpace->addWidget(new AsterCheckBox("Remember me"));

            auto *sw = new AsterSwitch();
            sw->setFixedSize(44, 22);
            ToggleSpace->addWidget(sw);

            cardSpace->addWidget(ToggleSpace);

            // Radio Group Row 1
            {
                auto *radioSpace = new AsterSpace(Qt::Horizontal);
                auto *r1 = new AsterRadio("Option A");
                radioSpace->addWidget(r1);
                auto *r2 = new AsterRadio("Option B");
                radioSpace->addWidget(r2);
                r1->setChecked(true);
                cardSpace->addWidget(radioSpace);
            }

            auto *divider = new AsterDivider;
            cardSpace->addWidget(divider);

            // Radio Group Row 2
            {
                auto *radioSpace = new AsterSpace(Qt::Horizontal);
                auto *r1 = new AsterRadio("Option 1");
                radioSpace->addWidget(r1);
                auto *r2 = new AsterRadio("Option 2");
                radioSpace->addWidget(r2);
                auto *r3 = new AsterRadio("Option 3");
                radioSpace->addWidget(r3);
                auto *r4 = new AsterRadio("Option 4");
                radioSpace->addWidget(r4);
                r2->setChecked(true);
                cardSpace->addWidget(radioSpace);
            }

            // Slider
            auto *slider = new AsterSlider(Qt::Horizontal);
            slider->setValue(45);
            cardSpace->addWidget(slider);

            auto *l = new QVBoxLayout(card);
            l->setContentsMargins(0, 0, 0, 0);
            l->addWidget(cardSpace);

            col->addWidget(card);
            row->addColumn(col);
        }

        // [Card 2] Data Entry
        {
            auto *col = new AsterColumn(12);
            auto *card = new AsterCard();
            card->setTitle("Data Entry");

            auto *cardSpace = new AsterSpace(Qt::Vertical);
            cardSpace->setSize(16);

            // Inputs
            auto *inputSpace = new AsterSpace(Qt::Vertical);
            inputSpace->setSize(8);
            inputSpace->addWidget(new AsterTextInput("Basic Input"));

            auto *inputClr = new AsterTextInput("Clearable Input");
            inputClr->setClearable(true);
            inputSpace->addWidget(inputClr);
            cardSpace->addWidget(inputSpace);

            // Text Area
            auto *textArea = new AsterTextArea("This is a multi-line text area.\nYou can enter more detailed information here.");
            cardSpace->addWidget(textArea);

            auto *l = new QVBoxLayout(card);
            l->setContentsMargins(0, 0, 0, 0);
            l->addWidget(cardSpace);

            col->addWidget(card);
            row->addColumn(col);
        }

        rootSpace->addWidget(row);
    }

    // --- Section 2: Data Display ---
    {
        auto *row = new AsterRow();
        row->setGutter(24);

        // [Card 3] Avatars & Tags (Flow Layout Demo)
        {
            auto *col = new AsterColumn(12);
            auto *card = new AsterCard();
            card->setTitle("Avatars & Tags");

            auto *cardSpace = new AsterSpace(Qt::Vertical);
            cardSpace->setSize(20);

            // Avatar Group
            auto *avSpace = new AsterSpace(Qt::Horizontal);
            avSpace->setSize(12);
            avSpace->addWidget(new AsterAvatar("U"));

            auto *avSq = new AsterAvatar("Admin");
            avSq->setShape(AsterAvatar::Shape::Square);
            avSq->setBackgroundColor(theme->color(AsterTheme::ColorRole::Primary));
            avSpace->addWidget(avSq);

            cardSpace->addWidget(avSpace);
            cardSpace->addWidget(new AsterDivider);

            // Tags in Flow Container
            auto *lblFlow = new AsterText("AsterFlow Layout:");
            lblFlow->setType(AsterText::Type::Secondary);
            cardSpace->addWidget(lblFlow);

            auto *flow = new AsterFlow();
            flow->setHSpacing(2);
            flow->setVSpacing(8);

            flow->addWidget(new AsterTag("Design"));

            auto *t1 = new AsterTag("Component");
            t1->setType(AsterTag::Type::Primary);
            flow->addWidget(t1);

            auto *t2 = new AsterTag("Checked");
            t2->setType(AsterTag::Type::Success);
            flow->addWidget(t2);

            auto *t3 = new AsterTag("Deprecated");
            t3->setType(AsterTag::Type::Error);
            flow->addWidget(t3);

            auto *t4 = new AsterTag("Closeable Tag");
            t4->setClosable(true);
            flow->addWidget(t4);

            cardSpace->addWidget(flow);

            auto *l = new QVBoxLayout(card);
            l->setContentsMargins(0, 0, 0, 0);
            l->addWidget(cardSpace);

            col->addWidget(card);
            row->addColumn(col);
        }

        // [Card 4] Feedback & Typography
        {
            auto *col = new AsterColumn(12);
            auto *card = new AsterCard();
            card->setTitle("Feedback & Typography");

            auto *cardSpace = new AsterSpace(Qt::Vertical);
            cardSpace->setSize(16);

            // Progress
            cardSpace->addWidget(new AsterText("Progress Status:"));
            auto *prog = new AsterProgress();
            prog->setValue(60);
            cardSpace->addWidget(prog);

            cardSpace->addWidget(new AsterDivider);

            // Icon & Text
            auto *iconSpace = new AsterSpace(Qt::Horizontal);

            auto *i1 = new AsterIcon(window.style()->standardIcon(QStyle::SP_MessageBoxInformation));
            i1->setColor(theme->color(AsterTheme::ColorRole::Primary));
            iconSpace->addWidget(i1);

            auto *i2 = new AsterIcon(window.style()->standardIcon(QStyle::SP_MessageBoxCritical));
            i2->setColor(theme->color(AsterTheme::ColorRole::Error));
            iconSpace->addWidget(i2);

            cardSpace->addWidget(iconSpace);

            cardSpace->addWidget(new AsterText("This is a standard text component used for body content in the UI."));
            auto *caption = new AsterText("Secondary text for captions or helper text.");
            caption->setType(AsterText::Type::Secondary);
            cardSpace->addWidget(caption);

            auto *l = new QVBoxLayout(card);
            l->setContentsMargins(0, 0, 0, 0);
            l->addWidget(cardSpace);

            col->addWidget(card);
            row->addColumn(col);
        }

        rootSpace->addWidget(row);
    }

    // --- Section 2.5: Interaction & Feedback ---
    {
        auto *row = new AsterRow();
        row->setGutter(24);

        // [Card 5] Global Message
        {
            auto *col = new AsterColumn(24);
            auto *card = new AsterCard();
            card->setTitle("Global Message (Toast)");

            auto *cardSpace = new AsterSpace(Qt::Vertical);
            cardSpace->setSize(16);

            cardSpace->addWidget(new AsterText("Click buttons to trigger global messages."));

            auto *btnSpace = new AsterSpace(Qt::Horizontal);

            auto *btnInfo = new AsterButton("Info Message");
            QObject::connect(btnInfo, &AsterButton::clicked, [=](){
                AsterMessage::info("This is an info message.");
            });
            btnSpace->addWidget(btnInfo);

            auto *btnSuccess = new AsterButton("Success Message");
            btnSuccess->setType(AsterButton::Type::Primary);
            QObject::connect(btnSuccess, &AsterButton::clicked, [=](){
                AsterMessage::success("Operation successful!");
            });
            btnSpace->addWidget(btnSuccess);

            auto *btnWarning = new AsterButton("Warning Message");
            QObject::connect(btnWarning, &AsterButton::clicked, [=](){
                AsterMessage::warning("This is a warning message.", 4000);
            });
            btnSpace->addWidget(btnWarning);

            auto *btnError = new AsterButton("Error Message");
            QObject::connect(btnError, &AsterButton::clicked, [=](){
                AsterMessage::error("Something went wrong!", 5000);
            });
            btnSpace->addWidget(btnError);

            cardSpace->addWidget(btnSpace);

            auto *l = new QVBoxLayout(card);
            l->setContentsMargins(0, 0, 0, 0);
            l->addWidget(cardSpace);

            col->addWidget(card);
            row->addColumn(col);
        }
        rootSpace->addWidget(row);
    }

    // --- Section 3: Advanced Layouts ---
    rootSpace->addWidget(new AsterDivider("Layout System Demo"));

    // 1. Asymmetric Layout (Ant Design Row/Col Style)
    // Using the 24-grid system for non-equal divisions
    {
        auto *sectionTitle = new AsterText("1. Asymmetric Layout (AsterRow & AsterColumn - 24 Grid System)");
        sectionTitle->setAlignment(Qt::AlignCenter);
        rootSpace->addWidget(sectionTitle);

        auto *layoutSpace = new AsterSpace(Qt::Vertical);
        layoutSpace->setSize(16);

        // Row A: 8 (33%) + 16 (66%)
        {
            auto *row = new AsterRow();
            row->setGutter(16);

            auto *c1 = new AsterColumn(8);
            auto *b1 = new AsterButton("Span 8 (33%)");
            c1->addWidget(b1);
            row->addColumn(c1);

            auto *c2 = new AsterColumn(16);
            auto *b2 = new AsterButton("Span 16 (66%)");
            c2->addWidget(b2);
            row->addColumn(c2);

            layoutSpace->addWidget(row);
        }

        // Row B: 6 (25%) + 6 (25%) + 12 (50%)
        {
            auto *row = new AsterRow();
            row->setGutter(16);

            auto *c1 = new AsterColumn(6);
            c1->addWidget(new AsterButton("Span 6"));
            row->addColumn(c1);

            auto *c2 = new AsterColumn(6);
            c2->addWidget(new AsterButton("Span 6"));
            row->addColumn(c2);

            auto *c3 = new AsterColumn(12);
            auto *b3 = new AsterButton("Span 12 (50%)");
            c3->addWidget(b3);
            row->addColumn(c3);

            layoutSpace->addWidget(row);
        }

        // Row C: 4 + 16 + 4 (Sidebar - Content - Sidebar)
        {
            auto *row = new AsterRow();
            row->setGutter(16);

            auto *c1 = new AsterColumn(4);
            c1->addWidget(new AsterButton("Left 4"));
            row->addColumn(c1);

            auto *c2 = new AsterColumn(16);
            c2->addWidget(new AsterButton("Main Content (Span 16)"));
            row->addColumn(c2);

            auto *c3 = new AsterColumn(4);
            c3->addWidget(new AsterButton("Right 4"));
            row->addColumn(c3);

            layoutSpace->addWidget(row);
        }

        rootSpace->addWidget(layoutSpace);
        rootSpace->addWidget(new AsterDivider);
    }

    // 2. Uniform Dashboard Grid
    auto *msg = new AsterText("2. Uniform Dashboard Grid (AsterGrid)");
    msg->setAlignment(Qt::AlignCenter);
    rootSpace->addWidget(msg);

    // Grid Container
    auto *gridCard = new AsterCard();
    gridCard->setTitle("AsterGrid (Dashboard Layout)");

    // The AsterGrid Component
    auto *myGrid = new AsterGrid(3); // 3 Columns for Dashboard
    myGrid->setHorizontalSpacing(24);
    myGrid->setVerticalSpacing(24);

    // Helper lambda to create a statistic card
    auto createStatCard = [&](const QString &title, const QString &value, const QColor &color, QStyle::StandardPixmap icon) -> QWidget *
    {
        AsterCard *card = new AsterCard();
        // Remove border for inner cards to make them look like panels, or keep them.
        // Let's keep them default.

        auto *vLayout = new AsterSpace(Qt::Vertical);
        vLayout->setSize(12);

        // Header: Icon + Title
        auto *hHeader = new AsterSpace(Qt::Horizontal);
        hHeader->setSize(10);

        auto *iconWidget = new AsterAvatar(window.style()->standardIcon(icon).pixmap(24, 24));
        iconWidget->setSize(32);
        iconWidget->setShape(AsterAvatar::Shape::Square);
        iconWidget->setBackgroundColor(color);
        hHeader->addWidget(iconWidget);

        auto *titleTxt = new AsterText(title);
        titleTxt->setType(AsterText::Type::Secondary);
        hHeader->addWidget(titleTxt);
        hHeader->addStretch();

        vLayout->addWidget(hHeader);

        // Value
        AsterTitle *valTxt = new AsterTitle(value, 2); // H2
        // valTxt->setAlignment(Qt::AlignLeft);
        vLayout->addWidget(valTxt);

        // Footer: Progress
        auto *prog = new AsterProgress();
        prog->setValue(rand() % 40 + 60); // Random 60-100
        prog->setFixedHeight(4);          // Thin progress
        vLayout->addWidget(prog);

        // Wrap in layout
        auto *cardL = new QVBoxLayout(card);
        cardL->setContentsMargins(16, 16, 16, 16); // Padding inside the stat card
        cardL->addWidget(vLayout);

        return card;
    };

    // Add Dashboard items
    myGrid->addWidget(createStatCard("Total Sales", "$126,560", QColor("#1890ff"), QStyle::SP_ComputerIcon));
    myGrid->addWidget(createStatCard("Active Users", "8,846", QColor("#52c41a"), QStyle::SP_DialogYesButton));
    myGrid->addWidget(createStatCard("New Orders", "1,234", QColor("#722ed1"), QStyle::SP_FileIcon));
    myGrid->addWidget(createStatCard("Pending Issues", "56", QColor("#fa8c16"), QStyle::SP_MessageBoxWarning));
    myGrid->addWidget(createStatCard("System Load", "78%", QColor("#f5222d"), QStyle::SP_DriveNetIcon));
    myGrid->addWidget(createStatCard("Avg. Response", "120ms", QColor("#13c2c2"), QStyle::SP_ArrowUp));

    auto *gLayout = new QVBoxLayout(gridCard);
    gLayout->setContentsMargins(0, 0, 0, 0);
    gLayout->addWidget(myGrid);

    rootSpace->addWidget(gridCard);

    // Push everything to top
    rootSpace->addStretch();

    // Footer
    auto *footerSpace = new AsterSpace(Qt::Vertical);
    footerSpace->setSize(5);

    auto *ft = new AsterText("Designed by AsterUI Team © 2026");
    ft->setType(AsterText::Type::Secondary);
    ft->setAlignment(Qt::AlignCenter); // QLabel has SetAlignment
    footerSpace->addWidget(ft);
    rootSpace->addWidget(footerSpace);

    // Finalize
    scrollArea->setWidget(rootSpace);
    window.setCentralWidget(scrollArea);
    window.show();

    return app.exec();
}
