#include "newgrammar.h"
#include "jugerecursion.h"
#include "indirectcursion.h"

NewGrammar::NewGrammar()
{

}

// 用~代替ε

// 消除直接左递归
QString NewGrammar::eliminateDirect(int countSymbol,QString* startSymbol,QString candidateStr[][100])
{
    /*
     * 直接改写法
     * U->Ux|y
     * U->yU' ------------  first
     * U'->xU'|~ ---------  second

     * 1、先遍历判断候选式是否含有开始符号
     * 2、若有，则这个产生式为直接左递归，将输出结果分为两行,第一行存储包含开始符号的结果，第二行存储不包含开始符号的内容
     * 3、若没有，则这个产生式不为直接左递归，直接输出
    */

    QString newGrammar = "";
    bool jugeSymbol;    // 判断候选式是否有开始符号

    for(int i = 0;i < countSymbol;i++){

        // 判断此产生式的候选式的第一个字符是否为开始字符
        jugeSymbol = false;
        for(int j = 0;'\0' != candidateStr[i][j];j++){
            if(candidateStr[i][j].left(1) == startSymbol[i]){
                jugeSymbol = true;
                break;
            }
        }

        // 对输出进行格式处理
        QString symbol = "";                // 开始字符
        symbol += startSymbol[i];           // 开始字符转化为QString储存
        QString first = symbol + "->";      // 不包含开始字符的输出
        QString second = symbol + "'->";    // 包含开始字符的输出

        // 对候选式进行格式处理
        for(int j = 0;'\0' != candidateStr[i][j];j++){
            // 有开始字符,则将结果分为两行    U->Ux|y
            if(true == jugeSymbol){
                if(candidateStr[i][j].left(1) != startSymbol[i]){     // 候选式的第一个字符不为开始字符
                    first += candidateStr[i][j]+startSymbol[i]+"'|";  // y+U+'|
                } else {
                    QString temp = candidateStr[i][j].remove(startSymbol[i]);   // 去掉开始字符
                    second += temp+startSymbol[i]+"'|";                         // x+U+'|
                }
            } else {   // 没有开始字符，直接输出，第二行置空   U->x|y
                first += candidateStr[i][j] + "|";
                second = "";
            }
        }

        // 去掉first最后的“|”
        first = first.left(first.size()-1);

        // 对结果进行格式处理，有开始符号则结果有两行，第二行加上~
        if(true == jugeSymbol){
            first += "\n";
            second += "~";
        }

        newGrammar += first + second + "\n";
//        qDebug() << "newGrammar:" << newGrammar;
    }
    return newGrammar;
}

// 消除间接左递归
QString NewGrammar::eliminateIndirect(int countSymbol,QString* startSymbol,QString candidateStr[][100]){
    QString newGrammar="";
    IndirectCursion indirect;
    indirect.juge_Generate(countSymbol,startSymbol,candidateStr);
    newGrammar = indirect.returnGrammar();
    return newGrammar;
}
