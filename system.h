#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "user.h"
#include "course.h"
#include "grade.h"

using namespace std;

// ϵͳ������
class ManagementSystem {
private:
    vector<shared_ptr<User>> users;
    vector<shared_ptr<Course>> courses;
    GradeManager gradeManager;
    string dataPath;

    // ���������������û�
    shared_ptr<User> findUser(string username) const;

    // �������������ҿγ�
    shared_ptr<Course> findCourse(string courseId) const;

    // ������������ȡѧ�������пγ�
    vector<shared_ptr<Course>> getStudentCourses(string studentId) const;

    // ����ͼ�������
    void saveUsers() const;
    void loadUsers();
    void saveCourses() const;
    void loadCourses();
    void saveGrades() const;
    void loadGrades();

public:
    ManagementSystem(string path = "./data/") : dataPath(path) {}

    // ϵͳ��ʼ��
    void initializeSystem();

    // �û���֤
    shared_ptr<User> login(string username, string password) const;

    // ��ʾ�û��˵�
    void displayUserMenu(shared_ptr<User> user);

    // ����Ա����
    void manageUsers();
    void manageCourses();
    void reviewGrades();
    void maintainData();

    // ��ʦ����
    void viewTeachingCourses(shared_ptr<Teacher> teacher);
    void enterOrModifyGrades(shared_ptr<Teacher> teacher);
    void publishNotification(shared_ptr<Teacher> teacher);
    void queryClassStatistics(shared_ptr<Teacher> teacher);

    // ѧ������
    void viewPersonalInfo(shared_ptr<Student> student);
    void viewCourseSchedule(shared_ptr<Student> student);
    void viewGradeDetails(shared_ptr<Student> student);
    void receiveNotifications(shared_ptr<Student> student);
    void submitReviewRequest(shared_ptr<Student> student);
};

#endif // SYSTEM_H    