#include "indirectcursion.h"
#include <QDebug>

IndirectCursion::IndirectCursion()
{

}

void IndirectCursion::juge_Generate(int countSymbol, QString *startSymbol, QString candidateStr[][100])
{
    /*
     * 按开始符号（非终结符）从后往前
     * 将产生式Ui->Ujα1|Ujα2..替换为
     * Ui->β1α1|β1α2|β2α1|β2α2|...|βmα
     * 其中Uj->β1|β2|...|βm
     *
     * 例：开始符号为A、B；故U1 = B，U2 = A
     *   A->Bcd  ：U1为B,α为cd;        含有开始符号除去后为α
     *   B->Ce|f ：β1为Ce，β2为f；      Uj的候选式即为β
     *  A->Cecd|fcd
    */

    QString temp_candidateStr[100][100];
    for(int i = 0;i < countSymbol;i++){
        for(int j = 0;"\0" != candidateStr[i][j];j++){
            temp_candidateStr[i][j] = candidateStr[i][j];
        }
    }

    /* ------------------------------- 消除间接左递归 ----------------------------------------- */
    int retain[100] = {0};                  // 需要一个数组来记录哪些产生式丢弃
    int j = countSymbol-1;                  // Uj，从倒数第一个起

    for(int i = countSymbol-2;i >= 0;){     // Ui,从开始符号的倒是第二个起，数组从0开始，故减2
        QString a[100];                     // a长的像α，用来储存α
        QString b[100];                     // a存α，b存不包含Uj的候选式β
        bool have = false;                  // 判断Ui候选式的第一位是否为Uj
        int a_count = 0,b_count = 0;        // a_count对α进行计数,b_count对b进行计数

        // 循环Ui的候选式，查找是否含有Uj
        for(int m = 0;'\0' != temp_candidateStr[i][m];m++){
            // 用a保存含Uj的α
            if(temp_candidateStr[i][m].left(1) == startSymbol[j]){   // 候选式的第一位与开始符号相比
                QString temp = temp_candidateStr[i][m];              // 保留候选式，防止候选式被破坏
                a[a_count] = temp.remove(startSymbol[j]);       // 获取α
                a_count++;
                have = true;                                    // 包含Uj，have置为true
                retain[j] = 1;                                  // Ui中找到Uj,置1,丢弃Uj这个产生式
            }
            // 用b保存不含Uj的候选式
            else{
                b[b_count] = temp_candidateStr[i][m];
                b_count++;
            }
        }

        // 如果在Ui的候选式中找到Uj，就将Ui的α与Uj的候选式组合，并赋值给candidateStr
        QString candidate[100];     // 保存β+α+“不包含Uj的候选式”
        int k = 0;                  // 计数candidate

        // Ui候选式第一个字符是否为Uj
        if(have){
            // 组合β与α
            for(int p = 0;temp_candidateStr[j][p]!='\0';p++){        // Uj,β
                for(int q = 0;a[q] != '\0';q++){                     // Ui,α
                    candidate[k] = temp_candidateStr[j][p] + a[q];   // β+α
                    k++;
                }
            }

            // 加上不包含Uj的候选式
            for(int p = 0;b[p] != '\0';p++){
                candidate[k] = b[p];
                k++;
            }

            // 将组合后的候选式赋值给Ui，覆盖掉原候选式
            for(int p = 0;candidate[p] != '\0';p++){
                temp_candidateStr[i][p] = candidate[p];
            }
        }

        // 当i、j相差1时，即Uj为Ui的下一个时，Ui向上走，Ui-1，Uj重新回到最后
        if(i == j-1){
            i--;
            j = countSymbol-1;
        }
        // Uj未到Ui下一个时，Uj向上移
        else
            j--;
    }

    /* ---------------------------- 判断消除后是否为直接左递归 ----------------------- */
    for(int i = 0;i < countSymbol;i++){
        int j = 0;
        while("\0" != temp_candidateStr[i][j]){
            if(temp_candidateStr[i][j].left(1) == startSymbol[i]){    // 判断候选式的第一个字符是否为开始符号
                jugeIndirect = true;
                break;
            }
            j++;
        }
        if(jugeIndirect)
            break;
    }

    /* ----------------------------- 生成输出格式 ------------------------------------ */
    for(int i = 0;i < countSymbol;i++){
        if(1 == retain[i])                 // 跳过丢弃部分，即在Ui候选式中找到Uj的产生式
            continue;
        newGrammar += startSymbol[i];
        newGrammar += "->";

        // 输出格式
        for(int j = 0;temp_candidateStr[i][j] != '\0';j++){
            newGrammar += temp_candidateStr[i][j] +"|";
        }

        newGrammar = newGrammar.left(newGrammar.size()-1);  // 去掉最后一个"|"
        newGrammar += "\n";
    }
}

bool IndirectCursion::returnJuge()
{
    return jugeIndirect;
}

QString IndirectCursion::returnGrammar()
{
    return newGrammar;
}




