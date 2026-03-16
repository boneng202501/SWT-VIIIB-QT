#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QTimer>
#include <QFontDatabase>
#include <QApplication>
#include <QRandomGenerator>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("高压开关机械特性测试系统");
    resize(1360, 860);

    buildInterface();
    applyGlobalStyles();
    setupCurvePage();
    setupDashboardPage();
    // 默认选中导航第一项 → 显示曲线页
    sidebar->setCurrentRow(0);
}

MainWindow::~MainWindow() = default;

void MainWindow::buildInterface()
{
    central = new QWidget(this);
    setCentralWidget(central);

    rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // ────────────── 顶部栏 ──────────────
    topPanel = new QWidget();
    topPanel->setFixedHeight(52);
    QHBoxLayout *topLay = new QHBoxLayout(topPanel);
    topLay->setContentsMargins(16, 0, 16, 0);
    topLay->setSpacing(12);

    pathLabel = new QLabel("Home > 测试管理");
    pathLabel->setStyleSheet("color:#b0b0b0; font-size:13px;");
    topLay->addWidget(pathLabel);

    topLay->addStretch();

    protocolCombo = new QComboBox();
    protocolCombo->addItems({"UDP", "TCP", "串口"});
    protocolCombo->setFixedWidth(110);
    topLay->addWidget(protocolCombo);

    ipEdit = new QLineEdit("127.0.0.1");
    ipEdit->setPlaceholderText("IP");
    ipEdit->setFixedWidth(150);
    topLay->addWidget(ipEdit);

    portEdit = new QLineEdit("5000");
    portEdit->setPlaceholderText("端口");
    portEdit->setFixedWidth(90);
    topLay->addWidget(portEdit);

    connectBtn = new QPushButton("连接设备");
    connectBtn->setFixedWidth(110);
    topLay->addWidget(connectBtn);

    rootLayout->addWidget(topPanel);

    // ────────────── 中间 Splitter ──────────────
    QSplitter *split = new QSplitter(Qt::Horizontal);
    split->setHandleWidth(2);

    // 左侧导航栏
    QWidget *leftSide = new QWidget();
    leftSide->setFixedWidth(240);
    QVBoxLayout *leftLay = new QVBoxLayout(leftSide);
    leftLay->setContentsMargins(0, 10, 0, 10);
    leftLay->setSpacing(2);

    sidebar = new QListWidget();
    sidebar->setIconSize(QSize(22, 22));
    sidebar->setStyleSheet("QListWidget::item{padding:12px 18px;}");

    QStringList menu = {
        "仪表盘", "设备连接", "电压调节",
        "采样控制", "波形分析", "历史记录", "报表导出"
    };

    for (const auto &txt : menu) {
        auto *item = new QListWidgetItem(txt);
        sidebar->addItem(item);
    }

    leftLay->addWidget(sidebar);
    split->addWidget(leftSide);

    // 中央内容
    contentStack = new QStackedWidget();
    split->addWidget(contentStack);

    rootLayout->addWidget(split, 1);

    // ────────────── 底部操作栏 ──────────────
    footer = new QWidget();
    footer->setFixedHeight(64);
    QHBoxLayout *footLay = new QHBoxLayout(footer);
    footLay->setContentsMargins(16, 10, 16, 10);
    footLay->setSpacing(16);

    btnOneKey = new QPushButton("一键测试");
    btnOneKey->setFixedHeight(44);
    btnOneKey->setMinimumWidth(140);
    footLay->addWidget(btnOneKey);

    btnIOInit = new QPushButton("IO 初始化");
    btnIOInit->setFixedHeight(44);
    footLay->addWidget(btnIOInit);

    btnVoltAdj = new QPushButton("电压调节");
    btnVoltAdj->setFixedHeight(44);
    footLay->addWidget(btnVoltAdj);

    testProgress = new QProgressBar();
    testProgress->setRange(0, 100);
    testProgress->setValue(0);
    testProgress->setFormat(" %p% ");
    footLay->addWidget(testProgress, 1);

    rootLayout->addWidget(footer);

    // ────────────── 日志 Dock ──────────────
    logDock = new QDockWidget("运行日志", this);
    logDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    logView = new QTextEdit();
    logView->setReadOnly(true);
    logDock->setWidget(logView);
    addDockWidget(Qt::BottomDockWidgetArea, logDock);

    // 信号槽
    connect(sidebar, &QListWidget::currentItemChanged, this, &MainWindow::onNavigationChanged);
    connect(connectBtn, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
    connect(btnOneKey, &QPushButton::clicked, this, &MainWindow::onOneKeyTestClicked);
}

void MainWindow::applyGlobalStyles()
{
    QString style = R"(
        QMainWindow {
            background: #0a0f1a;                /* 深蓝黑背景，贴近你的图片 */
        }

        QWidget {
            color: #d0e0ff;                     /* 浅蓝白文字，高对比 */
            font-family: "Segoe UI", "Microsoft YaHei", sans-serif;
            font-size: 13px;
        }

        QSplitter::handle {
            background: #1a2a3a;                /* 微蓝分隔条 */
            width: 2px;
        }

        /* 左侧导航栏 */
        QListWidget {
            background: #0f1622;
            border-right: 1px solid #1e2a3f;
        }

        QListWidget::item {
            padding: 12px 18px;
            border-radius: 4px;
            color: #a0b0d0;
        }

        QListWidget::item:selected {
            background: #1e3a5a;                /* 选中深蓝 */
            color: #ffffff;
            border-left: 4px solid #00bfff;     /* 青蓝光条，贴近你的高亮 */
        }

        QListWidget::item:hover {
            background: #152535;
        }

        /* 顶部连接区 */
        QComboBox, QLineEdit {
            background: #111822;
            color: #d0e0ff;
            border: 1px solid #2a3a50;
            border-radius: 4px;
            padding: 6px;
        }

        QPushButton#connectBtn {
            background: #1e3a5a;
            color: #ffffff;
            border: none;
            border-radius: 4px;
        }

        QPushButton#connectBtn:hover {
            background: #2a4a6a;
        }

        QPushButton#connectBtn[connected="true"] {
            background: #00b050;                /* 连接成功绿（可调成青蓝） */
        }

        /* 一键测试按钮 - 保持你的原绿，但加 cyan 变体可选 */
        QPushButton#onekey {
            background: #27ae60;                /* 原绿 */
            color: white;
            border: none;
            font: bold 15px;
            border-radius: 6px;
            min-height: 44px;
        }

        QPushButton#onekey:hover {
            background: #00c080;                /* hover 偏 cyan 绿 */
        }

        QPushButton#onekey:pressed {
            background: #1e8a4a;
        }

        /* 进度条 - 青蓝填充 */
        QProgressBar {
            background: #111822;
            border: 1px solid #2a3a50;
            border-radius: 4px;
            text-align: center;
            color: #d0e0ff;
        }

        QProgressBar::chunk {
            background: #00bfff;                /* 青蓝进度条，贴近你的风格 */
            border-radius: 3px;
        }

        /* 日志区 - 深黑 + 青绿/白文字 */
        QTextEdit {
            background: #050a12;
            color: #c0ffd0;                     /* 浅绿日志文字 */
            border-top: 1px solid #1e2a3f;
            font-family: Consolas, "Courier New", monospace;
            font-size: 12px;
        }

        /* QCustomPlot 暗黑 + 青蓝曲线 */
        QCustomPlot {
            background: #0a101c;
        }

        /* 其他按钮（如 IO初始化、电压调节） */
        QPushButton {
            background: #1e2a3a;
            color: #d0e0ff;
            border: 1px solid #2a3a50;
            border-radius: 4px;
        }

        QPushButton:hover {
            background: #2a3a50;
        }
    )";

    // 设置 objectName 以匹配 QSS
    btnOneKey->setObjectName("onekey");
    connectBtn->setObjectName("connectBtn");

    qApp->setStyleSheet(style);
}

