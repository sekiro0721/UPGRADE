#include "system.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <iostream>

using namespace std;

// ����������������뻺����
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// �����û�
shared_ptr<User> ManagementSystem::findUser(string username) const {
    for (const auto& user : users) {
        if (user->getUsername() == username) {
            return user;
        }
    }
    return nullptr;
}

// ���ҿγ�
shared_ptr<Course> ManagementSystem::findCourse(string courseId) const {
    for (const auto& course : courses) {
        if (course->getCourseId() == courseId) {
            return course;
        }
    }
    return nullptr;
}

// ��ȡѧ�������пγ�
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

// ϵͳ��ʼ��
void ManagementSystem::initializeSystem() {
    // ���Լ�������
    loadUsers();
    loadCourses();
    loadGrades();

    // ���û���û�������Ĭ�Ϲ���Ա
    if (users.empty()) {
        auto admin = make_shared<Administrator>("admin", "admin123", "ϵͳ����Ա");
        users.push_back(admin);
        cout << "����Ĭ�Ϲ���Ա�˺�: admin������: admin123" << endl;
        saveUsers();
    }
}

// �û���¼
shared_ptr<User> ManagementSystem::login(string username, string password) const {
    auto user = findUser(username);
    if (user && user->getPassword() == password) {
        return user;
    }
    return nullptr;
}

// ��ʾ�û��˵�
void ManagementSystem::displayUserMenu(shared_ptr<User> user) {
    while (true) {
        user->displayMenu();

        int choice;
        cout << "��ѡ�����: ";
        while (!(cin >> choice)) {
            clearInputBuffer();
            cout << "������Ч������������: ";
        }
        clearInputBuffer();

        if (user->getRole() == "����Ա") {
            auto admin = dynamic_pointer_cast<Administrator>(user);
            switch (choice) {
            case 1: manageUsers(); break;
            case 2: manageCourses(); break;
            case 3: reviewGrades(); break;
            case 4: maintainData(); break;
            case 5: return;
            default: cout << "��Чѡ�����������룡" << endl;
            }
        }
        else if (user->getRole() == "��ʦ") {
            auto teacher = dynamic_pointer_cast<Teacher>(user);
            switch (choice) {
            case 1: viewTeachingCourses(teacher); break;
            case 2: enterOrModifyGrades(teacher); break;
            case 3: publishNotification(teacher); break;
            case 4: queryClassStatistics(teacher); break;
            case 5: return;
            default: cout << "��Чѡ�����������룡" << endl;
            }
        }
        else if (user->getRole() == "ѧ��") {
            auto student = dynamic_pointer_cast<Student>(user);
            switch (choice) {
            case 1: viewPersonalInfo(student); break;
            case 2: viewCourseSchedule(student); break;
            case 3: viewGradeDetails(student); break;
            case 4: receiveNotifications(student); break;
            case 5: submitReviewRequest(student); break;
            case 6: return;
            default: cout << "��Чѡ�����������룡" << endl;
            }
        }
    }
}

