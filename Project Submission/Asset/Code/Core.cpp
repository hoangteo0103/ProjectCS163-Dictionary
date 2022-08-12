#include "Core.h"
#include "Menu.h"

void run()
{
	RenderWindow window{ {1150, 900}, "Dictionary" };

	Gui gui{ window };
	if (runMenu(gui))
		gui.mainLoop();
	ofstream fout("Assets/FavouriteWord");
	for (auto t : favData.isFavourited)
	{
		cout << t.first << ' ' << t.second << '\n';
		fout << t.first  << '\n';
	}
}