//
// Created by curlmike on 13-03-2023.
//

#include "menu.h"

Menu::Menu(const char *network_file, const char *stations_file) : m(network_file, stations_file) {}

void Menu::printLineSelectMenu()
{
    bool running = true;
    while (running)
    {
        std::cout << "|| 1 - Ler todas as linhas ||" << std::endl;
//        std::cout << "|| 2 - Ler linhas específicas ||" << std::endl;

        // get lines to import
        std::cout << "Opção pretendida: ";
        int opt = 0;
        std::cin >> opt;
        std::cout << std::endl;
        switch (opt)
        {
        case 1:
        {
            cout << "Ler todas as linhas" << endl;
            m.initialize_all();
            running = false;
            break;
        }
//        case 2:
//        {
//            std::cout << "Insira as linhas que pretende ler (uma por linha):" << std::endl;
//            std::cout << "(Para terminar, insira uma linha vazia)" << std::endl;
//            // fill vector of strings with line names
//            std::vector<std::string> lines;
//            std::string line;
//            std::cin.ignore();
//            while(std::getline(std::cin, line))
//            {
//                if(line.empty()) {
//                    break;
//                }
//                lines.push_back(line);
//            }
//
//            for(auto &l : lines) {
//                std::cout << l << std::endl;
//            }
//
//            m.initialize_selected(lines);
//            running = false;
//            break;
//        }
        default:
        {
            std::cout << "Número de opção inválido." << std::endl;
            break;
        }
        }
    }
}

/// @brief Imprime para a consola o Menu inicial.
void Menu::printBaseMenu()
{
    std::cout << "|| Menu ||" << std::endl;
    std::cout << "|| 0 - Sair ||" << std::endl;
    std::cout << "|| 1 - Serviços Básicos ||" << std::endl;
    std::cout << "|| 2 - Serviços Económicos ||" << std::endl;
    std::cout << "|| 3 - Situações de falha de linhas ||" << std::endl;
}

void Menu::printMenu1()
{
    bool running = true;
    while (running)
    {
        std::cout << "|| 0 - Voltar ||" << std::endl;
        std::cout << "|| 1 - Número máximos de comboios que podem viajar simultaneamente entre 2 estações ||"
                  << std::endl;
        std::cout << "|| 2 - Estações que usam o maior número de comboios, tirando partido da capacidade "
                     "máxima da rede ||"
                  << std::endl;
        std::cout << "|| 3 - Top k municípios e distritos, de acordo com as suas necessidades de transporte ||"
                  << std::endl;
        std::cout << "|| 4 - Número máximo de comboios que podem chegar simultaneamente a uma estação ||"
                  << std::endl;

        std::cout << "Opção pretendida: ";
        int opt = 0;
        std::string src, dest;
        std::cin >> opt;
        std::cout << std::endl;

        switch (opt)
        {
        case 0:
            running = false;
            break;
        case 1: {
            std::cout << "Estação de partida: ";
            cin.ignore();
            getline(std::cin, src);
            std::cout << "Estação de chegada: ";
            getline(std::cin, dest);

            std::cout << src << " - " << dest << std::endl;
            std::cout << "Número máximo de comboios que podem viajar simultaneamente: "
                      << m.max_simultaneous_trains_between(src, dest) << std::endl;
        }
            break;
        case 2: {
            std::cout << "Estações que usam o maior número de comboios, tirando partido da capacidade "
                         "máxima da rede: "
                      << std::endl;
            for (auto p: m.most_trains_for_network_cap()) {
                std::cout << p.first << " - " << p.second << std::endl;
            }
            break;
        }
        case 3: {
            std::cout << "Criterio de avaliacao capacidade ou flow (1 ou 2): "
                      << std::endl;
            int crit;
            std::cin >> crit;
            std::cout << "Municipio ou distrito (1 ou 2): "
                      << std::endl;
            int type;
            std::cin >> type;

            std::cout << "How many entries (k): "
                      << std::endl;
            int k;
            std::cin >> k;

            switch (crit) {
                case 1: {
                    switch (type) {
                        case 1: {
                            std::cout << "Top " << k << " municipios, de acordo com as suas necessidades de transporte: "
                                      << std::endl;
                            for (auto p: m.top_k_municipalities_capacity(k)) {
                                std::cout << p.first << " - " << p.second << std::endl;
                            }
                            break;
                        }
                        case 2: {
                            std::cout << "Top " << k << " distritos, de acordo com as suas necessidades de transporte: "
                                      << std::endl;
                            for (auto p: m.top_k_districts_capacity(k)) {
                                std::cout << p.first << " - " << p.second << std::endl;
                            }
                            break;
                        }
                        default:
                            std::cout << "Opcao invalida." << std::endl;
                            break;
                    }
                    break;
                }
                case 2: {
                    switch (type) {
                        case 1: {
                            std::cout << "Top " << k << " municipios, de acordo com as suas necessidades de transporte: "
                                      << std::endl;
                            for (auto p: m.top_k_municipalities_flow(k)) {
                                std::cout << p.first << " - " << p.second << std::endl;
                            }
                            break;
                        }
                        case 2: {
                            std::cout << "Top " << k << " distritos, de acordo com as suas necessidades de transporte: "
                                      << std::endl;
                            for (auto p: m.top_k_districts_flow(k)) {
                                std::cout << p.first << " - " << p.second << std::endl;
                            }
                            break;
                        }
                        default:
                            std::cout << "Opcao invalida." << std::endl;
                            break;
                    }
                    break;
                }
            }
            break;
        }
        case 4:
            std::cout << "Estação: ";
            std::cin.ignore();
            std::getline(std::cin, src);
            std::cout << "Número máximo de comboios que podem chegar simultaneamente a " << src << ": " << m.max_trains_arriving_at(src) << std::endl;
            break;
        default:
            std::cout << "Número de opção inválido." << std::endl;
            break;
        }
    }
}

