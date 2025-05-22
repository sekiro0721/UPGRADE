#include "grade.h"
#include <iostream>

using namespace std;

// 添加成绩
bool GradeManager::addGrade(string studentId, string courseId, double score) {
    // 检查成绩是否已存在
    for (const auto& grade : grades) {
        if (grade->getStudentId() == studentId && grade->getCourseId() == courseId) {
            cout << "该学生的该课程成绩已存在" << endl;
            return false;
        }
    }

    auto newGrade = make_shared<Grade>(studentId, courseId, score);
    grades.push_back(newGrade);
    return true;
}

// 更新成绩
bool GradeManager::updateGrade(string studentId, string courseId, double newScore) {
    for (const auto& grade : grades) {
        if (grade->getStudentId() == studentId && grade->getCourseId() == courseId) {
            // 检查成绩是否已锁定
            if (grade->getIsFinalized()) {
                cout << "该成绩已锁定，无法直接修改，请提交审核申请" << endl;
                return false;
            }

            grade->setScore(newScore);
            return true;
        }
    }

    cout << "未找到该学生的该课程成绩" << endl;
    return false;
}

// 获取学生某课程成绩
double GradeManager::getStudentGrade(string studentId, string courseId) const {
    for (const auto& grade : grades) {
        if (grade->getStudentId() == studentId && grade->getCourseId() == courseId) {
            return grade->getScore();
        }
    }
    return -1; // 表示未找到
}

// 提交成绩复核申请
bool GradeManager::submitReviewRequest(string studentId, string courseId, string reason) {
    for (const auto& grade : grades) {
        if (grade->getStudentId() == studentId && grade->getCourseId() == courseId) {
            // 检查是否已在审核中
            if (grade->getIsUnderReview()) {
                cout << "该成绩已在审核中" << endl;
                return false;
            }

            grade->setIsUnderReview(true);
            grade->setReviewReason(reason);
            cout << "成绩复核申请已提交，理由: " << reason << endl;
            return true;
        }
    }

    cout << "未找到该学生的该课程成绩" << endl;
    return false;
}

// 处理成绩复核申请
bool GradeManager::processReviewRequest(string studentId, string courseId, double newScore) {
    for (const auto& grade : grades) {
        if (grade->getStudentId() == studentId && grade->getCourseId() == courseId &&
            grade->getIsUnderReview()) {
            grade->setScore(newScore);
            grade->setIsUnderReview(false);
            grade->setIsFinalized(true); // 处理后锁定成绩
            cout << "成绩复核已处理，新成绩: " << newScore << endl;
            return true;
        }
    }

    cout << "未找到待审核的该学生的该课程成绩" << endl;
    return false;
}

// 获取所有待审核的成绩
vector<shared_ptr<Grade>> GradeManager::getPendingReviews() const {
    vector<shared_ptr<Grade>> pendingReviews;
    for (const auto& grade : grades) {
        if (grade->getIsUnderReview()) {
            pendingReviews.push_back(grade);
        }
    }
    return pendingReviews;
}

// 成绩是否已锁定
bool GradeManager::isGradeFinalized(string studentId, string courseId) const {
    for (const auto& grade : grades) {
        if (grade->getStudentId() == studentId && grade->getCourseId() == courseId) {
            return grade->getIsFinalized();
        }
    }
    return false; // 如果成绩不存在，返回false
}