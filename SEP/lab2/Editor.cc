#include <iostream>
#include <sstream>
#include "Editor.h"

using namespace std;

Editor::Editor()
{
    buffer = new Buffer();
}
Editor::~Editor()
{
    // TODO: Implement destructor
    buffer->~Buffer();
}

void Editor::run()
{
    string cmd;
    while (true)
    {
        cout << "cmd> ";
        cout.flush();
        getline(cin, cmd);
        if (cmd == "Q")
            break;
        try {
            dispatchCmd(cmd);
        } catch (const char *e) {
            cout << "? " << e << endl;
        } catch (const out_of_range &oor) {
            cout << "? " << oor.what() << endl;
        } catch (const range_error &re) {
            cout << "? " << re.what() << endl;
        }
    }
}
void Editor::cmdAppend()
{
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    // TODO: finish cmdAppend.
    while (1)
    {
        string t;
        getline(cin, t);
        if (t == ".")
            break;
        else{
            buffer->appendLine(t);
        }
    }
}

void Editor::cmdInsert()
{
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    bool firstLine = true;
    while (1)
    {
        string t;
        getline(cin, t);
        if (t == ".")
            break;
        if (firstLine) {
            buffer->insertLine(t);
            firstLine = false;
        }  else {
            buffer->appendLine(t);
        }
    }
}

void Editor::cmdDelete(int start, int end)
{
    buffer->deleteLines(start, end);
}

void Editor::cmdNull(int line)
{
    cout << buffer->moveToLine(line) << endl;
}

void Editor::cmdNumber(int start, int end)
{
    buffer->showLines(start, end);
}

void Editor::cmdWrite(const string &filename)
{
    buffer->writeToFile(filename);
}


void Editor::dispatchCmd(const string &cmd)
{
    if (cmd == "a") {
        cmdAppend();
        return;
    }
    if (cmd == "i") {
        cmdInsert();
        return;
    }
    if (cmd[0] == 'w' && cmd[1] == ' ') {
        // TODO: call cmdWrite with proper arguments
        cmdWrite(cmd.substr(2,cmd.size()-2));
        return;
    }
    // TODO: handle special case "1,$n".
    if(cmd=="1,$n"){
        cmdNumber(1, buffer->getcurrentLineNum());
        return;
    }
    int start, end;
    char comma, type = ' ';
    stringstream ss(cmd);
    ss >> start;
    if (ss.eof()) {
        cmdNull(start);
        return;
    }
    ss >> comma >> end >> type;
    if (ss.good()) {
        if (type == 'n') {
            cmdNumber(start, end);
            return;
        } else if (type == 'd') {
            cmdDelete(start, end);
            return;
        }  else if (type == 'l') {
            // for test, please use ?,?l -> ?,?s -> ?,?t
            buffer->loadAddr2(start+1, end+1);
            return;
        } else if (type == 't') {
            buffer->testSwap(start+1, end+1);
            return;
        }else if (type == 's') {
            buffer->Swap(start,end);
            return;
        }
    }
    throw "Bad/Unknown command";
}
