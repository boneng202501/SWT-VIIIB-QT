#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QStatusBar>
#include <QMessageBox>
#include <QDockWidget>
#include <QTime>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    updateStatusBar();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("高压开关机械特性测试系统");
    resize(1280, 800);

    // 中心部件
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ====================== 左侧导航栏 ======================
    navPanel = new QWidget;
    navPanel->setObjectName("navPanel");
    navPanel->setFixedWidth(240);
    navPanel->setStyleSheet("background-color: #2c3e50; color: white;");

    QVBoxLayout *navLayout = new QVBoxLayout(navPanel);
    navLayout->setContentsMargins(0, 0, 0, 0);

    // 折叠按钮
    btnToggleNav = new QToolButton;
    btnToggleNav->setText("<<");
    btnToggleNav->setStyleSheet("background-color: #34495e; color: white; font-size: 16px;");
    connect(btnToggleNav, &QToolButton::clicked, this, &MainWindow::on_btnToggleNav_clicked);
    navLayout->addWidget(btnToggleNav);

    // 菜单列表
    listNav = new QListWidget;
    listNav->setStyleSheet("QListWidget { background: transparent; border: none; color: white; }"
                           "QListWidget::item { padding: 14px; font-size: 15px; }"
                           "QListWidget::item:selected { background: #34495e; }");
    listNav->addItems({"仪表盘", "设备连接", "电压调节", "采样控制", "波形分析", "历史记录", "报表导出"});
    connect(listNav, &QListWidget::currentRowChanged, this, &MainWindow::onNavItemClicked);
    navLayout->addWidget(listNav);

    mainLayout->addWidget(navPanel);

    // ====================== 右侧主区域 ======================
    QWidget *rightPanel = new QWidget;
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(10, 10, 10, 10);

    // 顶部工具栏
    QWidget *topBar = new QWidget;
    topBar->setMaximumHeight(60);
    QHBoxLayout *topLayout = new QHBoxLayout(topBar);

    QLabel *breadcrumb = new QLabel("Home > 测试管理");
    breadcrumb->setStyleSheet("font-size: 15px; color: #333;");

    comboProtocol = new QComboBox;
    comboProtocol->addItems({"UDP", "TCP"});

    leIP = new QLineEdit("127.0.0.1");
    lePort = new QLineEdit("5000");

    btnConnect = new QPushButton("连接设备");
    connect(btnConnect, &QPushButton::clicked, this, &MainWindow::on_btnConnect_clicked);

    topLayout->addWidget(breadcrumb);
    topLayout->addStretch();
    topLayout->addWidget(new QLabel("协议:"));
    topLayout->addWidget(comboProtocol);
    topLayout->addWidget(new QLabel("IP:"));
    topLayout->addWidget(leIP);
    topLayout->addWidget(new QLabel("端口:"));
    topLayout->addWidget(lePort);
    topLayout->addWidget(btnConnect);

    rightLayout->addWidget(topBar);

    // 曲线占位区（占满剩余空间）
    plotContainer = new QWidget;
    plotContainer->setStyleSheet("background-color: #f8f9fa; border: 1px solid #ddd;");
    QVBoxLayout *plotLayout = new QVBoxLayout(plotContainer);
    QLabel *placeholder = new QLabel("曲线显示区\n（后续放置 QCustomPlot）", plotContainer);
    placeholder->setAlignment(Qt::AlignCenter);
    placeholder->setStyleSheet("font-size: 28px; color: #888;");
    plotLayout->addWidget(placeholder);
    rightLayout->addWidget(plotContainer, 1);  // 拉伸填充

    // 底部操作栏
    QWidget *bottomBar = new QWidget;
    bottomBar->setMaximumHeight(55);
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomBar);

    btnOneClickTest = new QPushButton("一键测试");
    //btnOneClickTest->setStyleSheet("background-color: #27ae60; color: white; font-size: 18px; padding: 12px;");

    btnOneClickTest->setStyleSheet(
        "QPushButton {"
        "    background-color: #27ae60;"           // 你的绿色
        "    color: white;"
        "    font: bold 18px 'Microsoft YaHei', 'SimSun';"  // 指定中文字体 + 备选
   //     "    min-height: 60px;"                    // 实际想要的高度，比如64/70
        "    padding: 0 24px;"                     // 上下 padding 改成 0，让下面属性控制垂直
        "    line-height: 40px;"                   // 关键：设成按钮高度（或比 min-height 小2px）
        "    text-align: center;"
        "    border: none;"
        "    border-radius: 10px;"                 // 你的圆角看起来挺圆的，保持
        "}"
        "QPushButton:hover { background-color: #2ecc71; }"
        "QPushButton:pressed { background-color: #1e8449; }"
        );


    connect(btnOneClickTest, &QPushButton::clicked, this, &MainWindow::on_btnOneClickTest_clicked);

    btnIOInit = new QPushButton("IO 初始化");
    btnVoltageReg = new QPushButton("电压调节");
    connect(btnIOInit, &QPushButton::clicked, this, &MainWindow::on_btnIOInit_clicked);
    connect(btnVoltageReg, &QPushButton::clicked, this, &MainWindow::on_btnVoltageReg_clicked);

    progressBar = new QProgressBar;
    progressBar->setValue(0);

    bottomLayout->addWidget(btnOneClickTest);
    bottomLayout->addWidget(btnIOInit);
    bottomLayout->addWidget(btnVoltageReg);
    bottomLayout->addWidget(progressBar);

    rightLayout->addWidget(bottomBar);

    mainLayout->addWidget(rightPanel);

    // 日志区（DockWidget）
    QDockWidget *dockLog = new QDockWidget("日志", this);
    plainLog = new QPlainTextEdit;
    plainLog->setReadOnly(true);
    dockLog->setWidget(plainLog);
    addDockWidget(Qt::BottomDockWidgetArea, dockLog);

    // 状态栏
    statusBar()->showMessage("未连接 | 协议：UDP");
}

