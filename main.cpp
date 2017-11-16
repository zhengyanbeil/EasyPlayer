#include "widget.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MyMediaPlayer w;
  w.setFixedSize(w.width(),w.height());
  w.setWindowTitle("我的播放器");
  w.setWindowIcon(QIcon(":/Images/icon"));
  w.show();

  return a.exec();
}
