

#include <iostream>

#include "Algoritmus.h"
#include <Windows.h>
#include "MemLeak.h"
#include <libds/heap_monitor.h>
#include <iterator>

int main()
{
    {
        initHeapMonitor();
        std::cout << "Hello World!\n";

        Algoritmus alg;
        std::string subory[5] = {"2020.csv", "2021.csv", "2022.csv", "2023.csv", "2024.csv"};

        SetConsoleOutputCP(1250);
        SetConsoleCP(1250);

        alg.nacitaj(subory);

        int rokIndex = 3;
        int minPocet = 3000;
        //std::vector<UzemnaJednotka> obceMin23 = alg.getObceZaRok(rokIndex);


        alg.nacitajHierarchiu("obce.csv", "uzemie.csv");
        alg.spustiAgregaciu();
        //alg.vypisObceHierarchia();


        alg.naplnTabulky();
        alg.konzola();

        //alg.najdiVTabulkach("Krumbach", 10000);                         // Mühldorf Krumbach Warth, Bad Schönau

    }


    //std::vector<Obec> prefiltrovaneObce = alg.filter(obceMin23, [&alg](const Obec obec) { return alg.hasMinResidents(obec, 3000, 0); });
    //std::vector<UzemnaJednotka> prefiltrovaneObce = alg.filter(obceMin23, [&alg](const UzemnaJednotka obec) {std::string hladanyText = "öf"; return alg.containsStr(obec, hladanyText); });
    //std::vector<Obec> prefiltrovaneObce = alg.filter(obceMin23, [&alg](const Obec obec) { return alg.containsStr(obec, "Deutsch"); });
    /*
    
    std::vector<UzemnaJednotka> prefiltrovaneObce = alg.filter(obceMin23.begin(), obceMin23.end(), [&alg](const UzemnaJednotka obec) { return alg.hasMinResidents(obec, 3000, 0); });

    std::cout << "Obce splnajuce podmienky:" << std::endl;
    for (auto& obec : prefiltrovaneObce)
    {
        std::cout << obec.getNazov() << " <" << obec.getId() << "> " << obec.getPocetMuzov(0) << " " << obec.getPocetZien(0) << std::endl;      // tu sme na 0 indexe lebo mame iba tento rok
    }

    alg.ulozSubor(prefiltrovaneObce, 0);
    */


    /*std::vector<Obec> obce = alg.nacitaj("2020.csv");

    if (obce.empty())
    {
        std::cout << "CHYBA - Zoznam je prazdny!" << std::endl;
        return 1;                   // 1 ci 0 je chyba?
    }

    std::vector<Obec> prefiltrovaneObce = alg.filter(obce, [&alg](const Obec obec) {return alg.containsStr(obec, "Deutsch"); });

    alg.ulozSubor(prefiltrovaneObce);

    std::cout << "Obce splnajuce podmienky: " << std::endl;

    for (Obec obec : prefiltrovaneObce)
    {
        std::cout << obec.getNazov() << " <" << obec.getId() << "> " << obec.getPocetMuzov() << " " << obec.getPocetZien() << std::endl;
    }
    */

    //nacitaj("2020.csv");
    /*std::string nazov = "PremennaMeno";
    if (nazov.find("X") != std::string::npos)
    {
        std::cout << "Najdene" << std::endl;
    }
    else
    {
        std::cout << "Nenajdene" << std::endl;
    }*/
    //std::string* stringy = new std::string[10];                   //mem leak test
    return 0;
}

