#pragma once
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

void onChangeGameMode(BackendGui& gui, TenarySearchTree tree , int mode );
class gg{
public :
	int numRound = 0, numRight = 0, numWrong = 0;
	void loadWord(BackendGui& gui, TenarySearchTree tree , int mode)
	{
		string wordGame = tree.genRandomWord();
		vector<string> listOrigin = tree.searchDefinition(tree.root, wordGame);
		string ansDef = listOrigin[0];
		vector<string> listAns;
		if (mode == 1)
			listAns.push_back(ansDef);
		else listAns.push_back(wordGame);

		if (mode == 1)
		{

			gui.get<Button>("Word Label")->setText(wordGame);
			for (int i = 0; i < 3; i++)
			{
				while (true)
				{
					string temp = tree.genRandomWord();
					if (temp == wordGame) continue;
					vector<string> tempvc = tree.searchDefinition(tree.root, temp);
					int index = tree.random(0, tempvc.size() - 1);
					string nowWord = tempvc[index];
					bool ok = true; 
					for (string t : listAns)
						ok &= t != nowWord;
					if (!ok) continue;
					listAns.push_back(nowWord);
					break;
				}
			}
		}
		else {
			gui.get<Button>("Word Label")->setTextSize(15);
			gui.get<Button>("Word Label")->setText(ansDef);
			for (int i = 0; i < 3; i++)
			{
				while (true)
				{
					string temp = tree.genRandomWord();
					if (temp == wordGame) continue;
					listAns.push_back(temp);
					break;
				}
			}
		}

		int indexAns = tree.random(1,4);
		swap(listAns[0], listAns[indexAns]);

		for (int i = 1; i <= 4; i++)
		{
			string name = "Answer" + to_string(i);
			gui.get<Group>("groupIngame")->get<Button>(name)->setText(listAns[i - 1]);
		}

	}
	void beginGame(BackendGui& gui, TenarySearchTree tree, int mode)
	{
		numRound = numRight = numWrong = 0;
		gui.get<Label>("GameRoundNumberLabel")->setText("0/50");
		gui.get<Label>("RightNumber")->setText("0 Right");
		gui.get<Label>("WrongNumber")->setText("0 Wrong");  
		loadWord(gui, tree, mode);
	}
	void loadWidgetGame(BackendGui& gui)
	{

		auto groupIngame= tgui::Group::create();
		groupIngame->loadWidgetsFromFile("Assets/Form/IngameForm/IngameForm.txt");
		gui.add(groupIngame, "groupIngame");
		gui.get<Group>("groupIngame")->setVisible(false);
	}
	void setupButton(BackendGui& gui, TenarySearchTree tree)
	{
		gui.get<Button>("ChooseGameModeButton1")->onClick(&onChangeGameMode, ref(gui) , tree , 1);
		gui.get<Button>("ChooseGameModeButton2")->onClick(&onChangeGameMode, ref(gui) , tree , 2);
	}
	void startupGame(BackendGui& gui, TenarySearchTree tree)
	{
		loadWidgetGame(gui);
		setupButton(gui , tree);
	}

} Game;

string gamerule[2] = { "Choose the right definition for the word"  ,"Choose the right word for the definition" };
void onChangeGameMode(BackendGui& gui, TenarySearchTree tree , int mode)
{
	gui.get<Group>("groupGame")->setVisible(false);
	gui.get<Group>("groupIngame")->setVisible(true);
	gui.get<Label>("GameModeLabel")->setText(to_string(mode));
	gui.get<Label>("GameRuleLabel")->setText(gamerule[mode - 1]);
	Game.beginGame(gui , tree ,  mode);
}