// void MainWindow::applyGlobalStyles()
// {
//     QString style = R"(
//         QMainWindow {
//             background: #181818;
//         }

//         QSplitter::handle {
//             background: #2a2a2a;
//             width: 2px;
//         }

//         /* 通用按钮 */
//         QPushButton {
//             background: #2e2e2e;
//             color: #e8e8e8;
//             border: 1px solid #3c3c3c;
//             border-radius: 4px;
//             padding: 6px 14px;
//         }

//         QPushButton:hover {
//             background: #3c3c3c;
//         }

//         QPushButton:pressed {
//             background: #252525;
//         }

//         /* 一键测试 - 核心绿色强调 */
//         QPushButton#onekey {
//             background: #27ae60;
//             color: white;
//             border: none;
//             font: bold 15px;
//             border-radius: 6px;
//             min-height: 44px;
//         }

//         QPushButton#onekey:hover {
//             background: #2ecc71;          /* 鼠标悬停更亮 */
//         }

//         QPushButton#onekey:pressed {
//             background: #1e8449;          /* 按下更深 */
//         }

//         /* 进度条 - 绿色填充 */
//         QProgressBar {
//             background: #2e2e2e;
//             border: 1px solid #3c3c3c;
//             border-radius: 4px;
//             text-align: center;
//             color: #ccc;
//         }

