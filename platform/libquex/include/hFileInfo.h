#ifndef HFILEINFO_H
#define HFILEINFO_H
#include <QString>
#include <QVector>
#include "classInfo.h"

class HFileInfo
{
public:
    HFileInfo();
    ~HFileInfo();
	
	QString hFileName;
    QString hFilePath;
	//QString cppFileName;
	//QString cppFilePath;
	QVector<ClassInfo> classInfos;
    QVector<QString> include_local_file;
    QVector<QString> include_standard_file;
	//QString libBelong;

    QVector<QString> toStringVector();
};

#endif // HFILEINFO_H
