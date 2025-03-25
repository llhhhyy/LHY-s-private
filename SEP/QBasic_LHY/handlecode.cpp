#include "handlecode.h"
#include <QString>
// 构造函数，初始化成员变量
HandleCode::HandleCode(QObject *parent, QLineEdit* cmd, QLineEdit* input, QTextBrowser* res, QTextBrowser* breakpoint, QTextBrowser* monitor)
    : QObject{parent}, cmdLineEdit(cmd), inputLineEdit(input), resultDisplay(res), breakPointDisplay(breakpoint), monitorDisplay(monitor), line(0), ifEND(false) {}

// 查找指定行号的代码行
bool HandleCode::findLine(QString tar) {
    bool find = false;
    for (int i = 0; i < CodeList.size(); i++) {
        QString it = CodeList[i];
        QRegularExpression re("^(\\d+)"); // 匹配句首的数字
        QRegularExpressionMatch match = re.match(it);
        if (match.hasMatch()) {
            if (match.captured(1) == tar) {
                line = i;
                find = true;
                break;
            }
        }
    }
    return find;
}

// 重置代码列表和相关状态
void HandleCode::remake(QStringList code) {
    resultDisplay->clear();
    CodeList = code;
    line = 0;
    ifEND = false;
    breakPoint.clear();
    breakline.clear();
    varList.clear();
    varMap.clear();
}

// 重置运行状态
void HandleCode::remake() {
    resultDisplay->clear();
    ifEND = false;
    line = 0;
    varList.clear();
    varMap.clear();
}

// 获取表达式的类型
HandleCode::Exp_Type HandleCode::getEtype(QString exp) {
    bool ok;
    QChar f=' ';
    if(exp.size()>1&&exp[0]=='-'){
        f = '-';
        exp.remove(0,1);
    }
    exp.toInt(&ok);
    if (ok) {
        return NUM;
    }
    if (isOp(exp)) {
        return OP;
    }
    if (exp == "=") {
        return ASS;
    }
    bool isvar = false;
    for (const QString &it : varList) {
        if (exp == it) {
            isvar = true;
            break;
        }
    }
    if (isvar)
        return VAR;
    return NON;
}

// 判断是否为操作符
bool HandleCode::isOp(QString token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "**" || token == "MOD";
}

// 处理REM指令
void HandleCode::handleREM(bool &ok) {
    line++;
    return;
}

// 处理END指令
void HandleCode::handleEND(bool &ok) {
    ok = false;
    ifEND = true;
    return;
}

// 处理GOTO指令
void HandleCode::handleGOTO(bool &ok) {
    QString go = CodeList[line];
    QStringList goList = go.split(" ");
    QString tar = goList.last();
    ok = findLine(tar);
    if(!ok){
        QMessageBox::critical(nullptr,"Error","There is no the line of "+tar);
    }
}

// 处理IF指令
void HandleCode::handleIF(bool &ok) {
    QString If = CodeList[line];
    QStringList ifList = If.split(" ");
    ifList.removeFirst();
    ifList.removeFirst();
    ifList.removeAll("THEN");
    QString tar = ifList.last();
    ifList.removeLast();
    int n = 0;
    for (; n < ifList.size(); n++) {
        QString token = ifList[n];
        if (token == ">" || token == "<" || token == "=") {
            break;
        }
    }
    if (n == ifList.size() - 1) {
        ok = false;
        QMessageBox::critical(nullptr, "Error", "In IF sentence: no compare op.");
        return;
    }
    QString comop = ifList[n];
    QStringList inexp1 = ifList.mid(0, n);
    QStringList inexp2 = ifList.mid(n + 1);
    int res1 = calculateExp(inexp1);
    int res2 = calculateExp(inexp2);
    if (res1 != -999 && res2 != -999) {
        ok = true;
        if (comop == ">") {
            if (res1 > res2) {
                ok = findLine(tar);
                if(!ok){
                    QMessageBox::critical(nullptr,"Error","There is no the line of "+tar);
                    return;
                }
            } else line++;

        } else if (comop == "<") {
            if (res1 < res2) {
                ok = findLine(tar);
                if(!ok){
                    QMessageBox::critical(nullptr,"Error","There is no the line of "+tar);
                    return;
                }
            } else line++;
        } else if (comop == "=") {
            if (res1 == res2) {
                ok = findLine(tar);
                if(!ok){
                    QMessageBox::critical(nullptr,"Error","There is no the line of "+tar);
                    return;
                }
            } else line++;
        }
    } else {
        ok = false;
        QMessageBox::critical(nullptr, "Error", "In IF sentence: calculate false.");
    }
}

