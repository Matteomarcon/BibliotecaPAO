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

    // Creazione della pagina principale (central widget)
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Creazione del layout principale
    mainLayout = new QVBoxLayout();
    centralWidget->setLayout(mainLayout);

    // Creazione del layout per la barra dei bottoni
    buttonBarLayout = new QHBoxLayout();
    mainLayout->addLayout(buttonBarLayout);

    // Creazione del layout per il corpo centrale
    centralBodyLayout = new QHBoxLayout();
    mainLayout->addLayout(centralBodyLayout);


    // Creazione del pannello di sinistra e relativo layout
    leftPanel = new QWidget();
    leftPanelLayout = new QVBoxLayout();  // Creazione separata del layout
    leftPanel->setLayout(leftPanelLayout);  // Assegnazione del layout al pannello


    //Buttons Bar
    button1 = new QPushButton("Bottone 1");
    button2 = new QPushButton("Bottone 2");
    buttonBarLayout->addWidget(button1);
    buttonBarLayout->addWidget(button2);

    //Search Bar
    QLineEdit *searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Cerca...");
    leftPanelLayout->addWidget(searchBar);

    //Info Label
    QLabel *infoLabel = new QLabel("Testo iniziale");
    leftPanelLayout->addWidget(infoLabel);

    //Scroll Panel
    QListWidget *listaLibri = new QListWidget();
    leftPanelLayout->addWidget(listaLibri);

    //Right Panel
    rightPanel = new QLabel("Pannello destro");
    rightPanel->setStyleSheet("background-color: lightgray; padding: 20px;");

    //Unione Left e Right Panel
    centralBodyLayout->addWidget(leftPanel, 1);
    centralBodyLayout->addWidget(rightPanel, 3);

    //Prova
    libro *libro1 = new libro("a", "n", "c", "d");

    QListWidgetItem *item = new QListWidgetItem();

    QString itemText = "Titolo: " + libro1->getTitolo() + "\n"
                       + "Autore: " + libro1->getAutore() + "\n"
                       + "Editore: " + libro1->getEditore() + "\n"
                       + "Descrizione: " + libro1->getDescrizione();
    item->setText(itemText);
    listaLibri->addItem(item);

    connect(listaLibri, &QListWidget::itemClicked, this, &MainWindow::onItemClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::onItemClicked(QListWidgetItem *item)
{
    // Quando un item è cliccato, aggiorna il pannello a destra con i dettagli del libro
    if (item) {
        // Ottieni il titolo del libro selezionato
        QString titolo = item->text();

        // Controlla quale libro è stato selezionato
        QString details = "ciao";
        rightPanel->setText(details);

    }
}
