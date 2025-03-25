#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpdialog.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ifINPUT(false)
{
    ui->setupUi(this);
    code = new HandleCode(this,ui->cmdLineEdit,ui->inputLineEdit,ui->resultDisplay,ui->breakPointsDisplay,ui->monitorDisplay);
    if(!ui->CodeDisplay->toPlainText().isEmpty())
        code->CodeList = ui->CodeDisplay->toPlainText().split("\n");
    setUIExitDebugMode();
    ui->inputLineEdit->setVisible(false);
    ui->inputLabel->setVisible(false);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveCodeToFile);
    connect(ui->btnDebugMode, &QPushButton::clicked, this, &MainWindow::setUIForDebugMode);
    connect(ui->btnExitDebugMode, &QPushButton::clicked, this, &MainWindow::setUIExitDebugMode);
    connect(ui->btnClearCode,&QPushButton::clicked,this,&MainWindow::clearAll);
    connect(ui->cmdLineEdit,&QLineEdit::returnPressed,this,&MainWindow::readCommand);
    connect(ui->btnLoadCode, &QPushButton::clicked, this, &MainWindow::loadCode);
    connect(ui->CodeDisplay, &QTextBrowser::textChanged, this, [this](){
        QString text = ui->CodeDisplay->toPlainText();
        if (!text.isEmpty()){
            printExpTree();
        }
    });
    connect(ui->CodeDisplay, &QTextBrowser::textChanged, this, [this](){
        QString text = ui->CodeDisplay->toPlainText();
        if (!text.isEmpty()) { // 检查文本是否为空
            this->code->remake(text.split("\n"));
        }
    });
    connect(ui->btnRunCode, &QPushButton::clicked, this, &MainWindow::RunCode);
    connect(ui->btnStopRun, &QPushButton::clicked, this, [this](){
        this->code->stopRun();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
 设置UI进入调试模式
 */
void MainWindow::setUIForDebugMode(){
    code->ifDebug = true;
    code->remake();
    ui->btnClearCode->setVisible(false);
    ui->btnLoadCode->setVisible(false);
    ui->btnDebugMode->setVisible(false);

    ui->btnExitDebugMode->setVisible(true);
    ui->btnStopRun->setVisible(true);

    ui->labelSyntaxTree->setVisible(false);
    ui->treeDisplay->setVisible(false);

    ui->labelMonitor->setVisible(true);
    ui->monitorDisplay->setVisible(true);
    ui->labelBreakPoints->setVisible(true);
    ui->breakPointsDisplay->setVisible(true);
}

/*
 设置UI退出调试模式
 */
void MainWindow::setUIExitDebugMode(){
    code->ifDebug = false;
    code->remake();
    ui->btnClearCode->setVisible(true);
    ui->btnLoadCode->setVisible(true);
    ui->btnDebugMode->setVisible(true);

    ui->btnExitDebugMode->setVisible(false);
    ui->btnStopRun->setVisible(false);

    ui->labelSyntaxTree->setVisible(true);
    ui->treeDisplay->setVisible(true);

    ui->labelMonitor->setVisible(false);
    ui->monitorDisplay->setVisible(false);
    ui->labelBreakPoints->setVisible(false);
    ui->breakPointsDisplay->setVisible(false);
}

/*
  读取命令行输入并处理
 */
void MainWindow::readCommand(){
    if(ifINPUT)return;
    //处理命令
    try{
        QString input = ui->cmdLineEdit->text();
        QStringList words = input.split(" ");
        QString text = ui->CodeDisplay->toPlainText();
        QStringList lines = text.split("\n");
        if(words[0][0].isDigit()){//首个字符是数字，说明是对对应行号进行输入
            if(words.size()==1){
                bool ok;
                int lineNum = words[0].toInt(&ok);
                if(ok){
                    QRegularExpression reg("^\\d+");
                    for(int i=0;i<lines.size();++i){
                        QString line = lines[i];
                        QRegularExpressionMatch match = reg.match(line);
                        if(match.hasMatch()&&match.captured(0).toInt()==lineNum){
                            lines.removeAt(i);
                            ui->CodeDisplay->setPlainText(lines.join("\n"));
                            break;
                        }else{
                            // QMessageBox::critical(nullptr,"Error","This line doesn't exist.");
                        }
                    }
                }else{
                    QMessageBox::critical(nullptr,"Error","Your input is not num.");
                }
                ui->cmdLineEdit->clear();
            }else{
                bool ok;
                int lineNum = words[0].toInt(&ok);
                // if(words[1]!="REM"){
                //     bool invalid = conInvaildChar(words);
                //     if(invalid){
                //         QMessageBox::critical(nullptr,"Error",QString::number(lineNum)+" at main has invalid code.");
                //         return;
                //     }
                // }
                if(ok){
                    QRegularExpression reg("^\\d+");
                    int newpos = 0,lastpos = 0;
                    bool inserted = false;
                    for(int i=0;i<lines.size();++i){
                        QString line = lines[i];
                        QRegularExpressionMatch match = reg.match(line);
                        if(match.hasMatch()){
                            newpos = match.captured(0).toInt();
                            if(lineNum>lastpos&&lineNum<newpos){
                                lines.insert(i,input);
                                inserted = true;
                                break;
                            }
                            if(lineNum==newpos){
                                lines[i]=input;
                                inserted=true;
                                break;
                            }
                            lastpos = newpos;
                        }
                    }
                    if(!inserted){
                        lines.append(input);
                    }
                    ui->CodeDisplay->setPlainText(lines.join("\n"));
                }
            }
        }else if(words[0]=="RUN"){
            RunCode();
        }else if(words[0]=="LOAD"){
            loadCode();
        }else if(words[0]=="CLEAR"){
            clearAll();
        }else if(words[0]=="HELP"){
            showHelp();
        }else if(words[0]=="QUIT"){
            delete code;
            this->close();
        }else if(words[0]=="PRINT"){
            PRINT(words);
        }else if(words[0]=="ADD"){
            code->addBreakPoint(words[1]);
        }else if(words[0]=="DELETE"){
            code->deleteBreakPoint(words[1]);
        }else if(words[0]=="LET"){
            LET(words);
        }else if(words[0]=="INPUT"){
            QString var = words[1];
            ifINPUT = true;
            connection = connect(ui->cmdLineEdit, &QLineEdit::returnPressed, this, [this, var]() {
                this->INPUT(var);
            });
        }else{//还有什么输入吗？
            //无效输入
            QMessageBox::critical(nullptr,"Error","Your input isn't a command.");
        }

        ui->cmdLineEdit->clear();
    }catch(QString error){
        //报错，未提取到输入
    }
}
void MainWindow::executeCommand() {
    if(CommandList_of_Load.isEmpty())return;
    for (const QString &command : CommandList_of_Load) {
        QStringList words = command.split(" ");

        if (words[0] == "RUN") {
            RunCode();
        } else if (words[0] == "LOAD") {
            loadCode();
        } else if (words[0] == "CLEAR") {
            clearAll();
        } else if (words[0] == "HELP") {
            showHelp();
        } else if (words[0] == "QUIT") {
            delete code;
            this->close();
        } else if (words[0] == "PRINT") {
            PRINT(words);
        } else if (words[0] == "ADD") {
            code->addBreakPoint(words[1]);
        } else if (words[0] == "DELETE") {
            code->deleteBreakPoint(words[1]);
        } else if (words[0] == "LET") {
            LET(words);
        } else if (words[0] == "INPUT") {
            QString var = words[1];
            ifINPUT = true;
            connection = connect(ui->cmdLineEdit, &QLineEdit::returnPressed, this, [this, var]() {
                this->INPUT(var);
            });
        }else if(words[0]==""){
            return;
        }else {
            // 无效命令
            QMessageBox::critical(nullptr, "Error", "Invalid command: " + command);
        }
    }
    CommandList_of_Load.clear();
}

/*
  打印表达式树
 */
void MainWindow::printExpTree(){
    ui->treeDisplay->clear();
    QString code = ui->CodeDisplay->toPlainText();
    QStringList codelist = code.split("\n");
    for(int i=0;i<codelist.size();i++){
        QStringList input = codelist[i].split(" ");
        QString lineNum = input[0];
        // if(input[1]!="REM"){
        //     bool invalid = conInvaildChar(input);
        //     if(invalid){
        //         QMessageBox::critical(nullptr,"Error",lineNum+" has invalid code.");
        //         return;
        //     }
        // }
        input.removeFirst();
        bool ok=false;
        tree.creatTree(input,ok);
        QString treestr;
        if(ok)
            treestr = lineNum+QString(" ")+tree.printTree();
        else treestr = codelist[i];
        ui->treeDisplay->append(treestr);
    }
}

/*
  运行代码
 */
void MainWindow::RunCode(){
    bool ok = true;
    ui->inputLineEdit->setVisible(true);
    ui->inputLabel->setVisible(true);
    code->RUN(ok);
    ui->inputLabel->setVisible(false);
    ui->inputLineEdit->setVisible(false);
}

/*
  加载代码文件
 */
void MainWindow::loadCode(){
    clearAll();
    // 打开文件对话框，选择.txt文件
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Code File"), "", tr("Text Files (*.txt);;All Files (*)"));

    if (!fileName.isEmpty()) {
        // 打开文件
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString code = in.readAll();  // 读取文件所有内容
            QStringList codelist = code.split("\n");
            for (int i = 0; i < codelist.size(); i++) {
                QString it = codelist[i];
                QRegularExpression re("^(\\d+)"); // 匹配句首的数字
                QRegularExpressionMatch match = re.match(it);
                if (match.hasMatch()) {
                    ui->CodeDisplay->append(it);
                }else{
                    CommandList_of_Load.append(it);
                }
            }
            file.close();
            executeCommand();
        } else {
            // 如果无法打开文件，给出提示
            QMessageBox::warning(this, tr("Error"), tr("Cannot open file"));
        }
    }
}

/*
 清除所有内容
 */
void MainWindow::clearAll(){
    ui->CodeDisplay->clear();
    ui->resultDisplay->clear();
    ui->treeDisplay->clear();
    ui->breakPointsDisplay->clear();
    code->clear();
}

/*
  显示帮助对话框
 */
void MainWindow::showHelp(){
    HelpDialog *helpDialog = new HelpDialog(this);
    helpDialog->exec();  // 显示对话框并阻塞，直到对话框关闭
}

/*
  处理PRINT命令
 */
void MainWindow::PRINT(QStringList printList){
    printList.removeFirst();
    if(printList.size()<2){
        QString var = printList[0];
        if(priVar.contains(var)){
            int val = priMap[var];
            QString valstr = "Private var "+var+" = "+QString::number(val);
            ui->resultDisplay->append(valstr);
        }else{
            QMessageBox::critical(nullptr,"Error",var+" is not in the priVar.");
            qDebug()<<var<<"In PRINT: is not in the priVar\n";
        }
    }else{
        int res = calExp(printList);
        if(res!=-999){
            QString resstr = "Private exp "+printList.join(" ")+" = "+QString::number(res);
            ui->resultDisplay->append(resstr);
        }else{
            QMessageBox::critical(nullptr,"Error","In PRINT: calculate false.");
            qDebug()<<"In PRINT: calculate false\n";
        }
    }
}

/*
  处理LET命令
 */
void MainWindow::LET(QStringList cmd){
    cmd.removeFirst();
    QString var = cmd[0];
    cmd.removeFirst();cmd.removeFirst();
    if(!priVar.isEmpty()){
        if(!priVar.contains(var)){
            priVar.append(var);
            priMap[var]=calExp(cmd);
        }else{
            priMap[var]=calExp(cmd);
        }
    }else{
        priVar.append(var);
        priMap[var]=calExp(cmd);
    }
    QString let = "LET "+var+" = "+QString::number(calExp(cmd));
    ui->resultDisplay->append(let);
}

/*
  计算表达式的值
 */
int MainWindow::calExp(QStringList exp){
    QStringList postfix = inToPost(exp);
    if(postfix!=QStringList{"fal","se"}){
        return evaluate(postfix);
    }
    return -999;
}

/*
  将中缀表达式转换为后缀表达式
 */
QStringList MainWindow::inToPost(QStringList infix){
    QStringList posfix;
    QStack<QString> stack;

    for(int i = 0; i < infix.size(); ++i) {
        QString token = infix[i];
        bool ok;
        if(token.size()>1&&token[0]=='-'){
            token.remove(0,1);
            token.toInt(&ok);
            token = "-"+token;
        }else
            token.toInt(&ok);
        if (ok) {
            posfix.append(token);  // 如果是数字
        }else if (token == "(") {//左括号
            stack.push(token);
        } else if (token == ")") {//右括号
            while (!stack.isEmpty() && stack.top() != "(") {
                posfix.append(stack.pop());
            }
            if (!stack.isEmpty() && stack.top() == "(") {
                stack.pop();
            }
        } else if (code->isOp(token)) {//操作符
            while (!stack.isEmpty() && code->isOp(stack.top()) && code->precedence(stack.top()) >= code->precedence(token)) {
                posfix.append(stack.pop());
            }
            stack.push(token);
        } else if (token == "=") {//赋值等号
            stack.push(token);
        }else if(getEtype(token)==VAR){//变量
            posfix.append(token);
        }else if(getEtype(token)==NON){
            qDebug()<<token<<" is not a var\n";
            return QStringList{"fal","se"};//要有报错方式
        }
    }
    while(!stack.isEmpty()){
        posfix.append(stack.pop());
    }
    // qDebug()<<"achieve infix to postfix"<<"\n";
    return posfix;
}

/*
  计算后缀表达式的值
 */
int MainWindow::evaluate(QStringList postfix){
    QStack<int> stack; // 用于存储操作数的栈
    for (const QString& token : postfix) {
        // 如果是数字，直接入栈
        bool ok;
        token.toInt(&ok);
        if (ok) {
            stack.push(token.toInt());
        }else if(getEtype(token)==VAR){
            stack.push(priMap[token]);
        }
        // 如果是运算符，弹出栈顶的两个操作数进行计算
        else {
            int val1 = stack.pop(); // 弹出栈顶的第一个操作数
            int val2 = stack.pop(); // 弹出栈顶的第二个操作数
            if (token == "+") {
                stack.push(val2 + val1); // 加法
            } else if (token == "-") {
                stack.push(val2 - val1); // 减法
            } else if (token == "*") {
                stack.push(val2 * val1); // 乘法
            } else if (token == "/") {
                if (val1 == 0) {
                    throw std::runtime_error("Division by zero"); // 除零错误
                }
                stack.push(val2 / val1); // 除法
            } else if (token == "**") {
                stack.push(std::pow(val2, val1)); // 幂运算
            } else if (token == "MOD") {
                if (val1 == 0) {
                    throw std::runtime_error("Modulo by zero"); // 取模零错误
                }
                stack.push(code->mod(val2, val1)); // 取模运算（自定义规则）
            }
        }
    }

    return stack.pop(); // 返回最终结果
}

/*
  获取表达式的类型
 */
MainWindow::Exp_Type MainWindow::getEtype(QString exp){
    bool ok;
    QChar f=' ';
    if(exp.size()>1&&exp[0]=='-'){
        f = '-';
        exp.remove(0,1);
    }
    exp.toInt(&ok);
    if(ok){
        return NUM;
    }
    if(code->isOp(exp)){
        return OP;
    }
    if(exp=="="){
        return ASS;
    }
    bool isvar=false;
    for(const QString &it: priVar){
        if(exp==it){
            isvar=true;
            break;
        }
    }
    if(isvar)
        return VAR;
    return NON;
}

/*
  处理INPUT命令
 */
void MainWindow::INPUT(QString var){
    try{
        bool get = false;
        int Val = ui->cmdLineEdit->text().toInt(&get);
        if(get){
            if(!priVar.contains(var)){
                priVar.append(var);
                priMap[var] = Val;
            }else{
                priMap[var]=Val;
            }
            QString in = "INPUT "+var+" = "+QString::number(Val);
            ui->resultDisplay->append(in);
            ui->cmdLineEdit->clear();
        }else{
            QMessageBox::critical(nullptr,"Error","In INPUT: Invalid input");
            qDebug()<<"In INPUT: Invalid input\n";
        }
    }catch(QString error){
        //接收失败
    }
    ifINPUT=false;
    // 断开连接
    disconnect(connection);
}

/*
  检查表达式中是否包含无效字符
 */
bool MainWindow::conInvaildChar(QStringList exp){
    for(int i = 0;i<exp.size();i++){
        QString n = exp[i];
        if(n[0]=='-'&&n.size()>1){
            // 如果负号后面是数字，则视为有效
            /*QString numPart = n.mid(1);*/ // 去掉负号部分
            n.remove(0,1);
            bool ok;
            n.toInt(&ok); // 判断负号后的部分是否是有效数字
            if (!ok) {
                return true; // 如果不是有效数字，返回true表示无效
            }
            // n.remove(0,1);
        }
        QRegularExpression regex("[^a-zA-Z0-9]");
        QRegularExpressionMatch match = regex.match(n);
        if (match.hasMatch()) {
            if(!(n.size()==1||n=="**"))
                return true;
        }
    }
    return false;
}
/*保存文件*/
void MainWindow::saveCodeToFile()
{
    // 打开文件对话框，选择保存文件的路径和名称
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Code File"), "", tr("Text Files (*.txt);;All Files (*)"));

    if (!fileName.isEmpty()) { // 如果用户选择了文件路径
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) { // 以写入模式打开文件
            QTextStream out(&file);
            QString code = ui->CodeDisplay->toPlainText(); // 获取 CodeDisplay 中的文本
            out << code; // 将文本写入文件
            file.close(); // 关闭文件
        } else {
            // 如果无法打开文件，给出提示
            QMessageBox::warning(this, tr("Error"), tr("Cannot save file"));
        }
    }
}
