#include "mainwindow.h"
#include "media.h"
#include "film.h"
#include "giornale.h"
#include "libro.h"
#include "rivista.h"
#include "vinile.h"

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
#include <QMessageBox>
#include <QCoreApplication>
#include <QPalette>
#include <QApplication>
#include <QToolTip>
#include <QTimer>
#include <QStatusBar>
#include <typeinfo>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setMinimumSize(400,300);

    //Inizializzazione pagine
    paginaPrincipale = new QWidget(this);
    paginaNuovoMedia = new QWidget(this);

    //Pagina principale

    //Top Bar
    QHBoxLayout *layoutTopBar = new QHBoxLayout();

    QPushButton *nuovaBibliotecaButton = new QPushButton("  Apri Biblioteca");
    QPushButton *toNuovoMedia = new QPushButton("Nuovo Media");

    nuovaBibliotecaButton->setIconSize(QSize(36, 36));
    toNuovoMedia->setIconSize(QSize(48, 48));

    nuovaBibliotecaButton->setIcon(QIcon(":/icone/folder.png"));
    toNuovoMedia->setIcon(QIcon(":/icone/add.png"));
    nuovaBibliotecaButton->setCursor(Qt::PointingHandCursor);
    toNuovoMedia->setCursor(Qt::PointingHandCursor);
    nuovaBibliotecaButton->setFixedHeight(50);
    toNuovoMedia->setFixedHeight(50);
    nuovaBibliotecaButton->setFlat(true);
    toNuovoMedia->setFlat(true);
    nuovaBibliotecaButton->setToolTip("Carica una nuova biblioteca (*.json)");
    toNuovoMedia->setToolTip("Inserisci un nuovo media nella biblioteca");
    layoutTopBar->addWidget(nuovaBibliotecaButton);
    layoutTopBar->addWidget(toNuovoMedia);

    //Left Panel
    QVBoxLayout *layoutLeftPanel = new QVBoxLayout();
    layoutLeftPanel->addLayout(layoutTopBar);

    QLineEdit *searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Cerca...");
    QLabel *leftLabel = new QLabel("Risultati:");
    listaMedia = new QListWidget();

    QString percorsoBase = QCoreApplication::applicationDirPath();
    QString percorsoFile = QDir(percorsoBase).filePath("../../../BibliotecaDefault.json");
    QWidget* formContainer = new QWidget();
    formLayout = new QFormLayout(formContainer);

    QFileInfo checkFile(percorsoFile);
    if (!checkFile.exists() || !checkFile.isFile()) {
        gestore = new GestoreMedia(listaMedia, formLayout, percorsoFile); // !!! Da creare il file se non presente
        QFile file(percorsoFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            out << "[]";  // JSON vuoto
            file.close();
        }
    }

    // !!! cosa succede se non crea il file?
    gestore = new GestoreMedia(listaMedia, formLayout, percorsoFile);
    gestore->caricaBiblioteca();

    connect(listaMedia, &QListWidget::itemClicked, this, &MainWindow::mostraInfo);

    layoutLeftPanel->addWidget(searchBar);
    layoutLeftPanel->addWidget(leftLabel);
    layoutLeftPanel->addWidget(listaMedia);

    QWidget *leftPanel = new QWidget();
    leftPanel->setLayout(layoutLeftPanel);

    // Right Panel
    rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    // ---------- Top Bar ----------
    QWidget *topBar = new QWidget(rightPanel);
    topBar->setFixedHeight(60);
    QHBoxLayout *topBarLayout = new QHBoxLayout(topBar);

    // Icona utente (es: QLabel con QPixmap o QPushButton flat)
    labelIcona = new QLabel(topBar);

    // Nome contatto
    labelTitoloTopBar = new QLabel(topBar);
    QFont nameFont = labelTitoloTopBar->font();
    nameFont.setBold(true);
    labelTitoloTopBar->setFont(nameFont);

    // Spacer centrale
    topBarLayout->addWidget(labelIcona);
    topBarLayout->addSpacing(8);
    topBarLayout->addWidget(labelTitoloTopBar);
    topBarLayout->addStretch();

    // Bottone 1
    QPushButton *modificaButton = new QPushButton(topBar);
    modificaButton->setIcon(QIcon(":/icone/modifica.png"));
    modificaButton->setFlat(true);
    modificaButton->setCursor(Qt::PointingHandCursor);
    modificaButton->setIconSize(QSize(48, 48));
    modificaButton->setToolTip("Modifica media");

    // Bottone 2
    QPushButton *eliminaButton = new QPushButton(topBar);
    eliminaButton->setIcon(QIcon(":/icone/elimina.png"));
    eliminaButton->setFlat(true);
    eliminaButton->setCursor(Qt::PointingHandCursor);
    eliminaButton->setIconSize(QSize(48, 48));
    eliminaButton->setToolTip("Elimina media");

    connect(eliminaButton, &QPushButton::clicked, this, &MainWindow::eliminaMedia);

    // Spazio tra bottoni (opzionale)
    topBarLayout->addWidget(modificaButton);
    topBarLayout->addWidget(eliminaButton);
    // ---------- Contenuto inferiore ----------
    labelCategoria = new QLabel(rightPanel);
    labelTitolo = new QLabel(rightPanel);
    labelAutore = new QLabel(rightPanel);
    labelEditore = new QLabel(rightPanel);
    labelTestata = new QLabel(rightPanel);
    labelTitolo = new QLabel(rightPanel);
    labelPrezzo = new QLabel(rightPanel);
    labelGenere = new QLabel(rightPanel);
    labelData = new QLabel(rightPanel);
    labelDisponibilita = new QLabel(rightPanel);
    labelCopie = new QLabel(rightPanel);
    labelFormato = new QLabel(rightPanel);
    labelLingua = new QLabel(rightPanel);
    labelLinguaOriginale = new QLabel(rightPanel);
    labelRegista = new QLabel(rightPanel);
    labelProduzione = new QLabel(rightPanel);
    labelDurata = new QLabel(rightPanel);
    labelPaese = new QLabel(rightPanel);
    labelNumero = new QLabel(rightPanel);
    labelPeriodicita = new QLabel(rightPanel);
    labelArtista = new QLabel(rightPanel);
    labelNumeroTracce = new QLabel(rightPanel);
    labelImmagine = new QLabel(rightPanel);
    // ---------- Layout finale ----------
    rightLayout->addWidget(topBar);
    rightLayout->addWidget(labelCategoria);
    rightLayout->addWidget(labelTitolo);
    rightLayout->addWidget(labelAutore);
    rightLayout->addWidget(labelEditore);
    rightLayout->addWidget(labelTestata);
    rightLayout->addWidget(labelPrezzo);
    rightLayout->addWidget(labelGenere);
    rightLayout->addWidget(labelData);
    rightLayout->addWidget(labelDisponibilita);
    rightLayout->addWidget(labelCopie);
    rightLayout->addWidget(labelFormato);
    rightLayout->addWidget(labelLingua);
    rightLayout->addWidget(labelLinguaOriginale);
    rightLayout->addWidget(labelRegista);
    rightLayout->addWidget(labelProduzione);
    rightLayout->addWidget(labelDurata);
    rightLayout->addWidget(labelPaese);
    rightLayout->addWidget(labelNumero);
    rightLayout->addWidget(labelPeriodicita);
    rightLayout->addWidget(labelArtista);
    rightLayout->addWidget(labelNumeroTracce);
    rightLayout->addWidget(labelImmagine);
    rightLayout->setAlignment(Qt::AlignTop);

    //Layout left e right panel
    QHBoxLayout *LayoutLeftRightPanel = new QHBoxLayout();
    LayoutLeftRightPanel->addWidget(leftPanel, 1);
    LayoutLeftRightPanel->addWidget(rightPanel, 3);

    //Layout pagina principale
    QVBoxLayout *layoutPaginaPrincipale = new QVBoxLayout();
    layoutPaginaPrincipale->addLayout(LayoutLeftRightPanel);
    paginaPrincipale->setLayout(layoutPaginaPrincipale);

    // Pagina nuovo media

    // Layout seconda pagina
    QVBoxLayout *layoutNuovoMedia = new QVBoxLayout(paginaNuovoMedia);

    //Combo box per selezione tipo
    QComboBox *selettoreMedia = new QComboBox();
    selettoreMedia->addItem("Scegli media...");
    selettoreMedia->setItemData(0, 0, Qt::UserRole - 1); //Disabilita la voce
    selettoreMedia->addItems(GestoreMedia::getTipiDisponibili());
    selettoreMedia->setCurrentIndex(0);
    layoutNuovoMedia->addWidget(selettoreMedia);

    layoutNuovoMedia->addWidget(formContainer);

    connect(selettoreMedia, &QComboBox::currentTextChanged, this, [=](const QString& tipoSelezionato) {
        // Pulisce tutti i widget dal formLayout
        while (formLayout->rowCount() > 0) {
            QLayoutItem* labelItem = formLayout->itemAt(0, QFormLayout::LabelRole);
            QLayoutItem* fieldItem = formLayout->itemAt(0, QFormLayout::FieldRole);

            if (labelItem && labelItem->widget()) delete labelItem->widget();
            if (fieldItem && fieldItem->widget()) delete fieldItem->widget();

            formLayout->removeRow(0);
        }

        gestore->creaForm(tipoSelezionato);
    });

    // Bottom Bar
    QHBoxLayout *bottomBarLayout = new QHBoxLayout();
    QPushButton *toPrincipale = new QPushButton("Annulla");
    QPushButton *salvaNuovoMediaButton = new QPushButton("Salva");
    bottomBarLayout->addWidget(toPrincipale);
    bottomBarLayout->addWidget(salvaNuovoMediaButton);
    bottomBarLayout->setAlignment(Qt::AlignBottom);
    layoutNuovoMedia->addLayout(bottomBarLayout);

    // Stack di pagine
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(paginaPrincipale);
    stackedWidget->addWidget(paginaNuovoMedia);
    stackedWidget->setCurrentWidget(paginaPrincipale);
    setCentralWidget(stackedWidget);

    // Connessioni
    connect(nuovaBibliotecaButton, &QPushButton::clicked, this, &MainWindow::caricaBiblioteca);

    connect(toPrincipale, &QPushButton::clicked, this, &MainWindow::showPaginaPrincipale);
    connect(toNuovoMedia, &QPushButton::clicked, this, [=]() {
        listaMedia->setCurrentItem(NULL);
        selettoreMedia->setCurrentIndex(0); // !!! Resettare form
        MainWindow::showPaginaNuovoMedia();
    });

    connect(salvaNuovoMediaButton, &QPushButton::clicked, this, [=]() {
        if (selettoreMedia->currentIndex() == 0) {
            QMessageBox::warning(nullptr, "Attenzione", "Per favore seleziona un tipo di media.");
        }

        int indiceSelezionato = -1;

        if (listaMedia->currentItem()) {
            // Modifica esistente
            indiceSelezionato = listaMedia->row(listaMedia->currentItem());
            gestore->salvaMediaDaForm(selettoreMedia->currentText(), indiceSelezionato);
        } else {
            // Nuovo media
            gestore->salvaMediaDaForm(selettoreMedia->currentText());
            indiceSelezionato = listaMedia->count(); // sarà l’ultimo
        }

        gestore->caricaBiblioteca();

        // Reimposta il current row correttamente dopo il reload
        if (indiceSelezionato >= 0 && indiceSelezionato < listaMedia->count()) {
            listaMedia->setCurrentRow(indiceSelezionato);
            mostraInfo(listaMedia->currentItem());
        }

        stackedWidget->setCurrentWidget(paginaPrincipale);
    });


    connect(modificaButton, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentWidget(paginaNuovoMedia);

        Media* media = listaMedia->currentItem()->data(Qt::UserRole).value<Media*>();

        if (typeid(*media) == typeid(Film)) selettoreMedia->setCurrentIndex(1);
        else if (typeid(*media) == typeid(Giornale)) selettoreMedia->setCurrentIndex(2);
        else if (typeid(*media) == typeid(Libro)) selettoreMedia->setCurrentIndex(3);
        else if (typeid(*media) == typeid(Rivista)) selettoreMedia->setCurrentIndex(4);
        else if (typeid(*media) == typeid(Vinile)) selettoreMedia->setCurrentIndex(5);

        gestore->caricaFormDaMedia(listaMedia->row(listaMedia->currentItem()));
    });
}

