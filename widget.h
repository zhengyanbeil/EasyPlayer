#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QtMultimedia/QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <QTimer>
#include <QPixmap>
#include <QFont>
#include <QFile>
#include <QStringList>
#include <QMessageBox>

namespace Ui {
  class MyMediaPlayer;
}

class MyMediaPlayer : public QWidget
{
  Q_OBJECT

private:
    static QString noNameMusic;
    struct musicAttributes
    {
        QString AlbumTitle;
        QString AlbumArtist;
        QStringList ContributingArtist;
        QStringList  Composer;
        QString Conductor;
        QString Lyrics;
        QString  Mood;
        int TrackNumber;
        int TrackCount;
        QImage CoverArtImage;
    }mp3Info;
    enum
    {
        ONELOOP,
        LISTLOOP,
        ONEONLY,
        LISTONLY,
        RANDOM
    }playMethodSelect;

    QPixmap CoverArtImage_Temp;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    static int playListIndex;
    QTimer *timer;
    QFont mp3InfoFont;

public:
  explicit MyMediaPlayer(QWidget *parent = 0);
  ~MyMediaPlayer();
  void setupConnect();
  void setupUIAttr();
  void playMethod();

  QImage *getCoverArtImage_Temp() const;
  void setCoverArtImage_Temp(QImage *value);

private slots:
  void on_prior_clicked();

  void on_next_clicked();

  void playOrStopUpdate();

  void on_playOrStop_clicked();

  void on_volume_sliderMoved(int position);

  void progressSliderMoved(int position);

  void sliderUpdate();

  void getMetaData();

private:
  Ui::MyMediaPlayer *ui;
};

#endif // WIDGET_H
