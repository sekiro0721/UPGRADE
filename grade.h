#ifndef GRADE_H
#define GRADE_H

#include <string>
#include <vector>
#include <memory>
#include "user.h"
#include "course.h"

using namespace std;

// 成绩类
class Grade {
private:
    string studentId;
    string courseId;
    double score;
    bool isFinalized;
    bool isUnderReview;
    string reviewReason;

public:
    Grade(string studentId, string courseId, double score)
        : studentId(studentId), courseId(courseId), score(score),
        isFinalized(false), isUnderReview(false), reviewReason("") {
    }

    // Getters
    string getStudentId() const { return studentId; }
    string getCourseId() const { return courseId; }
    double getScore() const { return score; }
    bool getIsFinalized() const { return isFinalized; }
    bool getIsUnderReview() const { return isUnderReview; }
    string getReviewReason() const { return reviewReason; }

    // Setters
    void setScore(double newScore) { score = newScore; }
    void setIsFinalized(bool finalized) { isFinalized = finalized; }
    void setIsUnderReview(bool review) { isUnderReview = review; }
    void setReviewReason(string reason) { reviewReason = reason; }
};

// 成绩管理类
class GradeManager {
private:
    vector<shared_ptr<Grade>> grades;

public:
    // 添加成绩
    bool addGrade(string studentId, string courseId, double score);

    // 更新成绩
    bool updateGrade(string studentId, string courseId, double newScore);

    // 获取学生某课程成绩
    double getStudentGrade(string studentId, string courseId) const;

    // 提交成绩复核申请
    bool submitReviewRequest(string studentId, string courseId, string reason);

    // 处理成绩复核申请
    bool processReviewRequest(string studentId, string courseId, double newScore);

    // 获取所有待审核的成绩
    vector<shared_ptr<Grade>> getPendingReviews() const;

    // 成绩是否已锁定（最终确定）
    bool isGradeFinalized(string studentId, string courseId) const;
};

#endif // GRADE_H    