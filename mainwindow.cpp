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
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QFormLayout>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QCoreApplication>
#include <QStackedWidget>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    inizializzaGUI();
    creaPaginaPrincipale();
    creaPaginaForm();
    creaPaginaPrestiti();
    stackPagine->setCurrentIndex(0);
}

MainWindow::~MainWindow() {}

void MainWindow::inizializzaGUI() {
    this->setMinimumSize(400, 300);
    stackPagine = new QStackedWidget(this);
    setCentralWidget(stackPagine);

    paginaPrincipale = new QWidget(this);
    paginaPrestiti = new QWidget(this);
    paginaForm = new QWidget(this);

    labelRisultati = new QLabel("Risultati:");
    listaPrestiti = new QListWidget();
    listaMedia = new QListWidget();
    connect(listaMedia, &QListWidget::itemClicked, this, &MainWindow::mostraInfo);
    formLayout = new QFormLayout();
    labelAnteprimaImmagine = new QLabel("Anteprima Immagine");

    labelAnteprimaImmagine->setFixedSize(200, 300);
    labelAnteprimaImmagine->setAlignment(Qt::AlignCenter);
    labelAnteprimaImmagine->setScaledContents(true);
    labelAnteprimaImmagine->setStyleSheet("border: 1px solid gray;");

    QString percorsoFile = QDir(QCoreApplication::applicationDirPath()).filePath("../../../BibliotecaDefault.json");
    QFileInfo checkFile(percorsoFile);
    if (!checkFile.exists()) {
        QFile file(percorsoFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            out << "[]";
            file.close();
        }
    }

    gestore = new GestoreMedia(listaMedia, formLayout, labelAnteprimaImmagine, percorsoFile);
    gestore->caricaBiblioteca(labelRisultati);
}

QLabel* MainWindow::creaLabel(QWidget *parent, QVBoxLayout *layout) {
    QLabel* label = new QLabel(parent);
    layout->addWidget(label);
    return label;
}

