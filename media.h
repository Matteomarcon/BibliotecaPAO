#ifndef MEDIA_H
#define MEDIA_H
#include <QIcon>
#include <QFormLayout>
#include "Data.h"

class Media {
private:
    QString immagine;
    QString titolo;
    float prezzo;
    Data dataPubblicazione;
    QString genere;
    bool disponibilita;
    int copie;
public:
    virtual ~Media();

    Media(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere, bool disponibilita, int copie);
    Media();
    QString getImmagine() const;
    QString getTitolo() const;
    float getPrezzo() const;
    QString getData() const;
    QString getGenere() const;
    bool getDisponibilita() const;
    int getCopie() const;

    virtual QIcon getIcon() const = 0;
};

#endif // MEDIA_H
