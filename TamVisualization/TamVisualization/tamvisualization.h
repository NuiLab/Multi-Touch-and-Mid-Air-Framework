#ifndef TAMVISUALIZATION_H
#define TAMVISUALIZATION_H

#include <QtWidgets/QMainWindow>
#include "ui_tamvisualization.h"

class TamVisualization : public QMainWindow
{
	Q_OBJECT

public:
	TamVisualization(QWidget *parent = 0);
	~TamVisualization();

private:
	Ui::TamVisualizationClass ui;
};

#endif // TAMVISUALIZATION_H
