//
// Created by valmorx on 25-4-19.
//

#include "people.hpp"

namespace base {

    People::People() {
        this->id = "NULL";
        this->classId = "NULL";
        this->name = "NULL";
        this->gender = "NULL";
    }

    People::People(const std::string &id, const std::string &classId, const std::string &name, const std::string &gender) {
        this->id = id;
        this->classId = classId;
        this->name = name;
        this->gender = gender;
    }

    People::People(const People &people) {
        this->id = people.id;
        this->classId = people.classId;
        this->name = people.name;
        this->gender = people.gender;
    }

    void People::setId(const std::string &id) {
        this->id = id;
    }

    void People::setClassId(const std::string &classId) {
        this->classId = classId;
    }

    void People::setName(const std::string &name) {
        this->name = name;
    }

    void People::setGender(const std::string &gender) {
        this->gender = gender;
    }

    std::string People::getId() const {
        return this->id;
    }

    std::string People::getClassId() const {
        return this->classId;
    }

    std::string People::getName() const {
        return this->name;
    }

    std::string People::getGender() const {
        return this->gender;
    }

    People::~People() = default;

    Student::Student() {
        this->id = "NULL";
        this->name = "NULL";
        this->classId = "NULL";
        this->gender = "NULL";
        this->password = "NULL";
    }

    Student::Student(const std::string &id, const std::string &classId, const std::string &name, const std::string &gender, const std::string &password, const std::tm &startTime, const std::tm &endTime) {
        this->id = id;
        this->classId = classId;
        this->name = name;
        this->gender = gender;
        this->password = password;
    }

    Student::Student(const std::string &id, const std::string &classId, const std::string &name, const std::string &gender, const std::string &password) {
        this->id = id;
        this->classId = classId;
        this->name = name;
        this->gender = gender;
        this->password = password;
    }

    Student::Student(const Student &student) {
        this->id = student.id;
        this->classId = student.classId;
        this->name = student.name;
        this->gender = student.gender;
        this->password = student.password;
    }

    void Student::setPassword(const std::string &password) {
        this->password = password;
    }


    std::string Student::getPassword() const {
        return this->password;
    }


    Student::~Student() = default;

    Manager::Manager(){
        this->id = "NULL";
        this->classId = "NULL";
        this->name = "NULL";
        this->gender = "NULL";
        this->password = "NULL";
    }

    Manager::Manager(const std::string &id, const std::string &classId, const std::string &name, const std::string &gender, const std::string &password){
        this->id = id;
        this->classId = classId;
        this->name = name;
        this->gender = gender;
        this->password = password;
    }

    Manager::Manager(const Manager &manager) {
        this->id = manager.id;
        this->classId = manager.classId;
        this->name = manager.name;
        this->gender = manager.gender;
        this->password = manager.password;
    }

    void Manager::setPassword(const std::string &password) {
        this->password = password;
    }

    std::string Manager::getPassword() const {
        return this->password;
    }

    Manager::~Manager() = default;

    Machine::Machine() {
        this->next = nullptr;
        this->id = NULL;
        this->flag = 0;
        this->user = nullptr;
    }

    Machine::Machine(const int id) {
        this->next = nullptr;
        this->user = nullptr;
        this->id = id;
        this->flag = 0;
    }

    Machine::Machine(const int id, Student *user) {
        this->id = id;
        this->user = user;
        this->next = nullptr;
    }

    int Machine::getId() const {
        return this->id;
    }

    bool Machine::getFlag() const {
        return this->flag;
    }

    void Machine::setFlag(bool flag) {
        this->flag = flag;
    }

    void Machine::setId(int id) {
        this->id = id;
    }


    Machine::~Machine() = default;

}