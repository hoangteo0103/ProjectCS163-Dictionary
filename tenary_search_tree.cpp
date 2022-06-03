#include<iostream>
using namespace std;
struct Node {
    char data ;
    int EOS = 1 ;
    Node *left , *middle , *right ;
    Node(char data)
    {
        this->data = data ;
        this->EOS = 0 ;
        this->left = this->middle = this->right = NULL ;
    }
};

void insert(Node* &root , string word , int pos = 0 )
{
    if(root == nullptr)
    {
        root = new Node(word[pos]);
    }

    if(word[pos] < root->data)
    {
        insert(root->left , word , pos ) ;
    }
    else if(word[pos] > root->data)
    {
        insert(root->right , word , pos ) ;
    }
    else {
        if(pos < word.size() - 1)
        {
            insert(root->middle,word ,pos + 1);
        }
        else root->EOS = 1 ;
    }
}

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

void traverseTSTuntil(Node* root , char *buffer , int depth = 0 )
{
    if(root)
    {
        traverseTSTuntil(root->left , buffer , depth);
        buffer[depth] = root->data ;
        if(root->EOS == 1)
        {
            buffer[depth + 1] = '\0';
            cout << buffer ;
        }
        traverseTSTuntil(root->middle , buffer , depth + 1);
        traverseTSTuntil(root->right , buffer , depth) ;
    }
}

void traverseTST(Node* root )
{
    char buffer[100];
    traverseTSTuntil(root , buffer ) ;
}

int main()
{
    Node *root = nullptr ;
    insert(root , "conga");
    insert(root , "dm");
}
