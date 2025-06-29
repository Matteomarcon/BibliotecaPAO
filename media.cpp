#include "media.h"

Media::Media(int idMedia, QString immagine, QString titolo, float prezzo, QDate dataPubblicazione, QString genere, bool disponibilita, int copie):
    idMedia(idMedia), immagine(immagine), titolo(titolo), prezzo(prezzo), dataPubblicazione(dataPubblicazione), genere(genere), disponibilita(disponibilita), copie(copie){}

Media::~Media() {}

int Media::getId() const {
    return idMedia;
}

QString Media::getImmagine() const {
    return immagine;
}
QString Media::getTitolo() const {
    return titolo;
}
float Media::getPrezzo() const {
    return prezzo;
}
QDate Media::getData() const {
    return dataPubblicazione;
}
QString Media::getGenere() const {
    return genere;
}
bool Media::getDisponibilita() const {
    return disponibilita;
}
int Media::getCopie() const {

    return copie;
}
void Media::setCopie(int copie) {
    this->copie=copie;
}
void Media::setDisponibilita(bool disponibilita) {
    this->disponibilita=disponibilita;
}
