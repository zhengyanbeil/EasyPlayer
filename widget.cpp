/***********************************************************************************
歌曲图片无法放置在Label上；
播放列表无法正常使用；
 **********************************************************************************/

#include "widget.h"
#include "ui_widget.h"
#define DEBUG 1                 //0:启用调试
                                //1：禁用调试

QString MyMediaPlayer::noNameMusic = "未知曲目";
int MyMediaPlayer::playListIndex = 1;

MyMediaPlayer::MyMediaPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyMediaPlayer)
{
    ui->setupUi(this);
    setupUIAttr();
    setupConnect();

    player->setMedia(QUrl::fromLocalFile("E://Music/心跳.mp3"));
}

MyMediaPlayer::~MyMediaPlayer()
{
  delete ui;
}

void MyMediaPlayer::setupConnect()      //关联信号与槽
{
    connect(this->ui->playOrStop,SIGNAL(clicked()),\
            this,SLOT(MyMediaPlayer::on_playOrStop_clicked()));
    connect(ui->progress,SIGNAL(sliderMoved(int)),this,SLOT(progressSliderMoved(int)));
    connect(player,SIGNAL(stateChanged(QMediaPlayer::PausedState)),\
            ui->playOrStop,SLOT(MyMediaPlayer::playOrStopUpdate()));    //信号发出后无法进入槽函数，类似问题同前后两个connect（）!!!!!!!!!!!
    connect(this->timer,SIGNAL(timeout()),\
            this,SLOT(sliderUpdate()));
    connect(player,SIGNAL(metaDataChanged()),\
            this,SLOT(getMetaData()));
}

void MyMediaPlayer::setupUIAttr()       //设置控件属性
{
    playMethodSelect = ONELOOP;
    playlist = new QMediaPlaylist();
    //itemsList = new QStringList();
    //playlist->setMediaObject(player);     //有问题！！！！！！！！！！！！！！！！！
    playlist->addMedia(QUrl("E://Music/心跳.mp3"));
    playlist->addMedia(QUrl("E://Music/独角戏.mp3"));
    playlist->addMedia(QUrl("E://Music/사랑해요 只对你说.mp3"));
    playlist->setCurrentIndex(1);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    player = new QMediaPlayer();
    player->setPlaylist(playlist);

    //CoverArtImage_Temp = new QImage(QSize(250,250),QImage::Format_RGB32);    //专辑封面
    timer = new QTimer();
    timer->setInterval(1000);   //设定每隔一秒启动一次定时器
    ui->artOrder->setText("无");
    ui->volume->setMaximum(100);
    ui->volume->setValue(player->volume());
    ui->artOrder->setTextFormat(Qt::RichText);

    // 加载CSS文件
    QString strPath = QCoreApplication::applicationDirPath();
    QString strCssFile = strPath + "/default.css";
    QFile fCss(strCssFile);
    if(!fCss.open(QFile::ReadOnly))
        QMessageBox(QMessageBox::Warning,"Warning","load "+strCssFile+" failed",QMessageBox::Yes).exec();
    QString strCssContent(fCss.readAll());
    setStyleSheet(strCssContent);
    fCss.close();
}

void MyMediaPlayer::on_playOrStop_clicked()                     //播放按钮的槽
{
  if(ui->playOrStop->text() == "播放") //要求播放
  {
    ui->playOrStop->setText("暂停");
#if DEBUG == 0
    qDebug() << "当前歌曲总时间 ： " << player->duration();
#endif
    timer->start();
    ui->progress->setRange(0,player->duration());
    int pos = ui->progress->value();
    player->setPosition(pos);
    /*****************************************************************************************
     * 以下测试：
     * 当APP启动后，先拉进度条，让歌曲从pos处开始播放。
     * 但目前APP依然会从头开始（pos=0）播放歌曲
     ****************************************************************************************/
#if DEBUG == 0
    qDebug() << "!!!!!!!!!!!!!" << pos;
    qDebug() << "#############" << player->position();
#endif
    player->play();  //play后，歌曲的各项属性才回读入，暂时这样理解

    qDebug() << "#############" << player->position();  //pos值未变，但player从头开始播放！！！
  }
  else  //要求暂停
  {
    ui->playOrStop->setText("播放");
    player->pause();
  }
  qDebug() << "current state : " << player->state();
#if DEBUG == 0
  qDebug() << "当前歌曲进度 ： " << player->position();
  qDebug() << "当前曲目 ： " << playlist->currentIndex();
#endif
}

