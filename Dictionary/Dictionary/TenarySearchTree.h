#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include<random>
using namespace std;

using namespace std;
struct Node {
    // data
    char data;
    int EOS = 1;
    Node* left, * middle, * right;
    vector <string> listDef;

    Node() {
        this->EOS = 0;
        this->left = this->middle = this->right = NULL;
        listDef.clear();
    }

    // Constructor new node
    Node(char data)
    {
        this->data = data;
        this->EOS = 0;
        this->left = this->middle = this->right = NULL;
        listDef.clear();
    }

    // destruction
    void destroy() {
        this->EOS = 0;
        delete this->left;
        delete this->right;
        delete this->middle;
        this->left = this->middle = this->right = NULL;
        listDef.clear();
    }
};
struct TenarySearchTree {
    // number suggestion word
    static const int numFillWord = 6;
    // number word in searching history
    static const int numHistoryWord = 10;
    static const int numFavouriteWord = 100;

    Node* root = nullptr;
    vector <string> listHistoryWord, listFavouriteWord;

    // to insert a new word
    void insert(Node*& root, string word, string definition, int pos = 0) {
        if (root == nullptr) {
            root = new Node(word[pos]);
        }

        if (word[pos] < root->data) {
            insert(root->left, word, definition, pos);
        }
        else if (word[pos] > root->data)
        {
            insert(root->right, word, definition, pos);
        }
        else {
            if (pos < word.size() - 1) {
                insert(root->middle, word, definition, pos + 1);
            }
            else {
                root->EOS = 1;
                root->listDef.push_back(definition);
            }
        }
    }
    void insertVec(Node*& root, string word, vector<string> definition, int pos = 0) {
        if (root == nullptr) {
            root = new Node(word[pos]);
        }

        if (word[pos] < root->data) {
            insertVec(root->left, word, definition, pos);
        }
        else if (word[pos] > root->data)
        {
            insertVec(root->right, word, definition, pos);
        }
        else {
            if (pos < word.size() - 1) {
                insertVec(root->middle, word, definition, pos + 1);
            }
            else {
                root->EOS = 1;
                root->listDef = definition;
            }
        }
    }

    // to remove a word
    void remove(Node*& root, string word, int pos = 0) {
        if (root == nullptr) return;

        if (word[pos] < root->data) remove(root->left, word, pos);
        else {
            if (word[pos] > root->data) remove(root->right, word, pos);
            else {
                if (pos < word.size() - 1) {
                    remove(root->middle, word, pos + 1);
                }
                else {
                    root->listDef.clear();
                    root->EOS = 0;
                    return;
                }
            }
        }
    }

    // to search a word, return true or false corresponding to successful find or not
    bool searchTST(Node* root, string word, int pos)
    {
        if (root == nullptr)
        {
            return false;
        }

        if (root->data > word[pos])
            return searchTST(root->left, word, pos);
        else if (root->data < word[pos])
            return searchTST(root->right, word, pos);
        else {
            if (pos + 1 == (int)word.size())
            {
                return root->EOS == 1;
            }
            return searchTST(root->middle, word, pos + 1);
        }
    }

    char convertUpperCaseToLowerCase(char c)
    {
        return (c >= 'A' && c <= 'Z') ? char(int(c) + 32) : c;
    }

    void resetHistoryList() {
        listHistoryWord.clear();
    }

    void resetFavouriteList() {
        listFavouriteWord.clear();
    }

    void addWordToHistoryList(string word) {
        int cnt = 0;
        for (auto i : listHistoryWord) {
            if (i == word) listHistoryWord.erase(listHistoryWord.begin() + cnt);
            ++cnt;
        }

        if (listHistoryWord.size() < numHistoryWord)
            listHistoryWord.push_back(word);
        else {
            listHistoryWord.erase(listHistoryWord.begin());
            listHistoryWord.push_back(word);
        }
    }

    bool isInFavouriteList(string word) {
        for (auto i : listFavouriteWord) if (i == word) return true;

        return false;
    }

