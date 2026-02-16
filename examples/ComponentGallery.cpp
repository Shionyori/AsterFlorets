#include <QApplication>
#include <QMainWindow>
#include <QScrollArea>
#include <QStyle>
#include <QDebug>
#include "AsterFlorets.h"

using namespace AsterFlorets;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto theme = AsterFlorets::AsterTheme::instance();

    QMainWindow window;
    window.setWindowTitle("AsterFlorets Demo - Component Gallery");
    window.resize(1100, 900);

    // 设置主调色板
    QPalette pal = window.palette();
    // 设置 QMainWindow 窗口背景色
    pal.setColor(QPalette::Window, theme->color(AsterFlorets::AsterTheme::ColorRole::Background));
    window.setPalette(pal);

    AsterFlorets::AsterScrollArea *scrollArea = new AsterFlorets::AsterScrollArea();

    // Root Container
    AsterSpace *rootSpace = new AsterSpace(Qt::Vertical);
    rootSpace->setPalette(pal); // 继承主窗口的调色板
    rootSpace->setAutoFillBackground(true);
    rootSpace->setContentsMargins(40, 40, 40, 40);
    rootSpace->setSize(30);

    // Header Container
    {
        auto *headerSpace = new AsterSpace(Qt::Vertical);
        headerSpace->setSize(10);
        headerSpace->addWidget(new AsterTitle("AsterFlorets Gallery", 1));
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

            // Badges
            auto *lblBadge = new AsterText("Badges:");
            lblBadge->setType(AsterText::Type::Secondary);
            cardSpace->addWidget(lblBadge);

            auto *badgeSpace = new AsterSpace(Qt::Horizontal);
            badgeSpace->setSize(24);

            auto *b1 = new AsterBadge(new AsterAvatar("User"), nullptr);
            b1->setCount(5);
            badgeSpace->addWidget(b1);

            auto *b2 = new AsterBadge(new AsterAvatar("A"), nullptr);
            b2->setDot(true);
            b2->setColor(theme->color(AsterTheme::ColorRole::Success));
            badgeSpace->addWidget(b2);

            auto *b3 = new AsterBadge(new AsterButton("Inbox"), nullptr);
            b3->setCount(100);
            b3->setMaxCount(99);
            badgeSpace->addWidget(b3);

            cardSpace->addWidget(badgeSpace);
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

    // --- Section 3: Messages
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

            cardSpace->addWidget(new AsterText("Dialogs:"));
            
            auto *dialogSpace = new AsterSpace(Qt::Horizontal);
            
            auto *btnAlert = new AsterButton("Alert Dialog");
            QMainWindow* winPtr = &window;
            QObject::connect(btnAlert, &AsterButton::clicked, [winPtr](){
                 AsterModal::alert(winPtr, "System Alert", "This is a modal alert dialog.\nYou must acknowledge this message.");
            });
            dialogSpace->addWidget(btnAlert);
            
            auto *btnConfirm = new AsterButton("Confirm Dialog");
            QObject::connect(btnConfirm, &AsterButton::clicked, [winPtr](){
                 bool res = AsterModal::confirm(winPtr, "Confirm Action", "Are you sure you want to delete this item?\nThis action cannot be undone.");
                 if (res) AsterMessage::success("Deleted successfully!");
                 else AsterMessage::info("Cancelled.");
            });
            dialogSpace->addWidget(btnConfirm);

            cardSpace->addWidget(dialogSpace);

            cardSpace->addWidget(new AsterText("Notifications:"));
            auto *notifySpace = new AsterSpace(Qt::Horizontal);
            
            auto *btnNotify1 = new AsterButton("Success Notify");
            QObject::connect(btnNotify1, &AsterButton::clicked, [winPtr](){
                AsterNotification::success("Download Complete", "The file 'AsterFlorets_v1.0.zip' has been downloaded successfully.", 4500, winPtr);
            });
            notifySpace->addWidget(btnNotify1);

            auto *btnNotify2 = new AsterButton("Error Notify");
            QObject::connect(btnNotify2, &AsterButton::clicked, [winPtr](){
                // Duration 6000ms
                AsterNotification::error("Connection Failed", "Could not connect to the server. Please check your network settings.", 6000, winPtr); 
            });
            notifySpace->addWidget(btnNotify2);

            auto *btnNotify3 = new AsterButton("Persistent Notify");
            QObject::connect(btnNotify3, &AsterButton::clicked, [winPtr](){
                // Duration 0 = persistent (manual close)
                AsterNotification::info("System Update", "A new update is available. Please save your work.", 0, winPtr);
            });
            notifySpace->addWidget(btnNotify3);
            
            cardSpace->addWidget(notifySpace);

            auto *l = new QVBoxLayout(card);
            l->setContentsMargins(0, 0, 0, 0);
            l->addWidget(cardSpace);

            col->addWidget(card);
            row->addColumn(col);
        }
        rootSpace->addWidget(row);
    }

    {
        auto row = new AsterRow();
        row->setGutter(24);

                // [Card 6] Tabs Component
        {
            auto *col = new AsterColumn(24);
            auto *card = new AsterCard();
            card->setTitle("Tabs");
            
            auto *cardSpace = new AsterSpace(Qt::Vertical);
            cardSpace->setSize(16);

            auto *tabs = new AsterTabs();
            tabs->setFixedHeight(200); // Fixed height for demo

            // Tab 1 Content
            auto *p1 = new QWidget();
            auto *l1 = new QVBoxLayout(p1);
            l1->addWidget(new AsterText("Content of Tab Pane 1"));
            l1->addWidget(new AsterButton("Action 1"));
            l1->addStretch();
            tabs->addTab(p1, "Tab 1");

            // Tab 2 Content
            auto *p2 = new QWidget();
            auto *l2 = new QVBoxLayout(p2);
            l2->addWidget(new AsterText("Content of Tab Pane 2"));
            l2->addWidget(new AsterCheckBox("Option A"));
            l2->addStretch();
            tabs->addTab(p2, "Tab 2");

            // Tab 3 Content
            auto *p3 = new QWidget();
            auto *l3 = new QVBoxLayout(p3);
            l3->addWidget(new AsterText("Content of Tab Pane 3"));
            l3->addStretch();
            tabs->addTab(p3, "Tab 3");

            cardSpace->addWidget(tabs);

            auto *l = new QVBoxLayout(card);
            l->setContentsMargins(0, 0, 0, 0);
            l->addWidget(cardSpace);

            col->addWidget(card);
            row->addColumn(col);
        }

        // --- Section 3: Navigation ---
        {
             auto *col = new AsterColumn(24);
             auto *card = new AsterCard();
             card->setTitle("Navigation - Breadcrumbs");
             
             auto *cardSpace = new AsterSpace(Qt::Vertical);

             auto *bc1 = new AsterBreadcrumb();
             bc1->addItem("Home", [](){ qDebug() << "Home clicked"; });
             bc1->addItem("Application Center", [](){ qDebug() << "App Center clicked"; });
             bc1->addItem("Application List", [](){ qDebug() << "App List clicked"; });
             bc1->addItem("Details");
             cardSpace->addWidget(bc1);

             auto *bc2 = new AsterBreadcrumb();
             bc2->setSeparator(">");
             bc2->addItem("Home");
             bc2->addItem("User");
             bc2->addItem("Profile");
             cardSpace->addWidget(bc2);

             auto *l = new QVBoxLayout(card);
             l->setContentsMargins(0, 0, 0, 0);
             l->addWidget(cardSpace);

             col->addWidget(card);
             row->addColumn(col);
        }
        rootSpace->addWidget(row);
    }

    // --- Section 4: Feedback & Others ---
    {
        auto *row = new AsterRow();
        row->setGutter(24);

        // [Card] Feedback Components
        {
            auto *col = new AsterColumn(12);
            auto *card = new AsterCard();
            card->setTitle("Feedback & Overlay");

            auto *cardSpace = new AsterSpace(Qt::Vertical);
            cardSpace->setSize(16);

            // Alerts
            cardSpace->addWidget(new AsterAlert("Success Tips", AsterAlert::Type::Success));
            cardSpace->addWidget(new AsterAlert("Informational Notes", AsterAlert::Type::Info));
            cardSpace->addWidget(new AsterAlert("Warning", AsterAlert::Type::Warning));
            cardSpace->addWidget(new AsterAlert("Error", AsterAlert::Type::Error));

            // Spin
            auto *spinRow = new AsterSpace(Qt::Horizontal);
            spinRow->setSize(20);
            spinRow->addWidget(new AsterText("Spinning: "));
            spinRow->addWidget(new AsterSpin());
            cardSpace->addWidget(spinRow);

            // Tooltip & Popover
            auto *tpRow = new AsterSpace(Qt::Horizontal);
            auto *btnTip = new AsterButton("Tooltip");
            AsterTooltip::install(btnTip, "This is a tooltip text");
            tpRow->addWidget(btnTip);

            auto *btnPop = new AsterButton("Popover");
            auto *pop = new AsterPopover(btnPop); // parented to button for demo? No, usually parent to window or nothing
            pop->setTitle("Popover Title");
            pop->setContent(new AsterText("Content inside popover"));
            pop->setTarget(btnPop);
            tpRow->addWidget(btnPop);
            
            cardSpace->addWidget(tpRow);

            // Dropdown
            auto *dd = new AsterDropdown("Hover me (Dropdown)");
            dd->addAction("1st menu item", [](){ qDebug() << "1st item clicked"; });
            dd->addAction("2nd menu item", [](){ qDebug() << "2nd item clicked"; });
            cardSpace->addWidget(dd);

            // Drawer
            auto *btnDrawer = new AsterButton("Open Drawer");
            // Drawer needs parent window usually to cover it
            // We can try to use 'window' reference if we capture it, but main() variable isn't accessible easily here
            // Let's create drawer with button as parent for test, or just skip full implementation detail
            // In real app, drawer is attached to window.
            cardSpace->addWidget(btnDrawer);

            auto *l = new QVBoxLayout(card);
            l->setContentsMargins(0, 0, 0, 0);
            l->addWidget(cardSpace);
            
            col->addWidget(card);
            row->addColumn(col);
        }

        // [Card] Collapse & Result
        {
            auto *col = new AsterColumn(12);
            auto *card = new AsterCard();
            card->setTitle("Collapse & Result");
            
            auto *cardSpace = new AsterSpace(Qt::Vertical);
            cardSpace->setSize(16);
            
            // Collapse
            auto *collapse = new AsterCollapse();
            collapse->setAccordion(true);
            collapse->addPanel("Panel 1", new AsterText("Content of Panel 1\nLine 2\nLine 3"));
            collapse->addPanel("Panel 2", new AsterText("Content of Panel 2 is visible when expanded."));
            collapse->addPanel("Panel 3", new AsterButton("A Button inside panel"));
            cardSpace->addWidget(collapse);
            
            // Result (Mini)
            auto *res = new AsterResult();
            res->setStatus(AsterResult::Status::Success);
            res->setTitle("Successfully Purchased");
            // res->setSubTitle("Order number: 2017182818828182881");
            
            cardSpace->addWidget(res);

            auto *l = new QVBoxLayout(card);
            l->setContentsMargins(0, 0, 0, 0);
            l->addWidget(cardSpace);

            col->addWidget(card);
            row->addColumn(col);
        }

        rootSpace->addWidget(row);
    }

    scrollArea->setWidget(rootSpace);
    scrollArea->setWidgetResizable(true);

    window.setCentralWidget(scrollArea);
    window.show();
    return app.exec();
}
