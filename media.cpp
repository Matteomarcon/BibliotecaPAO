#include "media.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QFileDialog>
#include <QBuffer>
#include <QLabel>
#include <QMessageBox>
#include <QTextEdit>

using std::string;

Media::Media(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere, bool disponibilita, int copie):
    immagine(immagine), titolo(titolo), prezzo(prezzo), dataPubblicazione(dataPubblicazione), genere(genere), disponibilita(disponibilita), copie(copie){

}

Media::Media() {}

Media::~Media() {}

QString Media::getImmagine() const {
    return immagine;
}
QString Media::getTitolo() const {
    return titolo;
}
float Media::getPrezzo() const {
    return prezzo;
}
Data Media::getData() const {
    return dataPubblicazione;
}
QString Media::getGenere() const {
    return genere;
}
bool Media::getDisponibilita() const {
    return disponibilita;
}
int Media::getCopie() const {
    return copie;
}

void Media::creaForm(QFormLayout* layout) {
    QPushButton* caricaImmagine = new QPushButton("Carica Immagine");
    caricaImmagine->setObjectName("BottoneImmagine");
    layout->addRow("Immagine", caricaImmagine);

    QTextEdit* base64Edit = new QTextEdit();
    base64Edit->setObjectName("Immagine");
    base64Edit->setVisible(false);
    layout->addRow("Immagine", base64Edit);

    QLabel* label = qobject_cast<QLabel*>(layout->labelForField(base64Edit));
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
        }

        QString base64 = QString::fromLatin1(byteArray.toBase64());
        base64Edit->setText(base64);
    });
    QLineEdit* titoloEdit = new QLineEdit();
    titoloEdit->setObjectName("titoloEdit");
    layout->addRow("Titolo", new QLineEdit());

    QDoubleSpinBox* prezzoSpin = new QDoubleSpinBox();
    prezzoSpin->setObjectName("prezzoSpin");
    layout->addRow("Prezzo", prezzoSpin);

    QDateEdit* dataEdit = new QDateEdit();
    dataEdit->setObjectName("dataEdit");
    layout->addRow("Data", dataEdit);

    QLineEdit* genereEdit = new QLineEdit();
    genereEdit->setObjectName("genereEdit");
    layout->addRow("Genere", genereEdit);

    QCheckBox* disponibilitaCheck = new QCheckBox("Disponibile");
    disponibilitaCheck->setObjectName("disponibilitaCheck");
    layout->addRow("Disponibilita", disponibilitaCheck);

    QSpinBox* copieSpin = new QSpinBox();
    copieSpin->setObjectName("copieSpin");
    layout->addRow("Copie", copieSpin);


}
