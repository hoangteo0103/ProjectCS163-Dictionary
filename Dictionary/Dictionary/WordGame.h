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

void onChangeGameMode(BackendGui& gui)
{
	gui.get<Group>("groupGame")->setVisible(false);
	gui.get<Group>("groupIngame")->setVisible(true);
}
class {
public :
	void loadWidgetGame(BackendGui& gui)
	{

		auto groupIngame= tgui::Group::create();
		groupIngame->loadWidgetsFromFile("Assets/Form/IngameForm/IngameForm.txt");
		gui.add(groupIngame, "groupIngame");
		gui.get<Group>("groupIngame")->setVisible(false);
	}
	void setupButton(BackendGui& gui)
	{
		gui.get<Button>("ChooseGameModeButton1")->onClick(&onChangeGameMode, ref(gui));
		gui.get<Button>("ChooseGameModeButton2")->onClick(&onChangeGameMode, ref(gui));
	}
	void startupGame(BackendGui& gui)
	{
		loadWidgetGame(gui);
		setupButton(gui);
	}

} Game;