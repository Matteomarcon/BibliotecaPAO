#include "gestore.h"
#include "prestito.h"
#include "film.h"
#include "libro.h"
#include "giornale.h"
#include "rivista.h"
#include "vinile.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QStringList>
#include <QByteArray>
#include "media.h"
#include <QFile>
#include <QBuffer>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

Gestore::Gestore(const QString& nomeFile) : percorsoFile(nomeFile) {}

QStringList Gestore::getTipiDisponibili() {
    return {"Film", "Giornale", "Libro", "Rivista", "Vinile"};
}

Media* Gestore::getMedia(int indice) {
    return listaMedia.at(indice);
}

Prestito* Gestore::getPrestito(int indice) {
    return listaPrestiti.at(indice);
}

int Gestore::getQuantitaMedia() {
    return listaMedia.count();
}

int Gestore::getQuantitaPrestiti() {
    return listaPrestiti.count();
}

void Gestore::eliminaMedia(int indice) {
    QFile file(percorsoFile);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isArray()) return;

    QJsonArray array = doc.array();
    if (indice >= 0 && indice < array.size()) {
        array.removeAt(indice);
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;

    file.write(QJsonDocument(array).toJson());
    file.close();
}

void Gestore::eliminaPrestito(int indice) {
    QFileInfo info(percorsoFile);
    QString nuovaBase = "prestiti" + info.fileName();
    QString percorsoPrestiti = info.dir().absoluteFilePath(nuovaBase);
    QFile file(percorsoPrestiti);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isArray()) return;

    QJsonArray array = doc.array();
    if (indice >= 0 && indice < array.size()) {
        array.removeAt(indice);
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;

    file.write(QJsonDocument(array).toJson());
    file.close();
}


void Gestore::salvaMedia(Media* media, int indice) {
    if (indice == -1) {
        listaMedia.append(media);
    }
    else {
        listaMedia.replace(indice, media);
    }

    QJsonObject nuovoOggetto;
    if (typeid(*media)==typeid(Libro)) nuovoOggetto = salvaMedia(static_cast<Libro*>(media));
    else if (typeid(*media)==typeid(Giornale)) nuovoOggetto = salvaMedia(static_cast<Giornale*>(media));
    else if (typeid(*media)==typeid(Vinile)) nuovoOggetto = salvaMedia(static_cast<Vinile*>(media));
    else if (typeid(*media)==typeid(Film)) nuovoOggetto = salvaMedia(static_cast<Film*>(media));
    else if (typeid(*media)==typeid(Rivista)) nuovoOggetto = salvaMedia(static_cast<Rivista*>(media));

    QFile file(percorsoFile);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Impossibile aprire 1 dati.json");
    }
    QJsonDocument documento = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!documento.isArray()) {
        throw std::runtime_error("Non c'è nessun array in dati.json");
        return;
    }
    QJsonArray jsonArray = documento.array();

    if (indice == -1) {
        jsonArray.append(nuovoOggetto);
    }
    else {
        jsonArray.replace(indice, nuovoOggetto);
    }

    QFile outputFile(percorsoFile);
    QJsonDocument aggiornaDocumento(jsonArray);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Impossibile aprire 2 dati.json");
        return;
    }
    outputFile.write(aggiornaDocumento.toJson());
    outputFile.close();
}

void Gestore::salvaMedia(Media* media, QJsonObject& oggetto){
    oggetto["idMedia"] = media->getId();
    oggetto["Immagine"] = media->getImmagine();
    oggetto["Titolo"] = media->getTitolo();
    oggetto["Prezzo"] = media->getPrezzo();
    oggetto["Data di pubblicazione"] = media->getData().toString("dd/MM/yyyy");
    oggetto["Genere"] = media->getGenere();
    oggetto["Disponibilita"] = media->getDisponibilita();
    oggetto["Copie"] = media->getCopie();
}

void Gestore::salvaMedia(Audiovisivo* audiovisivo, QJsonObject& oggetto){
    oggetto["Durata"] = audiovisivo->getDurata();
    oggetto["Produzione"] = audiovisivo->getProduzione();
}

void Gestore::salvaMedia(Cartaceo* cartaceo, QJsonObject& oggetto){
    oggetto["Autore"] = cartaceo->getAutore();
    oggetto["Editore"] = cartaceo->getEditore();
}

QJsonObject Gestore::salvaMedia(Libro* libro){
    QJsonObject oggetto;
    oggetto["Classe"]="Libro";
    salvaMedia(static_cast<Media*>(libro), oggetto);
    salvaMedia(static_cast<Cartaceo*>(libro), oggetto);
    QString formato;
    oggetto["Formato"] = libro->getFormato();
    oggetto["Lingua"] = libro->getLingua();
    return oggetto;
}

