#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include "AsterUI/AsterUI.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qDebug() << "AsterUI Version:" << AsterUI::version();

    QMainWindow window;
    window.setWindowTitle("AsterUI Demo");
    window.resize(800, 600);

    QWidget *centralWidget = new QWidget(&window);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    QLabel *label = new QLabel("Welcome to AsterUI Demo", centralWidget);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    window.setCentralWidget(centralWidget);
    window.show();

    return app.exec();
}
