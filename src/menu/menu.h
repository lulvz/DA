//
// Created by curlmike on 13-03-2023.
//

#ifndef PROJ_DA_1_MENU_H
#define PROJ_DA_1_MENU_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../manager.h"

/// @brief Classe responável pela interface da aplicação.
class Menu {
public:
    Menu(const char *network_file, const char *stations_file);
    Menu(Manager m);
    void printLineSelectMenu();
    void printBaseMenu(); // select from reading certain stations or all stations
    void printMenu1();
    void printMenu2();
    void printMenu3();
    void printMenu4();

    void MenuLoop();
private:
    Manager m;

    int menuState = -1; // -1 means the lines have not been selected yet
};

#endif //PROJ_DA_1_MENU_H
