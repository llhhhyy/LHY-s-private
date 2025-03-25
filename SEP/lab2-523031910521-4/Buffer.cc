#include <iostream>
#include <fstream>
#include "Buffer.h"

using namespace std;
//TODO: your code here
//implement the functions in Buffer

Buffer::Buffer() {
    currentLineNum=0;
    headLine=new Line;
    currentLine=headLine;
}

Buffer::~Buffer() {
    Line *p=headLine->next,*q;
    currentLine=headLine;
    delete headLine;
    while(p->next!= nullptr){
        q=p->next;
        delete p;
        p=q;
    }
    currentLineNum=0;
}

void Buffer::writeToFile(const string &filename) const {
    size_t t=0;
    ofstream offile;
    Line* p=headLine;
    offile.open(filename);
    if(!offile) {
        throw "Filename not specified";
    }
    for(int i=0;i<currentLineNum;i++){
        p=p->next;
        offile<<p->line;
        t+=(p->line).length()+1;
    }
    cout<<t<<" byte(s) written"<<endl;
}

void Buffer::showLines(int from, int to) {
    if(from>to) throw "Number range error";
    if(from<=0||to>currentLineNum) throw "Line number out of range";
    Line* p=headLine;
    for(int i=0;i<to;i++){
        p=p->next;
        if(i>=from-1){
            cout<<i+1<<"\t"<<p->line<<endl;
            cout<<p->line.size()<<endl;
        }
    }
    currentLine=p;
}

void Buffer::deleteLines(int from, int to){
    if(from>to) throw "Delete range error";
    if(from<=0||to>currentLineNum) throw "Line number out of range";
    Line* p=headLine;
    Line* tmp;
    for(int i=0;i<to;i++){
        p=p->next;
        if(i>from-2&&i!=to-1){
            tmp=p;
            p=p->prev;
            p->next=tmp->next;
            p=p->next;
            p->prev=tmp->prev;
            p=p->prev;
            delete tmp;
        }
        else if(i==to-1){
            if(p->next== NULL){
                tmp=p;
                p=p->prev;
                p->next= NULL;
                delete tmp;
            }
            else{
                tmp=p;
                p=p->prev;
                p->next=tmp->next;
                p=p->next;
                p->prev=tmp->prev;
                delete tmp;
            }
        }
    }
    currentLine=p;
    currentLineNum-=to-from+1;
}

void Buffer::insertLine(const string &text){
    Line *p=new Line;
    p->line=text;
    if(currentLine!=headLine){
        Line* tmp= nullptr;
        tmp=currentLine;
        currentLine=currentLine->prev;
        p->prev=currentLine;
        p->next=tmp;
        currentLine->next=p;
        tmp->prev=p;
        currentLine=currentLine->next;
        currentLineNum++;
    }
    else{
        currentLine->next=p;
        p->prev=currentLine;
        currentLine=currentLine->next;
        currentLineNum++;
    }

}

void Buffer::appendLine(const string &text){
    Line *p=new Line;
    p->line=text;
    if(currentLine==headLine||currentLine->next==NULL){
        currentLine->next=p;
        p->prev=currentLine;
        currentLine=currentLine->next;
        currentLineNum++;
    }
    else{
        Line* tmp= currentLine->next;
        p->prev=currentLine;
        p->next=tmp;
        currentLine->next=p;
        tmp->prev=p;
        currentLine=currentLine->next;
        currentLineNum++;
    }
}

const string &Buffer::moveToLine(int idx)  {
    if(idx>currentLineNum||idx<=0) throw "Line number out of range";
    Line *p=headLine;
    for(int i=0;i<currentLineNum;i++){
        p=p->next;
        if(idx==i+1){
            currentLine=p;
            return currentLine->line;
        }
    }
}
void Buffer::Swap(int one, int another) {
    if(one<=0||another<=0||one>currentLineNum||another>currentLineNum) throw "Line number out of range";
    Line *p1=headLine;
    Line *p2=headLine;
    Line *tmp;
    int i=0;
    if(one==another) return;
    while(i<std::max(one, another)){
        p2=p2->next;
        if(i<std::min(one, another)){
            p1=p2;
        }
        i++;
    }
    if(another!=currentLineNum){
        tmp=p1->next;
        p1->next=p2->next;
        p2->next->prev=p1;
        tmp->prev=p2;
        p2->next=tmp;
        tmp=p1->prev;
        p1->prev=p2->prev;
        p2->prev->next=p1;
        tmp->next=p2;
        p2->prev=tmp;
    }
    else{
        tmp=p1->next;
        tmp->prev=p2;
        p2->next=tmp;
        tmp=p1->prev;
        p1->prev=p2->prev;
        p2->prev->next=p1;
        tmp->next=p2;
        p2->prev=tmp;
    }

}
int Buffer::getcurrentLineNum() const{
    return currentLineNum;
}
// for test, Don't modify
static void* aPtr = nullptr;
static void* bPtr = nullptr;

void Buffer::printAddr(int idx) const {

    int curLineNo = 1;
    Line *curLine = headLine;
    while (curLineNo < idx) {
        curLineNo += 1;
        curLine = curLine->next;
    }

    std::cout << idx << ":" << curLine << std::endl;
}

void Buffer::loadAddr2(int one, int another) const {
    if (one == another)
        return;

    int first = std::min(one, another);
    int second = std::max(one, another);

    Line *firstLine = nullptr, *secondLine = nullptr;

    int curLineNo = 1;
    Line *curLine = headLine;
    while (curLineNo <= second) {
        if (curLineNo == first)
            firstLine = curLine;
        if (curLineNo == second)
            secondLine = curLine;

        curLineNo += 1;
        curLine = curLine->next;
    }

    if (firstLine && secondLine) {
        aPtr = (void *)firstLine;
        bPtr = (void *)secondLine;
    }
}

void Buffer::testSwap(int one, int another) const {

    if (one == another)
        return;

    int first = std::min(one, another);
    int second = std::max(one, another);

    Line *firstLine = nullptr, *secondLine = nullptr;

    int curLineNo = 1;
    Line *curLine = headLine;
    while (curLineNo <= second) {
        if (curLineNo == first)
            firstLine = curLine;
        if (curLineNo == second)
            secondLine = curLine;

        curLineNo += 1;
        curLine = curLine->next;
    }

    if (firstLine && secondLine) {
        if (aPtr == secondLine && bPtr == firstLine)
            std::cout << "Swap 2 Nodes successfully!" << std::endl;
        else
            std::cout << "Swap 2 Nodes failed!" << std::endl;
        aPtr = nullptr;
        bPtr = nullptr;
    }
}