#include <bits/stdc++.h>

using namespace std;

struct Node {
    // data
    char data;
    int EOS = 1;
    Node *left , *middle , *right ;
    vector <string> listDef;

    // Constructor new node
    Node(char data)
    {
        this->data = data ;
        this->EOS = 0 ;
        this->left = this->middle = this->right = NULL ;
        listDef.clear();
    }

    // destruction
    void destroy() {
        this->EOS = 0;
        delete this->left, this->right, this->middle;
        this->left = this->middle = this->right = NULL;
        listDef.clear();
    }
};


struct TenarySearchTree {
    // number suggestion word
    static const int numFillWord = 10;
    // number word in searching history
    static const int numHistoryWord = 10;

    Node* root = nullptr;
    vector <string> listHistoryWord;

    // to insert a new word
    void insert(Node* &root , string word, string definition, int pos = 0) {
        if(root == nullptr) {
            root = new Node(word[pos]);
        }

        if(word[pos] < root->data) {
            insert(root->left, word, definition, pos) ;
        }
        else if(word[pos] > root->data)
        {
            insert(root->right, word, definition, pos) ;
        }
        else {
            if(pos < word.size() - 1) {
                insert(root->middle, word, definition, pos + 1);
            }
            else {
                root->EOS = 1;
                root->listDef.push_back(definition);
            }
        }
    }

    // to remove a word
    void remove(Node* &root, string word, int pos = 0) {
        if(root == nullptr) return;

        if(word[pos] < root->data) remove(root->left, word, pos);
        else {
            if(word[pos] > root->data) remove(root->right, word, pos);
            else {
                if(pos < word.size() - 1) {
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
    bool searchTST(Node* root , string word , int pos )
    {
        if(root == nullptr)
        {
            return false ;
        }
        if(root->data > word[pos])
            return searchTST(root->left , word , pos ) ;
        else if(root->data < word[pos])
            return searchTST(root->right , word , pos );
        else {
            if(pos + 1 == (int)word.size())
            {
                return root->EOS == 1 ;
            }
            return searchTST(root->middle, word , pos + 1) ;
        }
    }

    // return a vector contains all definition of my searching word
    vector <string> searchDefinition(Node* root, string word, int pos = 0) {
        if(root == nullptr) {
            vector <string> res;
            res.clear();
            return res;
        }

        if(root->data > word[pos])
            return searchDefinition(root->left, word, pos);
        else {
            if(root->data < word[pos])
                return searchDefinition(root->right, word, pos);
            else {
                if(pos + 1 == int(word.size())) {
                    if(listHistoryWord.size() < numHistoryWord)
                        listHistoryWord.push_back(word);
                    else {
                        listHistoryWord.erase(listHistoryWord.begin());
                        listHistoryWord.push_back(word);
                    }
                    return root->listDef;
                }
                return searchDefinition(root->middle, word, pos + 1);
            }
        }
    }

    // to edit a definition of a word
    void edit(Node* root, string word, string newDef, int pos = 0) {
        if(!root) return;

        if(root->data > word[pos])
            edit(root->left, word, newDef, pos);
        else {
            if(root->data < word[pos])
                edit(root->right, word, newDef, pos);
            else {
                if(pos + 1 == int(word.size())) {
                    if(!root->listDef.empty()) {
                        root->listDef[0] = newDef;
                    }
                    return;
                }
                edit(root->middle, word, newDef, pos + 1);
            }
        }
    }

    // to reset my tree
    void reset(Node* &root) {
        if(!root) return;

        reset(root->left);
        reset(root->middle);
        reset(root->right);

        root->destroy();
        root = nullptr;
    }

    // to traverse the tree to find the suggestion of my word
    void traverseToFillWord(Node* root, vector <string> &res, char* buffer, string pattern, int depth = 0) {
        if(!root) return;

        traverseToFillWord(root->left, res, buffer, pattern, depth);

        buffer[depth] = root->data;

        if(root->EOS) {
            buffer[depth + 1] = '\0';
            string resStr = pattern;
            resStr += string(buffer);
            res.push_back(resStr);
        }

        traverseToFillWord(root->middle, res, buffer, pattern, depth + 1);
        traverseToFillWord(root->right, res, buffer, pattern, depth);
    }

    // to fill my current word (suggest to fill)
    vector <string> fillCurrentWord(Node* root, string pattern) {
        char buffer[1001];
 
        vector<string> res;
     
        traverseToFillWord(root, res, buffer, pattern);
     
        if(root->EOS == 1)
            res.push_back(pattern);
        return res;
    }

    // to return a vector including the suggestion of my word
    vector <string> autoComplete(Node* root, string pattern) {
        vector <string> res;

        int pos = 0;

        if(pattern.empty()) return res;

        while(root && pos < pattern.size()) {
            if(root->data > pattern[pos]) {
                root = root->left;
            }
            else {
                if(root->data > pattern[pos]) root = root->right;
                else {
                    root = root->middle;
                    pos++;
                }
            }
        }

        res = fillCurrentWord(root, pattern);
        return res;
    }

    // to view the word in my history of searching
    void viewListHistoryWord() {
        for(auto i : listHistoryWord) cout << i << '\n';
    }
    
    // to travese all my tree
    void traverseTSTuntil(Node* root , char *buffer , int depth = 0 )
    {
        if(root)
        {
            traverseTSTuntil(root->left , buffer , depth);
            buffer[depth] = root->data ;
            if(root->EOS == 1)
            {
                buffer[depth + 1] = '\0';
                cout << buffer;
                cout << ": ";
                for(auto i : root->listDef) cout << i << ", ";
                cout << '\n';
            }
            traverseTSTuntil(root->middle , buffer , depth + 1);
            traverseTSTuntil(root->right , buffer , depth) ;
        }
    }

    // to traverse all my tree
    void traverseTST(Node* root )
    {
        char buffer[100];
        traverseTSTuntil(root , buffer ) ;
    }
} myKeywordTree;

// to select data from file
void selectData() {
    string myWord;
    while(getline(cin, myWord)) {
        string keyWord = "", definition = "";

        int i = 0;
        while(i < myWord.size() && myWord[i] != ':') keyWord += myWord[i++];

        i++;
        while(i < myWord.size() && myWord[i] == ' ') i++;

        while(i < myWord.size()) definition += myWord[i++];

        myKeywordTree.insert(myKeywordTree.root, keyWord, definition);
    }
}

int main()
{
    // if(fopen("Data.inp", "r"))
    //     freopen("Data.inp", "r", stdin), freopen("Data.out", "w", stdout);

    // selectData();

    // myKeywordTree.remove(myKeywordTree.root, "do");

    // myKeywordTree.reset(myKeywordTree.root);

    // myKeywordTree.traverseTST(myKeywordTree.root);

    // myKeywordTree.edit(myKeywordTree.root, "go", "di nao");

    // cout << endl << endl;

    // vector <string> res;
    // res = myKeywordTree.searchDefinition(myKeywordTree.root, "go");
    // res = myKeywordTree.autoComplete(myKeywordTree.root, "de");

    // for(auto i : res) cout << i << '\n';
}
