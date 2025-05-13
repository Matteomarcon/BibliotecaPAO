#ifndef LIBRO_H
#define LIBRO_H

#include <QString>

class libro {
private:
    QString titolo;
    QString autore;
    QString editore;
    QString descrizione;

public:
    libro(const QString& titolo, const QString& autore, const QString& editore, const QString& descrizione);

    QString getTitolo() const;
    QString getAutore() const;
    QString getEditore() const;
    QString getDescrizione() const;
};

#endif // LIBRO_H
