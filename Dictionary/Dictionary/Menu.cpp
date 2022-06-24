#include "Menu.h"
#include <iostream>
using namespace std;
void loadWidgetsMenu(tgui::BackendGui& gui)
{
    gui.removeAllWidgets();
    gui.loadWidgetsFromFile("MenuForm.txt");
}
bool runMenu(BackendGui& gui) {
    try
    {
        loadWidgetsMenu(gui);
        return true;
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
        return false;
    }
}


