#include "Core.h"
#include "Menu.h"

void run()
{
	RenderWindow window{ {1150, 800}, "Dictionary" };
	Gui gui{ window };
	TenarySearchTree tree;
	tree.selectData("Assets/Data/OCHO.txt");
	if (runMenu(gui, tree))
		//cerr << 1 << '\n';
		gui.mainLoop();
}