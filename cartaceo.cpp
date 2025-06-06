#include "cartaceo.h"

Cartaceo::Cartaceo(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere, bool disponibilita,
                   int copie, QString autore, QString editore):
    Media(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie), autore(autore), editore(editore) {

}

Cartaceo::~Cartaceo() {}

QString Cartaceo::getAutore() const {
    return autore;
}

QString Cartaceo::getEditore() const {
    return editore;
}
