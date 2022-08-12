#pragma once 
#include<string>
#include<fstream>
#include<vector>
#include<map>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "TenarySearchTree.h"
using namespace sf;
using namespace tgui;

class  {
public :
	map<string, bool> isFavourited;
	map<string, bool > historyList;
	void loadFavouriteList()
	{
		ifstream fin("Assets/FavouriteWord");
		string word;
		while (fin >> word)
		{
			isFavourited[word] = true;
		}
	}
} favData;

