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
	MainWindow(vector <vector <int> > *cords);


protected:
	void closeEvent(QCloseEvent *event);

	private slots:
	void open();
	void save();
	void about();

private:
	void CreateActions();
	void CreateMenus();
	bool maybeSave();
	bool saveFile(const QByteArray &fileFormat);


	ReplayArea *replayarea;

	QMenu *saveAsMenu;
	QMenu *fileMenu;
	QMenu *optionMenu;
	QMenu *helpMenu;

	QAction *openAct;
	QList<QAction *> saveAct;
	QAction *exitAct;
	QAction *printAct;
	QAction *clearScreenAct;


};

#endif // MAINWINDOW_H
