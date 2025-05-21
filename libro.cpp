#include "libro.h"

Libro::Libro(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita,
             int copie, string autore, string editore, string lingua, formato tipoFormato) :
    Cartaceo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita,
               copie, autore, editore), lingua(lingua), tipoFormato(tipoFormato) {

}

string Libro::getLingua() const {
    return lingua;
}

Libro::formato Libro::getFormato() const {
    return tipoFormato;
}
