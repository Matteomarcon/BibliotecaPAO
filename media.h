#ifndef MEDIA_H
#define MEDIA_H

#include <QDate>
#include <QIcon>

class Media {
private:
    int idMedia;
    QString immagine;
    QString titolo;
    float prezzo;
    QDate dataPubblicazione;
    QString genere;
    bool disponibilita;
    int copie;
public:
    virtual ~Media();

    Media(int idMedia, QString immagine, QString titolo, float prezzo, QDate dataPubblicazione, QString genere, bool disponibilita, int copie);
    int getId() const;
    QString getImmagine() const;
    QString getTitolo() const;
    float getPrezzo() const;
    QDate getData() const;
    QString getGenere() const;
    bool getDisponibilita() const;
    int getCopie() const;
    void setCopie(int copie);
    void setDisponibilita(bool disponibilita);

    virtual QIcon getIcon() const = 0;
};

#endif // MEDIA_H
