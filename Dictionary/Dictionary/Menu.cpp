#include "Menu.h"

void startup(BackendGui& gui)
{
    gui.get<Group>("groupHome")->setVisible(true);
    gui.get<Group>("groupWordDefinition")->setVisible(false);
    gui.get<Group>("groupFavourite")->setVisible(false);
}

void onSwitchForm(BackendGui& gui, int id)
{

    gui.get<Group>("groupHome")->setVisible(id == 1);
    gui.get<Group>("groupFavourite")->setVisible(id == 2);
    gui.get<Group>("groupWordDefinition")->setVisible(id == 5);
}

void onSearch(BackendGui& gui , TenarySearchTree tree)
{
    string word = gui.get<EditBox>("SearchBar")->getText().toStdString();
    vector<string> ans =  tree.searchDefinition(tree.root , word , 0);
    if (ans.empty()) return;
    gui.get<Group>("groupWordDefinition")->get<Button>("Word")->setText(word);
    bool ok = favData.isFavourited[word];
    if (ok == false)
    {
        gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->getRenderer()->setTexture(texture.onUnClickedFavouriteButtonTexture);
    }
    else
    {
        gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->getRenderer()->setTexture(texture.onClickedFavouriteButtonTexture);
    }
    for (int i = 0; i < ans.size(); i++)
    {
        string id = "TextArea" + to_string(i + 1 );
        gui.get<Group>("groupWordDefinition")->get<Panel>("PanelWord")->get<TextArea>(id)->setText(ans[i]);
    }
    onSwitchForm(gui, 5);
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
    bool ok = favData.isFavourited[word];
    if (ok == false)
    {
        gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->getRenderer()->setTexture(texture.onClickedFavouriteButtonTexture);
        favData.isFavourited[word] = true;
    }
    else
    {
        gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->getRenderer()->setTexture(texture.onUnClickedFavouriteButtonTexture);
        favData.isFavourited[word] = false;
    }

    int index = 0;
    gui.get<Group>("groupFavourite")->get<Panel>("FavouriteWordListPanel")->removeAllWidgets();
    for (auto t : favData.isFavourited)
    {
        if (!t.second) continue;
        auto dm = tgui::Button::create();
        dm->setHeight(100);
        dm->setWidth(100);
        dm->setPosition(0 + 150, 0 + index * 150);
        dm->setText(t.first);
        index++;
        gui.get<Group>("groupFavourite")->get<Panel>("FavouriteWordListPanel")->add(dm);
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

    auto groupFavourite = tgui::Group::create();
    groupFavourite->loadWidgetsFromFile("Assets/Form/FavouriteForm/FavouriteForm.txt");
    gui.add(groupFavourite, "groupFavourite");    

}

void setAction(BackendGui& gui, TenarySearchTree& tree) 
{
    gui.get<Button>("SearchButton")->onClick(&onSearch, ref(gui), tree);
    gui.get<Button>("SearchButton")->onMouseEnter(&onBlurred, ref(gui));
    gui.get<Button>("SearchButton")->onMouseLeave(&onUnBlurred, ref(gui));
    gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->onClick(&onLike, ref(gui));
    gui.get<Button>("HomeButton")->onClick(&onSwitchForm,ref(gui), 1);
    gui.get<Button>("FavouriteButton")->onClick(&onSwitchForm,ref(gui), 2);
}


bool runMenu(BackendGui& gui , TenarySearchTree& tree) {
    try
    {
        texture.loadTexture();
        loadWidgetsMenu(gui);
        startup(gui);
        setAction(gui, tree);
        int index = 0;
        for (auto t : favData.isFavourited)
        {
            auto dm = tgui::Button::create();
            dm->setHeight(100);
            dm->setWidth(100);
            dm->setPosition(0 + 150, 0 + index * 150);
            dm->setText(t.first);
            index++;
            gui.get<Group>("groupFavourite")->get<Panel>("FavouriteWordListPanel")->add(dm);
        }
        return true;    
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
        return false;
    }
}

