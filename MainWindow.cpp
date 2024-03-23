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

	// Define State Machine and States
	machine = new StateMachine();

	// Create main scene, camera, viewport
	mScene = new Scene(this);
	mCamera = new Camera(this, QSize(600, 600), 100.0);
	this->widget = new Viewport(this, mScene, mCamera, machine);
	mData = new component{ this->widget, mScene, mCamera };
	setCentralWidget(this->widget);

	this->setSidebarWidget(this->widget);
	this->setUnderbarWidget(this->widget);

	sIDLE = new State("IDLE");
	sDLS = new DrawLineState("DRAW_LINE", mData);
	sDLS1 = new DrawLineState("DRAW_LINE", mData);
	sDLS2 = new DrawLineState("DRAW_LINE2", mData);
	sDFS = new DrawFaceState("DRAW_FACE", mData);
	sSPS = new SelectPointState("SELECT_POINT", mData);

	this->machine->addState(sIDLE);
	this->machine->addState(sDLS);
	this->machine->addState(sDLS1);
	this->machine->addState(sDLS2);
	this->machine->addState(sSPS);
	this->machine->printAllStates();
	this->machine->setState(sIDLE);
	this->machine->setState(sSPS);
	this->machine->getCurrentState();
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
			qDebug() << "Draw Line Clicked";
			this->machine->transition(sDLS);
			this->widget->updateState
			(
				this->machine->getCurrentState()
			);
		});
	connect(btnFace, &QPushButton::clicked, [this]()
		{
			qDebug() << "Draw Face Clicked";
			this->machine->transition(sDFS);
			this->widget->updateState
			(
				this->machine->getCurrentState()
			);
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
	QPushButton* btnPoint = new QPushButton("Point", wUnderbar);
	QPushButton* btnLine = new QPushButton("Line", wUnderbar);
	QPushButton* btnFace = new QPushButton("Face", wUnderbar);

	connect(btnPoint, &QPushButton::clicked, [this]()
		{
			qDebug() << "Point Clicked";
		});
	connect(btnLine, &QPushButton::clicked, [this]()
		{
			qDebug() << "Line Clicked";
		});
	connect(btnFace, &QPushButton::clicked, [this]()
		{
			qDebug() << "Face Clicked";
		});

	QHBoxLayout* hBox = new QHBoxLayout(wUnderbar);
	hBox->addWidget(btnPoint);
	hBox->addStretch();
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
