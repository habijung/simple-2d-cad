#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);
	this->setMenuBar();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setMenuBar()
{
	QAction* quit = new QAction("&Quit", this);
	QMenu* file = menuBar()->addMenu("&File");
	file->addAction(quit);

	connect(quit, &QAction::triggered, qApp, QApplication::quit);
}
