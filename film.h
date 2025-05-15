#ifndef FILM_H
#define FILM_H
#include "audiovisivo.h"

class Film: public audiovisivo {
private:
    string regista;
    string linguaOriginale;
    string paeseProduzione;
public:
    Film(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, int durata, string produzione,
string regista, string linguaOriginale, string paeseProduzione);
    virtual ~Film();
    string getRegista() const;
    string getLinguaOriginale() const;
    string getPaeseProduzione() const;
};

#endif // FILM_H
