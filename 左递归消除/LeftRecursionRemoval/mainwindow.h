#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString newGrammar = "";         // 储存新文法

public slots:
    void start();                    // 判断递归类型
    void openFile();                 // 打开文件
    void storeFile();                // 保存文件

public:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
