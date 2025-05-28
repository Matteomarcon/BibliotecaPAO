#include "media.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QCheckBox>

using std::string;

Media::Media(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie):
    immagine(immagine), titolo(titolo), prezzo(prezzo), dataPubblicazione(dataPubblicazione), genere(genere), disponibilita(disponibilita), copie(copie){

}

Media::Media() {}

Media::~Media() {}

QImage Media::getImmagine() const {
    return immagine;
}
string Media::getTitolo() const {
    return titolo;
}
float Media::getPrezzo() const {
    return prezzo;
}
Data Media::getData() const {
    return dataPubblicazione;
}
string Media::getGenere() const {
    return genere;
}
bool Media::getDisponibilita() const {
    return disponibilita;
}
int Media::getCopie() const {
    return copie;
}

void Media::creaForm(QFormLayout* layout) {
    layout->addRow("Immagine", new QPushButton("Carica immagine"));
    layout->addRow("Titolo", new QLineEdit());
    layout->addRow("Prezzo", new QDoubleSpinBox());
    layout->addRow("Data", new QDateEdit());
    layout->addRow("Genere", new QLineEdit());
    layout->addRow("Disponibilita", new QCheckBox("Disponibile"));
    layout->addRow("Copie", new QSpinBox());

}
