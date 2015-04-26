#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#define SIZE_X  900
#define SIZE_Y  660


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Modelowanie nieba w różnych warunkach pogodowych");

	sky_display = new SkyDisplayer( SIZE_X,SIZE_Y, this );
	//drawing_area = new DrawingArea(sky_display, SIZE_X, SIZE_Y, this);
	ui->scrollDrawingArea->setWidget(sky_display);


    //slidery
    connect(ui->slider_red,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
    connect(ui->slider_green,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
    connect(ui->slider_blue,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
	connect(ui->sky_intensity,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
	connect(ui->solar_intensity,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
	connect(ui->slider_solar_elevation,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
	connect(ui->slider_view_angle,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
	connect(ui->slider_turbidity,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
    //spinboxy
    connect(ui->SpinBox_red,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
    connect(ui->SpinBox_green,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
    connect(ui->SpinBox_blue,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
	connect(ui->sky_intensity_spinbox,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
	connect(ui->solar_intensity_spinbox,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
	connect(ui->SpinBox_solar_elevation,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
	connect(ui->SpinBox_view_angle,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
	connect(ui->SpinBox_turbidity,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
	//actions
	connect(ui->actionZapisz_obraz,SIGNAL(triggered()),this,SLOT(save_file()));
    //zakończenie generowania nieba
    connect(sky_display,SIGNAL(sky_completed()),this,SLOT(generation_ended()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete sky_display;
}

void MainWindow::value_changed(double value)
{
	int ret_value = (int)round( value * 100.0 );

    if( sender() == ui->SpinBox_red )
		ui->slider_red->setValue(ret_value);
	else if( sender() == ui->SpinBox_green )
		ui->slider_green->setValue(ret_value);
	else if( sender() == ui->SpinBox_blue )
		ui->slider_blue->setValue(ret_value);
	else if( sender() == ui->sky_intensity_spinbox )
		ui->sky_intensity->setValue(ret_value);
	else if( sender() == ui->solar_intensity_spinbox )
		ui->solar_intensity->setValue(ret_value);
	else if( sender() == ui->SpinBox_view_angle )
		ui->slider_view_angle->setValue(ret_value);
	else if( sender() == ui->SpinBox_solar_elevation )
		ui->slider_solar_elevation->setValue(ret_value);
	else if( sender() == ui->SpinBox_turbidity )
		ui->slider_turbidity->setValue(ret_value);
}

void MainWindow::value_changed(int value)
{
	double ret_value = (double)value/(double)100;

    if( sender() == ui->slider_red )
		ui->SpinBox_red->setValue(ret_value);
	else if( sender() == ui->slider_green )
		ui->SpinBox_green->setValue(ret_value);
	else if( sender() == ui->slider_blue )
		ui->SpinBox_blue->setValue(ret_value);
	else if( sender() == ui->sky_intensity )
		ui->sky_intensity_spinbox->setValue(ret_value);
	else if( sender() == ui->solar_intensity )
		ui->solar_intensity_spinbox->setValue(ret_value);
	else if( sender() == ui->slider_view_angle )
		ui->SpinBox_view_angle->setValue(ret_value);
	else if( sender() == ui->slider_solar_elevation )
		ui->SpinBox_solar_elevation->setValue(ret_value);
	else if( sender() == ui->slider_turbidity )
		ui->SpinBox_turbidity->setValue(ret_value);
}

/**@brief Funkcja wykonywana w reakcji na wciśnięcie przycisku do generowania nieba.

Funkcja pobiera parametry z odpowiednich kontrolek i przekazuje do klasy QtSkyDisplay.
Przycisk do generowania nieba jest blokowany do zakończenia operacji. Wyniki generowania
pojawiają się na bierząco na ekranie dzięki timerowi, który jest uruchamiany i cyklicznie
odświeża okno do momentu zakończenia.
*/
void MainWindow::on_generate_clicked()
{
    ui->generate->setEnabled(false);

	sky_display->set_sky_intensity((float)ui->sky_intensity_spinbox->value());
	sky_display->set_solar_intensity((float)ui->solar_intensity_spinbox->value());
	sky_display->set_solar_elevation((float)ui->SpinBox_solar_elevation->value());

	if( ui->dithering->isChecked() )
		sky_display->set_dithering(1);
	else
		sky_display->set_dithering(0);

	sky_display->set_perspective_correction( ui->check_perspective_correction->isChecked() );
	sky_display->set_gamma_correction( ui->gamma_correction->value() );

	sky_display->generate_sky(ui->sky_width->value(),
							   ui->sky_height->value(),
							   ui->SpinBox_view_angle->value(),
							   ui->SpinBox_red->value(),
							   ui->SpinBox_green->value(),
							   ui->SpinBox_blue->value(),
							   ui->horizontalSlider_rotation->value(),
							   ui->verticalSlider_rotation->value(),
							   static_cast<float>( ui->SpinBox_turbidity->value() ));
}

void MainWindow::save_file()
{
	QString name = QFileDialog::getSaveFileName(this, tr("Save File"),
													"",
													tr("Images (*.png *.bmp *.jpg)"));
	sky_display->save_image(name);
}

/**@brief Funkcja wywoływana po zakończeniu generowania. Zatrzymuje timer
i odświeża obszar rysowania.
*/
void MainWindow::generation_ended()
{
	sky_display->repaint();
	int elapsed_time = sky_display->get_last_generation_time();
	ui->time_of_execution->setText(QString::number(elapsed_time));
    ui->generate->setEnabled(true);
}

