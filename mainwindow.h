#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "DrawingArea.h"
//#include "QtSkyDisplay.h"
#include <QTimer>
#include "skydisplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	//DrawingArea* drawing_area;
	SkyDisplayer* sky_display;

public slots:
    void value_changed(double);
    void value_changed(int);
private slots:
    void on_generate_clicked();
    void generation_ended();
	void save_file();
};

#endif // MAINWINDOW_H
