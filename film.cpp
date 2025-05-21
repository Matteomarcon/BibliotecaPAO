#include "film.h"

Film::Film(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, int durata, string produzione,
           string regista, string linguaOriginale, string paeseProduzione):
    Audiovisivo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, durata, produzione),
    regista(regista), linguaOriginale(linguaOriginale), paeseProduzione(paeseProduzione) {

}

string Film::getRegista() const {
    return regista;
}
string Film::getLinguaOriginale() const {
    return linguaOriginale;
}
string Film::getPaeseProduzione() const {
    return paeseProduzione;
}
