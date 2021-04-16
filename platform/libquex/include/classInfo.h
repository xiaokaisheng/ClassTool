#ifndef CLASSINFO_H
#define CLASSINFO_H

#include <QString>
#include <QVector>


class Variate 
{
public:
    enum VariateModifiers {
        CONST = 0x1,    //const
        VOLATILE = 0x2, //volatile
        RESTRICT = 0x4, //restrict
        POINT = 0x8,    //指针
        QUOTE = 0x10,    //引用
        STATIC = 0x20    //静态
    };
    QString type;
    QString name;
    int modifiers = 0x0;
    QString toString() {
        QString str;
        if (modifiers & CONST)
            str += "const ";
        if (modifiers & VOLATILE)
            str += "volatile ";
        str += type;
        if (modifiers & POINT)
            str += "*";
        if (modifiers & QUOTE)
            str += "&";
        if(!name.isEmpty())
            str += " " + name;
        return str;
    }
};

class FunctionInfo 
{
public:
    enum FunctionType {
        NORMAL = 0x0,         //普通函数
        CONSTRUCTED = 0x1,    //构造函数
        DESTRUCTOR = 0x2,     //析构函数
        PURE = 0x4,           //函数=0
        VIRTUAL = 0x8,        //虚函数
        OVERRIDE = 0x10       //重写函数
    };
    Variate returnValue;
    QString functionName;
    QList<Variate> paraValue;
    int type = 0x0;
    QString toString() {
        QString str;
        if (type & VIRTUAL)
            str += "virtal ";
        if (type & DESTRUCTOR)
            str += "~";
        if(!returnValue.toString().isEmpty())
            str += returnValue.toString() + " ";
        str += functionName + "(";
        for (int i = 0; i < paraValue.size(); i++) {
            Variate var = paraValue.at(i);
            str += var.toString();
            if (i != paraValue.size() - 1)
                str += ",";
        }
        str += ")";
        if (type & PURE)
            str += " = 0";
        else if (type & OVERRIDE)
            str += "override";
        return str;
    }
};

class ClassInfo
{
public:
	QString className;
    QList<QString> parentName;
    QList<FunctionInfo> functionInfo[3]; //public protected private   
    QList<Variate> variate[3];
    void clear();
};

#endif // CLASSINFO_H
