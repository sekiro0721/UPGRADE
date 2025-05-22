#include "course.h"
#include <iostream>

using namespace std;

// ���֪ͨ
void Course::addNotification(string notification) {
    notifications.push_back(notification);
    cout << "֪ͨ�ѷ���: " << notification << endl;
}

// ��ʾ����֪ͨ
void Course::displayNotifications() const {
    if (notifications.empty()) {
        cout << "����֪ͨ" << endl;
        return;
    }

    cout << "\n=== " << courseName << " �γ�֪ͨ ===" << endl;
    for (size_t i = 0; i < notifications.size(); i++) {
        cout << i + 1 << ". " << notifications[i] << endl;
    }
}

// ��ӳɼ�
bool Course::addGrade(string studentId, double grade) {
    // ���ɼ���Χ
    if (grade < 0 || grade > 100) {
        cout << "�ɼ���Ч��������0-100֮��" << endl;
        return false;
    }

    // ����Ƿ��Ѵ��ڸ�ѧ���ɼ�
    for (auto& g : grades) {
        if (g.first == studentId) {
            cout << "��ѧ���ɼ��Ѵ��ڣ������޸���ʹ�ø��¹���" << endl;
            return false;
        }
    }

    grades.push_back(make_pair(studentId, grade));
    return true;
}

// ���³ɼ�
bool Course::updateGrade(string studentId, double newGrade) {
    // ���ɼ���Χ
    if (newGrade < 0 || newGrade > 100) {
        cout << "�ɼ���Ч��������0-100֮��" << endl;
        return false;
    }

    for (auto& g : grades) {
        if (g.first == studentId) {
            g.second = newGrade;
            return true;
        }
    }

    cout << "δ�ҵ���ѧ���ɼ�" << endl;
    return false;
}

// ��ȡѧ���ɼ�
double Course::getStudentGrade(string studentId) const {
    for (const auto& g : grades) {
        if (g.first == studentId) {
            return g.second;
        }
    }
    return -1; // ��ʾδ�ҵ�
}

// ����Ƿ������ѧ��
bool Course::hasStudent(string studentId) const {
    for (const auto& g : grades) {
        if (g.first == studentId) {
            return true;
        }
    }
    return false;
}