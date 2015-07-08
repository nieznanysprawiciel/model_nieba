#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSlider>

#include "skydisplayer.h"
#include "SunPosition.h"

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
	SkyDisplayer* sky_display;

	QLabel*				albedo_labels[9];
	QDoubleSpinBox*		albedo_spinboxes[9];
	QSlider*			albedo_sliders[9];

	SunPosition			sun_position;

	void recomputeSunPosition();
public slots:
    void value_changed(double);
    void value_changed(int);
private slots:
    void on_generate_clicked();
    void generation_ended();
	void save_file();
	void calendarChanged();
	void dateTimeChanged();

	void version_RGB();
	void version_spectral();

	void latitudeChanged( double value );
	void longitudeChanged( double value );
};

#endif // MAINWINDOW_H
