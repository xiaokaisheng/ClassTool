#include "hFileInfo.h"

HFileInfo::HFileInfo()
{
}

HFileInfo::~HFileInfo()
{
}

QVector<QString> HFileInfo::toStringVector()
{
    QVector<QString> w_info;
    w_info.append("#pragma once");
    w_info.append(include_local_file);
    w_info.append(include_standard_file);
    for (int i = 0; i < classInfos.size(); i++)
    {
        w_info.append(" ");
        QString str_class = "class " + classInfos.at(i).className;
        for (int j = 0; j < classInfos.at(i).parentName.size(); j++) {
            if (j == 0)
                str_class += " : public ";
            else
                str_class += ", ";
            str_class += classInfos.at(i).parentName.at(j);
        }
        w_info.append(str_class);
        w_info.append("{");
        for (int m = 0; m < 3; m++) {
            for (int j = 0; j < classInfos[i].functionInfo[m].size(); j++) {
                if (j == 0) {
                    if (m == 0) {
                        w_info.append("public:");
                    }
                    else if (m == 1) {
                        w_info.append("protected:");
                    }
                    else {
                        w_info.append("private:");
                    }
                }
                FunctionInfo function_info = classInfos[i].functionInfo[m].at(j);
                w_info.append("    " + function_info.toString() + ";");
            }
        }
        for (int m = 0; m < 3; m++) {
            for (int j = 0; j < classInfos[i].variate[m].size(); j++) {
                if (j == 0) {
                    if (m == 0) {
                        w_info.append("public:");
                    }
                    else if (m == 1) {
                        w_info.append("protected:");
                    }
                    else {
                        w_info.append("private:");
                    }
                }
                Variate var = classInfos[i].variate[m].at(j);
                w_info.append("    " + var.toString() + ";");
            }
        }
        w_info.append("};");
    }
    return w_info;
}