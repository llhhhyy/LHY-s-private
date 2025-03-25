#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QVector>
#include <QMap>
#include <QStack>
#include "handlecode.h"
#include "syntaxtree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // void on_cmdLineEdit_editingFinished();

private:
    enum Exp_Type{VAR,OP,NUM,ASS,COM,NON};
    Ui::MainWindow *ui;
    HandleCode* code;
    SyntaxTree tree;
    QVector<QString> priVar;
    QVector<QString> CommandList_of_Load;
    QMap<QString,int> priMap;
    QMetaObject::Connection connection;
    bool ifINPUT;

    void setUIForDebugMode();
    void setUIExitDebugMode();
    void readCommand();
    void printExpTree();
    void RunCode();
    void loadCode();
    void clearAll();
    void showHelp();
    void PRINT(QStringList printList);
    void LET(QStringList cmd);
    void INPUT(QString var);
    QStringList inToPost(QStringList infix);
    int evaluate(QStringList postfix);
    int calExp(QStringList exp);
    QString getCode();
    Exp_Type getEtype(QString exp);
    bool conInvaildChar(QStringList exp);
    void saveCodeToFile();
    void executeCommand();

};
#endif // MAINWINDOW_H
