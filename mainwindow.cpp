#include "mainwindow.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QScrollArea>
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setMinimumSize(400,300);

    //Inizializzazione pagine
    paginaPrincipale = new QWidget(this);
    paginaNuovoMedia = new QWidget(this);

    //Pagina principale

    //Top Bar
    QHBoxLayout *layoutTopBar = new QHBoxLayout();

    QPushButton *button1 = new QPushButton("Bottone 1");
    QPushButton *button2 = new QPushButton("Bottone 2");
    QPushButton *toNuovoMedia = new QPushButton("Vai a Pagina 2");

    layoutTopBar->addWidget(button1);
    layoutTopBar->addWidget(button2);
    layoutTopBar->addWidget(toNuovoMedia);

    //Left Panel
    QVBoxLayout *layoutLeftPanel = new QVBoxLayout();

    QLineEdit *searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Cerca...");
    QLabel *leftLabel = new QLabel("Risultati:");
    QListWidget *listaMedia = new QListWidget();
    for (int i = 1; i <= 50; ++i) { //Esempio
        listaMedia->addItem("Media " + QString::number(i));
    }

    layoutLeftPanel->addWidget(searchBar);
    layoutLeftPanel->addWidget(leftLabel);
    layoutLeftPanel->addWidget(listaMedia);

    QWidget *leftPanel = new QWidget();
    leftPanel->setLayout(layoutLeftPanel);

    //Right Panel
    QWidget *rightPanel = new QWidget();
    rightPanel->setStyleSheet("background-color: white;");

    //Layout left e right panel
    QHBoxLayout *LayoutLeftRightPanel = new QHBoxLayout();
    LayoutLeftRightPanel->addWidget(leftPanel, 1);
    LayoutLeftRightPanel->addWidget(rightPanel, 3);

    //Layout pagina principale
    QVBoxLayout *layoutPaginaPrincipale = new QVBoxLayout();
    layoutPaginaPrincipale->addLayout(layoutTopBar);
    layoutPaginaPrincipale->addLayout(LayoutLeftRightPanel);
    paginaPrincipale->setLayout(layoutPaginaPrincipale);

    //Pagina nuovo media

    //Top Bar
    QHBoxLayout *topBar2Layout = new QHBoxLayout();
    QPushButton *button21 = new QPushButton("Bottone 1");
    QPushButton *button22 = new QPushButton("Bottone 2");
    QPushButton *toPrincipale = new QPushButton("Torna a Pagina 1");
    topBar2Layout->addWidget(button21);
    topBar2Layout->addWidget(button22);
    topBar2Layout->addWidget(toPrincipale);

    paginaNuovoMedia->setLayout(topBar2Layout);

    // Stack di pagine
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(paginaPrincipale);
    stackedWidget->addWidget(paginaNuovoMedia);
    stackedWidget->setCurrentWidget(paginaPrincipale);
    setCentralWidget(stackedWidget);

    // Connessioni
    connect(toPrincipale, &QPushButton::clicked, this, &MainWindow::showPaginaPrincipale);
    connect(toNuovoMedia, &QPushButton::clicked, this, &MainWindow::showPaginaNuovoMedia);
}

MainWindow::~MainWindow() {}

void MainWindow::showPaginaPrincipale() {
    stackedWidget->setCurrentWidget(paginaPrincipale);
}

void MainWindow::showPaginaNuovoMedia() {
    stackedWidget->setCurrentWidget(paginaNuovoMedia);
}