// 处理INPUT指令
void HandleCode::handleINPUT(bool &ok) {
    inputLineEdit->setPlaceholderText(" ? ");
    inputLineEdit->setFocus(); // 让输入框获得焦点

    // 创建一个事件循环来等待用户输入并按下回车键
    QEventLoop loop;
    QObject::connect(inputLineEdit, &QLineEdit::returnPressed, &loop, &QEventLoop::quit);
    loop.exec(); // 启动事件循环，等待用户输入并按下回车键

    // 用户按下回车键后，获取输入内容
    QString input = inputLineEdit->text();
    if (!input.isEmpty()) {
        bool get = false;
        int val = input.toInt(&get);
        if (get) {
            QString in = CodeList[line];
            QStringList inList = in.split(" ");
            QString inVar = inList.last();
            if (!varList.contains(inVar)) {
                varList.append(inVar);
                varMap[inVar] = val;
            } else {
                varMap[inVar] = val;
            }
            line++;
            ok = true;
        } else {
            QMessageBox::critical(nullptr, "Error", "In INPUT sentence: Invalid input");
            qDebug() << "In INPUT: Invalid input\n";
            ok = false;
        }
    } else {
        QMessageBox::critical(nullptr, "Error", "In INPUT sentence: No input provided");
        qDebug() << "In INPUT: No input provided\n";
        ok = false;
    }

    inputLineEdit->clear();
}

// 处理LET指令
void HandleCode::handleLET(bool &ok) {
    QString let = CodeList[line];
    QStringList letList = let.split(" ");
    letList.removeFirst();
    letList.removeFirst();
    QString var = letList[0];
    letList.removeFirst();
    letList.removeFirst();
    int val = calculateExp(letList);
    if (val != -999) {
        if (!varList.contains(var)) {
            varList.append(var);
            varMap[var] = val;
        } else {
            varMap[var] = val;
        }
        ok = true;
    } else {
        QMessageBox::critical(nullptr, "Error", "In LET sentence: calculate false.");
        qDebug() << "In LET: calculate false\n";
        ok = false;
    }
    if (ok)
        line++;
}

// 处理PRINT指令
void HandleCode::handlePRINT(bool &ok) {
    QString print = CodeList[line];
    QStringList printList = print.split(" ");
    printList.removeFirst();
    printList.removeFirst();
    if (printList.size() < 2) {
        QString var = printList[0];
        if (varList.contains(var)) {
            int val = varMap[var];
            QString valstr = QString::number(val);
            resultDisplay->append(valstr);
            ok = true;
        } else {
            QMessageBox::critical(nullptr, "Error", "In PRINT sentence: is not a var in this program.");
            qDebug() << var << "In PRINT: is not in this program\n";
            ok = false;
        }
    } else {
        int res = calculateExp(printList);
        if (res != -999) {
            QString resstr = QString::number(res);
            resultDisplay->append(resstr);
            ok = true;
        } else {
            QMessageBox::critical(nullptr, "Error", "In PRINT sentence: calculate false.");
            qDebug() << "In PRINT: calculate false\n";
            ok = false;
        }
    }
    if (ok)
        line++;
}

// 处理代码行
void HandleCode::handleCode(bool &ok) {
    if(line>CodeList.size()){
        ifEND = true;
        return;
    }
    QString code = CodeList[line];
    QStringList thisline = code.split(" ");

    thisline.removeFirst();
    QString type = thisline[0];
    // if (type != "REM") {
    //     bool invalid = conInvaildChar(thisline);
    //     if (invalid) {
    //         QMessageBox::critical(nullptr, "Error", QString::number(line) + " at code has invalid code.");
    //         ok = false;
    //         return;
    //     }
    // }
    if (type == "REM") {
        handleREM(ok);
    } else if (type == "LET") {
        handleLET(ok);
    } else if (type == "IF") {
        handleIF(ok);
    } else if (type == "INPUT") {
        handleINPUT(ok);
    } else if (type == "PRINT") {
        handlePRINT(ok);
    } else if (type == "END") {
        handleEND(ok);
    } else if (type == "GOTO") {
        handleGOTO(ok);
    } else {
        QMessageBox::critical(nullptr, "Error", "In RUN: Invalid command.");
        qDebug() << "In RUN: Invaild command\n";
        ifEND = true;
        ok = false;
    }
    // if(!ok){
    //     QMessageBox::critical(nullptr,"Error","There is error in the codes.");
    // }
}

