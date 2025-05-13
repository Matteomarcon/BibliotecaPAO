#include "libro.h"

libro::libro(const QString& titolo, const QString& autore, const QString& editore, const QString& descrizione) : titolo(titolo), autore(autore), editore(editore), descrizione(descrizione) {

}

QString libro::getTitolo() const {
    return titolo;
}
QString libro::getAutore() const {
    return autore;
}
QString libro::getEditore() const {
    return editore;
}
QString libro::getDescrizione() const {
    return descrizione;
}
