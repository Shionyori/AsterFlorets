#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include "AsterUI/AsterUI.h"

using namespace AsterUI;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AsterTheme::instance();

    QMainWindow window;
    window.setWindowTitle("AsterUI - Slot-Based Layout Demo");
    window.resize(1280, 800);

    // Create the high-level layout container
    AsterLayout *pageLayout = new AsterLayout();
    
    // Set Layout Mode: HeaderFirst (Header top full width) or SiderFirst (Sider left full height)
    // Let's use SiderFirst as it's a very common modern dashboard style (like VS Code or Discord)
    pageLayout->setMode(AsterLayout::Mode::SiderFirst);

    // 1. Setup Sider (Left)
    AsterSider *sider = new AsterSider();
    pageLayout->setSider(sider);
    pageLayout->setSiderWidth(280); // Fixed width for Sider

    auto *siderLayout = new QVBoxLayout(sider);
    auto *logoTitle = new AsterTitle("AsterUI", 3);
    logoTitle->setStyleSheet("color: white; margin: 20px;");
    siderLayout->addWidget(logoTitle);
    
    siderLayout->addSpacing(20);
    siderLayout->addWidget(new AsterButton("Dashboard", AsterButton::Type::Text));
    siderLayout->addWidget(new AsterButton("Settings", AsterButton::Type::Text));
    siderLayout->addWidget(new AsterButton("Profile", AsterButton::Type::Text));
    siderLayout->addStretch();
    
    auto *versionText = new AsterText("v1.0.0");
    versionText->setStyleSheet("color: rgba(255,255,255,0.5); margin: 20px;");
    siderLayout->addWidget(versionText);

    // 2. Setup Header (Top)
    AsterHeader *header = new AsterHeader();
    pageLayout->setHeader(header);
    pageLayout->setHeaderHeight(64); // Fixed height for Header

    auto *headerLayout = new QHBoxLayout(header);
    headerLayout->addWidget(new AsterTitle("Overview", 4));
    headerLayout->addStretch();
    headerLayout->addWidget(new AsterAvatar("U"));
    headerLayout->addWidget(new AsterText("User Name"));
    headerLayout->addSpacing(16);

    // 3. Setup Content (Middle, auto expands)
    AsterContent *content = new AsterContent();
    pageLayout->setContent(content);

    // Use a layout for AsterContent to hold the ScrollArea
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    
    // Create ScrollArea
    AsterScrollArea *scrollArea = new AsterScrollArea();
    contentLayout->addWidget(scrollArea);

    // Create the Scrollable Container (AsterSpace)
    AsterSpace *scrollSpace = new AsterSpace(Qt::Vertical);
    scrollSpace->setObjectName("ScrollSpace");
    // Explicitly set background to white to verify visibility
    scrollSpace->setStyleSheet("#ScrollSpace { background-color: white; }");

    // Note: AsterSpace manages its own layout, use addWidget()
    // Add padding around the content inside the scroll area
    scrollSpace->setContentsMargins(32, 32, 32, 32); 

    // Card 1
    AsterCard *card1 = new AsterCard();
    auto *card1Layout = new QVBoxLayout(card1);
    card1Layout->setContentsMargins(24, 24, 24, 24); 
    card1Layout->addWidget(new AsterTitle("Welcome", 3));
    card1Layout->addWidget(new AsterText("This content is correctly nested: Content > ScrollArea > Space > Card."));
    card1Layout->addSpacing(10);
    card1Layout->addWidget(new AsterText("Key Fix: Do not create a new layout on AsterSpace. Use addWidget directly."));
    card1Layout->addWidget(new AsterText("Scrolling Fix: Added more cards and enabled background styling."));
    
    scrollSpace->addWidget(card1);

    // Add extra cards to demonstrate scrolling
    for(int i=0; i<10; ++i) {
        AsterCard *extraCard = new AsterCard();
        auto *l = new QVBoxLayout(extraCard);
        l->addWidget(new AsterTitle(QString("Feature Card %1").arg(i+1), 4));
        l->addWidget(new AsterText(QString("This is item number %1 in the list. Ensure it scrolls.").arg(i+1)));
        l->addStretch();
        scrollSpace->addWidget(extraCard);
    }
    
    scrollSpace->addStretch();

    // Set the widget for ScrollArea
    scrollArea->setWidget(scrollSpace);

    // 4. Setup Footer (Bottom)
    AsterFooter *footer = new AsterFooter();
    pageLayout->setFooter(footer);
    pageLayout->setFooterHeight(40); // Fixed height

    auto *footerLayout = new QHBoxLayout(footer);
    auto *ft = new AsterText("© 2026 AsterUI");
    ft->setStyleSheet("color: #999;");
    footerLayout->addWidget(ft);
    footerLayout->setAlignment(Qt::AlignCenter);

    window.setCentralWidget(pageLayout);
    window.show();

    return app.exec();
}
