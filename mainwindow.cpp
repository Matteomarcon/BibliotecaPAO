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
#include <QDateEdit>
#include <QDate>
#include <QList>
#include <QDoubleSpinBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    inizializzaGUI();
    creaPaginaPrincipale();
    creaPaginaForm();
    creaPaginaPrestiti();
    stackPagine->setCurrentIndex(0);
}

MainWindow::~MainWindow() {}

void MainWindow::inizializzaGUI() {
    this->setMinimumSize(1000, 550);
    stackPagine = new QStackedWidget(this);
    setCentralWidget(stackPagine);

    paginaPrincipale = new QWidget(this);
    paginaPrestiti = new QWidget(this);
    paginaForm = new QWidget(this);

    labelRisultatiMedia = new QLabel();
    labelRisultatiPrestiti = new QLabel("Risultati:");
    listaPrestiti = new QListWidget();
    listaMedia = new QListWidget();

    connect(listaMedia, &QListWidget::itemClicked, this, [this]() {
        mostraInfo();
    });
    formLayout = new QFormLayout();
    labelAnteprimaImmagine = new QLabel("Anteprima Immagine");

    labelAnteprimaImmagine->setFixedSize(200, 300);
    labelAnteprimaImmagine->setAlignment(Qt::AlignCenter);
    labelAnteprimaImmagine->setScaledContents(true);
    labelAnteprimaImmagine->setStyleSheet("border: 1px solid gray;");
    labelAnteprimaImmagine->hide();

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
    QString percorsoPrestiti = QDir(QCoreApplication::applicationDirPath()).filePath("../../../prestitiBibliotecaDefault.json");
    QFileInfo checkPrestiti(percorsoPrestiti);
    if (!checkPrestiti.exists()) {
        QFile file(percorsoPrestiti);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            out << "[]";
            file.close();
        }
    }

    gestore = new GestoreMedia(listaMedia, listaPrestiti, formLayout, labelAnteprimaImmagine, percorsoFile);
    gestore->caricaBiblioteca(labelRisultatiMedia);
    gestore->caricaPrestiti(labelRisultatiPrestiti);


    dialogFiltri = new QDialog(this);
    spinPrezzoMin = new QDoubleSpinBox(dialogFiltri);
    spinPrezzoMax = new QDoubleSpinBox(dialogFiltri);
    comboCategoria = new QComboBox(dialogFiltri);
    comboDisponibilita = new QComboBox(dialogFiltri);

    dialogFiltri->setWindowTitle("Filtra ricerca");

    QLabel* labelPrezzoMin = new QLabel("Prezzo minimo:");
    spinPrezzoMin->setRange(0, 1000);
    spinPrezzoMin->setDecimals(2);

    QLabel* labelPrezzoMax = new QLabel("Prezzo massimo:");
    spinPrezzoMax->setRange(0, 1000);
    spinPrezzoMax->setDecimals(2);
    spinPrezzoMax->setValue(1000);

    QLabel* labelDisponibilita = new QLabel("Disponibilità:");
    comboDisponibilita->addItem("Tutti");
    comboDisponibilita->addItem("Disponibili");
    comboDisponibilita->addItem("Non disponibili");

    QLabel* labelCategoria = new QLabel("Categoria:");
    comboCategoria->addItem("Tutti");
    comboCategoria->addItems(GestoreMedia::getTipiDisponibili());

    QPushButton* bottoneOk = new QPushButton("Applica filtri");
    QPushButton* bottoneAnnulla = new QPushButton("Annulla");
    QPushButton* bottonePulisci = new QPushButton("Pulisci");

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(bottoneOk);
    buttonLayout->addWidget(bottoneAnnulla);
    buttonLayout->addWidget(bottonePulisci);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow(labelPrezzoMin, spinPrezzoMin);
    formLayout->addRow(labelPrezzoMax, spinPrezzoMax);
    formLayout->addRow(labelDisponibilita, comboDisponibilita);
    formLayout->addRow(labelCategoria, comboCategoria);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);
    dialogFiltri->setLayout(mainLayout);

    connect(spinPrezzoMin, &QDoubleSpinBox::valueChanged, [=]() {
        spinPrezzoMin->setPalette(QPalette());
        spinPrezzoMax->setPalette(QPalette());
    });
    connect(spinPrezzoMax, &QDoubleSpinBox::valueChanged, [=]() {
        spinPrezzoMin->setPalette(QPalette());
        spinPrezzoMax->setPalette(QPalette());
    });
    // Connect pulsanti
    connect(bottoneOk, &QPushButton::clicked, dialogFiltri, [=](){
        double prezzoMin = spinPrezzoMin->value();
        double prezzoMax = spinPrezzoMax->value();


        if (prezzoMin > prezzoMax) {
            QPalette palette = spinPrezzoMin->palette();
            palette.setColor(QPalette::Base, QColor(255, 0, 0));
            spinPrezzoMin->setPalette(palette);
            spinPrezzoMax->setPalette(palette);
        }
        else {
            aggiornaFiltroMedia(barraRicerca->text(), comboCategoria->currentText(), comboDisponibilita->currentText(), prezzoMin, prezzoMax);
            dialogFiltri->close();
        }
    });
    connect(bottoneAnnulla, &QPushButton::clicked, dialogFiltri, &QDialog::reject);
    connect(bottonePulisci, &QPushButton::clicked, dialogFiltri, [=]() {
        spinPrezzoMin->setValue(0);
        spinPrezzoMax->setValue(1000);
        comboCategoria->setCurrentIndex(0);
        comboDisponibilita->setCurrentIndex(0);
    });
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

    QPushButton *bottoneNuovaBiblioteca = new QPushButton("  Apri\n  Biblioteca");
    bottoneNuovaBiblioteca->setIcon(QIcon(":/icone/folder.png"));
    bottoneNuovaBiblioteca->setIconSize(QSize(36, 36));
    bottoneNuovaBiblioteca->setCursor(Qt::PointingHandCursor);
    bottoneNuovaBiblioteca->setFixedHeight(50);
    bottoneNuovaBiblioteca->setFlat(true);
    bottoneNuovaBiblioteca->setToolTip("Carica una nuova biblioteca (*.json)");

    QPushButton *bottoneForm = new QPushButton("Nuovo\nMedia");
    bottoneForm->setIcon(QIcon(":/icone/add.png"));
    bottoneForm->setIconSize(QSize(48, 48));
    bottoneForm->setCursor(Qt::PointingHandCursor);
    bottoneForm->setFixedHeight(50);
    bottoneForm->setFlat(true);
    bottoneForm->setToolTip("Inserisci un nuovo media nella biblioteca");

    QPushButton *bottonePrestiti = new QPushButton("Visualizza\nPrestiti");
    bottonePrestiti->setIcon(QIcon(":/icone/listaprestiti.png"));
    bottonePrestiti->setIconSize(QSize(48, 48));
    bottonePrestiti->setCursor(Qt::PointingHandCursor);
    bottonePrestiti->setFixedHeight(50);
    bottonePrestiti->setFlat(true);
    bottonePrestiti->setToolTip("Visualizza i prestiti attivi");

    layoutTopBar->addWidget(bottoneNuovaBiblioteca);
    layoutTopBar->addWidget(bottoneForm);
    layoutTopBar->addWidget(bottonePrestiti);

    QWidget* ricerca = new QWidget();
    QHBoxLayout* ricercaLayout = new QHBoxLayout(ricerca);
    ricercaLayout->setContentsMargins(0,0,0,0);

    //Barra di ricerca
    barraRicerca = new QLineEdit();
    barraRicerca->setPlaceholderText("Cerca media...");

    // Bottone filtri
    QPushButton* bottoneFiltri = new QPushButton("Filtri");

    ricercaLayout->addWidget(barraRicerca);
    ricercaLayout->addWidget(bottoneFiltri);
    ricercaLayout->setStretch(0, 2);
    ricercaLayout->setStretch(1, 1);

    // Connect barra di ricerca
    connect(barraRicerca, &QLineEdit::textChanged, this, [=]() {
        aggiornaFiltroMedia(barraRicerca->text(), comboCategoria->currentText(), comboDisponibilita->currentText(), spinPrezzoMin->value(), spinPrezzoMax->value());
    });

    // Connect bottone filtri

    connect(bottoneFiltri, &QPushButton::clicked, this, [=]() {
        dialogFiltri->exec();
    });


    layoutPannelloSinistra->addWidget(topBarSinistra);
    layoutPannelloSinistra->addWidget(ricerca);
    layoutPannelloSinistra->addWidget(labelRisultatiMedia);
    layoutPannelloSinistra->addWidget(listaMedia);

    //DIVISORE
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    //PANNELLO DESTRA
    QWidget *pannelloDestra = new QWidget();
    QVBoxLayout *layoutPannelloDestra = new QVBoxLayout(pannelloDestra);

    //Top bar destra
    QWidget *topBarDestra = new QWidget(this);
    QHBoxLayout *layoutTopBarDestra = new QHBoxLayout(topBarDestra);

    labelIconaTopBar = new QLabel(topBarDestra);
    labelTitoloTopBar = new QLabel(topBarDestra);
    bottoneNuovoPrestito = new QPushButton("Inserisci\nPrestito");
    bottoneModifica = new QPushButton("Modifica\nMedia");
    bottoneElimina = new QPushButton("Elimina\nMedia");

    QFont font = labelTitoloTopBar->font();
    font.setBold(true);
    labelTitoloTopBar->setFont(font);

    bottoneNuovoPrestito->setIcon(QIcon(":/icone/prestito.png"));
    bottoneNuovoPrestito->setFlat(true);
    bottoneNuovoPrestito->setCursor(Qt::PointingHandCursor);
    bottoneNuovoPrestito->setIconSize(QSize(48, 48));
    bottoneNuovoPrestito->setToolTip("Inserisci Prestito");
    bottoneNuovoPrestito->hide();

    bottoneModifica->setIcon(QIcon(":/icone/modifica.png"));
    bottoneModifica->setFlat(true);
    bottoneModifica->setCursor(Qt::PointingHandCursor);
    bottoneModifica->setIconSize(QSize(48, 48));
    bottoneModifica->setToolTip("Modifica Media");
    bottoneModifica->hide();

    bottoneElimina->setIcon(QIcon(":/icone/elimina.png"));
    bottoneElimina->setFlat(true);
    bottoneElimina->setCursor(Qt::PointingHandCursor);
    bottoneElimina->setIconSize(QSize(48, 48));
    bottoneElimina->setToolTip("Elimina Media");
    bottoneElimina->hide();

    layoutTopBarDestra->addWidget(labelIconaTopBar);
    layoutTopBarDestra->addWidget(labelTitoloTopBar);
    layoutTopBarDestra->addStretch();
    layoutTopBarDestra->addWidget(bottoneNuovoPrestito);
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
    labelContainerLayout->addStretch();

    layoutPannelloInfo->addWidget(labelImmagine, 0, Qt::AlignTop);
    layoutPannelloInfo->addWidget(labelContainer);

    layoutPannelloDestra->addWidget(topBarDestra);
    layoutPannelloDestra->addWidget(pannelloInfo);

    layoutPaginaPrincipale->addWidget(pannelloSinistra, 1);
    layoutPaginaPrincipale->addWidget(line);
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
    connect(bottoneNuovoPrestito, &QPushButton::clicked, this, [=]() {
        Media* media = listaMedia->currentItem()->data(Qt::UserRole).value<Media*>();
        if (media->getDisponibilita()) {
            media->setCopie(media->getCopie() - 1);
            if (media->getCopie() == 0) media->setDisponibilita(false);

            QDialog dialog;
            dialog.setWindowTitle("Inserimento nuovo prestito");

            // Campi del form
            QLabel *labelNome = new QLabel("Nome:");
            QLineEdit *editNome = new QLineEdit();

            QLabel *labelCognome = new QLabel("Cognome:");
            QLineEdit *editCognome = new QLineEdit();

            QLabel *labelInizio = new QLabel("Data inizio:");
            QDateEdit *editInizio = new QDateEdit(QDate::currentDate());
            editInizio->setCalendarPopup(true);

            QLabel *labelFine = new QLabel("Data fine:");
            QDateEdit *editFine = new QDateEdit(QDate::currentDate().addDays(7));
            editFine->setCalendarPopup(true);

            QPushButton *okButton = new QPushButton("OK");

            QObject::connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

            // Layout
            QVBoxLayout *layout = new QVBoxLayout();
            layout->addWidget(labelNome);
            layout->addWidget(editNome);
            layout->addWidget(labelCognome);
            layout->addWidget(editCognome);
            layout->addWidget(labelInizio);
            layout->addWidget(editInizio);
            layout->addWidget(labelFine);
            layout->addWidget(editFine);
            layout->addWidget(okButton);

            dialog.setLayout(layout);

            if (dialog.exec() == QDialog::Accepted) {
                QString nome = editNome->text();
                QString cognome = editCognome->text();
                QDate dataInizio = editInizio->date();
                QDate dataFine = editFine->date();

                // Costruisci il contenitore grafico
                QWidget* container = new QWidget;
                QVBoxLayout* layout = new QVBoxLayout(container);
                layout->setContentsMargins(10, 6, 10, 6);
                layout->setSpacing(2);

                // Etichette
                QLabel* labelPrestito = new QLabel("Prestito Nr. " + QString::number(listaPrestiti->count()));
                QLabel* labelUtente = new QLabel("🧑 Richiedente: " + nome + " " + cognome);
                QLabel* labelTitolo = new QLabel("📘 Titolo media: " + media->getTitolo());
                QLabel* labelDataInizio = new QLabel("📅 Data inizio: " + dataInizio.toString("dd/MM/yyyy"));
                QLabel* labelDataFine = new QLabel("📅 Data fine: " + dataFine.toString("dd/MM/yyyy"));

                // Font più evidente per il nome
                QFont boldFont = labelPrestito->font();
                boldFont.setBold(true);
                labelPrestito->setFont(boldFont);

                // Componi
                layout->addWidget(labelPrestito);
                layout->addWidget(labelUtente);
                layout->addWidget(labelTitolo);
                layout->addWidget(labelDataInizio);
                layout->addWidget(labelDataFine);

                // Crea item invisibile e collega il widget
                QListWidgetItem* item = new QListWidgetItem;
                item->setSizeHint(container->sizeHint());
                item->setData(Qt::UserRole, media->getId());
                item->setText(nome + cognome + dataInizio.toString("dd/MM/yyy") + dataFine.toString("dd/MM/yyyy"));
                listaPrestiti->addItem(item);
                listaPrestiti->setItemWidget(item, container);
                mostraInfo();
                gestore->salvaPrestito(new Prestito(nome, cognome, dataInizio, dataFine, media->getId()));  // eventualmente aggiorna se vuoi passare anche nome/cognome/date
            }
        } else {
            QMessageBox::warning(nullptr, "Errore", "Il media selezionato non è disponibile nella biblioteca.");
        }
    });

    connect(bottonePrestiti, &QPushButton::clicked, this, &MainWindow::mostraPaginaPrestiti);
}


