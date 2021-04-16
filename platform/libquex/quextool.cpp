#include "quextool.h"
#include <iostream> 
#include <QList>
#include <QFileInfo>
#include "lexical_analysis/LexerForC"
#include "quexNode.h"
#include "QuexNodeFile/hFileQuexNode.h"

QuexTool::QuexTool()
{
    initHFileNodeRoot();
    baseType = { "bool", "char", "int", "long", "QString", "std::string"};
}

QuexTool::~QuexTool()
{
    delete hFileQuexNode;
}

void QuexTool::initHFileNodeRoot()
{
    hFileQuexNode = new HFileQuexNode();
}

HFileInfo QuexTool::quexHFile(std::string file_path)
{
    HFileInfo   h_file_info;
    QFileInfo file_info(QString::fromStdString(file_path));
    if (file_info.exists()) {
        h_file_info.hFileName = file_info.fileName();
        h_file_info.hFilePath = file_info.absoluteFilePath();
    }
    else {
        return HFileInfo();
    }
    LexerForC qlex(file_path.c_str());
    LexerForC_Token*    token_p = 0;
    int             number_of_tokens = 0;
    hFileCurrentNode = hFileQuexNode->getRootNode();
    do {
        qlex.receive(&token_p);
        if( ! token_p ) break;
    
        if (hFileCurrentNode->validIds.contains(token_p->id)) {
            analyzeHfileQuexNode(hFileCurrentNode->getId() + "&" + token_p->id_name(), token_p, h_file_info);
            if (hFileCurrentNode->next.keys().contains(token_p->id)) {
                hFileCurrentNode = hFileCurrentNode->next.value(token_p->id);
            }
        }
    
        ++number_of_tokens;
    
    } while( token_p->id != QUEX_TKN_TERMINATION );

    std::cout << "[END] number of tokens = " << number_of_tokens << std::endl;
    if( qlex.error_code != E_Error_None ) qlex.print_this(); 
    return h_file_info;
}

Variate QuexTool::analyzeVariate(QList<QString> class_member_data)
{
    Variate variate_info;
    if (class_member_data.contains("=") && class_member_data.contains("(") && class_member_data.contains(")") && class_member_data.indexOf("=") < class_member_data.indexOf("(")) {
        while (class_member_data.last() != "=") {
            class_member_data.takeLast();
        }
        class_member_data.takeLast();
    }
    if (!class_member_data.contains("(") && !class_member_data.contains(")")) {
        variate_info.name = class_member_data.takeLast();
        QString key[5] = {"*", "&", "const", "volatile", "restrict"};
        Variate::VariateModifiers value[5] = { Variate::POINT, Variate::QUOTE, Variate::CONST, Variate::VOLATILE, Variate::RESTRICT };
        for (int i = 0; i < 5; i++) {
            if (class_member_data.contains(key[i])) {
                variate_info.modifiers = value[i] | variate_info.modifiers;
            }
        }
        return variate_info;
    }
    return variate_info;
}

