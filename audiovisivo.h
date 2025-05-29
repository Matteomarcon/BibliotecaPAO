#ifndef AUDIOVISIVO_H
#define AUDIOVISIVO_H
#include "media.h"
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>

class Audiovisivo: public Media {
private:
    int durata;
    QString produzione;
public:
    virtual ~Audiovisivo();

    Audiovisivo(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere, bool disponibilita, int copie, int durata, QString produzione);
    Audiovisivo();
    int getDurata() const;
    QString getProduzione() const;

    virtual QIcon getIcon() const = 0;
    static void creaForm(QFormLayout* layout);
};

#endif // AUDIOVISIVO_H
