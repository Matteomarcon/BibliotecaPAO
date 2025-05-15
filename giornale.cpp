#include "giornale.h"

Giornale::Giornale(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, string autore, string editore,
                   string testata):
    cartaceo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, autore, editore), testata(testata) {

}

string Giornale::getTestata() const {
    return testata;
}