// 获取操作符优先级
int HandleCode::precedence(QString op) {
    if (op == "**") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    if (op == "MOD") return 2;
    return 0;
}

// 计算表达式的值
int HandleCode::calculateExp(QStringList exp) {
    QStringList postfix = infixToPostfix(exp);
    if (postfix != QStringList{"fal", "se"}) {
        return evaluate(postfix);
    }
    return -999;
}

// 取模运算
int HandleCode::mod(int a, int b) {
    int result = a % b;
    if ((result < 0 && b > 0) || (result > 0 && b < 0)) {
        result += b;
    }
    return result;
}

// 将中缀表达式转换为后缀表达式
QStringList HandleCode::infixToPostfix(QStringList infix) {
    QStringList posfix;
    QStack<QString> stack;

    for (int i = 0; i < infix.size(); ++i) {
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
        } else if (token == "(") { // 左括号
            stack.push(token);
        } else if (token == ")") { // 右括号
            while (!stack.isEmpty() && stack.top() != "(") {
                posfix.append(stack.pop());
            }
            if (!stack.isEmpty() && stack.top() == "(") {
                stack.pop();
            }
        } else if (isOp(token)) { // 操作符
            while (!stack.isEmpty() && isOp(stack.top()) && precedence(stack.top()) >= precedence(token)) {
                posfix.append(stack.pop());
            }
            stack.push(token);
        } else if (token == "=") { // 赋值等号
            stack.push(token);
        } else if (getEtype(token) == VAR) { // 变量
            posfix.append(token);
        } else if (getEtype(token) == NON) {
            qDebug() << token << " is not a var\n";
            return QStringList{"fal", "se"}; // 要有报错方式
        }
    }
    while (!stack.isEmpty()) {
        posfix.append(stack.pop());
    }
    // qDebug() << "achieve infix to postfix" << "\n";
    for(const QString& n: posfix){
        qDebug()<<n<<" ";
    }
    qDebug()<<"\n";
    return posfix;
}

// 计算后缀表达式的值
int HandleCode::evaluate(QStringList postfix) {
    QStack<int> stack; // 用于存储操作数的栈
    for (int i = 0;i<postfix.size();i++) {
        QString token = postfix[i];
        // 如果是数字，直接入栈
        bool ok;
        QChar f =' ';
        if(token.size()>1&&token[0]=='-'){
            f = '-';
            token.remove(0,1);
            token.toInt(&ok);
        }else
            token.toInt(&ok);
        int value;
        if(f=='-'){
            value = 0-token.toInt(&ok);
            token = "-"+token;
        }else value = token.toInt(&ok);

        if (ok) {
            stack.push(value);
        } else if (getEtype(token) == VAR) {
            stack.push(varMap[token]);
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
                stack.push(mod(val2, val1)); // 取模运算（自定义规则）
            }
        }
    }

    return stack.pop(); // 返回最终结果
}

// 运行代码
void HandleCode::RUN(bool &ok) {
    if (ifEND) {
        resultDisplay->clear();
        ifEND = false;
        line = 0;
        varList.clear();
        varMap.clear();
    }
    if (!ifDebug) {
        while (ok) {
            handleCode(ok);
        }
        getEND();
    } else {
        rankBreakPoint();
        continueExe();
    }
    inputLineEdit->setVisible(false);
}

// 清除所有代码和状态
void HandleCode::clear() {
    CodeList.clear();
    breakPoint.clear();
    varList.clear();
    varMap.clear();
    line = 0;
    displayBreakPoint();
}

// 添加断点
void HandleCode::addBreakPoint(QString b) {
    bool find = false;
    for (const QString &it : breakPoint) {
        if (b == it) {
            QMessageBox::information(nullptr, "Tip", "This breakpoint has existed.");
            qDebug() << "The breakpoint " << b << " has existed\n";
            return;
        }
    }
    for (int i = 0; i < CodeList.size(); i++) {
        QString it = CodeList[i];
        QRegularExpression re("^(\\d+)"); // 匹配句首的数字
        QRegularExpressionMatch match = re.match(it);
        if (match.hasMatch()) {
            if (match.captured(1) == b) {
                breakPoint.append(b);
                find = true;
                break;
            }
        }
    }
    if (!find) {
        // 报错
        QMessageBox::critical(nullptr, "Error", "This line doesn't existed.");
        qDebug() << "There isn't the line " << b << "\n";
    }
    findBreakLine();
    displayBreakPoint();
}

