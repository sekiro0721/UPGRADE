#include "system.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <iostream>

using namespace std;

// 辅助函数：清除输入缓冲区
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// 查找用户
shared_ptr<User> ManagementSystem::findUser(string username) const {
    for (const auto& user : users) {
        if (user->getUsername() == username) {
            return user;
        }
    }
    return nullptr;
}

// 查找课程
shared_ptr<Course> ManagementSystem::findCourse(string courseId) const {
    for (const auto& course : courses) {
        if (course->getCourseId() == courseId) {
            return course;
        }
    }
    return nullptr;
}

// 获取学生的所有课程
vector<shared_ptr<Course>> ManagementSystem::getStudentCourses(string studentId) const {
    vector<shared_ptr<Course>> studentCourses;
    auto student = dynamic_pointer_cast<Student>(findUser(studentId));

    if (!student) return studentCourses;

    string className = student->getClassName();
    for (const auto& course : courses) {
        if (course->getClassName() == className) {
            studentCourses.push_back(course);
        }
    }
    return studentCourses;
}

// 系统初始化
void ManagementSystem::initializeSystem() {
    // 尝试加载数据
    loadUsers();
    loadCourses();
    loadGrades();

    // 如果没有用户，创建默认管理员
    if (users.empty()) {
        auto admin = make_shared<Administrator>("admin", "admin123", "系统管理员");
        users.push_back(admin);
        cout << "创建默认管理员账号: admin，密码: admin123" << endl;
        saveUsers();
    }
}

// 用户登录
shared_ptr<User> ManagementSystem::login(string username, string password) const {
    auto user = findUser(username);
    if (user && user->getPassword() == password) {
        return user;
    }
    return nullptr;
}

// 显示用户菜单
void ManagementSystem::displayUserMenu(shared_ptr<User> user) {
    while (true) {
        user->displayMenu();

        int choice;
        cout << "请选择操作: ";
        while (!(cin >> choice)) {
            clearInputBuffer();
            cout << "输入无效，请输入数字: ";
        }
        clearInputBuffer();

        if (user->getRole() == "管理员") {
            auto admin = dynamic_pointer_cast<Administrator>(user);
            switch (choice) {
            case 1: manageUsers(); break;
            case 2: manageCourses(); break;
            case 3: reviewGrades(); break;
            case 4: maintainData(); break;
            case 5: return;
            default: cout << "无效选择，请重新输入！" << endl;
            }
        }
        else if (user->getRole() == "教师") {
            auto teacher = dynamic_pointer_cast<Teacher>(user);
            switch (choice) {
            case 1: viewTeachingCourses(teacher); break;
            case 2: enterOrModifyGrades(teacher); break;
            case 3: publishNotification(teacher); break;
            case 4: queryClassStatistics(teacher); break;
            case 5: return;
            default: cout << "无效选择，请重新输入！" << endl;
            }
        }
        else if (user->getRole() == "学生") {
            auto student = dynamic_pointer_cast<Student>(user);
            switch (choice) {
            case 1: viewPersonalInfo(student); break;
            case 2: viewCourseSchedule(student); break;
            case 3: viewGradeDetails(student); break;
            case 4: receiveNotifications(student); break;
            case 5: submitReviewRequest(student); break;
            case 6: return;
            default: cout << "无效选择，请重新输入！" << endl;
            }
        }
    }
}