    void addWordToFavouriteList(string word) {
        int cnt = 0;
        for (auto i : listFavouriteWord) {
            if (i == word) listFavouriteWord.erase(listFavouriteWord.begin() + cnt);
            ++cnt;
        }

        if (listFavouriteWord.size() < numFavouriteWord) {
            listFavouriteWord.push_back(word);
        }
        else {
            listFavouriteWord.erase(listHistoryWord.begin());
            listFavouriteWord.push_back(word);
        }
    }

    void removeWordFromFavouriteList(string word) {
        int cnt = 0;
        for (auto i : listFavouriteWord) {
            if (i == word) listFavouriteWord.erase(listFavouriteWord.begin() + cnt);
            ++cnt;
        }
    }

    void removeWordFromHistoryList(string word) {
        int cnt = 0;
        for (auto i : listHistoryWord) {
            if (i == word) listHistoryWord.erase(listHistoryWord.begin() + cnt);
            ++cnt;
        }
    }

    // return a vector contains all definition of my searching word
    vector <string> searchDefinition(Node* root, string word, int pos = 0) {
        if (root == nullptr) {
            vector <string> res;
            res.clear();
            return res;
        }

        if (root->data > word[pos])
            return searchDefinition(root->left, word, pos);
        else {
            if (root->data < word[pos])
                return searchDefinition(root->right, word, pos);
            else {
                if (pos + 1 == int(word.size())) {
                    /*
                        if (listHistoryWord.size() < numHistoryWord)
                            listHistoryWord.push_back(word);
                        else {
                            listHistoryWord.erase(listHistoryWord.begin());
                            listHistoryWord.push_back(word);
                        }
                    */
                    return root->listDef;
                }
                return searchDefinition(root->middle, word, pos + 1);
            }
        }
    }

    // to edit a definition of a word
    void edit(Node* root, string word, string newDef, int pos = 0) {
        if (!root) return;

        if (root->data > word[pos])
            edit(root->left, word, newDef, pos);
        else {
            if (root->data < word[pos])
                edit(root->right, word, newDef, pos);
            else {
                if (pos + 1 == int(word.size())) {
                    if (!root->listDef.empty()) {
                        root->listDef[0] = newDef;
                    }
                    return;
                }
                edit(root->middle, word, newDef, pos + 1);
            }
        }
    }

    // to reset my tree
    void reset(Node*& root) {
        if (!root) return;

        reset(root->left);
        reset(root->middle);
        reset(root->right);

        root->destroy();
        root = nullptr;
    }

    // to traverse the tree to find the suggestion of my word
    void traverseToFillWord(Node* root, vector <string>& res, char* buffer, string pattern, int depth = 0) {
        if (!root) return;

        buffer[depth] = root->data;

        if (root->EOS) {
            buffer[depth + 1] = '\0';
            string resStr = "";
            int len = pattern.size() - 1;
            for (int i = 0; i < len; i++) {
                resStr += pattern[i];
            }

            resStr += string(buffer);
            if (res.size() < numFillWord)
                res.push_back(resStr);
            else return;
        }

        if (res.size() >= numFillWord) return;

        traverseToFillWord(root->left, res, buffer, pattern, depth);

        if (res.size() >= numFillWord) return;

        traverseToFillWord(root->middle, res, buffer, pattern, depth + 1);

        if (res.size() >= numFillWord) return;

        traverseToFillWord(root->right, res, buffer, pattern, depth);
    }

    // to fill my current word (suggest to fill)
    vector <string> fillCurrentWord(Node* root, string pattern) {
        char buffer[1005];

        vector<string> res;

        traverseToFillWord(root, res, buffer, pattern);

        return res;
    }

    // to return a vector including the suggestion of my word
    vector <string> autoComplete(Node* root, string pattern) {
        vector <string> res;

        int pos = 0;

        if (pattern.empty()) return res;

        while (root && pos < pattern.size()) {
            if (root->data > pattern[pos]) {
                root = root->left;
            }
            else {
                if (root->data < pattern[pos]) root = root->right;
                else {
                    if (pos + 1 == pattern.size()) break;
                    root = root->middle;
                    pos++;
                }
            }
        }

        if (!root) return res;

        res = fillCurrentWord(root, pattern);
        return res;
    }

    // to view the word in my history of searching
    void viewListHistoryWord() {
        cout << listHistoryWord.size() << endl;
        for (auto i : listHistoryWord) cout << i << '\n';
    }


