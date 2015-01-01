#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //
    QTranslator translator;
    //
    QStringList translationDirs;
    translationDirs << QString(".")
                    << QString("../translations")
                    << QString("../../examples-cv/findRectObjects/translations");
    QStringListIterator tsDirsIterator(translationDirs);
    //
    bool bTranslationLoaded = false;
    while(!bTranslationLoaded && tsDirsIterator.hasNext())
    {
        bTranslationLoaded =
                translator.load(QLocale::system(), QString("findRectObjects"), "_", tsDirsIterator.next());
    }
    //
    if(bTranslationLoaded)
        a.installTranslator(&translator);
    else
        QMessageBox::warning(0, QObject::tr("Translation"), QObject::tr("Not loaded"));
    //
    MainWindow w;
    w.show();
    //
    return a.exec();
}