// 管理员：用户管理
void ManagementSystem::manageUsers() {
    while (true) {
        cout << "\n===== 用户管理 =====" << endl;
        cout << "1. 添加用户" << endl;
        cout << "2. 删除用户" << endl;
        cout << "3. 修改用户信息" << endl;
        cout << "4. 查看所有用户" << endl;
        cout << "5. 返回上级菜单" << endl;
        cout << "请选择操作: ";

        int choice;
        while (!(cin >> choice)) {
            clearInputBuffer();
            cout << "输入无效，请输入数字: ";
        }
        clearInputBuffer();

        switch (choice) {
        case 1: { // 添加用户
            string username, password, name, role, className;
            cout << "用户名: ";
            getline(cin, username);

            // 检查用户名是否已存在
            if (findUser(username)) {
                cout << "用户名已存在！" << endl;
                break;
            }

            cout << "密码: ";
            getline(cin, password);
            cout << "姓名: ";
            getline(cin, name);

            while (true) {
                cout << "角色 (教师/学生): ";
                getline(cin, role);
                if (role == "教师" || role == "学生") break;
                cout << "无效角色，请重新输入！" << endl;
            }

            if (role == "学生") {
                cout << "班级: ";
                getline(cin, className);
                auto student = make_shared<Student>(username, password, name, className);
                users.push_back(student);
            }
            else {
                auto teacher = make_shared<Teacher>(username, password, name);
                users.push_back(teacher);
            }

            cout << "用户添加成功！" << endl;
            saveUsers();
            break;
        }
        case 2: { // 删除用户
            string username;
            cout << "请输入要删除的用户名: ";
            getline(cin, username);

            if (username == "admin") {
                cout << "默认管理员账号不能删除！" << endl;
                break;
            }

            for (auto it = users.begin(); it != users.end(); ++it) {
                if ((*it)->getUsername() == username) {
                    users.erase(it);
                    cout << "用户删除成功！" << endl;
                    saveUsers();
                    return;
                }
            }

            cout << "未找到该用户！" << endl;
            break;
        }
        case 3: { // 修改用户信息
            string username;
            cout << "请输入要修改的用户名: ";
            getline(cin, username);

            auto user = findUser(username);
            if (!user) {
                cout << "未找到该用户！" << endl;
                break;
            }

            string newName, newPassword;
            cout << "新姓名 (留空则不修改): ";
            getline(cin, newName);
            if (!newName.empty()) {
                user->setName(newName);
            }

            cout << "新密码 (留空则不修改): ";
            getline(cin, newPassword);
            if (!newPassword.empty()) {
                user->setPassword(newPassword);
            }

            // 特殊处理：修改学生班级或教师科目
            if (user->getRole() == "学生") {
                string newClass;
                cout << "新班级 (留空则不修改): ";
                getline(cin, newClass);
                if (!newClass.empty()) {
                    auto student = dynamic_pointer_cast<Student>(user);
                    student = make_shared<Student>(
                        student->getUsername(),
                        student->getPassword(),
                        student->getName(),
                        newClass
                    );
                }
            }

            cout << "用户信息修改成功！" << endl;
            saveUsers();
            break;
        }
        case 4: { // 查看所有用户
            cout << "\n===== 用户列表 =====" << endl;
            cout << left << setw(15) << "用户名" << setw(10) << "角色" << setw(10) << "姓名";
            if (!users.empty() && dynamic_pointer_cast<Student>(users[0])) {
                cout << setw(15) << "班级";
            }
            cout << endl;

            for (const auto& user : users) {
                cout << left << setw(15) << user->getUsername()
                    << setw(10) << user->getRole()
                    << setw(10) << user->getName();

                if (user->getRole() == "学生") {
                    auto student = dynamic_pointer_cast<Student>(user);
                    cout << setw(15) << student->getClassName();
                }
                cout << endl;
            }
            break;
        }
        case 5:
            return;
        default:
            cout << "无效选择，请重新输入！" << endl;
        }
    }
}

