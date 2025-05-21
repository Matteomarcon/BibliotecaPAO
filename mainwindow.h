#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

private slots:
    void showPaginaPrincipale();
    void showPaginaNuovoMedia();
    QWidget * widgetMedia(const QString& titolo, const QString& descrizione, const QIcon& icona);

private:
    QStackedWidget *stackedWidget;
    QWidget *paginaPrincipale;
    QWidget *paginaNuovoMedia;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
