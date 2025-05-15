#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

private slots:
    void showPaginaPrincipale();
    void showPaginaNuovoMedia();

private:
    QStackedWidget *stackedWidget;
    QWidget *paginaPrincipale;
    QWidget *paginaNuovoMedia;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
