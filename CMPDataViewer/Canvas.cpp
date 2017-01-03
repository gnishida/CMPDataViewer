#include "Canvas.h"
#include <QPainter>
#include <iostream>
#include <QFileInfoList>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QDomDocument>

Canvas::Canvas(QWidget *parent) : QWidget(parent) {
	ctrlPressed = false;
	shiftPressed = false;
}

Canvas::~Canvas() {
}

void Canvas::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.drawImage(0, 0, image);

	painter.setPen(QPen(QColor(0, 0, 255), 3));
	for (int i = 0; i < params["facade"].size(); ++i) {
		painter.drawRect(params["facade"][i].x, params["facade"][i].y, params["facade"][i].width, params["facade"][i].height);
	}

	painter.setPen(QPen(QColor(255, 255, 0), 3));
	for (int i = 0; i < params["window"].size(); ++i) {
		painter.drawRect(params["window"][i].x, params["window"][i].y, params["window"][i].width, params["window"][i].height);
	}
	painter.setPen(QPen(QColor(0, 255, 0), 3));
	for (int i = 0; i < params["door"].size(); ++i) {
		painter.drawRect(params["door"][i].x, params["door"][i].y, params["door"][i].width, params["door"][i].height);
	}
	painter.setPen(QPen(QColor(0, 255, 255), 3));
	for (int i = 0; i < params["shop"].size(); ++i) {
		painter.drawRect(params["shop"][i].x, params["shop"][i].y, params["shop"][i].width, params["shop"][i].height);
	}
	painter.setPen(QPen(QColor(255, 0, 0), 3));
	for (int i = 0; i < params["sill"].size(); ++i) {
		painter.drawRect(params["sill"][i].x, params["sill"][i].y, params["sill"][i].width, params["sill"][i].height);
	}
	painter.setPen(QPen(QColor(0, 255, 255), 3));
	for (int i = 0; i < params["cornice"].size(); ++i) {
		painter.drawRect(params["cornice"][i].x, params["cornice"][i].y, params["cornice"][i].width, params["cornice"][i].height);
	}

}

void Canvas::mousePressEvent(QMouseEvent* e) {
	update();
}

void Canvas::load(const QString& basename) {
	image = QImage(basename + ".jpg");
	int max_size = std::max(image.width(), image.height());
	float scale = 800.0f / max_size;
	image = image.scaled(image.width() * scale, image.height() * scale);

	// load annotation
	loadAnnotation(basename + ".xml", image.width(), image.height());
}

void Canvas::loadAnnotation(const QString& filename, int width, int height) {
	params.clear();

	QFile file(filename);

	QDomDocument doc;
	doc.setContent(&file, true);
	QDomElement root = doc.documentElement();
	QDomNode object_node = root.firstChild();
	while (!object_node.isNull()) {
		if (object_node.toElement().tagName() == "object") {
			int label;
			QString label_name;
			std::vector<float> xs;
			std::vector<float> ys;

			QDomNode data_node = object_node.firstChild();
			while (!data_node.isNull()) {
				if (data_node.toElement().tagName() == "points") {
					QDomNode coord_node = data_node.firstChild();
					while (!coord_node.isNull()) {
						if (coord_node.toElement().tagName() == "x") {
							ys.push_back(getNodeValue(coord_node).toFloat() * height);
						}
						else if (coord_node.toElement().tagName() == "y") {
							xs.push_back(getNodeValue(coord_node).toFloat() * width);
						}

						coord_node = coord_node.nextSibling();
					}
				}
				else if (data_node.toElement().tagName() == "label") {
					label = getNodeValue(data_node).toInt();
				}
				else if (data_node.toElement().tagName() == "labelname") {
					label_name = getNodeValue(data_node);
				}

				data_node = data_node.nextSibling();
			}

			if (!params.contains(label_name)) {
				params[label_name] = std::vector<cv::Rect>();
			}
			params[label_name].push_back(cv::Rect(xs[0], ys[0], xs[1] - xs[0], ys[1] - ys[0]));


		}

		object_node = object_node.nextSibling();
	}

}

QString Canvas::getNodeValue(const QDomNode& node) {
	QDomNode text_node = node.firstChild();
	while (!text_node.isNull()) {
		if (text_node.nodeType() == QDomNode::TextNode) {
			return text_node.toText().data().trimmed();
		}

		text_node = text_node.nextSibling();
	}

	return "";
}

void Canvas::keyPressEvent(QKeyEvent* e) {
	ctrlPressed = false;
	ctrlPressed = false;

	if (e->modifiers() & Qt::ControlModifier) {
		ctrlPressed = true;
	}
	if (e->modifiers() & Qt::ShiftModifier) {
		shiftPressed = true;
	}

	switch (e->key()) {
	case Qt::Key_Space:
		break;
	}

	update();
}

void Canvas::keyReleaseEvent(QKeyEvent* e) {
	switch (e->key()) {
	case Qt::Key_Control:
		ctrlPressed = false;
		break;
	case Qt::Key_Shift:
		shiftPressed = false;
		break;
	default:
		break;
	}
}
