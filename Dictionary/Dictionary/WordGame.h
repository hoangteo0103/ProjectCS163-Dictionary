#pragma once
#include<string>
#include<fstream>
#include<vector>
#include<map>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "TenarySearchTree.h"
#include<random>
using namespace sf;
using namespace tgui;
tgui::Texture onWrongTexture, onRightTexture, onNormalTexture, onNormalHoverTexture;


long long random(long long L, long long R) {
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937 mt(seed);
	return L + mt() % (R - L + 1LL);
}


void onChangeGameMode(BackendGui& gui, TenarySearchTree tree , int mode );
void onWrongAns(BackendGui& gui, int id, int indexAns , TenarySearchTree tree, int mode);
void onRightAns(BackendGui& gui, int id, TenarySearchTree tree, int mode);
void updateGameScreen(BackendGui& gui);

int pos_x[] = {0, 60,580,60,580 };
int pos_y[] = {0,20,20,170,170};
int seed;
class gg{
public :
	int numRound = 0, numRight = 0, numWrong = 0;
	void loadWord(BackendGui& gui, TenarySearchTree tree , int mode)

	{
		gui.get<Group>("groupIngame")->get<Picture>("Veil")->setVisible(false);
		seed = random(1, 100000);
		updateGameScreen(gui);
		string wordGame;
		wordGame = tree.genRandomWord();
		vector<string> listOrigin = tree.searchDefinition(tree.root, wordGame);

		while(listOrigin.empty())
		{
			wordGame = tree.genRandomWord();
			listOrigin = tree.searchDefinition(tree.root, wordGame);
		}
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
					string nowWord = tempvc[0];
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
					bool ok = true;
					for (string t : listAns)
						ok &= t != temp;
					if (!ok) continue;
					listAns.push_back(temp);
					break;
				}
			}
		}
		gui.get<Group>("groupIngame")->get<Panel>("AnswerButtonPanel")->removeAllWidgets();
		int indexAns = tree.random(1,4);
		swap(listAns[0], listAns[indexAns-1]);
		for (int i = 1; i <= 4; i++)
		{
			string name = "Answer" + to_string(i) + to_string(seed);
			auto dm = tgui::Button::copy(gui.get<Group>("groupIngame")->get<Button>("AnswerButton"));
			dm->setPosition(pos_x[i] , pos_y[i]);
			dm->moveToFront();
			dm->setText(listAns[i - 1]);
			dm->setHeight(101);
			dm->setWidth(258);
			dm->getRenderer()->setTexture(onNormalTexture);
			dm->getRenderer()->setTextureHover(onNormalHoverTexture);
			dm->setVisible(true);
			//dm->showWithEffect(tgui::ShowEffectType::Fade, 1000);
			if (i != indexAns)
			{
				dm->onClick(&onWrongAns, ref(gui), i, indexAns, tree, mode);
			}
			else
			{
				dm->onClick(&onRightAns, ref(gui), i, tree, mode);
			}
			gui.get<Group>("groupIngame")->get<Panel>("AnswerButtonPanel")->add(dm, name);
		}
	}
	void beginGame(BackendGui& gui, TenarySearchTree tree, int mode)
	{
		numRound = numRight = numWrong = 0;
		updateGameScreen(gui);
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
		numRound = numRight = numWrong = 0;
		loadWidgetGame(gui);
		setupButton(gui , tree);
		onWrongTexture.load("Assets/Form/IngameForm/wrong-button.png");
		onRightTexture.load("Assets/Form/IngameForm/right-button.png");
		onNormalTexture.load("Assets/Form/IngameForm/answer-button-unhovered.png");
		onNormalHoverTexture.load("Assets/Form/IngameForm/answer-button-hovered.png");
	}

} Game;

string gamerule[2] = { "Choose the right definition for the word"  ,"Choose the right word for the definition" };

void updateGameScreen(BackendGui& gui)
{
	gui.get<Label>("GameRoundNumberLabel")->setText(to_string(Game.numRound) + "/50");
	gui.get<Label>("RightNumber")->setText(to_string(Game.numRight)+" Right");
	gui.get<Label>("WrongNumber")->setText(to_string(Game.numWrong)+" Wrong");
	gui.get<Group>("groupIngame")->get<Button>("NextButton")->setVisible(false);
	gui.get<Group>("groupIngame")->get<Button>("Noti")->setVisible(false);
}

void onNextRound(BackendGui& gui, TenarySearchTree tree, int mode)
{
	Game.loadWord(ref(gui), tree, mode);
}

void onChangeGameMode(BackendGui& gui, TenarySearchTree tree , int mode)
{
	gui.get<Group>("groupGame")->setVisible(false);
	gui.get<Group>("groupIngame")->setVisible(true);
	gui.get<Label>("GameModeLabel")->setText(to_string(mode));
	gui.get<Label>("GameRuleLabel")->setText(gamerule[mode - 1]);
	Game.beginGame(ref(gui), tree ,  mode);
}

void onWrongAns(BackendGui& gui , int id, int indexAns, TenarySearchTree tree, int mode)
{
	gui.get<Group>("groupIngame")->get<Button>("Noti")->setText("You are wronggg!!!");
	gui.get<Group>("groupIngame")->get<Button>("Noti")->setVisible(true);
	
	string name = "Answer" + to_string(id) + to_string(seed);
	gui.get<Group>("groupIngame")->get<Button>(name)->getRenderer()->setTexture(onWrongTexture);
	gui.get<Group>("groupIngame")->get<Button>(name)->getRenderer()->setTextureHover(onWrongTexture);
	
	gui.get<Group>("groupIngame")->get<Button>("Answer"+to_string(indexAns) + to_string(seed))->getRenderer()->setTexture(onRightTexture);
	gui.get<Group>("groupIngame")->get<Button>("Answer"+to_string(indexAns) + to_string(seed))->getRenderer()->setTextureHover(onRightTexture);
	
	gui.get<Group>("groupIngame")->get<Picture>("Veil")->setVisible(true);
	gui.get<Group>("groupIngame")->get<Picture>("Veil")->moveToFront();

	gui.get<Group>("groupIngame")->get<Button>("NextButton")->moveToFront();
	gui.get<Group>("groupIngame")->get<Button>("NextButton")->setVisible(true);
	Game.numRound++;
	Game.numWrong++;
	gui.get<Group>("groupIngame")->get<Button>("NextButton")->onClick(&onNextRound, ref(gui), tree, mode);

}
void onRightAns(BackendGui& gui, int id, TenarySearchTree tree, int mode)
{
	string name = "Answer" + to_string(id) + to_string(seed);
	gui.get<Group>("groupIngame")->get<Button>("Noti")->setVisible(true);
	gui.get<Group>("groupIngame")->get<Button>("Noti")->setText("You are a genius!!!");
	gui.get<Group>("groupIngame")->get<Button>(name)->getRenderer()->setTexture(onRightTexture);
	gui.get<Group>("groupIngame")->get<Button>(name)->getRenderer()->setTextureHover(onRightTexture);
	gui.get<Group>("groupIngame")->get<Button>("Noti")->hideWithEffect(tgui::ShowEffectType::Fade , 500);
	Game.numRound++;
	Game.numRight++;
	Game.loadWord(ref(gui), tree, mode);
	
}