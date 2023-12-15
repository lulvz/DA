#include <iostream>
#include "manager.h"
#include "menu/menu.h"

#define CSV_LOCATION "../dataset/"
#define CSV_NETWORK "network.csv"
#define CSV_STATIONS "stations.csv"

int main() {
    // Manager manager("../dataset/network.csv", "../dataset/stations.csv");

    // manager.initialize_all();
    // manager.test();
    
    // FIRST NETWORK, THEN STATIONS CSV LOCATION
    Menu menu("../dataset/network.csv", "../dataset/stations.csv");
    // Menu menu("../dataset_presentation/network.csv", "../dataset_presentation/stations.csv");

    menu.MenuLoop();

    return 0;
}
