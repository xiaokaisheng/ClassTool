#include "mainwindow.h"
#include <QApplication>
#include "quextool.h"
#include "fileTool.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

QVector<HFileInfo> getHfileInfo(const QString& root_src_path, const QString& root_dest_path, const QString to_path[], int to_path_num)
{
    FileTool::removeRecursively(root_dest_path);
    QVector<HFileInfo> libmeasureHfileInfo;
    for (int i = 0; i < to_path_num; i++) {
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
                libmeasureHfileInfo.append(info);
            }
        }
    }
    return libmeasureHfileInfo;
}

QVector<ClassInfo*> getClassInfoLink(const QVector<HFileInfo>& libmeasureHfileInfo)
{
    QVector<ClassInfo*> include_class;
    for (int i = 0; i < libmeasureHfileInfo.size(); i++) {
        for (int j = 0; j < libmeasureHfileInfo.at(i).classInfos.size(); j++) {
            ClassInfo* class_info = (ClassInfo*)&libmeasureHfileInfo.at(i).classInfos[j];
            include_class.append(class_info);
        }
    }
    return include_class;
}

void setIncludeLink(QVector<ClassInfo*>& class_info_link)
{
    const QVector<QString> keywork_type = { "int", "std::string", "QString", "bool", "long", "float" };
    for (int i = 0; i < class_info_link.size(); i++) {
        ClassInfo* class_info = class_info_link.at(i);
        for (int k = 0; k < 3; k++) {
            const QList<Variate>& var_list = class_info->variate[k];
            for (int m = 0; m < var_list.size(); m++) {
                const Variate& var = var_list.at(m);
                for (QVector<ClassInfo*>::iterator it = class_info_link.begin(); it != class_info_link.end(); it++) {
                    QString class_name = (*it)->className;
                    if (var.type.contains(class_name)) {
                        int start_idx = var.type.indexOf(class_name);
                        int end_idx = start_idx + class_name.length();
                        if (start_idx > 0) {
                            if ((var.type.at(start_idx - 1) > 'a' && var.type.at(start_idx - 1) < 'z') ||
                                (var.type.at(start_idx - 1) > 'A' && var.type.at(start_idx - 1) < 'Z')) {
                                continue;
                            }
                        }
                        if (var.type.length() > end_idx) {
                            if ((var.type.at(end_idx) > 'a' && var.type.at(end_idx) < 'z') ||
                                (var.type.at(end_idx) > 'A' && var.type.at(end_idx) < 'Z')) {
                                continue;
                            }
                        }
                        class_info->includeLink.append(*it);
                        break;
                    }
                }
            }
        }
    }
}

void setParentChildLink(QVector<ClassInfo*>& class_info_link)
{
    for (int i = 0; i < class_info_link.size(); i++) {
        ClassInfo* class_info = class_info_link.at(i);
        if (class_info->parentName.size() == 0)
            continue;
        for (int j = 0; j < class_info_link.size(); j++) {
            if (class_info->parentName.contains(class_info_link.at(j)->className)) {
                class_info->parentLink.append(class_info_link.at(j));
                class_info_link.at(j)->childLink.append(class_info);
                if (class_info->parentName.size() == class_info->parentLink.size())
                    break;
            }
        }
    }
}

QString classInfo2Str(const ClassInfo* class_info) 
{
    QString str;
    str += class_info->className;
    //for (int i = 0; i < class_info->parentLink.size(); i++) {
    //    if (i == 0)
    //        str += " [";
    //    str += class_info->parentLink.at(i)->className;
    //    if(i != class_info->parentLink.size() - 1)
    //        str += ",";
    //    else
    //        str += "]";
    //}
    for (int i = 0; i < class_info->includeLink.size(); i++) {
        if (i == 0)
            str += " {";
        str += class_info->includeLink.at(i)->className;
        if (i != class_info->includeLink.size() - 1)
            str += ",";
        else
            str += "}";
    }
    return str;
}

void classInfo2StrList(QVector<ClassInfo*>& had_insert, QVector<QString>&w_list, ClassInfo* class_info, int level = 0) {
    if (had_insert.contains(class_info))
        return;
    QString str;
    for (int i = 0; i < level; i++) {
        str += "    ";
    }
    str += classInfo2Str(class_info);
    w_list.append(str);
    had_insert.append(class_info);
    for (int i = 0; i < class_info->childLink.size(); i++) {
        classInfo2StrList(had_insert, w_list, class_info->childLink.at(i), level + 1);
    }
}

int main(int argc, char *argv[])
{
    QString root_src_path = "C:/Work/TEMPWA55/platform";
    QString root_dest_path = "f:/quexTestFile";
    QString to_path[1] = { "libmeasure" };
    QVector<HFileInfo> libmeasureHfileInfo = getHfileInfo(root_src_path, root_dest_path, to_path, 1);
    QVector<ClassInfo*> class_info_link = getClassInfoLink(libmeasureHfileInfo);
    setIncludeLink(class_info_link);
    setParentChildLink(class_info_link);

    QVector<QString> w_list;
    QVector<ClassInfo*> had_insert;
    for (int i = 0; i < class_info_link.size(); i++) {
        classInfo2StrList(had_insert, w_list, class_info_link.at(i));
    }
    FileTool::mkPath("f:/quexTestFile");
    FileTool::writeTextFile("f:/quexTestFile/a_class_info.txt", w_list);

    return 0;
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //return a.exec();
}
