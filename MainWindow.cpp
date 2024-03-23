#include <QPushButton>
#include <QBoxLayout>
#include <QFileDialog>
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
	mDrawPolygonState = new DrawPolygonState("DRAW_POLY", mData);
	mSelectPointState = new SelectPointState("SELECT_POINT", mData);
	mSelectLineState = new SelectLineState("SELECT_LINE", mData);
	mSelectPolygonState = new SelectPolygonState("SELECT_POLYGON", mData);

	machine->addState(mDrawLineState);
	machine->addState(mDrawPolygonState);
	machine->addState(mSelectPointState);
	machine->addState(mSelectLineState);
	machine->addState(mSelectPolygonState);
	machine->setState(mSelectPointState);
	machine->getCurrentState();

	// TODO: 이 부분이 없으면 State에서 nullptr로 들어가는 이유 확인하고 중복되는 곳 병합하기
	widget->updateState(machine->getCurrentState(), mScene);
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
	/* Toolbar */
	QToolBar* toolbar = addToolBar("main toolbar");
	QAction* newa = toolbar->addAction(QIcon(QPixmap("img/new.png")), "New File");
	QAction* open = toolbar->addAction(QIcon(QPixmap("img/open.png")), "Open File");
	QAction* save = toolbar->addAction(QIcon(QPixmap("img/save.png")), "Save File");
	toolbar->addSeparator();

	QAction* quit = toolbar->addAction(QIcon(QPixmap("img/quit.png")), "Quit Application");

	connect(quit, &QAction::triggered, qApp, &QApplication::quit);
	connect(newa, &QAction::triggered, qApp, [this]()
		{
			mScene = widget->createNewScene(mScene);
		});
	connect(open, &QAction::triggered, qApp, [this]()
		{
			QFileDialog* fd = new QFileDialog;
			fd->setFileMode(QFileDialog::AnyFile);
			fd->setOptions(QFileDialog::ShowDirsOnly);
			fd->setViewMode(QFileDialog::Detail);
			int result = fd->exec();
			if (result)
			{
				QString filePath = fd->selectedFiles()[0];
				//ui->filePathEdit->setText(filePath);
			}
		});
	connect(save, &QAction::triggered, qApp, [this]()
		{
			widget->saveScene();
		});
}

void MainWindow::setSidebarWidget(QWidget* widget)
{
	QWidget* wSidebar = new QWidget(widget);
	QPushButton* btnLine = new QPushButton("Line", wSidebar);
	QPushButton* btnFace = new QPushButton("Poly", wSidebar);

	connect(btnLine, &QPushButton::clicked, [this]()
		{
			qDebug() << "Draw Line Clicked";
			this->machine->transition(mDrawLineState);
			this->widget->updateState(machine->getCurrentState(), mScene);
		});
	connect(btnFace, &QPushButton::clicked, [this]()
		{
			qDebug() << "Draw Poly Clicked";
			this->machine->transition(mDrawPolygonState);
			this->widget->updateState(machine->getCurrentState(), mScene);
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
	QPushButton* btnFace = new QPushButton("Poly", wUnderbar);

	connect(btnPoint, &QPushButton::clicked, [this]()
		{
			qDebug() << "Select Point Clicked";
			machine->transition(mSelectPointState);
			this->widget->updateState(machine->getCurrentState(), mScene);
		});
	connect(btnLine, &QPushButton::clicked, [this]()
		{
			qDebug() << "Select Line Clicked";
			machine->transition(mSelectLineState);
			this->widget->updateState(machine->getCurrentState(), mScene);
		});
	connect(btnFace, &QPushButton::clicked, [this]()
		{
			qDebug() << "Select Poly Clicked";
			machine->transition(mSelectPolygonState);
			this->widget->updateState(machine->getCurrentState(), mScene);
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
	switch (event->key())
	{
	case Qt::Key_QuoteLeft:
		machine->setState(mSelectPointState);
		this->widget->updateState(this->machine->getCurrentState(), mScene);

		// TODO: State refacroting 하면서 자동으로 초기화가 되도록 만들기
		mDrawLineState = new DrawLineState("DRAW_LINE", mData);
		mDrawPolygonState = new DrawPolygonState("DRAW_POLY", mData);
		break;
	}

	this->widget->getKeyEvent(event);
}
