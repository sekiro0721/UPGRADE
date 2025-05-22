#include "course.h"
#include <iostream>

using namespace std;

// 添加通知
void Course::addNotification(string notification) {
    notifications.push_back(notification);
    cout << "通知已发布: " << notification << endl;
}

// 显示所有通知
void Course::displayNotifications() const {
    if (notifications.empty()) {
        cout << "暂无通知" << endl;
        return;
    }

    cout << "\n=== " << courseName << " 课程通知 ===" << endl;
    for (size_t i = 0; i < notifications.size(); i++) {
        cout << i + 1 << ". " << notifications[i] << endl;
    }
}

// 添加成绩
bool Course::addGrade(string studentId, double grade) {
    // 检查成绩范围
    if (grade < 0 || grade > 100) {
        cout << "成绩无效，必须在0-100之间" << endl;
        return false;
    }

    // 检查是否已存在该学生成绩
    for (auto& g : grades) {
        if (g.first == studentId) {
            cout << "该学生成绩已存在，如需修改请使用更新功能" << endl;
            return false;
        }
    }

    grades.push_back(make_pair(studentId, grade));
    return true;
}

// 更新成绩
bool Course::updateGrade(string studentId, double newGrade) {
    // 检查成绩范围
    if (newGrade < 0 || newGrade > 100) {
        cout << "成绩无效，必须在0-100之间" << endl;
        return false;
    }

    for (auto& g : grades) {
        if (g.first == studentId) {
            g.second = newGrade;
            return true;
        }
    }

    cout << "未找到该学生成绩" << endl;
    return false;
}

// 获取学生成绩
double Course::getStudentGrade(string studentId) const {
    for (const auto& g : grades) {
        if (g.first == studentId) {
            return g.second;
        }
    }
    return -1; // 表示未找到
}

// 检查是否包含该学生
bool Course::hasStudent(string studentId) const {
    for (const auto& g : grades) {
        if (g.first == studentId) {
            return true;
        }
    }
    return false;
}