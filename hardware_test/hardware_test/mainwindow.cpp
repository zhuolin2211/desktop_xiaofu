#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playaudio.h"
Playaudio *play_music;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(800,480);
    //this->showFullScreen();
    ui->progressBar_music->setValue(0);
    ui->progressBar_music->setMinimum(0);
    ui->progressBar_music->setMaximum(100);
    /*set widget background*/
    set_background_img();
    set_text();
    set_size();
}
void MainWindow::set_text(void)
{
#define sheet "color:#FFFFE0;font-size:16pt;"
    ui->label->setStyleSheet(sheet);
    ui->label_2->setStyleSheet(sheet);
    ui->label_3->setStyleSheet(sheet);
#define sheet_pushbutton_text "color:#000000;font-size:16pt;"
    ui->pushButton_stop->setStyleSheet(sheet_pushbutton_text);
    ui->pushButton_suspend->setStyleSheet(sheet_pushbutton_text);
    ui->pushButton_paly->setStyleSheet(sheet_pushbutton_text);

    ui->pushButton_up->setStyleSheet(sheet_pushbutton_text);
    ui->pushButton_down->setStyleSheet(sheet_pushbutton_text);
    ui->pushButton_left->setStyleSheet(sheet_pushbutton_text);
    ui->pushButton_right->setStyleSheet(sheet_pushbutton_text);
}
void MainWindow::set_size(void)
{
    ui->progressBar_music->setFixedSize(300,30);
    ui->horizontalScrollBar_light->setFixedSize(300,30);
    ui->horizontalScrollBar_value->setFixedSize(300,30);
}
void MainWindow::set_background_img(void)
{
    QPixmap pixmap(":/file/img/beijing.png");
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);
}

MainWindow::~MainWindow()
{
    delete ui;
}


unsigned char play_flg=0;
unsigned long play_time=0;
void MainWindow::on_pushButton_paly_clicked()
{
    if(play_flg==0)//stop
    {
        /*start play*/
        play_flg=1;
    play_music= new Playaudio(QString("./Mojito.wav"));
    play_time=play_music->get_totaltime_s();
    std::cout<<"play time is "<<play_time<<"s\r\n";
    play_music->start_play();
    connect(play_music,&Playaudio::paly_done,this,&MainWindow::play_nusic_done_slot);
    connect(play_music,&Playaudio::progress_signal,this,&MainWindow::play_progress_slot);
    }
    else if(play_flg==2)//suspend
    {
        play_flg=1;
        /*continue play*/
        play_music->start_play();
    }
}

void MainWindow::on_pushButton_suspend_clicked()
{
   if(play_flg==1)//playing
   {
       play_flg=2;
       /*suspend play*/
       play_music->suspend_play();
   }
}

void MainWindow::on_pushButton_stop_clicked()
{
    if((play_flg==1) || (play_flg==2))
    {
        play_flg=0;
        /*stop*/
        play_music->stop_play();
        play_music->wait();
        delete play_music ;
    }
}
void MainWindow::play_nusic_done_slot()
{
    play_flg=0;
}
void MainWindow::play_progress_slot(unsigned char par)
{
    ui->progressBar_music->setValue((int)par);
}
