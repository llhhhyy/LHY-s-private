#pragma once

#include <string>

using std::string;

class Line {
public:
    Line * prev;
    Line * next;
    string line;
    Line(){
        prev=NULL;
        next=NULL;
    }
};

class Buffer {
private:
    int currentLineNum;
    Line *headLine, *currentLine;

public:
    Buffer();
    ~Buffer();

    void writeToFile(const string &filename) const;

    // When we need `const`?
    const string &moveToLine(int idx);
    void showLines(int from, int to);

    void deleteLines(int from, int to);
    void insertLine(const string &text);
    void appendLine(const string &text);
    int getcurrentLineNum() const;
    void Swap(int one,int another);
    // for test
    void printAddr(int idx) const;
    void loadAddr2(int one, int another) const;
    void testSwap(int one, int another) const;
};
