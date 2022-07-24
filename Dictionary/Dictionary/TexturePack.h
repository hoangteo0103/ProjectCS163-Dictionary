#pragma once 
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "TenarySearchTree.h"
using namespace sf;
using namespace tgui;


class {
public : 
	tgui::Texture onClickedFavouriteButtonTexture, onUnClickedFavouriteButtonTexture;

	void loadTexture()
	{
		onClickedFavouriteButtonTexture.load("Assets/Form/WordDefinitionForm1/star-button-unclicked.png");
		onUnClickedFavouriteButtonTexture.load("Assets/Form/WordDefinitionForm1/star-button-clicked.png");
	}
} texture;