MainWindow::~MainWindow() {}

void MainWindow::eliminaMedia() {
    QListWidgetItem* item = listaMedia->currentItem();
    int indice = listaMedia->row(item);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Conferma eliminazione");
    msgBox.setText("Sei sicuro di voler eliminare questo elemento?");
    msgBox.setIcon(QMessageBox::Warning);

    QPushButton* confermaButton = msgBox.addButton(tr("Conferma"), QMessageBox::AcceptRole);
    QPushButton* annullaButton = msgBox.addButton(tr("Annulla"), QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == confermaButton) {
        delete listaMedia->takeItem(indice);
        gestore->eliminaMedia(indice);
    }
    else if (msgBox.clickedButton() == annullaButton) {
        msgBox.close();
    }
}

void MainWindow::caricaBiblioteca() {
    QString percorsoFile = QFileDialog::getOpenFileName(nullptr, "Scegli Biblioteca", "", "Documento (*.json)");
    if (!percorsoFile.isEmpty()) {
        gestore = new GestoreMedia(listaMedia, formLayout, percorsoFile); // !!! Distruzione funziona????
        gestore->caricaBiblioteca();
    }
}

void MainWindow::showPaginaPrincipale() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Modifiche non salvate");
    msgBox.setText("Vuoi salvare la bozza prima di uscire?");
    msgBox.setIcon(QMessageBox::Warning);

    QPushButton* confermaButton = msgBox.addButton(tr("Conferma"), QMessageBox::AcceptRole);
    QPushButton* annullaButton = msgBox.addButton(tr("Annulla"), QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == confermaButton) {
        stackedWidget->setCurrentWidget(paginaPrincipale);
    }
    else if (msgBox.clickedButton() == annullaButton) {
        msgBox.close();
    }
}

