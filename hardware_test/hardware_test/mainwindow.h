#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_paly_clicked();

    void on_pushButton_suspend_clicked();

    void on_pushButton_stop_clicked();

    void play_nusic_done_slot();
    void play_progress_slot(unsigned char pro);
private:
    Ui::MainWindow *ui;

    void set_background_img(void);
    void set_text();
    void set_size();
};

#endif // MAINWINDOW_H
