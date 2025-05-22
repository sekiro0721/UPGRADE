#ifndef GRADE_H
#define GRADE_H

#include <string>
#include <vector>
#include <memory>
#include "user.h"
#include "course.h"

using namespace std;

// �ɼ���
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

// �ɼ�������
class GradeManager {
private:
    vector<shared_ptr<Grade>> grades;

public:
    // ��ӳɼ�
    bool addGrade(string studentId, string courseId, double score);

    // ���³ɼ�
    bool updateGrade(string studentId, string courseId, double newScore);

    // ��ȡѧ��ĳ�γ̳ɼ�
    double getStudentGrade(string studentId, string courseId) const;

    // �ύ�ɼ���������
    bool submitReviewRequest(string studentId, string courseId, string reason);

    // ����ɼ���������
    bool processReviewRequest(string studentId, string courseId, double newScore);

    // ��ȡ���д���˵ĳɼ�
    vector<shared_ptr<Grade>> getPendingReviews() const;

    // �ɼ��Ƿ�������������ȷ����
    bool isGradeFinalized(string studentId, string courseId) const;
};

#endif // GRADE_H    