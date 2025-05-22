#include "grade.h"
#include <iostream>

using namespace std;

// ��ӳɼ�
bool GradeManager::addGrade(string studentId, string courseId, double score) {
    // ���ɼ��Ƿ��Ѵ���
    for (const auto& grade : grades) {
        if (grade->getStudentId() == studentId && grade->getCourseId() == courseId) {
            cout << "��ѧ���ĸÿγ̳ɼ��Ѵ���" << endl;
            return false;
        }
    }

    auto newGrade = make_shared<Grade>(studentId, courseId, score);
    grades.push_back(newGrade);
    return true;
}

// ���³ɼ�
bool GradeManager::updateGrade(string studentId, string courseId, double newScore) {
    for (const auto& grade : grades) {
        if (grade->getStudentId() == studentId && grade->getCourseId() == courseId) {
            // ���ɼ��Ƿ�������
            if (grade->getIsFinalized()) {
                cout << "�óɼ����������޷�ֱ���޸ģ����ύ�������" << endl;
                return false;
            }

            grade->setScore(newScore);
            return true;
        }
    }

    cout << "δ�ҵ���ѧ���ĸÿγ̳ɼ�" << endl;
    return false;
}

// ��ȡѧ��ĳ�γ̳ɼ�
double GradeManager::getStudentGrade(string studentId, string courseId) const {
    for (const auto& grade : grades) {
        if (grade->getStudentId() == studentId && grade->getCourseId() == courseId) {
            return grade->getScore();
        }
    }
    return -1; // ��ʾδ�ҵ�
}

// �ύ�ɼ���������
bool GradeManager::submitReviewRequest(string studentId, string courseId, string reason) {
    for (const auto& grade : grades) {
        if (grade->getStudentId() == studentId && grade->getCourseId() == courseId) {
            // ����Ƿ����������
            if (grade->getIsUnderReview()) {
                cout << "�óɼ����������" << endl;
                return false;
            }

            grade->setIsUnderReview(true);
            grade->setReviewReason(reason);
            cout << "�ɼ������������ύ������: " << reason << endl;
            return true;
        }
    }

    cout << "δ�ҵ���ѧ���ĸÿγ̳ɼ�" << endl;
    return false;
}

// ����ɼ���������
bool GradeManager::processReviewRequest(string studentId, string courseId, double newScore) {
    for (const auto& grade : grades) {
        if (grade->getStudentId() == studentId && grade->getCourseId() == courseId &&
            grade->getIsUnderReview()) {
            grade->setScore(newScore);
            grade->setIsUnderReview(false);
            grade->setIsFinalized(true); // ����������ɼ�
            cout << "�ɼ������Ѵ����³ɼ�: " << newScore << endl;
            return true;
        }
    }

    cout << "δ�ҵ�����˵ĸ�ѧ���ĸÿγ̳ɼ�" << endl;
    return false;
}

// ��ȡ���д���˵ĳɼ�
vector<shared_ptr<Grade>> GradeManager::getPendingReviews() const {
    vector<shared_ptr<Grade>> pendingReviews;
    for (const auto& grade : grades) {
        if (grade->getIsUnderReview()) {
            pendingReviews.push_back(grade);
        }
    }
    return pendingReviews;
}

// �ɼ��Ƿ�������
bool GradeManager::isGradeFinalized(string studentId, string courseId) const {
    for (const auto& grade : grades) {
        if (grade->getStudentId() == studentId && grade->getCourseId() == courseId) {
            return grade->getIsFinalized();
        }
    }
    return false; // ����ɼ������ڣ�����false
}