//         QProgressBar::chunk {
//             background: #27ae60;
//             border-radius: 3px;
//         }

//         /* 左侧导航选中 */
//         QListWidget::item:selected {
//             background: #27ae60;          /* 用你的绿突出选中 */
//             color: white;
//         }

//         QListWidget::item {
//             padding: 12px 18px;
//             border-radius: 4px;
//         }

//         /* 输入框、下拉框 */
//         QLineEdit, QComboBox {
//             background: #222222;
//             color: #dddddd;
//             border: 1px solid #444444;
//             border-radius: 4px;
//             padding: 5px;
//         }

//         QComboBox::drop-down {
//             border: none;
//         }

//         /* 日志区 - 深黑 + 浅绿文字 */
//         QTextEdit {
//             background: #0f0f0f;
//             color: #d0ffd0;               /* 浅绿调日志，更舒适 */
//             font-family: Consolas, monospace;
//             font-size: 13px;
//         }

//         /* 连接按钮 - 成功时变绿 */
//         QPushButton#connectBtn[connected="true"] {
//             background: #388e3c;
//             color: white;
//         }

//         /* QCustomPlot 暗黑主题 */
//         QCustomPlot {
//             background: #0e0e0e;
//         }
//     )";

//     // 为一键测试按钮设置 objectName 以匹配 QSS
//     btnOneKey->setObjectName("onekey");

//     // 为连接按钮设置 objectName（可选，后续动态改 connected 属性）
//     connectBtn->setObjectName("connectBtn");

//     qApp->setStyleSheet(style);
// }
// void MainWindow::applyGlobalStyles()
// {
//     QString style = R"(
//         QMainWindow { background:#181818; }

//         QSplitter::handle { background:#2a2a2a; width:2px; }

//         QPushButton {
//             background:#2e2e2e;
//             color:#e8e8e8;
//             border:1px solid #3c3c3c;
//             border-radius:4px;
//             padding:6px 14px;
//         }
//         QPushButton:hover { background:#3c3c3c; }

//         QPushButton#onekey {
//             background:#28a745;
//             border:none;
//             font-weight:bold;
//             font-size:15px;
//         }
//         QPushButton#onekey:hover { background:#2ecc71; }

//         QProgressBar {
//             background:#2e2e2e;
//             border:1px solid #3c3c3c;
//             border-radius:4px;
//             text-align:center;
//             color:#ccc;
//         }
//         QProgressBar::chunk { background:#28a745; border-radius:3px; }

//         QListWidget::item:selected {
//             background:#0d47a1;
//             color:white;
//         }

//         QLineEdit, QComboBox {
//             background:#222;
//             color:#ddd;
//             border:1px solid #444;
//             border-radius:4px;
//             padding:5px;
//         }

//         QTextEdit { background:#0f0f0f; color:#ccc; font-family:Consolas; }
//     )";

//     btnOneKey->setObjectName("onekey");