/*
-1：解析错误
1： 变量
2： 函数
*/
int QuexTool::analyzeClassMember(QList<QString> class_member_data, Variate& variate_info, FunctionInfo& function_info)
{
    QString shield_first[2] = { "Q_OBJECT", "slots" };
    for (int i = 0; i < 2; i++) {
        if (class_member_data.startsWith(shield_first[i])) {
            class_member_data.takeFirst();
        }
    }
    if (class_member_data.contains("operator")) {
        int oprator_idx = class_member_data.indexOf("operator");
        static const QVector<QString> key = { "=", "-", "*", "/", "%", "^", "&", "|", "~", "!", ",", "=", "<", ">", "<=" , ">=", "++", "--", "<<", ">>",
            "==", "!=", "&&", "||", "+=", "-=", "/=", "%=", "^=" , "&=" , "|=", "*=", "<<=?", ">>=" };
        Q_ASSERT(key.contains(class_member_data.at(oprator_idx + 1)));
        class_member_data[oprator_idx] += class_member_data.takeAt(oprator_idx + 1);
    }
    if (class_member_data.contains("::") || class_member_data.contains("<") || class_member_data.contains(">")) {
        int less_num = 0;
        int greater_num = 0;
        for (int i = 0; i < class_member_data.size(); i++) {
            if (class_member_data.at(i) == "<")
                less_num++;
            else if (class_member_data.at(i) == ">")
                greater_num++;
        }
        Q_ASSERT(less_num == greater_num);
        QList<QString> new_class_member_data;
        for (int i = 0; i < class_member_data.size(); i++) {
            if (class_member_data.at(i) == "::") {
                new_class_member_data.last().append("::");
                new_class_member_data.last().append(class_member_data.at(++i));
                continue;
            }
            else if (class_member_data.at(i) == "<") {
                int less_num = 0;
                for (; i < class_member_data.size(); i++) {
                    if (class_member_data.at(i) == "<") {
                        less_num++;
                    }
                    else if (class_member_data.at(i) == ">") {
                        less_num--;
                    }
                    new_class_member_data.last().append(class_member_data.at(i));
                    if (less_num == 0) {
                        i++;
                        break;
                    }
                }
            }
            new_class_member_data.append(class_member_data.at(i));
        }
        class_member_data = new_class_member_data;
    }

    /*变量*/
    if (class_member_data.contains("=") && class_member_data.contains("(") && class_member_data.contains(")") && class_member_data.indexOf("=") < class_member_data.indexOf("(")) {
        while (class_member_data.last() != "=") {
            class_member_data.takeLast();
        }
        class_member_data.takeLast();
    }
    if (!class_member_data.contains("(") && !class_member_data.contains(")")) {
        if (class_member_data.contains("=")) {
            while (class_member_data.last() != "=") {
                class_member_data.takeLast();
            }
            class_member_data.takeLast();
        }
        QString key[6] = { "*", "&", "const", "volatile", "restrict", "static" };
        Variate::VariateModifiers value[6] = { Variate::POINT, Variate::QUOTE, Variate::CONST, Variate::VOLATILE, Variate::RESTRICT, Variate::STATIC };
        for (int i = 0; i < 6; i++) {
            int idx = class_member_data.indexOf(key[i]);
            if (idx >= 0) {
                if (i == 1) {
                    int b = 0;
                    b++;
                }
                variate_info.modifiers = value[i] | variate_info.modifiers;
                class_member_data.removeAt(idx);
            }
        }
        if (class_member_data.size() == 0)
            return -1;
        if (class_member_data.size() == 1) {
            variate_info.type = class_member_data[0];
        }
        else if (class_member_data.size() == 2) {
            variate_info.type = class_member_data[0];
            variate_info.name = class_member_data[1];
        }
        else {
            variate_info.name = class_member_data.takeLast();
            for (int i = 0; i < class_member_data.size(); i++) {
                variate_info.type += class_member_data.at(i);
            }
        }
        return 1;
    }
    else if (class_member_data.contains("(") && class_member_data.contains(")")) {//函数
        if (class_member_data.contains("=")) {
            if (class_member_data.lastIndexOf("=") > class_member_data.lastIndexOf(")")) {
                if (class_member_data.first() == "virtual") {
                    function_info.type |= FunctionInfo::PURE;
                }
                while (class_member_data.last() != "=") {
                    class_member_data.takeLast();
                }
                class_member_data.takeLast();
            }
        }
        if (class_member_data.first() == "virtual") {
            function_info.type |= FunctionInfo::VIRTUAL;
            class_member_data.takeFirst();
        }
        if (class_member_data.first() == "~") {
            function_info.type |= FunctionInfo::DESTRUCTOR;
            class_member_data.takeFirst();
        }
        int left_idx = class_member_data.indexOf("(");
        int right_idx = class_member_data.indexOf(")");
        Q_ASSERT(left_idx < right_idx);
        if (left_idx == 1) {
            if (!(function_info.type & FunctionInfo::DESTRUCTOR)) {
                function_info.type |= FunctionInfo::CONSTRUCTED;
            }
            function_info.functionName = class_member_data.at(0);
        }
        else if (left_idx > 1) {
            function_info.functionName = class_member_data.at(left_idx - 1);
            int ret = analyzeClassMember(class_member_data.mid(0, left_idx - 1), function_info.returnValue, FunctionInfo());
            if (ret != 1)
                return -1;
        }
        else {
            return -1;
        }
        QList<QString> para_value;
        int less_num = 0;
        for (int i = left_idx + 1; i <= right_idx; i++) {
            if (class_member_data.at(i) == "<") {
                less_num++;
            }
            else if (class_member_data.at(i) == ">") {
                less_num--;
            }
            if (less_num == 0) {
                if ((class_member_data.at(i) == "," || class_member_data.at(i) == ")") && para_value.size() > 0) {
                    Variate para_var;
                    int ret = analyzeClassMember(para_value, para_var, FunctionInfo());
                    if (ret != 1)
                        return -1;
                    function_info.paraValue.append(para_var);
                    para_value.clear();
                }
                if ((class_member_data.at(i) == "," || class_member_data.at(i) == ")"))
                    continue;
            }
            para_value.append(class_member_data.at(i));
        }
        Q_ASSERT(less_num == 0);
        return 2;
    }
    return -1;
}

