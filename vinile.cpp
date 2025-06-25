#include "vinile.h"

Vinile::Vinile(int idMedia, QString immagine, QString titolo, float prezzo, QDate dataPubblicazione, QString genere, bool disponibilita, int copie, int durata, QString produzione,
               QString artista, int numeroTracce):
    Audiovisivo(idMedia, immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, durata, produzione),
    artista(artista), numeroTracce(numeroTracce){

}

QString Vinile::getArtista() const {
    return artista;
}

int Vinile::getNumeroTracce() const {
    return numeroTracce;
}

QIcon Vinile::getIcon() const {
    QIcon icona(":/icone/record.png");
    return icona;
}