// 管理员：课程管理
void ManagementSystem::manageCourses() {
    while (true) {
        cout << "\n===== 课程管理 =====" << endl;
        cout << "1. 添加课程" << endl;
        cout << "2. 修改课程信息" << endl;
        cout << "3. 删除课程" << endl;
        cout << "4. 查看所有课程" << endl;
        cout << "5. 返回上级菜单" << endl;
        cout << "请选择操作: ";

        int choice;
        while (!(cin >> choice)) {
            clearInputBuffer();
            cout << "输入无效，请输入数字: ";
        }
        clearInputBuffer();

        switch (choice) {
        case 1: { // 添加课程
            string courseId, courseName, semester, className, teacherUsername;
            int credits;

            cout << "课程ID: ";
            getline(cin, courseId);

            // 检查课程ID是否已存在
            if (findCourse(courseId)) {
                cout << "课程ID已存在！" << endl;
                break;
            }

            cout << "课程名称: ";
            getline(cin, courseName);
            cout << "学分: ";
            while (!(cin >> credits)) {
                clearInputBuffer();
                cout << "输入无效，请输入数字: ";
            }
            clearInputBuffer();
            cout << "学期: ";
            getline(cin, semester);
            cout << "班级: ";
            getline(cin, className);
            cout << "授课教师用户名: ";
            getline(cin, teacherUsername);

            auto teacher = dynamic_pointer_cast<Teacher>(findUser(teacherUsername));
            if (!teacher) {
                cout << "未找到该教师！" << endl;
                break;
            }

            auto course = make_shared<Course>(
                courseId, courseName, credits, semester, teacher, className
            );
            courses.push_back(course);
            teacher->addCourse(course);

            cout << "课程添加成功！" << endl;
            saveCourses();
            break;
        }
        case 2: { // 修改课程信息
            string courseId;
            cout << "请输入要修改的课程ID: ";
            getline(cin, courseId);

            auto course = findCourse(courseId);
            if (!course) {
                cout << "未找到该课程！" << endl;
                break;
            }

            string newName, newSemester, newClassName, newTeacherUsername;
            int newCredits;

            cout << "新课程名称 (留空则不修改): ";
            getline(cin, newName);
            if (!newName.empty()) {
                // 由于没有setter，需要创建新的course对象
                auto teacher = course->getTeacher();
                auto newCourse = make_shared<Course>(
                    courseId, newName, course->getCredits(), course->getSemester(),
                    teacher, course->getClassName()
                );
                *course = *newCourse;
            }

            cout << "新学分 (留空则不修改): ";
            string input;
            getline(cin, input);
            if (!input.empty()) {
                stringstream ss(input);
                if (ss >> newCredits) {
                    auto teacher = course->getTeacher();
                    auto newCourse = make_shared<Course>(
                        courseId, course->getCourseName(), newCredits, course->getSemester(),
                        teacher, course->getClassName()
                    );
                    *course = *newCourse;
                }
                else {
                    cout << "输入无效，学分未修改！" << endl;
                }
            }

            cout << "新学期 (留空则不修改): ";
            getline(cin, newSemester);
            if (!newSemester.empty()) {
                auto teacher = course->getTeacher();
                auto newCourse = make_shared<Course>(
                    courseId, course->getCourseName(), course->getCredits(), newSemester,
                    teacher, course->getClassName()
                );
                *course = *newCourse;
            }

            cout << "新班级 (留空则不修改): ";
            getline(cin, newClassName);
            if (!newClassName.empty()) {
                auto teacher = course->getTeacher();
                auto newCourse = make_shared<Course>(
                    courseId, course->getCourseName(), course->getCredits(), course->getSemester(),
                    teacher, newClassName
                );
                *course = *newCourse;
            }

            cout << "新授课教师用户名 (留空则不修改): ";
            getline(cin, newTeacherUsername);
            if (!newTeacherUsername.empty()) {
                auto newTeacher = dynamic_pointer_cast<Teacher>(findUser(newTeacherUsername));
                if (!newTeacher) {
                    cout << "未找到该教师，教师未修改！" << endl;
                }
                else {
                    auto newCourse = make_shared<Course>(
                        courseId, course->getCourseName(), course->getCredits(), course->getSemester(),
                        newTeacher, course->getClassName()
                    );
                    *course = *newCourse;

                    // 将课程添加到新教师的课程列表
                    newTeacher->addCourse(course);
                }
            }

            cout << "课程信息修改成功！" << endl;
            saveCourses();
            break;
        }
        case 3: { // 删除课程
            string courseId;
            cout << "请输入要删除的课程ID: ";
            getline(cin, courseId);

            for (auto it = courses.begin(); it != courses.end(); ++it) {
                if ((*it)->getCourseId() == courseId) {
                    courses.erase(it);
                    cout << "课程删除成功！" << endl;
                    saveCourses();
                    return;
                }
            }

            cout << "未找到该课程！" << endl;
            break;
        }
        case 4: { // 查看所有课程
            cout << "\n===== 课程列表 =====" << endl;
            cout << left << setw(10) << "课程ID" << setw(20) << "课程名称"
                << setw(10) << "学分" << setw(15) << "学期"
                << setw(15) << "班级" << setw(15) << "授课教师" << endl;

            for (const auto& course : courses) {
                cout << left << setw(10) << course->getCourseId()
                    << setw(20) << course->getCourseName()
                    << setw(10) << course->getCredits()
                    << setw(15) << course->getSemester()
                    << setw(15) << course->getClassName()
                    << setw(15) << course->getTeacher()->getName() << endl;
            }
            break;
        }
        case 5:
            return;
        default:
            cout << "无效选择，请重新输入！" << endl;
        }
    }
}

