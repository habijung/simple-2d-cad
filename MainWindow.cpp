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
	/* File */
	QAction* newa = new QAction("&New", this);
	QAction* open = new QAction("&Open", this);
	QAction* quit = new QAction("&Quit", this);
	quit->setShortcut(tr("CTRL+Q"));

	QMenu* file = menuBar()->addMenu("&File");
	file->addAction(newa);
	file->addAction(open);
	file->addSeparator();
	file->addAction(quit);

	qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);  // Icons are no longer shown in menus

	connect(quit, &QAction::triggered, qApp, QApplication::quit);


	/* Edit */

	/* View */
	QAction* viewst = new QAction("&View statusbar", this);
	viewst->setCheckable(true);
	viewst->setChecked(true);

	QMenu* view = menuBar()->addMenu("&View");
	view->addAction(viewst);
	statusBar();

	if (viewst->isChecked())
	{
		statusBar()->show();
	}
	else
	{
		statusBar()->hide();
	}
}
