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
#include <QFileDialog>
#include <QMessageBox>
#include <QCoreApplication>
#include <QPalette>
#include <QApplication>

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

    QFileInfo checkFile(percorsoFile);
    if (!checkFile.exists() || !checkFile.isFile()) {
        gestore = new GestoreMedia(listaMedia, percorsoFile); // !!! Da creare il file se non presente
        QFile file(percorsoFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            out << "[]";  // JSON vuoto
            file.close();
        }
    }

    // !!! cosa succede se non crea il file?
    gestore = new GestoreMedia(listaMedia, percorsoFile);
    gestore->caricaBiblioteca();

    connect(listaMedia, &QListWidget::itemClicked, this, &MainWindow::onItemClicked);

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
    selettoreMedia->addItems(gestore->getTipiDisponibili());
    selettoreMedia->setCurrentIndex(0);
    layoutNuovoMedia->addWidget(selettoreMedia);

    QWidget* formContainer = new QWidget();
    formLayout = new QFormLayout(formContainer);
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

        gestore->creaForm(tipoSelezionato, formLayout);
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
    connect(toNuovoMedia, &QPushButton::clicked, this, &MainWindow::showPaginaNuovoMedia);
    connect(salvaNuovoMediaButton, &QPushButton::clicked, this, [=]() {
        if (selettoreMedia->currentIndex() == 0) QMessageBox::warning(nullptr, "Attenzione", "Per favore seleziona un tipo di media.");
        else  {
            gestore->salvaMediaDaForm(selettoreMedia->currentText(), formLayout);
            gestore->caricaBiblioteca();
            //showPaginaPrincipale();
        }
    });
}

MainWindow::~MainWindow() {}

void MainWindow::eliminaMedia() {
    QListWidgetItem* item = listaMedia->currentItem();
    int indice = listaMedia->row(item);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Conferma eliminazione");
    msgBox.setText("Sei sicuro di voler eliminare questo elemento?");
    msgBox.setIcon(QMessageBox::Question);

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
        gestore = new GestoreMedia(listaMedia, percorsoFile); // !!! Distruzione funziona????
        gestore->caricaBiblioteca();
    }
}


void MainWindow::showPaginaPrincipale() {

    // !!! Dividi pulsante annulla e salva

    QMessageBox msgBox;
    msgBox.setWindowTitle("Modifiche non salvate");
    msgBox.setText("Vuoi salvare la bozza prima di uscire?");
    msgBox.setIcon(QMessageBox::Warning);

    // Aggiungi i pulsanti personalizzati
    QPushButton *annullaButton = msgBox.addButton("Annulla", QMessageBox::RejectRole);
    QPushButton *nonSalvareButton = msgBox.addButton("Non salvare", QMessageBox::DestructiveRole);
    QPushButton *salvaBozzaButton = msgBox.addButton("Salva bozza", QMessageBox::AcceptRole);

    // Mostra la finestra di dialogo
    msgBox.exec();

    // Controlla il pulsante premuto
    if (msgBox.clickedButton() == salvaBozzaButton) {
         stackedWidget->setCurrentWidget(paginaPrincipale);
    } else if (msgBox.clickedButton() == nonSalvareButton) {
        stackedWidget->setCurrentWidget(paginaPrincipale); // !!! da implementare: senza salvare
    } else if (msgBox.clickedButton() == annullaButton) {
        msgBox.close();
    }
}

void MainWindow::showPaginaNuovoMedia() {
    stackedWidget->setCurrentWidget(paginaNuovoMedia);
}

void MainWindow::onItemClicked(QListWidgetItem *item) {
    if (item) {
        labelIcona->setPixmap(item->icon().pixmap(44,44));
        labelTitoloTopBar->setText(item->text());

        QVariant val;
        val = item->data(Qt::UserRole);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelCategoria->setText("Categoria: " + val.toString());
            labelCategoria->show();
        }
        else labelCategoria->hide();

        val = item->data(Qt::UserRole + 2);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelTitolo->setText("Titolo: " + val.toString());
            labelTitolo->show();
        }
        else labelTitolo->hide();

        val = item->data(Qt::UserRole + 3);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelPrezzo->setText("Prezzo: " + val.toString() + "€");
            labelPrezzo->show();
        }
        else labelPrezzo->hide();

        val = item->data(Qt::UserRole + 4);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelData->setText("Data di Pubblicazione: " + val.toString());
            labelData->show();
        }
        else labelData->hide();

        val = item->data(Qt::UserRole + 5);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelGenere->setText("Genere: " + val.toString());
            labelGenere->show();
        }
        else labelGenere->hide();

        val = item->data(Qt::UserRole + 6);
        if (val.isValid() && !val.toString().isEmpty()) {
            if (val.toString() == "true") labelDisponibilita->setText("Disponibilità: Disponibile");
            else labelDisponibilita->setText("Disponibilità: Non disponibile");
            labelDisponibilita->show();
        }
        else labelDisponibilita->hide();

        val = item->data(Qt::UserRole + 7);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelCopie->setText("Copie: " + val.toString());
            labelCopie->show();
        }
        else labelCopie->hide();

        val = item->data(Qt::UserRole + 8);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelDurata->setText("Durata: " + val.toString() + " minuti");
            labelDurata->show();
        }
        else labelDurata->hide();

        val = item->data(Qt::UserRole + 9);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelProduzione->setText("Produzione: " + val.toString());
            labelProduzione->show();
        }
        else labelProduzione->hide();

        val = item->data(Qt::UserRole + 10);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelAutore->setText("Autore: " + val.toString());
            labelAutore->show();
        }
        else labelAutore->hide();

        val = item->data(Qt::UserRole + 11);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelEditore->setText("Editore: " + val.toString());
            labelEditore->show();
        }
        else labelEditore->hide();

        val = item->data(Qt::UserRole + 12);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelRegista->setText("Regista: " + val.toString());
            labelRegista->show();
        }
        else labelRegista->hide();

        val = item->data(Qt::UserRole + 13);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelLinguaOriginale->setText("Lingua Originale: " + val.toString());
            labelLinguaOriginale->show();
        }
        else labelLinguaOriginale->hide();

        val = item->data(Qt::UserRole + 14);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelPaese->setText("Paese di Produzione: " + val.toString());
            labelPaese->show();
        }
        else labelPaese->hide();

        val = item->data(Qt::UserRole + 15);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelTestata->setText("Testata: " + val.toString());
            labelTestata->show();
        }
        else labelTestata->hide();

        val = item->data(Qt::UserRole + 16);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelFormato->setText("Formato: " + val.toString());
            labelFormato->show();
        }
        else labelFormato->hide();

        val = item->data(Qt::UserRole + 17);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelLingua->setText("Lingua: " + val.toString());
            labelLingua->show();
        }
        else labelLingua->hide();

        val = item->data(Qt::UserRole + 18);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelNumero->setText("Numero: " + val.toString());
            labelNumero->show();
        }
        else labelNumero->hide();

        val = item->data(Qt::UserRole + 19);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelPeriodicita->setText("Periodicità: " + val.toString());
            labelPeriodicita->show();
        }
        else labelPeriodicita->hide();

        val = item->data(Qt::UserRole + 20);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelArtista->setText("Artista: " + val.toString());
            labelArtista->show();
        }
        else labelArtista->hide();

        val = item->data(Qt::UserRole + 21);
        if (val.isValid() && !val.toString().isEmpty()) {
            labelNumeroTracce->setText("Numero Tracce: " + val.toString());
            labelNumeroTracce->show();
        }
        else labelNumeroTracce->hide();


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