void MainWindow::creaPaginaPrincipale() {
    QHBoxLayout *layoutPaginaPrincipale = new QHBoxLayout(paginaPrincipale);
    stackPagine->addWidget(paginaPrincipale);

    //PANNELLO SINISTRA
    QWidget *pannelloSinistra = new QWidget();
    QVBoxLayout *layoutPannelloSinistra = new QVBoxLayout(pannelloSinistra);

    //Top bar sinistra
    QWidget *topBarSinistra = new QWidget(this);
    QHBoxLayout *layoutTopBar = new QHBoxLayout(topBarSinistra);

    QPushButton *bottoneNuovaBiblioteca = new QPushButton("  Apri Biblioteca");
    bottoneNuovaBiblioteca->setIcon(QIcon(":/icone/folder.png"));
    bottoneNuovaBiblioteca->setIconSize(QSize(36, 36));
    bottoneNuovaBiblioteca->setCursor(Qt::PointingHandCursor);
    bottoneNuovaBiblioteca->setFixedHeight(50);
    bottoneNuovaBiblioteca->setFlat(true);
    bottoneNuovaBiblioteca->setToolTip("Carica una nuova biblioteca (*.json)");

    QPushButton *bottoneForm = new QPushButton("Nuovo Media");
    bottoneForm->setIcon(QIcon(":/icone/add.png"));
    bottoneForm->setIconSize(QSize(48, 48));
    bottoneForm->setCursor(Qt::PointingHandCursor);
    bottoneForm->setFixedHeight(50);
    bottoneForm->setFlat(true);
    bottoneForm->setToolTip("Inserisci un nuovo media nella biblioteca");

    layoutTopBar->addWidget(bottoneNuovaBiblioteca);
    layoutTopBar->addWidget(bottoneForm);

    //Barra di ricerca
    QLineEdit *barraRicerca = new QLineEdit();
    barraRicerca->setPlaceholderText("Cerca...");
    connect(barraRicerca, &QLineEdit::textChanged, this, [=](const QString &text) {
        int visibili = 0;
        for (int i = 0; i < listaMedia->count(); ++i) {
            QListWidgetItem *item = listaMedia->item(i);
            bool match = item->text().contains(text, Qt::CaseInsensitive);
            item->setHidden(!match);
            if (match) ++visibili;
        }
        labelRisultati->setText(QString("Risultati: %1").arg(visibili));
    });

    QPushButton *bottoneMostraPrestiti = new QPushButton("Mostra prestiti");

    layoutPannelloSinistra->addWidget(topBarSinistra);
    layoutPannelloSinistra->addWidget(barraRicerca);
    layoutPannelloSinistra->addWidget(labelRisultati);
    layoutPannelloSinistra->addWidget(listaMedia);
    layoutPannelloSinistra->addWidget(bottoneMostraPrestiti);

    //PANNELLO DESTRA
    QWidget *pannelloDestra = new QWidget();
    QVBoxLayout *layoutPannelloDestra = new QVBoxLayout(pannelloDestra);

    //Top bar destra
    QWidget *topBarDestra = new QWidget(this);
    QHBoxLayout *layoutTopBarDestra = new QHBoxLayout(topBarDestra);

    labelIconaTopBar = new QLabel(topBarDestra);
    labelTitoloTopBar = new QLabel(topBarDestra);
    QPushButton *bottoneModifica = new QPushButton(topBarDestra);
    QPushButton *bottoneElimina = new QPushButton(topBarDestra);

    QFont font = labelTitoloTopBar->font();
    font.setBold(true);
    labelTitoloTopBar->setFont(font);
    bottoneModifica->setIcon(QIcon(":/icone/modifica.png"));
    bottoneModifica->setFlat(true);
    bottoneModifica->setCursor(Qt::PointingHandCursor);
    bottoneModifica->setIconSize(QSize(48, 48));
    bottoneModifica->setToolTip("Modifica media");
    bottoneElimina->setIcon(QIcon(":/icone/elimina.png"));
    bottoneElimina->setFlat(true);
    bottoneElimina->setCursor(Qt::PointingHandCursor);
    bottoneElimina->setIconSize(QSize(48, 48));
    bottoneElimina->setToolTip("Elimina media");

    layoutTopBarDestra->addWidget(labelIconaTopBar);
    layoutTopBarDestra->addWidget(labelTitoloTopBar);
    layoutTopBarDestra->addStretch();
    layoutTopBarDestra->addWidget(bottoneModifica);
    layoutTopBarDestra->addWidget(bottoneElimina);

    //Pannello Info
    QWidget *pannelloInfo = new QWidget(this);
    QHBoxLayout *layoutPannelloInfo = new QHBoxLayout(pannelloInfo);

    labelImmagine = new QLabel(pannelloInfo);
    labelImmagine->setFixedSize(300, 400); // Dimensioni fisse opzionali
    labelImmagine->setStyleSheet("border: 1px solid gray;"); // Bordi visivi
    labelImmagine->setAlignment(Qt::AlignCenter);
    labelImmagine->setScaledContents(true);
    labelImmagine->hide();

    QWidget *labelContainer = new QWidget(this);
    QVBoxLayout *labelContainerLayout = new QVBoxLayout(labelContainer);
    labelCategoria = creaLabel(this, labelContainerLayout);
    labelTitolo = creaLabel(this, labelContainerLayout);
    labelAutore = creaLabel(this, labelContainerLayout);
    labelEditore = creaLabel(this, labelContainerLayout);
    labelTestata = creaLabel(this, labelContainerLayout);
    labelPrezzo = creaLabel(this, labelContainerLayout);
    labelGenere = creaLabel(this, labelContainerLayout);
    labelData = creaLabel(this, labelContainerLayout);
    labelDisponibilita = creaLabel(this, labelContainerLayout);
    labelCopie = creaLabel(this, labelContainerLayout);
    labelFormato = creaLabel(this, labelContainerLayout);
    labelLingua = creaLabel(this, labelContainerLayout);
    labelLinguaOriginale = creaLabel(this, labelContainerLayout);
    labelRegista = creaLabel(this, labelContainerLayout);
    labelProduzione = creaLabel(this, labelContainerLayout);
    labelDurata = creaLabel(this, labelContainerLayout);
    labelPaese = creaLabel(this, labelContainerLayout);
    labelNumero = creaLabel(this, labelContainerLayout);
    labelPeriodicita = creaLabel(this, labelContainerLayout);
    labelArtista = creaLabel(this, labelContainerLayout);
    labelNumeroTracce = creaLabel(this, labelContainerLayout);
    QPushButton *bottonePrestiti = new QPushButton("Inserisci nuovo prestito");
    labelContainerLayout->addWidget(bottonePrestiti);
    labelContainerLayout->addStretch();

    layoutPannelloInfo->addWidget(labelImmagine, 0, Qt::AlignTop);
    layoutPannelloInfo->addWidget(labelContainer);

    layoutPannelloDestra->addWidget(topBarDestra);
    layoutPannelloDestra->addWidget(pannelloInfo);

    layoutPaginaPrincipale->addWidget(pannelloSinistra, 1);
    layoutPaginaPrincipale->addWidget(pannelloDestra, 3);

    //Connect
    connect(bottoneNuovaBiblioteca, &QPushButton::clicked, this, &MainWindow::caricaBiblioteca);
    connect(bottoneForm, &QPushButton::clicked, this, &MainWindow::mostraPaginaForm);
    connect(bottoneElimina, &QPushButton::clicked, this, &MainWindow::eliminaMedia);
    connect(bottoneModifica, &QPushButton::clicked, this, [=]() {
        stackPagine->setCurrentWidget(paginaForm);

        Media* media = listaMedia->currentItem()->data(Qt::UserRole).value<Media*>();

        if (typeid(*media) == typeid(Film)) selettoreMedia->setCurrentIndex(1);
        else if (typeid(*media) == typeid(Giornale)) selettoreMedia->setCurrentIndex(2);
        else if (typeid(*media) == typeid(Libro)) selettoreMedia->setCurrentIndex(3);
        else if (typeid(*media) == typeid(Rivista)) selettoreMedia->setCurrentIndex(4);
        else if (typeid(*media) == typeid(Vinile)) selettoreMedia->setCurrentIndex(5);

        gestore->caricaFormDaMedia(listaMedia->row(listaMedia->currentItem()));
    });
    connect(bottonePrestiti, &QPushButton::clicked, this, [=]() {
        Media* media = listaMedia->currentItem()->data(Qt::UserRole).value<Media*>();
        if (media->getDisponibilita()) {
            media->setCopie(media->getCopie()-1);
            if (media->getCopie()==0) media->setDisponibilita(false);
            QDialog dialog;
            dialog.setWindowTitle("Inserimento nuovo prestito");

            QLabel *label = new QLabel("Inserisci nome e cognome:");
            QLineEdit *edit = new QLineEdit();
            QPushButton *okButton = new QPushButton("OK");

            QObject::connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

            QVBoxLayout *layout = new QVBoxLayout();
            layout->addWidget(label);
            layout->addWidget(edit);
            layout->addWidget(okButton);

            dialog.setLayout(layout);

            if (dialog.exec() == QDialog::Accepted) {
                QListWidgetItem *item = new QListWidgetItem(edit->text());
                listaPrestiti->addItem(item);
            }
        }
        else {
            QMessageBox::warning(nullptr, "Errore", "Il media selezionato non e' disponibile nella biblioteca.");
        }
    });
    connect(bottoneMostraPrestiti, &QPushButton::clicked, this, &MainWindow::mostraPaginaPrestiti);
}