void MainWindow::creaPaginaForm() {
    QVBoxLayout *layoutPaginaForm = new QVBoxLayout(paginaForm);
    stackPagine->addWidget(paginaForm);

    //Top Bar
    QWidget *topBar = new QWidget();
    QHBoxLayout *layoutTopBar = new QHBoxLayout(topBar);

    QPushButton *bottoneIndietro = new QPushButton("Torna\nIndietro");
    bottoneSalva = new QPushButton("Salva\nMedia");

    bottoneIndietro->setIcon(QIcon(":/icone/indietro.png"));
    bottoneIndietro->setIconSize(QSize(36, 36));
    bottoneIndietro->setCursor(Qt::PointingHandCursor);
    bottoneIndietro->setFixedHeight(50);
    bottoneIndietro->setFlat(true);
    bottoneIndietro->setToolTip("Torna alla pagina principale");

    bottoneSalva->setIcon(QIcon(":/icone/salva.png"));
    bottoneSalva->setIconSize(QSize(36, 36));
    bottoneSalva->setCursor(Qt::PointingHandCursor);
    bottoneSalva->setFixedHeight(50);
    bottoneSalva->setFlat(true);
    bottoneSalva->setToolTip("Salva media");
    bottoneSalva->hide();

    layoutTopBar->addWidget(bottoneIndietro);
    layoutTopBar->addStretch();
    layoutTopBar->addWidget(bottoneSalva);

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

    layoutPaginaForm->setAlignment(Qt::AlignTop);
    layoutPaginaForm->addWidget(topBar);
    layoutPaginaForm->addWidget(selettoreMedia);
    layoutPaginaForm->addWidget(form);

    //Connect
    connect(selettoreMedia, &QComboBox::currentTextChanged, this, [=](const QString &tipoSelezionato) {
        bottoneSalva->show();
        labelAnteprimaImmagine->show();
        while (formLayout->rowCount() > 0) {
            QLayoutItem* labelItem = formLayout->itemAt(0, QFormLayout::LabelRole);
            QLayoutItem* fieldItem = formLayout->itemAt(0, QFormLayout::FieldRole);
            if (labelItem && labelItem->widget()) delete labelItem->widget();
            if (fieldItem && fieldItem->widget()) delete fieldItem->widget();
            formLayout->removeRow(0);
        }
        gestore->creaForm(tipoSelezionato);
    });
    connect(bottoneIndietro, &QPushButton::clicked, this, &MainWindow::mostraPaginaPrincipale);
    connect(bottoneSalva, &QPushButton::clicked, this, [=]() {
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
        gestore->caricaBiblioteca(labelRisultatiMedia);

        if (indiceSelezionato >= 0 && indiceSelezionato < listaMedia->count()) {
            listaMedia->setCurrentRow(indiceSelezionato);
            mostraInfo();
        }
        stackPagine->setCurrentWidget(paginaPrincipale);
    });
}

