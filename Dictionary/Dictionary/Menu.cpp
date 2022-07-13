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
    if (ans.size())
    {
        for (string t : ans)
            cout << t << endl; 
    }
    gui.get<Group>("groupWordDefinition")->get<Button>("Word")->setText(word);
}

void onBlurred(BackendGui& gui)
{
    gui.get<Button>("SearchButton")->setInheritedOpacity(0.6);
}

void onUnBlurred(BackendGui& gui)
{
    gui.get<Button>("SearchButton")->setInheritedOpacity(1);
}

void loadWidgetsMenu(tgui::BackendGui& gui, TenarySearchTree& tree)
{
    gui.removeAllWidgets();
    gui.loadWidgetsFromFile("Assets/Form/MenuForm/MenuForm.txt");
    //gui.get<ListBox>("Navigation")->onItemSelect(&onNavSelected, ref(gui) ); 
    gui.get<Button>("SearchButton")->onClick(&onSearch, ref(gui) , tree);
    gui.get<Button>("SearchButton")->onMouseEnter(&onBlurred, ref(gui));
    gui.get<Button>("SearchButton")->onMouseLeave(&onUnBlurred, ref(gui));

}
bool runMenu(BackendGui& gui , TenarySearchTree& tree) {
    try
    {
        loadWidgetsMenu(gui,tree);
        auto groupWordDefinition = tgui::Group::create();
        groupWordDefinition->loadWidgetsFromFile("Assets/Form/WordDefinitionForm/WordDefinitionForm.txt"); 
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


