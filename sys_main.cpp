//
// Created by valmorx on 25-4-19.
//

#include <people.hpp>
#include<bits/stdc++.h>
#include<ftxui/dom/elements.hpp>
#include "ftxui/screen/color.hpp"
#include<ftxui/screen/screen.hpp>
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/mouse.hpp"
#include <ftxui/dom/table.hpp>
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp"
#include<time.h>

#include "record.hpp"
#include "utils.hpp"

using namespace ftxui;

namespace sys {

    const float cash_index = 1.0;

    void menu_register();
    void menu_login();
    void menu_modify();

    void menu_stu_check(const std::string &account);
    void menu_man_check(const std::string &account, int num1, int num2, int num3, int flag);

    std::vector<base::Student> list_stu;
    std::vector<base::Manager> list_man;
    std::vector<base::Record> list_rec;
    std::vector<base::Machine> list_mach;
    inline std::string detectNum = "ZJUT";
    inline int machineNum = 40;

    Element colorTile(const int &red, const int &green, const int &blue, const int &width, const int &height) { // 创建色块 用于明确机器状态
        return text("") | size(WIDTH, EQUAL, width ) | size(HEIGHT,EQUAL, height) | bgcolor(Color::RGB(red, green, blue));
    }

    void preload() {
        std::string a,b,c,d,e;
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

        int a1, b1, c1, d1, e1 ,f1 ;
        int a2, b2, c2, d2, e2 ,f2 ,num;

        in.open("../out/data_record.txt", std::ios::in);
        while (in >> a >> b >> c >> d >> a1 >> b1 >> c1 >> d1 >> e1 >> f1 >>a2 >> b2 >> c2 >> d2 >> e2 >> f2 >> num) {
            tm sx = tm(f1,e1,d1,c1,b1,a1,0,0,0,0,0);
            tm ex = tm(f2,e2,d2,c2,b2,a2,0,0,0,0,0);
            list_rec.emplace_back(base::Student(a,b,c,d,"NULL",tm(),tm()),sx,ex,num);
        }
        in.close();

        // machine列表

        for (int i = 0; i < machineNum; i++) {
            list_mach.emplace_back(i+1);
        }

    }

