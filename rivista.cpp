#include "rivista.h"

rivista::rivista(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere,
                 bool disponibilita, int copie, string autore, string editore, int numero, periodicita tipoPeriodicita):
    cartaceo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, autore, editore),
    numero(numero), tipoPeriodicita(tipoPeriodicita){

}

int rivista::getNumero() const {
    return numero;
}
rivista::periodicita rivista::getPeriodicita() const {
    return tipoPeriodicita;
}
