#ifndef VINILE_H
#define VINILE_H
#include "audiovisivo.h"

class Vinile: public Audiovisivo {
private:
    QString artista;
    int numeroTracce;
public:
    Vinile(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere, bool disponibilita, int copie, int durata, QString produzione,
QString artista, int numeroTracce);
    QString getArtista() const;
    int getNumeroTracce() const;

    QIcon getIcon() const override;
};

#endif // VINILE_H
