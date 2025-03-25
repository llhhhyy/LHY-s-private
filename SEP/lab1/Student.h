#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>
#include <vector>

using namespace std;
class Class;

enum Degree {
    undergraduate,
    graduate
};

class Student {
    // TODO: implement class Student.
private:
    string name;
    string year;
    enum Degree degree;
protected:
    vector<Class*> cla;
public:
    const string id;
    Student(string &i,string &nam,string &yea):id(i)
    {
        name=nam;
        year=yea;
    }
    Student(const Student &student)
    {
        name=student.name;
        year=student.year;
        degree=student.degree;
        cla.assign(student.cla.begin(),student.cla.end());
    }
    void changedegreeg();
    void changedegreeu();
    string toString() const;
    virtual double getGpa()=0;
    virtual double getAvgScore()=0;
    void addClass(Class* c);
};

// TODO: implement class Graduate.
class Graduate:public Student{
public:
    Graduate(string &i,string &nam,string &yea):Student(i,nam,yea)
    {
         changedegreeg();
    }
    double getGpa();
    double getAvgScore();
};
// TODO: implement class Undergraduate.
class Undergraduate:public Student{
public:
    Undergraduate(string &i,string &nam,string &yea):Student(i,nam,yea)
    {
        changedegreeu();
    }

    double getGpa();
    double getAvgScore();
};
class StudentWrapper {
private:
    const Student &student;
    double score=0.0;
public:
    const string id;
    // TODO: fix error
    StudentWrapper(const string &i, const Student  &studen) :id(i),student(studen)
    {}
    void setScore(double scor)
    {
        if (scor < 0 || scor > 100)
            throw "Invalid Score!";
        this->score = scor;
    }

    double getScore() const
    {
        return this->score;
    }

    string toString() const
    {
        return student.toString();
    }
    };

#endif // STUDENT_H_
