#include <QApplication>
#include "myform.h"


int main(int argc, char* argv[])
{
  printf("Start application->>>>");
  QApplication app(argc, argv);

  //QApplication::addLibraryPath(QApplication::applicationDirPath() + "/plugins");
  //QString Path = QApplication::applicationDirPath() + "/dll";
  //QByteArray byteArray = Path.toAscii();
  //const char *str      = byteArray.constData();
  //printf("Start application->>>> %s ",str);

  MyForm* form = new MyForm;
  form->show();
  return app.exec();
}

