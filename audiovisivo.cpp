#include "audiovisivo.h"

Audiovisivo::Audiovisivo(QString immagine, QString titolo, float prezzo, Data dataPubblicazione,
                                     QString genere, bool disponibilita, int copie, int durata, QString produzione):
    Media(immagine, titolo, prezzo, dataPubblicazione, genere, disponibilita, copie), durata(durata), produzione(produzione){

}

Audiovisivo::Audiovisivo(): Media() {}

Audiovisivo::~Audiovisivo() {}

int Audiovisivo::getDurata() const {
    return durata;
}

QString Audiovisivo::getProduzione() const {
    return produzione;
}

void Audiovisivo::creaForm(QFormLayout* layout) {
    Media::creaForm(layout);
    layout->addRow("Durata (min)", new QSpinBox());
    layout->addRow("Produzione", new QLineEdit());
}
