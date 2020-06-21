#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filter.h"
#include "jugerecursion.h"
#include "newgrammar.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("消除左递归");
    this->setFixedSize(700,500);

    QToolBar * toolBar = new QToolBar;
    toolBar->setParent(this);
    toolBar->setGeometry(0,0,700,20);

    // 清除按钮，点击新建按钮，将输入内容清空
    QToolButton * clearBtn = new QToolButton(this);
    clearBtn->setIcon(QIcon(":/res/clearBtn.jpg"));
    clearBtn->setFixedSize(15,15);
    clearBtn->setParent(toolBar);
    clearBtn->move(10,3);
    connect(clearBtn,&QPushButton::clicked,[=](){
        ui->textEdit->clear();
    });

    // 打开文件按钮,点击选择文件，将内容输出到文法输入区域
    QToolButton * openBtn = new QToolButton(this);
    openBtn->setIcon(QIcon(":/res/openBtn.jpg"));
    openBtn->setFixedSize(15,15);
    openBtn->setParent(toolBar);
    openBtn->move(30,3);
    connect(openBtn, SIGNAL(clicked()), this, SLOT(openFile()));

    // 保存按钮
    QToolButton * storeBtn = new QToolButton(this);
    storeBtn->setIcon(QIcon(":/res/storeBtn.jpg"));
    storeBtn->setFixedSize(15,15);
    storeBtn->setParent(toolBar);
    storeBtn->move(50,3);
    connect(storeBtn, SIGNAL(clicked()), this, SLOT(storeFile()));

    // 开始按钮
    QToolButton * startBtn = new QToolButton(this);
    startBtn->setIcon(QIcon(":/res/startBtn.jpg")); // 设置图标
    startBtn->setFixedSize(15,15);                  // 设置固定大小
    startBtn->setParent(toolBar);
    startBtn->move(80,3);
    connect(startBtn,SIGNAL(clicked()),this,SLOT(start()));

    // 退出按钮
    QToolButton * exitBtn = new QToolButton (this);
    exitBtn->setIcon(QIcon(":/res/exitBtn.jpg"));
    exitBtn->setFixedSize(15,15);
    exitBtn->setParent(toolBar);
    exitBtn->move(100,3);
    connect(exitBtn,&QPushButton::clicked,[=](){
        this->close();
    });

    // 复制按钮
    QToolButton * copyBtn = new QToolButton (this);
    copyBtn->setIcon(QIcon(":/res/copyBtn.jpg"));
    copyBtn->setFixedSize(15,15);
    copyBtn->setParent(toolBar);
    copyBtn->move(350,3);
    connect(copyBtn,&QPushButton::clicked,[=](){
        ui->textEdit->setText(newGrammar);
    });

    ui->textEdit->setText("提示：\n");
    ui->textEdit->append("  ~代表空!\n");
    ui->textEdit->append("  加号按钮：清空输入区域!\n");
    ui->textEdit->append("  文件按钮：选择文法文件，将内容输入至输入区域!\n");
    ui->textEdit->append("          消除直接左递归后可能还剩间接左递归!\n");
    ui->textEdit->append("          消除间接左递归后可能还剩直接左递归!\n");
    ui->textEdit->append("  保存按钮：将消除递归后的内容保存至文件!\n");
    ui->textEdit->append("  开始按钮：开始消除左递归!\n");
    ui->textEdit->append("  退出按钮：退出程序!");
    ui->textEdit->append("  复制按钮：将输出区域内容复制至输入区域!");
}

MainWindow::~MainWindow()
{
    delete ui;
}


// 文法分析及输出结果
void MainWindow::start()
{
    ui->textBrowser->clear();              // 清除前面缓存的结果

    // 将为QString的内容转换为char数组储存
    QString cfg = ui->textEdit->toPlainText();
    QByteArray letter = cfg.toLatin1();
    char *cfg_char = letter.data();        // 字符数组储存上下文无关文法

    // 过滤空格
    Filter filter;
    cfg_char = filter.filterSpace(cfg_char);

    // 获取开始字符及其个数
    QString * startSymbol = filter.filterStartSymbol(cfg_char);
    int countSymbol = filter.returnCountSymbol();

    /* -------------------------- 获取候选式 --------------------------------------------*/
    // 将候选式用QString二维数组储存
    QString candidateStr[100][100];
    int i = 0;  // 计数cfg文法
    for(int j = 0;j < countSymbol;j++){
        int k = 0;
        while(true){
            // '>'和'|'后即为候选式
            if('>' == cfg_char[i] || '|' == cfg_char[i]){
                i++;        // 跳过'>'和'|'，进入候选式

                // 未遇到'|'、换行、结尾就将其加入候选式中
                while('|' != cfg_char[i] && '\n' != cfg_char[i] && '\0' != cfg_char[i]){
                    candidateStr[j][k] += cfg_char[i];
                    i++;
                }

                // 当碰到换行时退出while循环，进入下一个开始符号的候选式；碰见数组结尾时退出while循环
                if('\n' == cfg_char[i] || '\0' == cfg_char[i])
                    break;
                else
                    k++;    // 当前开始字符的候选式后移
            }
            else
                i++;
        }
    }

    /* -------------------------------------判断递归类型 -----------------------------*/
    JugeRecursion juge;
    QString jugeType = juge.jugeRecursion(countSymbol,startSymbol,candidateStr);

    /* ---------------------------------根据类型生成新文法-----------------------------------*/
    NewGrammar grammer;
    // 直接左递归
    if("direct" == jugeType){
        QMessageBox::warning(this,"类型","直接左递归");
        newGrammar = grammer.eliminateDirect(countSymbol,startSymbol,candidateStr);
    }

    // 间接左递归
    else if("indirect" == jugeType){
        QMessageBox::warning(this,"类型","间接左递归");
        newGrammar = grammer.eliminateIndirect(countSymbol,startSymbol,candidateStr);
    }

    // 不是左递归
    else{
        QMessageBox::warning(this,"类型","不是左递归");
        newGrammar = cfg;
    }

    /* -----------------------------------------输出-------------------------------------- */
    // 输出提示语句
    if("direct" == jugeType)
        ui->textBrowser->setText("消除直接左递归的新文法：");
    else if("indirect" == jugeType)
        ui->textBrowser->setText("消除间接左递归的新文法：");
    else if("neither" == jugeType)
        ui->textBrowser->setText("原文法：");

    ui->textBrowser->append(newGrammar);
}

// 打开文件
void MainWindow::openFile(){
    QString filePath = QFileDialog::getOpenFileName(this,"选择文件","E:\\Programing\\QT\\LeftRecursionRemoval\\LeftRecursionRemoval\\grammar\\cfg文法","(*.txt)");
    if(filePath.isEmpty()){
        return;
    }
    QFile file(filePath);
    file.open(QIODevice::ReadOnly); // 以只读方式打开
    QByteArray content = file.readAll();
    ui->textEdit->setText(content);  // 显示数据
    file.close();
}

// 储存文件
void MainWindow::storeFile()
{
    QString filePath = QFileDialog::getSaveFileName(this,"保存文件", "E:\\Programing\\QT\\LeftRecursionRemoval\\LeftRecursionRemoval\\grammar\\result","(*.txt)");
    if(filePath.isEmpty()){
        return;
    }

    QString grammar = newGrammar;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate)){ // 文件不存在则创建，覆盖写入
        QMessageBox::warning(this,"ERROR","打开文件失败,数据保存失败");
        return;
    }
    file.write(grammar.toLatin1());
    file.close();
}
