#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <limits>
#include <ctime>
#include <algorithm>
#include "user.h"
#include "course.h"
#include "grade.h"
#include "system.h"

using namespace std;

// ����������������뻺����
void clearInputBuffer1() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    // ��ʼ��ϵͳ
    ManagementSystem system;
    system.initializeSystem();

    // ���˵�
    while (true) {
        cout << "\n===== ѧ���ɼ�����ϵͳ =====" << endl;
        cout << "1. ��¼" << endl;
        cout << "2. �˳�ϵͳ" << endl;
        cout << "��ѡ�����: ";

        int choice;
        while (!(cin >> choice)) {
            clearInputBuffer1();
            cout << "������Ч������������: ";
        }
        clearInputBuffer1();

        switch (choice) {
        case 1: {
            string username, password;
            cout << "�û���: ";
            getline(cin, username);
            cout << "����: ";
            getline(cin, password);

            shared_ptr<User> currentUser = system.login(username, password);
            if (currentUser) {
                system.displayUserMenu(currentUser);
            }
            else {
                cout << "��¼ʧ�ܣ��û������������" << endl;
            }
            break;
        }
        case 2:
            cout << "��лʹ��ѧ���ɼ�����ϵͳ���ټ���" << endl;
            return 0;
        default:
            cout << "��Чѡ�����������룡" << endl;
        }
    }

    return 0;
}