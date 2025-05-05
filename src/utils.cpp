//
// Created by valmorx on 25-4-19.
//

#include "utils.hpp"

namespace cmp {

    bool cmp_id_up(base::Record &a,base::Record &b) { // id升序
        return a.getId() < b.getId();
    }

    bool cmp_id_down(base::Record &a,base::Record &b) { // id降序
        return a.getId() > b.getId();
    }

    bool cmp_startTime_up(base::Record &a,base::Record &b) { // 开始时间升序
        return timeX::timeComp(a.getStartTime(), b.getStartTime()) == false;
    }

    bool cmp_startTime_down(base::Record &a,base::Record &b) { // 开始时间降序
        return timeX::timeComp(a.getStartTime(), b.getStartTime()) == true;
    }

    bool cmp_duringTime_up(base::Record &a, base::Record &b) { // 持续时间升序
        return timeX::timeCal(a.getStartTime(), a.getEndTime()) < timeX::timeCal(b.getStartTime(), b.getEndTime());
    }

    bool cmp_duringTime_down(base::Record &a, base::Record &b) { // 持续时间降序
        return timeX::timeCal(a.getStartTime(), a.getEndTime()) > timeX::timeCal(b.getStartTime(), b.getEndTime());
    }

    bool cmp_classId_up(base::Record &a, base::Record &b) { // 班级升序
        return a.getClassId() < b.getClassId();
    }

    bool cmp_classId_down(base::Record &a, base::Record &b) { // 班级降序
        return a.getClassId() > b.getClassId();
    }

    bool cmp_name_up(base::Record &a, base::Record &b) { // 姓名升序
        return a.getName() < b.getName();
    }

    bool cmp_name_down(base::Record &a, base::Record &b) { // 姓名降序
        return a.getName() > b.getName();
    }

}

namespace timeX {

    timeViewer::timeViewer() {
        timep = 0;
        time(&this->timep);
    }

    tm timeViewer::getTime() {
        time(&this->timep);
        tm ptm;
        localtime_r(&timep,&ptm);
        return ptm;
    }

    bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    }

    bool timeComp(tm t1, tm t2) { // t1 > t2 则返回true， t1 < t2 则返回false
        int year1 = t1.tm_year + 1900;
        int year2 = t2.tm_year + 1900;
        int month1 = t1.tm_mon + 1;
        int month2 = t2.tm_mon + 1;
        int day1 = t1.tm_mday;
        int day2 = t2.tm_mday;
        int hour1 = t1.tm_hour;
        int hour2 = t2.tm_hour;
        int minute1 = t1.tm_min;
        int minute2 = t2.tm_min;
        int second1 = t1.tm_sec;
        int second2 = t2.tm_sec;

        if (year1 > year2) return true;
        else if (year1 < year2) return false;

        if (month1 > month2) return true;
        else if (month1 < month2) return false;

        if (day1 > day2) return true;
        else if (day1 < day2) return false;

        if (hour1 > hour2) return true;
        else if (hour1 < hour2) return false;

        if (minute1 > minute2) return true;
        else if (minute1 < minute2) return false;

        if (second1 > second2) return true;
        else if (second1 < second2) return false;

        return false;
    }

    long timeCal(const tm &t1, const tm &t2) {
        int year1 = t1.tm_year + 1900;
        int year2 = t2.tm_year + 1900;
        int month1 = t1.tm_mon + 1;
        int month2 = t2.tm_mon + 1;
        int day1 = t1.tm_mday;
        int day2 = t2.tm_mday;
        int hour1 = t1.tm_hour;
        int hour2 = t2.tm_hour;
        int minute1 = t1.tm_min;
        int minute2 = t2.tm_min;
        int second1 = t1.tm_sec;
        int second2 = t2.tm_sec;

        if (year1 == year2 && month1 == month2 && day1 == day2) {
            return hour2 - hour1 + (((minute2 - minute1) * 60 + (second2 - second1)) >= 0);
        }

        if (year1 == year2 && month1 == month2) {
            return (day2 - day1) * 24 + hour2 - hour1 + (((minute2 - minute1) * 60 + (second2 - second1)) >= 0);
        }

        if (year1 == year2) {
            if (isLeapYear(year1)) monthTable[2] = 29;
            else monthTable[2] = 28;
            int sum_mon = 0;
            for (int i = month1; i < month2; i++) {
                sum_mon += monthTable[i];
            }

            return (day2 - day1) * 24 + hour2 - hour1 + (((minute2 - minute1) * 60 + (second2 - second1)) >= 0) + sum_mon * 24;
        }

        if (year1 < year2) {
            int sum_year = 0, sum_mon = 0;

            if (isLeapYear(year1)) monthTable[2] = 29;
            else monthTable[2] = 28;
            for (int i = month1; i < 12; i++) {
                sum_mon += monthTable[i];
            }

            for (int i = year1 + 1; i < year2; i++) {
                if (isLeapYear(i)) sum_year += 366;
                else sum_year += 365;
            }

            for (int i = 1; i < month2; i++) {
                sum_mon += monthTable[i];
            }

            return (day2 - day1) * 24 + hour2 - hour1 + (((minute2 - minute1) * 60 + (second2 - second1)) >= 0) + sum_year * 24 + sum_mon * 24;
        }

        return -1;
    }

}