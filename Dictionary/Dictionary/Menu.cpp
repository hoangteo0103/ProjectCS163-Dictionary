#include "Menu.h"

// Menu Form
void startup(BackendGui& gui)
{
    gui.get<Group>("groupHome")->setVisible(true);
    gui.get<Group>("groupWordDefinition")->setVisible(false);
    gui.get<Group>("groupFavourite")->setVisible(false);
    gui.get<Group>("groupChooseLangague")->setVisible(false);
}

void onSwitchForm(BackendGui& gui, int id )
{

    gui.get<Group>("groupHome")->setVisible(id == 1);
    gui.get<Group>("groupFavourite")->setVisible(id == 2);
    gui.get<Group>("groupChooseLangague")->setVisible(id == 3);
    gui.get<Group>("groupWordDefinition")->setVisible(id == 5);
}


void onSwitchToDefinition(BackendGui& gui, string word, TenarySearchTree tree)
{
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
    onSwitchForm(gui, 5);
    vector<string> ans = tree.searchDefinition(tree.root, word, 0);
    for (int i = 0; i < ans.size(); i++)
    {
        string index = "TextArea" + to_string(i + 1);
        gui.get<Group>("groupWordDefinition")->get<TextArea>(index)->setText(ans[i]);
    }
}

void onSearch(BackendGui& gui, TenarySearchTree tree)
{
    string word = gui.get<EditBox>("SearchBar")->getText().toStdString();
    vector<string> ans = tree.searchDefinition(tree.root, word, 0);
    if (ans.empty()) return;
    onSwitchToDefinition(gui, word, tree);
}

void onBlurred(BackendGui& gui)
{
    gui.get<Button>("SearchButton")->setInheritedOpacity(0.6);
}

void onUnBlurred(BackendGui& gui)
{
    gui.get<Button>("SearchButton")->setInheritedOpacity(1);
}

// Word Definition Form

void onLike(BackendGui& gui, TenarySearchTree tree)
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
        dm->onClick(&onSwitchToDefinition, ref(gui), t.first, tree);
        gui.get<Group>("groupFavourite")->get<Panel>("FavouriteWordListPanel")->add(dm);
    }
}



void onSwitchToChooseLang(BackendGui& gui)
{
    onSwitchForm(gui, 3); 
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

    auto groupChooseLangague = tgui::Group::create();
    groupChooseLangague->loadWidgetsFromFile("Assets/Form/ChooseLangagueForm/ChooseLangagueForm.txt");
    gui.add(groupChooseLangague, "groupChooseLangague");

}

void setAction(BackendGui& gui, TenarySearchTree tree) 
{
    gui.get<Button>("SearchButton")->onClick(&onSearch, ref(gui), tree);
    gui.get<Button>("SearchButton")->onMouseEnter(&onBlurred, ref(gui));
    gui.get<Button>("SearchButton")->onMouseLeave(&onUnBlurred, ref(gui));
    gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->onClick(&onLike, ref(gui)  , tree);
    gui.get<Button>("HomeButton")->onClick(&onSwitchForm,ref(gui), 1);
    gui.get<Button>("FavouriteButton")->onClick(&onSwitchForm,ref(gui), 2);
    gui.get<Button>("ChooseLangagueButton")->onClick(&onSwitchToChooseLang, ref(gui));
}


bool runMenu(BackendGui& gui , TenarySearchTree& tree) {
    try
    {
        texture.loadTexture();
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

