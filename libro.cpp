#include "libro.h"

Libro::Libro(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita,
             int copie, string autore, string editore, string lingua, string formato) :
    Cartaceo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita,
               copie, autore, editore), formato(formato), lingua(lingua) {

}

string Libro::getLingua() const {
    return lingua;
}

string Libro::getFormato() const {
    return formato;
}

QIcon Libro::getIcon() const {
    QIcon icona(":/icone/book.png");
    return icona;
}
