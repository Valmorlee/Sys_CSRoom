//
// Created by valmorx on 25-4-19.
//

// 万能库
#include <bits/stdc++.h>

// FTXUI库
#include <ftxui/dom/elements.hpp>
#include "ftxui/screen/color.hpp"
#include <ftxui/screen/screen.hpp>
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/mouse.hpp"
#include <ftxui/dom/table.hpp>
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp"
#include <ctime>

// 自定义库
#include <people.hpp>
#include "record.hpp"
#include "utils.hpp"

using namespace ftxui;

namespace sys {

    const float cash_index = 1.0; // 默认单小时价格

    void menu_register(); // 注册菜单
    void menu_login(); // 登陆菜单
    void menu_modify(); // 修改密码菜单

    void menu_stu_check(const std::string &account); // 学生可视界面
    void menu_man_check(const std::string &account); // 管理员可视界面

    // 系统总数据
    std::vector<base::Student> list_stu; // 学生列表
    std::vector<base::Manager> list_man; // 管理员列表
    std::vector<base::Record> list_rec; // 记录列表
    std::vector<base::Machine> list_mach; // 机器列表

    inline std::string detectNum = "ZJUT"; // 管理员邀请码（系统内可修改）
    inline int machineNum = 40; // 机器数量（系统内可修改）
    inline std::string test_modify = ""; // 修改界面警告提示词

    Element colorTile(const int &red, const int &green, const int &blue, const int &width, const int &height) { // 创建色块 用于明确机器状态
        return text("") | size(WIDTH, EQUAL, width ) | size(HEIGHT,EQUAL, height) | bgcolor(Color::RGB(red, green, blue));
    }

    void preload() { // 预加载数据
        std::string a,b,c,d,e; // 一些瞎写的临时变量
        std::fstream in;

        in.open("../out/data_student.txt", std::ios::in);
        while (in >> a >> b >> c >> d >> e) {
            list_stu.emplace_back(base::Student(a,c,d,e,b));
        }
        in.close();

        in.open("../out/data_manager.txt", std::ios::in);
        while (in >> a >> b >> c >> d >> e) {
            list_man.emplace_back(base::Manager(a,c,d,e,b));
        }
        in.close();

        int a1, b1, c1, d1, e1 ,f1 ; // 一些瞎写的临时变量
        int a2, b2, c2, d2, e2 ,f2 ,num; // 一些瞎写的临时变量

        in.open("../out/data_record.txt", std::ios::in);
        while (in >> a >> b >> c >> d >> a1 >> b1 >> c1 >> d1 >> e1 >> f1 >>a2 >> b2 >> c2 >> d2 >> e2 >> f2 >> num) {
            tm sx = tm(f1,e1,d1,c1,b1,a1,0,0,0,0,0);
            tm ex = tm(f2,e2,d2,c2,b2,a2,0,0,0,0,0);
            list_rec.emplace_back(base::Student(a,b,c,d,"NULL",tm(),tm()),sx,ex,num);
        }
        in.close();

        // machine列表初始化
        in.open("../out/data_machine.txt", std::ios::in);
        int num1; std::string num2;
        in >> num1 >> num2;
        machineNum = num1;
        detectNum = num2;

        for (int i = 0; i < machineNum; i++) {
            list_mach.emplace_back(i+1);
        }

    }

    void save() { // 保存数据
        std::fstream out;
        out.open("../out/data_student.txt",std::ios::out);
        for (auto stu : list_stu) {
            out << stu.getId() << " " << stu.getPassword() << " " << stu.getClassId() << " " << stu.getName() << " " << stu.getGender() << std::endl;
        }
        out.close();

        out.open("../out/data_manager.txt",std::ios::out);
        for (auto man : list_man) {
            out << man.getId() << " " << man.getPassword() << " " << man.getClassId() << " " << man.getName() << " " << man.getGender() << std::endl;
        }
        out.close();

        out.open("../out/data_record.txt",std::ios::out);
        for (auto rec : list_rec) {
            out << rec.getId() << " " << rec.getClassId() << " " << rec.getName() << " " << rec.getGender() << " "
            << rec.getStartTime().tm_year << " " << rec.getStartTime().tm_mon << " " << rec.getStartTime().tm_mday << " "
            << rec.getStartTime().tm_hour << " " << rec.getStartTime().tm_min << " " << rec.getStartTime().tm_sec << " "
            << rec.getEndTime().tm_year << " " << rec.getEndTime().tm_mon << " " << rec.getEndTime().tm_mday << " "
            << rec.getEndTime().tm_hour << " " << rec.getEndTime().tm_min << " " << rec.getEndTime().tm_sec << " " << rec.getMachineId() << std::endl;
        }
        out.close();

        out.open("../out/data_machine.txt",  std::ios::out);
        out << machineNum << std::endl << detectNum << std::endl;
        out.close();
    }

