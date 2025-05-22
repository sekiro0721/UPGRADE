#include "user.h"
#include "course.h"
#include "system.h"
#include <iostream>

using namespace std;

// 管理员菜单
void Administrator::displayMenu() const {
    cout << "\n===== 欢迎，管理员 " << name << " =====" << endl;
    cout << "1. 用户管理" << endl;
    cout << "2. 课程管理" << endl;
    cout << "3. 成绩审核" << endl;
    cout << "4. 数据维护" << endl;
    cout << "5. 退出登录" << endl;
}

// 教师菜单
void Teacher::displayMenu() const {
    cout << "\n===== 欢迎，教师 " << name << " =====" << endl;
    cout << "1. 查看所授课程" << endl;
    cout << "2. 录入/修改成绩" << endl;
    cout << "3. 发布课程通知" << endl;
    cout << "4. 查询统计班级成绩" << endl;
    cout << "5. 退出登录" << endl;
}

// 学生菜单
void Student::displayMenu() const {
    cout << "\n===== 欢迎，学生 " << name << " =====" << endl;
    cout << "1. 查看个人信息" << endl;
    cout << "2. 查看课程表" << endl;
    cout << "3. 查看成绩详情" << endl;
    cout << "4. 接收通知" << endl;
    cout << "5. 提交成绩复核申请" << endl;
    cout << "6. 退出登录" << endl;
}

// 添加课程到教师
void Teacher::addCourse(shared_ptr<Course> course) {
    courses.push_back(course);
}

// 添加课程到学生
void Student::addCourse(shared_ptr<Course> course) {
    courses.push_back(course);
}