// 管理员：成绩审核
void ManagementSystem::reviewGrades() {
    while (true) {
        cout << "\n===== 成绩审核 =====" << endl;
        cout << "1. 查看待审核的成绩申请" << endl;
        cout << "2. 处理成绩复核申请" << endl;
        cout << "3. 返回上级菜单" << endl;
        cout << "请选择操作: ";

        int choice;
        while (!(cin >> choice)) {
            clearInputBuffer();
            cout << "输入无效，请输入数字: ";
        }
        clearInputBuffer();

        switch (choice) {
        case 1: { // 查看待审核的成绩申请
            auto pendingReviews = gradeManager.getPendingReviews();
            if (pendingReviews.empty()) {
                cout << "没有待审核的成绩申请！" << endl;
                break;
            }

            cout << "\n===== 待审核的成绩申请 =====" << endl;
            cout << left << setw(15) << "学生ID" << setw(15) << "课程ID"
                << setw(15) << "当前成绩" << setw(30) << "复核理由" << endl;

            for (const auto& review : pendingReviews) {
                cout << left << setw(15) << review->getStudentId()
                    << setw(15) << review->getCourseId()
                    << setw(15) << review->getScore()
                    << setw(30) << review->getReviewReason() << endl;
            }
            break;
        }
        case 2: { // 处理成绩复核申请
            string studentId, courseId;
            double newScore;

            cout << "学生ID: ";
            getline(cin, studentId);
            cout << "课程ID: ";
            getline(cin, courseId);

            // 检查是否有待审核的申请
            bool hasPendingReview = false;
            auto pendingReviews = gradeManager.getPendingReviews();
            for (const auto& review : pendingReviews) {
                if (review->getStudentId() == studentId && review->getCourseId() == courseId) {
                    hasPendingReview = true;
                    break;
                }
            }

            if (!hasPendingReview) {
                cout << "未找到该学生该课程的待审核申请！" << endl;
                break;
            }

            cout << "新成绩: ";
            while (!(cin >> newScore)) {
                clearInputBuffer();
                cout << "输入无效，请输入数字: ";
            }
            clearInputBuffer();

            if (gradeManager.processReviewRequest(studentId, courseId, newScore)) {
                cout << "成绩复核处理成功！" << endl;
                saveGrades();
            }
            else {
                cout << "成绩复核处理失败！" << endl;
            }
            break;
        }
        case 3:
            return;
        default:
            cout << "无效选择，请重新输入！" << endl;
        }
    }
}

// 管理员：数据维护
void ManagementSystem::maintainData() {
    while (true) {
        cout << "\n===== 数据维护 =====" << endl;
        cout << "1. 备份数据" << endl;
        cout << "2. 恢复数据" << endl;
        cout << "3. 返回上级菜单" << endl;
        cout << "请选择操作: ";

        int choice;
        while (!(cin >> choice)) {
            clearInputBuffer();
            cout << "输入无效，请输入数字: ";
        }
        clearInputBuffer();

        switch (choice) {
        case 1: { // 备份数据
            saveUsers();
            saveCourses();
            saveGrades();
            cout << "数据备份成功！" << endl;
            break;
        }
        case 2: { // 恢复数据
            loadUsers();
            loadCourses();
            loadGrades();
            cout << "数据恢复成功！" << endl;
            break;
        }
        case 3:
            return;
        default:
            cout << "无效选择，请重新输入！" << endl;
        }
    }
}

// 教师：查看所授课程
void ManagementSystem::viewTeachingCourses(shared_ptr<Teacher> teacher) {
    auto courses = teacher->getCourses();
    if (courses.empty()) {
        cout << "您尚未分配任何课程！" << endl;
        return;
    }

    cout << "\n===== 您所授的课程 =====" << endl;
    cout << left << setw(10) << "课程ID" << setw(20) << "课程名称"
        << setw(10) << "学分" << setw(15) << "学期"
        << setw(15) << "班级" << endl;

    for (const auto& course : courses) {
        cout << left << setw(10) << course->getCourseId()
            << setw(20) << course->getCourseName()
            << setw(10) << course->getCredits()
            << setw(15) << course->getSemester()
            << setw(15) << course->getClassName() << endl;
    }
}

