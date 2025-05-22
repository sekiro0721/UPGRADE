#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <memory>

using namespace std;

// 前向声明
class Course;

// 用户基类
class User {
protected:
    string username;
    string password;
    string name;
    string role;

public:
    User(string username, string password, string name, string role)
        : username(username), password(password), name(name), role(role) {
    }

    virtual ~User() = default;

    // Getters
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getName() const { return name; }
    string getRole() const { return role; }

    // Setters
    void setPassword(string newPassword) { password = newPassword; }
    void setName(string newName) { name = newName; }

    // 纯虚函数：显示用户菜单
    virtual void displayMenu() const = 0;
};

// 管理员类
class Administrator : public User {
public:
    Administrator(string username, string password, string name)
        : User(username, password, name, "管理员") {
    }

    void displayMenu() const override;
};

// 教师类
class Teacher : public User {
private:
    vector<shared_ptr<Course>> courses;

public:
    Teacher(string username, string password, string name)
        : User(username, password, name, "教师") {
    }

    void displayMenu() const override;
    void addCourse(shared_ptr<Course> course);
    vector<shared_ptr<Course>> getCourses() const { return courses; }
};

// 学生类
class Student : public User {
private:
    string className;
    vector<shared_ptr<Course>> courses;

public:
    Student(string username, string password, string name, string className)
        : User(username, password, name, "学生"), className(className) {
    }

    void displayMenu() const override;
    string getClassName() const { return className; }
    void addCourse(shared_ptr<Course> course);
    vector<shared_ptr<Course>> getCourses() const { return courses; }
};

#endif // USER_H    