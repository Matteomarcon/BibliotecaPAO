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
#include <QFont>
#include <QShortcut>
#include <QTextEdit>
#include <QBuffer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    inizializzaGUI();
    creaPaginaPrincipale();
    creaPaginaForm();
    creaPaginaPrestiti();
    stackPagine->setCurrentIndex(0);
}

MainWindow::~MainWindow() {
    delete gestore;
}

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

    QShortcut* shortcutUpMedia = new QShortcut(QKeySequence(Qt::Key_Up), this);
    connect(shortcutUpMedia, &QShortcut::activated, this, [=] {
        if (listaMedia->currentItem() && listaMedia->item(listaMedia->currentRow()-1) && stackPagine->currentIndex()==0) {
            listaMedia->setCurrentRow(listaMedia->currentRow()-1);
            mostraInfo();
        }
        if (listaPrestiti->currentItem() && listaPrestiti->item(listaPrestiti->currentRow()-1) && stackPagine->currentIndex() == 2) {
            listaPrestiti->setCurrentRow(listaPrestiti->currentRow()-1);
        }
    });

    QShortcut* shortcutDownMedia = new QShortcut(QKeySequence(Qt::Key_Down), this);
    connect(shortcutDownMedia, &QShortcut::activated, this, [=] {
        if (listaMedia->currentItem() && listaMedia->item(listaMedia->currentRow()+1) && stackPagine->currentIndex() == 0) {
            listaMedia->setCurrentRow(listaMedia->currentRow()+1);
            mostraInfo();
        }
        if (listaPrestiti->currentItem() && listaPrestiti->item(listaPrestiti->currentRow()+1) && stackPagine->currentIndex() == 2) {
            listaPrestiti->setCurrentRow(listaPrestiti->currentRow()+1);
        }
    });

    QShortcut* shortcutCancMedia = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    connect(shortcutCancMedia, &QShortcut::activated, this, [=] {
        if (listaMedia->currentItem() && stackPagine->currentIndex()==0) {
            bool prestitiAttivi = false;
            for (int i=0; i<listaPrestiti->count(); i++) {
                if (listaPrestiti->item(i)->data(Qt::UserRole)==gestore->getMedia(listaMedia->currentRow())->getId()) {
                    prestitiAttivi = true;
                }
            }
            if (prestitiAttivi) {
                QMessageBox::warning(nullptr, "Attenzione", "Non puoi eliminare un media con uno o piu' prestiti attivi.");
            } else {
                eliminaMedia();
            }
        }
        int visibili = 0;
        for (int i = 0; i < listaMedia->count(); ++i) {
            QListWidgetItem *item = listaMedia->item(i);
            item->setHidden(false);
            ++visibili;
        }
        labelRisultatiMedia->setText(QString("Risultati: %1").arg(visibili));
        if (visibili==0) pulisciInfo();
    });

    formLayout = new QFormLayout();
    labelAnteprimaImmagine = new QLabel("Anteprima Immagine");

    labelAnteprimaImmagine->setFixedSize(200, 300);
    labelAnteprimaImmagine->setAlignment(Qt::AlignCenter);
    labelAnteprimaImmagine->setScaledContents(true);
    labelAnteprimaImmagine->setStyleSheet("border: 1px solid gray;");
    labelAnteprimaImmagine->hide();

    QString percorsoFile = QDir(QCoreApplication::applicationDirPath()).filePath("BibliotecaDefault.json");
    QFileInfo checkFile(percorsoFile);
    if (!checkFile.exists()) {
        QFile file(percorsoFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            out << "[]";
            file.close();
        }
    }
    QString percorsoPrestiti = QDir(QCoreApplication::applicationDirPath()).filePath("prestitiBibliotecaDefault.json");
    QFileInfo checkPrestiti(percorsoPrestiti);
    if (!checkPrestiti.exists()) {
        QFile file(percorsoPrestiti);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            out << "[]";
            file.close();
        }
    }

    gestore = new Gestore(percorsoFile);
    caricaMedia();
    caricaPrestiti();


    dialogFiltri = new QDialog(this);
    spinPrezzoMin = new QDoubleSpinBox(dialogFiltri);
    spinPrezzoMax = new QDoubleSpinBox(dialogFiltri);
    comboCategoria = new QComboBox(dialogFiltri);
    comboDisponibilita = new QComboBox(dialogFiltri);

    dialogFiltri->setWindowTitle("Filtra ricerca");

    QLabel* labelPrezzoMin = new QLabel("Prezzo minimo:", dialogFiltri);
    spinPrezzoMin->setRange(0, 1000);
    spinPrezzoMin->setDecimals(2);

    QLabel* labelPrezzoMax = new QLabel("Prezzo massimo:", dialogFiltri);
    spinPrezzoMax->setRange(0, 1000);
    spinPrezzoMax->setDecimals(2);
    spinPrezzoMax->setValue(1000);

    QLabel* labelDisponibilita = new QLabel("Disponibilità:", dialogFiltri);
    comboDisponibilita->addItem("Tutti");
    comboDisponibilita->addItem("Disponibili");
    comboDisponibilita->addItem("Non disponibili");

    QLabel* labelCategoria = new QLabel("Categoria:", dialogFiltri);
    comboCategoria->addItem("Tutti");
    comboCategoria->addItems(Gestore::getTipiDisponibili());

    QPushButton* bottoneOk = new QPushButton("Applica filtri", dialogFiltri);
    QPushButton* bottoneAnnulla = new QPushButton("Annulla", dialogFiltri);
    QPushButton* bottonePulisci = new QPushButton("Pulisci", dialogFiltri);

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

    QVBoxLayout* mainLayout = new QVBoxLayout(dialogFiltri);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    connect(spinPrezzoMin, &QDoubleSpinBox::valueChanged, [=]() {
        spinPrezzoMin->setPalette(QPalette());
        spinPrezzoMax->setPalette(QPalette());
    });
    connect(spinPrezzoMax, &QDoubleSpinBox::valueChanged, [=]() {
        spinPrezzoMin->setPalette(QPalette());
        spinPrezzoMax->setPalette(QPalette());
    });

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
    bottoneNuovaBiblioteca->setToolTip("Carica una nuova biblioteca (*.json) (CTRL+O)");

    QShortcut* shortcutNuovaBiblioteca = new QShortcut(QKeySequence("Ctrl+O"), this);
    connect(shortcutNuovaBiblioteca, &QShortcut::activated, this, &MainWindow::caricaBiblioteca);

    QPushButton *bottoneForm = new QPushButton("Nuovo\nMedia");
    bottoneForm->setIcon(QIcon(":/icone/add.png"));
    bottoneForm->setIconSize(QSize(48, 48));
    bottoneForm->setCursor(Qt::PointingHandCursor);
    bottoneForm->setFixedHeight(50);
    bottoneForm->setFlat(true);
    bottoneForm->setToolTip("Inserisci un nuovo media nella biblioteca (CTRL+N)");

    QShortcut* shortcutNuovoMedia = new QShortcut(QKeySequence("Ctrl+N"), this);
    connect(shortcutNuovoMedia, &QShortcut::activated, this, &MainWindow::mostraPaginaForm);

    QPushButton *bottonePrestiti = new QPushButton("Visualizza\nPrestiti");
    bottonePrestiti->setIcon(QIcon(":/icone/listaprestiti.png"));
    bottonePrestiti->setIconSize(QSize(48, 48));
    bottonePrestiti->setCursor(Qt::PointingHandCursor);
    bottonePrestiti->setFixedHeight(50);
    bottonePrestiti->setFlat(true);
    bottonePrestiti->setToolTip("Visualizza i prestiti attivi (CTRL+P)");

    QShortcut* shortcutListaPrestiti = new QShortcut(QKeySequence("Ctrl+P"), this);
    connect(shortcutListaPrestiti, &QShortcut::activated, this, &MainWindow::mostraPaginaPrestiti);

    layoutTopBar->addWidget(bottoneNuovaBiblioteca);
    layoutTopBar->addWidget(bottoneForm);
    layoutTopBar->addWidget(bottonePrestiti);

    QWidget* ricerca = new QWidget();
    QHBoxLayout* ricercaLayout = new QHBoxLayout(ricerca);
    ricercaLayout->setContentsMargins(0,0,0,0);

    //Barra di ricerca
    barraRicerca = new QLineEdit();
    barraRicerca->setPlaceholderText("Cerca media... (CTRL+F)");

    // Bottone filtri
    QPushButton* bottoneFiltri = new QPushButton("Filtri", ricerca);

    ricercaLayout->addWidget(barraRicerca);
    ricercaLayout->addWidget(bottoneFiltri);
    ricercaLayout->setStretch(0, 2);
    ricercaLayout->setStretch(1, 1);

    connect(barraRicerca, &QLineEdit::textChanged, this, [=]() {
        aggiornaFiltroMedia(barraRicerca->text(), comboCategoria->currentText(), comboDisponibilita->currentText(), spinPrezzoMin->value(), spinPrezzoMax->value());
    });

    connect(bottoneFiltri, &QPushButton::clicked, this, [=]() {
        dialogFiltri->exec();
    });


    layoutPannelloSinistra->addWidget(topBarSinistra);
    layoutPannelloSinistra->addWidget(ricerca);
    layoutPannelloSinistra->addWidget(labelRisultatiMedia);
    layoutPannelloSinistra->addWidget(listaMedia);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    QWidget *pannelloDestra = new QWidget();
    QVBoxLayout *layoutPannelloDestra = new QVBoxLayout(pannelloDestra);

    QWidget *topBarDestra = new QWidget(this);
    QHBoxLayout *layoutTopBarDestra = new QHBoxLayout(topBarDestra);

    labelIconaTopBar = new QLabel(topBarDestra);
    labelTitoloTopBar = new QLabel(topBarDestra);
    bottoneNuovoPrestito = new QPushButton("Inserisci\nPrestito");
    bottoneModifica = new QPushButton("Modifica\nMedia");
    bottoneElimina = new QPushButton("Elimina\nMedia");

    QShortcut* shortcutModifica = new QShortcut(QKeySequence("Ctrl+M"), this);
    connect(shortcutModifica, &QShortcut::activated, this, [=] {
        if (listaMedia->currentItem() && stackPagine->currentIndex()==0) {
            stackPagine->setCurrentWidget(paginaForm);
            caricaForm();
        }
    });

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
    bottoneModifica->setToolTip("Modifica Media (CTRL+M)");
    bottoneModifica->hide();

    bottoneElimina->setIcon(QIcon(":/icone/elimina.png"));
    bottoneElimina->setFlat(true);
    bottoneElimina->setCursor(Qt::PointingHandCursor);
    bottoneElimina->setIconSize(QSize(48, 48));
    bottoneElimina->setToolTip("Elimina Media (CANC)");
    bottoneElimina->hide();

    layoutTopBarDestra->addWidget(labelIconaTopBar);
    layoutTopBarDestra->addWidget(labelTitoloTopBar);
    layoutTopBarDestra->addStretch();
    layoutTopBarDestra->addWidget(bottoneNuovoPrestito);
    layoutTopBarDestra->addWidget(bottoneModifica);
    layoutTopBarDestra->addWidget(bottoneElimina);

    QWidget *pannelloInfo = new QWidget(this);
    QHBoxLayout *layoutPannelloInfo = new QHBoxLayout(pannelloInfo);

    labelImmagine = new QLabel(pannelloInfo);
    labelImmagine->setFixedSize(300, 400);
    labelImmagine->setStyleSheet("border: 1px solid gray;");
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

    connect(bottoneNuovaBiblioteca, &QPushButton::clicked, this, &MainWindow::caricaBiblioteca);
    connect(bottoneForm, &QPushButton::clicked, this, &MainWindow::mostraPaginaForm);
    connect(bottoneElimina, &QPushButton::clicked, this, [=]() {
        bool prestitiAttivi = false;
        for (int i=0; i<listaPrestiti->count(); i++) {
            if (listaPrestiti->item(i)->data(Qt::UserRole)==gestore->getMedia(listaMedia->currentRow())->getId()) {
                prestitiAttivi = true;
            }
        }
        if (prestitiAttivi) {
            QMessageBox::warning(nullptr, "Attenzione", "Non puoi eliminare un media con uno o piu' prestiti attivi.");
        } else {
            eliminaMedia();
        }
        int visibili = 0;
        for (int i = 0; i < listaMedia->count(); ++i) {
            QListWidgetItem *item = listaMedia->item(i);
            item->setHidden(false);
            ++visibili;
        }
        labelRisultatiMedia->setText(QString("Risultati: %1").arg(visibili));
        if (visibili==0) pulisciInfo();
    });
    connect(bottoneModifica, &QPushButton::clicked, this, [=]() {
        stackPagine->setCurrentWidget(paginaForm);
        caricaForm();
    });
    connect(bottoneNuovoPrestito, &QPushButton::clicked, this, [=]() {
        Media* media = gestore->getMedia(listaMedia->currentRow());
        if (media->getDisponibilita()) {

            dialog = new QDialog(this);
            dialog->setWindowTitle("Inserimento nuovo prestito");

            QLabel *labelNome = new QLabel("Nome:", dialog);
            QLineEdit *editNome = new QLineEdit(dialog);

            QLabel *labelCognome = new QLabel("Cognome:", dialog);
            QLineEdit *editCognome = new QLineEdit(dialog);

            QLabel *labelInizio = new QLabel("Data inizio:", dialog);
            QDateEdit *editInizio = new QDateEdit(QDate::currentDate(), dialog);
            editInizio->setCalendarPopup(true);

            QLabel *labelFine = new QLabel("Data fine:", dialog);
            QDateEdit *editFine = new QDateEdit(QDate::currentDate().addDays(7), dialog);
            editFine->setCalendarPopup(true);

            QPushButton *okButton = new QPushButton("OK", dialog);
            connect(okButton, &QPushButton::clicked, dialog, [=](){
                if (editNome->text()=="") {
                    QPalette palette = editNome->palette();
                    palette.setColor(QPalette::Base, QColor(255, 0, 0));
                    editNome->setPalette(palette);
                }
                if (editCognome->text()=="") {
                    QPalette palette = editCognome->palette();
                    palette.setColor(QPalette::Base, QColor(255, 0, 0));
                    editCognome->setPalette(palette);
                }
                if (editNome->text()!="" && editCognome->text()!="") {
                    QString nome = editNome->text();
                    QString cognome = editCognome->text();
                    QDate dataInizio = editInizio->date();
                    QDate dataFine = editFine->date();

                    QWidget* container = new QWidget(listaPrestiti);
                    QVBoxLayout* layout = new QVBoxLayout(container);
                    layout->setContentsMargins(10, 6, 10, 6);
                    layout->setSpacing(2);

                    QLabel* labelPrestito = new QLabel("Prestito", container);
                    QLabel* labelUtente = new QLabel("🧑 Richiedente: " + nome + " " + cognome, container);
                    QLabel* labelTitolo = new QLabel("📘 Titolo media: " + media->getTitolo(), container);
                    QLabel* labelDataInizio = new QLabel("📅 Data inizio: " + dataInizio.toString("dd/MM/yyyy"), container);
                    QLabel* labelDataFine = new QLabel("📅 Data fine: " + dataFine.toString("dd/MM/yyyy"), container);

                    QFont boldFont = labelPrestito->font();
                    boldFont.setBold(true);
                    labelPrestito->setFont(boldFont);

                    layout->addWidget(labelPrestito);
                    layout->addWidget(labelUtente);
                    layout->addWidget(labelTitolo);
                    layout->addWidget(labelDataInizio);
                    layout->addWidget(labelDataFine);

                    media->setCopie(media->getCopie() - 1);
                    if (media->getCopie() == 0) media->setDisponibilita(false);
                    gestore->modificaMedia(listaMedia->currentRow(), media->getCopie(), media->getDisponibilita());

                    QListWidgetItem* item = new QListWidgetItem;
                    item->setSizeHint(container->sizeHint());
                    item->setData(Qt::UserRole, media->getId());
                    item->setText(nome + " " + cognome + " " + media->getTitolo() + " " + dataInizio.toString("dd/MM/yyy") + " " + dataFine.toString("dd/MM/yyyy"));
                    listaPrestiti->addItem(item);
                    listaPrestiti->setItemWidget(item, container);
                    mostraInfo();
                    gestore->salvaPrestito(new Prestito(nome, cognome, dataInizio, dataFine, media->getId()));
                    int visibili = 0;
                    for (int i = 0; i < listaPrestiti->count(); ++i) {
                        QListWidgetItem *item = listaPrestiti->item(i);
                        item->setHidden(false);
                        ++visibili;
                    }
                    labelRisultatiPrestiti->setText(QString("Risultati: %1").arg(visibili));
                    dialog->close();
                }
            });

            connect(editNome, &QLineEdit::textChanged, [=]() {
                editNome->setPalette(QPalette());
            });
            connect(editCognome, &QLineEdit::textChanged, [=]() {
                editCognome->setPalette(QPalette());
            });

            QVBoxLayout *layout = new QVBoxLayout(dialog);
            layout->addWidget(labelNome);
            layout->addWidget(editNome);
            layout->addWidget(labelCognome);
            layout->addWidget(editCognome);
            layout->addWidget(labelInizio);
            layout->addWidget(editInizio);
            layout->addWidget(labelFine);
            layout->addWidget(editFine);
            layout->addWidget(okButton);

            dialog->setLayout(layout);
            dialog->exec();
        }
    });

    connect(bottonePrestiti, &QPushButton::clicked, this, &MainWindow::mostraPaginaPrestiti);
}


