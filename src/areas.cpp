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
        if (area.auxFunction != nullptr)
        {
            area.auxFunction(heroes);
        }

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

        auto selection = pickOptionFromList(prompt, menu);
        if (selection == area.connections.size())
        {
            return;
        }

        navigateToArea(area.connections[selection], heroes);
    }

}