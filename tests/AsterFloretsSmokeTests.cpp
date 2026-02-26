#include <QtTest/QtTest>
#include <QSignalSpy>

#include "AsterButton.h"
#include "AsterTabs.h"
#include "AsterTheme.h"
#include "AsterTextInput.h"
#include "AsterSelect.h"
#include "AsterQuick.h"
#include "AsterModal.h"

using namespace AsterFlorets;

class AsterFloretsSmokeTests : public QObject {
    Q_OBJECT

private slots:
    void button_type_switch_updates_state();
    void tabs_add_remove_keeps_page_alive();
    void theme_dark_mode_switch_works();
    void text_input_status_switch_works();
    void select_can_add_items();
    void quick_api_creates_controls();
    void button_loading_disables_control();
    void tabs_support_keyboard_navigation();
    void modal_supports_keyboard_shortcuts();
    void accessibility_metadata_is_present();
};

void AsterFloretsSmokeTests::button_type_switch_updates_state() {
    AsterButton button("Click");
    QCOMPARE(button.type(), AsterButton::Type::Default);

    button.setType(AsterButton::Type::Primary);
    QCOMPARE(button.type(), AsterButton::Type::Primary);
}

void AsterFloretsSmokeTests::tabs_add_remove_keeps_page_alive() {
    AsterTabs tabs;

    auto* page1 = new QWidget();
    auto* page2 = new QWidget();

    const int idx1 = tabs.addTab(page1, "Tab1");
    const int idx2 = tabs.addTab(page2, "Tab2");

    QCOMPARE(idx1, 0);
    QCOMPARE(idx2, 1);
    QCOMPARE(tabs.count(), 2);

    tabs.removeTab(0);
    QCOMPARE(tabs.count(), 1);

    QVERIFY(page1 != nullptr);
    QVERIFY(page1->parent() != &tabs);

    delete page1;
}

void AsterFloretsSmokeTests::theme_dark_mode_switch_works() {
    auto* theme = AsterTheme::instance();

    theme->setDarkMode(true);
    QVERIFY(theme->isDarkMode());

    const auto darkPrimary = theme->color(AsterTheme::ColorRole::Primary);
    QVERIFY(darkPrimary.isValid());

    theme->setDarkMode(false);
    QVERIFY(!theme->isDarkMode());
}

void AsterFloretsSmokeTests::text_input_status_switch_works() {
    AsterTextInput input;
    QCOMPARE(input.status(), AsterTextInput::Status::Normal);

    input.setStatus(AsterTextInput::Status::Error);
    QCOMPARE(input.status(), AsterTextInput::Status::Error);

    input.setStatus(AsterTextInput::Status::Warning);
    QCOMPARE(input.status(), AsterTextInput::Status::Warning);
}

void AsterFloretsSmokeTests::select_can_add_items() {
    AsterSelect select;
    select.addItem("One");
    select.addItem("Two");

    QCOMPARE(select.count(), 2);
    QCOMPARE(select.itemText(0), QString("One"));
}

void AsterFloretsSmokeTests::quick_api_creates_controls() {
    QWidget host;

    auto* primary = Quick::primaryButton("Save", &host);
    QVERIFY(primary != nullptr);
    QCOMPARE(primary->type(), AsterButton::Type::Primary);

    auto* input = Quick::input("Type here", true, &host);
    QVERIFY(input != nullptr);
    QCOMPARE(input->placeholderText(), QString("Type here"));
    QVERIFY(input->isClearable());

    auto* select = Quick::select({"A", "B"}, "Choose", &host);
    QVERIFY(select != nullptr);
    QCOMPARE(select->count(), 2);
    QCOMPARE(select->placeholderText(), QString("Choose"));
    QCOMPARE(select->currentIndex(), -1);

    auto* configuredInput = Quick::with(Quick::input("Email", false, &host), [](AsterTextInput* inputCtrl) {
        inputCtrl->setClearable(true);
        inputCtrl->setText("hello");
    });
    QVERIFY(configuredInput != nullptr);
    QVERIFY(configuredInput->isClearable());
    QCOMPARE(configuredInput->text(), QString("hello"));
}

void AsterFloretsSmokeTests::button_loading_disables_control() {
    AsterButton button("Submit");
    QVERIFY(button.isEnabled());

    button.setLoading(true);
    QVERIFY(!button.isEnabled());

    button.setLoading(false);
    QVERIFY(button.isEnabled());
}

void AsterFloretsSmokeTests::tabs_support_keyboard_navigation() {
    AsterTabs tabs;
    tabs.addTab(new QWidget(), "General");
    tabs.addTab(new QWidget(), "Advanced");
    tabs.addTab(new QWidget(), "About");
    tabs.setCurrentIndex(0);

    tabs.show();
    QTest::qWait(10);

    QWidget* tabBar = tabs.findChild<QWidget*>();
    QVERIFY(tabBar != nullptr);

    tabBar->setFocus(Qt::OtherFocusReason);
    QTest::keyClick(tabBar, Qt::Key_Right);
    QCOMPARE(tabs.currentIndex(), 1);

    QTest::keyClick(tabBar, Qt::Key_End);
    QCOMPARE(tabs.currentIndex(), 2);

    QTest::keyClick(tabBar, Qt::Key_Home);
    QCOMPARE(tabs.currentIndex(), 0);
}

void AsterFloretsSmokeTests::modal_supports_keyboard_shortcuts() {
    AsterModal modal;
    modal.setTitle("Confirm");
    modal.setContent("Proceed?");
    modal.show();
    QTest::qWait(10);

    QSignalSpy acceptedSpy(&modal, &QDialog::accepted);
    QSignalSpy rejectedSpy(&modal, &QDialog::rejected);

    QTest::keyClick(&modal, Qt::Key_Return);
    QCOMPARE(acceptedSpy.count(), 1);

    AsterModal modal2;
    modal2.setTitle("Confirm");
    modal2.setContent("Proceed?");
    modal2.show();
    QTest::qWait(10);

    QSignalSpy acceptedSpy2(&modal2, &QDialog::accepted);
    QSignalSpy rejectedSpy2(&modal2, &QDialog::rejected);

    QTest::keyClick(&modal2, Qt::Key_Escape);
    QCOMPARE(rejectedSpy2.count(), 1);
    QCOMPARE(acceptedSpy2.count(), 0);
}

void AsterFloretsSmokeTests::accessibility_metadata_is_present() {
    AsterModal modal;
    modal.setTitle("User Settings");
    modal.setContent("Update your profile information");

    QCOMPARE(modal.accessibleName(), QString("User Settings"));

    AsterTabs tabs;
    tabs.addTab(new QWidget(), "General");
    tabs.addTab(new QWidget(), "Security");

    QWidget* tabBar = tabs.findChild<QWidget*>();
    QVERIFY(tabBar != nullptr);
    QCOMPARE(tabBar->accessibleName(), QString("Aster Tabs"));
    QCOMPARE(tabBar->accessibleDescription(), QString("Use arrow keys to switch tabs"));
}

QTEST_MAIN(AsterFloretsSmokeTests)
#include "AsterFloretsSmokeTests.moc"
