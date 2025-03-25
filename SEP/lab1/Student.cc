#include "Student.h"
#include <string>
#include <sstream>
#include "Class.h"

using namespace std;

string Student::toString() const
{
    // TODO: uncomment the following code after implementing class Student.
    ostringstream oss;
    oss << "Student Information:"
        << "\n\tid: " << id
        << "\n\tname: " << name
        << "\n\tenrollment year: " << year
        << "\n\tdegree: " << (degree == graduate ? "graduate" : "undergraduate")
        << std::endl;
    return oss.str();
}

// TODO: implement functions which are declared in Student.h.
void Student::changedegreeg() {
    degree=graduate;
}
void Student::changedegreeu() {
    degree=undergraduate;
}
void Student::addClass(Class *c)
{
    cla.push_back(c);
}
double Graduate::getGpa() {
    double pointsum=0;
    for(int i=0;i<cla.size();i++)
    {
        pointsum+=cla[i]->point;
    }
    double gpasum=0;
    double gpa[5]={4.0,3.5,3.0,2.5,2.0};
    for(int i=0;i<cla.size();i++)
    {
        StudentWrapper st=cla[i]->getStudentWrapper(this->id);
        double sc=st.getScore();
        if(sc<=100&&sc>=90)gpasum+=cla[i]->point*gpa[0]/pointsum;
        if(sc<=89&&sc>=80)gpasum+=cla[i]->point*gpa[1]/pointsum;
        if(sc<=79&&sc>=70)gpasum+=cla[i]->point*gpa[2]/pointsum;
        if(sc<=69&&sc>=60)gpasum+=cla[i]->point*gpa[3]/pointsum;
        if(sc<60)gpasum+=cla[i]->point*gpa[4]/pointsum;
    }
    double Gpa=gpasum/cla.size()*2;
    return Gpa;
}
double Graduate::getAvgScore() {
    double pointsum=0;
    for(int i=0;i<cla.size();i++)
    {
        pointsum+=cla[i]->point;
    }
    double avgscoresum=0;
    for(int i=0;i<cla.size();i++)
    {
        StudentWrapper st=cla[i]->getStudentWrapper(this->id);
        avgscoresum+=st.getScore()*(cla[i]->point)/pointsum;
    }
    double avgscore=avgscoresum/cla.size()*2;
    return avgscore;
}
double Undergraduate::getGpa(){
    double pointsum=0;
    for(int i=0;i<cla.size();i++)
    {
        pointsum+=cla[i]->point;
    }
    double gpasum=0;
    for(int i=0;i<cla.size();i++)
    {
        StudentWrapper st=cla[i]->getStudentWrapper(this->id);
        gpasum+=(st.getScore()/20)*(cla[i]->point)/pointsum;
    }
    double gpa=gpasum/cla.size()*2;
    return gpa;
}
double Undergraduate::getAvgScore() {
    double pointsum=0;
    for(int i=0;i<cla.size();i++)
    {
        pointsum+=cla[i]->point;
    }
    double avgscoresum=0;
    for(int i=0;i<cla.size();i++)
    {
        StudentWrapper st=cla[i]->getStudentWrapper(this->id);
        avgscoresum+=st.getScore()*(cla[i]->point)/pointsum;
    }
    double avgscore=avgscoresum/cla.size()*2;
    return avgscore;
}