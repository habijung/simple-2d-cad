#include <QPushButton>
#include <QBoxLayout>
#include "MainWindow.h"


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);

	this->setMenuBar();
	this->setToolbar();

	this->widget = new Viewport(this);
	setCentralWidget(this->widget);

	this->setSidebarWidget(this->widget);
	this->setUnderbarWidget(this->widget);

	// State Machine Test
	stateMachine = new StateMachine();
	this->stateMachine->addState("s1");
	this->stateMachine->addState("s2");
	this->stateMachine->printAllStates();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setMenuBar()
{
	/* Menu Icon */
	QPixmap newpix("img/new.png");
	QPixmap openpix("img/open.png");
	QPixmap quitpix("img/quit.png");


	/* File */
	QAction* newa = new QAction(newpix, "&New", this);
	QAction* open = new QAction(openpix, "&Open", this);
	QAction* quit = new QAction(quitpix, "&Quit", this);
	quit->setShortcut(tr("ESC"));

	QMenu* file = menuBar()->addMenu("&File");
	file->addAction(newa);
	file->addAction(open);
	file->addSeparator();
	file->addAction(quit);

	qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);  // Icons are no longer shown in menus

	connect(quit, &QAction::triggered, qApp, QApplication::quit);


	/* Edit */
	QMenu* edit = menuBar()->addMenu("&Edit");
	edit->addAction(newa);
	edit->addAction(open);


	/* View */
	QAction* viewst = new QAction("&View statusbar", this);
	viewst->setCheckable(true);
	viewst->setChecked(true);

	QMenu* view = menuBar()->addMenu("&View");
	view->addAction(viewst);
	statusBar()->showMessage("Ready");

	if (viewst->isChecked())
	{
		statusBar()->show();
	}
	else
	{
		statusBar()->hide();
	}
}

void MainWindow::setToolbar()
{
	/* Menu Icon */
	QPixmap newpix("img/new.png");
	QPixmap openpix("img/open.png");
	QPixmap quitpix("img/quit.png");


	/* Toolbar */
	QToolBar* toolbar = addToolBar("main toolbar");
	toolbar->addAction(QIcon(newpix), "New File");
	toolbar->addAction(QIcon(openpix), "Open File");
	toolbar->addSeparator();

	QAction* quit = toolbar->addAction(QIcon(quitpix), "Quit Application");

	connect(quit, &QAction::triggered, qApp, &QApplication::quit);
}

void MainWindow::setSidebarWidget(QWidget* widget)
{
	QWidget* wSidebar = new QWidget(widget);
	QPushButton* btnLine = new QPushButton("Line", wSidebar);
	QPushButton* btnFace = new QPushButton("Face", wSidebar);

	connect(btnLine, &QPushButton::clicked, [this]()
		{
			qDebug() << "Line Clicked";
		});
	connect(btnFace, &QPushButton::clicked, [this]()
		{
			qDebug() << "Face Clicked";
		});

	QVBoxLayout* vBox = new QVBoxLayout(wSidebar);
	vBox->addWidget(btnLine);
	vBox->addStretch();
	vBox->addWidget(btnFace);
	wSidebar->setLayout(vBox);
}

void MainWindow::setUnderbarWidget(QWidget* widget)
{
	QWidget* wUnderbar = new QWidget(widget);
	QPushButton* btnLine = new QPushButton("Line", wUnderbar);
	QPushButton* btnFace = new QPushButton("Face", wUnderbar);

	connect(btnLine, &QPushButton::clicked, [this]()
		{
			qDebug() << "Line Clicked";
		});
	connect(btnFace, &QPushButton::clicked, [this]()
		{
			qDebug() << "Face Clicked";
		});

	QHBoxLayout* hBox = new QHBoxLayout(wUnderbar);
	hBox->addWidget(btnLine);
	hBox->addStretch();
	hBox->addWidget(btnFace);
	wUnderbar->setLayout(hBox);
	wUnderbar->move(100, 450);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	this->widget->getKeyEvent(event);
}