QJsonObject Gestore::salvaMedia(Rivista* rivista){
    QJsonObject oggetto;
    oggetto["Classe"]="Rivista";
    salvaMedia(static_cast<Media*>(rivista), oggetto);
    salvaMedia(static_cast<Cartaceo*>(rivista), oggetto);
    oggetto["Numero"] = rivista->getNumero();
    oggetto["Periodicita"] = rivista->getPeriodicita();
    return oggetto;
}

QJsonObject Gestore::salvaMedia(Vinile* vinile){
    QJsonObject oggetto;
    oggetto["Classe"]="Vinile";
    salvaMedia(static_cast<Media*>(vinile), oggetto);
    salvaMedia(static_cast<Audiovisivo*>(vinile), oggetto);
    oggetto["Artista"] = vinile->getArtista();
    oggetto["Numero tracce"] = vinile->getNumeroTracce();
    return oggetto;
}

QJsonObject Gestore::salvaMedia(Film* film){
    QJsonObject oggetto;
    oggetto["Classe"]="Film";
    salvaMedia(static_cast<Media*>(film), oggetto);
    salvaMedia(static_cast<Audiovisivo*>(film), oggetto);
    oggetto["Regista"] = film->getRegista();
    oggetto["Lingua originale"] = film->getLinguaOriginale();
    oggetto["Paese produzione"] = film->getPaeseProduzione();
    return oggetto;
}

QJsonObject Gestore::salvaMedia(Giornale* giornale){
    QJsonObject oggetto;
    oggetto["Classe"]="Giornale";
    salvaMedia(static_cast<Media*>(giornale), oggetto);
    salvaMedia(static_cast<Cartaceo*>(giornale), oggetto);
    oggetto["Testata"] = giornale->getTestata();
    return oggetto;
}

void Gestore::caricaMedia() {
    QFile file(percorsoFile);

    for (Media* media : listaMedia) delete media;
    listaMedia.clear();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Impossibile aprire il file per la lettura:" << file.errorString();
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray array = doc.array();

    for (const QJsonValue& value : array) {
        if (value.isObject()) {
            QJsonObject jsonObject = value.toObject();
            QString classe = jsonObject["Classe"].toString();
            Media* media = nullptr;
            if(classe == "Rivista") media = caricaRivista(jsonObject);
            else if(classe == "Film") media = caricaFilm(jsonObject);
            else if(classe =="Vinile") media = caricaVinile(jsonObject);
            else if(classe =="Libro") media = caricaLibro(jsonObject);
            else if (classe =="Giornale") media = caricaGiornale(jsonObject);

            if (media) listaMedia.append(media);
        }
    }
}

Rivista* Gestore::caricaRivista(const QJsonObject& jsonObject) const {
    return new Rivista(jsonObject["idMedia"].toInt(), jsonObject["Immagine"].toString(), jsonObject["Titolo"].toString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                       QDate::fromString(jsonObject["Data di pubblicazione"].toString(), "dd/MM/yyyy"), jsonObject["Genere"].toString(), jsonObject["Disponibilita"].toBool(),
                       jsonObject["Copie"].toInt(), jsonObject["Autore"].toString(), jsonObject["Editore"].toString(),
                       jsonObject["Numero"].toInt(), jsonObject["Periodicita"].toString());
}
Film* Gestore::caricaFilm(const QJsonObject& jsonObject) const {
    return new Film(jsonObject["idMedia"].toInt(), jsonObject["Immagine"].toString(), jsonObject["Titolo"].toString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                    QDate::fromString(jsonObject["Data di pubblicazione"].toString(), "dd/MM/yyyy"), jsonObject["Genere"].toString(), jsonObject["Disponibilita"].toBool(),
                    jsonObject["Copie"].toInt(), jsonObject["Durata"].toInt(), jsonObject["Produzione"].toString(), jsonObject["Regista"].toString(),
                    jsonObject["Lingua originale"].toString(), jsonObject["Paese produzione"].toString());
}
Vinile* Gestore::caricaVinile(const QJsonObject& jsonObject) const {
    return new Vinile(jsonObject["idMedia"].toInt(), jsonObject["Immagine"].toString(), jsonObject["Titolo"].toString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                      QDate::fromString(jsonObject["Data di pubblicazione"].toString(), "dd/MM/yyyy"), jsonObject["Genere"].toString(), jsonObject["Disponibilita"].toBool(),
                      jsonObject["Copie"].toInt(), jsonObject["Durata"].toInt(), jsonObject["Produzione"].toString(),
                      jsonObject["Artista"].toString(), jsonObject["Numero tracce"].toInt());
}
Giornale* Gestore::caricaGiornale(const QJsonObject& jsonObject) const {
    return new Giornale(jsonObject["idMedia"].toInt(), jsonObject["Immagine"].toString(), jsonObject["Titolo"].toString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                        QDate::fromString(jsonObject["Data di pubblicazione"].toString(), "dd/MM/yyyy"), jsonObject["Genere"].toString(), jsonObject["Disponibilita"].toBool(),
                        jsonObject["Copie"].toInt(), jsonObject["Autore"].toString(), jsonObject["Editore"].toString(),
                        jsonObject["Testata"].toString());
}
Libro* Gestore::caricaLibro(const QJsonObject& jsonObject) const {
    return new Libro(jsonObject["idMedia"].toInt(), jsonObject["Immagine"].toString(), jsonObject["Titolo"].toString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                     QDate::fromString(jsonObject["Data di pubblicazione"].toString(), "dd/MM/yyyy"), jsonObject["Genere"].toString(), jsonObject["Disponibilita"].toBool(),
                     jsonObject["Copie"].toInt(), jsonObject["Autore"].toString(), jsonObject["Editore"].toString(),
                     jsonObject["Lingua"].toString(), jsonObject["Formato"].toString());
}

