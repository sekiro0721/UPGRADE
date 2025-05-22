#include "user.h"
#include "course.h"
#include "system.h"
#include <iostream>

using namespace std;

// ����Ա�˵�
void Administrator::displayMenu() const {
    cout << "\n===== ��ӭ������Ա " << name << " =====" << endl;
    cout << "1. �û�����" << endl;
    cout << "2. �γ̹���" << endl;
    cout << "3. �ɼ����" << endl;
    cout << "4. ����ά��" << endl;
    cout << "5. �˳���¼" << endl;
}

// ��ʦ�˵�
void Teacher::displayMenu() const {
    cout << "\n===== ��ӭ����ʦ " << name << " =====" << endl;
    cout << "1. �鿴���ڿγ�" << endl;
    cout << "2. ¼��/�޸ĳɼ�" << endl;
    cout << "3. �����γ�֪ͨ" << endl;
    cout << "4. ��ѯͳ�ư༶�ɼ�" << endl;
    cout << "5. �˳���¼" << endl;
}

// ѧ���˵�
void Student::displayMenu() const {
    cout << "\n===== ��ӭ��ѧ�� " << name << " =====" << endl;
    cout << "1. �鿴������Ϣ" << endl;
    cout << "2. �鿴�γ̱�" << endl;
    cout << "3. �鿴�ɼ�����" << endl;
    cout << "4. ����֪ͨ" << endl;
    cout << "5. �ύ�ɼ���������" << endl;
    cout << "6. �˳���¼" << endl;
}

// ��ӿγ̵���ʦ
void Teacher::addCourse(shared_ptr<Course> course) {
    courses.push_back(course);
}

// ��ӿγ̵�ѧ��
void Student::addCourse(shared_ptr<Course> course) {
    courses.push_back(course);
}