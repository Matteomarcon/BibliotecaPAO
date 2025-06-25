#include "audiovisivo.h"

Audiovisivo::Audiovisivo(int idMedia, QString immagine, QString titolo, float prezzo, QDate dataPubblicazione,
                                     QString genere, bool disponibilita, int copie, int durata, QString produzione):
    Media(idMedia, immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie), durata(durata), produzione(produzione){}

Audiovisivo::Audiovisivo(): Media() {}

Audiovisivo::~Audiovisivo() {}

int Audiovisivo::getDurata() const {
    return durata;
}

QString Audiovisivo::getProduzione() const {
    return produzione;
}
