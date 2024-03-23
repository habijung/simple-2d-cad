#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegularExpression>
#include "Viewport.h"


Viewport::Viewport(QWidget* parent)
	: QWidget(parent)
{
	mCamera = new Camera(this, parent->size(), 100.0);
	mScene = new Scene(mCamera);
	pValue = 80.0;
	zValue = 60.0;
	setMouseTracking(true);

	// Define States
	mComp = new component{ this, mScene, mCamera };
	mMachine = new StateMachine;
	mMachine->addState(new SelectPointState("SELECT_POINT", mComp));
	mMachine->addState(new SelectLineState("SELECT_LINE", mComp));
	mMachine->addState(new SelectFaceState("SELECT_FACE", mComp));
	mMachine->addState(new DrawLineState("DRAW_LINE", mComp));
	mMachine->addState(new DrawFaceState("DRAW_FACE", mComp));
	mMachine->setState("SELECT_POINT");
}

void Viewport::updateState(string name)
{
	if (!name.compare("DRAW_LINE"))
	{
		mMachine->deleteState(name);
		delete dynamic_cast<DrawLineState*>(mMachine->getState(name));
		mMachine->addState(new DrawLineState("DRAW_LINE", mComp));
	}
	else if (!name.compare("DRAW_FACE"))
	{
		mMachine->deleteState(name);
		delete dynamic_cast<DrawFaceState*>(mMachine->getState(name));
		mMachine->addState(new DrawFaceState("DRAW_FACE", mComp));
	}

	mMachine->transition(name, mComp);
}

void Viewport::saveScene()
{
	QJsonObject content;
	list<Shape*> shapes = mScene->retShapes();
	int numShape = 0; // Check priority
	int numLine = 0;
	int numFace = 0;

	for (auto iter = shapes.begin(); iter != shapes.end(); iter++)
	{
		size_t n = 3; // Max precision
		string s = to_string(numShape);
		int precision = n - min(n, s.size());
		string name = "Shape-" + string(precision, '0').append(s);
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
			list<Shape*> shapes = mScene->retShapes();
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
							list<Vertex*> lVertices;

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
	delete mScene;
	mScene = new Scene(mCamera);
	mComp->scene = mScene;
	mMachine->state()->updateScene(mScene);
	mCamera->reset(size());
	update();
}

void Viewport::paintEvent(QPaintEvent* event)
{
	QPainter* painter = new QPainter(this);

	painter->save();
	mScene->renderScreenCoordinate(painter);
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
	button = event->button();
	mMachine->state()->mousePressEvent(event);
}

void Viewport::mouseMoveEvent(QMouseEvent* event)
{
	if ((button == Qt::RightButton) && (event->button() == Qt::NoButton))
	{
		QPointF offset = event->pos() - mPos;
		mCamera->pan(-offset.x(), offset.y());
		mPos = event->pos();
	}

	mMachine->state()->mouseMoveEvent(event);
}

void Viewport::mouseReleaseEvent(QMouseEvent* event)
{
	button = Qt::NoButton;
	mMachine->state()->mouseReleaseEvent(event);
}

void Viewport::wheelEvent(QWheelEvent* event)
{
	mCamera->zoom(static_cast<float>(event->angleDelta().y() / 8));
	update();
}

void Viewport::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_E: // Zoom In
		mCamera->zoom(zValue);
		break;
	case Qt::Key_Q: // Zoom Out
		mCamera->zoom(-zValue);
		break;
	case Qt::Key_W: // Move Up
		mCamera->pan(0.0, pValue);
		break;
	case Qt::Key_S: // Move Down
		mCamera->pan(0.0, -pValue);
		break;
	case Qt::Key_D: // Move Right
		mCamera->pan(pValue, 0.0);
		break;
	case Qt::Key_A: // Move Left
		mCamera->pan(-pValue, 0.0);
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
