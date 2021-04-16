#include "mainwindow.h"
#include <QApplication>
#include "quextool.h"
#include "fileTool.h"

#include <QFile>
#include <QTextStream>

void writeTextFile(QString filename, QVector<QString> str)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    for (int i = 0; i < str.size(); i++)
    {
        file.write(str.at(i).toLocal8Bit());
        file.write("\n");
    }
    file.close();
}

int main(int argc, char *argv[])
{
    QString root_src_path = "C:/Work/TEMPWA55";
    QString root_dest_path = "f:/quexTestFile";
    QString to_path[1] = {"src"};
    FileTool::removeRecursively(root_dest_path);
    for (int i = 0; i < 1; i++) {
        QStringList src_file_list;
        QString src_path = root_src_path + "/" + to_path[i];
        QString dest_path = root_dest_path + "/" + to_path[i];
        FileTool::getDirAllFile(src_path, src_file_list);
        QStringList dest_file_list;
        for (QString str : src_file_list) {
            dest_file_list.append(str.replace(src_path, dest_path));
        }

        QuexTool* tool = new QuexTool;
        for (int i = 0; i < src_file_list.size(); i++) {
            if (src_file_list.at(i).endsWith(".h")) {
                HFileInfo info = tool->quexHFile(src_file_list.at(i).toStdString());
                FileTool::mkPath(FileTool::getParentPath(dest_file_list.at(i)));
                writeTextFile(dest_file_list.at(i), info.toStringVector());
            }
        }
    }

    return 0;
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //return a.exec();
}
