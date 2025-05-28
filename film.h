#ifndef FILM_H
#define FILM_H
#include "audiovisivo.h"

class Film: public Audiovisivo {
private:
    string regista;
    string linguaOriginale;
    string paeseProduzione;
public:
    Film(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, int durata, string produzione,
string regista, string linguaOriginale, string paeseProduzione);
    Film();
    string getRegista() const;
    string getLinguaOriginale() const;
    string getPaeseProduzione() const;

    QIcon getIcon() const override;
    static void creaForm(QFormLayout *layout);
};

#endif // FILM_H
