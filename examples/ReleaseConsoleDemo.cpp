#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QStackedWidget>
#include <QDateTime>
#include <QFrame>
#include <QLabel>
#include "AsterFlorets.h"

using namespace AsterFlorets;

class ReleaseConsoleDemo : public QMainWindow {
public:
    ReleaseConsoleDemo() {
        buildUi();
        wireEvents();
        seedRecentReleases();
    }

private:
    AsterMenu* navMenu = nullptr;
    QStackedWidget* pageStack = nullptr;

    QLabel* releaseCountValue = nullptr;
    QLabel* successRateValue = nullptr;
    QLabel* progressValue = nullptr;

    AsterProgress* deploymentProgress = nullptr;
    AsterTag* envTag = nullptr;
    AsterSelect* projectSelect = nullptr;

    AsterTextInput* versionInput = nullptr;
    AsterTextArea* notesInput = nullptr;
    AsterSelect* envSelect = nullptr;
    AsterSelect* strategySelect = nullptr;
    AsterSwitch* canarySwitch = nullptr;

    QVBoxLayout* releaseFeedLayout = nullptr;
    int releaseCount = 12;
    int successRate = 98;
    int rolloutProgress = 64;

    void buildUi() {
        setWindowTitle("AsterFlorets Demo - Release Console");
        resize(1260, 780);

        auto* central = new QWidget(this);
        auto* root = new QHBoxLayout(central);
        root->setContentsMargins(16, 16, 16, 16);
        root->setSpacing(12);

        auto* navCard = new AsterCard();
        navCard->setFixedWidth(240);
        navCard->setTitle("Release Console");
        navCard->setHoverable(false);

        auto* navLayout = new QVBoxLayout(navCard);
        navLayout->setContentsMargins(12, 32, 12, 12);
        navLayout->setSpacing(10);

        auto* profile = new AsterText("Team: Platform Ops");
        navLayout->addWidget(profile);

        navMenu = new AsterMenu();
        navMenu->addItem("dashboard", "仪表盘");
        navMenu->addItem("release", "发布配置");
        navMenu->addItem("incidents", "异常中心");
        navMenu->setSelectedKey("dashboard");
        navLayout->addWidget(navMenu);
        navLayout->addStretch();

        auto* right = new QWidget();
        auto* rightLayout = new QVBoxLayout(right);
        rightLayout->setContentsMargins(0, 0, 0, 0);
        rightLayout->setSpacing(12);

        rightLayout->addWidget(createTopBar());

        pageStack = new QStackedWidget();
        pageStack->addWidget(createDashboardPage());
        pageStack->addWidget(createReleasePage());
        pageStack->addWidget(createIncidentsPage());
        rightLayout->addWidget(pageStack, 1);

        root->addWidget(navCard);
        root->addWidget(right, 1);

        setCentralWidget(central);
    }

    QWidget* createTopBar() {
        auto* container = new AsterCard();
        container->setHoverable(false);

        auto* layout = new QHBoxLayout(container);
        layout->setContentsMargins(16, 12, 16, 12);
        layout->setSpacing(12);

        auto* title = new AsterTitle("部署运营台", 3);
        layout->addWidget(title);
        layout->addStretch();

        projectSelect = new AsterSelect();
        projectSelect->setMinimumWidth(180);
        projectSelect->addItems({"aster-ui-core", "payment-service", "gateway-api"});
        projectSelect->setCurrentIndex(0);
        layout->addWidget(projectSelect);

        envTag = new AsterTag("Production");
        envTag->setType(AsterTag::Type::Error);
        layout->addWidget(envTag);

        auto* notifyBtn = new AsterButton("测试通知", AsterButton::Type::Default);
        auto* themeBtn = new AsterButton("切换明暗", AsterButton::Type::Text);
        layout->addWidget(notifyBtn);
        layout->addWidget(themeBtn);

        connect(notifyBtn, &AsterButton::clicked, this, [this]() {
            AsterNotification::info("系统通知", "当前无阻塞告警", 2500, this);
        });

        connect(themeBtn, &AsterButton::clicked, this, []() {
            auto* theme = AsterTheme::instance();
            theme->setDarkMode(!theme->isDarkMode());
        });

        return container;
    }

