#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QDockWidget>
#include <QTextEdit>

#include <QVBoxLayout>
#include <QLabel>

//#include "qcustomplot.h"   // 假设你已把 qcustomplot.h 加入项目

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNavigationChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void onConnectButtonClicked();
    void onOneKeyTestClicked();

private:
    void buildInterface();
    void applyGlobalStyles();
    void setupCurvePage();
    void setupDashboardPage();
    // 核心容器
    QWidget *central = nullptr;
    QVBoxLayout *rootLayout = nullptr;

    // 顶部区域
    QWidget *topPanel = nullptr;
    QLabel *pathLabel = nullptr;
    QComboBox *protocolCombo = nullptr;
    QLineEdit *ipEdit = nullptr;
    QLineEdit *portEdit = nullptr;
    QPushButton *connectBtn = nullptr;

    // 左侧导航
    QListWidget *sidebar = nullptr;

    // 中央内容切换
    QStackedWidget *contentStack = nullptr;

    // 示例页面：曲线区
    QWidget *curvePage = nullptr;
   // QCustomPlot *mainPlot = nullptr;
    QLabel *plotPlaceholder = nullptr;

    // 底部操作区
    QWidget *footer = nullptr;
    QPushButton *btnOneKey = nullptr;
    QPushButton *btnIOInit = nullptr;
    QPushButton *btnVoltAdj = nullptr;
    QProgressBar *testProgress = nullptr;

    // 日志区
    QDockWidget *logDock = nullptr;
    QTextEdit *logView = nullptr;

    // mainwindow.h 新增


private:
    QWidget *pageDashboard = nullptr;  // 仪表盘页面
    QLabel *lblStatus = nullptr;       // 整体状态
    QLabel *lblTripTime = nullptr;     // 分闸时间
    QLabel *lblSyncDiff = nullptr;     // 同期差
    QLabel *lblStroke = nullptr;       // 行程
    QLabel *lblSpeed = nullptr;        // 平均速度
 //   QCustomPlot *miniPlot = nullptr;   // 小型曲线预览
    QTimer *dashboardTimer = nullptr;  // 刷新定时器

    void createPages()  ;
     void updateDashboard()  ;
    // 原：QLabel* createCard(...)
    QWidget* createCard(const QString &title, const QString &value, const QString &color);

};

#endif // MAINWINDOW_H
