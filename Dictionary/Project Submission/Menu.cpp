#include "Menu.h"
#include "WordGame.h"
// Menu Form

TenarySearchTree tree, treeEn, treeVn, treeSlang, treeEmo , treeDef;
TenarySearchTree oriTreeEn, oriTreeVn, oriTreeSlang, oriTreeEmo, oriTreeDef;
int curTreeState;

void quitAndSave(BackendGui& gui)
{
    cout << "quit and save" << endl;
    if (curTreeState == 1) treeEn = tree;
    if (curTreeState == 2) treeVn = tree;
    if (curTreeState == 3) treeSlang = tree;
    if (curTreeState == 4) treeEmo = tree;

    treeDef.saveTreeToTxt("Assets/TreeFormat/DefEnToVn.txt");
    treeEn.saveTreeToTxt("Assets/TreeFormat/EnToVn.txt");
    treeVn.saveTreeToTxt("Assets/TreeFormat/VnToEn.txt");
    treeSlang.saveTreeToTxt("Assets/TreeFormat/Slang.txt");
    treeEmo.saveTreeToTxt("Assets/TreeFormat/Emotional.txt");

    treeEn.saveFavouriteListToFile("Assets/FavouriteWord/treeEn.txt");
    treeVn.saveFavouriteListToFile("Assets/FavouriteWord/treeVn.txt");
    treeSlang.saveFavouriteListToFile("Assets/FavouriteWord/treeSlang.txt");
    treeEmo.saveFavouriteListToFile("Assets/FavouriteWord/treeEmo.txt");

    treeEn.saveHistoryListToFile("Assets/HistoryList/treeEn.txt");
    treeVn.saveHistoryListToFile("Assets/HistoryList/treeVn.txt");
    treeSlang.saveHistoryListToFile("Assets/HistoryList/treeSlang.txt");
    treeEmo.saveHistoryListToFile("Assets/HistoryList/treeEmo.txt");
    exit(0);

}

void startup(BackendGui& gui)
{
    // creater quit button
    gui.get<Button>("quitButton")->onClick(&quitAndSave, ref(gui));


    gui.add(tgui::Group::create(), "suggestWordGroup");
    gui.get<Group>("groupHome")->setVisible(true);
    gui.get<Group>("groupWordDefinition")->setVisible(false);
    gui.get<Group>("groupFavourite")->setVisible(false);
    gui.get<Group>("groupChooseLangague")->setVisible(false);
    gui.get<Group>("groupHistory")->setVisible(false);
    gui.get<Group>("groupAdd")->setVisible(false);
    gui.get<Group>("groupGame")->setVisible(false);
    gui.get<Group>("groupReset")->setVisible(false);
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
    gui.get<Group>("groupGame")->setVisible(id == 7);
    gui.get<Group>("groupIngame")->setVisible(false);
    gui.get<Group>("groupReset")->setVisible(id == 9);
}


void onSwitchToDefinition(BackendGui& gui, string word)
{
    vector<string> ans = tree.searchDefinition(tree.root, word, 0);
    if (ans.empty()) return;
    gui.get<Group>("groupWordDefinition")->get<Button>("Word")->setText(word);

    /*
    cout << "#\n";
    cout << tree.listFavouriteWord.size() << '\n';
    for (auto i : tree.listFavouriteWord) cout << i << ' ';
    cout << endl << endl;
    */

    bool ok = tree.isInFavouriteList(word);
    if (ok == false)
    {
        gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->getRenderer()->setTexture(texture.onUnClickedFavouriteButtonTexture);
    }
    else
    {
        gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->getRenderer()->setTexture(texture.onClickedFavouriteButtonTexture);
    }

    onSwitchForm(gui, 5);

    int ansSize = ans.size();

    for (int i = 0; i < 3; i++)
    {
        string InputIndex = "NewDef" + to_string(i + 1);
        string index = "Def" + to_string(i + 1);
        string EditIndex = "Edit" + to_string(i + 1);
        string SaveIndex = "Save" + to_string(i + 1);

        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setVisible(false);
        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setText("");
        gui.get<Group>("groupWordDefinition")->get<Button>(EditIndex)->setVisible(false);
        gui.get<Group>("groupWordDefinition")->get<Button>(SaveIndex)->setVisible(false);
        gui.get<Group>("groupWordDefinition")->get<EditBox>(InputIndex)->setVisible(false);
    }

    for (int i = 0; i < min(ansSize, 3); i++)
    {
        string index = "Def" + to_string(i + 1);
        string EditIndex = "Edit" + to_string(i + 1);
        string SaveIndex = "Save" + to_string(i + 1);

        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setVisible(true);
        gui.get<Group>("groupWordDefinition")->get<Button>(EditIndex)->setVisible(true);
        gui.get<Group>("groupWordDefinition")->get<Button>(SaveIndex)->setVisible(true);
        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setText(ans[i]);
        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setTextSize(20);
    }
}