// 教师：录入/修改成绩
void ManagementSystem::enterOrModifyGrades(shared_ptr<Teacher> teacher) {
    auto courses = teacher->getCourses();
    if (courses.empty()) {
        cout << "您尚未分配任何课程！" << endl;
        return;
    }

    // 选择课程
    cout << "\n===== 您所授的课程 =====" << endl;
    for (size_t i = 0; i < courses.size(); i++) {
        cout << i + 1 << ". " << courses[i]->getCourseName()
            << " (" << courses[i]->getClassName() << ")" << endl;
    }

    int courseChoice;
    cout << "请选择课程 (1-" << courses.size() << "): ";
    while (!(cin >> courseChoice) || courseChoice < 1 || courseChoice > static_cast<int>(courses.size())) {
        clearInputBuffer();
        cout << "输入无效，请输入范围内的数字: ";
    }
    clearInputBuffer();

    auto selectedCourse = courses[courseChoice - 1];

    // 选择操作
    cout << "\n1. 录入成绩" << endl;
    cout << "2. 修改成绩" << endl;
    cout << "请选择操作: ";

    int operation;
    while (!(cin >> operation) || (operation != 1 && operation != 2)) {
        clearInputBuffer();
        cout << "输入无效，请输入1或2: ";
    }
    clearInputBuffer();

    if (operation == 1) { // 录入成绩
        // 获取该班级的所有学生
        vector<shared_ptr<User>> classStudents;
        for (const auto& user : users) {
            if (user->getRole() == "学生") {
                auto student = dynamic_pointer_cast<Student>(user);
                if (student->getClassName() == selectedCourse->getClassName()) {
                    classStudents.push_back(student);
                }
            }
        }

        if (classStudents.empty()) {
            cout << "该班级没有学生！" << endl;
            return;
        }

        cout << "\n===== 录入 " << selectedCourse->getCourseName()
            << " 课程成绩 =====" << endl;

        for (const auto& user : classStudents) {
            auto student = dynamic_pointer_cast<Student>(user);
            double grade;

            // 检查是否已有成绩
            if (selectedCourse->hasStudent(student->getUsername())) {
                cout << student->getName() << " (" << student->getUsername()
                    << ") 已有成绩，跳过" << endl;
                continue;
            }

            cout << student->getName() << " (" << student->getUsername()
                << ") 的成绩: ";

            while (!(cin >> grade) || grade < 0 || grade > 100) {
                clearInputBuffer();
                cout << "输入无效，请输入0-100之间的数字: ";
            }
            clearInputBuffer();

            // 添加成绩
            if (selectedCourse->addGrade(student->getUsername(), grade)) {
                // 同时更新成绩管理器
                gradeManager.addGrade(student->getUsername(),
                    selectedCourse->getCourseId(),
                    grade);
                cout << "成绩录入成功！" << endl;
            }
        }

        saveCourses();
        saveGrades();
    }
    else { // 修改成绩
        // 获取该班级的所有学生成绩
        auto grades = selectedCourse->getGrades();
        if (grades.empty()) {
            cout << "该课程尚未录入成绩！" << endl;
            return;
        }

        cout << "\n===== " << selectedCourse->getCourseName()
            << " 课程成绩 =====" << endl;
        cout << left << setw(15) << "学生ID" << setw(15) << "学生姓名"
            << setw(10) << "成绩" << endl;

        // 存储学生ID到索引的映射
        map<string, int> studentIndex;
        int index = 1;

        for (const auto& grade : grades) {
            auto student = dynamic_pointer_cast<Student>(findUser(grade.first));
            if (student) {
                cout << left << setw(15) << grade.first
                    << setw(15) << student->getName()
                    << setw(10) << grade.second << endl;
                studentIndex[grade.first] = index++;
            }
        }

        string studentId;
        double newGrade;

        cout << "请输入要修改成绩的学生ID: ";
        getline(cin, studentId);

        // 检查学生是否存在
        auto student = findUser(studentId);
        if (!student || student->getRole() != "学生") {
            cout << "未找到该学生！" << endl;
            return;
        }

        // 检查该学生是否在班级中
        if (!selectedCourse->hasStudent(studentId)) {
            cout << "该学生不在此班级中！" << endl;
            return;
        }

        // 检查成绩是否已锁定
        if (gradeManager.isGradeFinalized(studentId, selectedCourse->getCourseId())) {
            cout << "该成绩已锁定，无法直接修改，请提交审核申请！" << endl;
            return;
        }

        cout << "新成绩: ";
        while (!(cin >> newGrade) || newGrade < 0 || newGrade > 100) {
            clearInputBuffer();
            cout << "输入无效，请输入0-100之间的数字: ";
        }
        clearInputBuffer();

        // 更新成绩
        if (selectedCourse->updateGrade(studentId, newGrade)) {
            // 同时更新成绩管理器
            gradeManager.updateGrade(studentId, selectedCourse->getCourseId(), newGrade);
            cout << "成绩修改成功！" << endl;
            saveCourses();
            saveGrades();
        }
    }
}

// 教师：发布课程通知
void ManagementSystem::publishNotification(shared_ptr<Teacher> teacher) {
    auto courses = teacher->getCourses();
    if (courses.empty()) {
        cout << "您尚未分配任何课程！" << endl;
        return;
    }

    // 选择课程
    cout << "\n===== 您所授的课程 =====" << endl;
    for (size_t i = 0; i < courses.size(); i++) {
        cout << i + 1 << ". " << courses[i]->getCourseName()
            << " (" << courses[i]->getClassName() << ")" << endl;
    }

    int courseChoice;
    cout << "请选择课程 (1-" << courses.size() << "): ";
    while (!(cin >> courseChoice) || courseChoice < 1 || courseChoice > static_cast<int>(courses.size())) {
        clearInputBuffer();
        cout << "输入无效，请输入范围内的数字: ";
    }
    clearInputBuffer();

    auto selectedCourse = courses[courseChoice - 1];

    // 获取通知内容
    string notification;
    cout << "请输入通知内容: ";
    getline(cin, notification);

    // 发布通知
    selectedCourse->addNotification(notification);
    saveCourses();
}

