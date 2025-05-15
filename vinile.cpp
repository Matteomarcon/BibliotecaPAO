#include "vinile.h"

vinile::vinile(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, int durata, string produzione,
               string artista, int numeroTracce):
    audiovisivo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, durata, produzione),
    artista(artista), numeroTracce(numeroTracce){

}

string vinile::getArtista() const {
    return artista;
}

int vinile::getNumeroTracce() const {
    return numeroTracce;
}
