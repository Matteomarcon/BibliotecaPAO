#ifndef FILM_H
#define FILM_H
#include "audiovisivo.h"

class Film: public Audiovisivo {
private:
    QString regista;
    QString linguaOriginale;
    QString paeseProduzione;
public:
    Film(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere, bool disponibilita, int copie, int durata, QString produzione,
QString regista, QString linguaOriginale, QString paeseProduzione);
    Film();
    QString getRegista() const;
    QString getLinguaOriginale() const;
    QString getPaeseProduzione() const;

    QIcon getIcon() const override;
    static void creaForm(QFormLayout *layout);
};

#endif // FILM_H