void convertStringToLowercase(string& s)
{
    for (int i = 0; i < (int)s.size(); i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = char(int(s[i]) + 32);
    }
}

void onSearchDef(BackendGui& gui , string def)
{
    vector<string> ans = treeDef.searchDefinition(treeDef.root, def, 0);
    if (ans.empty()) return; 
    string word = ans[0];

    gui.get<Group>("suggestWordGroup")->removeAllWidgets();
    tree.addWordToHistoryList(word);

    onSwitchToDefinition(gui, word);
    return;
}

void onSearch(BackendGui& gui)
{
    string word = gui.get<EditBox>("SearchBar")->getText().toStdString();
    vector<string> ans = tree.searchDefinition(tree.root, word, 0);

    if (ans.empty() ) {
        if (curTreeState == 1)
            onSearchDef(gui, word);
        return;
    }
    gui.get<Group>("suggestWordGroup")->removeAllWidgets();
    tree.addWordToHistoryList(word);

    onSwitchToDefinition(gui, word);
    return;
}

void clickSuggestWord(BackendGui& gui, string word)
{
    gui.get<EditBox>("SearchBar")->setText(word);
    onSearch(gui);
    gui.get<Group>("suggestWordGroup")->removeAllWidgets();
}

void suggestWord(BackendGui& gui)
{
    gui.get<Group>("suggestWordGroup")->removeAllWidgets();
    string word = gui.get<EditBox>("SearchBar")->getText().toStdString();
    if ((int)word.size() == 0) return;
    vector<string> res = tree.autoComplete(tree.root, word);

    for (int i = 0; i < res.size(); i++)
    {
        tgui::Button::Ptr box = tgui::Button::create();
        gui.get<Group>("suggestWordGroup")->add(box);
        box->setHeight(60);
        box->setWidth(gui.get<EditBox>("SearchBar")->getFullSize().x);
        box->setText(res[i]);
        box->setTextSize(gui.get<EditBox>("SearchBar")->getTextSize());
        box->setPosition(gui.get<EditBox>("SearchBar")->getPosition().x, gui.get<EditBox>("SearchBar")->getPosition().y + box->getFullSize().y * (i + 1));
        box->setVisible(true);
        box->onClick(&clickSuggestWord, ref(gui), (string)box->getText());
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

// Word Definition Form

void onLike(BackendGui& gui)
{
    string word = gui.get<Group>("groupWordDefinition")->get<Button>("Word")->getText().toStdString();
    bool ok = tree.isInFavouriteList(word);

    if (ok == false)
    {
        gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->getRenderer()->setTexture(texture.onClickedFavouriteButtonTexture);
        tree.addWordToFavouriteList(word);
    }
    else
    {
        gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->getRenderer()->setTexture(texture.onUnClickedFavouriteButtonTexture);
        tree.removeWordFromFavouriteList(word);
    }
}

void onRemove(BackendGui& gui)
{
    string word = gui.get<Group>("groupWordDefinition")->get<Button>("Word")->getText().toStdString();
    tree.remove(tree.root, word, 0);

    if (curTreeState == 1)
    {
        vector<string> def = tree.searchDefinition(tree.root, word, 0);
        if (!def.empty())
        {
            string gg = def[0];
            treeDef.remove(treeDef.root, gg, 0);
        }
    }
    tree.removeWordFromFavouriteList(word);
    tree.removeWordFromHistoryList(word);

    gui.get<Group>("groupWordDefinition")->get<Button>("Word")->setText("");
    for (int i = 0; i < 3; i++)
    {
        string index = "Def" + to_string(i + 1);
        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setText("");
    }
    for (int i = 0; i < 3; i++)
    {
        string InputIndex = "NewDef" + to_string(i + 1);
        string index = "Def" + to_string(i + 1);
        string EditIndex = "Edit" + to_string(i + 1);
        string SaveIndex = "Save" + to_string(i + 1);

        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setVisible(false);
        gui.get<Group>("groupWordDefinition")->get<Button>(index)->setText("");
        gui.get<Group>("groupWordDefinition")->get<Button>(EditIndex)->setVisible(false);
        gui.get<Group>("groupWordDefinition")->get<Button>(SaveIndex)->setVisible(false);
        gui.get<Group>("groupWordDefinition")->get<EditBox>(InputIndex)->setVisible(false);
    }

}
void onEdit(BackendGui& gui, int index)
{
    if (1 <= index && index <= 3)
    {
        string InputIndex = "NewDef" + to_string(index);
        gui.get<Group>("groupWordDefinition")->get<EditBox>(InputIndex)->setVisible(true);
    }
    else
    {
        string InputIndex = "NewDef" + to_string(index - 3);
        string Newdef = gui.get<Group>("groupWordDefinition")->get<EditBox>(InputIndex)->getText().toStdString();
        if (Newdef == "") return;
        string word = gui.get<Group>("groupWordDefinition")->get<Button>("Word")->getText().toStdString();
        vector<string> ans = tree.searchDefinition(tree.root, word, 0);
        if (ans.size() == 0)
        {
            cout << "ERROR!!";
            return;
        }
        ans[index - 4] = Newdef;
        tree.remove(tree.root, word, 0);
        for (int i = 0; i < ans.size(); i++)
            tree.insert(tree.root, word, ans[i]);
        gui.get<Group>("groupWordDefinition")->get<EditBox>(InputIndex)->setText("");
        gui.get<Group>("groupWordDefinition")->get<EditBox>(InputIndex)->setVisible(false);
        onSwitchToDefinition(gui, word);

    }

}

void onChangeDataset(BackendGui& gui, int index)
{
    string button;

    if (curTreeState == 1) treeEn = tree;
    if (curTreeState == 2) treeVn = tree;
    if (curTreeState == 3) treeSlang = tree;
    if (curTreeState == 4) treeEmo = tree;

    curTreeState = index;

    switch (index)
    {
    case 1:
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
    Game.setupButton(gui, tree);
    onSwitchForm(gui, 1);
}

void onAddNewWord(BackendGui& gui)
{
    string word = gui.get<Group>("groupAdd")->get<EditBox>("WordInput")->getText().toStdString();
    string Def1 = gui.get<Group>("groupAdd")->get<EditBox>("Def1Input")->getText().toStdString();
    string Def2 = gui.get<Group>("groupAdd")->get<EditBox>("Def2Input")->getText().toStdString();
    string Def3 = gui.get<Group>("groupAdd")->get<EditBox>("Def3Input")->getText().toStdString();
    vector<string> def;
    if (Def1 != "") def.push_back(Def1);
    if (Def2 != "") def.push_back(Def2);
    if (Def3 != "") def.push_back(Def3);
    tree.insertVec(tree.root, word, def);
    if (curTreeState == 1)
    {
        if (def.size() > 0 && def[0] != "")
            treeDef.insert(treeDef.root, def[0], word);
    }
    gui.get<Group>("groupAdd")->get<EditBox>("WordInput")->setText("");
    gui.get<Group>("groupAdd")->get<EditBox>("Def1Input")->setText("");
    gui.get<Group>("groupAdd")->get<EditBox>("Def2Input")->setText("");
    gui.get<Group>("groupAdd")->get<EditBox>("Def3Input")->setText("");

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

    auto groupReset = tgui::Group::create();
    groupReset->loadWidgetsFromFile("Assets/Form/ResetForm/ResetForm.txt");
    gui.add(groupReset, "groupReset");

}

void resetHistory(BackendGui& gui) {
    gui.get<Group>("groupHistory")->get<Panel>("HistoryListPanel")->removeAllWidgets();
    tree.resetHistoryList();
}

void viewHistoryWord(BackendGui& gui) {
    gui.get<Group>("groupHistory")->get<Button>("ResetButton")->onClick(&resetHistory, ref(gui));
    gui.get<Group>("groupHistory")->get<Panel>("HistoryListPanel")->removeAllWidgets();

    //cout << "Here\n";
    int len = tree.listHistoryWord.size();
    for (int i = 1; i <= len; i++) {
        string word = tree.listHistoryWord[len - i];
        //cout << i << ' ' << word << endl;
        auto cur = tgui::Button::copy(gui.get<Group>("groupHistory")->get<Button>("HistoryListPanelButton"));
        cur->setVisible(true);
        cur->setHeight(75);
        cur->setWidth(840);
        cur->setPosition(0, 0 + (i - 1) * 86);
        cur->setText(word);

        gui.get<Group>("groupHistory")->get<Panel>("HistoryListPanel")->add(cur);

        cur->onClick(&onSwitchToDefinition, ref(gui), word);
    }
}

void resetFavourite(BackendGui& gui) {
    gui.get<Group>("groupFavourite")->get<Panel>("FavouriteWordListPanel")->removeAllWidgets();
    tree.resetFavouriteList();
}

void viewFavouriteList(BackendGui& gui) {
    gui.get<Group>("groupFavourite")->get<Button>("ReloadButton")->onClick(&resetFavourite, ref(gui));
    gui.get<Group>("groupFavourite")->get<Panel>("FavouriteWordListPanel")->removeAllWidgets();

    int len = tree.listFavouriteWord.size();

    for (int i = 0; i < len; i++)
    {
        string word = tree.listFavouriteWord[len - i - 1];
        auto cur = tgui::Button::copy(gui.get<Group>("groupFavourite")->get<Button>("FavouriteListWordButton"));
        cur->setVisible(true);
        cur->setHeight(75);
        cur->setWidth(840);
        cur->setPosition(0, 0 + i * 86);
        cur->setText(word);
        cur->onClick(&onSwitchToDefinition, ref(gui), word);
        gui.get<Group>("groupFavourite")->get<Panel>("FavouriteWordListPanel")->add(cur);
    }
}

void loadRandomWord(BackendGui& gui)
{
    map<string, bool > mp;
    string st = tree.genRandomWord();
    gui.get<Group>("groupHome")->get<Button>("WordDay1")->setText(st);
    mp[st] = true;
    while (mp[st])
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

void onReset(BackendGui& gui)
{
    treeEn = oriTreeEn;
    treeVn = oriTreeVn;
    treeSlang = oriTreeSlang;
    treeEmo = oriTreeEmo;
    treeDef = oriTreeDef;

    if (curTreeState == 1) tree = treeEn;
    if (curTreeState == 2)  tree = treeVn;
    if (curTreeState == 3)  tree = treeSlang;
    if (curTreeState == 4)  tree = treeEmo;

    resetFavourite(gui);
    resetHistory(gui);

    onSwitchForm(ref(gui), 1);
    loadRandomWord(gui);
}

long long random(int L, int R) {
    long long t = RAND_MAX + 1;

    return L + (t * t * t * rand() + t * t * rand() + t * rand() + rand()) % (R - L + 1);
}
void onOutMouse(BackendGui& gui)
{
    gui.get<Group>("suggestWordGroup")->removeAllWidgets();
}

void setAddWordAction(BackendGui& gui)
{
    gui.get<Group>("groupAdd")->get<Button>("Add Word Button")->onClick(&onAddNewWord, ref(gui));
}

void setResetAction(BackendGui& gui)
{
    gui.get<Group>("groupReset")->get<Button>("YesButton")->onClick(&onReset, ref(gui));
    gui.get<Group>("groupReset")->get<Button>("NoButton")->onClick(&onSwitchForm, ref(gui), 1);
    loadRandomWord(gui);
}

void setAction(BackendGui& gui)
{
    // Menu
    gui.get<Button>("SearchButton")->onClick(&onSearch, ref(gui));
    gui.get<Button>("SearchButton")->onMouseEnter(&onBlurred, ref(gui));
    gui.get<Button>("SearchButton")->onMouseLeave(&onUnBlurred, ref(gui));
    gui.get<EditBox>("SearchBar")->onTextChange(&suggestWord, ref(gui));
    //gui.get<EditBox>("SearchBar")->onUnfocus(&onOutMouse , ref(gui));
    // Word Definition
    gui.get<Group>("groupWordDefinition")->get<Button>("LikeButton")->onClick(&onLike, ref(gui));
    gui.get<Group>("groupWordDefinition")->get<Button>("RemoveButton")->onClick(&onRemove, ref(gui));
    gui.get<Group>("groupWordDefinition")->get<Button>("Edit1")->onClick(&onEdit, ref(gui), 1);
    gui.get<Group>("groupWordDefinition")->get<Button>("Edit2")->onClick(&onEdit, ref(gui), 2);
    gui.get<Group>("groupWordDefinition")->get<Button>("Edit3")->onClick(&onEdit, ref(gui), 3);
    gui.get<Group>("groupWordDefinition")->get<Button>("Save1")->onClick(&onEdit, ref(gui), 4);
    gui.get<Group>("groupWordDefinition")->get<Button>("Save2")->onClick(&onEdit, ref(gui), 5);
    gui.get<Group>("groupWordDefinition")->get<Button>("Save3")->onClick(&onEdit, ref(gui), 6);

    // Home
    gui.get<Button>("HomeButton")->onClick(&onSwitchForm, ref(gui), 1);
    loadRandomWord(gui);

    // Favourite
    gui.get<Button>("FavouriteButton")->onClick(&onSwitchForm, ref(gui), 2);
    gui.get<Button>("FavouriteButton")->onClick(&viewFavouriteList, ref(gui));


    //Change Language
    gui.get<Button>("ChooseLangagueButton")->onClick(&onSwitchForm, ref(gui), 3);

    // Choose Langague
    gui.get<Group>("groupChooseLangague")->get<Button>("EnToVn")->onClick(&onChangeDataset, ref(gui), 1);
    gui.get<Group>("groupChooseLangague")->get<Button>("VnToEn")->onClick(&onChangeDataset, ref(gui), 2);
    gui.get<Group>("groupChooseLangague")->get<Button>("Slang")->onClick(&onChangeDataset, ref(gui), 3);
    gui.get<Group>("groupChooseLangague")->get<Button>("Emotional")->onClick(&onChangeDataset, ref(gui), 4);

    // History
    gui.get<Button>("HistoryButton")->onClick(&onSwitchForm, ref(gui), 4);
    gui.get<Button>("HistoryButton")->onClick(&viewHistoryWord, ref(gui));

    // Game
    gui.get<Button>("GameButton")->onClick(&onSwitchForm, ref(gui), 7);

    // Add New Word
    gui.get<Button>("AddWordButton")->onClick(&onSwitchForm, ref(gui), 6);
    setAddWordAction(gui);

    // reset
    gui.get<Button>("ResetButton")->onClick(&onSwitchForm, ref(gui), 9);
    setResetAction(gui);

}

bool runMenu(BackendGui& gui) {
    try
    {
        texture.loadTexture();
        loadWidgetsMenu(gui);
        startup(gui);

        oriTreeDef.loadTreeFromTxt("Assets/OriginalTreeFormat/DefEnToVn.txt");
        treeDef.loadTreeFromTxt("Assets/TreeFormat/DefEnToVn.txt");

        oriTreeEn.loadTreeFromTxt("Assets/OriginalTreeFormat/EnToVn.txt");
        oriTreeVn.loadTreeFromTxt("Assets/OriginalTreeFormat/VnToEn.txt");
        oriTreeSlang.loadTreeFromTxt("Assets/OriginalTreeFormat/Slang.txt");
        oriTreeEmo.loadTreeFromTxt("Assets/OriginalTreeFormat/Emotional.txt");

        treeEn.loadTreeFromTxt("Assets/TreeFormat/EnToVn.txt");
        treeVn.loadTreeFromTxt("Assets/TreeFormat/VnToEn.txt");
        treeSlang.loadTreeFromTxt("Assets/TreeFormat/Slang.txt");
        treeEmo.loadTreeFromTxt("Assets/TreeFormat/Emotional.txt");

        treeEn.loadFavouriteListFromTxt("Assets/FavouriteWord/treeEn.txt");
        treeVn.loadFavouriteListFromTxt("Assets/FavouriteWord/treeVn.txt");
        treeSlang.loadFavouriteListFromTxt("Assets/FavouriteWord/treeSlang.txt");
        treeEmo.loadFavouriteListFromTxt("Assets/FavouriteWord/treeEmo.txt");

        treeEn.loadHistoryListFromTxt("Assets/HistoryList/treeEn.txt");
        treeVn.loadHistoryListFromTxt("Assets/HistoryList/treeVn.txt");
        treeSlang.loadHistoryListFromTxt("Assets/HistoryList/treeSlang.txt");
        treeEmo.loadHistoryListFromTxt("Assets/HistoryList/treeEmo.txt");

        tree = treeEn;
        curTreeState = 1;

        Game.startupGame(gui, tree);
        setAction(gui);
        return true;
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
        return false;
    }
}
