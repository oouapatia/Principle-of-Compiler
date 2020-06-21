#ifndef FILTER_H
#define FILTER_H

#include <QString>

class Filter
{
public:
    Filter();

    int countSymbol = 0;                            // 计数开始字符个数
    QString startSymbol[100];                       // 储存开始字符,出现A'情况

    char* filterSpace(char* cfg_char);               // 过滤空格
    QString * filterStartSymbol(char* cfg_char);     // 获取开始字符
    int returnCountSymbol();                        // 返回countSymbol
};

#endif // FILTER_H
