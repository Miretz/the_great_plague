#include "areas.hpp"

#include "utils.hpp"

#include <iostream>

namespace Areas
{
    void navigateToArea(int areaId, std::vector<Hero> &heroes)
    {
        auto area = areas[areaId];

        // print the area description
        clearScreen();

        // execute auxilary area function
        area.auxFunction(heroes);

        // display navigation menu
        std::vector<std::string> menu;
        for (int aId : area.connections)
        {
            menu.push_back(areas[aId].name);
        }
        menu.push_back("Exit");

        // print the area text and navigation menu
        auto prompt = [area]()
        {
            printBorder(130);
            std::cout << "|" << area.name << "\n";
            printBorder(130);
            std::cout << area.body;
            std::cout << "Go to:\n\n";
        };

        int selection = pickOptionFromList(prompt, menu);
        if(selection == menu.size()-1)
        {
            return;
        }

        navigateToArea(area.connections[selection], heroes);
    }

    void f00_introduction(std::vector<Hero> &heroes) {}
    void f01_shore(std::vector<Hero> &heroes) {}
}