    QWidget* createDashboardPage() {
        auto* page = new QWidget();
        auto* layout = new QVBoxLayout(page);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(12);

        auto* tabs = new AsterTabs();

        auto* overview = new QWidget();
        auto* overviewLayout = new QVBoxLayout(overview);
        overviewLayout->setContentsMargins(0, 0, 0, 0);
        overviewLayout->setSpacing(12);

        auto* metricRow = new QWidget();
        auto* metricLayout = new QHBoxLayout(metricRow);
        metricLayout->setContentsMargins(0, 0, 0, 0);
        metricLayout->setSpacing(12);

        metricLayout->addWidget(createKpiCard("本周发布次数", QString::number(releaseCount), "较上周 +2", &releaseCountValue));
        metricLayout->addWidget(createKpiCard("成功率", QString::number(successRate) + "%", "24h 无回滚", &successRateValue));
        metricLayout->addWidget(createKpiCard("灰度进度", QString::number(rolloutProgress) + "%", "进行中", &progressValue));
        overviewLayout->addWidget(metricRow);

        auto* progressCard = new AsterCard();
        progressCard->setTitle("当前发布进度");
        auto* progressLayout = new QVBoxLayout(progressCard);
        progressLayout->setContentsMargins(12, 32, 12, 12);

        deploymentProgress = new AsterProgress();
        deploymentProgress->setRange(0, 100);
        deploymentProgress->setValue(rolloutProgress);
        progressLayout->addWidget(deploymentProgress);
        progressLayout->addWidget(new AsterText("集群滚动升级中，预计 8 分钟完成"));

        overviewLayout->addWidget(progressCard);

        auto* activity = new QWidget();
        auto* activityLayout = new QVBoxLayout(activity);
        activityLayout->setContentsMargins(0, 0, 0, 0);
        activityLayout->setSpacing(10);

        auto* feedCard = new AsterCard();
        feedCard->setTitle("最近发布记录");
        auto* feedCardLayout = new QVBoxLayout(feedCard);
        feedCardLayout->setContentsMargins(12, 32, 12, 12);

        auto* feedContainer = new QWidget();
        releaseFeedLayout = new QVBoxLayout(feedContainer);
        releaseFeedLayout->setContentsMargins(0, 0, 0, 0);
        releaseFeedLayout->setSpacing(8);
        feedCardLayout->addWidget(feedContainer);

        activityLayout->addWidget(feedCard);

        tabs->addTab(overview, "总览");
        tabs->addTab(activity, "发布流水");
        layout->addWidget(tabs);

        return page;
    }

    QWidget* createReleasePage() {
        auto* page = new QWidget();
        auto* layout = new QVBoxLayout(page);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(12);

        auto* formCard = new AsterCard();
        formCard->setTitle("新建发布任务");
        auto* formCardLayout = new QVBoxLayout(formCard);
        formCardLayout->setContentsMargins(12, 32, 12, 12);
        formCardLayout->setSpacing(10);

        auto* form = new QFormLayout();
        form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
        form->setHorizontalSpacing(12);
        form->setVerticalSpacing(10);

        versionInput = new AsterTextInput("例如: v1.8.12");
        versionInput->setClearable(true);

        envSelect = new AsterSelect();
        envSelect->addItems({"Production", "Staging", "Dev"});
        envSelect->setCurrentIndex(-1);
        envSelect->setPlaceholderText("请选择环境");

        strategySelect = new AsterSelect();
        strategySelect->addItems({"Rolling", "Blue-Green", "Canary"});
        strategySelect->setCurrentIndex(0);

        canarySwitch = new AsterSwitch();
        canarySwitch->setChecked(true);

        notesInput = new AsterTextArea("填写变更说明、风险点与回滚策略");
        notesInput->setMinimumHeight(120);

        form->addRow("版本号", versionInput);
        form->addRow("部署环境", envSelect);
        form->addRow("发布策略", strategySelect);
        form->addRow("启用灰度", canarySwitch);
        form->addRow("变更说明", notesInput);

        formCardLayout->addLayout(form);

        auto* actionRow = new QHBoxLayout();
        actionRow->addStretch();

        auto* previewBtn = new AsterButton("预检", AsterButton::Type::Default);
        auto* submitBtn = new AsterButton("发起发布", AsterButton::Type::Primary);
        actionRow->addWidget(previewBtn);
        actionRow->addWidget(submitBtn);

        formCardLayout->addLayout(actionRow);

        connect(previewBtn, &AsterButton::clicked, this, [this]() {
            AsterMessage::info("预检通过：依赖检查、资源配额、镜像签名均正常", 2500, this);
        });

        connect(submitBtn, &AsterButton::clicked, this, [this]() {
            submitRelease();
        });

        layout->addWidget(formCard);
        layout->addStretch();
        return page;
    }

    QWidget* createIncidentsPage() {
        auto* page = new QWidget();
        auto* layout = new QVBoxLayout(page);
        layout->setContentsMargins(0, 0, 0, 0);

        auto* result = new AsterResult();
        result->setStatus(AsterResult::Status::Info);
        result->setTitle("当前无进行中的 P1/P0 事故");
        result->setSubTitle("最近一次异常已在 2 小时前恢复，建议关注发布后 30 分钟指标波动。");

        auto* extraBtn = new AsterButton("查看告警历史", AsterButton::Type::Primary);
        connect(extraBtn, &AsterButton::clicked, this, [this]() {
            AsterMessage::info("告警历史检索功能示例：此处可跳转到日志中心", 2200, this);
        });
        result->setExtra(extraBtn);

        layout->addWidget(result);
        return page;
    }