// 删除断点
void HandleCode::deleteBreakPoint(QString b) {
    bool find = false;
    for (int i = 0; i < CodeList.size(); i++) {
        QString it = CodeList[i];
        QRegularExpression re("^(\\d+)"); // 匹配句首的数字
        QRegularExpressionMatch match = re.match(it);
        if (match.hasMatch()) {
            if (match.captured(1) == b) {
                breakPoint.removeAll(b);
                find = true;
                break;
            }
        }
    }
    if (!find) {
        // 报错
        QMessageBox::critical(nullptr, "Error", "This breakpoint doesn't existed.");
        qDebug() << "There isn't a breakpoint at " << b << "\n";
    }
    findBreakLine();
    displayBreakPoint();
}

// 显示断点
void HandleCode::displayBreakPoint() {
    breakPointDisplay->clear();
    if (breakPoint.isEmpty()) return;
    for (const QString &token : breakPoint) {
        breakPointDisplay->append(token);
    }
}

// 查找断点对应的代码行
void HandleCode::findBreakLine() {
    if (!breakline.isEmpty())
        breakline.clear();
    for (int i = 0; i < CodeList.size(); i++) {
        QString it = CodeList[i];
        QRegularExpression re("^(\\d+)");
        QRegularExpressionMatch match = re.match(it);
        if (match.hasMatch()) {
            for (const QString &token : breakPoint) {
                if (token == match.captured(1)) {
                    breakline.append(i);
                }
            }
        }
    }
}

// 改变断点位置
void HandleCode::changeBreakPos(int pos) {
    QStringList bstrList = breakPointDisplay->toPlainText().split("\n");
    if (!bstrList.isEmpty()) {
        if (bstrList[bstrList.size() - 1][0].isDigit()) {
            QString nextText = "Stop at " + QString::number(pos);
            breakPointDisplay->append(nextText);
        } else {
            bstrList[bstrList.size() - 1] = "Stop at " + QString::number(pos);
            QString newText = bstrList.join("\n");
            breakPointDisplay->setPlainText(newText);
        }
    }
}

// 执行代码
void HandleCode::exeCode(bool &con) {
    while (con) {
        handleCode(con);
        if (!breakline.isEmpty()) {
            for (int i = 0; i < breakline.size(); i++) {
                int it = breakline[i];
                int pos = breakPoint[i].toInt();
                if (line == it) {
                    con = false;
                    changeBreakPos(pos);
                }
            }
        }
    }
    showMonitor();
}

// 继续执行代码
void HandleCode::continueExe() {
    bool con = true;
    while (con) {
        exeCode(con);
    }
    getEND();
}

// 获取结束状态
void HandleCode::getEND() {
    if (ifEND) {
        line = 0;
        varList.clear();
        varMap.clear();
    }
}

// 显示监控信息
void HandleCode::showMonitor() {
    monitorDisplay->clear();
    for (int i = 0; i < varList.size(); i++) {
        QString v = varList[i] + " : " + QString::number(varMap[varList[i]]) + "\n";
        monitorDisplay->append(v);
    }
}

// 停止运行
void HandleCode::stopRun() {
    resultDisplay->clear();
    monitorDisplay->clear();
    line = 0;
    ifEND = false;
    varList.clear();
    varMap.clear();
}

// 对断点进行排序
void HandleCode::rankBreakPoint() {
    std::sort(breakline.begin(), breakline.end());
    std::sort(breakPoint.begin(), breakPoint.end(), [](const QString &s1, const QString &s2) {
        return s1.toInt() < s2.toInt(); // 比较两个字符串的整数值
    });
}

// 检查表达式中是否包含无效字符
bool HandleCode::conInvaildChar(QStringList exp) {
    for (const QString &n : exp) {
        if(n[0]=='-'&&n.size()>1){
            // 如果负号后面是数字，则视为有效
            QString numPart = n.mid(1); // 去掉负号部分
            bool ok;
            numPart.toInt(&ok); // 判断负号后的部分是否是有效数字
            if (!ok) {
                return true; // 如果不是有效数字，返回true表示无效
            }
        }
        QRegularExpression regex("[^a-zA-Z0-9]");
        QRegularExpressionMatch match = regex.match(n);
        if (match.hasMatch()) {
            if (!(n.size() == 1 || n == "**"))
                return true;
        }
    }
    return false;
}
