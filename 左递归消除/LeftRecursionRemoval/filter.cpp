#include "filter.h"
#include <QRegExp>

Filter::Filter()
{

}

// 过滤空格
char* Filter::filterSpace(char* cfg_char)
{
    int j = 0;
    for(int i = 0;'\0' != cfg_char[i];i++){
        if(' ' == cfg_char[i])     // 跳过空格
            continue;
        cfg_char[j] = cfg_char[i];
        j++;
    }
    cfg_char[j] = '\0';
    return cfg_char;
}

// 提取开始字符
QString* Filter::filterStartSymbol(char *cfg_char){
    for(int i = 0;'\0' != cfg_char[i];i++){
        QString symbol = "";

        // 将第一行单独拿出，文法开始处即为开始符号
        if(0 == i){
            // 如果开始字符的起始位置前有空行则跳过
            while('\n' == cfg_char[i]){
                if('\0' == cfg_char[i])
                    break;
                i++;
            }

            while(true){
                // 若到达结束则直接退出
                if('\0' == cfg_char[i])
                    break;

                // 即遇到'->'代表开始符号结束，就将前面获取的字符串赋值给开始符号
                if('-' == cfg_char[i] && '>' == cfg_char[i+1]){
                    startSymbol[countSymbol] = symbol;
                    countSymbol++;
                    break;
                }
                symbol += cfg_char[i];          // 有可能遇见两个字符的开始符号，即A'
                i++;
            }
        }
        // 其余行情况
        else if('\n' == cfg_char[i]){   // 换行，下一行开始即为开始符号
            i++;
            while(true){
                // 若到达结束则直接退出
                if('\0' == cfg_char[i])
                    break;

                // 即遇到'->'代表开始符号结束，就将前面获取的字符串赋值给开始符号
                if('-' == cfg_char[i] && '>' == cfg_char[i+1]){
                    startSymbol[countSymbol] = symbol;
                    countSymbol++;
                    break;
                }
                symbol += cfg_char[i];
                i++;
            }
        }
    }
    return startSymbol;
}

// 返回开始字符的个数
int Filter::returnCountSymbol()
{
    return countSymbol;
}
