#ifndef INDIRECTCURSION_H
#define INDIRECTCURSION_H
#include <QString>

class IndirectCursion
{
public:
    IndirectCursion();

    bool jugeIndirect = false;
    QString newGrammar;

    void juge_Generate(int countSymbol,QString* startSymbol,QString candidateStr[][100]);  // 判断是否为间接左递归并生成新的文法
    bool returnJuge();            // 返回判断
    QString returnGrammar();      // 返回新文法
};

#endif // INDIRECTCURSION_H
