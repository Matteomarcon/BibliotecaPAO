#include "rivista.h"

Rivista::Rivista(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere,
                 bool disponibilita, int copie, string autore, string editore, int numero, string periodicita):
    Cartaceo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, autore, editore),
    numero(numero), periodicita(periodicita){

}

int Rivista::getNumero() const {
    return numero;
}
string Rivista::getPeriodicita() const {
    return periodicita;
}

QIcon Rivista::getIcon() const {
    QIcon icona("C:/Users/javie/Desktop/magazine.png");
    return icona;
}
