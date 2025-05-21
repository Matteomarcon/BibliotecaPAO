#include "gestoremedia.h"
#include <QHBoxLayout>
#include <QLabel>

GestoreMedia::GestoreMedia(QListWidget* lista, const QString& percorso)
    : gestoreJson(percorso), listaMedia(lista) {}

void GestoreMedia::inserisciNuovoMedia(Media* media) {
    // Salva nel JSON usando l'istanza gestoreJson
    gestoreJson.salvaNuovoMedia(media);

    // Mostra sulla lista (semplificato)
    QListWidgetItem* item = new QListWidgetItem(listaMedia);
    item->setSizeHint(QSize(200, 60));

    QString titolo = QString::fromStdString("prova");
    QString descrizione = QString::fromStdString("prova2");

    QWidget* placeholder = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(placeholder);
    layout->addWidget(new QLabel(titolo + " - " + descrizione));
    placeholder->setLayout(layout);
    listaMedia->setItemWidget(item, placeholder);
}
