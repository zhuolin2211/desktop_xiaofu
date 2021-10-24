#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QPushButton"
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
    void Key_Change_slot(unsigned int key_code,unsigned char key_value);
private:
    Ui::MainWindow *ui;

    void set_background_img(void);
    void set_text();
    void set_size();
    void set_pushbutton_color(QPushButton *button,const QColor& color);
};

#endif // MAINWINDOW_H
