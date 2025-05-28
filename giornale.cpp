#include "giornale.h"

Giornale::Giornale(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, string autore, string editore,
                   string testata):
    Cartaceo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, autore, editore), testata(testata) {

}

string Giornale::getTestata() const {
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
