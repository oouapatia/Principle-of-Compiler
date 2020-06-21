#ifndef NEWGRAMMAR_H
#define NEWGRAMMAR_H

#include <QString>
#include <QtDebug>

class NewGrammar
{
public:
    NewGrammar();

    QString eliminateDirect(int countSymbol,QString* startSymbol,QString candidateStr[][100]);     // 消除直接左递归
    QString eliminateIndirect(int countSymbol,QString* startSymbol,QString candidateStr[][100]);   // 消除间接左递归
};

#endif // NEWGRAMMAR_H
