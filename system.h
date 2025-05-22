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

// 系统管理类
class ManagementSystem {
private:
    vector<shared_ptr<User>> users;
    vector<shared_ptr<Course>> courses;
    GradeManager gradeManager;
    string dataPath;

    // 辅助函数：查找用户
    shared_ptr<User> findUser(string username) const;

    // 辅助函数：查找课程
    shared_ptr<Course> findCourse(string courseId) const;

    // 辅助函数：获取学生的所有课程
    vector<shared_ptr<Course>> getStudentCourses(string studentId) const;

    // 保存和加载数据
    void saveUsers() const;
    void loadUsers();
    void saveCourses() const;
    void loadCourses();
    void saveGrades() const;
    void loadGrades();

public:
    ManagementSystem(string path = "./data/") : dataPath(path) {}

    // 系统初始化
    void initializeSystem();

    // 用户认证
    shared_ptr<User> login(string username, string password) const;

    // 显示用户菜单
    void displayUserMenu(shared_ptr<User> user);

    // 管理员功能
    void manageUsers();
    void manageCourses();
    void reviewGrades();
    void maintainData();

    // 教师功能
    void viewTeachingCourses(shared_ptr<Teacher> teacher);
    void enterOrModifyGrades(shared_ptr<Teacher> teacher);
    void publishNotification(shared_ptr<Teacher> teacher);
    void queryClassStatistics(shared_ptr<Teacher> teacher);

    // 学生功能
    void viewPersonalInfo(shared_ptr<Student> student);
    void viewCourseSchedule(shared_ptr<Student> student);
    void viewGradeDetails(shared_ptr<Student> student);
    void receiveNotifications(shared_ptr<Student> student);
    void submitReviewRequest(shared_ptr<Student> student);
};

#endif // SYSTEM_H    