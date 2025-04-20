//
// Created by valmorx on 25-4-19.
//

#ifndef PEOPLE_HPP
#define PEOPLE_HPP

#include<bits/stdc++.h>

namespace base {

    class People {
    protected:
        std::string id; // 工号/学号/上机账号
        std::string classId; // 所属班级 / 组织
        std::string name; // 姓名
        std::string gender; // 性别
        bool flag = false;

    public:
        People();
        People(const std::string &id, const std::string &classId, const std::string &name, const std::string &gender);
        People(const People &people);
        ~People();

        std::string getId() const;// 获取工号/学号/上机账号
        void setId(const std::string &id);// 设置工号/学号/上机账号
        std::string getClassId() const;// 获取班级/组织
        void setClassId(const std::string &classId);// 设置班级/组织
        std::string getName() const;// 获取姓名
        void setName(const std::string &name);// 设置姓名
        std::string getGender() const;// 获取性别
        void setGender(const std::string &gender);// 设置性别

    };

    class Student : public People {
    protected:
        std::string password;
        std::tm startTime;
        std::tm endTime;
        int level;

    public:
        Student *next;
        Student();
        Student(const std::string &id, const std::string &classId, const std::string &name, const std::string &gender,const std::string &password);
        Student(const std::string &id, const std::string &classId, const std::string &name, const std::string &gender,const std::string &password,const std::tm &startTime,const std::tm &endTime);
        Student(const Student &student);
        ~Student();

        std::string getPassword() const;
        void setPassword(const std::string &password);
        std::tm getStartTime() const;
        void setStartTime(const std::tm &startTime);
        std::tm getEndTime() const;
        void setEndTime(const std::tm &endTime);

    };

    class Teacher : public People {
    protected:
        std::string password;
        int level;


    public:
        Teacher *next;
        Teacher();
        Teacher(const std::string &id, const std::string &classId, const std::string &name, const std::string &gender,const std::string &password);
        Teacher(const Teacher &teacher);
        ~Teacher();

        std::string getPassword() const;
        void setPassword(const std::string &password);

    };

    class Manager: public People {
    protected:
        std::string password;
        int level;


    public:
        Manager *next;
        Manager();
        Manager(const std::string &id, const std::string &classId, const std::string &name, const std::string &gender,const std::string &password);
        Manager(const Manager &manager);
        ~Manager();

        std::string getPassword() const;
        void setPassword(const std::string &password);

    };

    class Machine {
    private:
        int id;
        Student *user;

    public:

        Machine *next; // 指向下一个机器

        Machine(); //创建空机器
        Machine(int id); // 创建空机器 编号
        Machine(int id, Student *user); //创建正在使用的机器

        ~Machine();

    };

}

#endif //PEOPLE_HPP
