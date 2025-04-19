//
// Created by valmorx on 25-4-19.
//

#include <people.hpp>
#include<bits/stdc++.h>
#include<ftxui/dom/elements.hpp>
#include<ftxui/screen/screen.hpp>
#include<time.h>
#include "utils.hpp"

int main() {
    std::cout<<"Hello World, FTXUI!"<<std::endl;

    using namespace ftxui;

    Element document =
        hbox({
            text("left") | border,
            text("middle") | border | flex,
            text("right") | border,
        });

    auto screen = Screen::Create(
        Dimension::Full(),
        Dimension::Fit(document)
    );

    Render(screen, document);
    screen.Print();
    std::cout<<std::endl;

    timeX::timeViewer x;
    tm t1 = x.getTime();
    sleep(1);
    tm t2 = x.getTime();
    std::cout<<timeX::timeCal(t1,t2)<<std::endl;

    base::Student s;
    base::People p;
    base::Teacher t;
    base::Manager m;
    t.

    return EXIT_SUCCESS;
}