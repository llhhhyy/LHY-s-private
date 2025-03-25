#ifndef HELPDIALOG_H
#define HELPDIALOG_H
#include <QDialog>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QIODevice>
#include <QFile>

class HelpDialog : public QDialog {
    Q_OBJECT

public:
    HelpDialog(QWidget *parent = nullptr);
};

#endif // HELPDIALOG_H
