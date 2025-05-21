#include "rivista.h"

Rivista::Rivista(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere,
                 bool disponibilita, int copie, string autore, string editore, int numero, periodicita tipoPeriodicita):
    Cartaceo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, autore, editore),
    numero(numero), tipoPeriodicita(tipoPeriodicita){

}

int Rivista::getNumero() const {
    return numero;
}
Rivista::periodicita Rivista::getPeriodicita() const {
    return tipoPeriodicita;
}