void MainWindow::creaPaginaForm() {
    QVBoxLayout *layoutPaginaForm = new QVBoxLayout(paginaForm);
    stackPagine->addWidget(paginaForm);

    //Selettore Media
    selettoreMedia = new QComboBox();
    selettoreMedia->addItem("Scegli media...");
    selettoreMedia->setItemData(0, 0, Qt::UserRole - 1);
    selettoreMedia->addItems(GestoreMedia::getTipiDisponibili());
    selettoreMedia->setCurrentIndex(0);

    //Form
    QWidget *form = new QWidget();
    QHBoxLayout *layoutForm = new QHBoxLayout(form);

    layoutForm->addLayout(formLayout);
    layoutForm->addWidget(labelAnteprimaImmagine);
    layoutForm->setStretch(0, 2);
    layoutForm->setStretch(1, 1);

    //Bottom Bar
    QWidget *bottomBar = new QWidget(this);
    QHBoxLayout *bottomBarLayout = new QHBoxLayout(bottomBar);

    QPushButton *toPrincipale = new QPushButton("Annulla");
    QPushButton *salvaNuovoMediaButton = new QPushButton("Salva");

    bottomBarLayout->addWidget(toPrincipale);
    bottomBarLayout->addWidget(salvaNuovoMediaButton);
    bottomBarLayout->setAlignment(Qt::AlignBottom);

    layoutPaginaForm->addWidget(selettoreMedia);
    layoutPaginaForm->addWidget(form);
    layoutPaginaForm->addWidget(bottomBar);

    //Connect
    connect(selettoreMedia, &QComboBox::currentTextChanged, this, [=](const QString &tipoSelezionato) {
        while (formLayout->rowCount() > 0) {
            QLayoutItem* labelItem = formLayout->itemAt(0, QFormLayout::LabelRole);
            QLayoutItem* fieldItem = formLayout->itemAt(0, QFormLayout::FieldRole);
            if (labelItem && labelItem->widget()) delete labelItem->widget();
            if (fieldItem && fieldItem->widget()) delete fieldItem->widget();
            formLayout->removeRow(0);
        }
        gestore->creaForm(tipoSelezionato);
    });
    connect(toPrincipale, &QPushButton::clicked, this, &MainWindow::mostraPaginaPrincipale);
    connect(salvaNuovoMediaButton, &QPushButton::clicked, this, [=]() {
        if (selettoreMedia->currentIndex() == 0) {
            QMessageBox::warning(nullptr, "Attenzione", "Per favore seleziona un tipo di media.");
            return;
        }

        int indiceSelezionato = -1;
        if (listaMedia->currentItem()) {
            indiceSelezionato = listaMedia->row(listaMedia->currentItem());
            gestore->salvaMediaDaForm(selettoreMedia->currentText(), indiceSelezionato);
        } else {
            gestore->salvaMediaDaForm(selettoreMedia->currentText());
            indiceSelezionato = listaMedia->count();
        }
        gestore->caricaBiblioteca(labelRisultati);

        if (indiceSelezionato >= 0 && indiceSelezionato < listaMedia->count()) {
            listaMedia->setCurrentRow(indiceSelezionato);
            mostraInfo(listaMedia->currentItem());
        }
        stackPagine->setCurrentWidget(paginaPrincipale);
    });
}

