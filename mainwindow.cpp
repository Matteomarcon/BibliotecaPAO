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


    layoutNuovoMedia->addStretch(1);

    // Pulsante salva
    QPushButton *salvaNuovoMediaButton = new QPushButton("Salva");
    layoutNuovoMedia->addWidget(salvaNuovoMediaButton);

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
        else gestore->salvaMediaDaForm(selettoreMedia->currentText(), formLayout);
    });
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

        QString base64 = item->data(Qt::UserRole).toString();
        QByteArray byteArray = QByteArray::fromBase64(base64.toLatin1());

        qDebug() << "Base64 length:" << base64.size();
        qDebug() << "Base64 head:" << base64.left(50);

        QPixmap pixmap;
        if (pixmap.loadFromData(byteArray)) {
            labelImmagine->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }

    }
}