void MainWindow::mostraPaginaPrincipale() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Attenzione");
    msgBox.setText("Sei sicuro di voler annullare?");
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
    listaMedia->setCurrentItem(nullptr);
    stackPagine->setCurrentWidget(paginaForm);
}

void MainWindow::caricaBiblioteca() {
    QString percorsoFile = QFileDialog::getOpenFileName(nullptr, "Scegli Biblioteca", "", "Documento (*.json)");
    if (!percorsoFile.isEmpty()) {
        gestore = new GestoreMedia(listaMedia, listaPrestiti, formLayout, labelAnteprimaImmagine, percorsoFile); // !!! Distruzione funziona????
        gestore->caricaBiblioteca(labelRisultatiMedia);
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
        mostraInfo();
    }
    else if (msgBox.clickedButton() == annullaButton) {
        msgBox.close();
    }
}

void MainWindow::creaPaginaPrestiti() {
    QVBoxLayout *layoutPaginaPrestiti = new QVBoxLayout(paginaPrestiti);
    stackPagine->addWidget(paginaPrestiti);

    //Top bar
    QWidget *topBar= new QWidget(this);
    QHBoxLayout *layoutTopBar = new QHBoxLayout(topBar);

    QPushButton *bottoneIndietro = new QPushButton("Torna\nIndietro");
    bottoneRestituzione = new QPushButton("Restituisci\nPrestito");

    bottoneIndietro->setIcon(QIcon(":/icone/indietro.png"));
    bottoneIndietro->setIconSize(QSize(36, 36));
    bottoneIndietro->setCursor(Qt::PointingHandCursor);
    bottoneIndietro->setFixedHeight(50);
    bottoneIndietro->setFlat(true);
    bottoneIndietro->setToolTip("Torna alla pagina principale");

    bottoneRestituzione->setIcon(QIcon(":/icone/restituzione.png"));
    bottoneRestituzione->setFlat(true);
    bottoneRestituzione->setCursor(Qt::PointingHandCursor);
    bottoneRestituzione->setIconSize(QSize(48, 48));
    bottoneRestituzione->setToolTip("Restituzione media");
    bottoneRestituzione->hide();

    layoutTopBar->addWidget(bottoneIndietro);
    layoutTopBar->addStretch();
    layoutTopBar->addWidget(bottoneRestituzione);

    //Barra di ricerca
    QLineEdit *barraRicerca = new QLineEdit();
    barraRicerca->setPlaceholderText("Cerca prestiti...");
    connect(barraRicerca, &QLineEdit::textChanged, this, [=](const QString &text) {
        int visibili = 0;
        for (int i = 0; i < listaPrestiti->count(); ++i) {
            QListWidgetItem *item = listaPrestiti->item(i);
            bool match = item->text().contains(text, Qt::CaseInsensitive);
            item->setHidden(!match);
            if (match) ++visibili;
        }
        labelRisultatiPrestiti->setText(QString("Risultati: %1").arg(visibili));
    });

    layoutPaginaPrestiti->addWidget(topBar);
    layoutPaginaPrestiti->addWidget(barraRicerca);
    layoutPaginaPrestiti->addWidget(labelRisultatiPrestiti);
    layoutPaginaPrestiti->addWidget(listaPrestiti);

    connect(bottoneIndietro, &QPushButton::clicked, this, [=]() {
        stackPagine->setCurrentWidget(paginaPrincipale);
        if (listaMedia->currentItem()) mostraInfo();
    });

    connect(listaPrestiti, &QListWidget::itemSelectionChanged, this, [=]() {
        bottoneRestituzione->show();
    });

    connect(bottoneRestituzione, &QPushButton::clicked, this, [=]() {
        QListWidgetItem* item = listaPrestiti->currentItem();
        int indice = listaPrestiti->row(item);
        qDebug()<<item->data(Qt::UserRole).toString();

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Conferma restituzione");
        msgBox.setText("Sei sicuro di voler restituire questo prestito?");
        msgBox.setIcon(QMessageBox::Warning);

        QPushButton* confermaButton = msgBox.addButton(tr("Conferma"), QMessageBox::AcceptRole);
        QPushButton* annullaButton = msgBox.addButton(tr("Annulla"), QMessageBox::RejectRole);

        msgBox.exec();

        if (msgBox.clickedButton() == confermaButton) {
            Media* media = listaMedia->item(0)->data(Qt::UserRole).value<Media*>();
            for (int i=0; media->getId()!=item->data(Qt::UserRole).toInt(); i++) {
                media = listaMedia->item(i)->data(Qt::UserRole).value<Media*>();

            }
            if (!media->getCopie()) {

                media->setDisponibilita(true);
            }
            media->setCopie(media->getCopie()+1);
            delete listaPrestiti->takeItem(indice);
            gestore->eliminaPrestito(indice);

            if (!listaPrestiti->count()) bottoneRestituzione->hide();
        }
        else if (msgBox.clickedButton() == annullaButton) {
            msgBox.close();
        }
    });
}

