#ifndef LIBRO_H
#define LIBRO_H
#include "cartaceo.h"
#include <QString>

class Libro: public Cartaceo {
private:
    QString formato;
    QString lingua;
public:
    Libro(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere, bool disponibilita,
          int copie, QString autore, QString editore, QString lingua, QString formato);
    QString getFormato() const;
    QString getLingua() const;

    QIcon getIcon() const override;
    static void creaForm(QFormLayout *layout);
};

#endif // LIBRO_H