    long long random(long long L, long long R) {
        auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 mt(seed);
        return L + mt() % (R - L + 1LL);
    }

    // to random word
    string randomWord(Node* root, char* buffer, int pos = 0) {
        if (!root) return "";

        if (!root->left && !root->right && !root->middle) {
            buffer[pos] = root->data;
            buffer[++pos] = '\0';
            string res = string(buffer);
            return res;
        }

        int isChoose = random(0, 10);
        if (root->EOS && isChoose == 0) {
            buffer[pos] = root->data;
            buffer[++pos] = '\0';
            string res = string(buffer);
            return res;
        }

        Node* nextNode;

        while (1) {
            int curRand = random(1, 100);

            nextNode = root;

            if (curRand < 5) nextNode = root->left;
            else {
                if (curRand < 20) nextNode = root->middle;
                else nextNode = root->right;
            }

            if (nextNode) {
                break;
            }
        }

        string res = "";
        buffer[pos] = root->data;
        if (nextNode == root->left) {
            res = randomWord(nextNode, buffer, pos);
        }
        else {
            if (nextNode == root->right) {
                res = randomWord(nextNode, buffer, pos);
            }
            else {
                res = randomWord(nextNode, buffer, pos + 1);
            }
        }

        if (res == "") {
            if (root->EOS) {
                buffer[++pos] = '\0';
                res = string(buffer);
                return res;
            }
        }
        return res;
    }

    string genRandomWord()
    {
        char buffer[105];
        memset(buffer, 0, sizeof(buffer));

        string res = randomWord(root, buffer);

        return res;
    }

    void quizGame() {
        char buffer[105];

        string res = randomWord(root, buffer);

        cout << res << '\n';
    }

    // to travese all my tree
    void traverseTSTuntil(Node* root, char* buffer, int depth = 0)
    {
        if (root)
        {
            traverseTSTuntil(root->left, buffer, depth);
            buffer[depth] = root->data;
            if (root->EOS == 1)
            {
                buffer[depth + 1] = '\0';
                cout << buffer;
                cout << ": ";
                for (auto i : root->listDef) cout << i << ", ";
                cout << '\n';
            }
            traverseTSTuntil(root->middle, buffer, depth + 1);
            traverseTSTuntil(root->right, buffer, depth);
        }
    }

    // to traverse all my tree
    void traverseTST(Node* root)
    {
        char buffer[1000];
        traverseTSTuntil(root, buffer);
    }

    void selectData(string filePath) {
        ifstream fin;
        fin.open(filePath.c_str());

        string myWord;

        while (getline(fin, myWord)) {
            string keyWord = "";
            vector <string> definitionList;
            int i = 0;
            while (i < myWord.size() - 2) {
                if (myWord[i] == ' ' && myWord[i + 1] == ' ' && myWord[i + 2] == ':' && myWord[i + 3] == ' ' && myWord[i + 4] == ' ') break;
                keyWord += myWord[i++];
            }

            i += 5;

            string tmpStr = "";
            while (i < myWord.size()) {
                if (myWord[i] == ';') {
                    definitionList.push_back(tmpStr);
                    tmpStr = "";
                    i += 2;
                }
                else {
                    tmpStr += myWord[i++];
                }
            }

            if (keyWord != "")
                for (auto definition : definitionList)
                    this->insert(this->root, keyWord, definition);
        }
    }

    void saveHistoryListToFile(string filePath) {
        ofstream fout(filePath.c_str());

        for (auto i : listHistoryWord) {
            fout << i << '\n';
        }

        fout.close();
    }

    void saveFavouriteListToFile(string filePath) {
        ofstream fout(filePath.c_str());

        for (auto i : listFavouriteWord) {
            fout << i << '\n';
        }

        fout.close();
    }

    struct TreeFormat {
        int nextState = -1;
        Node* nextNode = nullptr;
    };

    vector <TreeFormat> listTreeNode;
    int posTreeNode;

