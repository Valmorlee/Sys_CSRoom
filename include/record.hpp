//
// Created by valmorx on 25-4-21.
//

#ifndef RECORD_HPP
#define RECORD_HPP

#include <bits/stdc++.h>
#include <people.hpp>

namespace base {

    class Record {
    private:

        std::string id; //学号
        std::string name; //姓名
        std::string classId; //班级
        std::string gender; //性别
        int machineId; //设备id
        tm startTime; //开始时间
        tm endTime; //结束时间

        int RecordNum;

    public:
        Record(const base::Student &student, const tm &startTimeX, const tm &endTimeX, const int &machineId);
        Record();
        ~Record();

        void setId(const std::string &id); //设置学号
        void setName(const std::string &name); //设置姓名
        void setClassId(const std::string &classId); //设置班级
        void setGender(const std::string &gender); //设置性别
        void setStartTime(const tm &startTime); //设置开始时间
        void setEndTime(const tm &endTime); //设置结束时间
        void setMachineId(int machineId); //设置设备id

        std::string getId() const; //获取学号
        std::string getName() const; //获取姓名
        std::string getClassId() const; //获取班级
        std::string getGender() const; //获取性别
        tm getStartTime() const; //获取开始时间
        tm getEndTime() const; //获取结束时间
        int getMachineId() const; //获取设备id

    };


}

#endif //RECORD_HPP
