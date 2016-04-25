#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	while (w.GetGameState() == GameState::Playing)
	{
		a.processEvents();
		w.UpdateGame();
	}
	a.exit();
	return 0; 
}
