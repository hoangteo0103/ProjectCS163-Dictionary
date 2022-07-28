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
    gui.get<Group>("groupGame")->setVisible(false);
}

void loadRandomWord(BackendGui& gui);

void gamePlay(BackendGui& gui);

void onSwitchForm(BackendGui& gui, int id)
{
    if (id == 1) loadRandomWord(gui);
    if (id == 7) gamePlay(gui);
    gui.get<Group>("groupHome")->setVisible(id == 1);
    gui.get<Group>("groupFavourite")->setVisible(id == 2);
    gui.get<Group>("groupChooseLangague")->setVisible(id == 3);
    gui.get<Group>("groupHistory")->setVisible(id == 4);
    gui.get<Group>("groupWordDefinition")->setVisible(id == 5);
    gui.get<Group>("groupAdd")->setVisible(id == 6);
    gui.get<Group>("groupGame")->setVisible(id == 7);
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
        string index = "Def" + to_string(i + 1);
        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setVisible(false);
        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setText("");
    }
    for (int i = 0; i < ans.size(); i++)
    {
        string index = "Def" + to_string(i + 1);
        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setVisible(true);
        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setText(ans[i]);
    }
}

void convertStringToLowercase(string& s)
{
    for (int i = 0; i < (int)s.size(); i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = char(int(s[i]) + 32);
    }
}

void onSearch(BackendGui& gui)
{
    string word = gui.get<EditBox>("SearchBar")->getText().toStdString();
    convertStringToLowercase(word);
    vector<string> ans = tree.searchDefinition(tree.root, word, 0);

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

void onRemove(BackendGui& gui)
{
    string word = gui.get<Group>("groupWordDefinition")->get<Button>("Word")->getText().toStdString();
    tree.remove(tree.root, word, 0);
    gui.get<Group>("groupWordDefinition")->get<Button>("Word")->setText("");
    for (int i = 0; i < 3; i++)
    {
        string index = "Def" + to_string(i + 1);
        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setText("");
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

    auto groupGame = tgui::Group::create();
    groupGame->loadWidgetsFromFile("Assets/Form/GameForm/GameForm.txt");
    gui.add(groupGame, "groupGame");
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

long long random(int L, int R) {
    long long t = RAND_MAX + 1;

    return L + (t * t * t * rand() + t * t * rand() + t * rand() + rand()) % (R - L + 1);
}

void isGameChoose(BackendGui& gui, int pos, int answer) {
    string correctCellStr = "CorrectCell" + to_string(answer);
    gui.get<Group>("groupGame")->get<Button>(correctCellStr)->setVisible(true);

    if (pos == answer) {
        cout << "Correct\n";
    }
    else {
        cout << "Wrong\n";
        string wrongCellStr = "WrongCell" + to_string(pos);
        gui.get<Group>("groupGame")->get<Button>(wrongCellStr)->setVisible(true);
    }
}

void gamePlay(BackendGui& gui)
{
    char buffer[205];

    string res = treeEn.randomWord(treeEn.root, buffer);

    //cout << res << endl << endl;

    vector <string> lstDef;
    lstDef = treeEn.searchDefinition(treeEn.root, res);

    if (lstDef.size() == 0) {
        cout << "No find word\n";
        return;
    }

    vector <string> listGameWord;
    listGameWord.push_back(lstDef[0]);

    for (int i = 1; i <= 3; i++) {
        string S;

        while (1) {
            char tmpBuffer[205];
            S = treeVn.randomWord(treeVn.root, tmpBuffer);

            bool curState = true;

            for (auto it : listGameWord)
                if (it == S) curState = false;

            if (curState) break;
        }

        listGameWord.push_back(S);
    }

    int state[5], answer;

    for (int i = 0; i < 4; i++) {
        int t;

        while (1) {
            t = random(0, 3);

            bool curState = true;
            for (int j = 0; j < i; j++) {
                if (state[j] == t) curState = false;
            }

            if (curState) break;
        }

        state[i] = t;
        if (t == 0) answer = i + 1;
    }

    for (int i = 0; i < 4; i++) {
        string curCell = "GameCell" + to_string(i + 1);
        string correctCellStr = "CorrectCell" + to_string(i + 1);
        string wrongCellStr = "WrongCell" + to_string(i + 1);
        gui.get<Group>("groupGame")->get<Button>(curCell)->setText(listGameWord[state[i]]);
        gui.get<Group>("groupGame")->get<Button>(correctCellStr)->setText(listGameWord[state[i]]);
        gui.get<Group>("groupGame")->get<Button>(wrongCellStr)->setText(listGameWord[state[i]]);
    }

    gui.get<Group>("groupGame")->get<Button>("KeyWord")->setText(res);

    for (int i = 0; i < 4; i++) {
        string curCell = "GameCell" + to_string(i + 1);
        gui.get<Group>("groupGame")->get<Button>(curCell)->onClick(&isGameChoose, ref(gui), i + 1, answer);
    }
}

void setAction(BackendGui& gui)
{
    // Menu
    gui.get<Button>("SearchButton")->onClick(&onSearch, ref(gui));
    gui.get<Button>("SearchButton")->onMouseEnter(&onBlurred, ref(gui));
    gui.get<Button>("SearchButton")->onMouseLeave(&onUnBlurred, ref(gui));
    
    // Word Definition
    gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->onClick(&onLike, ref(gui));
    gui.get<Group>("groupWordDefinition")->get<Button>("RemoveButton")->onClick(&onRemove, ref(gui));

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

    // Game
    gui.get<Button>("GameButton")->onClick(&onSwitchForm, ref(gui), 7);

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

