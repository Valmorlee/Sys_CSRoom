//
// Created by valmorx on 25-4-19.
//

#include <people.hpp>
#include<bits/stdc++.h>
#include<ftxui/dom/elements.hpp>
#include<ftxui/screen/screen.hpp>
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/mouse.hpp"

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp"
#include<time.h>
#include "utils.hpp"

using namespace ftxui;

namespace sys {

    void menu_register();
    void menu_login();
    void menu_modify();

    void menu_stu_check(const std::string &account);
    void menu_man_check(const std::string &account);

    std::vector<base::Student> list_stu;
    std::vector<base::Manager> list_man;
    inline std::string detectNum = "ZJUT";

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
    }

    void save() {
        std::fstream out;
        out.open("../out/data_student.txt",std::ios::out);

        for (auto stu : list_stu) {
            out << stu.getId() << " " << stu.getPassword() << " " << stu.getClassId() << " " << stu.getName() << " " << stu.getGender() << std::endl;
        }

        for (auto man : list_man) {
            out << man.getId() << " " << man.getPassword() << " " << man.getClassId() << " " << man.getName() << " " << man.getGender() << std::endl;
        }
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
                            menu_man_check(account);
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

                // base::Student* stu = map_stu;
                // while (stu!=nullptr) {
                //     checkExist = true;
                //     if (stu->getId()==account) {
                //         if (stu->getPassword()==password) {
                //             menu_stu_check(); // 学生界面
                //         }else {
                //             test = "密码错误";
                //             password = "";
                //             break;
                //         }
                //     }
                //     stu = stu->next;
                // }
                //
                // base::Manager* man = map_man;
                // while (man!=nullptr) {
                //     if (man->getId()==account) {
                //         checkExist = true;
                //         if (man->getPassword()==password) {
                //             menu_man_check(); // 管理员界面
                //         }else {
                //             test = "密码错误";
                //             password = "";
                //             break;
                //         }
                //     }
                //     man = man->next;
                // }
                //
                // if (!checkExist) {
                //     test = "账户信息不存在";
                //     account = "";
                //     password = "";
                // }

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
                hbox(button_login->Render(), separatorEmpty(), button_register->Render(), separatorEmpty(), button_modify->Render()) | center
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
                        list_stu.emplace_back(base::Student(account, password, classid, name, gender));

                        save();

                        test = "注册成功，正在返回登录界面";
                        sleep(1);
                        menu_login();
                    }

                    // //学生
                    // base::Student* stu = map_stu;
                    // while (stu!=nullptr) {
                    //     if (stu->getId()==account) {
                    //         test = "该账号已存在";
                    //         account = "";
                    //         password = "";
                    //         checkExist = true;
                    //     }
                    //
                    //     stu = stu->next;
                    // }
                    //
                    // if (!checkExist) {
                    //     stu = new base::Student(account, password, classid, name, gender);
                    //
                    //     //写入文件
                    //     std::ofstream out;
                    //     out.open("../out/data_student.txt",std::ios::app);
                    //     out<<account<<" "<<classid<<" "<<name<<" "<<gender<<" "<<password<<std::endl;
                    //     out.close();
                    //
                    //     test = "注册成功，正在返回登录界面";
                    //     sleep(1);
                    //     menu_login();
                    // }

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

                    // //管理者
                    // base::Manager* man = map_man;
                    // while (man != nullptr) {
                    //     if (man->getId() == account) {
                    //         test = "该账号已存在";
                    //         account = "";
                    //         password = "";
                    //         checkExist = true;
                    //     }
                    //     man = man->next;
                    // }
                    //
                    // if (!checkExist) {
                    //     man = new base::Manager(account, password, classid, name, gender);
                    //
                    //     //写入文件
                    //     std::ofstream out;
                    //     out.open("../out/data_manager.txt.txt",std::ios::app);
                    //     out<<account<<" "<<classid<<" "<<name<<" "<<gender<<" "<<password<<std::endl;
                    //     out.close();
                    //
                    //     test = "注册成功，正在返回登录界面";
                    //     sleep(1);
                    //     menu_login();
                    // }
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
                for (auto stu : list_stu) {
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

                for (auto man : list_man) {
                    if (man.getId() == account) {
                        checkExist = true;
                        if (man.getName() == name) {
                            man.setPassword(password);
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

    void menu_man_check(const std::string &account) {

    }

    void menu_stu_check(const std::string &account) {

        base::Student* stu = nullptr;
        for (auto stu_ : list_stu) {
            if (stu_.getId() == account) {
                stu = &stu_;
                break;
            }
        }

        std::string name = stu->getName();

        auto screen = ScreenInteractive::TerminalOutput();

        Component button_start_machine = Button("  上机" , [&] {


        }) | size(WIDTH,EQUAL,10) | center;

        Component button_end_machine = Button("  下机", [&] {


        }) | size(WIDTH,EQUAL,10) | center;

        Component button_logout = Button("  注销", [&] {



        }) | size(WIDTH,EQUAL,10) | center;

        Component button_look = Button("  查看", [&] {



        }) | size(WIDTH,EQUAL,10) | center;

        auto comp = Container::Vertical({
            button_start_machine,
            button_end_machine,
            button_logout,
            button_look,

        });

        Component model1 = Renderer(comp, [&] {
            return hbox({
                separatorEmpty(),
                vbox({separatorEmpty(), button_start_machine->Render(), separatorEmpty(), button_end_machine->Render(), separatorEmpty(), button_look->Render(), separatorEmpty(), button_logout->Render(),separatorEmpty()}),
                separatorEmpty(),
                separator(),

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