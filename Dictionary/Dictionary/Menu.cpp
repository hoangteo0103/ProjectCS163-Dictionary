#include "Menu.h"
#include <iostream>
using namespace std;

void onNavSelected(BackendGui& gui , tgui::String selectedItem)
{
    if (selectedItem == "Home")
    {
        gui.get<Group>("groupWordDefinition")->setVisible(true);
    }
}

void onSearch(BackendGui& gui , TenarySearchTree tree)
{
    string word = gui.get<EditBox>("SearchBar")->getText().toStdString();
    vector<string> ans =  tree.searchDefinition(tree.root , word , 0);
    tgui::String gg = ans[0]; 
    gui.get<Group>("groupWordDefinition")->get<Panel>("Panel1")->get<TextArea>("Definition")->setText(gg);
}

void loadWidgetsMenu(tgui::BackendGui& gui, TenarySearchTree& tree)
{
    gui.removeAllWidgets();
    gui.loadWidgetsFromFile("MenuForm.txt");
    gui.get<ListBox>("Navigation")->onItemSelect(&onNavSelected, ref(gui) ); 
    gui.get<Picture>("SearchButton")->onClick(&onSearch, ref(gui) , tree);
}
bool runMenu(BackendGui& gui , TenarySearchTree& tree) {
    try
    {
        loadWidgetsMenu(gui,tree);
        auto groupWordDefinition = tgui::Group::create();
        groupWordDefinition->loadWidgetsFromFile("WordDefinitionForm.txt"); 
        gui.add(groupWordDefinition , "groupWordDefinition");
        groupWordDefinition->setVisible(true);
        return true;
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
        return false;
    }
}


