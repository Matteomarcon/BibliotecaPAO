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

class GestoreJson: public QObject {
private:
    QList<Media*> listaMedia;
    QList<Prestito*> listaPrestiti;
    QString percorsoFile;
public:
    GestoreJson(const QString& nomeFile);
    QList<Media*> caricaBiblioteca();
    void eliminaMedia(int indice);
    void salvaMedia(Media* media, int indice);
    void salvaMedia(Media* media, QJsonObject& oggetto);
    void salvaMedia(Audiovisivo* audiovisivo, QJsonObject& oggetto);
    void salvaMedia(Cartaceo* cartaceo, QJsonObject& oggetto);
    QJsonObject salvaMedia(Libro* libro);
    QJsonObject salvaMedia(Rivista* rivista);
    QJsonObject salvaMedia(Vinile* vinile);
    QJsonObject salvaMedia(Film* film);
    QJsonObject salvaMedia(Giornale* giornale);

    Rivista* caricaRivista(const QJsonObject& jsonObject);
    Film* caricaFilm(const QJsonObject& jsonObject);
    Vinile* caricaVinile(const QJsonObject& jsonObject);
    Giornale* caricaGiornale(const QJsonObject& jsonObject);
    Libro* caricaLibro(const QJsonObject& jsonObject);

    QList<Prestito*> caricaPrestiti();
    void salvaPrestito(Prestito* prestito);
    Prestito* caricaPrestito (const QJsonObject& jsonObject);
};

#endif // GESTOREJSON_H
