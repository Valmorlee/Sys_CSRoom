//
// Created by valmorx on 25-4-19.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <bits/stdc++.h>
#include <time.h>
#include "people.hpp"
#include "record.hpp"

namespace cmp {

    // 学号
    bool cmp_id_up(base::Record &a,base::Record &b);
    bool cmp_id_down(base::Record &a,base::Record &b);

    // 班级
    bool cmp_classId_up(base::Record &a,base::Record &b);
    bool cmp_classId_down(base::Record &a,base::Record &b);

    // 姓名
    bool cmp_name_up(base::Record &a,base::Record &b);
    bool cmp_name_down(base::Record &a,base::Record &b);

    // 开始时间
    bool cmp_startTime_up(base::Record &a,base::Record &b);
    bool cmp_startTime_down(base::Record &a,base::Record &b);

    // 时长
    bool cmp_duringTime_up(base::Record &a,base::Record &b);
    bool cmp_duringTime_down(base::Record &a,base::Record &b);

}

namespace timeX {

    inline std::vector<int> monthTable = {0,31,0,31,30,31,30,31,31,30,31,30,31}; // 月份表
    bool isLeapYear(int year);
    bool timeComp(tm t1, tm t2);
    long timeCal(const tm &t1, const tm &t2); // 时间差计算 返回值为小时 层级优化

    class timeViewer { // 时间查看器
    private:
        time_t timep;

    public:

        timeViewer(); // 构造函数
        tm getTime(); // 获取当前时间
        ~timeViewer() = default;

    };


}

#endif //UTILS_HPP