void QuexTool::analyzeHfileQuexNode(const QString str, LexerForC_Token* token_p, HFileInfo& hfile_info)
{
    static ClassInfo class_info;
    static QList<QString> class_member_data;

    static int current_member_privilege = 2;
    QString token_str = QString::fromLocal8Bit((char*)token_p->text);
    if (str == "root&PP_LOCAL_INCLUDE") {		//#include "file.h"
        hfile_info.include_local_file.append(token_str);
    } else if (str == "root&PP_STANDARD_INCLUDE") {		//#include <file.h>
        hfile_info.include_standard_file.append(token_str);
    } else if (str == "root&PP_CLASS") {		//class
    } else if (str == "class_name&IDENTIFIER") {		//string
        class_info.className = token_str;
        current_member_privilege = 2;
    } else if (str == "class_name_end&SEMICOLON") {		//;
    } else if (str == "class_name_end&COLON") {		//:
    } else if (str == "class_name_end&CURLY_BRACKET_O") {		//{
    } else if (str == "class_parent&PP_PUBLIC") {		//public
    } else if (str == "class_parent&PP_PROTECTED") {		//protected
    } else if (str == "class_parent&IDENTIFIER") {		//string
        class_info.parentName.append(token_str);
    } else if (str == "class_parent_end&COMMA") {		//,
    } else if (str == "class_parent_end&CURLY_BRACKET_O") {		//{
    } else if (str == "class_body&PP_PUBLIC") {		//public
        current_member_privilege = 0;
    } else if (str == "class_body&PP_PROTECTED") {		//protected
        current_member_privilege = 1;
    } else if (str == "class_body&PP_PRIVATE") {		//private
        current_member_privilege = 2;
    } else if (str == "class_body&IDENTIFIER") {		//string
        Q_ASSERT(class_member_data.isEmpty());
        class_member_data.clear();
        class_member_data.append(token_str);
    } else if (str == "class_body&CURLY_BRACKET_C") {		//}
    } else if (str == "class_body&CURLY_BRACKET_O") {		//{
    } else if (str == "class_body_privilege&COLON") {		//:
    } else if (str == "class_body_end&SEMICOLON") {		//;
        hfile_info.classInfos.append(class_info);
        class_info.clear();
    } else if (str == "class_member_define&IDENTIFIER"		//string
        || str == "class_member_define&BRACKET_O"		//(
        || str == "class_member_define&BRACKET_C"		//)
        || str == "class_member_define&COMMA"		//,
        || str == "class_member_define&OP_ASSIGNMENT"		//=
        || str == "class_member_define&DOUBLE_COLON"		//::
        || str == "class_member_define&TILDE"		//~
        || str == "class_member_define&MULT"		//*
        || str == "class_member_define&LESS"		//<
        || str == "class_member_define&GREATER"		//>
        || str == "class_member_define&AMPERSANT"		//&
        || str == "class_member_define&ASSIGN_PLUS"		//+=
        || str == "class_member_define&ASSIGN_MINUS"		//-=
        || str == "class_member_define&ASSIGN_MULT"		//*=
        || str == "class_member_define&ASSIGN_DIV"		///=
        || str == "class_member_define&EQ"		//==
        || str == "class_member_define&NOT_EQ"		//!=
        || str == "class_member_define&OR"		//||
        || str == "class_member_define&AND"		//&&
        || str == "class_member_define&NOT"){		//|
        class_member_data.append(token_str);
    } else if (str == "class_member_define&SEMICOLON" || str == "class_member_define&CURLY_BRACKET_O") {		//;  {
        if (class_member_data.size() > 0) {
            Variate variate_info;
            FunctionInfo function_info;
            int result = analyzeClassMember(class_member_data, variate_info, function_info);
            class_member_data.clear();
            if (result == 1) {
                class_info.variate[current_member_privilege].append(variate_info);
            }
            else if (result == 2) {
                class_info.functionInfo[current_member_privilege].append(function_info);
            }
        }
    } else if (str == "class_member_define_3&CURLY_BRACKET_O") {		//{
        Q_ASSERT(false);
    }
}