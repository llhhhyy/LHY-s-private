#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Class.h"
#include "Student.h"

using namespace std;

class AppX {
private:
    vector<Student *> studentVec;
    vector<Class *> classVec;

    void loadFiles();
    void inputScore();
    void printHighestScore();
    void printGrade();

public:
    AppX();
    ~AppX();
    int run();
};

AppX::~AppX()
{
    // You can use the traditional loop, which is more clear.
    for (vector<Class *>::iterator it = classVec.begin();
         it != classVec.end();
         ++it) {
        if (*it) delete (*it);
    }
    // You can use modern and simpler loops only if you know what it is doing.
    for (const auto &s: studentVec) {
        if (s) delete (s);
    }
}

AppX::AppX()
{
    loadFiles();
}

void AppX::loadFiles()
{
    string line;
    size_t pos1, pos2;
    vector<string> bufv;
    Student *st = nullptr;
    string clsname;
    int point;
    Class *cl = nullptr;

    // Open a file as a input stream.
    ifstream stfile("./Students.txt");

    while (getline(stfile, line)) {
        if (line.empty()) // It's an empty line.
            continue;
        if (line[0] == '#') // It's a comment line.
            continue;

        // The bufv vector stores each column in the line.
        bufv.clear();
        // Split the line into columns.
        //   pos1: beginning of the column.
        //   pos2: end of the column.
        pos1 = 0;
        while (true) {
            pos2 = line.find(';', pos1 + 1);
            if (pos2 == string::npos) { // No more columns.
                // Save the last column (pos1 ~ eol).
                bufv.push_back(line.substr(pos1, string::npos));
                break;
            } else {
                // Save the column (pos1 ~ pos2).
                bufv.push_back(line.substr(pos1, pos2 - pos1));
            }
            pos1 = pos2 + 1;
        }

        // TODO: uncomment next lines after implementing class Undergraduate
        // and Graduate.

        if (bufv[3] == "U")
            st = new Undergraduate(bufv[0], bufv[1], bufv[2]);
        else
            st = new Graduate(bufv[0], bufv[1], bufv[2]);

        studentVec.push_back(st);
    }
    stfile.close();
    bufv.clear();
    // TODO: load data from ./Classes.txt and push objects to the vector.
    ifstream stfile1("./Classes.txt");
    while (getline(stfile1, line)) {

        if (line.empty()) // It's an empty line.
        {
            point = int(bufv[1][0] - '0');
            cl = new Class(bufv[0], point);
            for (int i = 2; i < bufv.size(); i++) {
                for (int j = 0; j < studentVec.size(); j++) {
                    if (bufv[i] == studentVec[j]->id) {
                        st = studentVec[j];
                        cl->addStudent(*st);
                        break;
                    }
                }
            }
            classVec.push_back(cl);
            bufv.clear();
            continue;
        }
        if (line[0] == '#') // It's a comment line.
            continue;


        pos1 = 0;
        pos2 = line.find(':', pos1 + 1);
        if (pos2 != string::npos) {
            bufv.push_back(line.substr(pos2 + 1));
        } else {
            bufv.push_back(line.substr(pos1, string::npos));
        }
    }
    point = int(bufv[1][0] - '0');
    cl = new Class(bufv[0], point);
    for (int i = 2; i < bufv.size(); i++) {
        for (int j = 0; j < studentVec.size(); j++) {
            if (bufv[i] == studentVec[j]->id) {
                st = studentVec[j];
                cl->addStudent(*st);
                break;
            }
        }
    }
    classVec.push_back(cl);
    bufv.clear();
    stfile1.close();
    //还要把学生所修的课程录进去
    ifstream stfile2("./Classes.txt");
    int m=0;
    while(getline(stfile2, line))
    {
        string idd;
        if (line.empty())
        {
            m++;
            continue;
        }
        if (line[0] == '#')
            continue;
        pos1 = 0;
        idd=line.substr(pos1, string::npos);
        for(int i=0;i<studentVec.size();i++)
        {
               if(idd==studentVec[i]->id)
               {studentVec[i]->addClass(classVec[m]);}
        }
    }
}
void opera(string &a,double &num,bool &flag){

    num=atof(a.c_str());
    if(num>100)flag=false;
}
void AppX::inputScore()
{
    // TODO: implement inputScore.
    // Hint: Take a look at printHighestScore().
    string name,line;
    while(cin>>name&&name!="q"){
        Class* cl= nullptr;
        for (std::vector<Class*>::iterator it=classVec.begin();it!=classVec.end();it++) {
            if((*it)->name==name){
                cl=*it;
                cout<<"Please input the student id and score(or input q to quit): ";
                while(cin>>line&&line!="q"){
                    size_t pos1=0,pos2;
                    pos2=line.find(',',0);
                    string id=line.substr(pos1,pos2);
                    string poi=line.substr(pos2+1,string::npos);
                    double num=0;bool flag=true;
                    opera(poi,num,flag);
                    if(flag==false){
                        cout<<"Wrong score!"<<endl;
                        continue;
                    }
                    int cnt=0;
                    for(std::vector<StudentWrapper>::iterator ptr=(*it)->students.begin();
                        ptr!=(*it)->students.end();
                        ptr++){
                        if(id==ptr->id){
                            try {
                                ptr->setScore(num);
                                break;
                            }
                            catch(char const*) {
                                cout<<"Wrong score!"<<endl;
                            }

                        }
                        else cnt++;
                    }
                    if(cnt==(*it)->students.size()){
                        cout<<"No match student!"<<endl;
                    }
                }
            }
        }
        if(cl== nullptr){
            cout<<"No match class!"<<endl;
        }
    }
    }


