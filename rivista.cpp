#include "rivista.h"
#include <QSpinBox>
#include <QComboBox>

Rivista::Rivista(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere,
                 bool disponibilita, int copie, QString autore, QString editore, int numero, QString periodicita):
    Cartaceo(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie, autore, editore),
    numero(numero), periodicita(periodicita){

}

int Rivista::getNumero() const {
    return numero;
}
QString Rivista::getPeriodicita() const {
    return periodicita;
}

QIcon Rivista::getIcon() const {
    QIcon icona(":/icone/magazine.png");
    return icona;
}

void Rivista::creaForm(QFormLayout* layout) {
    Cartaceo::creaForm(layout);
    layout->addRow("Numero", new QSpinBox());
    QComboBox* periodicita = new QComboBox();
    periodicita->addItems({"Seleziona periocità...", "Settimanale", "Mensile", "Trimestrale", "Quadrimestrale", "Semestrale", "Annuale"});
    periodicita->setItemData(0, 0, Qt::UserRole - 1); //Disabilita la voce
    periodicita->setCurrentIndex(-1);
    layout->addRow("Periodicità", periodicita);
}

