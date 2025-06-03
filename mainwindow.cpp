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

    gestore = new GestoreMedia(listaMedia, "dati.json");
    gestore->caricaBiblioteca();

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
    labelImmagine = new QLabel("Immagine:", rightPanel);

    rightLayout->addWidget(labelTitolo);
    rightLayout->addWidget(labelDescrizione);
    rightLayout->addWidget(labelImmagine);
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
    connect(toPrincipale, &QPushButton::clicked, this, &MainWindow::showPaginaPrincipale);
    connect(toNuovoMedia, &QPushButton::clicked, this, &MainWindow::showPaginaNuovoMedia);
    connect(salvaNuovoMediaButton, &QPushButton::clicked, this, [=]() {
        if (selettoreMedia->currentIndex() == 0) QMessageBox::warning(nullptr, "Attenzione", "Per favore seleziona un tipo di media.");
        else  {
            gestore->salvaMediaDaForm(selettoreMedia->currentText(), formLayout);
            gestore->caricaBiblioteca();
        }
    });
}

MainWindow::~MainWindow() {}

void MainWindow::showPaginaPrincipale() {
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
        stackedWidget->setCurrentWidget(paginaPrincipale); // da implementare: senza salvare
    } else if (msgBox.clickedButton() == annullaButton) {
        msgBox.close();
    }
}

void MainWindow::showPaginaNuovoMedia() {
    stackedWidget->setCurrentWidget(paginaNuovoMedia);
}

void MainWindow::onItemClicked(QListWidgetItem *item) {
    if (item) {
        labelTitolo->setText(item->text());

        QString base64 = item->data(Qt::UserRole).toString();
        QByteArray byteArray = QByteArray::fromBase64(base64.toLatin1());

        qDebug() << "Base64 length:" << base64.size();
        qDebug() << "Base64 head:" << base64.left(50);

        QPixmap pixmap;
        if (pixmap.loadFromData(byteArray)) {
            labelImmagine->setPixmap(pixmap.scaled(500, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        else {
            labelImmagine->clear();
            labelImmagine->setText("Immagine:");
        }
    }
}
