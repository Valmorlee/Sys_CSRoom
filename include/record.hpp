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

        std::string id;
        std::string name;
        std::string classId;
        std::string gender;
        int machineId;
        tm startTime;
        tm endTime;

    public:
        Record(const base::Student &student, const tm &startTimeX, const tm &endTimeX, const int &machineId);
        Record();
        ~Record();

        void setId(const std::string &id);
        void setName(const std::string &name);
        void setClassId(const std::string &classId);
        void setGender(const std::string &gender);
        void setStartTime(const tm &startTime);
        void setEndTime(const tm &endTime);
        void setMachineId(int machineId);

        std::string getId() const;
        std::string getName() const;
        std::string getClassId() const;
        std::string getGender() const;
        tm getStartTime() const;
        tm getEndTime() const;
        int getMachineId() const;

    };


}

#endif //RECORD_HPP
