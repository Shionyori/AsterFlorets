#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include "AsterUI.h"

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
    pageLayout->setMode(AsterLayout::Mode::SiderFirst);

    // 1. Setup Sider (Left)
    AsterSider *sider = new AsterSider();
    pageLayout->setSider(sider);
    pageLayout->setSiderWidth(280); // Fixed width for Sider

    // Layout is now managed by AsterSider (Default: QVBoxLayout, 0 margins)
    auto *siderLayout = static_cast<QVBoxLayout*>(sider->layout());

    // --- Logo Area ---
    QWidget* logoArea = new QWidget();
    logoArea->setFixedHeight(64);
    auto* logoLayout = new QHBoxLayout(logoArea);
    logoLayout->setContentsMargins(20, 0, 0, 0);
    auto *logoTitle = new AsterTitle("AsterUI", 3);
    logoLayout->addWidget(logoTitle);
    siderLayout->addWidget(logoArea);
    
    // --- Main Menu ---
    AsterMenu *siderMenu = new AsterMenu();
    siderMenu->setMode(AsterMenu::Mode::Vertical);
    
    // Add items with Keys, Titles, and Icons (optional)
    siderMenu->addItem("dashboard", "Dashboard", QIcon());
    siderMenu->addItem("projects", "Projects", QIcon());
    siderMenu->addItem("reports", "Reports", QIcon());
    siderMenu->addItem("settings", "Settings", QIcon());
    
    // Set initial selection
    siderMenu->setSelectedKey("dashboard");
    
    // Explicitly set alignment (default is Top, but good to show usage)
    siderMenu->setAlignment(Qt::AlignTop);

    siderLayout->addWidget(siderMenu);
    
    // --- Footer/Version Area ---
    // If we want a footer in Sider, we can add it here.
    // Menu is expanding, so it pushes this down.
    QWidget* versionArea = new QWidget();
    auto* vl = new QVBoxLayout(versionArea);
    auto *versionText = new AsterText("v1.0.0");
    versionText->setStyleSheet("color: rgba(0,0,0,0.5);"); 
    vl->addWidget(versionText);
    vl->setAlignment(Qt::AlignCenter);
    
    siderLayout->addWidget(versionArea);

    // 2. Setup Header (Top)
    AsterHeader *header = new AsterHeader();
    pageLayout->setHeader(header);
    pageLayout->setHeaderHeight(64); // Fixed height for Header

    auto *headerLayout = static_cast<QHBoxLayout*>(header->layout());
    headerLayout->addWidget(new AsterTitle("Overview", 4));
    headerLayout->addStretch();
    headerLayout->addWidget(new AsterAvatar("U"));
    headerLayout->addWidget(new AsterText("User Name"));
    headerLayout->addSpacing(16);

    // 3. Setup Content (Middle, auto expands)
    AsterContent *content = new AsterContent();
    pageLayout->setContent(content);

    // AsterContent -> Default Layout (VBox) -> AsterScrollArea
    auto *contentLayout = static_cast<QVBoxLayout*>(content->layout());
    
    // Create ScrollArea (Now includes internal container)
    AsterScrollArea *scrollArea = new AsterScrollArea();
    scrollArea->setContentsMargins(32, 32, 32, 32); 

    // // Access internal container to style it if needed (optional)
    // // scrollArea->container()->setObjectName("ScrollContent");
    // // scrollArea->container()->setStyleSheet("#ScrollContent { background-color: white; }");

    contentLayout->addWidget(scrollArea);

    // Card 1: Add DIRECTLY to scrollArea
    AsterCard *card1 = new AsterCard();
    auto *card1Layout = new QVBoxLayout(card1);
    card1Layout->setContentsMargins(24, 24, 24, 24); 
    card1Layout->addWidget(new AsterTitle("Unified API", 3));
    card1Layout->addWidget(new AsterText("Now AsterScrollArea works like a container naturally."));
    card1Layout->addSpacing(10);
    card1Layout->addWidget(new AsterText("Usage: scrollArea->addWidget(card)"));
    
    scrollArea->addWidget(card1);

    // Add extra cards
    for(int i=0; i<10; ++i) {
        AsterCard *extraCard = new AsterCard();
        auto *l = new QVBoxLayout(extraCard);
        l->addWidget(new AsterTitle(QString("Unified Card %1").arg(i+1), 4));
        l->addWidget(new AsterText(QString("This item is added directly to ScrollArea.").arg(i+1)));
        l->addStretch();
        scrollArea->addWidget(extraCard);
    }
    
    scrollArea->addStretch();

    // 4. Setup Footer (Bottom)
    AsterFooter *footer = new AsterFooter();
    pageLayout->setFooter(footer);
    pageLayout->setFooterHeight(40); // Fixed height

    auto *footerLayout = static_cast<QHBoxLayout*>(footer->layout());
    auto *ft = new AsterText("© 2026 AsterUI");
    ft->setStyleSheet("color: #999;");
    footerLayout->addWidget(ft);
    footerLayout->setAlignment(Qt::AlignCenter);

    window.setCentralWidget(pageLayout);
    window.show();

    return app.exec();
}
