#include "prestito.h"

Prestito::Prestito(QString nome, QString cognome, QDate dataInizioPrestito, QDate dataFinePrestito, int idMedia):
    nome(nome), cognome(cognome), dataInizioPrestito(dataInizioPrestito), dataFinePrestito(dataFinePrestito), idMedia(idMedia) {}

QString Prestito::getNome() const {
    return nome;
}
QString Prestito::getCognome() const {
    return cognome;
}
QDate Prestito::getDataInizio() const {
    return dataInizioPrestito;
}
QDate Prestito::getDataFine() const {
    return dataFinePrestito;
}
int Prestito::getIdMedia() const {
    return idMedia;
}