    void menu_login() { // 登录菜单

        std::string account; // 账号
        std::string password; // 密码

        Component input_account = Input(&account, "在此输入学号/工号");
        InputOption pwd_option;
        pwd_option.password = true; // 密码输入选项：使得密码打码（
        Component input_password = Input(&password, "在此输入密码", pwd_option);

        std::string test = "";

        Component button_login = Button("  登陆", [&] { // 登陆按钮功能
            if (account.empty() || password.empty()) test = "请输入账号/密码";
            else {

                bool checkExist = false; // 检测账户是否存在

                for (auto stu : list_stu) { // 搜索学生列表
                    if (stu.getId()==account) {
                        checkExist = true;
                        if (stu.getPassword()==password) {
                            menu_stu_check(account);
                        }else {
                            test = "密码错误";
                            password = "";
                            break;
                        }
                    }
                }

                for (auto man : list_man) { // 搜索管理员列表
                    if (man.getId()==account) {
                        checkExist = true;
                        if (man.getPassword()==password) {
                            menu_man_check(account);
                        }else {
                            test = "密码错误";
                            password = "";
                            break;
                        }
                    }
                }

                if (!checkExist) { // 检查账户信息是否存在
                    test = "账户信息不存在";
                    password = "";
                    account = "";
                }

            }
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_register = Button("  注册", [&] { // 注册按钮功能
            menu_register(); // 跳转注册界面
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_modify = Button("修改密码", [&] { // 修改密码按钮功能
            menu_modify(); //  跳转修改密码界面
        }) | size(WIDTH,EQUAL,10) | center;

        auto comp = Container::Vertical({ // 垂直布局组件工具集
            input_account,
            input_password,
            button_login,
            button_register,
            button_modify,
        });

        auto renderer = Renderer(comp, [&] { // 菜单界面渲染器

            return vbox({
                text("Hello " + account + " !"),
                separator(),
                hbox(text("账号: "), input_account->Render()),
                hbox(text("密码: "), input_password->Render()),

            }) | border | size(WIDTH,EQUAL,60) | center;
        });

        auto result = Renderer(renderer,[&] { // 外轮廓渲染
            return vbox({
                text("浙江工业大学机房收费管理系统欢迎您") | bold | color(LinearGradient(45,Color::Pink1,Color::White)) | center,
                renderer->Render(),
                text(test) | center | bold | color(Color::Red),
                hbox(button_login->Render(), separatorEmpty(), button_register->Render(), separatorEmpty(), button_modify->Render()) | center,
            })| border | size(WIDTH,EQUAL,80) | center;
        });

        auto screen = ScreenInteractive::TerminalOutput();
        screen.CaptureMouse();
        screen.Loop(result);

    }

    void menu_register() { // 注册界面
        std::string account; // 账号
        std::string password; // 密码
        std::string detectNumber; // 管理员邀请码
        std::string classid; // 班级组织
        std::string name; // 姓名
        std::string gender; // 性别
        auto screen	= ScreenInteractive::TerminalOutput();

        // input输入框接口组件
        Component input_account = Input(&account, "在此输入学号/工号");
        InputOption pwd_option;
        pwd_option.password = true;
        Component input_password = Input(&password, "在此输入想要设置的密码", pwd_option);
        Component input_classid = Input(&classid, "在此输入班级");
        Component input_name = Input(&name, "在此输入姓名");
        Component input_gender = Input(&gender, "在此输入性别");
        Component input_detectNumber = Input(&detectNumber, "在此输入识别码(可以为空)");

        std::string test = ""; // 测试用提示词

        Component button_register = Button("  注册", [&] { // 注册按钮
            //注册...
            if(account.empty() || password.empty() || classid.empty() || name.empty() || gender.empty() ) { // 检测所有框是否其中有为空
                test = "请输入完整信息";
            } else if(account.find("\\") != account.npos || account.find("/") != account.npos
                || account.find(":") != account.npos || account.find("*") != account.npos
                || account.find("?") != account.npos || account.find("\"") != account.npos
                || account.find("<") != account.npos || account.find(">") != account.npos
                || account.find("|") != account.npos || password.find("\\") != password.npos || password.find("/") != password.npos
                || password.find(":") != password.npos || password.find("*") != password.npos
                || password.find("?") != password.npos || password.find("\"") != password.npos
                || password.find("<") != password.npos || password.find(">") != password.npos
                || password.find("|") != password.npos) { // 判断用户名/密码是否包含非法字符
                    test    = "输入用户名/密码非法，不能包含 \\/:*?\"<>| 这些字符";
                    account = "";
                    password = "";
            } else { // 输入合法，判断用户名/密码是否存在

                bool checkExist = false;

                if (detectNumber!=detectNum) { // 检测输入邀请码是否正确
                    // 邀请码不正确或者为空
                    for (auto stu : list_stu) {
                        if (stu.getId()==account) {
                            test = "该账号已存在";
                            account = "";
                            password = "";
                            checkExist = true;
                        }
                    }

                    if (!checkExist) {
                        list_stu.emplace_back(base::Student(account, classid, name, gender, password));

                        save();

                        test = "注册成功，正在返回登录界面";
                        sleep(1);
                        menu_login();
                    }

                }else {
                    // 邀请码正确
                    for (auto man : list_man) {
                        if (man.getId() == account) {
                            test = "该账号已存在";
                            account = "";
                            password = "";
                            checkExist = true;
                        }
                    }

                    if (!checkExist) {
                        list_man.emplace_back(base::Manager(account, classid, name, gender, password));

                        save();

                        test = "注册成功，正在返回登录界面";
                        sleep(1);
                        test = "";
                        screen.Exit();
                    }

                }
            }

        }) | size(WIDTH,EQUAL,10) | center;

        Component button_back = Button("  返回", [&] { // 返回按钮
            test = "";
            screen.Exit();
        }) | size(WIDTH,EQUAL,10) | center;

        auto comp = Container::Vertical({ // 组件工具集
            input_account,
            input_password,
            input_name,
            input_gender,
            input_classid,
            input_detectNumber,
            button_register,
            button_back
        });

        auto renderer = Renderer(comp, [&] { // 界面渲染器

            return vbox({
                text("Hello " + name + " , 欢迎注册!"),
                separator(),
                hbox(text("账号: "), input_account->Render()),
                hbox(text("密码: "), input_password->Render()),
                hbox(text("姓名: "), input_name->Render()),
                hbox(text("性别: "), input_gender->Render()),
                hbox(text("班级: "), input_classid->Render()),
                hbox(text("邀请码: "), input_detectNumber->Render()),

            }) | border | size(WIDTH,EQUAL,60) | center;
        });

        auto result = Renderer(renderer,[&] { // 外框架渲染界面
            return vbox({
                text("浙江工业大学机房收费管理系统欢迎您") | bold | color(LinearGradient(45,Color::Pink1,Color::White)) | center,
                renderer->Render(),
                text(test) | center | bold | color(Color::Red),
                hbox(button_register->Render(), separatorEmpty(), button_back->Render()) | center
            })| border | size(WIDTH,EQUAL,80) | center;
        });

        screen.CaptureMouse();
        screen.Loop(result);
    }

    void menu_modify() { // 修改密码界面
        // 一堆修改密码需要的变量
        std::string account;
        std::string password;
        std::string detectNumber;
        std::string classid;
        std::string name;
        std::string gender;
        auto screen	= ScreenInteractive::TerminalOutput();

        // 创建输入组件
        Component input_account = Input(&account, "在此输入您的学号/工号");
        InputOption pwd_option;
        pwd_option.password = true;
        Component input_password = Input(&password, "在此输入修改后的密码", pwd_option);
        Component input_name = Input(&name, "在此输入您的真实姓名以验证");

        std::string test = ""; // 测试用提示词

        Component button_register = Button("  更改", [&] { // 更改按钮

            if (account.empty() || password.empty() || name.empty()) { // 输入为空时
                test = "请输入完整信息";
            }else {
                bool checkExist = false;
                //  检查用户在管理员用户库和学生用户库是否存在
                for (auto &stu : list_stu) {
                    if (stu.getId() == account) {
                        checkExist = true;
                        if (stu.getName() == name) {
                            stu.setPassword(password);
                            save();
                            test = "修改完成 请重新登录";
                            sleep(1);
                            screen.Exit();
                            break;
                        }else {
                            test = "该账号原信息与验证姓名不匹配";
                            name = "";
                        }
                    }
                }

                for (auto &man : list_man) {
                    if (man.getId() == account) {
                        checkExist = true;
                        if (man.getName() == name) {
                            man.setPassword(password);
                            save();
                            test = "修改完成 请重新登录";
                            sleep(1);
                            // screen.Exit();
                            menu_login();
                            break;
                        }else {
                            test = "该账号原信息与验证姓名不匹配";
                            name = "";
                        }
                    }
                }

                // 检查该用户名是否存在
                if (!checkExist) {
                        test = "不存在该账户";
                        name = "";
                        account = "";
                        password = "";
                }
            }
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_back = Button("  返回", [&] { // 返回按钮
            test = "";
            screen.Exit();
        }) | size(WIDTH,EQUAL,10) | center;

        auto comp = Container::Vertical({ // 组件工具集
            input_account,
            input_password,
            input_name,
            button_register,
            button_back
        });

        auto renderer = Renderer(comp, [&] { // 主界面渲染器

            return vbox({
                text("正在对" + account + " 的账户进行修改!"),
                separator(),
                hbox(text("修改账号: "), input_account->Render()),
                hbox(text("真实姓名: "), input_name->Render()),
                hbox(text("修改后密码: "), input_password->Render()),

            }) | border | size(WIDTH,EQUAL,60) | center;
        });

        auto result = Renderer(renderer,[&] { // 外框架渲染
            return vbox({
                text("浙江工业大学机房收费管理系统欢迎您") | bold | color(LinearGradient(45,Color::Pink1,Color::White)) | center,
                renderer->Render(),
                text(test) | center | bold | color(Color::Red),
                hbox(button_register->Render(), separatorEmpty(), button_back->Render()) | center
            })| border | size(WIDTH,EQUAL,80) | center;
        });

        screen.CaptureMouse();
        screen.Loop(result);
    }

    // 为了模拟多线程的使用，定义这些全局变量，实现界面切换时线程之间的数据交换
    inline int flag = 0; // 主模式选择
    inline int num_modify = 0; // 修改序号选择
    inline int num1 = 0; // 排序项目选择
    inline int num2 = 0; // 排序方式选择
    inline int num3 = 0; // 查询机器状态选择
    inline int num_find = 0; // 模糊查找项目选择
    std::string browseSTR = ""; // 模糊查找关键词

    void menu_man_check(const std::string &account) { // 管理员可视界面

        // 查找当前传入的人员信息
        int man_index = 0;

        for (int i = 0; i < list_man.size(); ++i) {
            if (list_man[i].getId() == account) {
                man_index = i;
                break;
            }
        }

        // 设置当前人员基本信息的变量
        std::string _name = list_man[man_index].getName();
        std::string _id = list_man[man_index].getId();
        std::string _classid = list_man[man_index].getClassId();
        std::string _gender = list_man[man_index].getGender();

        // 界面系统主要交换记录数据库（处理表格，选项等信息）
        std::vector<std::vector<std::string>> data_base; // 基础系统记录数据库
        data_base.emplace_back(std::vector<std::string>{" 学号 "," 姓名 "," 性别 "," 班级 "," 上机时间 "," 下机时间 "," 上机机器 "," 上机费用 "}); // 初始化基础数据库
        std::vector<std::vector<std::string>> data_CMP = data_base; // 比较模式 交换记录数据库
        std::vector<std::vector<std::string>> data_MACH = data_base; // 机器查询 交换记录数据库
        std::vector<std::vector<std::string>> data_FIND = data_base; // 查找数据 交换记录数据库

        int toggled_selected_num = num1; // 储存排序项目选择的变量
        int toggled_selected_num2 = num2; // 储存排序方式选择的变量

        auto screen = ScreenInteractive::TerminalOutput();
        std::string test = ""; // 检测提示词

        // 一堆功能界面 以model开头的（
        Component model_base;
        Component model_find;
        Component model_compare;
        Component model_machine_check;
        Component model_cal;
        Component model_exit;
        Component model_setting;
        Component model_modify_record;

        // button按钮功能区
        Component button_find = Button("  查询", [&] { // 查询按钮功能
            model_base = model_find;
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_compare = Button("  统计", [&] { // 统计按钮功能
            model_base = model_compare;
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_machine_check = Button("  监控", [&] { // 机器实时监控按钮功能
            model_base = model_machine_check;
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_exit = Button("  注销", [&] { // 退出系统按钮功能
            model_base = model_exit;
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_setting = Button("  设置", [&] { // 系统参数设置按钮功能
            model_base = model_setting;
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_out1 = Button("  重登", [&] { // 退出系统按钮功能 中的 重新登陆功能按钮
            menu_login();
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_out2 = Button("  退出", [&] { // 退出系统按钮功能 中的 退出系统功能按钮
            exit(EXIT_SUCCESS);
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_modify_record = Button("  修改", [&] { // 修改记录按钮功能
            model_base = model_modify_record;
        }) | size(WIDTH,EQUAL,10) | center;

        for (auto rec : list_rec) { // 遍历记录列表 完全初始化 base 数据库
            std::vector<std::string> temp;

            temp.emplace_back(rec.getId()+"  ");
            temp.emplace_back(rec.getName()+"  ");
            temp.emplace_back(rec.getGender()+"  ");
            temp.emplace_back(rec.getClassId()+"  ");

            std::string time1 = std::to_string(rec.getStartTime().tm_year + 1900) + "-" + std::to_string(rec.getStartTime().tm_mon + 1)+ "-" + std::to_string(rec.getStartTime().tm_mday) + " " + std::to_string(rec.getStartTime().tm_hour) + ":" + std::to_string(rec.getStartTime().tm_min) + ":" + std::to_string(rec.getStartTime().tm_sec);
            std::string time2 = std::to_string(rec.getEndTime().tm_year + 1900) + "-" + std::to_string(rec.getEndTime().tm_mon + 1) + "-" + std::to_string(rec.getEndTime().tm_mday) + " " + std::to_string(rec.getEndTime().tm_hour) + ":" + std::to_string(rec.getEndTime().tm_min) + ":" + std::to_string(rec.getEndTime().tm_sec);

            temp.emplace_back(time1+"  ");
            temp.emplace_back(time2+"  ");
            temp.emplace_back(std::to_string(rec.getMachineId()+1) + "号机器  ");
            temp.emplace_back(std::format("{}",timeX::timeCal(rec.getStartTime(),rec.getEndTime())));

            data_base.emplace_back(temp);
        }

        // 记录排序功能实现 以下-------------------------------------------------------------------------------------------------------------------------------------------

        data_CMP = data_base; // 将基础数据库复制给排序数据库

        // 创建选择栏目组件
        std::vector<std::string> header = {" 学号 "," 姓名 "," 班级 ", " 上机开始时间 ", " 上机时长 "};
        Component toggle1 = Toggle(&header, &toggled_selected_num);

        std::vector<std::string> header2 = {" 降序 "," 升序 "};
        Component toggle2 = Toggle(&header2, &toggled_selected_num2);

        Component button_confirm_compare = Button("   OK", [&] { // 确认排序 实现排序
            flag = 1; // 排序功能的flag表示
            num1 = toggled_selected_num; // 传出值1
            num2 = toggled_selected_num2; // 传出值2
            menu_man_check(account); // 还原
            flag = 0;
        }) | size(HEIGHT,EQUAL,3) | size(WIDTH,EQUAL,10);

        // 排序规则选择
        std::vector<base::Record> data_records = list_rec;
        if (toggled_selected_num == 0) {

            if (toggled_selected_num2 == 0) {
                std::sort(data_records.begin(), data_records.end(), cmp::cmp_id_down);
            }else {
                std::sort(data_records.begin(), data_records.end(), cmp::cmp_id_up);
            }

        }else if (toggled_selected_num == 1) {

            if (toggled_selected_num2 == 0) {
                std::sort(data_records.begin(), data_records.end(), cmp::cmp_name_down);
            }else {
                std::sort(data_records.begin(), data_records.end(), cmp::cmp_name_up);
            }

        }else if (toggled_selected_num == 2) {

            if (toggled_selected_num2 == 0) {
                std::sort(data_records.begin(), data_records.end(), cmp::cmp_classId_down);
            }else {
                std::sort(data_records.begin(), data_records.end(), cmp::cmp_classId_up);
            }

        }else if (toggled_selected_num == 3) {

            if (toggled_selected_num2 == 0) {
                std::sort(data_records.begin(), data_records.end(), cmp::cmp_startTime_down);
            }else {
                std::sort(data_records.begin(), data_records.end(), cmp::cmp_startTime_up);
            }

        }else if (toggled_selected_num == 4) {

            if (toggled_selected_num2 == 0) {
                std::sort(data_records.begin(), data_records.end(), cmp::cmp_duringTime_down);
            }else {
                std::sort(data_records.begin(), data_records.end(), cmp::cmp_duringTime_up);
            }

        }

        data_CMP.clear(); // 清空比较数据库
        data_CMP.emplace_back(std::vector<std::string>{" 学号 "," 姓名 "," 性别 "," 班级 "," 上机时间 "," 下机时间 "," 上机机器 "," 上机费用 "});

        for (auto &rec : data_records) { // 遍历记录 添加比较数据库
            std::vector<std::string> temp;

            temp.emplace_back(rec.getId()+"  ");
            temp.emplace_back(rec.getName()+"  ");
            temp.emplace_back(rec.getGender()+"  ");
            temp.emplace_back(rec.getClassId()+"  ");

            std::string time1 = std::to_string(rec.getStartTime().tm_year + 1900) + "-" + std::to_string(rec.getStartTime().tm_mon + 1) + "-" + std::to_string(rec.getStartTime().tm_mday) + " " + std::to_string(rec.getStartTime().tm_hour) + ":" + std::to_string(rec.getStartTime().tm_min) + ":" + std::to_string(rec.getStartTime().tm_sec);
            std::string time2 = std::to_string(rec.getEndTime().tm_year + 1900) + "-" + std::to_string(rec.getEndTime().tm_mon + 1) + "-" + std::to_string(rec.getEndTime().tm_mday) + " " + std::to_string(rec.getEndTime().tm_hour) + ":" + std::to_string(rec.getEndTime().tm_min) + ":" + std::to_string(rec.getEndTime().tm_sec);

            temp.emplace_back(time1+"  ");
            temp.emplace_back(time2+"  ");
            temp.emplace_back(std::to_string(rec.getMachineId()+1) + "号机器  ");
            temp.emplace_back(std::format("{}",static_cast<int>(cash_index * timeX::timeCal(rec.getStartTime(),rec.getEndTime()))));

            data_CMP.emplace_back(temp);
        }

        // 设置比较数据库呈现的表格组件 以及一些美化组件
        auto characters = Table({data_CMP});
        characters.SelectAll().Border(LIGHT);
        characters.SelectRow(0).Decorate(bold);
        characters.SelectRow(0).SeparatorVertical(LIGHT);
        characters.SelectRow(0).Border(DOUBLE);

        auto content = characters.SelectRows(1, -1);

        content.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
        content.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
        content.DecorateCellsAlternateRow(color(Color::White), 3, 2);

        auto table = characters.Render(); // 这个就是设置后的表格接口了


        // 机器查询以及实时监控功能实现 以下-------------------------------------------------------------------------------------------------------------------------------------

        int select_machine = num3; // 传入值
        Element cond_base = separatorEmpty(); // 默认状态
        Element cond_Free = vbox({text("空闲中") | color(Color::Green) | bold | center, colorTile(0,255,0,10,1) | center}) | center; // 空闲状态
        Element cond_Busy = vbox({text("占用中") | color(Color::Red) | bold | center, colorTile(255,0,0,10,1) | center}) | center; // 占用状态
        // 以上3个Element为三个状态的显示，其中内嵌了颜色块，颜色块为colorTile函数返回的Element来醒目表示状态

        for (auto &mach : list_mach) { // 检测当前选中的机位是否是被占用的
            if (mach.getId() == select_machine + 1) {
                if (mach.getFlag()) {
                    cond_base = cond_Busy;
                }else cond_base = cond_Free;
            }
        }

        std::vector<std::string> string_mach; // 排版机位信息以供dropdown列表使用
        for (auto mach : list_mach) {
            string_mach.emplace_back(std::to_string(mach.getId()) + "号机位");
        }

        auto layout = Container::Vertical({ // 垂直布局的dropdown组件实现
            Container::Horizontal({
                Dropdown(&string_mach, &select_machine),
            }),
        });

        data_MACH.clear(); // 清空机器状态数据
        data_MACH.emplace_back(std::vector<std::string>{" 学号 "," 姓名 "," 班级 "," 上机时间 "," 下机时间 "," 上机费用 "});

        for (auto &rec : data_records) { // 遍历所有记录 完善机器状态数据库

            if (rec.getMachineId() == select_machine) {
                std::vector<std::string> temp;

                temp.emplace_back(rec.getId()+"  ");
                temp.emplace_back(rec.getName()+"  ");
                temp.emplace_back(rec.getClassId()+"  ");

                std::string time1 = std::to_string(rec.getStartTime().tm_year + 1900) + "-" + std::to_string(rec.getStartTime().tm_mon + 1) + "-" + std::to_string(rec.getStartTime().tm_mday) + " " + std::to_string(rec.getStartTime().tm_hour) + ":" + std::to_string(rec.getStartTime().tm_min) + ":" + std::to_string(rec.getStartTime().tm_sec);
                std::string time2 = std::to_string(rec.getEndTime().tm_year + 1900) + "-" + std::to_string(rec.getEndTime().tm_mon + 1) + "-" + std::to_string(rec.getEndTime().tm_mday) + " " + std::to_string(rec.getEndTime().tm_hour) + ":" + std::to_string(rec.getEndTime().tm_min) + ":" + std::to_string(rec.getEndTime().tm_sec);

                temp.emplace_back(time1+"  ");
                temp.emplace_back(time2+"  ");
                temp.emplace_back(std::format("{}",static_cast<int>(cash_index * timeX::timeCal(rec.getStartTime(),rec.getEndTime()))));

                data_MACH.emplace_back(temp);
            }
        }

        // 呈现机器状态的表格 表示当前机器在记录库中历史使用记录数据
        auto characters2 = Table({data_MACH});
        characters2.SelectAll().Border(LIGHT);
        characters2.SelectRow(0).Decorate(bold);
        characters2.SelectRow(0).SeparatorVertical(LIGHT);
        characters2.SelectRow(0).Border(DOUBLE);

        auto content2 = characters2.SelectRows(1, -1);

        content2.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
        content2.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
        content2.DecorateCellsAlternateRow(color(Color::White), 3, 2);

        auto table2 = characters2.Render(); // 最终呈现出的机器状态数据表格

        auto button_confirm_mach = Button(" ENTER ", [&] { // 确认查询机器状态的功能按钮

            for (auto &mach : list_mach) { // 检测当前选中的机位是否是被占用的
                if (mach.getId() == select_machine + 1) {
                    if (mach.getFlag()) {
                        cond_base = cond_Busy;
                    }else cond_base = cond_Free;
                }
            }

            flag = 2;
            num3 = select_machine;
            menu_man_check(account);
            flag = 0;
        }) | center | size(WIDTH,EQUAL,12);

        // 系统参数设置功能实现 以下-----------------------------------------------------------------------------------------------------------------------------------------


        // 创建机器总数和管理员邀请码的输入框组件
        std::string input_MachNum = "";
        auto input_sys_machineNum = Input(&input_MachNum, std::to_string(machineNum));

        std::string input_detectNum = "";
        auto input_sys_detectNum = Input(&input_detectNum, detectNum);

        // 判断输入是否为数字 lambda函数
        auto isNum = [](const std::string &str) {
            for (auto &c : str) {
                if (c<'0'||c>'9') return false;
            }
            return true;
        };

        std::string test1 = "",test2 = ""; // 检测提示词
        Component button_modMachNum = Button(" 修改 ", [&] { // 修改功能按钮

            if (!input_MachNum.empty()) {
                if (isNum(input_MachNum)) {
                    int tempNum = std::stoi(input_MachNum);

                    if (tempNum >= machineNum) {
                        for (int i = machineNum; i < tempNum; i++) {
                            list_mach.emplace_back(i+1);
                        }
                    }else list_mach.resize(tempNum);

                    machineNum = tempNum;
                    test1 = "机房机器数量修改成功";
                }else {
                    test1  = "修改失败: 输入非数字";
                }
            }

            if (!input_detectNum.empty()) {
                detectNum = input_detectNum;
                test2 = "机房管理员邀请码修改成功";
            }

            save(); // 保存
        }) | center | size(WIDTH,EQUAL,12);

        // 记录查询界面功能实现 以下-----------------------------------------------------------------------------------------------------------------------------------------------

        // 创建模糊查询关键词输入框组件
        std::string input_browse = browseSTR; // 传入值
        auto input_sys_find = Input(&input_browse, "在此输入关键词");

        // 创建模糊查询选项组件
        int toggled_selected_num3 = num_find;
        std::vector<std::string> toggle = {" 学号 "," 姓名 "," 班级 "};
        Component toggle_find = Toggle(&toggle, &toggled_selected_num3);

        //  清空查询数据库
        data_FIND.clear();
        data_FIND.emplace_back(std::vector<std::string>{" 学号 "," 姓名 "," 性别 "," 班级 "," 上机时间 "," 下机时间 "," 上机机器 "," 上机费用 "});

        for (const auto &rec : data_records) { // 根据传入的选项和关键词查询数据库中内容 并添加到模糊查询数据库中
            std::vector<std::string> temp;
            // 先选择选项再查找关键词
            if (toggled_selected_num3 == 0) {
                if (rec.getId().find(browseSTR) != std::string::npos || browseSTR.empty()) { //debug tag
                    temp.emplace_back(rec.getId()+"  ");
                    temp.emplace_back(rec.getName()+"  ");
                    temp.emplace_back(rec.getGender()+"  ");
                    temp.emplace_back(rec.getClassId()+"  ");

                    std::string time1 = std::to_string(rec.getStartTime().tm_year + 1900) + "-" + std::to_string(rec.getStartTime().tm_mon + 1) + "-" + std::to_string(rec.getStartTime().tm_mday) + " " + std::to_string(rec.getStartTime().tm_hour) + ":" + std::to_string(rec.getStartTime().tm_min) + ":" + std::to_string(rec.getStartTime().tm_sec);
                    std::string time2 = std::to_string(rec.getEndTime().tm_year + 1900) + "-" + std::to_string(rec.getEndTime().tm_mon + 1) + "-" + std::to_string(rec.getEndTime().tm_mday) + " " + std::to_string(rec.getEndTime().tm_hour) + ":" + std::to_string(rec.getEndTime().tm_min) + ":" + std::to_string(rec.getEndTime().tm_sec);

                    temp.emplace_back(time1+"  ");
                    temp.emplace_back(time2+"  ");
                    temp.emplace_back(std::to_string(rec.getMachineId()+1) + "号机器  ");
                    temp.emplace_back(std::format("{}",static_cast<int>(cash_index * timeX::timeCal(rec.getStartTime(),rec.getEndTime()))));
                }
            }else if (toggled_selected_num3 == 1) {
                if (rec.getName().find(browseSTR) != std::string::npos || browseSTR.empty()) {
                    temp.emplace_back(rec.getId()+"  ");
                    temp.emplace_back(rec.getName()+"  ");
                    temp.emplace_back(rec.getGender()+"  ");
                    temp.emplace_back(rec.getClassId()+"  ");

                    std::string time1 = std::to_string(rec.getStartTime().tm_year + 1900) + "-" + std::to_string(rec.getStartTime().tm_mon + 1) + "-" + std::to_string(rec.getStartTime().tm_mday) + " " + std::to_string(rec.getStartTime().tm_hour) + ":" + std::to_string(rec.getStartTime().tm_min) + ":" + std::to_string(rec.getStartTime().tm_sec);
                    std::string time2 = std::to_string(rec.getEndTime().tm_year + 1900) + "-" + std::to_string(rec.getEndTime().tm_mon + 1) + "-" + std::to_string(rec.getEndTime().tm_mday) + " " + std::to_string(rec.getEndTime().tm_hour) + ":" + std::to_string(rec.getEndTime().tm_min) + ":" + std::to_string(rec.getEndTime().tm_sec);

                    temp.emplace_back(time1+"  ");
                    temp.emplace_back(time2+"  ");
                    temp.emplace_back(std::to_string(rec.getMachineId()+1) + "号机器  ");
                    temp.emplace_back(std::format("{}",static_cast<int>(cash_index * timeX::timeCal(rec.getStartTime(),rec.getEndTime()))));
                }
            }else {
                if (rec.getClassId().find(browseSTR) != std::string::npos || browseSTR.empty()) {
                    temp.emplace_back(rec.getId()+"  ");
                    temp.emplace_back(rec.getName()+"  ");
                    temp.emplace_back(rec.getGender()+"  ");
                    temp.emplace_back(rec.getClassId()+"  ");

                    std::string time1 = std::to_string(rec.getStartTime().tm_year + 1900) + "-" + std::to_string(rec.getStartTime().tm_mon + 1) + "-" + std::to_string(rec.getStartTime().tm_mday) + " " + std::to_string(rec.getStartTime().tm_hour) + ":" + std::to_string(rec.getStartTime().tm_min) + ":" + std::to_string(rec.getStartTime().tm_sec);
                    std::string time2 = std::to_string(rec.getEndTime().tm_year + 1900) + "-" + std::to_string(rec.getEndTime().tm_mon + 1) + "-" + std::to_string(rec.getEndTime().tm_mday) + " " + std::to_string(rec.getEndTime().tm_hour) + ":" + std::to_string(rec.getEndTime().tm_min) + ":" + std::to_string(rec.getEndTime().tm_sec);

                    temp.emplace_back(time1+"  ");
                    temp.emplace_back(time2+"  ");
                    temp.emplace_back(std::to_string(rec.getMachineId()+1) + "号机器  ");
                    temp.emplace_back(std::format("{}",static_cast<int>(cash_index * timeX::timeCal(rec.getStartTime(),rec.getEndTime()))));
                }
            }

            if (!temp.empty()) data_FIND.emplace_back(temp);
        }

        // 呈现模糊查询数据库的表格
        auto characters_find = Table({data_FIND});
        characters_find.SelectAll().Border(LIGHT);
        characters_find.SelectRow(0).Decorate(bold);
        characters_find.SelectRow(0).SeparatorVertical(LIGHT);
        characters_find.SelectRow(0).Border(DOUBLE);

        auto content_find = characters_find.SelectRows(1, -1);

        content_find.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
        content_find.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
        content_find.DecorateCellsAlternateRow(color(Color::White), 3, 2);

        auto table_find = characters_find.Render(); // 最后呈现的数据表格

        Component button_find_confirm = Button(" 确认 ", [&] { // 查找确认功能实现按钮
            flag = 3;
            num_find = toggled_selected_num3;
            browseSTR = input_browse;
            menu_man_check(account);
            flag = 0;
        }) | center | size(WIDTH,EQUAL,12);

        // 修改记录界面功能实现 以下-----------------------------------------------------------------------------------------------------------------------------------------------

        std::vector<std::vector<std::string>> data_MOD = data_base;

        for (int i = 0; i < data_MOD.size(); i++) { // 初始化修改记录数据库表格
            if (i==0) {
                data_MOD[i].emplace_back("记录序号");
            }else data_MOD[i].emplace_back(std::to_string(i));
        }

        // 创建总览记录数据库及其表格样式
        auto characters_base = Table({data_MOD}); // modify
        characters_base.SelectAll().Border(LIGHT);
        characters_base.SelectRow(0).Decorate(bold);
        characters_base.SelectRow(0).SeparatorVertical(LIGHT);
        characters_base.SelectRow(0).Border(DOUBLE);

        auto content_base = characters_base.SelectRows(1, -1);

        content_base.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
        content_base.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
        content_base.DecorateCellsAlternateRow(color(Color::White), 3, 2);

        auto table_base = characters_base.Render(); // 最终呈现的表格组件

        int selected_numId = num_modify; // 传入的numId
        std::vector<std::string> string_numId; // 设置好dropdown所需要的列表
        for (int i = 1; i < data_MOD.size(); i++) {
            string_numId.emplace_back(std::to_string(i));
        }

        auto layout2 = Container::Vertical({ // dropdown样式的记录选择组件
            Container::Horizontal({
                Dropdown(&string_numId, &selected_numId),
            }),
        });

        // 一堆修改记录输入框的组件
        std::string input_mod_id = "";
        auto input_modify_mod_id = Input(&input_mod_id, "在此输入修改后的学号");

        std::string input_mod_name = "";
        auto input_modify_mod_name = Input(&input_mod_name, "在此输入修改后的姓名");

        std::string input_mod_gender = "";
        auto input_modify_mod_gender = Input(&input_mod_gender, "在此输入修改后的性别");

        std::string input_mod_classId = "";
        auto input_modify_mod_classId = Input(&input_mod_classId, "在此输入修改后的班级");

        std::string input_mod_startTime = "";
        auto input_modify_mod_startTime = Input(&input_mod_startTime, "在此输入需修改的开始时间");

        std::string input_mod_endTime = "";
        auto input_modify_mod_endTime = Input(&input_mod_endTime, "在此输入需修改的结束时间");

        std::string test_mod_time = "请使用 \"xxxx-xx-xx xx:xx:xx\" 的时间格式"; // 固定提示词


        Component button_modify_modify = Button(" 修改 ", [&] { // 修改功能实现按钮

            // 输入为空检查
            if (input_mod_name.empty() && input_mod_id.empty() && input_mod_gender.empty() && input_mod_classId.empty() && input_mod_startTime.empty() && input_mod_endTime.empty()) {
                test_modify = "请填写至少一行需要修改的数据！";
                return;
            }

            if (!input_mod_id.empty()) list_rec[selected_numId].setId(input_mod_id);
            if (!input_mod_name.empty()) list_rec[selected_numId].setName(input_mod_name);
            if (!input_mod_gender.empty()) list_rec[selected_numId].setGender(input_mod_gender);
            if (!input_mod_classId.empty()) list_rec[selected_numId].setClassId(input_mod_classId);

            // 输入时间格式检查
            if (!input_mod_startTime.empty()) {
                if (input_mod_startTime.size() != 19) { // 时间字符串长度检查
                    test_modify = "请填写正确的开始时间格式!";
                    return;
                }

                // 时间字符串格式检查
                std::string str_year = input_mod_startTime.substr(0, 4);
                std::string str_month = input_mod_startTime.substr(5, 2);
                std::string str_day = input_mod_startTime.substr(8, 2);
                std::string str_hour = input_mod_startTime.substr(11, 2);
                std::string str_minute = input_mod_startTime.substr(14, 2);
                std::string str_second = input_mod_startTime.substr(17, 2);

                if (!isNum(str_year) || !isNum(str_month) || !isNum(str_day) || !isNum(str_hour) || !isNum(str_minute) || !isNum(str_second)) {
                    test_modify = "输入时间分析错误！请填写正确格式!";
                    return;
                }

                int year = std::stoi(str_year);
                int month = std::stoi(str_month);
                int day = std::stoi(str_day);
                int hour = std::stoi(str_hour);
                int minute = std::stoi(str_minute);
                int second = std::stoi(str_second);

                tm mod_startTime = timeX::timeTrans(year, month, day, hour, minute, second); // 输入时间转换成tm格式
                // 开始时间不能晚于结束时间
                if (timeX::timeComp(list_rec[selected_numId].getEndTime(), mod_startTime) == false) { // 如果结束时间早于开始时间，则提示错误
                    test_modify = "开始时间不能晚于结束时间!";
                    return;
                }

                if (timeX::timeCheck(mod_startTime) == false) { // 如果输入时间数据常识性错误，则提示错误
                    test_modify = "输入时间数据错误！请填写正确格式!";
                    return;
                }

                list_rec[selected_numId].setStartTime(mod_startTime); // 修改开始时间
            }

            // 结束时间格式检查
            if (!input_mod_endTime.empty()) {
                if (input_mod_endTime.size() != 19) { // 如果输入时间数据长度错误，则提示错误
                    test_modify = "请填写正确的结束时间格式!";
                    return;
                }

                // 判断输入时间格式是否正确
                std::string str_year = input_mod_endTime.substr(0, 4);
                std::string str_month = input_mod_endTime.substr(5, 2);
                std::string str_day = input_mod_endTime.substr(8, 2);
                std::string str_hour = input_mod_endTime.substr(11, 2);
                std::string str_minute = input_mod_endTime.substr(14, 2);
                std::string str_second = input_mod_endTime.substr(17, 2);

                if (!isNum(str_year) || !isNum(str_month) || !isNum(str_day) || !isNum(str_hour) || !isNum(str_minute) || !isNum(str_second)) {
                    test_modify = "输入时间分析错误！请填写正确格式!";
                    return;
                }

                int year = std::stoi(str_year);
                int month = std::stoi(str_month);
                int day = std::stoi(str_day);
                int hour = std::stoi(str_hour);
                int minute = std::stoi(str_minute);
                int second = std::stoi(str_second);

                tm mod_endTime = timeX::timeTrans(year, month, day, hour, minute, second); // 将输入的时间转换为tm结构体
                if (timeX::timeCheck(mod_endTime) == false) { // 判断输入的时间是否有常识性错误
                    test_modify = "输入时间数据错误！请填写正确格式!";
                    return;
                }
                // 结束时间不能早于开始时间
                if (timeX::timeComp(list_rec[selected_numId].getStartTime(), mod_endTime) == true) {
                    test_modify = "结束时间不能早于开始时间!";
                    return;
                }
                list_rec[selected_numId].setEndTime(mod_endTime);
            }

            save(); // 保存修改
            test_modify = "修改成功"; // 显示修改成功
            flag = 4;
            menu_man_check(account); // 显示刷新
            flag = 0;
        }) | center | size(WIDTH,EQUAL,12);

        Component button_modify_delete = Button(" 删除 ", [&] { // 删除功能实现按钮

            list_rec.erase(list_rec.begin()+selected_numId); // 删除记录

            save();
            test_modify = "删除成功";
            flag = 4;
            menu_man_check(account); // 显示刷新
            flag = 0;
        }) | center | size(WIDTH,EQUAL,12);

        auto comp = Container::Vertical({ // 交互组件功能工具集
            button_find,
            button_compare,
            button_machine_check,
            button_exit,
            button_out1,
            button_out2,
            button_modify_record,
            toggle1,
            toggle2,
            button_confirm_compare,
            layout,
            button_confirm_mach,
            button_setting,
            button_modMachNum,
            input_sys_detectNum,
            input_sys_machineNum,
            button_find_confirm,
            input_sys_find,
            toggle_find,
            layout2,
            button_modify_modify,
            button_modify_delete,
            input_modify_mod_classId,
            input_modify_mod_gender,
            input_modify_mod_id,
            input_modify_mod_name,
            input_modify_mod_startTime,
            input_modify_mod_endTime,

        });

        model_compare = Renderer(comp, [&] { // 排序界面渲染器
            return vbox({
                text("统计总览界面 | 排序") | bold | color(Color::Gold1) | center,
                separatorDouble(),
                hbox({
                    separator(),
                    text(" 查询选项：") | bold | color(Color::Gold1),
                    separator(),
                    toggle1->Render(),
                    separator(),
                    toggle2->Render(),
                    separator(),
                }) | center,
                // separator() | size(WIDTH,EQUAL,73),
                table | size(WIDTH,EQUAL,110) | center,
                separatorEmpty(),
                separatorEmpty(),
                button_confirm_compare->Render() | center | bold,
                separatorEmpty(),

            })| center | size(WIDTH,EQUAL,110);
        }) | size(WIDTH,EQUAL,110) | center;

        model_exit = Renderer(comp, [&] { // 退出界面渲染器
            return hbox({
                vbox({
                    separatorEmpty(),
                    text("重新登录？") | bold | color(Color::Gold1),
                    separator(),
                    button_out1 ->Render() | bold,

                }) | center | border | size(WIDTH,EQUAL,40) | size(HEIGHT,EQUAL,10),
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                vbox({
                    separatorEmpty(),
                    text("退出系统？") | bold | color(Color::Gold1),
                    separator(),
                    button_out2 ->Render() | bold,

                }) | center | border | size(WIDTH,EQUAL,40) | size(HEIGHT,EQUAL,10),
            }) | center;
        });

        // 机器查询界面渲染器
        model_machine_check = Renderer(comp, [&] { // 做成抽屉查询，竖直排列分别是抽屉选择，确定按钮，test&状态图标色块（红绿区分），表格为最近使用这台机器的人

            return vbox({
                text("机房机位记录查询") | center | bold | color(Color::Gold1),
                separator(),
                hbox({
                    vbox({
                        separatorEmpty(),
                        text("请选择机位: "),
                    }),
                    layout->Render() | center,
                    separatorEmpty(),
                    button_confirm_mach->Render(),
                    separatorEmpty(),

                }) | center,
                cond_base | center,
                separatorEmpty(),
                table2 | size(WIDTH,EQUAL,100) | center,
                separatorEmpty(),

            }) | center;
        }) | size(WIDTH,EQUAL,100) | center;

        model_find = Renderer(comp, [&] { // 模糊搜索查询界面渲染器
            return vbox({
                text("模糊查询搜索界面") | bold | color(Color::Gold1) | center,
                separatorDouble(),
                hbox({
                    separator(),
                    text(" 搜索: ") | bold | color(Color::Gold1),
                    input_sys_find->Render() | size(WIDTH,EQUAL,36),
                    separator(),
                    text(" 搜索选项: ") | bold | color(Color::Gold1),
                    separator(),
                    toggle_find->Render(),
                    separator(),
                }) | center,
                table_find | size(WIDTH,EQUAL,110) | center,
                separatorEmpty(),
                separatorEmpty(),
                button_find_confirm->Render() | center | bold,
                separatorEmpty(),

            }) | center;
        }) | border | size(WIDTH,EQUAL,110) | center;

        model_setting = Renderer(comp, [&] { // 设置界面渲染器
            return vbox({
                text("机房及系统参数修改界面") | center | bold | color(Color::Gold1),
                separator() | size(WIDTH, EQUAL, 73),
                hbox(text("  机器数量: ") | bold,  input_sys_machineNum->Render()),
                hbox(text("  管理邀请码: ") | bold,  input_sys_detectNum->Render()),
                separatorEmpty(),
                button_modMachNum->Render() | center,
                text(test1) | color(Color::Red) | center,
                text(test2) | color(Color::Red) | center,

            }) | border;
        }) | size(WIDTH,EQUAL,80) | center;

        model_modify_record = Renderer(comp,[&] { // 修改记录界面渲染器
            return vbox({
                text("修改记录界面") | bold | center | color(Color::Gold1),
                separator(),
                table_base | center | size(WIDTH,EQUAL,150) | center,
                separatorEmpty(),
                hbox({
                    vbox({
                        separatorEmpty(),
                        text("请选择操作序号: ") | bold,
                    }),
                    layout2->Render() | center,
                    separatorEmpty(),
                })| center | border | size(WIDTH,EQUAL,60)| center,
                separatorEmpty(),
                hbox({

                    vbox({
                        hbox({text("学号: ") | bold, input_modify_mod_id->Render()}) | center,
                        hbox({text("姓名: ") | bold, input_modify_mod_name->Render()}) | center,
                        hbox({text("性别: ") | bold, input_modify_mod_gender->Render()}) | center,
                        hbox({text("班级: ") | bold, input_modify_mod_classId->Render()}) | center,
                    }) | center,
                    separatorEmpty(),
                    separator(),
                    separatorEmpty(),
                    vbox({

                        hbox({text("开始时间: ") | bold, input_modify_mod_startTime->Render()}) | center,
                        hbox({text("结束时间: ") | bold, input_modify_mod_endTime->Render()}) | center,
                        text("Warning：输入账号姓名班级时请注意格式")| bold | color(Color::Red) | center,
                        text(test_mod_time) | bold | color(Color::Red) | center,

                    }) | center,

                }) | center,
                separatorEmpty(),
                hbox({button_modify_modify->Render(), separator(), button_modify_delete->Render()}) | center ,
                text(test_modify) | bold | color(Color::Red) | center,

            });
        }) | size(WIDTH,EQUAL,150) | center | border;

        model_base = Renderer(comp, [&] { // 欢迎界面渲染器
            return vbox({
                text(" ________    _____   __  __   ______   ") | center | bold,
                text("/\\_____  \\  /\\___ \\ /\\ \\/\\ \\ /\\__  _\\  ") | center | bold,
                text("\\/____//'/' \\/__/\\ \\ \\ \\ \\ \\\\/_/\\ \\/  ") | center | bold,
                text("     //'/'     _\\ \\ \\\\ \\ \\ \\ \\  \\ \\ \\  ") | center | bold,
                text("    //'/'___  /\\ \\_\\ \\\\ \\ \\_\\ \\  \\ \\ \\ ") | center | bold,
                text("    /\\_______\\\\ \\____/ \\ \\_____\\  \\ \\_\\") | center | bold,
                text("    \\/_______/ \\/___/   \\/_____/   \\/_/") | center | bold,
                separatorEmpty(),
                text("Welcome to ZJUT Computer Room Manage System!") | center | bold | color(Color::Cyan1),
                separatorEmpty(),

                vbox({paragraphAlignCenter("左侧是功能栏, 可以点击进行相关操作") | center | color(Color::Gold1),
                paragraphAlignLeft("  (1) 查询 -> 打开查询界面，按照条件查询学生历史上机信息") | color(Color::GreenLight), // 单词条查询信息
                paragraphAlignLeft("  (2) 统计 -> 打开统计界面，查看历史所有上机记录，并按照条件排序") | color(Color::GreenLight), // 展示所有Record按照排序输出
                paragraphAlignLeft("  (3) 监控 -> 打开监控界面，查看机房机器实时状态") | color(Color::GreenLight), // 改变机房机器个数 显示实时机器状况
                paragraphAlignLeft("  (4) 设置 -> 打开设置界面，修改基本机房信息和管理员相关信息") | color(Color::GreenLight),
                paragraphAlignLeft("  (5) 注销 -> 打开注销界面，注销后需要重新登录") | color(Color::GreenLight),}) | borderDouble | size(WIDTH,EQUAL,80),
                separatorEmpty(),

            }) | center;
        });

        Component model1 = Renderer(comp, [&] { // 主界面渲染器
            if (flag == 1) {
                flag = 0;
                model_base = model_compare;
                // toggled_selected_num = 0;
                // toggled_selected_num2 = 0;
            }

            if (flag == 2) {
                flag = 0;
                model_base = model_machine_check;
                // select_machine = 0;
            }

            if (flag == 3) {
                flag = 0;
                model_base = model_find;
            }

            if (flag == 4) {
                flag = 0;
                model_base = model_modify_record;
            }

            return hbox({
                separatorEmpty(),
                vbox({separatorEmpty(), button_find->Render() | center, separatorEmpty(), button_compare->Render() | center, separatorEmpty(), button_machine_check->Render() | center, separatorEmpty(), button_setting->Render() | center,separatorEmpty(), button_modify_record->Render() | center , separatorEmpty(), button_exit->Render() | center,separatorEmpty()}) | size(WIDTH,EQUAL,15) | center,
                separatorEmpty(),
                separator(),
                hbox({model_base->Render() | center}) | center | flex | size(WIDTH,EQUAL,150),
            });
        });

        auto result = Renderer(model1,[&] { // 外框架渲染器
            return vbox({
                separatorEmpty(),
                window(text("浙江工业大学机房收费管理系统")| bold | color(LinearGradient(45,Color::Pink1,Color::White)) | center,vbox({
                    text("你好, " + _name + " 管理员  ") | bold | color(Color::White) | align_right,
                    separator(),
                    model1->Render(),

                }))| size(WIDTH,EQUAL,150)| center,
            }) | center;
        });

        screen.CaptureMouse();
        screen.Loop(result);

    }

    inline int flag_stu = 0; // 主选择模式（学生版）
    inline int num_machine = 0; // 机器选择

    void menu_stu_check(const std::string &account) { // 学生可视界面

        // 获取当前学生账户信息
        int stu_index = 0;

        for (int i = 0; i < list_stu.size(); ++i) {
            if (list_stu[i].getId() == account) {
                stu_index = i;
                break;
            }
        }

        // 一堆当前学生信息的变量
        std::string name = list_stu[stu_index].getName();
        std::string id = list_stu[stu_index].getId();
        std::string classid = list_stu[stu_index].getClassId();
        std::string gender = list_stu[stu_index].getGender();

        auto screen = ScreenInteractive::TerminalOutput();
        std::string test = "", test2 = ""; // 提示词

        // 一堆窗口界面组件
        Component model_base;
        Component model_upMachine;
        Component model_downMachine;
        Component model_check;
        Component model_exit;
        Component model_timeMonitor = Renderer([&] {return vbox({});});
        Component model_timeMonitor2_ori = Renderer([&] {
            return vbox({
                text("您还暂未上机") | center | bold | color(Color::GreenLight),
            });
        });

        Component model_timeMonitor2 = model_timeMonitor2_ori;

        // 一堆功能按钮，功能应该很明显（懒得写注释
        Component button_start_machine = Button("  上机" , [&] {
            model_base = model_upMachine; // 切换到上机界面
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_end_machine = Button("  下机", [&] {
            model_base = model_downMachine;
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_logout = Button("  注销", [&] {
            model_base = model_exit;
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_look = Button("  查看", [&] {
            model_base = model_check;
        }) | size(WIDTH,EQUAL,10) | center;


        // 创建dropdown组件
        int select_machine = num_machine;
        std::vector<std::string> string_mach;
        for (auto mach : list_mach) {
            string_mach.emplace_back(std::to_string(mach.getId()) + "号机位");
        }

        auto layout = Container::Vertical({
            Container::Horizontal({
                Dropdown(&string_mach, &select_machine),
            }),
        });

        // 创建实时时间管理器
        timeX::timeViewer tv;
        tm timex = tv.getTime(); // 记录开始时间
        tm timey = tv.getTime(); // 记录实时时间
        tm timez = tv.getTime(); // 记录结束时间

        // 开始上机和下机的按钮（起名的时候略有些抽风
        Component button_upupup;
        Component button_downdowndown;

        button_upupup = Button("  开始", [&] { // 开始上机功能按钮
            // button_upupup = Renderer([&] {
            //    return vbox({});
            // });

            if (list_mach[select_machine - 1].getFlag() == 1) {
                test = "机位已被占用";
                return;
            }

            // stu->setStartTime(tv.getTime());
            timex = tv.getTime(); // 获取当前时间以当作开始时间
            list_mach[select_machine].setFlag(1); // 设置机位状态为占用
            num_machine = select_machine; // 记录当前机位编号

            model_timeMonitor = Renderer([&] { // 更换上机界面中timeMonitor组件的界面
                timey = tv.getTime();
                return vbox({
                    separatorEmpty(),
                    text("开始时间：" + std::to_string(timex.tm_year + 1900) + "-" + std::to_string(timex.tm_mon + 1) + "-" + std::to_string(timex.tm_mday) + " " + std::to_string(timex.tm_hour) + ":" + std::to_string(timex.tm_min) + ":" + std::to_string(timex.tm_sec) + "  现在时间：" + std::to_string(timey.tm_year + 1900) + "-" + std::to_string(timey.tm_mon + 1) + "-" + std::to_string(timey.tm_mday) + " " + std::to_string(timey.tm_hour) + ":" + std::to_string(timey.tm_min) + ":" + std::to_string(timey.tm_sec)) | center | color(Color::Red),
                });
            });

            model_timeMonitor2 = Renderer([&] { // 更换下机界面中的timeMonitor2的界面
                timey = tv.getTime();
                long num = timeX::timeCal(timex,timey);

                return vbox({
                    separatorEmpty(),
                    text("您已经上机了 " + std::to_string(num) + " 小时") | center | color(Color::Gold1),
                });
            });


        }) | size(WIDTH,EQUAL,10) | center;

        button_downdowndown = Button("  结束", [&] { // 下机功能按钮

            timez = tv.getTime(); // 获取当前时间以当作下机时间
            list_mach[select_machine].setFlag(0); // 设置当前机位为空闲状态

            // button_downdowndown = Renderer([&] {
            //     return vbox({});
            // });

            // stu->setEndTime(tv.getTime());

            model_timeMonitor2 = Renderer([&] { // 更换下机界面中的timeMonitor2的界面
                return vbox({
                    separatorEmpty(),
                    text("结束时间：" + std::to_string(timez.tm_year + 1900) + "-" + std::to_string(timez.tm_mon + 1) + "-" + std::to_string(timez.tm_mday) + " " + std::to_string(timez.tm_hour) + ":" + std::to_string(timez.tm_min) + ":" + std::to_string(timez.tm_sec)) | center | color(Color::Red),
                });
            });


            model_timeMonitor = Renderer([&] { // 还原上机界面中的timeMonitor界面
                return vbox({});
            });

            list_rec.emplace_back(base::Record(base::Student(id,classid,name,gender,"NULL"),timex,timez,select_machine));

            save(); // 保存到文件
            flag_stu = 1;
            menu_stu_check(account); // 显示刷新 后台立马就能看到
            flag_stu = 0;

        }) | size(WIDTH,EQUAL,10) | center;

        Component button_outoutout1 = Button("  重登", [&] { // 登出按钮

            menu_login();

        }) | size(WIDTH,EQUAL,10) | center;

        Component button_outoutout2 = Button("  退出", [&] { // 退出按钮

            exit(EXIT_SUCCESS);

        }) | size(WIDTH,EQUAL,10) | center;

        // 查看功能界面实现 以下--------------------------------------------------------------------------------------------------------------------------------------------------------
        //（ 学生界面的查看功能只能查看自己的记录 只有管理员能看到所有人的 ）

        std::vector<std::vector<std::string>> data;
        data.emplace_back(std::vector<std::string>{" 学号 "," 姓名 "," 性别 "," 班级 "," 上机时间 "," 下机时间 "," 上机机器 "," 上机费用 "});
        for (auto rec : list_rec) {
            if (rec.getId() == id) {
                std::vector<std::string> temp;

                temp.emplace_back(rec.getId()+"  ");
                temp.emplace_back(rec.getName()+"  ");
                temp.emplace_back(rec.getGender()+"  ");
                temp.emplace_back(rec.getClassId()+"  ");

                std::string time1 = std::to_string(rec.getStartTime().tm_year + 1900) + "-" + std::to_string(rec.getStartTime().tm_mon + 1) + "-" + std::to_string(rec.getStartTime().tm_mday) + " " + std::to_string(rec.getStartTime().tm_hour) + ":" + std::to_string(rec.getStartTime().tm_min) + ":" + std::to_string(rec.getStartTime().tm_sec);
                std::string time2 = std::to_string(rec.getEndTime().tm_year + 1900) + "-" + std::to_string(rec.getEndTime().tm_mon + 1) + "-" + std::to_string(rec.getEndTime().tm_mday) + " " + std::to_string(rec.getEndTime().tm_hour) + ":" + std::to_string(rec.getEndTime().tm_min) + ":" + std::to_string(rec.getEndTime().tm_sec);

                temp.emplace_back(time1+"  ");
                temp.emplace_back(time2+"  ");
                temp.emplace_back(std::to_string(rec.getMachineId()+1) + "号机器  ");
                temp.emplace_back(std::format("{}",timeX::timeCal(rec.getStartTime(),rec.getEndTime())));

                data.emplace_back(temp);
            }
        }

        // 数据呈现进表格
        auto characters = Table({data});
        characters.SelectAll().Border(LIGHT);
        characters.SelectRow(0).Decorate(bold);
        characters.SelectRow(0).SeparatorVertical(LIGHT);
        characters.SelectRow(0).Border(DOUBLE);

        auto content = characters.SelectRows(1, -1);

        content.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
        content.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
        content.DecorateCellsAlternateRow(color(Color::White), 3, 2);

        auto table = characters.Render() | center ; // 最终呈现的表格


        std::string timeNow; // 当前时间提示字符串

        auto comp = Container::Vertical({ // 组件功能工具集
            button_start_machine,
            button_end_machine,
            button_logout,
            button_look,
            button_downdowndown,
            button_upupup,
            button_outoutout1,
            button_outoutout2,
            layout,
        });

        model_upMachine = Renderer(comp, [&] { // 上机界面渲染
            return vbox({
                separatorEmpty(),
                text("欢迎 " + name + " 同学上机！") | center | bold,
                separatorDouble(),
                text("请确认收费学号 " + id + " !") | center | bold ,
                separatorDouble(),
                paragraphAlignCenter("收费标准： 1元/小时（ 不足1小时部分向上取整到1小时 ）") | center | bold | color(Color::Gold1),
                paragraphAlignCenter("* 费用将会从校园卡账户余额中自动扣除!") | center | bold | color(Color::Red),
                paragraphAlignCenter("** 账户注销或者手动下机后，计时将会自动结束!") | center | bold | color(Color::Red),
                separatorEmpty(),
                hbox({
                    text("请选择上机机台：") | center,
                    layout->Render() | center,
                }) | center,
                separatorEmpty(),
                button_upupup->Render() | bold,
                model_timeMonitor ->Render() | bold,
                separatorEmpty(),
                text(test),
            }) | center;
        });

        model_downMachine = Renderer(comp, [&] { // 下机界面渲染
            return vbox({
                separatorEmpty(),
                text("这里是下机界面，欢迎 " + name + " 同学下次上机！") | center | bold | color(Color::Gold1),
                separatorDouble(),
                text("收费稍后将在 " + id + " 校园卡账户扣除") | center | bold ,
                separatorDouble(),
                separatorEmpty(),
                button_downdowndown ->Render() | bold,
                model_timeMonitor2 ->Render() | bold,
            }) | center;
        });

        model_exit = Renderer(comp, [&] { // 退出窗口界面渲染
            return hbox({
                vbox({
                    separatorEmpty(),
                    text("重新登录？") | bold | color(Color::Gold1),
                    separator(),
                    button_outoutout1 ->Render() | bold,

                }) | center | border | size(WIDTH,EQUAL,40) | size(HEIGHT,EQUAL,10),
                vbox({
                    separatorEmpty(),
                    text("退出系统？") | bold | color(Color::Gold1),
                    separator(),
                    button_outoutout2 ->Render() | bold,

                }) | center | border | size(WIDTH,EQUAL,40) | size(HEIGHT,EQUAL,10),
            }) | center;
        });

        model_check = Renderer(comp, [&] { // 查看界面渲染
            return vbox({
                separatorEmpty(),
                text("  这里是查看界面，欢迎 " + name + " 同学查看自己上机记录！") | bold | color(Color::Gold1),
                text("  * 刚结束的上下机具有延迟，可以重新登陆进行查看！") | bold | color(Color::Green),
                separatorEmpty(),
                table | center | size(WIDTH,EQUAL,100),
                separatorEmpty(),
                text(test2),
            });
        });

        model_base = Renderer(comp, [&] { // 欢迎界面渲染
            return vbox({
                text(" ________    _____   __  __   ______   ") | center | bold,
                text("/\\_____  \\  /\\___ \\ /\\ \\/\\ \\ /\\__  _\\  ") | center | bold,
                text("\\/____//'/' \\/__/\\ \\ \\ \\ \\ \\\\/_/\\ \\/  ") | center | bold,
                text("     //'/'     _\\ \\ \\\\ \\ \\ \\ \\  \\ \\ \\  ") | center | bold,
                text("    //'/'___  /\\ \\_\\ \\\\ \\ \\_\\ \\  \\ \\ \\ ") | center | bold,
                text("    /\\_______\\\\ \\____/ \\ \\_____\\  \\ \\_\\") | center | bold,
                text("    \\/_______/ \\/___/   \\/_____/   \\/_/") | center | bold,
                separatorEmpty(),
                text("Welcome to ZJUT Computer Room Manage System!") | center | bold | color(Color::Cyan1),
                separatorEmpty(),

                vbox({paragraphAlignCenter("左侧是功能栏, 可以点击进行相关操作") | center | color(Color::Gold1),
                paragraphAlignLeft("  (1) 上机 -> 打开上机界面，选择机器，点击开始上机") | color(Color::GreenLight),
                paragraphAlignLeft("  (2) 下机 -> 打开下机界面，点击自动结束计时") | color(Color::GreenLight),
                paragraphAlignLeft("  (3) 查看 -> 打开查看界面，可以查看个人信息") | color(Color::GreenLight),
                paragraphAlignLeft("  (4) 注销 -> 打开注销界面，注销后需要重新登录") | color(Color::GreenLight),}) | borderDouble | size(WIDTH,EQUAL,60),
                separatorEmpty(),
                separatorEmpty(),

            }) | center;
        });

        Component model1 = Renderer(comp, [&] { // 主界面渲染

            if (flag_stu == 1) {
                flag_stu = 0;
                model_base = model_downMachine;
            }

            return hbox({
                separatorEmpty(),
                vbox({separatorEmpty(), button_start_machine->Render() | center, separatorEmpty(), button_end_machine->Render() | center, separatorEmpty(), button_look->Render() | center, separatorEmpty(), button_logout->Render() | center,separatorEmpty()}) | size(WIDTH,EQUAL,15) | center,
                separatorEmpty(),
                separator(),
                hbox({model_base->Render()}) | center | size(WIDTH,EQUAL,95),
            });
        });

        auto result = Renderer(model1,[&] { // 外框架渲染
            return vbox({
                separatorEmpty(),
                window(text("浙江工业大学机房收费管理系统")| bold | color(LinearGradient(45,Color::Pink1,Color::White)) | center,vbox({
                    text("你好, " + name + " 同学  ") | bold | color(Color::White) | align_right,
                    separator(),
                    model1->Render(),

                }))| size(WIDTH,EQUAL,120)| center,
            }) | center;
        });

        screen.CaptureMouse();
        screen.Loop(result);
    }


}

int main() {

    sys::preload();
    sys::menu_login();
    sys::save();

    return EXIT_SUCCESS;
}