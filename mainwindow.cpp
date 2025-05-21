#include "mainwindow.h"
#include "vinile.h"
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

    nuovaBibiotecaButton->setIcon(QIcon("C:/Users/javie/Desktop/folder.png"));
    toNuovoMedia->setIcon(QIcon("C:/Users/javie/Desktop/add.png"));

    layoutTopBar->addWidget(nuovaBibiotecaButton);
    layoutTopBar->addWidget(toNuovoMedia);

    //Left Panel
    QVBoxLayout *layoutLeftPanel = new QVBoxLayout();
    layoutLeftPanel->addLayout(layoutTopBar);

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

    // Stack dei form
    QStackedWidget *formStack = new QStackedWidget();
    layoutNuovoMedia->addWidget(formStack);

    // Form Vinile
    QWidget *vinileForm = new QWidget();
    QFormLayout *vinileLayout = new QFormLayout(vinileForm);

    // Campi comuni
    QLineEdit *titoloEdit = new QLineEdit();
    QLineEdit *prezzoEdit = new QLineEdit();
    QLineEdit *genereEdit = new QLineEdit();
    QLineEdit *copieEdit = new QLineEdit();
    QCheckBox *disponibileBox = new QCheckBox("Disponibile");

    // Campi specifici
    QLineEdit *durataEdit = new QLineEdit();
    QLineEdit *produzioneEdit = new QLineEdit();
    QLineEdit *artistaEdit = new QLineEdit();
    QLineEdit *tracceEdit = new QLineEdit();

    // Immagine
    QLabel *immagineLabel = new QLabel("Nessuna immagine selezionata");
    QPushButton *caricaImmagineBtn = new QPushButton("Carica immagine");
    QImage *immagine = new QImage();

    // Aggiunta campi al form
    vinileLayout->addRow("Titolo", titoloEdit);
    vinileLayout->addRow("Prezzo", prezzoEdit);
    vinileLayout->addRow("Genere", genereEdit);
    vinileLayout->addRow("Copie disponibili", copieEdit);
    vinileLayout->addRow("Disponibilità", disponibileBox);
    vinileLayout->addRow("Durata", durataEdit);
    vinileLayout->addRow("Produzione", produzioneEdit);
    vinileLayout->addRow("Artista", artistaEdit);
    vinileLayout->addRow("Numero tracce", tracceEdit);
    vinileLayout->addRow(caricaImmagineBtn, immagineLabel);

    formStack->addWidget(vinileForm);

    // Pulsante salva
    QPushButton *saveButton = new QPushButton("Salva");
    layoutNuovoMedia->addWidget(saveButton);

    // Caricamento immagine
    QObject::connect(caricaImmagineBtn, &QPushButton::clicked, [=]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Seleziona immagine", "", "Immagini (*.png *.jpg *.jpeg)");
        if (!fileName.isEmpty()) {
            immagine->load(fileName);
            immagineLabel->setText("Immagine caricata");
        }
    });

    // Salvataggio
    QObject::connect(saveButton, &QPushButton::clicked, [=]() {
        QString titolo = titoloEdit->text();
        float prezzo = prezzoEdit->text().toFloat();
        QString genere = genereEdit->text();
        int copie = copieEdit->text().toInt();
        QString artista = artistaEdit->text();
        int durata = durataEdit->text().toInt();
        QString produzione = produzioneEdit->text();
        int tracce = tracceEdit->text().toInt();
        bool disponibilita = disponibileBox->isChecked();

        // Costruzione oggetto Vinile
        Vinile *vinile = new Vinile(
            *immagine,
            titolo.toStdString(),
            prezzo,
            Data(),  // se non hai un campo data per ora usa costruttore vuoto
            genere.toStdString(),
            disponibilita,
            copie,
            durata,
            produzione.toStdString(),
            artista.toStdString(),
            tracce
            );

        GestoreMedia gestore(listaMedia, "dati.json");
        gestore.inserisciNuovoMedia(vinile);
    });


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
