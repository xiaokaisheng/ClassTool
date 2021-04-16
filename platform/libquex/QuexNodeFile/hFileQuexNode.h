#pragma once
#include "quexNode.h"
#include <QVector>

class HFileQuexNode
{
public:
    HFileQuexNode(){
        QuexNode* root = new QuexNode("root");
        nodeList.append(root);
        rootNode = root;
        QuexNode* class_name = new QuexNode("class_name");
        nodeList.append(class_name);
        QuexNode* class_name_end = new QuexNode("class_name_end");
        nodeList.append(class_name_end);
        QuexNode* class_parent = new QuexNode("class_parent");
        nodeList.append(class_parent);
        QuexNode* class_parent_end = new QuexNode("class_parent_end");
        nodeList.append(class_parent_end);
        QuexNode* class_body = new QuexNode("class_body");
        nodeList.append(class_body);
        QuexNode* class_body_enum = new QuexNode("class_body_enum");
        nodeList.append(class_body_enum);
        QuexNode* class_body_1 = new QuexNode("class_body_1");
        nodeList.append(class_body_1);
        QuexNode* class_body_2 = new QuexNode("class_body_2");
        nodeList.append(class_body_2);
        QuexNode* class_body_3 = new QuexNode("class_body_3");
        nodeList.append(class_body_3);
        QuexNode* class_body_end = new QuexNode("class_body_end");
        nodeList.append(class_body_end);
        QuexNode* class_member_define = new QuexNode("class_member_define");
        nodeList.append(class_member_define);
        QuexNode* class_member_define_1 = new QuexNode("class_member_define_1");
        nodeList.append(class_member_define_1);
        QuexNode* class_member_define_2 = new QuexNode("class_member_define_2");
        nodeList.append(class_member_define_2);
        QuexNode* class_member_define_3 = new QuexNode("class_member_define_3");
        nodeList.append(class_member_define_3);
        root->validIds.append(QUEX_TKN_PP_LOCAL_INCLUDE);
        root->validIds.append(QUEX_TKN_PP_STANDARD_INCLUDE);
        root->validIds.append(QUEX_TKN_PP_CLASS);
        root->next.insert(QUEX_TKN_PP_CLASS, class_name);
        class_name->validIds.append(QUEX_TKN_IDENTIFIER);
        class_name->next.insert(QUEX_TKN_IDENTIFIER, class_name_end);
        class_name_end->validIds.append(QUEX_TKN_SEMICOLON);
        class_name_end->next.insert(QUEX_TKN_SEMICOLON, root);
        class_name_end->validIds.append(QUEX_TKN_COLON);
        class_name_end->next.insert(QUEX_TKN_COLON, class_parent);
        class_name_end->validIds.append(QUEX_TKN_CURLY_BRACKET_O);
        class_name_end->next.insert(QUEX_TKN_CURLY_BRACKET_O, class_body);
        class_parent->validIds.append(QUEX_TKN_PP_PUBLIC);
        class_parent->validIds.append(QUEX_TKN_PP_PROTECTED);
        class_parent->validIds.append(QUEX_TKN_IDENTIFIER);
        class_parent->next.insert(QUEX_TKN_IDENTIFIER, class_parent_end);
        class_parent_end->validIds.append(QUEX_TKN_COMMA);
        class_parent_end->next.insert(QUEX_TKN_COMMA, class_parent);
        class_parent_end->validIds.append(QUEX_TKN_CURLY_BRACKET_O);
        class_parent_end->next.insert(QUEX_TKN_CURLY_BRACKET_O, class_body);
        class_body->validIds.append(QUEX_TKN_PP_PUBLIC);
        class_body->validIds.append(QUEX_TKN_PP_PROTECTED);
        class_body->validIds.append(QUEX_TKN_PP_PRIVATE);
        class_body->validIds.append(QUEX_TKN_PP_ENUM);
        class_body->next.insert(QUEX_TKN_PP_ENUM, class_body_enum);
        class_body->validIds.append(QUEX_TKN_IDENTIFIER);
        class_body->next.insert(QUEX_TKN_IDENTIFIER, class_member_define);
        class_body->validIds.append(QUEX_TKN_CURLY_BRACKET_C);
        class_body->next.insert(QUEX_TKN_CURLY_BRACKET_C, class_body_end);
        class_body->validIds.append(QUEX_TKN_CURLY_BRACKET_O);
        class_body->next.insert(QUEX_TKN_CURLY_BRACKET_O, class_body_1);
        class_body_enum->validIds.append(QUEX_TKN_CURLY_BRACKET_O);
        class_body_enum->next.insert(QUEX_TKN_CURLY_BRACKET_O, class_body_1);
        class_body_1->validIds.append(QUEX_TKN_CURLY_BRACKET_O);
        class_body_1->next.insert(QUEX_TKN_CURLY_BRACKET_O, class_body_2);
        class_body_1->validIds.append(QUEX_TKN_CURLY_BRACKET_C);
        class_body_1->next.insert(QUEX_TKN_CURLY_BRACKET_C, class_body);
        class_body_2->validIds.append(QUEX_TKN_CURLY_BRACKET_O);
        class_body_2->next.insert(QUEX_TKN_CURLY_BRACKET_O, class_body_3);
        class_body_2->validIds.append(QUEX_TKN_CURLY_BRACKET_C);
        class_body_2->next.insert(QUEX_TKN_CURLY_BRACKET_C, class_body_1);
        class_body_3->validIds.append(QUEX_TKN_CURLY_BRACKET_O);
        class_body_3->validIds.append(QUEX_TKN_CURLY_BRACKET_C);
        class_body_3->next.insert(QUEX_TKN_CURLY_BRACKET_C, class_body_2);
        class_body_end->validIds.append(QUEX_TKN_SEMICOLON);
        class_body_end->next.insert(QUEX_TKN_SEMICOLON, root);
        class_member_define->validIds.append(QUEX_TKN_IDENTIFIER);
        class_member_define->validIds.append(QUEX_TKN_BRACKET_O);
        class_member_define->validIds.append(QUEX_TKN_BRACKET_C);
        class_member_define->validIds.append(QUEX_TKN_COMMA);
        class_member_define->validIds.append(QUEX_TKN_OP_ASSIGNMENT);
        class_member_define->validIds.append(QUEX_TKN_DOUBLE_COLON);
        class_member_define->validIds.append(QUEX_TKN_MULT);
        class_member_define->validIds.append(QUEX_TKN_TILDE);
        class_member_define->validIds.append(QUEX_TKN_LESS);
        class_member_define->validIds.append(QUEX_TKN_GREATER);
        class_member_define->validIds.append(QUEX_TKN_AMPERSANT);
        class_member_define->validIds.append(QUEX_TKN_ASSIGN_PLUS);
        class_member_define->validIds.append(QUEX_TKN_ASSIGN_MINUS);
        class_member_define->validIds.append(QUEX_TKN_ASSIGN_MULT);
        class_member_define->validIds.append(QUEX_TKN_ASSIGN_DIV);
        class_member_define->validIds.append(QUEX_TKN_EQ);
        class_member_define->validIds.append(QUEX_TKN_NOT_EQ);
        class_member_define->validIds.append(QUEX_TKN_OR);
        class_member_define->validIds.append(QUEX_TKN_AND);
        class_member_define->validIds.append(QUEX_TKN_NOT);
        class_member_define->validIds.append(QUEX_TKN_CURLY_BRACKET_O);
        class_member_define->next.insert(QUEX_TKN_CURLY_BRACKET_O, class_member_define_1);
        class_member_define->validIds.append(QUEX_TKN_SEMICOLON);
        class_member_define->next.insert(QUEX_TKN_SEMICOLON, class_body);
        class_member_define_1->validIds.append(QUEX_TKN_CURLY_BRACKET_O);
        class_member_define_1->next.insert(QUEX_TKN_CURLY_BRACKET_O, class_member_define_2);
        class_member_define_1->validIds.append(QUEX_TKN_CURLY_BRACKET_C);
        class_member_define_1->next.insert(QUEX_TKN_CURLY_BRACKET_C, class_body);
        class_member_define_2->validIds.append(QUEX_TKN_CURLY_BRACKET_O);
        class_member_define_2->next.insert(QUEX_TKN_CURLY_BRACKET_O, class_member_define_3);
        class_member_define_2->validIds.append(QUEX_TKN_CURLY_BRACKET_C);
        class_member_define_2->next.insert(QUEX_TKN_CURLY_BRACKET_C, class_member_define_1);
        class_member_define_3->validIds.append(QUEX_TKN_CURLY_BRACKET_O);
        class_member_define_3->validIds.append(QUEX_TKN_CURLY_BRACKET_C);
        class_member_define_3->next.insert(QUEX_TKN_CURLY_BRACKET_C, class_member_define_2);
    }
    ~HFileQuexNode(){
        for(int i = 0; i < nodeList.size(); i++){
            delete nodeList.at(i);
        }
    }
    QuexNode* getRootNode(){ return rootNode;}
private:
    QuexNode* rootNode = nullptr;
    QVector<QuexNode*> nodeList;
};

