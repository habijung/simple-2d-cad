#include <QPushButton>
#include <QBoxLayout>
#include <QFileDialog>
#include "MainWindow.h"


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);
	setMenuBar();
	setToolbar();

	// Create main Viewport
	mViewport = new Viewport(this);
	setCentralWidget(mViewport);
	setSidebarWidget(mViewport);
	setUnderbarWidget(mViewport);
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
			mViewport->createNewScene();
		});
	connect(open, &QAction::triggered, qApp, [this]()
		{
			mViewport->loadScene();
		});
	connect(save, &QAction::triggered, qApp, [this]()
		{
			mViewport->saveScene();
		});
}

void MainWindow::setSidebarWidget(QWidget* widget)
{
	QWidget* wSidebar = new QWidget(widget);
	QPushButton* btnLine = new QPushButton("Line", wSidebar);
	QPushButton* btnFace = new QPushButton("Poly", wSidebar);

	connect(btnLine, &QPushButton::clicked, [this]()
		{
			mViewport->updateState("DRAW_LINE");
		});
	connect(btnFace, &QPushButton::clicked, [this]()
		{
			mViewport->updateState("DRAW_FACE");
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
			mViewport->updateState("SELECT_POINT");
		});
	connect(btnLine, &QPushButton::clicked, [this]()
		{
			mViewport->updateState("SELECT_LINE");
		});
	connect(btnFace, &QPushButton::clicked, [this]()
		{
			mViewport->updateState("SELECT_FACE");
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
