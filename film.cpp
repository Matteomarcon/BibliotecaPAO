#include "film.h"

Film::Film(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere, bool disponibilita, int copie, int durata, QString produzione,
           QString regista, QString linguaOriginale, QString paeseProduzione):
    Audiovisivo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, durata, produzione),
    regista(regista), linguaOriginale(linguaOriginale), paeseProduzione(paeseProduzione) {

}

Film::Film(): Audiovisivo() {}

QString Film::getRegista() const {
    return regista;
}
QString Film::getLinguaOriginale() const {
    return linguaOriginale;
}
QString Film::getPaeseProduzione() const {
    return paeseProduzione;
}

QIcon Film::getIcon() const {
    QIcon icona(":/icone/film.png");
    return icona;
}
