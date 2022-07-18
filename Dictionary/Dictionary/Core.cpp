
#include "Core.h"
#include "Menu.h"

void run()
{
	RenderWindow window{ {1150, 800}, "Dictionary" };
	Gui gui{ window };
	TenarySearchTree tree;
	tree.selectData("Assets/Data/EnToVn.txt");
	if (runMenu(gui, tree))
		gui.mainLoop();
	ofstream fout("D:/Project_CS163/Dictionary/Dictionary/Assets/FavouriteWord");
	for (auto t : favData.isFavourited)
	{
		cout << t.first << ' ' << t.second << '\n';
		fout << t.first  << '\n';
	}
}