void Gestore::caricaPrestiti() {
    QFileInfo info(percorsoFile);
    QString nuovaBase = "prestiti" + info.fileName();
    QString percorsoPrestiti = info.dir().absoluteFilePath(nuovaBase);
    QFile file(percorsoPrestiti);

    for (Prestito* prestito : listaPrestiti) delete prestito;
    listaPrestiti.clear();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Impossibile aprire il file per la lettura:" << file.errorString();
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray array = doc.array();

    for (const QJsonValue& value : array) {
        if (value.isObject()) {
            QJsonObject jsonObject = value.toObject();
            Prestito* prestito = caricaPrestito(jsonObject);
            listaPrestiti.append(prestito);
        }
    }
}

void Gestore::salvaPrestito(Prestito* prestito) {
    QJsonObject oggetto;
    oggetto["Nome"] = prestito->getNome();
    oggetto["Cognome"] = prestito->getCognome();
    oggetto["DataInizio"] = prestito->getDataInizio().toString("dd/MM/yyyy");
    oggetto["DataFine"] = prestito->getDataFine().toString("dd/MM/yyyy");
    oggetto["idMedia"] = prestito->getIdMedia();

    QFileInfo info(percorsoFile);
    QString nuovaBase = "prestiti" + info.fileName();
    QString percorsoPrestiti = info.dir().absoluteFilePath(nuovaBase);
    QFile file(percorsoPrestiti);

    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Impossibile aprire 1 dati.json");
    }
    QJsonDocument documento = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!documento.isArray()) {
        throw std::runtime_error("Non c'è nessun array in dati.json");
        return;
    }
    QJsonArray jsonArray = documento.array();

    jsonArray.append(oggetto);

    QFile outputFile(percorsoPrestiti);
    QJsonDocument aggiornaDocumento(jsonArray);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Impossibile aprire 2 dati.json");
        return;
    }
    outputFile.write(aggiornaDocumento.toJson());
    outputFile.close();
}
Prestito* Gestore::caricaPrestito (const QJsonObject& jsonObject) const {
    return new Prestito(jsonObject["Nome"].toString(), jsonObject["Cognome"].toString(), QDate::fromString(jsonObject["DataInizio"].toString(), "dd/MM/yyyy"), QDate::fromString(jsonObject["DataFine"].toString(), "dd/MM/yyyy"), jsonObject["idMedia"].toInt());
}

void Gestore::modificaMedia(int indice, int copie, bool disponibilita) {
    QFile file(percorsoFile);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Impossibile aprire 1 dati.json");
    }
    QJsonDocument documento = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!documento.isArray()) {
        throw std::runtime_error("Non c'è nessun array in dati.json");
        return;
    }
    QJsonArray jsonArray = documento.array();

    QJsonObject obj = jsonArray[indice].toObject();

    obj["Copie"] = copie;
    obj["Disponibilita"] = disponibilita;

    jsonArray[indice] = obj;

    QFile outputFile(percorsoFile);
    QJsonDocument aggiornaDocumento(jsonArray);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Impossibile aprire 2 dati.json");
        return;
    }
    outputFile.write(aggiornaDocumento.toJson());
    outputFile.close();
}
