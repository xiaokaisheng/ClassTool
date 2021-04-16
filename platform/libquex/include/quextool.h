#include <iostream> 
#include <string>
#include "hFileInfo.h"

class LexerForC_Token;
class QuexNode;
class HFileQuexNode;
class QuexTool
{
public:
    QuexTool();
    ~QuexTool();
    HFileInfo quexHFile(std::string file_path);
private:
    Variate analyzeVariate(QList<QString> data);
    int analyzeClassMember(QList<QString> class_member_data, Variate& variate_info, FunctionInfo& function_info);
    void analyzeHfileQuexNode(const QString nodeid_token, LexerForC_Token* token_p, HFileInfo& h_file_info);
    void initHFileNodeRoot();
private:
    HFileQuexNode* hFileQuexNode = nullptr;
    QuexNode* hFileCurrentNode = nullptr;
    ClassInfo* classInfo = nullptr;
    QVector<QString> baseType;
};