/*
    } else if (str == "root&PP_LOCAL_INCLUDE") {		//#include "file.h"
    } else if (str == "root&PP_STANDARD_INCLUDE") {		//#include <file.h>
    } else if (str == "root&PP_CLASS") {		//class
    } else if (str == "class_name&IDENTIFIER") {		//string
    } else if (str == "class_name_end&SEMICOLON") {		//;
    } else if (str == "class_name_end&COLON") {		//:
    } else if (str == "class_name_end&CURLY_BRACKET_O") {		//{
    } else if (str == "class_parent&PP_PUBLIC") {		//public
    } else if (str == "class_parent&PP_PROTECTED") {		//protected
    } else if (str == "class_parent&IDENTIFIER") {		//string
    } else if (str == "class_parent_end&COMMA") {		//,
    } else if (str == "class_parent_end&CURLY_BRACKET_O") {		//{
    } else if (str == "class_body&PP_PUBLIC") {		//public
    } else if (str == "class_body&PP_PROTECTED") {		//protected
    } else if (str == "class_body&PP_PRIVATE") {		//private
    } else if (str == "class_body&PP_ENUM") {		//enum
    } else if (str == "class_body&IDENTIFIER") {		//string
    } else if (str == "class_body&CURLY_BRACKET_C") {		//}
    } else if (str == "class_body&CURLY_BRACKET_O") {		//{
    } else if (str == "class_body_enum&CURLY_BRACKET_O") {		//{
    } else if (str == "class_body_1&CURLY_BRACKET_O") {		//{
    } else if (str == "class_body_1&CURLY_BRACKET_C") {		//}
    } else if (str == "class_body_2&CURLY_BRACKET_O") {		//{
    } else if (str == "class_body_2&CURLY_BRACKET_C") {		//}
    } else if (str == "class_body_3&CURLY_BRACKET_O") {		//{
    } else if (str == "class_body_3&CURLY_BRACKET_C") {		//}
    } else if (str == "class_body_end&SEMICOLON") {		//;
    } else if (str == "class_member_define&IDENTIFIER") {		//string
    } else if (str == "class_member_define&BRACKET_O") {		//(
    } else if (str == "class_member_define&BRACKET_C") {		//)
    } else if (str == "class_member_define&COMMA") {		//,
    } else if (str == "class_member_define&OP_ASSIGNMENT") {		//=
    } else if (str == "class_member_define&DOUBLE_COLON") {		//::
    } else if (str == "class_member_define&MULT") {		//*
    } else if (str == "class_member_define&TILDE") {		//~
    } else if (str == "class_member_define&LESS") {		//<
    } else if (str == "class_member_define&GREATER") {		//>
    } else if (str == "class_member_define&AMPERSANT") {		//&
    } else if (str == "class_member_define&ASSIGN_PLUS") {		//+=
    } else if (str == "class_member_define&ASSIGN_MINUS") {		//-=
    } else if (str == "class_member_define&ASSIGN_MULT") {		//*=
    } else if (str == "class_member_define&ASSIGN_DIV") {		///=
    } else if (str == "class_member_define&EQ") {		//==
    } else if (str == "class_member_define&NOT_EQ") {		//!=
    } else if (str == "class_member_define&OR") {		//||
    } else if (str == "class_member_define&AND") {		//&&
    } else if (str == "class_member_define&NOT") {		//|
    } else if (str == "class_member_define&CURLY_BRACKET_O") {		//{
    } else if (str == "class_member_define&SEMICOLON") {		//;
    } else if (str == "class_member_define_1&CURLY_BRACKET_O") {		//{
    } else if (str == "class_member_define_1&CURLY_BRACKET_C") {		//}
    } else if (str == "class_member_define_2&CURLY_BRACKET_O") {		//{
    } else if (str == "class_member_define_2&CURLY_BRACKET_C") {		//}
    } else if (str == "class_member_define_3&CURLY_BRACKET_O") {		//{
    } else if (str == "class_member_define_3&CURLY_BRACKET_C") {		//}
*/
