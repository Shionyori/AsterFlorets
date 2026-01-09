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

    // 设置主调色板
    QPalette pal = window.palette();
    // 设置 QMainWindow 窗口背景色
    pal.setColor(QPalette::Window, theme->color(AsterUI::AsterTheme::ColorRole::Background));
    window.setPalette(pal);

    AsterUI::AsterScrollArea *scrollArea = new AsterUI::AsterScrollArea();

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
                AsterNotification::success("Download Complete", "The file 'AsterUI_v1.0.zip' has been downloaded successfully.", 4500, winPtr);
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

    scrollArea->setWidget(rootSpace);
    scrollArea->setWidgetResizable(true);

    window.setCentralWidget(scrollArea);
    window.show();
    return app.exec();
}