void MainWindow::mostraPaginaPrestiti() {
    stackPagine->setCurrentWidget(paginaPrestiti);
}

void MainWindow::aggiornaFiltroMedia(QString testoRicerca, QString categoriaSelezionata, QString disponibilitaSelezionata, double minPrezzo, double maxPrezzo) {
    int visibili = 0;


    QListWidgetItem* currentItem = listaMedia->currentItem();

    for (int i = 0; i < listaMedia->count(); ++i) {
        QListWidgetItem *item = listaMedia->item(i);
        Media* media = item->data(Qt::UserRole).value<Media*>();

        if (!media) continue;

        bool matchTesto = item->text().contains(testoRicerca, Qt::CaseInsensitive);
        bool matchPrezzo = media->getPrezzo() >= minPrezzo && media->getPrezzo() <= maxPrezzo;

        bool matchDisp = (disponibilitaSelezionata == "Tutti" ||
                          (disponibilitaSelezionata == "Disponibili" && media->getDisponibilita()) ||
                          (disponibilitaSelezionata == "Non disponibili" && !media->getDisponibilita()));

        bool matchCategoria = false;
        if (categoriaSelezionata == "Tutti") matchCategoria = true;
        else if (categoriaSelezionata == "Libro" && dynamic_cast<Libro*>(media)) matchCategoria = true;
        else if (categoriaSelezionata == "Film" && dynamic_cast<Film*>(media)) matchCategoria = true;
        else if (categoriaSelezionata == "Rivista" && dynamic_cast<Rivista*>(media)) matchCategoria = true;
        else if (categoriaSelezionata == "Giornale" && dynamic_cast<Giornale*>(media)) matchCategoria = true;
        else if (categoriaSelezionata == "Vinile" && dynamic_cast<Vinile*>(media)) matchCategoria = true;

        bool visibile = matchTesto && matchPrezzo && matchDisp && matchCategoria;
        item->setHidden(!visibile);

        if (visibile) ++visibili;
    }

    labelRisultatiMedia->setText(QString("Risultati: %1").arg(visibili));

    if (currentItem && currentItem->isHidden()) {
        pulisciInfo();
    }
    else mostraInfo();
}

