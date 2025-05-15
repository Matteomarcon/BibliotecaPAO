#include "libro.h"

libro::libro(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita,
             int copie, string autore, string editore, string lingua, formato tipoFormato) :
    cartaceo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita,
               copie, autore, editore), lingua(lingua), tipoFormato(tipoFormato) {

}

string libro::getLingua() const {
    return lingua;
}

libro::formato libro::getFormato() const {
    return tipoFormato;
}
