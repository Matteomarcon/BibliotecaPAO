#include "audiovisivo.h"

audiovisivo::audiovisivo(QImage immagine, string titolo, float prezzo, Data dataPubblicazione,
                                     string genere, bool disponibilita, int copie, int durata, string produzione):
    Media(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie), durata(durata), produzione(produzione){

}

int audiovisivo::getDurata() const {
    return durata;
}

string audiovisivo::getProduzione() const {
    return produzione;
}