void Menu::printMenu2() {
    bool running = true;
    while(running) {
        std::cout << "|| 0 - Voltar ||" << std::endl;
        std::cout << "|| 1 - Custo minimo para a empresa entre duas estacoes ||"
                  << std::endl;

        std::cout << "Opção pretendida: ";
        int opt = 0;
        std::string src, dest;
        std::cin >> opt;

        switch(opt) {
            case 0: {
                running = false;
                break;
            }
            case 1: {
                std::cout << "Estação de partida: ";
                cin.ignore();
                getline(std::cin, src);
                std::cout << "Estação de chegada: ";
                getline(std::cin, dest);

                std::cout << src << " - " << dest << std::endl;

                // pair of flow/cost
                std::pair<int, int> p = m.min_cost_max_flow(src, dest);
                std::cout << "Para um flow de " << 
                p.first << " o custo minimo é " << p.second << std::endl;
            }
        }
    }
}

void Menu::printMenu3() {
    bool running = true;
    while(running) {
        cout << "|| 0 - Voltar ||" << endl;
        cout << "|| 1 - Construir subgrafo sem certos segmentos ||" << endl;
        cout << "|| 2 - Numero maximo de comboios que podem viajar simultaneamente entre 2 estações no subgrafo ||" << endl;
        cout << "|| 3 - Top k estacoes mais afetadas pela falha de um segmento ||" << endl;
        cout << "Opção pretendida: ";

        int opt = 0;
        std::cin >> opt;

        switch (opt) {
            case 0:
                running = false;
                break;
            case 1: {
                std::cout << "Inserir pares de estacoes (pares separados por ,)" << std::endl;
                std::string src, dest;
                std::string line;
                std::vector<std::pair<std::string, std::string>> pairs;
                cin.ignore();
                while(std::getline(std::cin, line)) {
                    if (line.empty()) break;
                    src = line.substr(0, line.find(','));
                    dest = line.substr(line.find(',') + 1);
                    //std::cout << src << " " << dest << std::endl;
                    pairs.push_back(std::make_pair(src, dest));
                }
                m.initialize_subgraph(pairs);
                cout << "Subgrafo construido!" << endl;
            }
                break;

            case 2: {
                std::string src, dest;
                std::cout << "Estação de partida: ";
                cin.ignore();
                getline(std::cin, src);
                std::cout << "Estação de chegada: ";
                getline(std::cin, dest);

                std::cout << src << " - " << dest << std::endl;
                std::cout << "Número máximo de comboios que podem viajar simultaneamente: "
                        << m.sub_graph_max_simultaneous_trains_between(src, dest) << std::endl;
            }
                break;
            case 3: {
                std::cout << "Quantas estações afetadas pretende ver? " << std::endl;
                int k;
                std::cin >> k;
                std::cout << "Utilizando o subgrafo construido para comparar..." << std::endl;
                std::vector<std::pair<std::string, int>> flow_diff = m.top_k_stations_affected(k);
                for (auto p: flow_diff) {
                    std::cout << p.first << " - " << p.second << std::endl;
                }
            }
                break;
            default:
                std::cout << "Número de opção inválido." << std::endl;
                break;
        }
    }
}

/*! @brief Função responsável por manter o programa a correr até que o utilizador o termine voluntariamente.
 */
void Menu::MenuLoop()
{
    // use variable menu state to switch between menus
    bool running = true;
    while (running)
    {
        switch (menuState)
        {
        case -1 : {
            printLineSelectMenu();
            menuState = 0;
            break;
        }
        case 0: {
            printBaseMenu();
            std::cout << "Opção pretendida: ";
            int opt = 0;
            std::cin >> opt;
            std::cout << std::endl;
            switch (opt) {
                case 0:
                    running = false;
                    break;
                case 1:
                    menuState = 1;
                    break;
                case 2:
                    menuState = 2;
                    break;
                case 3:
                    menuState = 3;
                    break;
                case 4:
                    menuState = 4;
                    break;
                default:
                    std::cout << "Número de opção inválido." << std::endl;
                    break;
            }
            break;
        }
        case 1: {
            printMenu1();
            menuState = 0;
            break;
        }
        case 2: {
            printMenu2();
            menuState = 0;
            break;
        }
        case 3:
            printMenu3();
            menuState = 0;
            break;
        case 4:
//            printMenu4();
            menuState = 0;
            break;
        default:
            std::cout << "Número de opção inválido." << std::endl;
            break;
        }
    }
}