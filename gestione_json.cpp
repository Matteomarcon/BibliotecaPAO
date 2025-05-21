#include "gestione_json.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QStringList>
#include <QByteArray>
#include "media.h"
#include <QFile>
#include <QBuffer>

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

void gestione_json::salvaMedia(Media* media, QJsonObject& oggetto){
    QImage image = media->getImmagine();
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QString imageInBase64 = QString::fromLatin1(byteArray.toBase64());
    oggetto["Immagine"] = imageInBase64;

    oggetto["Titolo"] = QString::fromStdString(media->getTitolo());
    oggetto["Prezzo"] = media->getPrezzo();
    oggetto["Data di pubblicazione"] = QString::fromStdString(media->getData().toString());
    oggetto["Genere"] = QString::fromStdString(media->getGenere());
    oggetto["Disponibilita'"] = media->getDisponibilita();
    oggetto["Copie"] = media->getCopie();
}

void gestione_json::salvaMedia(Audiovisivo* audiovisivo, QJsonObject& oggetto){
    oggetto["Durata"] = audiovisivo->getDurata();
    oggetto["Produzione"] = QString::fromStdString(audiovisivo->getProduzione());
}

void gestione_json::salvaMedia(Cartaceo* cartaceo, QJsonObject& oggetto){
    oggetto["Autore"] = QString::fromStdString(cartaceo->getAutore());
    oggetto["Editore"] = QString::fromStdString(cartaceo->getEditore());
}

QJsonObject gestione_json::salvaMedia(Libro* libro){
    QJsonObject oggetto;
    oggetto["Classe"]="Libro";
    salvaMedia(static_cast<Media*>(libro), oggetto);
    salvaMedia(static_cast<Cartaceo*>(libro), oggetto);
    QString formato;
    switch (libro->getFormato()) {
    case 0: //copertinaRigida
        formato = "copertinaRigida";
        break;
    case 1: //copertinaFlessibile
        formato = "copertinaFlessibile";
        break;
    case 2: //tascabile
        formato = "tascabile";
        break;
    }
    oggetto["Formato"] = formato;
    oggetto["Lingua"] = QString::fromStdString(libro->getLingua());
    return oggetto;
}

QJsonObject gestione_json::salvaMedia(Rivista* rivista){
    QJsonObject oggetto;
    oggetto["Classe"]="Rivista";
    salvaMedia(static_cast<Media*>(rivista), oggetto);
    salvaMedia(static_cast<Cartaceo*>(rivista), oggetto);
    oggetto["Numero"] = rivista->getNumero();
    QString periodicita;
    switch (rivista->getPeriodicita()) {
    case 0: //settimanale
        periodicita = "settimanale";
        break;
    case 1: //mensile
        periodicita = "mensile";
        break;
    case 2: //bimestrale
        periodicita = "bimestrale";
        break;
    case 3: //trimestrale
        periodicita = "trimestrale";
        break;
    case 4: //semestrale
        periodicita = "semestrale";
        break;
    case 5: //annuale
        periodicita = "annuale";
        break;
    }
    oggetto["Periodicita'"] = periodicita;
    return oggetto;
}

QJsonObject gestione_json::salvaMedia(Vinile* vinile){
    QJsonObject oggetto;
    oggetto["Classe"]="Vinile";
    salvaMedia(static_cast<Media*>(vinile), oggetto);
    salvaMedia(static_cast<Audiovisivo*>(vinile), oggetto);
    oggetto["Artista"] = QString::fromStdString(vinile->getArtista());
    oggetto["Numero tracce"] = vinile->getNumeroTracce();
    return oggetto;
}

QJsonObject gestione_json::salvaMedia(Film* film){
    QJsonObject oggetto;
    oggetto["Classe"]="Film";
    salvaMedia(static_cast<Media*>(film), oggetto);
    salvaMedia(static_cast<Audiovisivo*>(film), oggetto);
    oggetto["Regista"] = QString::fromStdString(film->getRegista());
    oggetto["Lingua originale"] = QString::fromStdString(film->getLinguaOriginale());
    oggetto["Paese di produzione"] = QString::fromStdString(film->getPaeseProduzione());
    return oggetto;
}

QJsonObject gestione_json::salvaMedia(Giornale* giornale){
    QJsonObject oggetto;
    oggetto["Classe"]="Giornale";
    salvaMedia(static_cast<Media*>(giornale), oggetto);
    salvaMedia(static_cast<Cartaceo*>(giornale), oggetto);
    oggetto["Testata"] = QString::fromStdString(giornale->getTestata());
    return oggetto;
}



