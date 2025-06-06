#include "libro.h"

Libro::Libro(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere, bool disponibilita,
             int copie, QString autore, QString editore, QString lingua, QString formato) :
    Cartaceo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita,
               copie, autore, editore), formato(formato), lingua(lingua) {

}

QString Libro::getLingua() const {
    return lingua;
}

QString Libro::getFormato() const {
    return formato;
}

QIcon Libro::getIcon() const {
    QIcon icona(":/icone/book.png");
    return icona;
}
