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
		onClickedFavouriteButtonTexture.load("Assets/Img/star-solid_clicked.svg");
		onUnClickedFavouriteButtonTexture.load("Assets/Img/star-solid.svg");
	}
} texture;
