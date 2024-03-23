#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegularExpression>
#include "Viewport.h"


Viewport::Viewport(QWidget* parent) : QWidget(parent)
{
	// Set Viewport options
	setAutoFillBackground(true);
	setPalette(QPalette(QPalette::Window, Qt::white));
	setMouseTracking(true);

	// Initialization
	mButton = Qt::NoButton;
	mCamera = new Camera(parent->size(), 80.0);
	mScene = new Scene(mCamera);
	mScene->createSampleShapes();

	// Define States
	mData = new SelectUtils::ViewportData{ this, mScene, mCamera };
	mMachine = new StateMachine;
	mMachine->addState(new SelectPointState("SELECT_POINT", mData));
	mMachine->addState(new SelectLineState("SELECT_LINE", mData));
	mMachine->addState(new SelectFaceState("SELECT_FACE", mData));
	mMachine->addState(new DrawLineState("DRAW_LINE", mData));
	mMachine->addState(new DrawFaceState("DRAW_FACE", mData));
	mMachine->setState("SELECT_POINT");
}

Viewport::~Viewport()
{
	delete mScene;
	delete mCamera;
}

// Update Scene
void Viewport::updateState(std::string name)
{
	if (!name.compare("DRAW_LINE"))
	{
		mMachine->deleteState(name);
		delete dynamic_cast<DrawLineState*>(mMachine->getState(name));
		mMachine->addState(new DrawLineState("DRAW_LINE", mData));
	}
	else if (!name.compare("DRAW_FACE"))
	{
		mMachine->deleteState(name);
		delete dynamic_cast<DrawFaceState*>(mMachine->getState(name));
		mMachine->addState(new DrawFaceState("DRAW_FACE", mData));
	}

	mMachine->transition(name, mData);
}

void Viewport::saveScene()
{
	QJsonObject content;
	std::list<Shape*> shapes = mScene->retShapes();
	int numShape = 0; // Check priority
	int numLine = 0;
	int numFace = 0;

	for (auto iter = shapes.begin(); iter != shapes.end(); iter++)
	{
		size_t n = 3; // Max precision
		std::string s = std::to_string(numShape);
		int precision = n - std::min(n, s.size());
		std::string name = "Shape-" + std::string(precision, '0').append(s);
		QJsonObject obj;

		if ((*iter)->checkType("Line"))
		{
			obj.insert("Type", (*iter)->type().c_str());
			obj.insert("Num", numLine);
			obj.insert("Vertices", dynamic_cast<Line*>(*iter)->saveLine());
			content.insert(name.c_str(), obj);
			numLine++;
			numShape++;
		}

		if ((*iter)->checkType("Face"))
		{
			obj.insert("Type", (*iter)->type().c_str());
			obj.insert("Num", numFace);
			obj.insert("Vertices", dynamic_cast<Face*>(*iter)->saveFace());
			content.insert(name.c_str(), obj);
			numFace++;
			numShape++;
		}
	}

	QJsonDocument document;
	document.setObject(content);

	QString dirPath = QString(getenv("USERPROFILE")) + "/Desktop/Scene";
	QString filePath = QFileDialog::getSaveFileName(
		this,
		tr("Save Scene"),
		dirPath,
		tr("JSON (*.json);; Image (*.png *jpg);; Text (*.txt);; XML (*.xml)")
	);
	QFile file(filePath);

	if (file.open(QIODevice::WriteOnly))
	{
		file.write(document.toJson(QJsonDocument::Indented));
		file.close();
	}
	else
	{
		qDebug() << "QFile open failed: " << filePath.toStdString();
	}
}

