#include "gestione_json.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QStringList>
#include <QByteArray>
#include "media.h"
#include <QFile>

#include "film.h"
#include "libro.h"
#include "giornale.h"
#include "rivista.h"
#include "vinile.h"

gestione_json::gestione_json(const QString& nomeFile) : percorsoFile(nomeFile) {}

void gestione_json::salvaNuovoMedia(Media* nuovoMedia) {
    media.append(nuovoMedia);
    QJsonObject nuovoOggetto;
    if (typeid(*nuovoMedia)==typeid(Libro)) {
        nuovoOggetto = salvaMedia(static_cast<Libro*>(nuovoMedia));
    }
    else if (typeid(*nuovoMedia)==typeid(Giornale)) {
        nuovoOggetto = salvaMedia(static_cast<Giornale*>(nuovoMedia));
    }
    else if (typeid(*nuovoMedia)==typeid(Vinile)) {
        nuovoOggetto = salvaMedia(static_cast<Vinile*>(nuovoMedia));
    }
    else if (typeid(*nuovoMedia)==typeid(Film)) {
        nuovoOggetto = salvaMedia(static_cast<Film*>(nuovoMedia));
    }
    else if (typeid(*nuovoMedia)==typeid(Rivista)) {
        nuovoOggetto = salvaMedia(static_cast<Rivista*>(nuovoMedia));
    }
    QFile file(percorsoFile);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Impossibile aprire dati.json");
    }
    QJsonDocument documento = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!documento.isArray()) {
        throw std::runtime_error("Non c'è nessun array in dati.json");
        return;
    }
    QJsonArray jsonArray = documento.array();
    jsonArray.append(nuovoOggetto);

    QFile outputFile(percorsoFile);
    QJsonDocument aggiornaDocumento(jsonArray);
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Impossibile aprire dati.json");
        return;
    }
    outputFile.write(aggiornaDocumento.toJson());
    outputFile.close();
}

