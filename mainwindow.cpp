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
#include <QIcon>
#include <QFormLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

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
    for (int i = 1; i <= 10; ++i) { //Esempio
        QListWidgetItem *item = new QListWidgetItem(listaMedia);
        item->setSizeHint(QSize(200, 60)); // Altezza personalizzata

        QIcon icona("C:/Users/javie/Desktop/book.png"); // Sostituisci con il tuo percorso icona

        QString titolo = "Media " + QString::number(i);
        QString descrizione = "Descrizione per media " + QString::number(i);

        listaMedia->setItemWidget(item, widgetMedia(titolo, descrizione, icona));
    }

    layoutLeftPanel->addWidget(searchBar);
    layoutLeftPanel->addWidget(leftLabel);
    layoutLeftPanel->addWidget(listaMedia);

    QWidget *leftPanel = new QWidget();
    leftPanel->setLayout(layoutLeftPanel);

    //Right Panel
    QWidget *rightPanel = new QWidget();
    rightPanel->setStyleSheet("background-color: cyan;");

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
    topBar2Layout->setAlignment(Qt::AlignTop);

    // Layout principale della pagina
    QVBoxLayout *layoutNuovoMedia = new QVBoxLayout(paginaNuovoMedia);
    layoutNuovoMedia->addLayout(topBar2Layout);

    // Selettore tipo media
    QComboBox *comboTipoMedia = new QComboBox();
    comboTipoMedia->addItem("Libro");
    comboTipoMedia->addItem("Film");
    // Aggiungi gli altri 3 tipi
    layoutNuovoMedia->addWidget(new QLabel("Tipo di media:"));
    layoutNuovoMedia->addWidget(comboTipoMedia);

    // Form comune
    QFormLayout *formComune = new QFormLayout();
    QLineEdit *inputTitolo = new QLineEdit();
    QLineEdit *inputPrezzo = new QLineEdit();
    QLineEdit *inputData = new QLineEdit(); // oppure QDateEdit
    QLineEdit *inputGenere = new QLineEdit();
    QCheckBox *checkDisponibile = new QCheckBox("Disponibile");
    QSpinBox *inputCopie = new QSpinBox();
    inputCopie->setRange(0, 1000);

    formComune->addRow("Titolo:", inputTitolo);
    formComune->addRow("Prezzo:", inputPrezzo);
    formComune->addRow("Data pubblicazione:", inputData);
    formComune->addRow("Genere:", inputGenere);
    formComune->addRow("", checkDisponibile);
    formComune->addRow("Copie:", inputCopie);

    layoutNuovoMedia->addLayout(formComune);

    // Area campi specifici
    QStackedWidget *stackSpecifici = new QStackedWidget();
    layoutNuovoMedia->addWidget(stackSpecifici);

    // Widget per "Libro"
    QWidget *widgetLibro = new QWidget();
    QFormLayout *layoutLibro = new QFormLayout(widgetLibro);
    QLineEdit *inputRegista = new QLineEdit();
    QLineEdit *inputLingua = new QLineEdit();
    QLineEdit *inputPaese = new QLineEdit();
    layoutLibro->addRow("Regista:", inputRegista);
    layoutLibro->addRow("Lingua originale:", inputLingua);
    layoutLibro->addRow("Paese di produzione:", inputPaese);

    // Widget per "Film" (esempio con campi diversi, anche se identici per ora)
    QWidget *widgetFilm = new QWidget();
    QFormLayout *layoutFilm = new QFormLayout(widgetFilm);
    layoutFilm->addRow("Regista:", new QLineEdit());
    layoutFilm->addRow("Studio:", new QLineEdit());

    stackSpecifici->addWidget(widgetLibro); // index 0
    stackSpecifici->addWidget(widgetFilm);  // index 1

    // Cambia la sezione specifica quando si cambia tipo media
    QObject::connect(comboTipoMedia, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [stackSpecifici](int index){
                         stackSpecifici->setCurrentIndex(index);
                     });

    // Bottone Salva in fondo
    QPushButton *salvaButton = new QPushButton("Salva");
    layoutNuovoMedia->addWidget(salvaButton);

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

QWidget* MainWindow::widgetMedia(const QString& titolo, const QString& descrizione, const QIcon& icona) {
    // Widget base
    QWidget *widget = new QWidget();

    // Icona a sinistra
    QLabel *labelIcona = new QLabel();
    labelIcona->setPixmap(icona.pixmap(48, 48));  // Dimensione dell'icona

    // Titolo e descrizione a destra
    QLabel *labelTitolo = new QLabel(titolo);
    labelTitolo->setStyleSheet("font-weight: bold;");
    QLabel *labelDescrizione = new QLabel(descrizione);
    labelDescrizione->setStyleSheet("color: gray;");

    // Layout verticale per titolo + descrizione
    QVBoxLayout *layoutTesto = new QVBoxLayout();
    layoutTesto->addWidget(labelTitolo);
    layoutTesto->addWidget(labelDescrizione);

    // Layout orizzontale: icona a sinistra, testo a destra
    QHBoxLayout *layoutPrincipale = new QHBoxLayout(widget);
    layoutPrincipale->addWidget(labelIcona, 0);  // forza icona a sinistra
    layoutPrincipale->addLayout(layoutTesto, 1);                // layout testo si espande
    layoutPrincipale->setSpacing(10);
    layoutPrincipale->setContentsMargins(5, 5, 5, 5);
    layoutPrincipale->setAlignment(Qt::AlignLeft); // forza l'intero layout a sinistra

    return widget;
}