// 教师：查询统计班级成绩
void ManagementSystem::queryClassStatistics(shared_ptr<Teacher> teacher) {
    auto courses = teacher->getCourses();
    if (courses.empty()) {
        cout << "您尚未分配任何课程！" << endl;
        return;
    }

    // 选择课程
    cout << "\n===== 您所授的课程 =====" << endl;
    for (size_t i = 0; i < courses.size(); i++) {
        cout << i + 1 << ". " << courses[i]->getCourseName()
            << " (" << courses[i]->getClassName() << ")" << endl;
    }

    int courseChoice;
    cout << "请选择课程 (1-" << courses.size() << "): ";
    while (!(cin >> courseChoice) || courseChoice < 1 || courseChoice > static_cast<int>(courses.size())) {
        clearInputBuffer();
        cout << "输入无效，请输入范围内的数字: ";
    }
    clearInputBuffer();

    auto selectedCourse = courses[courseChoice - 1];

    // 获取该班级的所有学生成绩
    auto grades = selectedCourse->getGrades();
    if (grades.empty()) {
        cout << "该课程尚未录入成绩！" << endl;
        return;
    }

    // 计算统计数据
    int count = grades.size();
    double sum = 0;
    double maxGrade = -1;
    double minGrade = 101;
    int passCount = 0;

    for (const auto& grade : grades) {
        sum += grade.second;
        if (grade.second > maxGrade) maxGrade = grade.second;
        if (grade.second < minGrade) minGrade = grade.second;
        if (grade.second >= 60) passCount++;
    }

    double average = sum / count;
    double passRate = static_cast<double>(passCount) / count * 100;

    // 输出统计结果
    cout << "\n===== " << selectedCourse->getCourseName()
        << " 课程成绩统计 =====" << endl;
    cout << "班级: " << selectedCourse->getClassName() << endl;
    cout << "学生人数: " << count << endl;
    cout << "平均分: " << fixed << setprecision(2) << average << endl;
    cout << "最高分: " << maxGrade << endl;
    cout << "最低分: " << minGrade << endl;
    cout << "及格率: " << fixed << setprecision(2) << passRate << "%" << endl;

    // 输出成绩分布
    cout << "\n成绩分布:" << endl;
    vector<int> ranges(11, 0); // 0-9, 10-19, ..., 90-99, 100

    for (const auto& grade : grades) {
        int range = static_cast<int>(grade.second) / 10;
        if (range >= 10) range = 10;
        ranges[range]++;
    }

    for (int i = 0; i < 10; i++) {
        cout << i * 10 << "-" << (i * 10 + 9) << "分: " << ranges[i] << "人" << endl;
    }
    cout << "100分: " << ranges[10] << "人" << endl;
}

// 学生：查看个人信息
void ManagementSystem::viewPersonalInfo(shared_ptr<Student> student) {
    cout << "\n===== 个人信息 =====" << endl;
    cout << "用户名: " << student->getUsername() << endl;
    cout << "姓名: " << student->getName() << endl;
    cout << "班级: " << student->getClassName() << endl;
}

// 学生：查看课程表
void ManagementSystem::viewCourseSchedule(shared_ptr<Student> student) {
    auto studentCourses = getStudentCourses(student->getUsername());
    if (studentCourses.empty()) {
        cout << "您尚未分配任何课程！" << endl;
        return;
    }

    cout << "\n===== 您的课程表 =====" << endl;
    cout << left << setw(10) << "课程ID" << setw(20) << "课程名称"
        << setw(10) << "学分" << setw(15) << "学期"
        << setw(15) << "授课教师" << endl;

    for (const auto& course : studentCourses) {
        cout << left << setw(10) << course->getCourseId()
            << setw(20) << course->getCourseName()
            << setw(10) << course->getCredits()
            << setw(15) << course->getSemester()
            << setw(15) << course->getTeacher()->getName() << endl;
    }
}

// 学生：查看成绩详情
void ManagementSystem::viewGradeDetails(shared_ptr<Student> student) {
    auto studentCourses = getStudentCourses(student->getUsername());
    if (studentCourses.empty()) {
        cout << "您尚未分配任何课程！" << endl;
        return;
    }

    cout << "\n===== 您的成绩详情 =====" << endl;
    cout << left << setw(20) << "课程名称" << setw(10) << "学分"
        << setw(10) << "成绩" << setw(10) << "绩点" << endl;

    double totalCredits = 0;
    double totalGradePoints = 0;

    for (const auto& course : studentCourses) {
        double score = course->getStudentGrade(student->getUsername());
        double gradePoint = 0;

        if (score >= 90) gradePoint = 4.0;
        else if (score >= 85) gradePoint = 3.7;
        else if (score >= 80) gradePoint = 3.3;
        else if (score >= 75) gradePoint = 3.0;
        else if (score >= 70) gradePoint = 2.7;
        else if (score >= 65) gradePoint = 2.3;
        else if (score >= 60) gradePoint = 2.0;
        else gradePoint = 0;

        cout << left << setw(20) << course->getCourseName()
            << setw(10) << course->getCredits();

        if (score >= 0) {
            cout << setw(10) << fixed << setprecision(1) << score
                << setw(10) << fixed << setprecision(1) << gradePoint;

            totalCredits += course->getCredits();
            totalGradePoints += gradePoint * course->getCredits();
        }
        else {
            cout << setw(10) << "未录入" << setw(10) << "未录入";
        }

        cout << endl;
    }

    // 计算平均绩点
    if (totalCredits > 0) {
        double GPA = totalGradePoints / totalCredits;
        cout << "\n平均绩点: " << fixed << setprecision(2) << GPA << endl;
    }
}

