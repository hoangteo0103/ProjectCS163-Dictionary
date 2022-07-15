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

	void loadFavouriteList()
	{
		ifstream fin("D:/Project_CS163/Dictionary/Dictionary/Assets/FavouriteWord");
		string word;
		while (fin >> word)
		{
			isFavourited[word] = true;
		}
	}

} favData;