void MainWindow::creaPaginaForm() {
    QVBoxLayout *layoutPaginaForm = new QVBoxLayout(paginaForm);
    stackPagine->addWidget(paginaForm);

    QWidget *topBar = new QWidget();
    QHBoxLayout *layoutTopBar = new QHBoxLayout(topBar);

    QPushButton *bottoneIndietro = new QPushButton("Torna\nIndietro");

    QShortcut* shortcutIndietro = new QShortcut(QKeySequence("Ctrl+Z"), this);
    connect(shortcutIndietro, &QShortcut::activated, this, [=] {
        if (stackPagine->currentIndex()==1 || stackPagine->currentIndex()==2) {
            mostraPaginaPrincipale();
            selettoreMedia->setCurrentIndex(0);
            pulisciForm();
        }
    });

    bottoneSalva = new QPushButton("Salva\nMedia");

    QShortcut* shortcutSalvaMedia = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(shortcutSalvaMedia, &QShortcut::activated, this, [=] {
        if (!bottoneSalva->isHidden() && (stackPagine->currentIndex()==1)) {
            QString titolo = findChild<QLineEdit*>("campoTitolo")->text();
            if (selettoreMedia->currentIndex() == 0) {
                QMessageBox::warning(nullptr, "Attenzione", "Per favore seleziona un tipo di media.");
                return;
            } else if (titolo == "") {
                QMessageBox::warning(nullptr, "Attenzione", "Per favore inserisci un titolo.");
                return;
            }

            int indiceSelezionato = -1;
            if (listaMedia->currentItem()) {
                indiceSelezionato = listaMedia->row(listaMedia->currentItem());
                gestore->salvaMedia(creaMedia(), indiceSelezionato);
            } else {
                gestore->salvaMedia(creaMedia(), -1);
                indiceSelezionato = listaMedia->count();
            }
            caricaMedia();

            if (indiceSelezionato >= 0 && indiceSelezionato < listaMedia->count()) {
                listaMedia->setCurrentRow(indiceSelezionato);
                mostraInfo();
            }

            stackPagine->setCurrentWidget(paginaPrincipale);
            selettoreMedia->setCurrentIndex(0);
            pulisciForm();
        }
    });

    bottoneIndietro->setIcon(QIcon(":/icone/indietro.png"));
    bottoneIndietro->setIconSize(QSize(36, 36));
    bottoneIndietro->setCursor(Qt::PointingHandCursor);
    bottoneIndietro->setFixedHeight(50);
    bottoneIndietro->setFlat(true);
    bottoneIndietro->setToolTip("Torna alla pagina principale (CTRL+Z)");

    bottoneSalva->setIcon(QIcon(":/icone/salva.png"));
    bottoneSalva->setIconSize(QSize(36, 36));
    bottoneSalva->setCursor(Qt::PointingHandCursor);
    bottoneSalva->setFixedHeight(50);
    bottoneSalva->setFlat(true);
    bottoneSalva->setToolTip("Salva media (CTRL+S)");
    bottoneSalva->hide();

    layoutTopBar->addWidget(bottoneIndietro);
    layoutTopBar->addStretch();
    layoutTopBar->addWidget(bottoneSalva);

    selettoreMedia = new QComboBox();
    selettoreMedia->addItem("Scegli media...");
    selettoreMedia->setItemData(0, 0, Qt::UserRole - 1);
    selettoreMedia->addItems(Gestore::getTipiDisponibili());
    selettoreMedia->setCurrentIndex(0);

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

    connect(selettoreMedia, &QComboBox::currentTextChanged, this, [=]() {
        pulisciForm();
        bottoneSalva->show();
        labelAnteprimaImmagine->show();
        creaForm();
    });
    connect(bottoneIndietro, &QPushButton::clicked, this, [=]() {
        mostraPaginaPrincipale();
        selettoreMedia->setCurrentIndex(0);
        pulisciForm();
        if (listaMedia->currentItem()) mostraInfo();
    });
    connect(bottoneSalva, &QPushButton::clicked, this, [=]() {
        QString titolo = findChild<QLineEdit*>("campoTitolo")->text();
        if (selettoreMedia->currentIndex() == 0) {
            QMessageBox::warning(nullptr, "Attenzione", "Per favore seleziona un tipo di media.");
            return;
        } else if (titolo == "") {
            QMessageBox::warning(nullptr, "Attenzione", "Per favore inserisci un titolo.");
            return;
        }

        int indiceSelezionato = -1;
        if (listaMedia->currentItem()) {
            indiceSelezionato = listaMedia->currentRow();
            gestore->salvaMedia(creaMedia(), indiceSelezionato);
        } else {
            gestore->salvaMedia(creaMedia(), -1);
            indiceSelezionato = listaMedia->count();
        }
        caricaMedia();

        if (indiceSelezionato >= 0 && indiceSelezionato < listaMedia->count()) {
            listaMedia->setCurrentRow(indiceSelezionato);
            mostraInfo();
        }

        stackPagine->setCurrentWidget(paginaPrincipale);
        selettoreMedia->setCurrentIndex(0);
        pulisciForm();
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
        if (stackPagine->currentIndex()==1) {
            pulisciInfo();
            listaMedia->scrollToTop();
        }
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
        pulisciInfo();
        delete gestore;
        gestore = new Gestore(percorsoFile);
        caricaMedia();
        caricaPrestiti();
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

    QWidget *topBar= new QWidget(this);
    QHBoxLayout *layoutTopBar = new QHBoxLayout(topBar);

    QPushButton *bottoneIndietro = new QPushButton("Torna\nIndietro");

    bottoneRestituzione = new QPushButton("Restituisci\nPrestito");

    bottoneIndietro->setIcon(QIcon(":/icone/indietro.png"));
    bottoneIndietro->setIconSize(QSize(36, 36));
    bottoneIndietro->setCursor(Qt::PointingHandCursor);
    bottoneIndietro->setFixedHeight(50);
    bottoneIndietro->setFlat(true);
    bottoneIndietro->setToolTip("Torna alla pagina principale (CTRL+Z)");

    bottoneRestituzione->setIcon(QIcon(":/icone/restituzione.png"));
    bottoneRestituzione->setFlat(true);
    bottoneRestituzione->setCursor(Qt::PointingHandCursor);
    bottoneRestituzione->setIconSize(QSize(48, 48));
    bottoneRestituzione->setToolTip("Restituzione media");
    bottoneRestituzione->hide();

    layoutTopBar->addWidget(bottoneIndietro);
    layoutTopBar->addStretch();
    layoutTopBar->addWidget(bottoneRestituzione);

    QLineEdit *barraRicercaPrestiti = new QLineEdit();
    barraRicercaPrestiti->setPlaceholderText("Cerca prestiti... (CTRL+F)");
    connect(barraRicercaPrestiti, &QLineEdit::textChanged, this, [=](const QString &text) {
        int visibili = 0;
        for (int i = 0; i < listaPrestiti->count(); ++i) {
            QListWidgetItem *item = listaPrestiti->item(i);
            bool match = item->text().contains(text, Qt::CaseInsensitive);
            item->setHidden(!match);
            if (match) ++visibili;
        }
        labelRisultatiPrestiti->setText(QString("Risultati: %1").arg(visibili));
    });

    QShortcut* shortcutRicerca = new QShortcut(QKeySequence("Ctrl+F"), this);
    connect(shortcutRicerca, &QShortcut::activated, this, [=] {
        if (stackPagine->currentIndex()==0) {
            barraRicerca->setFocus();
        } else if (stackPagine->currentIndex()==2) {
            barraRicercaPrestiti->setFocus();
        }
    });

    layoutPaginaPrestiti->addWidget(topBar);
    layoutPaginaPrestiti->addWidget(barraRicercaPrestiti);
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

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Conferma restituzione");
        msgBox.setText("Sei sicuro di voler restituire questo prestito?");
        msgBox.setIcon(QMessageBox::Warning);

        QPushButton* confermaButton = msgBox.addButton(tr("Conferma"), QMessageBox::AcceptRole);
        QPushButton* annullaButton = msgBox.addButton(tr("Annulla"), QMessageBox::RejectRole);

        msgBox.exec();

        if (msgBox.clickedButton() == confermaButton) {
            Media* media = gestore->getMedia(0);
            int indiceMedia = 0;
            for (; media->getId()!=item->data(Qt::UserRole).toInt(); indiceMedia++) {
                media = gestore->getMedia(indiceMedia);

            }
            if (!media->getCopie()) {

                media->setDisponibilita(true);
            }
            media->setCopie(media->getCopie()+1);
            gestore->modificaMedia(indiceMedia, media->getCopie(), media->getDisponibilita());
            delete listaPrestiti->takeItem(indice);
            gestore->eliminaPrestito(indice);

            if (!listaPrestiti->count()) bottoneRestituzione->hide();
            int visibili = 0;
            for (int i = 0; i < listaPrestiti->count(); ++i) {
                QListWidgetItem *item = listaPrestiti->item(i);
                item->setHidden(false);
                ++visibili;
            }
            labelRisultatiPrestiti->setText(QString("Risultati: %1").arg(visibili));
        }
        else if (msgBox.clickedButton() == annullaButton) {
            msgBox.close();
        }
    });
}

