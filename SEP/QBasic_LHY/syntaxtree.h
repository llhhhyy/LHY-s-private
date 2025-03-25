#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H
#include <QString>
#include <QStringList>
#include <QStack>
#include <QVector>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>


class SyntaxTree
{
public:

    SyntaxTree();
    void creatTree(QStringList words,bool &ok);
    QString printTree();
    ~SyntaxTree();
private:
    enum Exp_Type{VAR,OP,NUM,ASS,COM};
    enum Sent_Type {LET=0,IF=1,GOTO=2,PRINT=3,OTHER};
    struct ExpNode
    {
        ExpNode(QString v,Exp_Type t):type(t),val(v),left(NULL),right(NULL){}
        Exp_Type type;
        QString val;
        ExpNode* left;
        ExpNode* right;
    };
    struct SentNode
    {
        SentNode(QString v,Sent_Type t):type(t),sent(v){}
        Sent_Type type;
        QString sent;
        QVector<ExpNode*> childs;
        ~SentNode(){}
    };
    QStringList varList;
    SentNode* sRoot;
    int* varValue;
    int findVar(QString tar);
    Exp_Type getEtype(QString exp);
    Sent_Type getStype(QString sent);
    int getPrecedence(QString op);
    bool isOp(QString token);
    QStringList infixToPostfix(QStringList infix);
    void deleteTree(SentNode* node);
    void deleteTree(ExpNode* node);
    QString displayTree(ExpNode* node,int indent);
    ExpNode* constructExpTree(QStringList postfix);
    bool conInvaildChar(QStringList exp);
};

#endif // SYNTAXTREE_H