//     qApp->setStyleSheet(style);

//    }

void MainWindow::setupCurvePage()
{
    curvePage = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(curvePage);
    lay->setContentsMargins(20, 20, 20, 20);

    plotPlaceholder = new QLabel("曲线显示区\n(QCustomPlot 将放置在这里)");
    plotPlaceholder->setAlignment(Qt::AlignCenter);
    plotPlaceholder->setStyleSheet("font-size:18px; color:#666; margin:60px 0;");
    lay->addWidget(plotPlaceholder);

    // mainPlot = new QCustomPlot(curvePage);
    // mainPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // mainPlot->setStyleSheet("background:#0e0e0e;");
    // lay->addWidget(mainPlot, 1);

    contentStack->addWidget(curvePage);
}

void MainWindow::setupDashboardPage()
{
    pageDashboard = new QWidget();
    QVBoxLayout *dashLayout = new QVBoxLayout(pageDashboard);
    dashLayout->setContentsMargins(20, 20, 20, 20);
    dashLayout->setSpacing(15);

    // ── 标题 + 状态指示 ──
    QHBoxLayout *header = new QHBoxLayout();
    
    QLabel *titleLabel = new QLabel("仪表盘 - 当前测试监控");
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #00bfff;");
    header->addWidget(titleLabel);
    
    header->addStretch();
    
    lblStatus = new QLabel("状态：待机");
    lblStatus->setStyleSheet(
        "font-size: 16px; color: #ffcc00; "
        "background: #1e2a3a; padding: 8px 16px; "
        "border-radius: 6px;"
    );
    header->addWidget(lblStatus);
    
    dashLayout->addLayout(header);

    // ── 关键参数卡片区（网格布局） ──
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(15);
    grid->setContentsMargins(0, 10, 0, 10);

    // 分闸时间
    QWidget *card1 = createCard("分闸时间", "18.2 ms", "#00bfff");
    grid->addWidget(card1, 0, 0);
    lblTripTime = card1->findChild<QLabel*>("valueLabel");

    // 三相同期差
    QWidget *card2 = createCard("三相同期差", "45 μs", "#00ff88");
    grid->addWidget(card2, 0, 1);
    lblSyncDiff = card2->findChild<QLabel*>("valueLabel");

    // 行程 / 开距
    QWidget *card3 = createCard("行程 / 开距", "12.5 mm", "#ffaa00");
    grid->addWidget(card3, 0, 2);
    lblStroke = card3->findChild<QLabel*>("valueLabel");

    // 平均速度
    QWidget *card4 = createCard("平均速度", "1.35 m/s", "#00d4ff");
    grid->addWidget(card4, 1, 0);
    lblSpeed = card4->findChild<QLabel*>("valueLabel");

    // 可以继续加更多卡片，例如：
    // QWidget *card5 = createCard("触头弹跳次数", "0 次", "#ff6666");
    // grid->addWidget(card5, 1, 1);

    dashLayout->addLayout(grid);

    // // ── 小型曲线预览 ──
    // miniPlot = new QCustomPlot(pageDashboard);
    // miniPlot->setFixedHeight(180);
    // miniPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // miniPlot->setStyleSheet("background: #0a101c; border: 1px solid #1e2a3f; border-radius: 6px;");

    // // 设置暗黑坐标轴样式
    // miniPlot->xAxis->setBasePen(QPen(QColor("#a0b0d0")));
    // miniPlot->yAxis->setBasePen(QPen(QColor("#a0b0d0")));
    // miniPlot->xAxis->setTickPen(QPen(QColor("#a0b0d0")));
    // miniPlot->yAxis->setTickPen(QPen(QColor("#a0b0d0")));
    // miniPlot->xAxis->setSubTickPen(QPen(QColor("#2a3a50")));
    // miniPlot->yAxis->setSubTickPen(QPen(QColor("#2a3a50")));

    // dashLayout->addWidget(miniPlot);

    // 把页面加入堆叠容器
    contentStack->addWidget(pageDashboard);
}

