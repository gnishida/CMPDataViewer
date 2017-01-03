#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QKeyEvent>
#include <opencv2/opencv.hpp>
#include <QDomNode>

class Canvas : public QWidget {
private:
	QImage image;
	QMap<QString, std::vector<cv::Rect>> params;

	bool ctrlPressed;
	bool shiftPressed;
	
public:
	Canvas(QWidget *parent = NULL);
    ~Canvas();
	
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent* e);
    
public:
	void load(const QString& basename);
	void Canvas::loadAnnotation(const QString& filename, int width, int height);
	QString getNodeValue(const QDomNode& node);
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
};

#endif // CANVAS_H