void Viewport::loadScene()
{
	QString dirPath = QString(getenv("USERPROFILE")) + "/Desktop/";
	QString filePath = QFileDialog::getOpenFileName(
		this,
		tr("Open Scene"),
		dirPath,
		tr("JSON (*.json);; Image (*.png *jpg);; Text (*.txt);; XML (*.xml)")
	);
	QFile file(filePath);

	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray bytes = file.readAll();
		file.close();

		QJsonParseError jsonError;
		QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);

		if (jsonError.error != QJsonParseError::NoError)
		{
			qDebug() << "fromJson failed: " << jsonError.errorString().toStdString();
			return;
		}

		if (document.isObject())
		{
			createNewScene();
			std::list<Shape*> shapes = mScene->retShapes();
			QJsonObject jsonScene = document.object(); // Scene.json file
			QJsonObject::iterator is = jsonScene.begin();

			for (is; is != jsonScene.end(); is++)
			{
				QJsonObject jsonShape = (*is).toObject();
				QStringList keys = jsonShape.keys();
				QStringList::iterator key = keys.begin();

				for (key; key != keys.end(); key++)
				{
					QJsonValue value = jsonShape.take(*key);

					if (value.isString())
					{
						QString shapeType = value.toString();

						if (!shapeType.compare("Line") || !shapeType.compare("Face"))
						{
							key++;
							QJsonObject vertices = jsonShape.take(*key).toObject();
							QJsonObject::iterator vertex = vertices.begin();
							std::list<Vertex*> lVertices;

							// Separate and Trim JSON Vertex data
							// https://stackoverflow.com/questions/42545597/how-to-match-spaces-at-the-strings-beginning-using-qregularexpression-in-qt
							QRegularExpression separator("[,\\s]+");

							for (vertex; vertex != vertices.end(); vertex++)
							{
								QStringList vData = (*vertex).toString().trimmed().split(separator);
								Vertex* v = new Vertex(QPointF(vData.front().toFloat(), vData.back().toFloat()));
								shapes.push_back(v);
								lVertices.push_back(v);
							}

							if (!shapeType.compare("Line"))
							{
								Line* l = new Line(lVertices);
								shapes.push_back(l);
							}
							else
							{
								Face* f = new Face(lVertices);
								shapes.push_back(f);
							}

							mScene->updateShapes(shapes);
							update();
						}
					}
				}
			}
		}
	}
}

void Viewport::createNewScene()
{
	// TODO: Create new scene 할 때는 sample 그리고, Load scene 할 때는 sample 그리지 않기
	delete mScene;
	mScene = new Scene(mCamera);
	mData->scene = mScene;
	mMachine->state()->updateScene(mScene);
	mCamera->reset(size());
	update();
}

void Viewport::paintEvent(QPaintEvent* event)
{
	QPainter* painter = new QPainter(this);

	painter->save();
	mScene->renderCoordinate(painter);
	mScene->renderShape(painter);
	painter->restore();

	painter->save();
	mMachine->state()->paintEvent(painter);
	painter->restore();

	painter->end();
}

void Viewport::mousePressEvent(QMouseEvent* event)
{
	mPos = event->pos();
	mButton = event->button();
	mMachine->state()->mousePressEvent(event);
}

void Viewport::mouseMoveEvent(QMouseEvent* event)
{
	if ((mButton == Qt::RightButton) && (event->button() == Qt::NoButton))
	{
		float sensitivity = 80.0;
		QPointF offset = (event->pos() - mPos) / sensitivity;
		mCamera->pan(-offset.x(), offset.y());
		mPos = event->pos();
	}

	mMachine->state()->mouseMoveEvent(event);
}

void Viewport::mouseReleaseEvent(QMouseEvent* event)
{
	mButton = Qt::NoButton;
	mMachine->state()->mouseReleaseEvent(event);
}

void Viewport::wheelEvent(QWheelEvent* event)
{
	mCamera->zoom(static_cast<float>(event->angleDelta().y() / 8));
	update();
}

void Viewport::keyPressEvent(QKeyEvent* event)
{
	float interval = 1.0;
	float scale = 60.0;

	switch (event->key())
	{
	case Qt::Key_E: // Zoom In
		mCamera->zoom(scale);
		break;
	case Qt::Key_Q: // Zoom Out
		mCamera->zoom(-scale);
		break;
	case Qt::Key_W: // Move Up
		mCamera->pan(0, interval);
		break;
	case Qt::Key_S: // Move Down
		mCamera->pan(0, -interval);
		break;
	case Qt::Key_D: // Move Right
		mCamera->pan(interval, 0);
		break;
	case Qt::Key_A: // Move Left
		mCamera->pan(-interval, 0);
		break;
	case Qt::Key_QuoteLeft:
		updateState("SELECT_POINT");
		break;
	}

	update();
}

void Viewport::resizeEvent(QResizeEvent* event)
{
	// TODO: Window 조절할 때 Camera Scale 조정하기
	//mCamera->resizeEvent(event);
}
