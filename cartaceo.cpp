#include "cartaceo.h"

cartaceo::cartaceo(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita,
                   int copie, string autore, string editore):
    Media(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie), autore(autore), editore(editore) {

}

string cartaceo::getAutore() const {
    return autore;
}

string cartaceo::getEditore() const {
    return editore;
}