void MainWindow::mostraPaginaPrestiti() {
    caricaPrestiti();
    stackPagine->setCurrentWidget(paginaPrestiti);
}

void MainWindow::aggiornaFiltroMedia(const QString& testoRicerca, const QString& categoriaSelezionata, const QString& disponibilitaSelezionata, double minPrezzo, double maxPrezzo) {
    int visibili = 0;


    QListWidgetItem* currentItem = listaMedia->currentItem();

    for (int i = 0; i < listaMedia->count(); ++i) {
        QListWidgetItem *item = listaMedia->item(i);
        Media* media = gestore->getMedia(i);

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

void MainWindow::pulisciForm() {
    while (formLayout->rowCount() > 0) {
        QLayoutItem* labelItem = formLayout->itemAt(0, QFormLayout::LabelRole);
        QLayoutItem* fieldItem = formLayout->itemAt(0, QFormLayout::FieldRole);
        if (labelItem && labelItem->widget()) delete labelItem->widget();
        if (fieldItem && fieldItem->widget()) delete fieldItem->widget();
        formLayout->removeRow(0);
    }
    bottoneSalva->hide();
    labelAnteprimaImmagine->setText("Anteprima Immagine");
    labelAnteprimaImmagine->hide();
}

void MainWindow::mostraInfo() {
    if (listaMedia->currentItem()) {
        Media* media = gestore->getMedia(listaMedia->currentRow());
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
}

void MainWindow::creaForm() {
    QPushButton* caricaImmagine = new QPushButton("Carica Immagine");
    caricaImmagine->setObjectName("Immagine");
    formLayout->addRow("Immagine", caricaImmagine);

    QTextEdit* base64Edit = new QTextEdit();
    base64Edit->setObjectName("campoImmagine");
    base64Edit->setVisible(false);
    formLayout->addRow("campoImmagine", base64Edit);

    QLabel* label = qobject_cast<QLabel*>(formLayout->labelForField(base64Edit));
    if (label) label->setVisible(false);

    QObject::connect(caricaImmagine, &QPushButton::clicked, [=]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Scegli immagine", "", "Immagini (*.png *.jpg *.bmp)");

        if (fileName.isEmpty())
            return;

        QPixmap pixmap(fileName);
        if (pixmap.isNull()) {
            QMessageBox::warning(nullptr, "Errore", "Impossibile caricare l'immagine.");
            return;
        }

        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        bool saved = pixmap.save(&buffer, "PNG");

        if (!saved) {
            QMessageBox::warning(nullptr, "Errore", "Errore nel salvataggio dell'immagine in memoria.");
            return;
        } else {
            labelAnteprimaImmagine->setPixmap(pixmap);
        }

        QString base64 = QString::fromLatin1(byteArray.toBase64());
        base64Edit->setText(base64);
    });

    QLineEdit* campoTitolo = new QLineEdit;
    campoTitolo->setObjectName("campoTitolo");
    formLayout->addRow("Titolo:", campoTitolo);

    QDoubleSpinBox* campoPrezzo = new QDoubleSpinBox();
    campoPrezzo->setObjectName("campoPrezzo");
    campoPrezzo->setMinimum(0.01);
    formLayout->addRow("Prezzo:", campoPrezzo);

    QDateEdit* campoData = new QDateEdit();
    campoData->setObjectName("campoData");
    formLayout->addRow("Data", campoData);

    QLineEdit* campoGenere = new QLineEdit();
    campoGenere->setObjectName("campoGenere");
    formLayout->addRow("Genere", campoGenere);

    QSpinBox* campoCopie = new QSpinBox();
    campoCopie->setRange(0, 999);
    campoCopie->setValue(1);
    campoCopie->setObjectName("campoCopie");
    formLayout->addRow("Copie", campoCopie);

    QString categoria = selettoreMedia->currentText();

    if (categoria == "Film") {
        QSpinBox* campoDurata = new QSpinBox();
        campoDurata->setRange(0, 999);
        campoDurata->setObjectName("campoDurata");
        formLayout->addRow("Durata (min)", campoDurata);

        QLineEdit* campoProduzione = new QLineEdit();
        campoProduzione->setObjectName("campoProduzione");
        formLayout->addRow("Produzione", campoProduzione);

        QLineEdit* campoRegista = new QLineEdit();
        campoRegista->setObjectName("campoRegista");
        formLayout->addRow("Regista", campoRegista);

        QLineEdit* campoLinguaOriginale = new QLineEdit();
        campoLinguaOriginale->setObjectName("campoLinguaOriginale");
        formLayout->addRow("Lingua originale", campoLinguaOriginale);

        QLineEdit* campoPaese = new QLineEdit();
        campoPaese->setObjectName("campoPaeseProduzione");
        formLayout->addRow("Paese di produzione", campoPaese);
    }
    else if (categoria == "Giornale") {
        QLineEdit* campoAutore = new QLineEdit();
        campoAutore->setObjectName("campoAutore");
        formLayout->addRow("Autore", campoAutore);

        QLineEdit* campoEditore = new QLineEdit();
        campoEditore->setObjectName("campoEditore");
        formLayout->addRow("Editore", campoEditore);

        QLineEdit* campoTestata = new QLineEdit();
        campoTestata->setObjectName("campoTestata");
        formLayout->addRow("Testata", campoTestata);
    }
    else if (categoria == "Libro") {
        QLineEdit* campoAutore = new QLineEdit();
        campoAutore->setObjectName("campoAutore");
        formLayout->addRow("Autore", campoAutore);

        QLineEdit* campoEditore = new QLineEdit();
        campoEditore->setObjectName("campoEditore");
        formLayout->addRow("Editore", campoEditore);

        QLineEdit* campoFormato = new QLineEdit();
        campoFormato->setObjectName("campoFormato");
        formLayout->addRow("Formato", campoFormato);

        QLineEdit* campoLingua = new QLineEdit();
        campoLingua->setObjectName("campoLingua");
        formLayout->addRow("Lingua", campoLingua);
    }
    else if (categoria == "Rivista") {
        QLineEdit* campoAutore = new QLineEdit();
        campoAutore->setObjectName("campoAutore");
        formLayout->addRow("Autore", campoAutore);

        QLineEdit* campoEditore = new QLineEdit();
        campoEditore->setObjectName("campoEditore");
        formLayout->addRow("Editore", campoEditore);

        QSpinBox* campoNumero = new QSpinBox();
        campoNumero->setObjectName("campoNumero");
        formLayout->addRow("Numero", campoNumero);

        QComboBox* comboPeriodicita = new QComboBox();
        comboPeriodicita->setObjectName("comboPeriodicita");
        comboPeriodicita->addItems({
            "Seleziona periodicità...",
            "Settimanale", "Mensile", "Trimestrale",
            "Quadrimestrale", "Semestrale", "Annuale"
        });
        comboPeriodicita->setItemData(0, 0, Qt::UserRole - 1);
        comboPeriodicita->setCurrentIndex(-1);
        formLayout->addRow("Periodicità", comboPeriodicita);
    }
    else if (categoria == "Vinile") {
        QSpinBox* campoDurata = new QSpinBox();
        campoDurata->setRange(0, 999);
        campoDurata->setObjectName("campoDurata");
        formLayout->addRow("Durata (min)", campoDurata);

        QLineEdit* campoProduzione = new QLineEdit();
        campoProduzione->setObjectName("campoProduzione");
        formLayout->addRow("Produzione", campoProduzione);

        QLineEdit* campoArtista = new QLineEdit();
        campoArtista->setObjectName("campoArtista");
        formLayout->addRow("Artista", campoArtista);

        QSpinBox* campoTracce = new QSpinBox();
        campoTracce->setObjectName("campoTracce");
        formLayout->addRow("Numero tracce", campoTracce);
    }
}

Media* MainWindow::creaMedia() const {
    QString titolo = findChild<QLineEdit*>("campoTitolo")->text();
    float prezzo = findChild<QDoubleSpinBox*>("campoPrezzo")->value();
    QDate data = findChild<QDateEdit*>("campoData")->date();
    QString genere = findChild<QLineEdit*>("campoGenere")->text();
    int copie = findChild<QSpinBox*>("campoCopie")->value();
    QString immagine = findChild<QTextEdit*>("campoImmagine")->toPlainText();
    bool disponibilita;
    if (copie) disponibilita = true;
    else disponibilita = false;


    int idMedia = 0;
    if (gestore->getQuantitaMedia()) {
        Media* media = gestore->getMedia(gestore->getQuantitaMedia()-1);
        if (listaMedia->currentItem()) idMedia = media->getId();
        else idMedia = media->getId()+1;
    }

    switch(selettoreMedia->currentIndex()) {
    case 1: {
        int durata = findChild<QSpinBox*>("campoDurata")->value();
        QString produzione = findChild<QLineEdit*>("campoProduzione")->text();
        QString regista = findChild<QLineEdit*>("campoRegista")->text();
        QString linguaOriginale = findChild<QLineEdit*>("campoLinguaOriginale")->text();
        QString paeseProduzione = findChild<QLineEdit*>("campoPaeseProduzione")->text();
        return new Film(idMedia, immagine, titolo, prezzo, data, genere, disponibilita, copie, durata, produzione,
                        regista, linguaOriginale, paeseProduzione);
    }
    case 2: {
        QString autore = findChild<QLineEdit*>("campoAutore")->text();
        QString editore = findChild<QLineEdit*>("campoEditore")->text();
        QString testata = findChild<QLineEdit*>("campoTestata")->text();
        return new Giornale(idMedia, immagine, titolo, prezzo, data, genere, disponibilita, copie, autore, editore, testata);
    }
    case 3: {
        QString autore = findChild<QLineEdit*>("campoAutore")->text();
        QString editore = findChild<QLineEdit*>("campoEditore")->text();
        QString formato = findChild<QLineEdit*>("campoFormato")->text();
        QString lingua = findChild<QLineEdit*>("campoLingua")->text();
        return new Libro(idMedia, immagine, titolo, prezzo, data, genere, disponibilita, copie, autore, editore, formato, lingua);
    }
    case 4: {
        QString autore = findChild<QLineEdit*>("campoAutore")->text();
        QString editore = findChild<QLineEdit*>("campoEditore")->text();
        int numero = findChild<QSpinBox*>("campoNumero")->value();
        QString periodicita = findChild<QComboBox*>("comboPeriodicita")->currentText();
        return new Rivista(idMedia, immagine, titolo, prezzo, data, genere, disponibilita, copie, autore, editore, numero, periodicita);
    }
    case 5: {
        int durata = findChild<QSpinBox*>("campoDurata")->value();
        QString produzione = findChild<QLineEdit*>("campoProduzione")->text();
        QString artista = findChild<QLineEdit*>("campoArtista")->text();
        int tracce = findChild<QSpinBox*>("campoTracce")->value();
        return new Vinile(idMedia, immagine, titolo, prezzo, data, genere, disponibilita, copie, durata, produzione, artista, tracce);
    }
    }
}

void MainWindow::caricaMedia() {
    gestore->caricaMedia();

    listaMedia->clear();

    for (int i=0; i<gestore->getQuantitaMedia(); i++) {
        Media* media = gestore->getMedia(i);
        QListWidgetItem* item = new QListWidgetItem(media->getIcon(), media->getTitolo(), listaMedia);

        QFont font;
        font.setBold(true);
        item->setFont(font);

        listaMedia->setIconSize(QSize(48, 48));
    }

    int visibili = 0;
    for (int i = 0; i < listaMedia->count(); ++i) {
        QListWidgetItem *item = listaMedia->item(i);
        item->setHidden(false);
        ++visibili;
    }
    labelRisultatiMedia->setText(QString("Risultati: %1").arg(visibili));
}

void MainWindow::caricaPrestiti() {
    gestore->caricaPrestiti();
    listaPrestiti->clear();

    for (int i=0; i<gestore->getQuantitaPrestiti(); ++i) {
        Media* media = gestore->getMedia(0);
        Prestito* prestito = gestore->getPrestito(i);
        for (int j=0; media->getId()!=prestito->getIdMedia(); ++j) {
            media = gestore->getMedia(j+1);
        }

        QString nome = prestito->getNome();
        QString cognome = prestito->getCognome();
        QDate dataInizio = prestito->getDataInizio();
        QDate dataFine = prestito->getDataFine();

        QWidget* container = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout(container);
        layout->setContentsMargins(10, 6, 10, 6);
        layout->setSpacing(2);

        QLabel* labelPrestito = new QLabel("Prestito");
        QLabel* labelUtente = new QLabel("🧑 Richiedente: " + nome + " " + cognome);
        QLabel* labelTitolo = new QLabel("📘 Titolo media: " + media->getTitolo());
        QLabel* labelDataInizio = new QLabel("📅 Data inizio: " + dataInizio.toString("dd/MM/yyyy"));
        QLabel* labelDataFine = new QLabel("📅 Data fine: " + dataFine.toString("dd/MM/yyyy"));

        QFont boldFont = labelPrestito->font();
        boldFont.setBold(true);
        labelPrestito->setFont(boldFont);

        layout->addWidget(labelPrestito);
        layout->addWidget(labelUtente);
        layout->addWidget(labelTitolo);
        layout->addWidget(labelDataInizio);
        layout->addWidget(labelDataFine);

        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(container->sizeHint());
        item->setData(Qt::UserRole, prestito->getIdMedia());
        item->setText(nome + " " + cognome + " " + media->getTitolo() + " " + dataInizio.toString("dd/MM/yyy") + " " + dataFine.toString("dd/MM/yyyy"));
        listaPrestiti->addItem(item);
        listaPrestiti->setItemWidget(item, container);
    }

    int visibili = 0;
    for (int i = 0; i < listaPrestiti->count(); ++i) {
        QListWidgetItem *item = listaPrestiti->item(i);
        item->setHidden(false);
        ++visibili;
    }
    labelRisultatiPrestiti->setText(QString("Risultati: %1").arg(visibili));
}

void MainWindow::caricaForm() {
    Media* media = gestore->getMedia(listaMedia->currentRow());

    if (dynamic_cast<Film*>(media)) selettoreMedia->setCurrentIndex(1);
    else if (dynamic_cast<Giornale*>(media)) selettoreMedia->setCurrentIndex(2);
    else if (dynamic_cast<Libro*>(media)) selettoreMedia->setCurrentIndex(3);
    else if (dynamic_cast<Rivista*>(media)) selettoreMedia->setCurrentIndex(4);
    else if (dynamic_cast<Vinile*>(media)) selettoreMedia->setCurrentIndex(5);

    QString base64 = media->getImmagine();
    QByteArray byteArray = QByteArray::fromBase64(base64.toLatin1());

    QPixmap pixmap;
    if (pixmap.loadFromData(byteArray)) {
        labelAnteprimaImmagine->setPixmap(pixmap.scaled(500, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        labelAnteprimaImmagine->show();
    }
    else {
        labelAnteprimaImmagine->show();
        labelAnteprimaImmagine->clear();
        labelAnteprimaImmagine->setText("Anteprima Immagine");
    }

    if (QTextEdit* base64Edit = findChild<QTextEdit*>("campoImmagine"))
        base64Edit->setText(media->getImmagine());

    if (QLineEdit* titolo = findChild<QLineEdit*>("campoTitolo"))
        titolo->setText(media->getTitolo());

    if (QDoubleSpinBox* prezzo = findChild<QDoubleSpinBox*>("campoPrezzo"))
        prezzo->setValue(media->getPrezzo());

    if (QDateEdit* data = findChild<QDateEdit*>("campoData"))
        data->setDate(media->getData());

    if (QLineEdit* genere = findChild<QLineEdit*>("campoGenere"))
        genere->setText(media->getGenere());

    if (QSpinBox* copie = findChild<QSpinBox*>("campoCopie"))
        copie->setValue(media->getCopie());

    if (Film* film = dynamic_cast<Film*>(media)) {
        if (QSpinBox* durata = findChild<QSpinBox*>("campoDurata"))
            durata->setValue(film->getDurata());
        if (QLineEdit* produzione = findChild<QLineEdit*>("campoProduzione"))
            produzione->setText(film->getProduzione());
        if (QLineEdit* regista = findChild<QLineEdit*>("campoRegista"))
            regista->setText(film->getRegista());
        if (QLineEdit* lingua = findChild<QLineEdit*>("campoLinguaOriginale"))
            lingua->setText(film->getLinguaOriginale());
        if (QLineEdit* paese = findChild<QLineEdit*>("campoPaeseProduzione"))
            paese->setText(film->getPaeseProduzione());
    }
    else if (Giornale* giornale = dynamic_cast<Giornale*>(media)) {
        if (QLineEdit* autore = findChild<QLineEdit*>("campoAutore"))
            autore->setText(giornale->getAutore());
        if (QLineEdit* editore = findChild<QLineEdit*>("campoEditore"))
            editore->setText(giornale->getEditore());
        if (QLineEdit* testata = findChild<QLineEdit*>("campoTestata"))
            testata->setText(giornale->getTestata());
    }
    else if (Libro* libro = dynamic_cast<Libro*>(media)) {
        if (QLineEdit* autore = findChild<QLineEdit*>("campoAutore"))
            autore->setText(libro->getAutore());
        if (QLineEdit* editore = findChild<QLineEdit*>("campoEditore"))
            editore->setText(libro->getEditore());
        if (QLineEdit* formato = findChild<QLineEdit*>("campoFormato"))
            formato->setText(libro->getFormato());
        if (QLineEdit* lingua = findChild<QLineEdit*>("campoLingua"))
            lingua->setText(libro->getLingua());
    }
    else if (Rivista* rivista = dynamic_cast<Rivista*>(media)) {
        if (QLineEdit* autore = findChild<QLineEdit*>("campoAutore"))
            autore->setText(rivista->getAutore());
        if (QLineEdit* editore = findChild<QLineEdit*>("campoEditore"))
            editore->setText(rivista->getEditore());
        if (QSpinBox* numero = findChild<QSpinBox*>("campoNumero"))
            numero->setValue(rivista->getNumero());
        if (QComboBox* periodicita = findChild<QComboBox*>("comboPeriodicita")) {
            int index = periodicita->findText(rivista->getPeriodicita());
            if (index != -1)
                periodicita->setCurrentIndex(index);
        }
    }
    else if (Vinile* vinile = dynamic_cast<Vinile*>(media)) {
        if (QSpinBox* durata = findChild<QSpinBox*>("campoDurata"))
            durata->setValue(vinile->getDurata());
        if (QLineEdit* produzione = findChild<QLineEdit*>("campoProduzione"))
            produzione->setText(vinile->getProduzione());
        if (QLineEdit* artista = findChild<QLineEdit*>("campoArtista"))
            artista->setText(vinile->getArtista());
        if (QSpinBox* tracce = findChild<QSpinBox*>("campoTracce"))
            tracce->setValue(vinile->getNumeroTracce());
    }
}
