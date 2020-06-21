#ifndef JUGERECURSION_H
#define JUGERECURSION_H

#include <QString>
#include <QDebug>

class JugeRecursion
{
public:
    JugeRecursion();

    QString jugeRecursion(int countSymbol,QString* startSymbol,QString candidateStr[][100]);   // 判断递归类型
};

#endif // JUGERECURSION_H
