#ifndef GESTOREJSON_H
#define GESTOREJSON_H

#include <prestito.h>
#include <media.h>
#include <audiovisivo.h>
#include <cartaceo.h>
#include <film.h>
#include <giornale.h>
#include <libro.h>
#include <rivista.h>
#include <vinile.h>

#include <QJsonObject>
#include <QString>
#include <QObject>

class GestoreJson {
public:
    GestoreJson(const QString& nomeFile);

    QList<Media*> caricaBiblioteca();
    QList<Prestito*> caricaPrestiti();
    void eliminaMedia(int indice);
    void eliminaPrestito(int indice);
    void salvaMedia(Media* media, int indice);
    void modificaMedia(int indice, int copie, bool disponibilita);
    void salvaPrestito(Prestito* prestito);

private:
    QList<Media*> listaMedia;
    QList<Prestito*> listaPrestiti;
    QString percorsoFile;

    void salvaMedia(Media* media, QJsonObject& oggetto);
    void salvaMedia(Audiovisivo* audiovisivo, QJsonObject& oggetto);
    void salvaMedia(Cartaceo* cartaceo, QJsonObject& oggetto);
    QJsonObject salvaMedia(Libro* libro);
    QJsonObject salvaMedia(Rivista* rivista);
    QJsonObject salvaMedia(Vinile* vinile);
    QJsonObject salvaMedia(Film* film);
    QJsonObject salvaMedia(Giornale* giornale);

    Rivista* caricaRivista(const QJsonObject& jsonObject) const;
    Film* caricaFilm(const QJsonObject& jsonObject) const;
    Vinile* caricaVinile(const QJsonObject& jsonObject) const;
    Giornale* caricaGiornale(const QJsonObject& jsonObject) const;
    Libro* caricaLibro(const QJsonObject& jsonObject) const;
    Prestito* caricaPrestito (const QJsonObject& jsonObject) const;
};

#endif // GESTOREJSON_H
