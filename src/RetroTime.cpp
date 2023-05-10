#include "CGame.h"

int main(int argc, char *argv[])
{
	CGame *Game = new CGame();
	Game->Run(argc, argv);
	delete Game;
	return 0;
}
