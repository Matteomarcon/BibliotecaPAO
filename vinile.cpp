#include "vinile.h"

Vinile::Vinile(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, int durata, string produzione,
               string artista, int numeroTracce):
    Audiovisivo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, durata, produzione),
    artista(artista), numeroTracce(numeroTracce){

}

string Vinile::getArtista() const {
    return artista;
}

int Vinile::getNumeroTracce() const {
    return numeroTracce;
}

QIcon Vinile::getIcon() const {
    QIcon icona("C:/Users/javie/Desktop/record.png");
    return icona;
}
