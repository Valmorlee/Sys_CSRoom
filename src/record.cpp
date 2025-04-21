//
// Created by valmorx on 25-4-21.
//

#include<record.hpp>

base::Record::Record() {
    this->id = "NULL";
    this->name = "NULL";
    this->classId = "NULL";
    this->gender = "NULL";
    this->startTime = {0,0,0,0,0,0,0,0,0};
    this->endTime = {0,0,0,0,0,0,0,0,0};
}

base::Record::Record(const base::Student &student, const tm &startTimeX, const tm &endTimeX) {
    this->id = student.getId();
    this->classId = student.getClassId();
    this->name = student.getName();
    this->gender = student.getGender();
    this->startTime = startTimeX;
    this->endTime = endTimeX;
}

base::Record::~Record() = default;

std::string base::Record::getClassId() const {
    return this->classId;
}

std::string base::Record::getGender() const {
    return this->gender;
}

std::string base::Record::getId() const {
    return this->id;
}

std::string base::Record::getName() const {
    return this->name;
}

tm base::Record::getStartTime() const {
    return this->startTime;
}

tm base::Record::getEndTime() const {
    return this->endTime;
}

void base::Record::setClassId(const std::string &classId) {
    this->classId = classId;
}

void base::Record::setGender(const std::string &gender) {
    this->gender = gender;
}

void base::Record::setId(const std::string &id) {
    this->id = id;
}

void base::Record::setName(const std::string &name) {
    this->name = name;
}

void base::Record::setStartTime(const tm &startTime) {
    this->startTime = startTime;
}

void base::Record::setEndTime(const tm &endTime) {
    this->endTime = endTime;
}


