#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDate>
#include <QDateTime>
#include <QCalendarWidget>

#include <ctime>


#include "glm/glm.hpp"
#include "glm/gtx/vector_angle.hpp"

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


	albedo_sliders[0] = ui->slider_albedo1;
	albedo_sliders[1] = ui->slider_albedo2;
	albedo_sliders[2] = ui->slider_albedo3;
	albedo_sliders[3] = ui->slider_albedo4;
	albedo_sliders[4] = ui->slider_albedo5;
	albedo_sliders[5] = ui->slider_albedo6;
	albedo_sliders[6] = ui->slider_albedo7;
	albedo_sliders[7] = ui->slider_albedo8;
	albedo_sliders[8] = ui->slider_albedo9;

	albedo_spinboxes[0] = ui->SpinBox_albedo1;
	albedo_spinboxes[1] = ui->SpinBox_albedo2;
	albedo_spinboxes[2] = ui->SpinBox_albedo3;
	albedo_spinboxes[3] = ui->SpinBox_albedo4;
	albedo_spinboxes[4] = ui->SpinBox_albedo5;
	albedo_spinboxes[5] = ui->SpinBox_albedo6;
	albedo_spinboxes[6] = ui->SpinBox_albedo7;
	albedo_spinboxes[7] = ui->SpinBox_albedo8;
	albedo_spinboxes[8] = ui->SpinBox_albedo9;

	albedo_labels[0] = ui->label_albedo1;
	albedo_labels[1] = ui->label_albedo2;
	albedo_labels[2] = ui->label_albedo3;
	albedo_labels[3] = ui->label_albedo4;
	albedo_labels[4] = ui->label_albedo5;
	albedo_labels[5] = ui->label_albedo6;
	albedo_labels[6] = ui->label_albedo7;
	albedo_labels[7] = ui->label_albedo8;
	albedo_labels[8] = ui->label_albedo9;

	for( unsigned k = 0; k < 9; ++k )
	{
		int wave_length = 400 + k * 40;
		albedo_labels[k]->setText( QString::number(wave_length) + " nm");
	}

	for( unsigned int i = 0; i < 9; ++i )
	{
		connect(albedo_sliders[i], SIGNAL(valueChanged(int)), this, SLOT(value_changed(int)));
		connect(albedo_spinboxes[i], SIGNAL(valueChanged(double)), this, SLOT(value_changed(double)));
	}

	connect( ui->UseSpectralVersion, SIGNAL(clicked()), this, SLOT(version_spectral()));
	connect( ui->UseRGBVersion, SIGNAL(clicked()), this, SLOT(version_RGB()));

    //slidery
	connect(ui->sky_intensity,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
	connect(ui->solar_intensity,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
	connect(ui->slider_solar_elevation,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
	connect(ui->slider_view_angle,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
	connect(ui->slider_turbidity,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
	connect(ui->slider_latitude,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
	connect(ui->slider_longitude,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
    //spinboxy
	connect(ui->sky_intensity_spinbox,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
	connect(ui->solar_intensity_spinbox,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
	connect(ui->SpinBox_solar_elevation,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
	connect(ui->SpinBox_view_angle,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
	connect(ui->SpinBox_turbidity,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
	connect(ui->SpinBox_latitude,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
	connect(ui->SpinBox_longitude,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));

	connect(ui->SpinBox_latitude,SIGNAL(valueChanged(double)),this,SLOT(latitudeChanged(double)));
	connect(ui->SpinBox_longitude,SIGNAL(valueChanged(double)),this,SLOT(longitudeChanged(double)));
	//actions
	connect(ui->actionZapisz_obraz,SIGNAL(triggered()),this,SLOT(save_file()));
    //zakończenie generowania nieba
    connect(sky_display,SIGNAL(sky_completed()),this,SLOT(generation_ended()));

	//kalendarz
	connect(ui->calendarWidget, SIGNAL(selectionChanged()), this, SLOT(calendarChanged()) );
	connect(ui->dateTimeEdit, SIGNAL(dateTimeChanged(QDateTime)), this,SLOT(dateTimeEditChanged()) );

	ui->dateTimeEdit->setDate(ui->calendarWidget->selectedDate());	// Ustawiamy czas, żeby był ten sam w obu kontrolkach.
}

MainWindow::~MainWindow()
{
    delete ui;
    delete sky_display;
}

/**Został zaznaczony RadioButton odpowiadający za włączenie wersji RGB.
Wyłączamy odpowiednie przyciski i zmieniamy podpisy na etykietach.*/
void MainWindow::version_RGB()
{
	for( unsigned int i = 3; i < 9; ++i )
	{
		albedo_sliders[i]->setEnabled( false );
		albedo_spinboxes[i]->setEnabled( false );
		albedo_labels[i]->setEnabled( false );
	}

	albedo_labels[0]->setText("RED");
	albedo_labels[1]->setText("GREEN");
	albedo_labels[2]->setText("BLUE");
}

/**Został zaznaczony RadioButton odpowiadający za włączenie wersji spektralnej.
Włączamy odpowiednie przyciski i zmieniamy podpisy na etykietach.*/
void MainWindow::version_spectral()
{
	for( unsigned int i = 3; i < 9; ++i )
	{
		albedo_sliders[i]->setEnabled( true );
		albedo_spinboxes[i]->setEnabled( true );
		albedo_labels[i]->setEnabled( true );
	}

	for( unsigned k = 0; k < 3; ++k )
	{
		int wave_length = 400 + k * 40;
		albedo_labels[k]->setText( QString::number(wave_length) + " nm");
	}
}

void MainWindow::calendarChanged()
{
	QDate selectedDate = ui->calendarWidget->selectedDate();
	ui->dateTimeEdit->setDate( selectedDate );
	latitudeChanged( 0.0 );		// Value not used
}

void MainWindow::dateTimeEditChanged()
{
	QDate selectedDate = ui->dateTimeEdit->date();
	ui->calendarWidget->setSelectedDate( selectedDate );
	latitudeChanged( 0.0 );		// Value not used
}

/**Funkcja zapewnia, że odpowiadające sobie SpinBoxy i slidery, będą
modyfikowały swoje wartości.*/
void MainWindow::value_changed(double value)
{
	int ret_value = (int)round( value * 100.0 );

	for( unsigned int i = 0; i < 9; ++i )
		if( sender() == albedo_spinboxes[i] )
		{
			if( ui->BindAlbedoCheckBox->isChecked() )
			{
				for( unsigned int j = 0; j < 9; ++j )
					albedo_sliders[j]->setValue( ret_value );
			}
			else
				albedo_sliders[i]->setValue( ret_value );

			return;
		}

	if( sender() == ui->sky_intensity_spinbox )
		ui->sky_intensity->setValue(ret_value);
	else if( sender() == ui->solar_intensity_spinbox )
		ui->solar_intensity->setValue(ret_value);
	else if( sender() == ui->SpinBox_view_angle )
		ui->slider_view_angle->setValue(ret_value);
	else if( sender() == ui->SpinBox_solar_elevation )
		ui->slider_solar_elevation->setValue(ret_value);
	else if( sender() == ui->SpinBox_turbidity )
		ui->slider_turbidity->setValue(ret_value);
	else if( sender() == ui->SpinBox_latitude )
		ui->slider_latitude->setValue(ret_value);
	else if( sender() == ui->SpinBox_longitude )
		ui->slider_longitude->setValue(ret_value);
}

/**Funkcja zapewnia, że odpowiadające sobie SpinBoxy i slidery, będą
modyfikowały swoje wartości.*/
void MainWindow::value_changed(int value)
{
	double ret_value = (double)value/(double)100;

	for( unsigned int i = 0; i < 9; ++i )
		if( sender() == albedo_sliders[i] )
		{
			if( ui->BindAlbedoCheckBox->isChecked() )
			{
				for( unsigned int j = 0; j < 9; ++j )
					albedo_spinboxes[j]->setValue( ret_value );
			}
			else
				albedo_spinboxes[i]->setValue( ret_value );

			return;
		}

	if( sender() == ui->sky_intensity )
		ui->sky_intensity_spinbox->setValue(ret_value);
	else if( sender() == ui->solar_intensity )
		ui->solar_intensity_spinbox->setValue(ret_value);
	else if( sender() == ui->slider_view_angle )
		ui->SpinBox_view_angle->setValue(ret_value);
	else if( sender() == ui->slider_solar_elevation )
		ui->SpinBox_solar_elevation->setValue(ret_value);
	else if( sender() == ui->slider_turbidity )
		ui->SpinBox_turbidity->setValue(ret_value);
	else if( sender() == ui->slider_latitude)
		ui->SpinBox_latitude->setValue(ret_value);
	else if( sender() == ui->slider_longitude )
		ui->SpinBox_longitude->setValue(ret_value);
}

void MainWindow::latitudeChanged( double value )
{
//	QDateTime dateTime( QDate(1970,1,1), QTime(0,0,0) );
//	unsigned int seconds = dateTime.secsTo( ui->dateTimeEdit->dateTime() );
	QDateTime dateTime = ui->dateTimeEdit->dateTime();
	time_t time = dateTime.toTime_t();

	sun_position.setSunConditions(	ui->SpinBox_latitude->value(),
									ui->SpinBox_longitude->value(),
									gmtime( &time ));
	recomputeSunPosition();
}

void MainWindow::longitudeChanged( double value )
{
//	QDateTime dateTime( QDate(1970,1,1), QTime(0,0,0) );
//	unsigned int seconds = dateTime.secsTo( ui->dateTimeEdit->dateTime() );
	QDateTime dateTime = ui->dateTimeEdit->dateTime();
	dateTime = dateTime.toLocalTime();
	time_t time = dateTime.toTime_t();

	sun_position.setSunConditions(	ui->SpinBox_latitude->value(),
									ui->SpinBox_longitude->value(),
									gmtime( &time ));
	recomputeSunPosition();
}

void MainWindow::recomputeSunPosition()
{
//	glm::vec3 sun_direction = sun_position.computeSunDirection();
//	glm::vec3 zenith_direction( 0.0, 1.0, 0.0 );

//	double elevation = glm::angle( sun_direction, zenith_direction );

//	sun_direction.z = 0.0;
//	sun_direction = glm::normalize( sun_direction );
//	double vertical_angle = glm::angle( sun_direction, glm::vec3( 0.0, 0.0, -1.0 ) );

	double vertical_angle = sun_position.computeAzimuth();
	double elevation = sun_position.computeElevation();
	if( elevation < 0 )
		elevation = 0.0;

	ui->SpinBox_solar_elevation->setValue( glm::degrees( elevation ) );
	ui->spinBox_horizontal->setValue( (int)glm::degrees( vertical_angle ) );
}

/**@brief Funkcja wykonywana w reakcji na wciśnięcie przycisku do generowania nieba.

Funkcja pobiera parametry z odpowiednich kontrolek i przekazuje do klasy QtSkyDisplay.
Przycisk do generowania nieba jest blokowany do zakończenia operacji. Wyniki generowania
pojawiają się na bierząco na ekranie dzięki timerowi, który jest uruchamiany i cyklicznie
odświeża okno do momentu zakończenia.
*/
void MainWindow::on_generate_clicked()
{
	// Blokujemy możliwość genrowania nieba, do czasu gdy będzie już gotowe.
    ui->generate->setEnabled(false);

	sky_display->set_sky_intensity((float)ui->sky_intensity_spinbox->value());
	sky_display->set_solar_intensity((float)ui->solar_intensity_spinbox->value());
	sky_display->set_solar_elevation((float)ui->SpinBox_solar_elevation->value());
	sky_display->set_gamma_correction( ui->gamma_correction->value() );

	// Włącza lub wyłącza dithering.
	if( ui->dithering->isChecked() )
		sky_display->set_dithering(1);
	else
		sky_display->set_dithering(0);

	// Ustawia wersję modelu, która ma zostać użyta.
	if( ui->UseSpectralVersion->isChecked() )
		sky_display->set_version_spectral();
	else
		sky_display->set_version_RGB();

	// Pobieramy wartości albedo. W wersji RGB zostaną użyte 3 pierwsze, pozostałe zostaną zignorowane.
	double albedo[9];
	for( int i = 0; i < 9; ++i )
		albedo[i] = albedo_spinboxes[i]->value();

	sky_display->generate_sky(	ui->sky_width->value(),
								ui->sky_height->value(),
								ui->SpinBox_view_angle->value(),
								albedo,
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