void MainWindow::on_btnToggleNav_clicked()
{
    navCollapsed = !navCollapsed;


    if (navPanel && listNav && btnToggleNav) {
        if (navCollapsed) {
            listNav->setVisible(false);            // 隐藏菜单列表
            navPanel->setFixedWidth(50);           // 只留按钮宽度
            btnToggleNav->setText(">>");
        } else {
            listNav->setVisible(true);
            navPanel->setFixedWidth(240);
            btnToggleNav->setText("<<");
        }

        centralWidget()->layout()->invalidate();
        centralWidget()->layout()->activate();
        centralWidget()->update();
    }
}

void MainWindow::onNavItemClicked(int index)
{
    qDebug() << "导航菜单点击：" << index;
    // 后续根据 index 切换页面
}

void MainWindow::updateStatusBar()
{
    QString protocol = comboProtocol ? comboProtocol->currentText() : "未知";
    statusBar()->showMessage(QString("未连接 | 协议：%1").arg(protocol));
}

void MainWindow::on_btnConnect_clicked()
{
    if (m_comm && m_comm->isOpen()) {
        m_comm->close();
        delete m_comm;
        m_comm = nullptr;
        btnConnect->setText("连接设备");
        updateStatusBar();
        return;
    }

    QString protocol = comboProtocol->currentText().toUpper();
    QString ip = leIP->text().trimmed();
    quint16 port = lePort->text().toUShort();

    if (ip.isEmpty() || port == 0) {
        QMessageBox::warning(this, "参数错误", "请填写有效的 IP 和端口");
        return;
    }

    // 使用接口的静态工厂方法创建
    m_comm = ICommunicator::create(protocol, this);
    if (!m_comm) {
        QMessageBox::warning(this, "协议错误", "不支持的协议类型: " + protocol);
        return;
    }

    QVariantMap params;
    params["ip"] = ip;
    params["port"] = port;

    if (!m_comm->open(params)) {
        QString err = m_comm->lastError();
        plainLog->appendPlainText("连接失败：" + err);
        delete m_comm;
        m_comm = nullptr;
        return;
    }

    m_currentProtocol = protocol;
    btnConnect->setText("断开连接");
    updateStatusBar();
    plainLog->appendPlainText("已连接到 " + protocol + " " + ip + ":" + QString::number(port));

    // 连接其他信号...
}
void MainWindow::on_btnIOInit_clicked()
{
    if (!m_comm || !m_comm->isOpen()) {
        QMessageBox::warning(this, "未连接", "请先连接设备");
        plainLog->appendPlainText("[" + QTime::currentTime().toString("hh:mm:ss") + "] IO 初始化失败：未连接设备");
        return;
    }

    // 禁用按钮，防止重复点击
    btnIOInit->setEnabled(false);
    btnIOInit->setText("初始化中...");
    progressBar->setValue(0);
    progressBar->setRange(0, 100);

    plainLog->appendPlainText("[" + QTime::currentTime().toString("hh:mm:ss") + "] 开始 IO 初始化...");

    // 构造 IO 初始化命令（示例：4字节命令）
    QByteArray initCmd(4, 0x00);
    initCmd[0] = 0xA0;     // 命令头
    initCmd[1] = 0x01;     // 子命令：初始化
    initCmd[2] = 0x00;     // 保留
    initCmd[3] = 0x00;     // 保留

    // 发送并等待响应（同步方式，超时 2000ms）
    QByteArray response = m_comm->sendAndReceive(initCmd, 2000, 2);  // 超时2秒，重试2次

    if (response.isEmpty()) {
        QString err = m_comm->lastError();
        plainLog->appendPlainText("[" + QTime::currentTime().toString("hh:mm:ss") + "] IO 初始化失败：" + err);
        statusBar()->showMessage("IO 初始化失败：" + err);
        QMessageBox::critical(this, "失败", "IO 初始化无响应或超时");
    } else {
        // 解析响应（示例：假设成功响应是 0xA0 0x01 0x00 0x00）
        if (response.size() >= 4 && int(response[0]) == 0xA0 && response[1] == 0x01) {
            plainLog->appendPlainText("[" + QTime::currentTime().toString("hh:mm:ss") + "] IO 初始化成功");
            statusBar()->showMessage("IO 初始化成功");
            progressBar->setValue(100);

            // 可选：自动刷新某些状态
            // m_comm->sendAsync(某个查询命令);
        } else {
            plainLog->appendPlainText("[" + QTime::currentTime().toString("hh:mm:ss") + "] IO 初始化响应异常：" + response.toHex(' ').toUpper());
            QMessageBox::warning(this, "警告", "IO 初始化响应异常");
            progressBar->setValue(0);
        }
    }

    // 恢复按钮状态
    btnIOInit->setEnabled(true);
    btnIOInit->setText("IO 初始化");
}
// 其他槽函数（占位）
//void MainWindow::on_btnConnect_clicked()     { QMessageBox::information(this, "提示", "连接功能待实现"); }
void MainWindow::on_btnOneClickTest_clicked(){ QMessageBox::information(this, "提示", "一键测试功能待实现"); }
//void MainWindow::on_btnIOInit_clicked()      { QMessageBox::information(this, "提示", "IO初始化功能待实现"); }
void MainWindow::on_btnVoltageReg_clicked()  { QMessageBox::information(this, "提示", "电压调节功能待实现"); }
