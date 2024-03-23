#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);

	QAction* quit = new QAction("&Quit", this);
	QMenu* file = menuBar()->addMenu("&File");
	file->addAction(quit);

	connect(quit, &QAction::triggered, qApp, QApplication::quit);
}

MainWindow::~MainWindow()
{
	delete ui;
}
