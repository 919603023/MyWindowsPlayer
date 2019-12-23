#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMediaPlayer  *music = new QMediaPlayer(this);
    QMediaPlaylist *playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    music->setPlaylist(playlist);
    ui->verticalSlider->setMaximum(100);
    ui->verticalSlider->setValue(50);
    music->setVolume(50);
    this->setWindowFlags(Qt::FramelessWindowHint);



    QSqlQuery query;

    list1=QFileDialog::getOpenFileNames(this,QString::fromLocal8Bit("�ļ�"),"../",QString::fromLocal8Bit("��Ƶ�ļ�(*.mp3)"));
    //�ø��ļ���½����ѡ��.mp3�ļ�����������ļ�

    if(!list1.isEmpty()){
        music->setMedia(QUrl::fromLocalFile(list1.first()));

        for(int i=0;i<list1.size();i++){

            path=QDir::toNativeSeparators(list1.at(i));

            playlist->addMedia(QUrl::fromLocalFile(path));
            QString Name=path.split("\\").last();
            QListWidgetItem *item = new QListWidgetItem(Name);
            item->setToolTip(Name);

            ui->listWidget->addItem(item);
            query.exec(QString("insert into values(%1,'%2','%3')").arg(i).arg(Name).arg(path));

        }
        playlist->setCurrentIndex(0);//���õ�ǰ����
        ui->listWidget->scrollToItem(ui->listWidget->item(0));//��������ǰ����
        music->setMedia(QUrl::fromLocalFile(list1[0]));
    }
    //���Ű�ť �ź����ӵ����Ź���
    connect(ui->pushButton,&QPushButton::clicked,[=]{
        music->play();
    });
    //��ͣ��ť �ź����ӵ���ͣ����
    connect(ui->pushButton_2,&QPushButton::clicked,[=]{
        music->pause();

    });

    connect(music,&QMediaPlayer::positionChanged,[=](qint64 position){


        ui->horizontalSlider->setValue(position);
        ui->label->setText(Time(position));


    });
    //������λ�û�ȡ���������ı�󣬲����������źţ��ú��������źŲ����ý�����Ϊ��ǰλ��
    //����ʱ���ǩΪ��ǰʱ��
    connect(music,&QMediaPlayer::durationChanged,[=](qint64 duration){


        ui->horizontalSlider->setRange(0,duration);//���ݲ���ʱ�������û���ķ�Χ
        ui->horizontalSlider->setEnabled(duration>0);
        ui->label_2->setText(Time(duration));
        ui->horizontalSlider->setPageStep(duration/10);


    });
    connect(playlist,&QMediaPlaylist::currentIndexChanged,[=](int value){

        ui->listWidget->item(value)->setSelected(true);
    });
    connect(ui->pushButton_3,&QPushButton::clicked,[=](){


        int row = playlist->mediaCount();//��ȡ��������
        int current  = playlist->currentIndex();//��ȡ��ǰλ��
        if(current == 0){
            current = playlist->mediaCount();
        }
        current = current - 1;
        playlist->setCurrentIndex(current);//���õ�ǰ����
        ui->listWidget->scrollToItem(ui->listWidget->item(current));//��������ǰ����

        music->setMedia(QUrl::fromLocalFile(list1[current]));
        music->play();

    });
    connect(ui->pushButton_4,&QPushButton::clicked,[=](){


        int row = playlist->mediaCount();//��ȡ��������
        int current  = playlist->currentIndex();//��ȡ��ǰλ��
        if(current == row - 1){
            current= -1 ;
        }
        current = current + 1;
        playlist->setCurrentIndex(current);//���õ�ǰ����
        ui->listWidget->scrollToItem(ui->listWidget->item(current));//��������ǰ����
        music->setMedia(QUrl::fromLocalFile(list1[current]));
        qDebug()<<list1[current];
        music->play();





    });
    connect(ui->verticalSlider,&QSlider::valueChanged,[=](int val){
        music->setVolume(val);
    });



    connect(ui->pushButton_5,&QPushButton::clicked,[=]{
        this->close();
    });
    connect(ui->horizontalSlider,&QSlider::sliderMoved,music,&QMediaPlayer::setPosition);
    connect(ui->listWidget,&QListWidget::itemDoubleClicked,[=](QListWidgetItem *item){
        playlist->setCurrentIndex(ui->listWidget->row(item));
        music->setMedia(QUrl::fromLocalFile(list1[ui->listWidget->row(item)]));
        music->play();
    });
    connect(ui->pushButton_7,&QPushButton::clicked,[=]{
        int i = list1.count();
        QStringList list=QFileDialog::getOpenFileNames(this,QString::fromLocal8Bit("�ļ�"),"../",QString::fromLocal8Bit("��Ƶ�ļ�(*.mp3)"));
        //�ø��ļ���½����ѡ��.mp3�ļ�����������ļ�

        if(!list.isEmpty()){
            for(int a = 0;a < list.size();a++)
            {
                list1.push_back(list[a]);
            }
            music->setMedia(QUrl::fromLocalFile(list1.first()));
            ui->listWidget->clear();

            for(int i=0;i<list1.size();i++){

                path=QDir::toNativeSeparators(list1.at(i));

                playlist->addMedia(QUrl::fromLocalFile(path));
                QString Name=path.split("\\").last();
                QListWidgetItem *item = new QListWidgetItem(Name);
                item->setToolTip(Name);

                ui->listWidget->addItem(item);
            }
        }
    });
}





















MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enterEvent(QEvent *event)
{
    QRect rc;
        QRect rect;
        rect = this->geometry();
        rc.setRect(rect.x(),rect.y(),rect.width(),rect.height());
        if(rect.top()<0)
        {
            rect.setX(rc.x());
            rect.setY(0);
            move(rc.x(),-2);
        }

}

void MainWindow::leaveEvent(QEvent *event)
{
    QRect rc;
        QRect rect;
        rect = this->geometry();
        rc.setRect(rect.x(),rect.y(),rect.width(),rect.height());
        if(rect.top()<0)
        {
            move(rc.x(),-rc.height()+2);
        }
}



void MainWindow::mousePressEvent(QMouseEvent *event)
{
    offset=event->globalPos()-pos();
    event->accept();
    if(event->button()==Qt::LeftButton)
    {
        setCursor(Qt::OpenHandCursor);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos()-offset);
    event->accept();
    setCursor(Qt::ClosedHandCursor);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    offset=QPoint();
    event->accept();
    setCursor(Qt::ArrowCursor);
}

void MainWindow::ReadLyric(QString site)
{
    QFile file(site);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->listWidget_2->clear();
        while (!file.atEnd())
        {

            QByteArray line = file.readLine();
            QString str(line);
            QListWidgetItem *item = new QListWidgetItem(str);
            item->setText(str);
            ui->listWidget_2->addItem(item);
        }
        file.close();

    }

}

void MainWindow::on_pushButton_2_clicked()
{

}
QString Time(qint64 time)//ת��ʱ��
{
    qint64 seconds=time/1000;
    const qint64 minutes=seconds/60;
    seconds-=minutes*60;
    return QStringLiteral("%1:%2")
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'));
}