void AppX::printHighestScore()
{
    string sbuf;
    Class *cl;
    double highest;

    while (true) {
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;
        bool f=false;
        for(int i=0;i<classVec.size();i++)
        {
            if(sbuf==classVec[i]->name)
                f=true;

        }
        if(f==false)
        {
            cout<<"No match class!"<<endl;
            break;
        }
        cl = nullptr;
        for (vector<Class*>::iterator it = classVec.begin();
                it != classVec.end();
                ++ it) {
            if ((*it)->name == sbuf) {
                cl = *it;
                break;
            }
        }
        if (cl == nullptr) {
            cout << "No match class!" << endl;
            continue;
        }

        highest = cl->getHighestScore();
        cout << setiosflags(ios::fixed) << setprecision(2)<< "The highest score is: " << highest << endl;
    }
}

void AppX::printGrade()
{
    // TODO: implement printGrade.
    // Hint: Take a look at printHighestScore().
    string Id;

    while (true) {
        cout << "Please input the id of the student (or input q to quit): ";
        cin>>Id;
        if(Id=="q")break;
        bool f=false;
        for(int i=0;i<studentVec.size();i++)
        {
            if(studentVec[i]->id==Id)
            {
                f=true;
                double g=studentVec[i]->getGpa();
                double a=studentVec[i]->getAvgScore();
                cout<<studentVec[i]->toString()<<endl;
                cout<<"GPA,AVG = "<<setiosflags(ios::fixed)<<setprecision(2)<<g<<","<<a<<endl;
            }
        }
        if(f==false){
            cout<<"No match student!"<<endl;
            continue;
        }
    }
}

int AppX::run()
{
    char cmd;
    while (true) {
        cout << "Command menu:\n"
            << "\ti: Input score\n"
            << "\ta: Compute highest score of a class\n"
            << "\tg: Compute grade of a student\n"
            << "\tq: Quit\n"
            << "Please input the command: ";
        cin >> cmd;
        if (cmd == 'i') {
            inputScore();
        } else if (cmd == 'a') {
            printHighestScore();
        } else if (cmd == 'g') {
            printGrade();
        } else if (cmd == 'q') {
            break;
        } else {
            cout << "Invalid command!\n" << endl;
        }
    }
    return 0;
}

int main()
{
    AppX app;
    return app.run();
}