void MainWindow::showPaginaNuovoMedia() {
    stackedWidget->setCurrentWidget(paginaNuovoMedia);
}

void MainWindow::mostraInfo(QListWidgetItem *item) {
    if (item) {
        labelIcona->setPixmap(item->icon().pixmap(44,44));
        labelTitoloTopBar->setText(item->text());

        Media* media = item->data(Qt::UserRole).value<Media*>();

        labelTitolo->setText("Titolo: " + media->getTitolo());
        labelPrezzo->setText("Prezzo: " + QString::number(media->getPrezzo()) + "€");
        labelData->setText("Data: " + media->getData());
        labelGenere->setText("Genere: " + media->getGenere());
        if (media->getDisponibilita()) labelDisponibilita->setText("Disponibilità: Disponibile");
        else labelDisponibilita->setText("Disponibilità: Non disponibile");
        labelCopie->setText("Copie: " + QString::number(media->getCopie()));
        labelDurata->setVisible(false);
        labelProduzione->setVisible(false);
        labelRegista->setVisible(false);
        labelLinguaOriginale->setVisible(false);
        labelPaese->setVisible(false);
        labelAutore->setVisible(false);
        labelEditore->setVisible(false);
        labelLingua->setVisible(false);
        labelFormato->setVisible(false);
        labelTestata->setVisible(false);
        labelNumero->setVisible(false);
        labelPeriodicita->setVisible(false);
        labelArtista->setVisible(false);
        labelNumeroTracce->setVisible(false);

        if (Film* film = dynamic_cast<Film*>(media)) {
            labelCategoria->setText("Categoria: Film");
            labelDurata->setText("Durata: " + QString::number(film->getDurata()) + " minuti");
            labelProduzione->setText("Produzione: " + film->getProduzione());
            labelRegista->setText("Regista: " + film->getRegista());
            labelLinguaOriginale->setText("Lingua originale: " + film->getLinguaOriginale());
            labelPaese->setText("Paese di produzione: " + film->getPaeseProduzione());
            labelDurata->setVisible(true);
            labelProduzione->setVisible(true);
            labelRegista->setVisible(true);
            labelLinguaOriginale->setVisible(true);
            labelPaese->setVisible(true);
        }

        else if (Giornale* giornale = dynamic_cast<Giornale*>(media)) {
            labelCategoria->setText("Categoria: Giornale");
            labelAutore->setText("Autore: " + giornale->getAutore());
            labelEditore->setText("Editore: " + giornale->getEditore());
            labelRegista->setText("Testata: " + giornale->getTestata());
            labelAutore->setVisible(true);
            labelEditore->setVisible(true);
            labelRegista->setVisible(true);
        }

        else if (Libro* libro = dynamic_cast<Libro*>(media)) {
            labelCategoria->setText("Categoria: Libro");
            labelAutore->setText("Autore: " + libro->getAutore());
            labelEditore->setText("Editore: " + libro->getEditore());
            labelLingua->setText("Lingua: " + libro->getLingua());
            labelFormato->setText("Formato: " + libro ->getFormato());
            labelAutore->setVisible(true);
            labelEditore->setVisible(true);
            labelLingua->setVisible(true);
            labelFormato->setVisible(true);
        }

        else if (Rivista* rivista = dynamic_cast<Rivista*>(media)) {
            labelCategoria->setText("Categoria: Rivista");
            labelAutore->setText("Autore: " + rivista->getAutore());
            labelEditore->setText("Editore: " + rivista->getEditore());
            labelNumero->setText("Numero: " + QString::number(rivista->getNumero()));
            labelPeriodicita->setText("Periodicità: " + rivista->getPeriodicita());
            labelAutore->setVisible(true);
            labelEditore->setVisible(true);
            labelNumero->setVisible(true);
            labelPeriodicita->setVisible(true);
        }

        else if (Vinile* vinile = dynamic_cast<Vinile*>(media)) {
            labelCategoria->setText("Categoria: Vinile");
            labelDurata->setText("Durata: " + QString::number(vinile->getDurata()) + " minuti");
            labelProduzione->setText("Produzione: " + vinile->getProduzione());
            labelArtista->setText("Artista: " + vinile->getArtista());
            labelNumeroTracce->setText("Numero tracce: " + QString::number(vinile->getNumeroTracce()));
            labelDurata->setVisible(true);
            labelProduzione->setVisible(true);
            labelArtista->setVisible(true);
            labelNumeroTracce->setVisible(true);
        }

        /*QString base64 = item->data(Qt::UserRole).toString();
        QByteArray byteArray = QByteArray::fromBase64(base64.toLatin1());

        QPixmap pixmap;
        if (pixmap.loadFromData(byteArray)) {
            labelImmagine->setPixmap(pixmap.scaled(500, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        else {
            labelImmagine->clear();
            labelImmagine->setText("Immagine:");
        }*/
    }
}
