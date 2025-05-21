#ifndef MEDIA_H
#define MEDIA_H
#include <string>
#include <QImage>
#include "Data.h"
using std::string;

class Media {
private:
    QImage immagine;
    string titolo;
    float prezzo;
    Data dataPubblicazione;
    string genere;
    bool disponibilita;
    int copie;
public:
    virtual ~Media() {}

    Media(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie);
    QImage getImmagine() const;
    string getTitolo() const;
    float getPrezzo() const;
    Data getData() const;
    string getGenere() const;
    bool getDisponibilita() const;
    int getCopie() const;
};

#endif // MEDIA_H
