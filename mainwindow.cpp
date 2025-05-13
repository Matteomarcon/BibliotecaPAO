#include "mainwindow.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    //Pagina principale
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    //main Layout
    mainLayout = new QVBoxLayout();

    //buttons bar
    buttonBarLayout = new QHBoxLayout();

    button1 = new QPushButton("Bottone 1");
    button2 = new QPushButton("Bottone 2");
    buttonBarLayout->addWidget(button1);
    buttonBarLayout->addWidget(button2);

    // --- Corpo centrale (due pannelli affiancati)
    centralBodyLayout = new QHBoxLayout();
    leftPanel = new QLabel("Pannello sinistro");
    rightPanel = new QLabel("Pannello destro");

    // (Opzionale) stile visivo per i pannelli
    leftPanel->setStyleSheet("background-color: lightblue; padding: 20px;");
    rightPanel->setStyleSheet("background-color: lightgray; padding: 20px;");

    centralBodyLayout->addWidget(leftPanel, 1);
    centralBodyLayout->addWidget(rightPanel, 3);

    // Assembla tutto nel layout principale
    mainLayout->addLayout(buttonBarLayout);
    mainLayout->addLayout(centralBodyLayout);

    // Imposta il layout nel central widget
    centralWidget->setLayout(mainLayout);
}

MainWindow::~MainWindow() {}
