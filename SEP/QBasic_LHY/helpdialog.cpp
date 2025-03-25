#include "helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Help");

    QTextBrowser *helpTextBrowser = new QTextBrowser(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(helpTextBrowser);
    setLayout(layout);

    // 加载并显示帮助文件
    QFile helpFile("://HELP.txt"); // 假设HELP.txt在Qt资源文件中
    if (helpFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&helpFile);
        helpTextBrowser->setPlainText(in.readAll());
        helpFile.close();
    } else {
        helpTextBrowser->setPlainText("Cannot load help content.");
    }
}
