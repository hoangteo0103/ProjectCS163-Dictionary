#include "Core.h"
#include "Mode.h"

void run()
{
	RenderWindow window{ {1300, 800}, "Dictionary" };
	Gui gui{ window };
	if (runChooseMode(gui))
		//cerr << 1 << '\n';
		gui.mainLoop();
}