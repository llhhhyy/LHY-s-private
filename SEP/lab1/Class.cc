#include "Class.h"
#include <string>
#include "Student.h"

using namespace std;

void Class::addStudent(const Student& st) {
    StudentWrapper sw(st.id, st);
    students.push_back(sw);
}

StudentWrapper& Class::getStudentWrapper(const std::string& studentId) {
    for (std::vector<StudentWrapper>::iterator it = students.begin();
            it != students.end();
            ++ it) {
        if (it->id == studentId)
            return *it;
    }
    throw "No match student!";
}
double Class::getHighestScore() {
    // TODO implement getHighestScore
    if(students.size()==0)return 0.0;
    else
    {
        double max=0;
        for(int i=0;i<students.size();i++)
        {
            double sci=students[i].getScore();
            if(sci>max)max=sci;
        }
        return max;
    }
}
