#include "Mode.h"
#include <iostream>
using namespace std;

void updateTextSize(tgui::BackendGui& gui)
{
    // Update the text size of all widgets in the gui, based on the current window height
    const float windowHeight = gui.getView().getRect().height;
    gui.setTextSize(static_cast<unsigned int>(0.02f * windowHeight)); // 2% of height
}

void UserClicked(tgui::BackendGui& gui)
{
    TenarySearchTree tree ;
    tree.selectData("Assets/Data/OCHO.txt");
    runMenu(gui , tree);
}

void loadWidgetsMode(tgui::BackendGui& gui)
{
    gui.loadWidgetsFromFile("ChooseModeForm.txt");
    updateTextSize(gui);
    gui.onViewChange([&gui] { updateTextSize(gui); });
    gui.get<Picture>("Picture1")->onClick(&UserClicked, ref(gui));
}

bool runChooseMode(BackendGui& gui)
{
    try
    {

        loadWidgetsMode(gui);
        return true;
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
        return false;
    }
}