void MainWindow::onNavigationChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current) return;

    QString txt = current->text();
    if (txt == "仪表盘") {

       qWarning() << "12345 6789a";
        contentStack->setCurrentWidget(pageDashboard);
    }
    else if (txt.contains("波形") || txt.contains("采样") || txt.contains("分析")) {
        contentStack->setCurrentWidget(curvePage);
    }
    else {
        // 其他未实现的页面，暂时显示仪表盘或曲线页作为占位
        // 你可以根据需要改成显示一个“开发中”页面
        contentStack->setCurrentWidget(pageDashboard);
        // 或者 contentStack->setCurrentWidget(curvePage);
    }
}
// void MainWindow::onNavigationChanged(QListWidgetItem *curr, QListWidgetItem *)
// {
//     if (!curr) return;
//     QString txt = curr->text();

//     // 目前所有页面都先指向曲线页（占位）
//     // 后续可根据 txt 创建/切换不同 QWidget
//     contentStack->setCurrentWidget(curvePage);
// }

void MainWindow::onConnectButtonClicked()
{
    QString proto = protocolCombo->currentText();
    QString ip = ipEdit->text().trimmed();
    QString port = portEdit->text().trimmed();

    logView->append(QString("[INFO] %1 尝试连接 %2:%3 ...").arg(proto).arg(ip).arg(port));

    // 模拟延迟
    QTimer::singleShot(1200, this, [=](){
        logView->append("[SUCCESS] 设备已连接");
        connectBtn->setText("已连接");
        connectBtn->setStyleSheet("background:#2e7d32; color:white;");
    });
}

void MainWindow::onOneKeyTestClicked()
{
    logView->append("[ACTION] 一键测试启动");

    testProgress->setValue(0);

    QTimer *sim = new QTimer(this);
    sim->setSingleShot(false);
    int pct = 0;

    connect(sim, &QTimer::timeout, this, [=]() mutable {
        pct += QRandomGenerator::global()->bounded(5, 17);
        if (pct > 100) pct = 100;
        testProgress->setValue(pct);
        logView->append(QString("[PROGRESS] %1%").arg(pct));

        if (pct >= 100) {
            sim->stop();
            sim->deleteLater();
            logView->append("[DONE] 测试完成");
        }
    });

    sim->start(400);
}


void MainWindow::createPages()
{
    // ... 原有曲线页面代码 ...

    // 新增仪表盘页面
    pageDashboard = new QWidget();
    QVBoxLayout *dashLayout = new QVBoxLayout(pageDashboard);
    dashLayout->setContentsMargins(20, 20, 20, 20);
    dashLayout->setSpacing(15);

    // 标题 + 状态
    QHBoxLayout *headerLay = new QHBoxLayout();
    QLabel *title = new QLabel("仪表盘 - 当前测试监控");
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #00bfff;");
    headerLay->addWidget(title);

    lblStatus = new QLabel("状态：待机");
    lblStatus->setStyleSheet("font-size: 16px; color: #ffcc00; background: #1e2a3a; padding: 8px; border-radius: 4px;");
    headerLay->addStretch();
    headerLay->addWidget(lblStatus);

    dashLayout->addLayout(headerLay);

    // 关键参数卡片（网格布局）
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(15);


    // 分闸时间
     QWidget *card1 = createCard("分闸时间", "18.2 ms", "#00bfff");
    grid->addWidget(card1, 0, 0);

    lblTripTime = card1->findChild<QLabel*>("valueLabel");  // 后续更新用

    // 同期差
     QWidget *card2 = createCard("三相同期差", "45 μs", "#00ff88");
    grid->addWidget(card2, 0, 1);

    lblSyncDiff = card2->findChild<QLabel*>("valueLabel");

    // 行程
    QWidget *card3 = createCard("行程 / 开距", "12.5 mm", "#ffaa00");
    grid->addWidget(card3, 0, 2);

    lblStroke = card3->findChild<QLabel*>("valueLabel");

    // 平均速度
    QWidget *card4 = createCard("平均速度", "1.35 m/s", "#00d4ff");
    grid->addWidget(card4, 1, 0);

    lblSpeed = card4->findChild<QLabel*>("valueLabel");

    dashLayout->addLayout(grid);

    // // 小型曲线预览
    // miniPlot = new QCustomPlot(pageDashboard);
    // miniPlot->setFixedHeight(180);
    // miniPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // miniPlot->setStyleSheet("background: #0a101c;");
    // dashLayout->addWidget(miniPlot);

    // // 示例曲线（占位）
    // miniPlot->addGraph();
    // miniPlot->graph(0)->setPen(QPen(QColor("#00bfff"), 2));
    // // 后续用真实数据 replot

    contentStack->addWidget(pageDashboard);

    // 定时刷新（模拟实时）
    dashboardTimer = new QTimer(this);
    connect(dashboardTimer, &QTimer::timeout, this, &MainWindow::updateDashboard);
    dashboardTimer->start(500);  // 每0.5秒刷新
}
// 原：QLabel* createCard(...)
QWidget*  MainWindow::createCard(const QString &title, const QString &value, const QString &color)
{
    QWidget *card = new QWidget();
    card->setStyleSheet(QString("background: #111822; border: 1px solid %1; border-radius: 8px; padding: 10px;").arg(color));

    QVBoxLayout *lay = new QVBoxLayout(card);
    lay->setSpacing(5);

    QLabel *titleLbl = new QLabel(title);
    titleLbl->setStyleSheet("color: #a0b0d0; font-size: 14px;");
    lay->addWidget(titleLbl);

    QLabel *valueLbl = new QLabel(value);
    valueLbl->setObjectName("valueLabel");
    valueLbl->setStyleSheet(QString("color: %1; font-size: 32px; font-weight: bold;").arg(color));
    valueLbl->setAlignment(Qt::AlignCenter);
    lay->addWidget(valueLbl);

    return card;
}

