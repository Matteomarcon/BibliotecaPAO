#include "media.h"
using std::string;

Media::Media(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie):
    immagine(immagine), titolo(titolo), prezzo(prezzo), dataPubblicazione(dataPubblicazione), genere(genere), disponibilita(disponibilita), copie(copie){

}

QImage Media::getImmagine() const {
    return immagine;
}
string Media::getTitolo() const {
    return titolo;
}
float Media::getPrezzo() const {
    return prezzo;
}
Data Media::getData() const {
    return dataPubblicazione;
}
string Media::getGenere() const {
    return genere;
}
bool Media::getDisponibilita() const {
    return disponibilita;
}
int Media::getCopie() const {
    return copie;
}
