#ifndef PRESTITO_H
#define PRESTITO_H

#include <QDate>

class Prestito {
private:
    QString nome;
    QString cognome;
    QDate dataInizioPrestito;
    QDate dataFinePrestito;
    int idMedia;
public:
    Prestito(QString nome, QString cognome, QDate dataInizioPrestito, QDate dataFinePrestito, int idMedia);

    QString getNome() const;
    QString getCognome() const;
    QDate getDataInizio() const;
    QDate getDataFine() const;
    int getIdMedia() const;
};

#endif // PRESTITO_H
