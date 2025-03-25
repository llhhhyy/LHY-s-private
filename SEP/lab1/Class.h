#ifndef CLASS_H_
#define CLASS_H_

#include "Student.h"
#include <string>
#include <vector>
using namespace std;

class Class {
private:


public:
    vector<StudentWrapper> students;
    const string name;
    const int point;
    Class(const string &name, const int point)
        : name(name), point(point) { }

    void addStudent(const Student &st);
    StudentWrapper &getStudentWrapper(const string &studentId);
    double getHighestScore();
};

#endif // CLASS_H_
