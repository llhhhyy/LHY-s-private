#include "syntaxtree.h"
#include "qregularexpression.h"

// 构造函数，初始化根节点为空
SyntaxTree::SyntaxTree():sRoot(NULL) {}

// 根据表达式的类型返回对应的枚举值
SyntaxTree::Exp_Type SyntaxTree::getEtype(QString exp){
    bool ok;
    if(exp.size()>1&&exp[0]=='-'){
        exp.remove(0,1);
    }
    exp.toInt(&ok);
    if(ok){
        return NUM; // 如果是数字
    }
    if(isOp(exp)){
        return OP; // 如果是操作符
    }
    if(exp=="="){
        return ASS; // 如果是赋值符号
    }
    return VAR; // 否则认为是变量
}

// 根据语句的类型返回对应的枚举值
SyntaxTree::Sent_Type SyntaxTree::getStype(QString sent){
    if(sent == "LET"){
        return LET; // 赋值语句
    }else if(sent == "IF"){
        return IF; // 条件语句
    }else if(sent == "GOTO"){
        return GOTO; // 跳转语句
    }else if(sent == "PRINT"){
        return PRINT; // 打印语句
    }
    return OTHER; // 其他语句
}

// 获取操作符的优先级
int SyntaxTree::getPrecedence(QString op){
    if (op == "**") return 3; // 幂运算优先级最高
    if (op == "*" || op == "/") return 2; // 乘除优先级次之
    if (op == "+" || op == "-") return 1; // 加减优先级最低
    if (op == "MOD") return 2; // 取模优先级与乘除相同
    return -1; // 非运算符
}

// 判断是否为操作符
bool SyntaxTree::isOp(QString token){
    return token=="+"||token=="-"||token=="*"||token=="/"||token=="**"||token=="MOD";
}

// 将中缀表达式转换为后缀表达式（逆波兰表达式）
QStringList SyntaxTree::infixToPostfix(QStringList infix){
    QStringList posfix;
    QStack<QString> stack;

    for (int i = 0; i < infix.size(); ++i) {
        QString token = infix[i];
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
            posfix.append(token);  // 如果是数字，直接加入后缀表达式
        }else if (token == "(") {//左括号
            stack.push(token);
        } else if (token == ")") {//右括号
            while (!stack.isEmpty() && stack.top() != "(") {
                posfix.append(stack.pop()); // 弹出栈中的操作符直到遇到左括号
            }
            if (!stack.isEmpty() && stack.top() == "(") {
                stack.pop(); // 弹出左括号
            }
        } else if (isOp(token)) {//操作符
            while (!stack.isEmpty() && isOp(stack.top()) && getPrecedence(stack.top()) >= getPrecedence(token)) {
                posfix.append(stack.pop()); // 弹出栈中优先级大于等于当前操作符的操作符
            }
            stack.push(token); // 将当前操作符压入栈
        } else if (token == "=") {//赋值等号
            stack.push(token); // 赋值符号直接压入栈
        }else if(getEtype(token)==VAR){//变量
            posfix.append(token); // 变量直接加入后缀表达式
        }
    }
    while(!stack.isEmpty()){
        posfix.append(stack.pop()); // 将栈中剩余的操作符加入后缀表达式
    }
    return posfix;
}

// 根据后缀表达式构建表达式树
SyntaxTree::ExpNode* SyntaxTree::constructExpTree(QStringList postfix){
    QStack<ExpNode*> stack;
    for(const QString &token:postfix){
        if(getEtype(token)==NUM){
            stack.push(new ExpNode(token,NUM)); // 数字节点
        }else if(getEtype(token)==VAR){
            stack.push(new ExpNode(token,VAR)); // 变量节点
        }else if(getEtype(token)==OP){
            ExpNode* right = stack.pop(); // 弹出右操作数
            ExpNode* left = stack.pop(); // 弹出左操作数
            ExpNode* node = new ExpNode(token,OP);
            node->left = left;
            node->right = right;
            stack.push(node); // 将操作符节点压入栈
        }else if(getEtype(token)==ASS){
            ExpNode *right = stack.pop(); // 弹出右操作数
            ExpNode *left = stack.pop(); // 弹出左操作数
            ExpNode *node = new ExpNode(token,ASS);
            node->left = left;
            node->right = right;
            stack.push(node); // 将赋值节点压入栈
        }
    }
    qDebug()<<"achieve construct"<<"\n";
    return stack.pop(); // 返回根节点
}