void MainWindow::mostraInfo(QListWidgetItem *item) {
    Media* media = item->data(Qt::UserRole).value<Media*>();

    labelIconaTopBar->setPixmap(item->icon().pixmap(44,44));
    labelTitoloTopBar->setText(item->text());
    labelTitolo->setText("Titolo: " + media->getTitolo());
    labelPrezzo->setText("Prezzo: " + QString::number(media->getPrezzo()) + "€");
    labelData->setText("Data: " + media->getData());
    labelGenere->setText("Genere: " + media->getGenere());
    labelDisponibilita->setText(media->getDisponibilita() ? "Disponibilità: Disponibile" : "Disponibilità: Non disponibile");
    labelCopie->setText("Copie: " + QString::number(media->getCopie()));

    QList<QLabel*> allLabels = {labelAutore, labelEditore, labelTestata, labelFormato, labelLingua, labelLinguaOriginale,
                                 labelRegista, labelProduzione, labelDurata, labelPaese, labelNumero, labelPeriodicita,
                                 labelArtista, labelNumeroTracce};
    for (QLabel* l : allLabels) l->hide();

    if (auto* film = dynamic_cast<Film*>(media)) {
        labelCategoria->setText("Categoria: Film");
        labelDurata->setText("Durata: " + QString::number(film->getDurata()));
        labelProduzione->setText("Produzione: " + film->getProduzione());
        labelRegista->setText("Regista: " + film->getRegista());
        labelLinguaOriginale->setText("Lingua originale: " + film->getLinguaOriginale());
        labelPaese->setText("Paese: " + film->getPaeseProduzione());
        labelDurata->show(); labelProduzione->show(); labelRegista->show(); labelLinguaOriginale->show(); labelPaese->show();
    }
    else if (auto* giornale = dynamic_cast<Giornale*>(media)) {
        labelCategoria->setText("Categoria: Giornale");
        labelAutore->setText("Autore: " + giornale->getAutore());
        labelEditore->setText("Editore: " + giornale->getEditore());
        labelTestata->setText("Testata: " + giornale->getTestata());
        labelAutore->show(); labelEditore->show(); labelTestata->show();
    }
    else if (auto* libro = dynamic_cast<Libro*>(media)) {
        labelCategoria->setText("Categoria: Libro");
        labelAutore->setText("Autore: " + libro->getAutore());
        labelEditore->setText("Editore: " + libro->getEditore());
        labelLingua->setText("Lingua: " + libro->getLingua());
        labelFormato->setText("Formato: " + libro->getFormato());
        labelAutore->show(); labelEditore->show(); labelLingua->show(); labelFormato->show();
    }
    else if (auto* rivista = dynamic_cast<Rivista*>(media)) {
        labelCategoria->setText("Categoria: Rivista");
        labelAutore->setText("Autore: " + rivista->getAutore());
        labelEditore->setText("Editore: " + rivista->getEditore());
        labelNumero->setText("Numero: " + QString::number(rivista->getNumero()));
        labelPeriodicita->setText("Periodicità: " + rivista->getPeriodicita());
        labelAutore->show(); labelEditore->show(); labelNumero->show(); labelPeriodicita->show();
    }
    else if (auto* vinile = dynamic_cast<Vinile*>(media)) {
        labelCategoria->setText("Categoria: Vinile");
        labelDurata->setText("Durata: " + QString::number(vinile->getDurata()));
        labelProduzione->setText("Produzione: " + vinile->getProduzione());
        labelArtista->setText("Artista: " + vinile->getArtista());
        labelNumeroTracce->setText("Numero tracce: " + QString::number(vinile->getNumeroTracce()));
        labelDurata->show(); labelProduzione->show(); labelArtista->show(); labelNumeroTracce->show();
    }

    QString base64 = media->getImmagine();
    QByteArray byteArray = QByteArray::fromBase64(base64.toLatin1());

    QPixmap pixmap;
    if (pixmap.loadFromData(byteArray)) {
        labelImmagine->setPixmap(pixmap.scaled(500, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else {
        labelImmagine->show();
        labelImmagine->clear();
        labelImmagine->setText("Immagine");
    }
}

void MainWindow::mostraPaginaPrincipale() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Modifiche non salvate");
    msgBox.setText("Vuoi salvare la bozza prima di uscire?");
    msgBox.setIcon(QMessageBox::Warning);

    QPushButton* confermaButton = msgBox.addButton(tr("Conferma"), QMessageBox::AcceptRole);
    QPushButton* annullaButton = msgBox.addButton(tr("Annulla"), QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == confermaButton) {
        stackPagine->setCurrentWidget(paginaPrincipale);
    }
    else if (msgBox.clickedButton() == annullaButton) {
        msgBox.close();
    }
}

void MainWindow::mostraPaginaForm() {
    stackPagine->setCurrentWidget(paginaForm);
}

void MainWindow::caricaBiblioteca() {
    QString percorsoFile = QFileDialog::getOpenFileName(nullptr, "Scegli Biblioteca", "", "Documento (*.json)");
    if (!percorsoFile.isEmpty()) {
        gestore = new GestoreMedia(listaMedia, formLayout, labelAnteprimaImmagine, percorsoFile); // !!! Distruzione funziona????
        gestore->caricaBiblioteca(labelRisultati);
    }
}

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

void MainWindow::creaPaginaPrestiti() {
    QVBoxLayout *layoutPaginaPrestiti = new QVBoxLayout(paginaPrestiti);
    stackPagine->addWidget(paginaPrestiti);
    QPushButton *bottoneIndietro = new QPushButton("Torna indietro");
    layoutPaginaPrestiti->addWidget(listaPrestiti);
    layoutPaginaPrestiti->addStretch();
    layoutPaginaPrestiti->addWidget(bottoneIndietro);
    connect(bottoneIndietro, &QPushButton::clicked, this, [=]() {
        stackPagine->setCurrentWidget(paginaPrincipale);
    });
}

void MainWindow::mostraPaginaPrestiti() {
    stackPagine->setCurrentWidget(paginaPrestiti);
}

