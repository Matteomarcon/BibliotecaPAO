#include "cartaceo.h"

Cartaceo::Cartaceo(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita,
                   int copie, string autore, string editore):
    Media(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie), autore(autore), editore(editore) {

}

string Cartaceo::getAutore() const {
    return autore;
}

string Cartaceo::getEditore() const {
    return editore;
}
