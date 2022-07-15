#include "Menu.h"
#include <iostream>
#include "TexturePack.h"
#include "PreloadedData.h"
using namespace std;

void startup(BackendGui& gui)
{
    gui.get<Group>("groupHome")->setVisible(true);
    gui.get<Group>("groupWordDefinition")->setVisible(false);
}

void onSearch(BackendGui& gui , TenarySearchTree tree)
{
    string word = gui.get<EditBox>("SearchBar")->getText().toStdString();
    vector<string> ans =  tree.searchDefinition(tree.root , word , 0);
    if (ans.empty()) return;
    gui.get<Group>("groupWordDefinition")->get<Button>("Word")->setText(word);
    gui.get<Group>("groupWordDefinition")->get<Button>("Word")->setText(word)   ;
    for (int i = 0; i < ans.size(); i++)
    {
        string id = "TextArea" + to_string(i + 1 );
        gui.get<Group>("groupWordDefinition")->get<Panel>("PanelWord")->get<TextArea>(id)->setText(ans[i]);
    }
}

void onBlurred(BackendGui& gui)
{
    gui.get<Button>("SearchButton")->setInheritedOpacity(0.6);
}

void onUnBlurred(BackendGui& gui)
{
    gui.get<Button>("SearchButton")->setInheritedOpacity(1);
}

void onLike(BackendGui& gui)
{
    string word = gui.get<Group>("groupWordDefinition")->get<Button>("Word")->getText().toStdString();
    bool ok = isFavourited[word];
    if (ok == false)
    {
        gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->getRenderer()->setTexture(onClickedFavouriteButtonTexture);
        isFavourited[word] = true;
    }
    else
    {
        gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->getRenderer()->setTexture(onUnClickedFavouriteButtonTexture);
        isFavourited[word] = false;
    }
}

void loadWidgetsMenu(tgui::BackendGui& gui)
{
    gui.loadWidgetsFromFile("Assets/Form/MenuForm/MenuForm.txt");

    auto groupWordDefinition = tgui::Group::create();
    groupWordDefinition->loadWidgetsFromFile("Assets/Form/WordDefinitionForm/WordDefinitionForm.txt");
    gui.add(groupWordDefinition, "groupWordDefinition");


    auto groupHome = tgui::Group::create();
    groupHome->loadWidgetsFromFile("Assets/Form/HomeForm/HomeForm.txt");
    gui.add(groupHome, "groupHome");
}

void setAction(BackendGui& gui, TenarySearchTree& tree)
{
    gui.get<Button>("SearchButton")->onClick(&onSearch, ref(gui), tree);
    gui.get<Button>("SearchButton")->onMouseEnter(&onBlurred, ref(gui));
    gui.get<Button>("SearchButton")->onMouseLeave(&onUnBlurred, ref(gui));
    gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->onClick(&onLike, ref(gui));
}
bool runMenu(BackendGui& gui , TenarySearchTree& tree) {
    try
    {
        loadTexture();
        loadWidgetsMenu(gui);
        startup(gui);
        setAction(gui, tree);
        return true;    
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
        return false;
    }
}


