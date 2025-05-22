#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <memory>
#include "user.h"

using namespace std;

// �γ���
class Course {
private:
    string courseId;
    string courseName;
    int credits;
    string semester;
    shared_ptr<Teacher> teacher;
    string className;
    vector<string> notifications;
    vector<pair<string, double>> grades; // ѧ��ID�ͳɼ�

public:
    Course(string courseId, string courseName, int credits, string semester,
        shared_ptr<Teacher> teacher, string className)
        : courseId(courseId), courseName(courseName), credits(credits),
        semester(semester), teacher(teacher), className(className) {
    }

    // Getters
    string getCourseId() const { return courseId; }
    string getCourseName() const { return courseName; }
    int getCredits() const { return credits; }
    string getSemester() const { return semester; }
    shared_ptr<Teacher> getTeacher() const { return teacher; }
    string getClassName() const { return className; }
    vector<string> getNotifications() const { return notifications; }
    vector<pair<string, double>> getGrades() const { return grades; }

    // Setters
    void setTeacher(shared_ptr<Teacher> newTeacher) { teacher = newTeacher; }

    // ֪ͨ����
    void addNotification(string notification);
    void displayNotifications() const;

    // �ɼ�����
    bool addGrade(string studentId, double grade);
    bool updateGrade(string studentId, double newGrade);
    double getStudentGrade(string studentId) const;
    bool hasStudent(string studentId) const;
};

#endif // COURSE_H    