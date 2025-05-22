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

// 辅助函数：清除输入缓冲区
void clearInputBuffer1() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    // 初始化系统
    ManagementSystem system;
    system.initializeSystem();

    // 主菜单
    while (true) {
        cout << "\n===== 学生成绩管理系统 =====" << endl;
        cout << "1. 登录" << endl;
        cout << "2. 退出系统" << endl;
        cout << "请选择操作: ";

        int choice;
        while (!(cin >> choice)) {
            clearInputBuffer1();
            cout << "输入无效，请输入数字: ";
        }
        clearInputBuffer1();

        switch (choice) {
        case 1: {
            string username, password;
            cout << "用户名: ";
            getline(cin, username);
            cout << "密码: ";
            getline(cin, password);

            shared_ptr<User> currentUser = system.login(username, password);
            if (currentUser) {
                system.displayUserMenu(currentUser);
            }
            else {
                cout << "登录失败，用户名或密码错误！" << endl;
            }
            break;
        }
        case 2:
            cout << "感谢使用学生成绩管理系统，再见！" << endl;
            return 0;
        default:
            cout << "无效选择，请重新输入！" << endl;
        }
    }

    return 0;
}