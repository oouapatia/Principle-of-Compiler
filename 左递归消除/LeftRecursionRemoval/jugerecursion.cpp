#include "jugerecursion.h"
#include "indirectcursion.h"

JugeRecursion::JugeRecursion()
{

}

QString JugeRecursion::jugeRecursion(int countSymbol,QString* startSymbol,QString candidateStr[][100])
{
   /* -------------------------- 判断直接左递归 ------------------------*/
   // 当候选式的第一个字符为对应行的开始字符时，即为直接左递归
   for(int i = 0;i < countSymbol;i++){
       int j = 0;
       while("\0" != candidateStr[i][j]){
           if(candidateStr[i][j].left(1) == startSymbol[i])    // 判断候选式的第一个字符是否为开始符号
               return "direct";
           j++;
       }
   }

   /* ---------------------------- 判断间接左递归 -------------------------*/
   IndirectCursion indirect;
   indirect.juge_Generate(countSymbol,startSymbol,candidateStr);
   bool jugeIndirect = indirect.returnJuge();
   if(true == jugeIndirect)
       return "indirect";

    return "neither";
}