// 创建语法树
void SyntaxTree::creatTree(QStringList words,bool &ok){
    QString sent = words[0];
    // if(sent!="REM"){
    //     bool invalid = conInvaildChar(words);
    //     if(invalid){
    //         QMessageBox::critical(nullptr,"Error","Create tree false, because it has invalid code.") ;
    //         return;
    //     }
    // }
    if(sent=="LET"){
        SentNode* newnode=new SentNode(sent,LET);
        sRoot = newnode;
        words.removeFirst();
        QStringList postfix=infixToPostfix(words);
        ExpNode* eRoot=constructExpTree(postfix);
        sRoot->childs.append(eRoot);
        ok=1;
    }else if(sent=="IF"){
        sent = "IF THEN";
        SentNode* newnode=new SentNode(sent,IF);
        sRoot = newnode;
        words.removeFirst();
        words.removeAll("THEN");
        int n=0;
        for(;n<words.size();n++){
            QString token=words[n];
            if(token==">"||token=="<"||token=="="){
                break;
            }
        }
        QStringList inexp1 = words.mid(0,n);
        QStringList inexp2 = words.mid(n+1);
        QString tar = inexp2.last();
        inexp2.removeLast();
        QStringList postexp1 = infixToPostfix(inexp1);
        QStringList postexp2 = infixToPostfix(inexp2);
        ExpNode* exp1 = constructExpTree(postexp1);
        ExpNode* compareop = new ExpNode(words[n],COM);
        ExpNode* exp2 = constructExpTree(postexp2);
        ExpNode* target = new ExpNode(tar,NUM);
        sRoot->childs.append(exp1);
        sRoot->childs.append(compareop);
        sRoot->childs.append(exp2);
        sRoot->childs.append(target);
        ok=1;
    }else if(sent=="GOTO"){
        SentNode* newnode = new SentNode(sent,GOTO);
        sRoot = newnode;
        words.removeFirst();
        ExpNode* line = new ExpNode(words[0],NUM);
        sRoot->childs.append(line);
        ok=1;
    }else if(sent=="PRINT"){
        SentNode* newnode = new SentNode(sent,PRINT);
        sRoot = newnode;
        words.removeFirst();
        QStringList postfix=infixToPostfix(words);
        ExpNode *eRoot = constructExpTree(postfix);
        sRoot->childs.append(eRoot);
        ok=1;
    }else{
        ok=false;
    }
}

// 递归显示表达式树
QString SyntaxTree::displayTree(ExpNode* node, int indent){
    QString result;
    if (node == nullptr) {
        result += "\n";
        return result;
    }
    for (int i = 0; i < indent; ++i) {
        result += "    "; // 每个缩进级别使用4个空格
    }

    result+=node->val+"\n";
    if(node->left!=nullptr){
        result+=displayTree(node->left,indent+1);
    }
    if(node->right!=nullptr){
        result+=displayTree(node->right,indent+1);
    }
    return result;
}

// 打印语法树
QString SyntaxTree::printTree(){
    if(sRoot==NULL){
        return "Empty tree\n";
    }
    QString result;
    if(sRoot->type==LET){
        result = sRoot->sent+displayTree(sRoot->childs[0],0);
    }else if(sRoot->type==IF){
        result = sRoot->sent+"\n    "
                 +displayTree(sRoot->childs[0],0)+"    "
                 +sRoot->childs[1]->val+"\n    "
                 +displayTree(sRoot->childs[2],0)+"    "
                 +sRoot->childs[3]->val;
    }else if(sRoot->type==GOTO){
        result =  sRoot->sent+"\n    "+displayTree(sRoot->childs[0],0);
    }else if(sRoot->type==PRINT){
        result =  sRoot->sent+"\n    "+displayTree(sRoot->childs[0],0);
    }
    return result;
}

// 析构函数，释放树的内存
SyntaxTree::~SyntaxTree() {
    deleteTree(sRoot);
}

// 递归删除语句节点
void SyntaxTree::deleteTree(SentNode* node) {
    if (node == nullptr) return;
    for (ExpNode* child : node->childs) {
        deleteTree(child);
    }
    delete node;
}

// 递归删除表达式节点
void SyntaxTree::deleteTree(ExpNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// 检查表达式中是否包含无效字符
bool SyntaxTree::conInvaildChar(QStringList exp){
    for(int i = 0;i<exp.size();i++){
        QString n = exp[i];
        if(n[0]=='-'&&n.size()>1){
            // 如果负号后面是数字，则视为有效
            QString numPart = n.mid(1); // 去掉负号部分
            bool ok;
            numPart.toInt(&ok); // 判断负号后的部分是否是有效数字
            if (!ok) {
                return true; // 如果不是有效数字，返回true表示无效
            }
            n.remove(0,1);
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
