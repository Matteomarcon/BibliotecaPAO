#include "giornale.h"

Giornale::Giornale(int idMedia, QString immagine, QString titolo, float prezzo, QDate dataPubblicazione, QString genere, bool disponibilita, int copie, QString autore, QString editore,
                   QString testata):
    Cartaceo(idMedia, immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, autore, editore), testata(testata) {

}

QString Giornale::getTestata() const {
    return testata;
}

QIcon Giornale::getIcon() const {
    QIcon icona(":/icone/newspaper.png");
    return icona;
}
