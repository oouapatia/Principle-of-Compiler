#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "analysis.h"
#include "file_operate.h"
#include <QDebug>
#include <QMenuBar>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("词法分析器");

    QMenuBar* menuBar = new QMenuBar(this);  // 创建一个菜单栏
    QMenu* menu = new QMenu("文件", this);    // 创建一个菜单
    menuBar->addMenu(menu);                  // 将菜单添加至菜单栏
    this->setMenuBar(menuBar);               // 将菜单栏添加到窗口

    // 创建菜单下的打开选项
    QAction* openFile = new QAction("打开 关键字|分隔符|运算符", this);
    menu->addAction(openFile);
    connect(openFile, SIGNAL(triggered()), this, SLOT(openFile()));

    // 创建菜单下的添加
    QAction* writeFile = new QAction("添加 关键字|分隔符|运算符", this);
    menu->addAction(writeFile);
    connect(writeFile, SIGNAL(triggered()), this, SLOT(addFile()));

    // 创建菜单下的删除
    QAction* deleteFile = new QAction("删除 关键字|分隔符|运算符", this);
    menu->addAction(deleteFile);
    connect(deleteFile, SIGNAL(triggered()), this, SLOT(deleteFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 开始按钮
void MainWindow::on_pushButton_clicked(){
    ui->textBrowser->clear();           // 清除前面缓存的结果
    ui->textBrowser->append("输出格式：\n(种别码 , 内部表示码) 单词 ----- 类型名称\n");    // 将结果显示在结果区域

    // 将为QString的程序转换为char数组储存
    QString program = ui->textEdit->toPlainText();
    QByteArray letter = program.toLatin1();
    char* program_char = letter.data();        // 字符数组储存程序

    // 计数程序长度
    int counter = 0;
    while(program_char[counter] != '\0'){
        counter++;
    }

    Analysis lexer;
    char word[100],number[100];   // 储存字母和数字

    QString result; // 储存显示结果
    QString code;   // 内部标识码

    bool juge_letter,juge_integer,juge_operator,juge_separator;

    for(int i = 0;i < counter;){
        result = "";

        /* ------------------------------------ 过滤'\n'和空格 ---------------------------------- */
        if(program_char[i] == '\n' || program_char[i] == ' '){  // 遇到'\n'和空格时，跳过
            i++;
            continue;
        }

        /* ------------------------------------ 判断是否为标识符 ---------------------------------- */
        juge_letter = false;
        memset(word, 0, sizeof(word));  // 将word数组清空

        // 以字母、下划线、$开头
        if(lexer.isLetter(program_char[i]) || program_char[i] == '_' || program_char[i] == '$'){
            int j = 0;
            word[j] = program_char[i];  // 将第一个字符存入
            juge_letter = true;
            while(1){
                i++;j++;
                if(lexer.isLetter(program_char[i]) || program_char[i] == '_' || program_char[i] == '$' || lexer.isInteger(program_char[i])){
                    word[j] = program_char[i];
                }
                else
                    break;
            }
        }

        /* ------------------------------------ 判断标识符是否为关键字 ---------------------------------- */
        if(juge_letter == true){
            code = lexer.isKeyword(word);
            if(code != ""){
                result = "(1 ," + code + ")    ";
                result = result + word + " -------- 关键字";
                ui->textBrowser->append(result);
            }
            else {
                result = "(2 , *)    ";
                result = result + word + " ------- 标识符";
                ui->textBrowser->append(result);   // 将结果显示在结果区域
            }
            // 这里不i++，因为前面判断字母时已加过
        }

        /* ---------------------------------- 判断是否是一个数字 ------------------------------------------- */
        juge_integer = false;
        memset(number, 0, sizeof(number));  // 将number数组清空
        if(lexer.isInteger(program_char[i])){
            int j = 0;

            // 获取以数字开始的字符串，存入number数组中
            while(1){
                if(program_char[i] != ' ' && program_char[i] != '\0' && program_char[i] != '\n' && lexer.isSeparator(program_char[i])=="" && lexer.isOperator(program_char[i]) == ""){
                    number[j] = program_char[i];
                    j++; i++;
                }
                else
                    break;
            }

            // 获得的number中有内容可能不为数字，需要判断数组中的值是否为数字
            j = 0;
            while(lexer.isInteger(number[j])){
                j++;
                if(!lexer.isInteger(number[j]) && number[j] != '\0'){ // 其中有不为数字 并且 不是数组结尾('\0')不为数字的情况，即不是数字（9main）
                    juge_integer = false;
                    break;
                }
                if(number[j] == '\0'){        // 遇到这个以数字开头的字符数组结尾时还没有碰到不是数字的情况，即为数字（999）
                    juge_integer = true;
                    break;
                }
            }

            // 为数字则输出数字
            if(juge_integer == true){
                result = "(3 , *)    ";
                result = result + number + " ------- 数字";
                ui->textBrowser->append(result);
                juge_letter = false;
            }

            // 不为数字则输出错误
            else {
                result = "(6 , *)    ";
                result = result + number + " ------- ERROR!";
                ui->textBrowser->append(result);
                juge_letter = false;
            }
        }

        /* ---------------------------------- 过滤注释------------------------------------------- */
        if('/' == program_char[i] && ('/' == program_char[i+1] || '*' == program_char[i+1]))
            i = lexer.filter(program_char,i,counter);  // 为注释，i++跳过注释部分

        /* ------------------------------------ 判断是否是运算符 ---------------------------------- */
        juge_operator = false;
        if((code =lexer.isOperator(program_char[i])) != ""){
            result = "(4 ," + code + ")    ";
            result = result + program_char[i] + " -------- 运算符";
            ui->textBrowser->append(result);
            juge_operator = true;
            i++;
        }

        /* ------------------------------------ 判断是否是分隔符 ---------------------------------- */
        juge_separator = false;
        if((code = lexer.isSeparator(program_char[i])) != ""){
            result = "(5 ," + code + ")    ";
            result = result + program_char[i] + " -------- 分隔符";
            ui->textBrowser->append(result);
            juge_separator = true;
            i++;
        }

        /* ------------------------------------ ERROR ---------------------------------- */
        if((!juge_letter && !juge_integer && !juge_operator && !juge_separator) || i+1 >= counter){
            if(program_char[i] != '\0' && program_char[i] != '\n'){
                result = "(6 , *)    ";
                result = result + program_char[i] + " ------- ERROR!";
                ui->textBrowser->append(result);
            }
            i++;
        }
    }
}

// 退出按钮
void MainWindow::on_pushButton_2_clicked()
{
    QApplication::exit();
}

// 打开文件，查看关键字、运算符、标识符
void MainWindow::openFile(){
    QString filePath = QFileDialog::getOpenFileName(this,"选择文件","E:\\Programing\\QT\\Lexer\\Lexer\\LookUp","(*.txt)");
    if(filePath.isEmpty()){
        QMessageBox::warning(this,"Failed!","文件路径为空！");
        return;
    }
    QFile file(filePath);
    file.open(QIODevice::ReadOnly); // 以只读方式打开
    QByteArray content = file.readAll();
    content = "文件内容：\n" + content;
    ui->textBrowser->setText(content);  // 显示数据
    file.close();
}

// 添加关键字、运算符、标识符
void MainWindow::addFile(){
    QDialog *dlg = new QDialog;    // 创建模态对话框
    dlg->setFixedSize(400,200);
    dlg->setWindowTitle("添加关键字|分隔符|运算符");

    /* ---------------------------------- 输入内容 -----------------------------*/
    // 文件名输入
    QLabel *input_name = new QLabel("文件名：(keywords|operators|separators).txt");    // 提示
    input_name->setParent(dlg);
    input_name->move(50,35);

    QLineEdit *name_line = new QLineEdit;
    name_line->setParent(dlg);
    name_line->move(50,50);

    // 值输入
    QLabel *input_value = new QLabel("内容");    // 提示
    input_value->setParent(dlg);
    input_value->move(50,80);

    QLineEdit *value_line = new QLineEdit;
    value_line->setParent(dlg);
    value_line->move(50,95);

    // 内部标识码输入
    QLabel *input_code = new QLabel("内部标识码");    // 提示
    input_code->setParent(dlg);
    input_code->move(200,80);

    QLineEdit *code_line = new QLineEdit;
    code_line->setParent(dlg);
    code_line->move(200,95);

    /* ---------------------------------- 确认按钮及写入文件 -----------------------------*/
    // 确认按钮
    QPushButton *ensure_btn = new QPushButton;
    ensure_btn->setParent(dlg);
    ensure_btn->setText("确认");
    ensure_btn->move(100,150);

    connect(ensure_btn, &QPushButton::clicked, [=](){
        // 获取三个LineEdit的值
        QString name = name_line->text();
        QString value = value_line->text();
        QString code = code_line->text();

        // 将内容转化为QByteArray格式
        QByteArray content = ("\n" + value + " " + code).toLatin1();

        if(name != "" && value != "" && code != ""){
            if(name == "keywords.txt" || name == "operators.txt" || name == "separators.txt"){
                QString filePath;
                filePath = "E://Programing//QT//Lexer//Lexer//LookUp//" + name;

                QFile file(filePath);
                file.open(QIODevice::Append); // 追加到末尾方式打开文件
                if(file.write(content))
                    QMessageBox::warning(dlg,"Success!","添加成功！");
                else
                    QMessageBox::warning(dlg,"Failed!","添加失败！");

                dlg->close();   // 确认后关闭
                file.close();

                // 重新打开文件，显示内容
                QFile alter_file(filePath);
                alter_file.open(QIODevice::ReadOnly);
                QByteArray alter_content = alter_file.readAll();
                alter_content = "添加后的文件内容：\n" + alter_content;
                ui->textBrowser->setText(alter_content);
                alter_file.close();
            }
            else {
                QMessageBox::warning(dlg,"Failed!","文件名输入错误！");
                return;
            }
        }
        else {
            QMessageBox::warning(dlg,"Failed!","还有值未输入！");
            return;
        }

    });

    /* ---------------------------------- 取消按钮 -----------------------------*/
    // 取消按钮
    QPushButton *cancel_btn = new QPushButton;
    cancel_btn->setParent(dlg);
    cancel_btn->setText("取消");
    cancel_btn->move(200,150);
    connect(cancel_btn, &QPushButton::clicked, [=](){
        dlg->close();
    });

    dlg->exec(); // 阻塞
}

// 删除关键字、运算符、标识符
void MainWindow::deleteFile(){
    QDialog *dlg = new QDialog;    // 创建模态对话框
    dlg->setFixedSize(400,200);
    dlg->setWindowTitle("删除关键字|分隔符|运算符");

    /* ---------------------------------- 输入内容 -----------------------------*/
    // 文件名输入
    QLabel *input_name = new QLabel("文件名：(keywords|operators|separators).txt");    // 提示
    input_name->setParent(dlg);
    input_name->move(50,35);

    QLineEdit *name_line = new QLineEdit;
    name_line->setParent(dlg);
    name_line->move(50,50);

    // 值输入
    QLabel *input_value = new QLabel("内容");    // 提示
    input_value->setParent(dlg);
    input_value->move(50,80);

    QLineEdit *value_line = new QLineEdit;
    value_line->setParent(dlg);
    value_line->move(50,95);

    /* ---------------------------------- 删除按钮及写入文件 -----------------------------*/
    // 删除按钮
    QPushButton *delete_btn = new QPushButton;
    delete_btn->setParent(dlg);
    delete_btn->setText("删除");
    delete_btn->move(100,150);

    connect(delete_btn, &QPushButton::clicked, [=](){
        // 获取三个LineEdit的值
        QString name = name_line->text();
        QString value = value_line->text();

        if(name != "" && value != ""){
            if(name == "keywords.txt" || name == "operators.txt" || name == "separators.txt"){
                QString filePath;
                filePath = "E://Programing//QT//Lexer//Lexer//LookUp//" + name;

                QFile file(filePath);
                file.open(QIODevice::ReadOnly);

                char *file_content = file.readAll().data();
                bool juge = false; // 判断是否在文件中找到
                file.close();

                // 计算长度
                int counter = 0;
                while(file_content[counter] != '\0'){
                    counter++;
                }

                // 在文件中查找对应内容
                int i,word_length = 0;
                for(i = 0;i < counter;i++){
                    word_length = 0;
                    QString temp_word = "";

                    // 获取文件中的空格前的内容
                    // 当当前字符不为空格且不为换行时，将其存入temp_word数组中，计数单词长度的word_length加1；
                    while(file_content[i] != ' ' && file_content[i] != '\n'){
                        temp_word = temp_word + file_content[i];
                        i++;
                        word_length++;
                    }

                    if(temp_word == value || file_content[i] == '\0'){
                        if(temp_word == value)
                            juge = true;
                        break;
                    }
                }

                if(juge == true){
                    // 计算code的长度
                    File_operate operate;
                    QString code = operate.lookUp(filePath,value);
                    char* code_length = code.toLatin1().data();
                    int len;
                    for(len = 0;'\0' != code_length[len];len++){/* 啥也不干 */}
                    len--;  // 去掉多余的一个

                    QString content = "";
                    for(int j = 0;j < counter;j++){
                        // 跳过整行，i在空格处，i-word_length回到行首，i+len+1：跳过内部表示码和换行 ；例：int 1234\n\r,\r不一定有，所以单独拿出
                        if((j >= (i - word_length) && j <= i+len+1) || file_content[j] == '\r')
                            continue;
                         content += file_content[j];
                    }

                    file.open(QIODevice::ReadWrite|QIODevice::Truncate);   // 以重写的方式打开文件
                    if(file.write(content.toLatin1()))     // 转化为QByteArray格式写入
                        QMessageBox::warning(dlg,"Success!","删除成功！");
                    else
                        QMessageBox::warning(dlg,"Failed!","删除失败！");

                    dlg->close();
                    file.close();

                    // 重新打开文件，显示内容
                    QFile alter_file(filePath);
                    alter_file.open(QIODevice::ReadOnly);
                    QByteArray alter_content = alter_file.readAll();
                    alter_content = "删除后的文件内容：\n" + alter_content;
                    ui->textBrowser->setText(alter_content);
                    alter_file.close();
                }
                else {
                    QMessageBox::warning(dlg,"Failed!","文件中没有这个值！");
                    return;
                }
            }
            else {
                QMessageBox::warning(dlg,"Failed!","文件名输入错误！");
                return;
            }
        }
        else {
            QMessageBox::warning(dlg,"Failed!","还有值未输入！");
            return;
        }
    });

    /* ---------------------------------- 取消按钮 -----------------------------*/
    // 取消按钮
    QPushButton *cancel_btn = new QPushButton;
    cancel_btn->setParent(dlg);
    cancel_btn->setText("取消");
    cancel_btn->move(200,150);
    connect(cancel_btn, &QPushButton::clicked, [=](){
        dlg->close();
    });

    dlg->exec(); // 阻塞
}
