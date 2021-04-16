quex_str = [
"#include \"file.h\"",
"#include <file.h>",
"public",
"protected",
"private",
"class",
"enum",
"{",
"}",
"string",
":",
"::",
"(",
")",
";",
",",
"=",
"enum",
"*",
"~",
">",
"<",
"&",
"+=",
"-=",
"*=",
"/=",
"==",
"!=",
"||",
"&&",
"|"]
quex_id = [
"QUEX_TKN_PP_LOCAL_INCLUDE",      
"QUEX_TKN_PP_STANDARD_INCLUDE",   
"QUEX_TKN_PP_PUBLIC", 
"QUEX_TKN_PP_PROTECTED",          
"QUEX_TKN_PP_PRIVATE",            
"QUEX_TKN_PP_CLASS",              
"QUEX_TKN_PP_ENUM",               
"QUEX_TKN_CURLY_BRACKET_O",       
"QUEX_TKN_CURLY_BRACKET_C",       
"QUEX_TKN_IDENTIFIER",           
"QUEX_TKN_COLON",          
"QUEX_TKN_DOUBLE_COLON",                 
"QUEX_TKN_BRACKET_O",             
"QUEX_TKN_BRACKET_C",             
"QUEX_TKN_SEMICOLON",            
"QUEX_TKN_COMMA",
"QUEX_TKN_OP_ASSIGNMENT",
"QUEX_TKN_PP_ENUM",
"QUEX_TKN_MULT",
"QUEX_TKN_TILDE",
"QUEX_TKN_GREATER",
"QUEX_TKN_LESS",
"QUEX_TKN_AMPERSANT",
"QUEX_TKN_ASSIGN_PLUS",
"QUEX_TKN_ASSIGN_MINUS",
"QUEX_TKN_ASSIGN_MULT",
"QUEX_TKN_ASSIGN_DIV",
"QUEX_TKN_EQ",
"QUEX_TKN_NOT_EQ",
"QUEX_TKN_OR",
"QUEX_TKN_AND",
"QUEX_TKN_NOT"]

f = open("hFileQuexNode.txt.qml")
w_f = open("hFileQuexNode.h", 'w')
w_f.write('#pragma once\n')
w_f.write('#include "quexNode.h"\n')
w_f.write('#include <QVector>\n\n')
w_f.write('class HFileQuexNode\n')
w_f.write('{\n')
w_f.write('public:\n')
w_f.write('    HFileQuexNode(){\n')
line = f.readline()
line = f.readline()
root_item = True
quex_node_id = ""
quex_define_str = []
quex_data_str = []
quex_comment_str = []
while line:
    if line[0] != ' ':
        quex_node_id = line.strip().replace(' ', '').replace('\n', '').replace('\t', '').replace('\r', '').strip()
        quex_define_str.append('        QuexNode* ' + quex_node_id + ' = new QuexNode("' + quex_node_id + '");\n')
        quex_define_str.append('        nodeList.append(' + quex_node_id + ');\n')
        if root_item:
            quex_define_str.append('        rootNode = ' + quex_node_id + ';\n')
            root_item = False
    else:
        line = line.replace('\n', '').replace('\r', '')
        line_data = line.split()
        valid_id = ''
        for idx in range(len(line_data)):
            if idx == 0:
                is_include_head = False
                if line_data[0] == '#include':
                    line_data[0] = line_data[0] + ' ' + line_data[1]
                    is_include_head = True
                find_idx = quex_str.index(line_data[idx])
                if find_idx != -1:
                    valid_id = quex_id[find_idx]
                    quex_data_str.append('        ' + quex_node_id + '->validIds.append(' + valid_id + ');\n')
                    quex_comment_str.append('    } else if (str == "' + quex_node_id + '&' +  valid_id[9:] + '") {\t\t//' + quex_str[find_idx])
                else:
                    break
                if is_include_head:
                    break
            if idx == 1 and quex_node_id != line_data[idx]:
                quex_data_str.append('        ' + quex_node_id + '->next.insert(' + valid_id + ', ' + line_data[idx] + ');\n')
            #if idx == 2:
            #    quex_data_str.append('        ' + quex_node_id + '->value = "' + line_data[idx] + '";\n')
    line = f.readline()
for str in quex_define_str:
    w_f.write(str)
for str in quex_data_str:
    w_f.write(str)

w_f.write('    }\n')
w_f.write('    ~HFileQuexNode(){\n')
w_f.write('        for(int i = 0; i < nodeList.size(); i++){\n')
w_f.write('            delete nodeList.at(i);\n')
w_f.write('        }\n')
w_f.write('    }\n')
w_f.write('    QuexNode* getRootNode(){ return rootNode;}\n')
w_f.write('private:\n')
w_f.write('    QuexNode* rootNode = nullptr;\n')
w_f.write('    QVector<QuexNode*> nodeList;\n')
w_f.write('};\n\n')
w_f.write('/*\n')
for str in quex_comment_str:
    w_f.write(str + '\n')
w_f.write('*/\n')
f.close()
w_f.close()
