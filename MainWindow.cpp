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
	mCamera = new Camera(this, QSize(600, 600), 100.0);
	mScene = new Scene(this, mCamera);
	this->widget = new Viewport(this, mScene, mCamera, machine);
	mData = new component{ this->widget, mScene, mCamera };
	setCentralWidget(this->widget);

	this->setSidebarWidget(this->widget);
	this->setUnderbarWidget(this->widget);

	component* comp = new component;
	mDrawLineState = new DrawLineState("DRAW_LINE", mData);
	mDrawFaceState = new DrawFaceState("DRAW_FACE", mData);
	mSelectPointState = new SelectPointState("SELECT_POINT", mData);

	machine->addState(mDrawLineState);
	machine->addState(mSelectPointState);
	machine->setState(mSelectPointState);
	machine->getCurrentState();

	// TODO: 이 부분이 없으면 State에서 nullptr로 들어가는 이유 확인하고 중복되는 곳 병합하기
	widget->updateState(machine->getCurrentState());
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
			this->machine->transition(mDrawLineState);
			this->widget->updateState
			(
				this->machine->getCurrentState()
			);
		});
	connect(btnFace, &QPushButton::clicked, [this]()
		{
			qDebug() << "Draw Face Clicked";
			this->machine->transition(mDrawFaceState);
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
			machine->transition(mSelectPointState);
			this->widget->updateState(machine->getCurrentState());
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