    AsterCard* createKpiCard(const QString& title, const QString& value, const QString& hint, QLabel** valueLabelOut) {
        auto* card = new AsterCard();
        card->setHoverable(true);

        auto* layout = new QVBoxLayout(card);
        layout->setContentsMargins(12, 12, 12, 12);
        layout->setSpacing(6);

        auto* titleText = new AsterText(title);
        auto* valueText = new AsterTitle(value, 2);
        auto* hintText = new AsterText(hint);

        layout->addWidget(titleText);
        layout->addWidget(valueText);
        layout->addWidget(hintText);

        if (valueLabelOut) {
            *valueLabelOut = valueText;
        }

        return card;
    }

    void wireEvents() {
        connect(navMenu, &AsterMenu::itemClicked, this, [this](const QString& key) {
            if (key == "dashboard") {
                pageStack->setCurrentIndex(0);
            } else if (key == "release") {
                pageStack->setCurrentIndex(1);
            } else {
                pageStack->setCurrentIndex(2);
            }
            navMenu->setSelectedKey(key);
        });

        connect(projectSelect, &QComboBox::currentTextChanged, this, [this](const QString& project) {
            AsterMessage::info(QString("已切换项目：%1").arg(project), 1600, this);
        });
    }

    void seedRecentReleases() {
        addReleaseRecord("v1.8.10", "Rolling", "Production", "Success");
        addReleaseRecord("v1.8.09", "Canary", "Production", "Success");
        addReleaseRecord("v1.8.08", "Blue-Green", "Staging", "Warning");
        releaseFeedLayout->addStretch();
    }

    void addReleaseRecord(const QString& version, const QString& strategy, const QString& environment, const QString& status) {
        auto* rowCard = new AsterCard();
        rowCard->setHoverable(true);

        auto* rowLayout = new QHBoxLayout(rowCard);
        rowLayout->setContentsMargins(10, 10, 10, 10);

        auto* left = new QWidget();
        auto* leftLayout = new QVBoxLayout(left);
        leftLayout->setContentsMargins(0, 0, 0, 0);
        leftLayout->setSpacing(2);

        leftLayout->addWidget(new AsterText(QString("版本 %1 · %2").arg(version, strategy)));
        leftLayout->addWidget(new AsterText(QString("%1 · %2").arg(environment, QDateTime::currentDateTime().toString("MM-dd hh:mm"))));

        rowLayout->addWidget(left, 1);

        auto* statusTag = new AsterTag(status);
        if (status == "Success") {
            statusTag->setType(AsterTag::Type::Success);
        } else if (status == "Warning") {
            statusTag->setType(AsterTag::Type::Warning);
        } else {
            statusTag->setType(AsterTag::Type::Error);
        }

        rowLayout->addWidget(statusTag);

        releaseFeedLayout->insertWidget(0, rowCard);
    }

    void submitRelease() {
        const QString version = versionInput->text().trimmed();
        const QString environment = envSelect->currentText();
        const QString strategy = strategySelect->currentText();
        const QString notes = notesInput->toPlainText().trimmed();

        bool valid = true;

        if (version.isEmpty()) {
            versionInput->setStatus(AsterTextInput::Status::Error);
            valid = false;
        } else {
            versionInput->setStatus(AsterTextInput::Status::Normal);
        }

        if (environment.isEmpty()) {
            AsterMessage::warning("请选择部署环境", 2200, this);
            valid = false;
        }

        if (notes.isEmpty()) {
            notesInput->setStatus(AsterTextArea::Status::Warning);
        } else {
            notesInput->setStatus(AsterTextArea::Status::Normal);
        }

        if (!valid) {
            AsterMessage::error("发布任务创建失败：请补齐必填项", 2500, this);
            return;
        }

        const bool confirmed = AsterModal::confirm(
            this,
            "确认发起发布",
            QString("版本 %1 将发布到 %2，策略 %3。是否继续？").arg(version, environment, strategy),
            "确认发布",
            "取消"
        );

        if (!confirmed) {
            AsterMessage::info("已取消发布", 1500, this);
            return;
        }

        releaseCount += 1;
        successRate = qMax(85, successRate - (canarySwitch->isChecked() ? 0 : 1));
        rolloutProgress = qMin(100, rolloutProgress + 7);

        releaseCountValue->setText(QString::number(releaseCount));
        successRateValue->setText(QString::number(successRate) + "%");
        progressValue->setText(QString::number(rolloutProgress) + "%");
        deploymentProgress->setValue(rolloutProgress);

        addReleaseRecord(version, strategy, environment, "Success");

        envTag->setText(environment);
        envTag->setType(environment == "Production" ? AsterTag::Type::Error : AsterTag::Type::Primary);

        AsterNotification::success("发布已创建", QString("%1 -> %2 (%3)").arg(version, environment, strategy), 2800, this);
        AsterMessage::success("发布流水已更新", 1800, this);

        versionInput->clear();
        notesInput->setPlainText(QString());
        pageStack->setCurrentIndex(0);
        navMenu->setSelectedKey("dashboard");
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    ReleaseConsoleDemo demo;
    demo.show();

    return app.exec();
}