// 学生：接收通知
void ManagementSystem::receiveNotifications(shared_ptr<Student> student) {
    auto studentCourses = getStudentCourses(student->getUsername());
    if (studentCourses.empty()) {
        cout << "您尚未分配任何课程！" << endl;
        return;
    }

    bool hasNotifications = false;

    for (const auto& course : studentCourses) {
        auto notifications = course->getNotifications();
        if (!notifications.empty()) {
            hasNotifications = true;
            cout << "\n=== " << course->getCourseName() << " 课程通知 ===" << endl;
            for (size_t i = 0; i < notifications.size(); i++) {
                cout << i + 1 << ". " << notifications[i] << endl;
            }
        }
    }

    if (!hasNotifications) {
        cout << "暂无任何课程通知！" << endl;
    }
}

// 学生：提交成绩复核申请
void ManagementSystem::submitReviewRequest(shared_ptr<Student> student) {
    auto studentCourses = getStudentCourses(student->getUsername());
    if (studentCourses.empty()) {
        cout << "您尚未分配任何课程！" << endl;
        return;
    }

    // 显示已录入成绩的课程
    vector<shared_ptr<Course>> gradedCourses;

    cout << "\n===== 已录入成绩的课程 =====" << endl;
    for (const auto& course : studentCourses) {
        double score = course->getStudentGrade(student->getUsername());
        if (score >= 0) {
            gradedCourses.push_back(course);
            cout << gradedCourses.size() << ". " << course->getCourseName()
                << " - 成绩: " << score << endl;
        }
    }

    if (gradedCourses.empty()) {
        cout << "暂无已录入成绩的课程！" << endl;
        return;
    }

    // 选择课程
    int courseChoice;
    cout << "请选择要申请复核的课程 (1-" << gradedCourses.size() << "): ";
    while (!(cin >> courseChoice) || courseChoice < 1 || courseChoice > static_cast<int>(gradedCourses.size())) {
        clearInputBuffer();
        cout << "输入无效，请输入范围内的数字: ";
    }
    clearInputBuffer();

    auto selectedCourse = gradedCourses[courseChoice - 1];

    // 检查成绩是否已锁定（最终确定）
    if (gradeManager.isGradeFinalized(student->getUsername(), selectedCourse->getCourseId())) {
        cout << "该成绩已锁定，无法提交复核申请！" << endl;
        return;
    }

    // 获取复核理由
    string reason;
    cout << "请输入复核理由: ";
    getline(cin, reason);

    // 提交申请
    if (gradeManager.submitReviewRequest(student->getUsername(),
        selectedCourse->getCourseId(),
        reason)) {
        cout << "成绩复核申请已提交！" << endl;
        saveGrades();
    }
    else {
        cout << "提交申请失败！" << endl;
    }
}

// 保存用户数据
void ManagementSystem::saveUsers() const {
    ofstream file(dataPath + "users.txt");
    if (file.is_open()) {
        for (const auto& user : users) {
            if (user->getRole() == "管理员") {
                file << "ADMIN," << user->getUsername() << ","
                    << user->getPassword() << "," << user->getName() << endl;
            }
            else if (user->getRole() == "教师") {
                file << "TEACHER," << user->getUsername() << ","
                    << user->getPassword() << "," << user->getName() << endl;
            }
            else if (user->getRole() == "学生") {
                auto student = dynamic_pointer_cast<Student>(user);
                file << "STUDENT," << user->getUsername() << ","
                    << user->getPassword() << "," << user->getName() << ","
                    << student->getClassName() << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "无法打开文件保存用户数据！" << endl;
    }
}

// 加载用户数据
void ManagementSystem::loadUsers() {
    users.clear();
    ifstream file(dataPath + "users.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string type, username, password, name, className;

            getline(ss, type, ',');
            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, name, ',');

            if (type == "ADMIN") {
                auto admin = make_shared<Administrator>(username, password, name);
                users.push_back(admin);
            }
            else if (type == "TEACHER") {
                auto teacher = make_shared<Teacher>(username, password, name);
                users.push_back(teacher);
            }
            else if (type == "STUDENT") {
                getline(ss, className);
                auto student = make_shared<Student>(username, password, name, className);
                users.push_back(student);
            }
        }
        file.close();
    }
}

