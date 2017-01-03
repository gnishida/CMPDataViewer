#include "MainWindow.h"
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	canvas = new Canvas(this);
	setCentralWidget(canvas);

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionFixXML, SIGNAL(triggered()), this, SLOT(onFixXML()));
}

MainWindow::~MainWindow() {
}

void MainWindow::onOpen() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Image file..."), "", tr("Image Files (*.jpg)"));
	if (filename.isEmpty()) return;

	int index = filename.lastIndexOf(".");
	if (index < 0) return;

	QString basename = filename.mid(0, index);

	canvas->load(basename);

	this->setWindowTitle("CMP Data Viewer - " + filename);
}

void MainWindow::onFixXML() {
	QStringList filters;
	filters << "*.xml";

	QFileInfoList fileInfoList = QDir("cmp").entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

	printf("Processing");
	for (int i = 0; i < fileInfoList.size(); ++i) {
		printf("\rProcessing %s    ", fileInfoList[i].fileName().toUtf8().constData());

		QFile file(fileInfoList[i].absoluteFilePath());
		file.open(QIODevice::ReadOnly);

		QString file_content;
		file_content = "<objects>\n";

		QTextStream in(&file);
		while (!in.atEnd()) {
			QString line = in.readLine();
			file_content += line + "\n";
		}
		file.close();

		file_content += "</objects>\n";


		file.open(QIODevice::WriteOnly);
		QTextStream out(&file);
		out << file_content;
		file.close();
	}
	printf("\n");
	printf("Done.");
}