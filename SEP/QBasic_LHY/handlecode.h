#ifndef HANDLECODE_H
#define HANDLECODE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMap>
#include <QStack>
#include <QLineEdit>
#include <QTextBrowser>
#include <QCoreApplication>
#include <QMessageBox>

class HandleCode : public QObject
{
    Q_OBJECT
private:

    QMap<QString,int> varMap;
    QVector<QString> varList;
    // QVector<int> val_of_var;
    QVector<QString> breakPoint;
    QVector<int> breakline;
    QLineEdit* cmdLineEdit;
    QLineEdit* inputLineEdit;
    QTextBrowser* resultDisplay;
    QTextBrowser* breakPointDisplay;
    QTextBrowser* monitorDisplay;
    int line;
    bool ifEND;

    void handleLET(bool &ok);
    void handleREM(bool &ok);
    void handlePRINT(bool &ok);
    void handleINPUT(bool &ok);
    void handleGOTO(bool &ok);
    void handleIF(bool &ok);
    void handleEND(bool &ok);
    void handleCode(bool &ok);
    void exeCode(bool &con);
    void continueExe();
    void showMonitor();
    QStringList infixToPostfix(QStringList infix);
    int evaluate(QStringList postfix);
    bool findLine(QString tar);
    void displayBreakPoint();
    void findBreakLine();
    void getEND();
    void changeBreakPos(int pos);
    void rankBreakPoint();
    bool conInvaildChar(QStringList exp);
    int calculateExp(QStringList exp);

public:
    enum Exp_Type{VAR,OP,NUM,ASS,COM,NON};
    bool ifDebug;
    QStringList CodeList;
    HandleCode(QObject *parent,QLineEdit* cmd,QLineEdit* input,QTextBrowser* res,QTextBrowser* breakpoint,QTextBrowser* monitor);
    void remake(QStringList code);
    void remake();
    void RUN(bool &ok);
    void addBreakPoint(QString b);
    void deleteBreakPoint(QString b);
    Exp_Type getEtype(QString exp);
    void clear();
    void stopRun();
    bool isOp(QString exp);
    int precedence(QString op);
    int mod(int a,int b);
};

#endif // HANDLECODE_H