void MainWindow::pulisciInfo() {
    QList<QLabel*> allLabels = {labelIconaTopBar, labelTitoloTopBar, labelTitolo, labelPrezzo, labelData, labelGenere,
                                 labelAutore, labelEditore, labelTestata, labelFormato, labelLingua, labelLinguaOriginale,
                                 labelRegista, labelProduzione, labelDurata, labelPaese, labelNumero, labelPeriodicita,
                                 labelArtista, labelNumeroTracce, labelImmagine, labelDisponibilita, labelCopie, labelCategoria};
    for (QLabel* l : allLabels) l->hide();

    bottoneNuovoPrestito->hide();
    bottoneModifica->hide();
    bottoneElimina->hide();
}

void MainWindow::mostraInfo() {
    Media* media = listaMedia->currentItem()->data(Qt::UserRole).value<Media*>();

    pulisciInfo();

    bottoneModifica->show();
    bottoneElimina->show();
    if (media->getDisponibilita()) bottoneNuovoPrestito->show();

    labelIconaTopBar->setPixmap(listaMedia->currentItem()->icon().pixmap(44,44));
    labelIconaTopBar->show();
    labelTitoloTopBar->setText(media->getTitolo());
    labelTitoloTopBar->show();
    labelTitolo->setText("Titolo: " + media->getTitolo());
    labelTitolo->show();
    labelPrezzo->setText("Prezzo: " + QString::number(media->getPrezzo()) + "€");
    labelPrezzo->show();
    labelData->setText("Data: " + media->getData().toString("dd/MM/yyyy"));
    labelData->show();
    labelGenere->setText("Genere: " + media->getGenere());
    labelGenere->show();
    labelDisponibilita->setText(media->getDisponibilita() ? "Disponibilità: Disponibile" : "Disponibilità: Non disponibile");
    labelDisponibilita->show();
    labelCopie->setText("Copie: " + QString::number(media->getCopie()));
    labelCopie->show();
    labelCategoria->show();


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
        labelImmagine->show();
    }
    else {
        labelImmagine->show();
        labelImmagine->clear();
        labelImmagine->setText("Immagine");
    }
}
