#ifndef KNVIS_H
#define KNVIS_H

#include <QtWidgets/QMainWindow>
#include "ui_knvis.h"

class KnVis : public QMainWindow
{
	Q_OBJECT

public:
	KnVis(QWidget *parent = 0);
	~KnVis();

private:
	Ui::KnVisClass ui;
};

#endif // KNVIS_H
