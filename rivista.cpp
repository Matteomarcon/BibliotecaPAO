#include "rivista.h"
#include <QSpinBox>

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
    QIcon icona(":/icone/magazine.png");
    return icona;
}

void Rivista::creaForm(QFormLayout* layout) {
    Cartaceo::creaForm(layout);
    layout->addRow("Numero", new QSpinBox());
    layout->addRow("Periodicita", new QLineEdit());
}

