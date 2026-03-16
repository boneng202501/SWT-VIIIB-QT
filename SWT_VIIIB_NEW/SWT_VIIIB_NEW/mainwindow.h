#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QToolButton>
#include <QWidget>
#include "ICommunicator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    // 通信对象指针（接口指针）
    ICommunicator *m_comm = nullptr;
   QString m_currentProtocol = "UDP";


private slots:
    void on_btnConnect_clicked();
    void on_btnOneClickTest_clicked();
    void on_btnIOInit_clicked();
    void on_btnVoltageReg_clicked();
    void on_btnToggleNav_clicked();
    void onNavItemClicked(int index);

private:
    void setupUI();           // 创建所有控件和布局
    void updateStatusBar();   // 更新状态栏信息

    // 成员变量 - 所有关键控件
    QWidget       *navPanel = nullptr;
    QToolButton   *btnToggleNav = nullptr;
    QListWidget   *listNav = nullptr;
    bool           navCollapsed = false;

    QWidget       *plotContainer = nullptr;  // 曲线占位区

    QPlainTextEdit *plainLog = nullptr;
    QProgressBar   *progressBar = nullptr;

    QComboBox      *comboProtocol = nullptr;
    QLineEdit      *leIP = nullptr;
    QLineEdit      *lePort = nullptr;
    QPushButton    *btnConnect = nullptr;

    QPushButton    *btnOneClickTest = nullptr;
    QPushButton    *btnIOInit = nullptr;
    QPushButton    *btnVoltageReg = nullptr;
};

#endif // MAINWINDOW_H
