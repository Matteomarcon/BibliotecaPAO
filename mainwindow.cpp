#include "mainwindow.h"
#include "gestoremedia.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QScrollArea>
#include <QListWidget>
#include <QIcon>
#include <QFormLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setMinimumSize(400,300);

    //Inizializzazione pagine
    paginaPrincipale = new QWidget(this);
    paginaNuovoMedia = new QWidget(this);

    //Pagina principale

    //Top Bar
    QHBoxLayout *layoutTopBar = new QHBoxLayout();

    QPushButton *nuovaBibiotecaButton = new QPushButton("  Apri Biblioteca");
    QPushButton *toNuovoMedia = new QPushButton("Nuovo Media");
    nuovaBibiotecaButton->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent;"
        "  border: none;"
        "}"
        );
    toNuovoMedia->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent;"
        "  border: none;"
        "}"
        );

    nuovaBibiotecaButton->setIconSize(QSize(36, 36));
    toNuovoMedia->setIconSize(QSize(48, 48));

    nuovaBibiotecaButton->setIcon(QIcon(":/icone/folder.png"));
    toNuovoMedia->setIcon(QIcon(":/icone/add.png"));

    layoutTopBar->addWidget(nuovaBibiotecaButton);
    layoutTopBar->addWidget(toNuovoMedia);

    //Left Panel
    QVBoxLayout *layoutLeftPanel = new QVBoxLayout();
    layoutLeftPanel->addLayout(layoutTopBar);

    QLineEdit *searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Cerca...");
    QLabel *leftLabel = new QLabel("Risultati:");
    QListWidget *listaMedia = new QListWidget();

    GestoreMedia gestore(listaMedia, "dati.json");
    gestore.caricaBiblioteca();

    connect(listaMedia, &QListWidget::itemClicked, this, &MainWindow::onItemClicked);

    layoutLeftPanel->addWidget(searchBar);
    layoutLeftPanel->addWidget(leftLabel);
    layoutLeftPanel->addWidget(listaMedia);

    QWidget *leftPanel = new QWidget();
    leftPanel->setLayout(layoutLeftPanel);

    //Right Panel
    rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    labelTitolo = new QLabel("Titolo:", rightPanel);
    labelDescrizione = new QLabel("Descrizione:", rightPanel);
    rightLayout->addWidget(labelTitolo);
    rightLayout->addWidget(labelDescrizione);
    rightPanel->setLayout(rightLayout);

    //Layout left e right panel
    QHBoxLayout *LayoutLeftRightPanel = new QHBoxLayout();
    LayoutLeftRightPanel->addWidget(leftPanel, 1);
    LayoutLeftRightPanel->addWidget(rightPanel, 3);

    //Layout pagina principale
    QVBoxLayout *layoutPaginaPrincipale = new QVBoxLayout();
    layoutPaginaPrincipale->addLayout(LayoutLeftRightPanel);
    paginaPrincipale->setLayout(layoutPaginaPrincipale);

    // Pagina nuovo media

    // Top Bar
    QHBoxLayout *topBar2Layout = new QHBoxLayout();
    QPushButton *button21 = new QPushButton("Bottone 1");
    QPushButton *button22 = new QPushButton("Bottone 2");
    QPushButton *toPrincipale = new QPushButton("Torna a Pagina 1");
    topBar2Layout->addWidget(button21);
    topBar2Layout->addWidget(button22);
    topBar2Layout->addWidget(toPrincipale);
    topBar2Layout->setAlignment(Qt::AlignTop);

    // Layout principale
    QVBoxLayout *layoutNuovoMedia = new QVBoxLayout(paginaNuovoMedia);
    layoutNuovoMedia->addLayout(topBar2Layout);

    //Combo box per selezione tipo
    QComboBox *selettoreMedia = new QComboBox();
    selettoreMedia->addItems(gestore.getTipiDisponibili());
    layoutNuovoMedia->addWidget(selettoreMedia);

    // Stack dei form
    QStackedWidget *formStack = new QStackedWidget();
    layoutNuovoMedia->addWidget(formStack);

    // Pulsante salva
    QPushButton *saveButton = new QPushButton("Salva");
    layoutNuovoMedia->addWidget(saveButton);


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

void MainWindow::onItemClicked(QListWidgetItem *item) {
    if (item) {
        labelTitolo->setText(item->text());
    }
}