    void save() {
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
            out << rec.getId() << " " << rec.getClassId() << " " << rec.getName() << " " << rec.getGender() << " " << rec.getStartTime().tm_year << " " << rec.getStartTime().tm_mon << " " << rec.getStartTime().tm_mday << " " << rec.getStartTime().tm_hour << " " << rec.getStartTime().tm_min << " " << rec.getStartTime().tm_sec << " " << rec.getEndTime().tm_year << " " << rec.getEndTime().tm_mon << " " << rec.getEndTime().tm_mday << " " << rec.getEndTime().tm_hour << " " << rec.getEndTime().tm_min << " " << rec.getEndTime().tm_sec << " " << rec.getMachineId() << std::endl;
        }
        out.close();
    }

    void menu_login() {

        std::string account;
        std::string password;

        Component input_account = Input(&account, "在此输入学号/工号");
        InputOption pwd_option;
        pwd_option.password = true;
        Component input_password = Input(&password, "在此输入密码", pwd_option);

        std::string test = "";

        Component button_login = Button("  登陆", [&] {
            if (account.empty() || password.empty()) test = "请输入账号/密码";
            else {

                bool checkExist = false;

                for (auto stu : list_stu) {
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

                for (auto man : list_man) {
                    if (man.getId()==account) {
                        checkExist = true;
                        if (man.getPassword()==password) {
                            menu_man_check(account, 0, 0, 0, 0);
                        }else {
                            test = "密码错误";
                            password = "";
                            break;
                        }
                    }
                }

                if (!checkExist) {
                    test = "账户信息不存在";
                    password = "";
                    account = "";
                }

            }
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_register = Button("  注册", [&] {
            menu_register();
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_modify = Button("修改密码", [&] {
            menu_modify();
        }) | size(WIDTH,EQUAL,10) | center;

        auto comp = Container::Vertical({
            input_account,
            input_password,
            button_login,
            button_register,
            button_modify
        });

        auto renderer = Renderer(comp, [&] {

            return vbox({
                text("Hello " + account + " !"),
                separator(),
                hbox(text("账号: "), input_account->Render()),
                hbox(text("密码: "), input_password->Render()),

            }) | border | size(WIDTH,EQUAL,60) | center;
        });

        auto result = Renderer(renderer,[&] {
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

    void menu_register() {
        std::string account;
        std::string password;
        std::string detectNumber;
        std::string classid;
        std::string name;
        std::string gender;
        auto screen	= ScreenInteractive::TerminalOutput();

        Component input_account = Input(&account, "在此输入学号/工号");
        InputOption pwd_option;
        pwd_option.password = true;
        Component input_password = Input(&password, "在此输入想要设置的密码", pwd_option);
        Component input_classid = Input(&classid, "在此输入班级");
        Component input_name = Input(&name, "在此输入姓名");
        Component input_gender = Input(&gender, "在此输入性别");
        Component input_detectNumber = Input(&detectNumber, "在此输入识别码(可以为空)");

        std::string test = "";

        Component button_register = Button("  注册", [&] {

            //注册...
            if(account.empty() || password.empty() || classid.empty() || name.empty() || gender.empty() ) {
                test = "请输入完整信息";
            } else if(account.find("\\") != account.npos || account.find("/") != account.npos
                || account.find(":") != account.npos || account.find("*") != account.npos
                || account.find("?") != account.npos || account.find("\"") != account.npos
                || account.find("<") != account.npos || account.find(">") != account.npos
                || account.find("|") != account.npos) {
                    test    = "输入用户名/密码非法，不能包含 \\/:*?\"<>| 这些字符";
                    account = "";
                    password = "";
            } else {

                bool checkExist = false;

                if (detectNumber!=detectNum) {

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

        Component button_back = Button("  返回", [&] {
            test = "";
            screen.Exit();
        }) | size(WIDTH,EQUAL,10) | center;

        auto comp = Container::Vertical({
            input_account,
            input_password,
            input_name,
            input_gender,
            input_classid,
            input_detectNumber,
            button_register,
            button_back
        });

        auto renderer = Renderer(comp, [&] {

            return vbox({
                text("Hello " + account + " , 欢迎注册!"),
                separator(),
                hbox(text("账号: "), input_account->Render()),
                hbox(text("密码: "), input_password->Render()),
                hbox(text("姓名: "), input_name->Render()),
                hbox(text("性别: "), input_gender->Render()),
                hbox(text("班级: "), input_classid->Render()),
                hbox(text("邀请码: "), input_detectNumber->Render()),

            }) | border | size(WIDTH,EQUAL,60) | center;
        });

        auto result = Renderer(renderer,[&] {
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

    void menu_modify() {
        std::string account;
        std::string password;
        std::string detectNumber;
        std::string classid;
        std::string name;
        std::string gender;
        auto screen	= ScreenInteractive::TerminalOutput();

        Component input_account = Input(&account, "在此输入您的学号/工号");
        InputOption pwd_option;
        pwd_option.password = true;
        Component input_password = Input(&password, "在此输入修改后的密码", pwd_option);
        Component input_name = Input(&name, "在此输入您的真实姓名以验证");

        std::string test = "";

        Component button_register = Button("  更改", [&] {

            if (account.empty() || password.empty() || name.empty()) {
                test = "请输入完整信息";
            }else {
                bool checkExist = false;
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

                if (!checkExist) {
                        test = "不存在该账户";
                        name = "";
                        account = "";
                        password = "";
                }
            }
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_back = Button("  返回", [&] {
            test = "";
            screen.Exit();
        }) | size(WIDTH,EQUAL,10) | center;

        auto comp = Container::Vertical({
            input_account,
            input_password,
            input_name,
            button_register,
            button_back
        });

        auto renderer = Renderer(comp, [&] {

            return vbox({
                text("正在对" + account + " 的账户进行修改!"),
                separator(),
                hbox(text("修改账号: "), input_account->Render()),
                hbox(text("真实姓名: "), input_name->Render()),
                hbox(text("修改后密码: "), input_password->Render()),

            }) | border | size(WIDTH,EQUAL,60) | center;
        });

        auto result = Renderer(renderer,[&] {
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

    void menu_man_check(const std::string &account, int num1, int num2, int num3, int flag) {

        int man_index = 0;

        for (int i = 0; i < list_man.size(); ++i) {
            if (list_man[i].getId() == account) {
                man_index = i;
                break;
            }
        }

        std::string _name = list_man[man_index].getName();
        std::string _id = list_man[man_index].getId();
        std::string _classid = list_man[man_index].getClassId();
        std::string _gender = list_man[man_index].getGender();

        std::vector<std::vector<std::string>> data_base;
        data_base.emplace_back(std::vector<std::string>{" 学号 "," 姓名 "," 性别 "," 班级 "," 上机时间 "," 下机时间 "," 上机机器 "," 上机费用 "});
        std::vector<std::vector<std::string>> data_CMP = data_base;
        std::vector<std::vector<std::string>> data_MACH = data_base;

        int toggled_selected_num = num1;
        int toggled_selected_num2 = num2;

        auto screen = ScreenInteractive::TerminalOutput();
        std::string test = "";
        Component model_base;
        Component model_find;
        Component model_compare;
        Component model_machine_check;
        Component model_cal;
        Component model_exit;
        Component model_setting;

        // button model类
        Component button_find = Button("  查询", [&] {
            model_base = model_find;
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_compare = Button("  统计", [&] {

            model_base = model_compare;

        }) | size(WIDTH,EQUAL,10) | center;

        Component button_machine_check = Button("  监控", [&] {
            model_base = model_machine_check;
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_exit = Button("  注销", [&] {
            model_base = model_exit;
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_setting = Button("  设置", [&] {
            model_base = model_setting;
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_out1 = Button("  重登", [&] {
            menu_login();
        }) | size(WIDTH,EQUAL,10) | center;

        Component button_out2 = Button("  退出", [&] {
            exit(EXIT_SUCCESS);
        }) | size(WIDTH,EQUAL,10) | center;

        for (auto rec : list_rec) {
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

            data_base.emplace_back(temp);
        }

        data_CMP = data_base;

        std::vector<std::string> header = {" 学号 "," 姓名 "," 班级 ", " 上机开始时间 ", " 上机时长 "};
        Component toggle1 = Toggle(&header, &toggled_selected_num);

        std::vector<std::string> header2 = {" 降序 "," 升序 "};
        Component toggle2 = Toggle(&header2, &toggled_selected_num2);

        Component button_confirm = Button("  确认", [&] {

            menu_man_check(account, toggled_selected_num, toggled_selected_num2, 0, 1);

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

        data_CMP.clear();
        data_CMP.emplace_back(std::vector<std::string>{" 学号 "," 姓名 "," 性别 "," 班级 "," 上机时间 "," 下机时间 "," 上机机器 "," 上机费用 "});

        for (auto &rec : data_records) {
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

        auto characters = Table({data_CMP});
        characters.SelectAll().Border(LIGHT);
        characters.SelectRow(0).Decorate(bold);
        characters.SelectRow(0).SeparatorVertical(LIGHT);
        characters.SelectRow(0).Border(DOUBLE);

        auto content = characters.SelectRows(1, -1);

        content.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
        content.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
        content.DecorateCellsAlternateRow(color(Color::White), 3, 2);

        auto table = characters.Render();

        int select_machine = num3;
        Element cond_base = separatorEmpty();
        Element cond_Free = vbox({text("空闲中") | color(Color::Green) | bold | center, colorTile(0,255,0,10,1) | center}) | center;
        Element cond_Busy = vbox({text("占用中") | color(Color::Red) | bold | center, colorTile(255,0,0,10,1) | center}) | center;

        for (auto &mach : list_mach) {
            if (mach.getId() == select_machine + 1) {
                if (mach.getFlag()) {
                    cond_base = cond_Busy;
                }else cond_base = cond_Free;
            }
        }

        std::vector<std::string> string_mach;
        for (auto mach : list_mach) {
            string_mach.emplace_back(std::to_string(mach.getId()) + "号机位");
        }

        auto layout = Container::Vertical({
            Container::Horizontal({
                Dropdown(&string_mach, &select_machine),
            }),
        });

        data_MACH.clear();
        data_MACH.emplace_back(std::vector<std::string>{" 学号 "," 姓名 "," 班级 "," 上机时间 "," 下机时间 "," 上机费用 "});

        for (auto &rec : data_records) {

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

        auto characters2 = Table({data_MACH});
        characters2.SelectAll().Border(LIGHT);
        characters2.SelectRow(0).Decorate(bold);
        characters2.SelectRow(0).SeparatorVertical(LIGHT);
        characters2.SelectRow(0).Border(DOUBLE);

        auto content2 = characters2.SelectRows(1, -1);

        content2.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
        content2.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
        content2.DecorateCellsAlternateRow(color(Color::White), 3, 2);

        auto table2 = characters2.Render();

        auto button_confirm_mach = Button(" ENTER ", [&] {
            menu_man_check(account, 0,0, select_machine, 2);
        }) | center | size(WIDTH,EQUAL,12);

        auto comp = Container::Vertical({
            button_find,
            button_compare,
            button_machine_check,
            button_exit,
            button_out1,
            button_out2,
            toggle1,
            toggle2,
            button_confirm,
            layout,
            button_confirm_mach,
            button_setting,
        });

        model_compare = Renderer(comp, [&] {
            return vbox({
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
                table | size(WIDTH,EQUAL,100) | center,
                separatorEmpty(),
                separatorEmpty(),
                button_confirm->Render() | center | bold,
                separatorEmpty(),

            })| center | size(WIDTH,EQUAL,100);
        }) | size(WIDTH,EQUAL,100) | center;

        model_exit = Renderer(comp, [&] {
            return hbox({
                vbox({
                    separatorEmpty(),
                    text("重新登录？") | bold | color(Color::Gold1),
                    separator(),
                    button_out1 ->Render() | bold,

                }) | center | border | size(WIDTH,EQUAL,40) | size(HEIGHT,EQUAL,10),
                vbox({
                    separatorEmpty(),
                    text("退出系统？") | bold | color(Color::Gold1),
                    separator(),
                    button_out2 ->Render() | bold,

                }) | center | border | size(WIDTH,EQUAL,40) | size(HEIGHT,EQUAL,10),
            }) | center;
        });

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
                table2 | size(WIDTH,EQUAL,120) | center,
                separatorEmpty(),

            }) | center;
        }) | size(WIDTH,EQUAL,80) | center;

        model_find = Renderer(comp, [&] {
            return vbox({});
        }) | border | size(WIDTH,EQUAL,60) | center;

        model_setting = Renderer(comp, [&] {
            return vbox({});
        }) | border | size(WIDTH,EQUAL,60) | center;

        model_base = Renderer(comp, [&] {
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
                paragraphAlignLeft("  (4) 注销 -> 打开注销界面，注销后需要重新登录") | color(Color::GreenLight),}) | borderDouble | size(WIDTH,EQUAL,80),
                separatorEmpty(),
                separatorEmpty(),

            }) | center;
        });

        Component model1 = Renderer(comp, [&] {
            if (flag == 1) {
                flag = 0;
                model_base = model_compare;
            }else if (flag == 2) {
                flag = 0;
                model_base = model_machine_check;
            }
            return hbox({
                separatorEmpty(),
                vbox({separatorEmpty(), button_find->Render() | center, separatorEmpty(), button_compare->Render() | center, separatorEmpty(), button_machine_check->Render() | center, separatorEmpty(), button_setting->Render() | center, separatorEmpty(), button_exit->Render() | center,separatorEmpty()}) | size(WIDTH,EQUAL,15) | center,
                separatorEmpty(),
                separator(),
                hbox({model_base->Render() | center}) | center | flex | size(WIDTH,EQUAL,95),
            });
        });

        auto result = Renderer(model1,[&] {
            return vbox({
                separatorEmpty(),
                window(text("浙江工业大学机房收费管理系统")| bold | color(LinearGradient(45,Color::Pink1,Color::White)) | center,vbox({
                    text("你好, " + _name + " 管理员  ") | bold | color(Color::White) | align_right,
                    separator(),
                    model1->Render(),

                }))| size(WIDTH,EQUAL,120)| center,
            }) | center;
        });

        screen.CaptureMouse();
        screen.Loop(result);

    }

    void menu_stu_check(const std::string &account) {

        int stu_index = 0;

        for (int i = 0; i < list_stu.size(); ++i) {
            if (list_stu[i].getId() == account) {
                stu_index = i;
                break;
            }
        }

        std::string name = list_stu[stu_index].getName();
        std::string id = list_stu[stu_index].getId();
        std::string classid = list_stu[stu_index].getClassId();
        std::string gender = list_stu[stu_index].getGender();

        auto screen = ScreenInteractive::TerminalOutput();
        std::string test = "", test2 = "";
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


        int select_machine = 0;
        std::vector<std::string> string_mach;
        for (auto mach : list_mach) {
            string_mach.emplace_back(std::to_string(mach.getId()) + "号机位");
        }

        auto layout = Container::Vertical({
            Container::Horizontal({
                Dropdown(&string_mach, &select_machine),
            }),
        });


        timeX::timeViewer tv;
        tm timex = tv.getTime(); // Rec startTime
        tm timey = tv.getTime(); // Rec liveTime
        tm timez = tv.getTime(); // Rec endTime

        Component button_upupup;
        Component button_downdowndown;

        button_upupup = Button("  开始", [&] {
            // button_upupup = Renderer([&] {
            //    return vbox({});
            // });

            if (list_mach[select_machine - 1].getFlag() == 1) {
                test = "机位已被占用";
                return;
            }

            // stu->setStartTime(tv.getTime());
            timex = tv.getTime();
            list_mach[select_machine - 1].setFlag(1);

            model_timeMonitor = Renderer([&] {
                timey = tv.getTime();
                return vbox({
                    separatorEmpty(),
                    text("开始时间：" + std::to_string(timex.tm_year + 1900) + "-" + std::to_string(timex.tm_mon + 1) + "-" + std::to_string(timex.tm_mday) + " " + std::to_string(timex.tm_hour) + ":" + std::to_string(timex.tm_min) + ":" + std::to_string(timex.tm_sec) + "  现在时间：" + std::to_string(timey.tm_year + 1900) + "-" + std::to_string(timey.tm_mon + 1) + "-" + std::to_string(timey.tm_mday) + " " + std::to_string(timey.tm_hour) + ":" + std::to_string(timey.tm_min) + ":" + std::to_string(timey.tm_sec)) | center | color(Color::Red),
                });
            });

            model_timeMonitor2 = Renderer([&] {
                timey = tv.getTime();
                long num = timeX::timeCal(timex,timey);

                return vbox({
                    separatorEmpty(),
                    text("您已经上机了 " + std::to_string(num) + " 小时") | center | color(Color::Gold1),
                });
            });


        }) | size(WIDTH,EQUAL,10) | center;

        button_downdowndown = Button("  结束", [&] {

            timez = tv.getTime();
            list_mach[select_machine - 1].setFlag(0);

            // button_downdowndown = Renderer([&] {
            //     return vbox({});
            // });

            // stu->setEndTime(tv.getTime());

            model_timeMonitor2 = Renderer([&] {
                return vbox({
                    separatorEmpty(),
                    text("结束时间：" + std::to_string(timez.tm_year + 1900) + "-" + std::to_string(timez.tm_mon + 1) + "-" + std::to_string(timez.tm_mday) + " " + std::to_string(timez.tm_hour) + ":" + std::to_string(timez.tm_min) + ":" + std::to_string(timez.tm_sec)) | center | color(Color::Red),
                });
            });


            model_timeMonitor = Renderer([&] {
                return vbox({});
            });

            list_rec.emplace_back(base::Record(base::Student(id,classid,name,gender,"NULL"),timex,timez,select_machine));

            save();


        }) | size(WIDTH,EQUAL,10) | center;

        Component button_outoutout1 = Button("  重登", [&] {

            menu_login();

        }) | size(WIDTH,EQUAL,10) | center;

        Component button_outoutout2 = Button("  退出", [&] {

            exit(EXIT_SUCCESS);

        }) | size(WIDTH,EQUAL,10) | center;

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

        auto characters = Table({data});
        characters.SelectAll().Border(LIGHT);
        characters.SelectRow(0).Decorate(bold);
        characters.SelectRow(0).SeparatorVertical(LIGHT);
        characters.SelectRow(0).Border(DOUBLE);

        auto content = characters.SelectRows(1, -1);

        content.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
        content.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
        content.DecorateCellsAlternateRow(color(Color::White), 3, 2);

        auto table = characters.Render() | center ;


        std::string timeNow;

        auto comp = Container::Vertical({
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

        model_upMachine = Renderer(comp, [&] {
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

        model_downMachine = Renderer(comp, [&] {
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

        model_exit = Renderer(comp, [&] {
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

        model_check = Renderer(comp, [&] {
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

        model_base = Renderer(comp, [&] {
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

        Component model1 = Renderer(comp, [&] {
            return hbox({
                separatorEmpty(),
                vbox({separatorEmpty(), button_start_machine->Render() | center, separatorEmpty(), button_end_machine->Render() | center, separatorEmpty(), button_look->Render() | center, separatorEmpty(), button_logout->Render() | center,separatorEmpty()}) | size(WIDTH,EQUAL,15) | center,
                separatorEmpty(),
                separator(),
                hbox({model_base->Render()}) | center | size(WIDTH,EQUAL,95),
            });
        });

        auto result = Renderer(model1,[&] {
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

    return EXIT_SUCCESS;
}