#include "analysis.h"
#include "file_operate.h"
#include <QString>
#include <QDebug>

Analysis::Analysis(){}

/* ------------------------------------- 过滤注释 -----------------------------------*/
int Analysis::filter(char* program_char,int i,int counter){
    if('/' == program_char[i+1]){          // 第二个字符为‘/’-----单行注释
        for(;i < counter;i++){
            if('\n' == program_char[i])    // 单行注释遇到‘\n’结束
                break;
        }
    }
    if('*' == (program_char[i+1])){ // 多行注释
        i = i+2;                    // 跳过‘/*’
        for(;i < counter;i++){      // 直到遇到‘*/’时结束
            if('*' == (program_char[i]) && '/' == (program_char[i+1])){
                i = i+2;            // 跳过‘*/’
                break;
            }
        }
    }
    return i;;
}

/* ------------------------------------- 判断是否为字母 -----------------------------------*/
bool Analysis::isLetter(char letter){
    if((letter <= 'Z' && letter >= 'A') || (letter <= 'z' && letter >= 'a'))
        return true;
    else
        return false;
}

/* ------------------------------------- 判断是否为关键字 -----------------------------------*/
QString Analysis::isKeyword(char* word){

    // 将字符数组变为QString
    QString juge_keyword;   // 程序中的字母
    for(int i = 0;;i++){
        if(word[i] == '\0')
            break;
        juge_keyword = juge_keyword + word[i];
    }

    File_operate file;
    QString filePath = "E://Programing//QT//Lexer//Lexer//LookUp//keywords.txt";
    return file.lookUp(filePath,juge_keyword);
}

/* ------------------------------------- 判断是否为运算符 -----------------------------------*/
QString Analysis::isOperator(char count){

    // 将字符变为QString
    QString juge_operator;
    juge_operator = juge_operator + count;

    File_operate file;
    QString filePath = "E://Programing//QT//Lexer//Lexer//LookUp//operators.txt";
    return file.lookUp(filePath,juge_operator);
}

/* ------------------------------------- 判断是否为分隔符 -----------------------------------*/
QString Analysis::isSeparator(char div){

    // 将字符变为QString
    QString juge_separator;
    juge_separator = juge_separator + div;

    File_operate file;
    QString filePath = "E://Programing//QT//Lexer//Lexer//LookUp//separators.txt";
    return file.lookUp(filePath,juge_separator);
}

/* ------------------------------------- 判断是否为数字 -----------------------------------*/
bool Analysis::isInteger(int number){
    if(number >= '0' && number <= '9')
        return true;
    else
        return false;
}

