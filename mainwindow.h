#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QSqlDatabase db;
public:
        QSqlTableModel *model;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //void onButtonSend();

    void on_pushButton_3_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    void checkreg(QString code);
    void checkcode(QString code);
    void getcode(QString &code);
    Ui::MainWindow *ui;
    int state;
    QChar *ch;
    bool compile;
    int qtype;
    QRegExp *rx;
    QMap<QString,QString> keyword;
    QMap<QString,QString> other;
    QMap<QString,QString> token;
};

#endif // MAINWINDOW_H