    void traverseTreeToSave(Node* root, int depth = 0) {
        if (root) {
            if (root->left) {
                TreeFormat tmp;
                tmp.nextState = 0;
                tmp.nextNode = root->left;
                listTreeNode.push_back(tmp);
                traverseTreeToSave(root->left, depth);
            }

            if (root->middle) {
                TreeFormat tmp;
                tmp.nextState = 1;
                tmp.nextNode = root->middle;
                listTreeNode.push_back(tmp);
                traverseTreeToSave(root->middle, depth + 1);
            }

            if (root->right) {
                TreeFormat tmp;
                tmp.nextState = 2;
                tmp.nextNode = root->right;
                listTreeNode.push_back(tmp);
                traverseTreeToSave(root->right, depth);
            }

            TreeFormat tmp;
            tmp.nextState = -1;

            listTreeNode.push_back(tmp);
        }
    }

    void saveTreeToTxt(string filePath) {
        ofstream fout(filePath);

        listTreeNode.clear();

        if (root) {
            fout << "# " << 5 << ' ' << root->data << ' ' << root->EOS << ' ' << root->listDef.size() << '\n';
            for (auto i : root->listDef) fout << i << '\n';
        }

        traverseTreeToSave(root);

        for (auto i : listTreeNode) {
            fout << "# " << i.nextState << ' ';
            if (i.nextState == -1) {
                fout << '\n';
                continue;
            }
            Node* nextNode = i.nextNode;
            fout << nextNode->data << ' ' << nextNode->EOS << ' ' << nextNode->listDef.size() << '\n';
            for (auto it : nextNode->listDef) fout << it << '\n';
        }

        fout.close();
    }

    void traverseToLoad(Node*& root) {
        if (posTreeNode >= listTreeNode.size()) return;

        if (posTreeNode == 0) {
            root = listTreeNode[posTreeNode].nextNode;
            posTreeNode++;
            traverseToLoad(root);
        }
        else {
            for (int i = 1; i <= 4; i++) {
                if (listTreeNode[posTreeNode].nextState == -1) {
                    // cout << "* " << posTreeNode << endl;
                    posTreeNode++;
                    return;
                }

                if (listTreeNode[posTreeNode].nextState == 0) {
                    root->left = listTreeNode[posTreeNode].nextNode;
                    posTreeNode++;
                    traverseToLoad(root->left);
                }
                else {
                    if (listTreeNode[posTreeNode].nextState == 1) {
                        root->middle = listTreeNode[posTreeNode].nextNode;
                        posTreeNode++;
                        traverseToLoad(root->middle);
                    }
                    else {
                        if (listTreeNode[posTreeNode].nextState == 2) {
                            root->right = listTreeNode[posTreeNode].nextNode;
                            posTreeNode++;
                            traverseToLoad(root->right);
                        }
                    }
                }
            }

            // cout << "# " << root->data << ' ' << posTreeNode << endl;
        }
    }

    void loadTreeFromTxt(string filePath) {
        ifstream fin(filePath.c_str());

        listTreeNode.clear();
        char isNode;
        int cnt = 0;

        while (fin >> isNode) {
            ++cnt;
            int nextState;

            fin >> nextState;

            TreeFormat tmp;
            tmp.nextState = nextState;

            if (nextState == -1) {
                // cout << isNode << ' ' << nextState << endl;           
                listTreeNode.push_back(tmp);
            }
            else {
                int numDef;
                tmp.nextNode = new Node;
                fin.get();
                fin.get(tmp.nextNode->data);

                fin >> tmp.nextNode->EOS >> numDef;

                if (numDef > 0) {
                    fin.get();

                    for (int i = 1; i <= numDef; i++) {
                        string curStr; getline(fin, curStr);

                        tmp.nextNode->listDef.push_back(curStr);
                    }
                }

                listTreeNode.push_back(tmp);
            }
        }

        if (listTreeNode.size() != 0) {
            posTreeNode = 0;
            traverseToLoad(root);
        }

        fin.close();
    }

    void loadFavouriteListFromTxt(string filePath)
    {
        ifstream fin(filePath.c_str());
        listFavouriteWord.clear();
        string s;
        while (getline(fin, s))
        {
            listFavouriteWord.push_back(s);
        }
        fin.close();
    }

    void loadHistoryListFromTxt(string filePath)
    {
        ifstream fin(filePath.c_str());
        listHistoryWord.clear();
        string s;
        while (getline(fin, s))
        {
            listHistoryWord.push_back(s);
        }
        fin.close();
    }
};

