#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QString>

class Analysis
{
public:
    Analysis();
    int filter(char* temp_word,int i,int counter);  // 过滤注释
    bool isLetter(char letter);           // 判断是否为字母
    QString isKeyword(char* word);        // 判断是否为关键字
    bool isInteger(int number);           // 判断是否为数字
    QString isSeparator(char div);        // 判断是否为分隔符
    QString isOperator(char count);       // 判断是否为运算符
};

#endif // ANALYSIS_H
