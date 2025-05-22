#include "audiovisivo.h"

Audiovisivo::Audiovisivo(QImage immagine, string titolo, float prezzo, Data dataPubblicazione,
                                     string genere, bool disponibilita, int copie, int durata, string produzione):
    Media(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie), durata(durata), produzione(produzione){

}

Audiovisivo::Audiovisivo(): Media() {}

Audiovisivo::~Audiovisivo() {}

int Audiovisivo::getDurata() const {
    return durata;
}

string Audiovisivo::getProduzione() const {
    return produzione;
}