// 保存课程数据
void ManagementSystem::saveCourses() const {
    ofstream file(dataPath + "courses.txt");
    if (file.is_open()) {
        for (const auto& course : courses) {
            file << course->getCourseId() << ","
                << course->getCourseName() << ","
                << course->getCredits() << ","
                << course->getSemester() << ","
                << course->getTeacher()->getUsername() << ","
                << course->getClassName() << endl;

            // 保存通知
            auto notifications = course->getNotifications();
            for (const auto& notification : notifications) {
                file << "NOTIFICATION," << course->getCourseId() << ","
                    << notification << endl;
            }

            // 保存成绩
            auto grades = course->getGrades();
            for (const auto& grade : grades) {
                file << "GRADE," << course->getCourseId() << ","
                    << grade.first << "," << grade.second << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "无法打开文件保存课程数据！" << endl;
    }
}

// 加载课程数据
void ManagementSystem::loadCourses() {
    courses.clear();
    ifstream file(dataPath + "courses.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string type, courseId, courseName, semester, teacherUsername, className;
            int credits;

            getline(ss, type, ',');

            if (type == "NOTIFICATION" || type == "GRADE") {
                // 这些行稍后处理
                continue;
            }

            courseId = type; // 第一列是courseId
            getline(ss, courseName, ',');

            string creditsStr;
            getline(ss, creditsStr, ',');
            credits = stoi(creditsStr);

            getline(ss, semester, ',');
            getline(ss, teacherUsername, ',');
            getline(ss, className);

            // 查找教师
            auto teacher = dynamic_pointer_cast<Teacher>(findUser(teacherUsername));
            if (!teacher) {
                // 如果找不到教师，跳过该课程
                continue;
            }

            auto course = make_shared<Course>(
                courseId, courseName, credits, semester, teacher, className
            );
            courses.push_back(course);
            teacher->addCourse(course);
        }

        // 重新读取文件以处理通知和成绩
        file.clear();
        file.seekg(0, ios::beg);

        while (getline(file, line)) {
            stringstream ss(line);
            string type, courseId;

            getline(ss, type, ',');
            getline(ss, courseId, ',');

            auto course = findCourse(courseId);
            if (!course) continue;

            if (type == "NOTIFICATION") {
                string notification;
                getline(ss, notification);
                course->addNotification(notification);
            }
            else if (type == "GRADE") {
                string studentId, gradeStr;
                getline(ss, studentId, ',');
                getline(ss, gradeStr);
                double grade = stod(gradeStr);
                course->addGrade(studentId, grade);
                gradeManager.addGrade(studentId, courseId, grade);
            }
        }

        file.close();
    }
}

// 保存成绩数据
void ManagementSystem::saveGrades() const {
    ofstream file(dataPath + "grades.txt");
    if (file.is_open()) {
        auto pendingReviews = gradeManager.getPendingReviews();
        for (const auto& review : pendingReviews) {
            file << review->getStudentId() << ","
                << review->getCourseId() << ","
                << review->getScore() << ","
                << (review->getIsFinalized() ? "1" : "0") << ","
                << (review->getIsUnderReview() ? "1" : "0") << ","
                << review->getReviewReason() << endl;
        }
        file.close();
    }
    else {
        cerr << "无法打开文件保存成绩数据！" << endl;
    }
}

// 加载成绩数据
void ManagementSystem::loadGrades() {
    // 注意：课程加载时已经加载了基本成绩
    // 这里只需要加载复核申请状态
    ifstream file(dataPath + "grades.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string studentId, courseId, scoreStr, finalizedStr, reviewStr, reason;

            getline(ss, studentId, ',');
            getline(ss, courseId, ',');
            getline(ss, scoreStr, ',');
            getline(ss, finalizedStr, ',');
            getline(ss, reviewStr, ',');
            getline(ss, reason);

            double score = stod(scoreStr);
            bool finalized = (finalizedStr == "1");
            bool underReview = (reviewStr == "1");

            // 创建成绩对象
            auto grade = make_shared<Grade>(studentId, courseId, score);
            grade->setIsFinalized(finalized);
            grade->setIsUnderReview(underReview);
            grade->setReviewReason(reason);

            // 这里无法直接访问gradeManager的私有成员，需要调用公共方法
            // 实际实现中可能需要调整设计
        }
        file.close();
    }
}