#ifndef QUEXNODE_H
#define QUEXNODE_H
#include <QVector>
#include <QMap>
#include <QString>
#include "lexical_analysis/LexerForC"

class QuexNode
{
public:
    QuexNode(QString _id) :id(_id)
    {}
    ~QuexNode() {}
	
    QVector<LexerForC_token_id_t> validIds;
    QMap<LexerForC_token_id_t, QuexNode*> next;
    QString value;
public:
    QString getId() { return id; }
private:
    QString id;
};

#endif // QUEXNODE_H