void MyMediaPlayer::on_volume_sliderMoved(int position)     //音量调节的槽
{
    player->setVolume(position);
}

void MyMediaPlayer::progressSliderMoved(int position)   //进度条的槽
{
    player->setPosition(position);

#if DEBUG == 0
    qDebug() << "当前滑块位置 ： " << position;
#endif
}

void MyMediaPlayer::sliderUpdate()                          //更新进度条滑块位置的槽
{
    ui->progress->setValue(player->position());
    if(ui->progress->value() == player->duration())
    {
        playMethod();
    }
}

void MyMediaPlayer::getMetaData()                           //获取音乐详尽信息（异步，等待信号metaDataChanged（））
{
    if(player->isMetaDataAvailable())
    {
        /******************************************************************
         * 思考能否循环获取单一文件的各成员信息
         * ******************************************************************/
        mp3Info.AlbumTitle = player->metaData(QMediaMetaData::AlbumTitle).toString();
        qDebug() << "AlbumTitle : " << mp3Info.AlbumTitle;
        mp3Info.AlbumArtist = player->metaData(QMediaMetaData::AlbumArtist).toString();
        qDebug() << "AlbumArtist : " << mp3Info.AlbumArtist;
        mp3Info.ContributingArtist = player->metaData(QMediaMetaData::ContributingArtist).toStringList();
        qDebug() << "Contributing Artist : " << mp3Info.ContributingArtist;
        mp3Info.Composer = player->metaData(QMediaMetaData::Composer).toStringList();
        qDebug() << "Composer : " << mp3Info.Composer;
        mp3Info.Conductor = player->metaData(QMediaMetaData::Conductor).toString();
        qDebug() << "Conductor : " << mp3Info.Conductor;
        mp3Info.Lyrics = player->metaData(QMediaMetaData::Lyrics).toString();
        qDebug() << "Lyrics : " << mp3Info.Lyrics;
        mp3Info.Mood = player->metaData(QMediaMetaData::Mood).toString();
        qDebug() << "Mood : " << mp3Info.Mood;
        mp3Info.TrackNumber = player->metaData(QMediaMetaData::TrackNumber).toInt();
        qDebug() << "TrackNumber : " << mp3Info.TrackNumber;
        mp3Info.TrackCount = player->metaData(QMediaMetaData::TrackCount).toInt();
        qDebug() << "TrackCount : " << mp3Info.TrackCount;

        QImage image = player->metaData(QMediaMetaData::CoverArtImage).value<QImage>();
        //QUrl PixUrl = player->metaData(QMediaMetaData::CoverArtImage).value<QUrl>();
        qDebug() << "Image : " << image;
        if(image.save("E://QT_Program/1.png",0,-1))    //return false!!!
            qDebug() << "save successfully ! ...\n";
        else
            qDebug() << "save failed ! ...\n";
        //ui->artOrder->setPixmap(QPixmap("E://QT_Program/test.JPG").scaled(250,250));    //此句没问题，图片能正常放上
        ui->artOrder->setPixmap(QPixmap(CoverArtImage_Temp));
    }
    else
        ui->artOrder->setText(MyMediaPlayer::noNameMusic);
}

void MyMediaPlayer::on_prior_clicked()                              //上一曲按钮的槽
{
    player->setMedia(QMediaContent(QUrl("E://Music/独角戏.mp3")));
    player->play();
    qDebug() << "current state : " << player->state();
}

void MyMediaPlayer::on_next_clicked()                               //下一曲按钮的槽
{
    playlist->setCurrentIndex(playListIndex++);
    player->play();
}

void MyMediaPlayer::playOrStopUpdate()                              //信号发出后，此槽不工作！！！！
{
    if((player->state() == QMediaPlayer::PausedState)|(player->state() == QMediaPlayer::StoppedState))
        ui->playOrStop->setText("播放");
    else if(player->state() == QMediaPlayer::PlayingState)
        ui->playOrStop->setText("暂停");
}

void MyMediaPlayer::playMethod()
{
    switch (playMethodSelect) {
    case ONELOOP:
        player->setPosition(0);
        player->play();
        break;
    case LISTLOOP:
        //add code
        break;
    case ONEONLY:
        break;
    case LISTONLY:
        //add code
        break;
    case RANDOM:
        //add code
        break;
    }
}
