#include "film.h"

Film::Film(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, int durata, string produzione,
           string regista, string linguaOriginale, string paeseProduzione):
    Audiovisivo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, durata, produzione),
    regista(regista), linguaOriginale(linguaOriginale), paeseProduzione(paeseProduzione) {

}

Film::Film(): Audiovisivo() {}

string Film::getRegista() const {
    return regista;
}
string Film::getLinguaOriginale() const {
    return linguaOriginale;
}
string Film::getPaeseProduzione() const {
    return paeseProduzione;
}

QIcon Film::getIcon() const {
    QIcon icona(":/icone/film.png");
    return icona;
}

void Film::creaForm(QFormLayout* layout) {
    Audiovisivo::creaForm(layout);
    layout->addRow("Regista", new QLineEdit());
    layout->addRow("Lingua Originale", new QLineEdit());
    layout->addRow("Paese Produzione", new QLineEdit());
}
