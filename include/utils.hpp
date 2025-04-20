//
// Created by valmorx on 25-4-19.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <bits/stdc++.h>
#include <time.h>

namespace func {



}

namespace timeX {

    inline std::vector<int> monthTable = {0,31,0,31,30,31,30,31,31,30,31,30,31}; // 月份表
    bool isLeapYear(int year);
    long timeCal(tm t1,tm t2); // 时间差计算 返回值为小时 层级优化

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