void MainWindow::updateDashboard()
{
    // 模拟数据（实际替换为从协议/测试结果获取）
    static int sim = 0;
    sim++;

    lblTripTime->setText(QString("%1 ms").arg(18.0 + sin(sim*0.1)*2, 0, 'f', 1));

    lblSyncDiff->setText(QString("%1 μs").arg(QRandomGenerator::global()->bounded(40, 70)));
  //  lblSyncDiff->setText(QString("%1 μs").arg(40 + qrand()%30));
    lblStroke->setText(QString("%1 mm").arg(12.0 + cos(sim*0.05)*0.8, 0, 'f', 1));
    lblSpeed->setText(QString("%1 m/s").arg(1.3 + sin(sim*0.2)*0.2, 0, 'f', 2));

    // 状态变化示例
    if (sim % 10 == 0) {
        lblStatus->setText("状态：测试中");
        lblStatus->setStyleSheet("font-size: 16px; color: #00ff88; background: #1e2a3a; padding: 8px; border-radius: 4px;");
    } else if (sim % 10 == 5) {
        lblStatus->setText("状态：异常 - 同期超差");
        lblStatus->setStyleSheet("font-size: 16px; color: #ff4444; background: #2a1a1a; padding: 8px; border-radius: 4px;");
    }

    // 小曲线更新示例（后续用真实采集数据）
    QVector<double> x(100), y(100);
    for (int i = 0; i < 100; ++i) {
        x[i] = i * 0.1;
        y[i] = sin(x[i] + sim*0.05) * 100 + 1200;  // 模拟电压波形
    }
    // miniPlot->graph(0)->setData(x, y);
    // miniPlot->xAxis->rescale();
    // miniPlot->yAxis->rescale();
    // miniPlot->replot();
}


// void MainWindow::onNavigationChanged(QListWidgetItem *curr, QListWidgetItem *)
// {
//     if (!curr) return;
//     QString txt = curr->text();

//     if (txt == "仪表盘") {
//         contentStack->setCurrentWidget(pageDashboard);
//     } else if (txt.contains("波形") || txt.contains("采样") || txt.contains("分析")) {
//         contentStack->setCurrentWidget(curvePage);
//     } else {
//         // 其他页面占位
//         contentStack->setCurrentWidget(pageDashboard);
//     }
// }