// ����Ա���û�����
void ManagementSystem::manageUsers() {
    while (true) {
        cout << "\n===== �û����� =====" << endl;
        cout << "1. ����û�" << endl;
        cout << "2. ɾ���û�" << endl;
        cout << "3. �޸��û���Ϣ" << endl;
        cout << "4. �鿴�����û�" << endl;
        cout << "5. �����ϼ��˵�" << endl;
        cout << "��ѡ�����: ";

        int choice;
        while (!(cin >> choice)) {
            clearInputBuffer();
            cout << "������Ч������������: ";
        }
        clearInputBuffer();

        switch (choice) {
        case 1: { // ����û�
            string username, password, name, role, className;
            cout << "�û���: ";
            getline(cin, username);

            // ����û����Ƿ��Ѵ���
            if (findUser(username)) {
                cout << "�û����Ѵ��ڣ�" << endl;
                break;
            }

            cout << "����: ";
            getline(cin, password);
            cout << "����: ";
            getline(cin, name);

            while (true) {
                cout << "��ɫ (��ʦ/ѧ��): ";
                getline(cin, role);
                if (role == "��ʦ" || role == "ѧ��") break;
                cout << "��Ч��ɫ�����������룡" << endl;
            }

            if (role == "ѧ��") {
                cout << "�༶: ";
                getline(cin, className);
                auto student = make_shared<Student>(username, password, name, className);
                users.push_back(student);
            }
            else {
                auto teacher = make_shared<Teacher>(username, password, name);
                users.push_back(teacher);
            }

            cout << "�û���ӳɹ���" << endl;
            saveUsers();
            break;
        }
        case 2: { // ɾ���û�
            string username;
            cout << "������Ҫɾ�����û���: ";
            getline(cin, username);

            if (username == "admin") {
                cout << "Ĭ�Ϲ���Ա�˺Ų���ɾ����" << endl;
                break;
            }

            for (auto it = users.begin(); it != users.end(); ++it) {
                if ((*it)->getUsername() == username) {
                    users.erase(it);
                    cout << "�û�ɾ���ɹ���" << endl;
                    saveUsers();
                    return;
                }
            }

            cout << "δ�ҵ����û���" << endl;
            break;
        }
        case 3: { // �޸��û���Ϣ
            string username;
            cout << "������Ҫ�޸ĵ��û���: ";
            getline(cin, username);

            auto user = findUser(username);
            if (!user) {
                cout << "δ�ҵ����û���" << endl;
                break;
            }

            string newName, newPassword;
            cout << "������ (�������޸�): ";
            getline(cin, newName);
            if (!newName.empty()) {
                user->setName(newName);
            }

            cout << "������ (�������޸�): ";
            getline(cin, newPassword);
            if (!newPassword.empty()) {
                user->setPassword(newPassword);
            }

            // ���⴦���޸�ѧ���༶���ʦ��Ŀ
            if (user->getRole() == "ѧ��") {
                string newClass;
                cout << "�°༶ (�������޸�): ";
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

            cout << "�û���Ϣ�޸ĳɹ���" << endl;
            saveUsers();
            break;
        }
        case 4: { // �鿴�����û�
            cout << "\n===== �û��б� =====" << endl;
            cout << left << setw(15) << "�û���" << setw(10) << "��ɫ" << setw(10) << "����";
            if (!users.empty() && dynamic_pointer_cast<Student>(users[0])) {
                cout << setw(15) << "�༶";
            }
            cout << endl;

            for (const auto& user : users) {
                cout << left << setw(15) << user->getUsername()
                    << setw(10) << user->getRole()
                    << setw(10) << user->getName();

                if (user->getRole() == "ѧ��") {
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
            cout << "��Чѡ�����������룡" << endl;
        }
    }
}

// ����Ա���γ̹���
void ManagementSystem::manageCourses() {
    while (true) {
        cout << "\n===== �γ̹��� =====" << endl;
        cout << "1. ��ӿγ�" << endl;
        cout << "2. �޸Ŀγ���Ϣ" << endl;
        cout << "3. ɾ���γ�" << endl;
        cout << "4. �鿴���пγ�" << endl;
        cout << "5. �����ϼ��˵�" << endl;
        cout << "��ѡ�����: ";

        int choice;
        while (!(cin >> choice)) {
            clearInputBuffer();
            cout << "������Ч������������: ";
        }
        clearInputBuffer();

        switch (choice) {
        case 1: { // ��ӿγ�
            string courseId, courseName, semester, className, teacherUsername;
            int credits;

            cout << "�γ�ID: ";
            getline(cin, courseId);

            // ���γ�ID�Ƿ��Ѵ���
            if (findCourse(courseId)) {
                cout << "�γ�ID�Ѵ��ڣ�" << endl;
                break;
            }

            cout << "�γ�����: ";
            getline(cin, courseName);
            cout << "ѧ��: ";
            while (!(cin >> credits)) {
                clearInputBuffer();
                cout << "������Ч������������: ";
            }
            clearInputBuffer();
            cout << "ѧ��: ";
            getline(cin, semester);
            cout << "�༶: ";
            getline(cin, className);
            cout << "�ڿν�ʦ�û���: ";
            getline(cin, teacherUsername);

            auto teacher = dynamic_pointer_cast<Teacher>(findUser(teacherUsername));
            if (!teacher) {
                cout << "δ�ҵ��ý�ʦ��" << endl;
                break;
            }

            auto course = make_shared<Course>(
                courseId, courseName, credits, semester, teacher, className
            );
            courses.push_back(course);
            teacher->addCourse(course);

            cout << "�γ���ӳɹ���" << endl;
            saveCourses();
            break;
        }
        case 2: { // �޸Ŀγ���Ϣ
            string courseId;
            cout << "������Ҫ�޸ĵĿγ�ID: ";
            getline(cin, courseId);

            auto course = findCourse(courseId);
            if (!course) {
                cout << "δ�ҵ��ÿγ̣�" << endl;
                break;
            }

            string newName, newSemester, newClassName, newTeacherUsername;
            int newCredits;

            cout << "�¿γ����� (�������޸�): ";
            getline(cin, newName);
            if (!newName.empty()) {
                // ����û��setter����Ҫ�����µ�course����
                auto teacher = course->getTeacher();
                auto newCourse = make_shared<Course>(
                    courseId, newName, course->getCredits(), course->getSemester(),
                    teacher, course->getClassName()
                );
                *course = *newCourse;
            }

            cout << "��ѧ�� (�������޸�): ";
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
                    cout << "������Ч��ѧ��δ�޸ģ�" << endl;
                }
            }

            cout << "��ѧ�� (�������޸�): ";
            getline(cin, newSemester);
            if (!newSemester.empty()) {
                auto teacher = course->getTeacher();
                auto newCourse = make_shared<Course>(
                    courseId, course->getCourseName(), course->getCredits(), newSemester,
                    teacher, course->getClassName()
                );
                *course = *newCourse;
            }

            cout << "�°༶ (�������޸�): ";
            getline(cin, newClassName);
            if (!newClassName.empty()) {
                auto teacher = course->getTeacher();
                auto newCourse = make_shared<Course>(
                    courseId, course->getCourseName(), course->getCredits(), course->getSemester(),
                    teacher, newClassName
                );
                *course = *newCourse;
            }

            cout << "���ڿν�ʦ�û��� (�������޸�): ";
            getline(cin, newTeacherUsername);
            if (!newTeacherUsername.empty()) {
                auto newTeacher = dynamic_pointer_cast<Teacher>(findUser(newTeacherUsername));
                if (!newTeacher) {
                    cout << "δ�ҵ��ý�ʦ����ʦδ�޸ģ�" << endl;
                }
                else {
                    auto newCourse = make_shared<Course>(
                        courseId, course->getCourseName(), course->getCredits(), course->getSemester(),
                        newTeacher, course->getClassName()
                    );
                    *course = *newCourse;

                    // ���γ���ӵ��½�ʦ�Ŀγ��б�
                    newTeacher->addCourse(course);
                }
            }

            cout << "�γ���Ϣ�޸ĳɹ���" << endl;
            saveCourses();
            break;
        }
        case 3: { // ɾ���γ�
            string courseId;
            cout << "������Ҫɾ���Ŀγ�ID: ";
            getline(cin, courseId);

            for (auto it = courses.begin(); it != courses.end(); ++it) {
                if ((*it)->getCourseId() == courseId) {
                    courses.erase(it);
                    cout << "�γ�ɾ���ɹ���" << endl;
                    saveCourses();
                    return;
                }
            }

            cout << "δ�ҵ��ÿγ̣�" << endl;
            break;
        }
        case 4: { // �鿴���пγ�
            cout << "\n===== �γ��б� =====" << endl;
            cout << left << setw(10) << "�γ�ID" << setw(20) << "�γ�����"
                << setw(10) << "ѧ��" << setw(15) << "ѧ��"
                << setw(15) << "�༶" << setw(15) << "�ڿν�ʦ" << endl;

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
            cout << "��Чѡ�����������룡" << endl;
        }
    }
}

// ����Ա���ɼ����
void ManagementSystem::reviewGrades() {
    while (true) {
        cout << "\n===== �ɼ���� =====" << endl;
        cout << "1. �鿴����˵ĳɼ�����" << endl;
        cout << "2. ����ɼ���������" << endl;
        cout << "3. �����ϼ��˵�" << endl;
        cout << "��ѡ�����: ";

        int choice;
        while (!(cin >> choice)) {
            clearInputBuffer();
            cout << "������Ч������������: ";
        }
        clearInputBuffer();

        switch (choice) {
        case 1: { // �鿴����˵ĳɼ�����
            auto pendingReviews = gradeManager.getPendingReviews();
            if (pendingReviews.empty()) {
                cout << "û�д���˵ĳɼ����룡" << endl;
                break;
            }

            cout << "\n===== ����˵ĳɼ����� =====" << endl;
            cout << left << setw(15) << "ѧ��ID" << setw(15) << "�γ�ID"
                << setw(15) << "��ǰ�ɼ�" << setw(30) << "��������" << endl;

            for (const auto& review : pendingReviews) {
                cout << left << setw(15) << review->getStudentId()
                    << setw(15) << review->getCourseId()
                    << setw(15) << review->getScore()
                    << setw(30) << review->getReviewReason() << endl;
            }
            break;
        }
        case 2: { // ����ɼ���������
            string studentId, courseId;
            double newScore;

            cout << "ѧ��ID: ";
            getline(cin, studentId);
            cout << "�γ�ID: ";
            getline(cin, courseId);

            // ����Ƿ��д���˵�����
            bool hasPendingReview = false;
            auto pendingReviews = gradeManager.getPendingReviews();
            for (const auto& review : pendingReviews) {
                if (review->getStudentId() == studentId && review->getCourseId() == courseId) {
                    hasPendingReview = true;
                    break;
                }
            }

            if (!hasPendingReview) {
                cout << "δ�ҵ���ѧ���ÿγ̵Ĵ�������룡" << endl;
                break;
            }

            cout << "�³ɼ�: ";
            while (!(cin >> newScore)) {
                clearInputBuffer();
                cout << "������Ч������������: ";
            }
            clearInputBuffer();

            if (gradeManager.processReviewRequest(studentId, courseId, newScore)) {
                cout << "�ɼ����˴���ɹ���" << endl;
                saveGrades();
            }
            else {
                cout << "�ɼ����˴���ʧ�ܣ�" << endl;
            }
            break;
        }
        case 3:
            return;
        default:
            cout << "��Чѡ�����������룡" << endl;
        }
    }
}

// ����Ա������ά��
void ManagementSystem::maintainData() {
    while (true) {
        cout << "\n===== ����ά�� =====" << endl;
        cout << "1. ��������" << endl;
        cout << "2. �ָ�����" << endl;
        cout << "3. �����ϼ��˵�" << endl;
        cout << "��ѡ�����: ";

        int choice;
        while (!(cin >> choice)) {
            clearInputBuffer();
            cout << "������Ч������������: ";
        }
        clearInputBuffer();

        switch (choice) {
        case 1: { // ��������
            saveUsers();
            saveCourses();
            saveGrades();
            cout << "���ݱ��ݳɹ���" << endl;
            break;
        }
        case 2: { // �ָ�����
            loadUsers();
            loadCourses();
            loadGrades();
            cout << "���ݻָ��ɹ���" << endl;
            break;
        }
        case 3:
            return;
        default:
            cout << "��Чѡ�����������룡" << endl;
        }
    }
}

// ��ʦ���鿴���ڿγ�
void ManagementSystem::viewTeachingCourses(shared_ptr<Teacher> teacher) {
    auto courses = teacher->getCourses();
    if (courses.empty()) {
        cout << "����δ�����κογ̣�" << endl;
        return;
    }

    cout << "\n===== �����ڵĿγ� =====" << endl;
    cout << left << setw(10) << "�γ�ID" << setw(20) << "�γ�����"
        << setw(10) << "ѧ��" << setw(15) << "ѧ��"
        << setw(15) << "�༶" << endl;

    for (const auto& course : courses) {
        cout << left << setw(10) << course->getCourseId()
            << setw(20) << course->getCourseName()
            << setw(10) << course->getCredits()
            << setw(15) << course->getSemester()
            << setw(15) << course->getClassName() << endl;
    }
}

// ��ʦ��¼��/�޸ĳɼ�
void ManagementSystem::enterOrModifyGrades(shared_ptr<Teacher> teacher) {
    auto courses = teacher->getCourses();
    if (courses.empty()) {
        cout << "����δ�����κογ̣�" << endl;
        return;
    }

    // ѡ��γ�
    cout << "\n===== �����ڵĿγ� =====" << endl;
    for (size_t i = 0; i < courses.size(); i++) {
        cout << i + 1 << ". " << courses[i]->getCourseName()
            << " (" << courses[i]->getClassName() << ")" << endl;
    }

    int courseChoice;
    cout << "��ѡ��γ� (1-" << courses.size() << "): ";
    while (!(cin >> courseChoice) || courseChoice < 1 || courseChoice > static_cast<int>(courses.size())) {
        clearInputBuffer();
        cout << "������Ч�������뷶Χ�ڵ�����: ";
    }
    clearInputBuffer();

    auto selectedCourse = courses[courseChoice - 1];

    // ѡ�����
    cout << "\n1. ¼��ɼ�" << endl;
    cout << "2. �޸ĳɼ�" << endl;
    cout << "��ѡ�����: ";

    int operation;
    while (!(cin >> operation) || (operation != 1 && operation != 2)) {
        clearInputBuffer();
        cout << "������Ч��������1��2: ";
    }
    clearInputBuffer();

    if (operation == 1) { // ¼��ɼ�
        // ��ȡ�ð༶������ѧ��
        vector<shared_ptr<User>> classStudents;
        for (const auto& user : users) {
            if (user->getRole() == "ѧ��") {
                auto student = dynamic_pointer_cast<Student>(user);
                if (student->getClassName() == selectedCourse->getClassName()) {
                    classStudents.push_back(student);
                }
            }
        }

        if (classStudents.empty()) {
            cout << "�ð༶û��ѧ����" << endl;
            return;
        }

        cout << "\n===== ¼�� " << selectedCourse->getCourseName()
            << " �γ̳ɼ� =====" << endl;

        for (const auto& user : classStudents) {
            auto student = dynamic_pointer_cast<Student>(user);
            double grade;

            // ����Ƿ����гɼ�
            if (selectedCourse->hasStudent(student->getUsername())) {
                cout << student->getName() << " (" << student->getUsername()
                    << ") ���гɼ�������" << endl;
                continue;
            }

            cout << student->getName() << " (" << student->getUsername()
                << ") �ĳɼ�: ";

            while (!(cin >> grade) || grade < 0 || grade > 100) {
                clearInputBuffer();
                cout << "������Ч��������0-100֮�������: ";
            }
            clearInputBuffer();

            // ��ӳɼ�
            if (selectedCourse->addGrade(student->getUsername(), grade)) {
                // ͬʱ���³ɼ�������
                gradeManager.addGrade(student->getUsername(),
                    selectedCourse->getCourseId(),
                    grade);
                cout << "�ɼ�¼��ɹ���" << endl;
            }
        }

        saveCourses();
        saveGrades();
    }
    else { // �޸ĳɼ�
        // ��ȡ�ð༶������ѧ���ɼ�
        auto grades = selectedCourse->getGrades();
        if (grades.empty()) {
            cout << "�ÿγ���δ¼��ɼ���" << endl;
            return;
        }

        cout << "\n===== " << selectedCourse->getCourseName()
            << " �γ̳ɼ� =====" << endl;
        cout << left << setw(15) << "ѧ��ID" << setw(15) << "ѧ������"
            << setw(10) << "�ɼ�" << endl;

        // �洢ѧ��ID��������ӳ��
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

        cout << "������Ҫ�޸ĳɼ���ѧ��ID: ";
        getline(cin, studentId);

        // ���ѧ���Ƿ����
        auto student = findUser(studentId);
        if (!student || student->getRole() != "ѧ��") {
            cout << "δ�ҵ���ѧ����" << endl;
            return;
        }

        // ����ѧ���Ƿ��ڰ༶��
        if (!selectedCourse->hasStudent(studentId)) {
            cout << "��ѧ�����ڴ˰༶�У�" << endl;
            return;
        }

        // ���ɼ��Ƿ�������
        if (gradeManager.isGradeFinalized(studentId, selectedCourse->getCourseId())) {
            cout << "�óɼ����������޷�ֱ���޸ģ����ύ������룡" << endl;
            return;
        }

        cout << "�³ɼ�: ";
        while (!(cin >> newGrade) || newGrade < 0 || newGrade > 100) {
            clearInputBuffer();
            cout << "������Ч��������0-100֮�������: ";
        }
        clearInputBuffer();

        // ���³ɼ�
        if (selectedCourse->updateGrade(studentId, newGrade)) {
            // ͬʱ���³ɼ�������
            gradeManager.updateGrade(studentId, selectedCourse->getCourseId(), newGrade);
            cout << "�ɼ��޸ĳɹ���" << endl;
            saveCourses();
            saveGrades();
        }
    }
}

// ��ʦ�������γ�֪ͨ
void ManagementSystem::publishNotification(shared_ptr<Teacher> teacher) {
    auto courses = teacher->getCourses();
    if (courses.empty()) {
        cout << "����δ�����κογ̣�" << endl;
        return;
    }

    // ѡ��γ�
    cout << "\n===== �����ڵĿγ� =====" << endl;
    for (size_t i = 0; i < courses.size(); i++) {
        cout << i + 1 << ". " << courses[i]->getCourseName()
            << " (" << courses[i]->getClassName() << ")" << endl;
    }

    int courseChoice;
    cout << "��ѡ��γ� (1-" << courses.size() << "): ";
    while (!(cin >> courseChoice) || courseChoice < 1 || courseChoice > static_cast<int>(courses.size())) {
        clearInputBuffer();
        cout << "������Ч�������뷶Χ�ڵ�����: ";
    }
    clearInputBuffer();

    auto selectedCourse = courses[courseChoice - 1];

    // ��ȡ֪ͨ����
    string notification;
    cout << "������֪ͨ����: ";
    getline(cin, notification);

    // ����֪ͨ
    selectedCourse->addNotification(notification);
    saveCourses();
}

// ��ʦ����ѯͳ�ư༶�ɼ�
void ManagementSystem::queryClassStatistics(shared_ptr<Teacher> teacher) {
    auto courses = teacher->getCourses();
    if (courses.empty()) {
        cout << "����δ�����κογ̣�" << endl;
        return;
    }

    // ѡ��γ�
    cout << "\n===== �����ڵĿγ� =====" << endl;
    for (size_t i = 0; i < courses.size(); i++) {
        cout << i + 1 << ". " << courses[i]->getCourseName()
            << " (" << courses[i]->getClassName() << ")" << endl;
    }

    int courseChoice;
    cout << "��ѡ��γ� (1-" << courses.size() << "): ";
    while (!(cin >> courseChoice) || courseChoice < 1 || courseChoice > static_cast<int>(courses.size())) {
        clearInputBuffer();
        cout << "������Ч�������뷶Χ�ڵ�����: ";
    }
    clearInputBuffer();

    auto selectedCourse = courses[courseChoice - 1];

    // ��ȡ�ð༶������ѧ���ɼ�
    auto grades = selectedCourse->getGrades();
    if (grades.empty()) {
        cout << "�ÿγ���δ¼��ɼ���" << endl;
        return;
    }

    // ����ͳ������
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

    // ���ͳ�ƽ��
    cout << "\n===== " << selectedCourse->getCourseName()
        << " �γ̳ɼ�ͳ�� =====" << endl;
    cout << "�༶: " << selectedCourse->getClassName() << endl;
    cout << "ѧ������: " << count << endl;
    cout << "ƽ����: " << fixed << setprecision(2) << average << endl;
    cout << "��߷�: " << maxGrade << endl;
    cout << "��ͷ�: " << minGrade << endl;
    cout << "������: " << fixed << setprecision(2) << passRate << "%" << endl;

    // ����ɼ��ֲ�
    cout << "\n�ɼ��ֲ�:" << endl;
    vector<int> ranges(11, 0); // 0-9, 10-19, ..., 90-99, 100

    for (const auto& grade : grades) {
        int range = static_cast<int>(grade.second) / 10;
        if (range >= 10) range = 10;
        ranges[range]++;
    }

    for (int i = 0; i < 10; i++) {
        cout << i * 10 << "-" << (i * 10 + 9) << "��: " << ranges[i] << "��" << endl;
    }
    cout << "100��: " << ranges[10] << "��" << endl;
}

// ѧ�����鿴������Ϣ
void ManagementSystem::viewPersonalInfo(shared_ptr<Student> student) {
    cout << "\n===== ������Ϣ =====" << endl;
    cout << "�û���: " << student->getUsername() << endl;
    cout << "����: " << student->getName() << endl;
    cout << "�༶: " << student->getClassName() << endl;
}

// ѧ�����鿴�γ̱�
void ManagementSystem::viewCourseSchedule(shared_ptr<Student> student) {
    auto studentCourses = getStudentCourses(student->getUsername());
    if (studentCourses.empty()) {
        cout << "����δ�����κογ̣�" << endl;
        return;
    }

    cout << "\n===== ���Ŀγ̱� =====" << endl;
    cout << left << setw(10) << "�γ�ID" << setw(20) << "�γ�����"
        << setw(10) << "ѧ��" << setw(15) << "ѧ��"
        << setw(15) << "�ڿν�ʦ" << endl;

    for (const auto& course : studentCourses) {
        cout << left << setw(10) << course->getCourseId()
            << setw(20) << course->getCourseName()
            << setw(10) << course->getCredits()
            << setw(15) << course->getSemester()
            << setw(15) << course->getTeacher()->getName() << endl;
    }
}

// ѧ�����鿴�ɼ�����
void ManagementSystem::viewGradeDetails(shared_ptr<Student> student) {
    auto studentCourses = getStudentCourses(student->getUsername());
    if (studentCourses.empty()) {
        cout << "����δ�����κογ̣�" << endl;
        return;
    }

    cout << "\n===== ���ĳɼ����� =====" << endl;
    cout << left << setw(20) << "�γ�����" << setw(10) << "ѧ��"
        << setw(10) << "�ɼ�" << setw(10) << "����" << endl;

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
            cout << setw(10) << "δ¼��" << setw(10) << "δ¼��";
        }

        cout << endl;
    }

    // ����ƽ������
    if (totalCredits > 0) {
        double GPA = totalGradePoints / totalCredits;
        cout << "\nƽ������: " << fixed << setprecision(2) << GPA << endl;
    }
}

// ѧ��������֪ͨ
void ManagementSystem::receiveNotifications(shared_ptr<Student> student) {
    auto studentCourses = getStudentCourses(student->getUsername());
    if (studentCourses.empty()) {
        cout << "����δ�����κογ̣�" << endl;
        return;
    }

    bool hasNotifications = false;

    for (const auto& course : studentCourses) {
        auto notifications = course->getNotifications();
        if (!notifications.empty()) {
            hasNotifications = true;
            cout << "\n=== " << course->getCourseName() << " �γ�֪ͨ ===" << endl;
            for (size_t i = 0; i < notifications.size(); i++) {
                cout << i + 1 << ". " << notifications[i] << endl;
            }
        }
    }

    if (!hasNotifications) {
        cout << "�����κογ�֪ͨ��" << endl;
    }
}

// ѧ�����ύ�ɼ���������
void ManagementSystem::submitReviewRequest(shared_ptr<Student> student) {
    auto studentCourses = getStudentCourses(student->getUsername());
    if (studentCourses.empty()) {
        cout << "����δ�����κογ̣�" << endl;
        return;
    }

    // ��ʾ��¼��ɼ��Ŀγ�
    vector<shared_ptr<Course>> gradedCourses;

    cout << "\n===== ��¼��ɼ��Ŀγ� =====" << endl;
    for (const auto& course : studentCourses) {
        double score = course->getStudentGrade(student->getUsername());
        if (score >= 0) {
            gradedCourses.push_back(course);
            cout << gradedCourses.size() << ". " << course->getCourseName()
                << " - �ɼ�: " << score << endl;
        }
    }

    if (gradedCourses.empty()) {
        cout << "������¼��ɼ��Ŀγ̣�" << endl;
        return;
    }

    // ѡ��γ�
    int courseChoice;
    cout << "��ѡ��Ҫ���븴�˵Ŀγ� (1-" << gradedCourses.size() << "): ";
    while (!(cin >> courseChoice) || courseChoice < 1 || courseChoice > static_cast<int>(gradedCourses.size())) {
        clearInputBuffer();
        cout << "������Ч�������뷶Χ�ڵ�����: ";
    }
    clearInputBuffer();

    auto selectedCourse = gradedCourses[courseChoice - 1];

    // ���ɼ��Ƿ�������������ȷ����
    if (gradeManager.isGradeFinalized(student->getUsername(), selectedCourse->getCourseId())) {
        cout << "�óɼ����������޷��ύ�������룡" << endl;
        return;
    }

    // ��ȡ��������
    string reason;
    cout << "�����븴������: ";
    getline(cin, reason);

    // �ύ����
    if (gradeManager.submitReviewRequest(student->getUsername(),
        selectedCourse->getCourseId(),
        reason)) {
        cout << "�ɼ������������ύ��" << endl;
        saveGrades();
    }
    else {
        cout << "�ύ����ʧ�ܣ�" << endl;
    }
}

// �����û�����
void ManagementSystem::saveUsers() const {
    ofstream file(dataPath + "users.txt");
    if (file.is_open()) {
        for (const auto& user : users) {
            if (user->getRole() == "����Ա") {
                file << "ADMIN," << user->getUsername() << ","
                    << user->getPassword() << "," << user->getName() << endl;
            }
            else if (user->getRole() == "��ʦ") {
                file << "TEACHER," << user->getUsername() << ","
                    << user->getPassword() << "," << user->getName() << endl;
            }
            else if (user->getRole() == "ѧ��") {
                auto student = dynamic_pointer_cast<Student>(user);
                file << "STUDENT," << user->getUsername() << ","
                    << user->getPassword() << "," << user->getName() << ","
                    << student->getClassName() << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "�޷����ļ������û����ݣ�" << endl;
    }
}

// �����û�����
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

// ����γ�����
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

            // ����֪ͨ
            auto notifications = course->getNotifications();
            for (const auto& notification : notifications) {
                file << "NOTIFICATION," << course->getCourseId() << ","
                    << notification << endl;
            }

            // ����ɼ�
            auto grades = course->getGrades();
            for (const auto& grade : grades) {
                file << "GRADE," << course->getCourseId() << ","
                    << grade.first << "," << grade.second << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "�޷����ļ�����γ����ݣ�" << endl;
    }
}

// ���ؿγ�����
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
                // ��Щ���Ժ���
                continue;
            }

            courseId = type; // ��һ����courseId
            getline(ss, courseName, ',');

            string creditsStr;
            getline(ss, creditsStr, ',');
            credits = stoi(creditsStr);

            getline(ss, semester, ',');
            getline(ss, teacherUsername, ',');
            getline(ss, className);

            // ���ҽ�ʦ
            auto teacher = dynamic_pointer_cast<Teacher>(findUser(teacherUsername));
            if (!teacher) {
                // ����Ҳ�����ʦ�������ÿγ�
                continue;
            }

            auto course = make_shared<Course>(
                courseId, courseName, credits, semester, teacher, className
            );
            courses.push_back(course);
            teacher->addCourse(course);
        }

        // ���¶�ȡ�ļ��Դ���֪ͨ�ͳɼ�
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

// ����ɼ�����
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
        cerr << "�޷����ļ�����ɼ����ݣ�" << endl;
    }
}

// ���سɼ�����
void ManagementSystem::loadGrades() {
    // ע�⣺�γ̼���ʱ�Ѿ������˻����ɼ�
    // ����ֻ��Ҫ���ظ�������״̬
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

            // �����ɼ�����
            auto grade = make_shared<Grade>(studentId, courseId, score);
            grade->setIsFinalized(finalized);
            grade->setIsUnderReview(underReview);
            grade->setReviewReason(reason);

            // �����޷�ֱ�ӷ���gradeManager��˽�г�Ա����Ҫ���ù�������
            // ʵ��ʵ���п�����Ҫ�������
        }
        file.close();
    }
}