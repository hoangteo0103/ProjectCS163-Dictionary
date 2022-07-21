#include "Menu.h"

// Menu Form


TenarySearchTree tree , treeEn, treeVn, treeSlang , treeEmo;

void startup(BackendGui& gui)
{
    gui.get<Group>("groupHome")->setVisible(true);
    gui.get<Group>("groupWordDefinition")->setVisible(false);
    gui.get<Group>("groupFavourite")->setVisible(false);
    gui.get<Group>("groupChooseLangague")->setVisible(false);
    gui.get<Group>("groupHistory")->setVisible(false);
    gui.get<Group>("groupAdd")->setVisible(false);

}
void loadRandomWord(BackendGui& gui);

void onSwitchForm(BackendGui& gui, int id)
{

    if (id == 1) loadRandomWord(gui);
    gui.get<Group>("groupHome")->setVisible(id == 1);
    gui.get<Group>("groupFavourite")->setVisible(id == 2);
    gui.get<Group>("groupChooseLangague")->setVisible(id == 3);
    gui.get<Group>("groupHistory")->setVisible(id == 4);
    gui.get<Group>("groupWordDefinition")->setVisible(id == 5);
    gui.get<Group>("groupAdd")->setVisible(id == 6);
}


void onSwitchToDefinition(BackendGui& gui, string word)
{
    vector<string> ans = tree.searchDefinition(tree.root, word, 0);
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
    onSwitchForm(gui, 5);

    for (int i = 0; i < 3; i++)
    {
        string index = "TextArea" + to_string(i + 1);
        gui.get<Group>("groupWordDefinition")->get<TextArea>(index)->setText("");
    }
    for (int i = 0; i < ans.size(); i++)
    {
        string index = "TextArea" + to_string(i + 1);
        gui.get<Group>("groupWordDefinition")->get<TextArea>(index)->setText(ans[i]);
    }
}

void onSearch(BackendGui& gui)
{
    string word = gui.get<EditBox>("SearchBar")->getText().toStdString();
    vector<string> ans = tree.searchDefinition(tree.root, word, 0);
    cout << "DM" << tree.genRandomWord();
    if (ans.empty()) return;
    onSwitchToDefinition(gui, word);
    int index = favData.historyList.size();
    favData.historyList[word] = true;
    auto dm = tgui::Button::create();
    dm->setHeight(100);
    dm->setWidth(100);
    dm->setPosition(0 + 150, 0 + index * 150);
    dm->setText(word);
    index++;
    dm->onClick(&onSwitchToDefinition, ref(gui), word);
    gui.get<Group>("groupHistory")->get<Panel>("HistoryListPanel")->add(dm);


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
        dm->onClick(&onSwitchToDefinition, ref(gui), t.first);
        gui.get<Group>("groupFavourite")->get<Panel>("FavouriteWordListPanel")->add(dm);
    }
}


void onChangeDataset(BackendGui& gui,  int index)
{
    string button;
    switch (index)
    {
    case 1 :
        tree = treeEn;
        button = "EnToVn";
        break;
    case 2:
        tree = treeVn;
        button = "VnToEn";
        break;
    case 3:
        tree = treeSlang;
        button = "Slang";
        break;
    case 4:
        tree = treeEmo;
        button = "Emotional";
        break;
    default:
        break;
    }
    gui.get<Button>("ChooseLangagueButton")->setText(button);
    onSwitchForm(gui, 1);
}


void loadWordOfTheDay(BackendGui& gui)
{

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

    auto groupHistory = tgui::Group::create();
    groupHistory->loadWidgetsFromFile("Assets/Form/HistoryForm/HistoryForm.txt");
    gui.add(groupHistory, "groupHistory");


    auto groupAdd = tgui::Group::create();
    groupAdd->loadWidgetsFromFile("Assets/Form/AddForm/AddForm.txt");
    gui.add(groupAdd, "groupAdd");
}

void loadRandomWord(BackendGui& gui)
{
    map<string, bool > mp;
    string st = tree.genRandomWord();
    gui.get<Group>("groupHome")->get<Button>("WordDay1")->setText(st);
    mp[st] = true;
    while(mp[st])
        st = tree.genRandomWord();
    gui.get<Group>("groupHome")->get<Button>("WordDay2")->setText(st);
    mp[st] = true;
    while (mp[st])
        st = tree.genRandomWord();
    gui.get<Group>("groupHome")->get<Button>("WordDay3")->setText(st);
    mp[st] = true;
    while (mp[st])
        st = tree.genRandomWord();
    gui.get<Group>("groupHome")->get<Button>("WordDay4")->setText(st);

    gui.get<Group>("groupHome")->get<Button>("WordDay1")->onClick(&onSwitchToDefinition, ref(gui), gui.get<Group>("groupHome")->get<Button>("WordDay1")->getText().toStdString());
    gui.get<Group>("groupHome")->get<Button>("WordDay2")->onClick(&onSwitchToDefinition, ref(gui), gui.get<Group>("groupHome")->get<Button>("WordDay2")->getText().toStdString());
    gui.get<Group>("groupHome")->get<Button>("WordDay3")->onClick(&onSwitchToDefinition, ref(gui), gui.get<Group>("groupHome")->get<Button>("WordDay3")->getText().toStdString());
    gui.get<Group>("groupHome")->get<Button>("WordDay4")->onClick(&onSwitchToDefinition, ref(gui), gui.get<Group>("groupHome")->get<Button>("WordDay4")->getText().toStdString());
}

void setAction(BackendGui& gui)
{

    // Menu
    gui.get<Button>("SearchButton")->onClick(&onSearch, ref(gui));
    gui.get<Button>("SearchButton")->onMouseEnter(&onBlurred, ref(gui));
    gui.get<Button>("SearchButton")->onMouseLeave(&onUnBlurred, ref(gui));
    
    // Word Definition
    gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->onClick(&onLike, ref(gui));

    // Home
    gui.get<Button>("HomeButton")->onClick(&onSwitchForm, ref(gui), 1);
    loadRandomWord(gui);
    // Favourite
    gui.get<Button>("FavouriteButton")->onClick(&onSwitchForm, ref(gui), 2);
    gui.get<Button>("ChooseLangagueButton")->onClick(&onSwitchForm, ref(gui),3);

    // Choose Langague

    gui.get<Group>("groupChooseLangague")->get<Button>("EnToVn")->onClick(&onChangeDataset, ref(gui), 1);
    gui.get<Group>("groupChooseLangague")->get<Button>("VnToEn")->onClick(&onChangeDataset, ref(gui), 2);
    gui.get<Group>("groupChooseLangague")->get<Button>("Slang")->onClick(&onChangeDataset, ref(gui), 3);
    gui.get<Group>("groupChooseLangague")->get<Button>("Emotional")->onClick(&onChangeDataset, ref(gui), 4);

    // History
    gui.get<Button>("HistoryButton")->onClick(&onSwitchForm, ref(gui), 4);

    // Add New Word

    gui.get<Button>("AddWordButton")->onClick(&onSwitchForm, ref(gui), 6);

}


bool runMenu(BackendGui& gui) {
    try
    {
        texture.loadTexture();
        loadWidgetsMenu(gui);
        startup(gui);
        treeEn.selectData("Assets/Data/EnToVn.txt");
        treeVn.selectData("Assets/Data/VnToEn.txt");
        treeSlang.selectData("Assets/Data/Slang.txt");
        treeEmo.selectData("Assets/Data/Emotional.txt");
        tree = treeEn;
        setAction(gui);
        return true;
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
        return false;
    }
}

