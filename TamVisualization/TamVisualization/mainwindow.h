#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QList>
#include <cstdlib>

using namespace std;

class ReplayArea;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(vector <vector <int> > *coords);


protected:
	void closeEvent(QCloseEvent *event);

	private slots:
	void open();
	void save();
	void about();

private:
	void createActions();
	void createMenus();
	bool maybeSave();
	bool saveFile(const QByteArray &fileFormat);


	ReplayArea *replayArea;

	QMenu *saveAsMenu;
	QMenu *fileMenu;
	QMenu *optionMenu;
	QMenu *helpMenu;

	QAction *openAct;
	QList<QAction *> saveAsActs;
	QAction *exitAct;
	QAction *printAct;
	QAction *clearScreenAct;
	QAction *playAct;
	QAction *aboutAct;


};

#endif // MAINWINDOW_H
