#include "giornale.h"

Giornale::Giornale(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere, bool disponibilita, int copie, QString autore, QString editore,
                   QString testata):
    Cartaceo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, autore, editore), testata(testata) {

}

QString Giornale::getTestata() const {
    return testata;
}

QIcon Giornale::getIcon() const {
    QIcon icona(":/icone/newspaper.png");
    return icona;
}

void Giornale::creaForm(QFormLayout* layout) {
    Cartaceo::creaForm(layout);
    layout->addRow("Testata", new QLineEdit());
}
