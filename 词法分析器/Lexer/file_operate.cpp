#include "file_operate.h"
#include <qfile.h>
#include <QDebug>

File_operate::File_operate()
{
}

QString File_operate::lookUp(QString filePath,QString word){

    QFile file_temp(filePath);
    file_temp.open(QIODevice::ReadOnly); // 以只读方式打开
    QByteArray content = file_temp.readAll();
    char *file = content.data();
    file_temp.close();

    QString temp_word,code; //temp_word为从文件中提取的单词，code为内部表示码

    // 遍历文件，查找关键字
    for(int i = 0;;i++){
        temp_word = "";
        code = "";
        if(file[i] == '\0')
            break;

        // 获取文件中的空格前的内容
        while(file[i] != ' ' && file[i] != '\n'){
            temp_word = temp_word + file[i];
            i++;
        }

        // 获取内部表示码
        if(file[i] == ' '){
            while(file[i] != '\n' && file[i] != '\0'){
                code = code + file[i];
                i++;
            }
        }

        // 退出遍历
        if(temp_word == word || file[i] == '\0'){
            if(temp_word == word)
                break;
            else {     // 到结束未找到，重置code
                code = "";
                break;
            }
        }
    }
    return code;
}
