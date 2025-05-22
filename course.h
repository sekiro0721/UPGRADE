#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <memory>
#include "user.h"

using namespace std;

// 课程类
class Course {
private:
    string courseId;
    string courseName;
    int credits;
    string semester;
    shared_ptr<Teacher> teacher;
    string className;
    vector<string> notifications;
    vector<pair<string, double>> grades; // 学生ID和成绩

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

    // 通知管理
    void addNotification(string notification);
    void displayNotifications() const;

    // 成绩管理
    bool addGrade(string studentId, double grade);
    bool updateGrade(string studentId, double newGrade);
    double getStudentGrade(string studentId) const;
    bool hasStudent(string studentId) const;
};

#endif // COURSE_H    