#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QLabel>
#include <QScrollArea>
#include <worker-thread.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *pEvent);
private slots:
    void onActionOpenTriggered();
    void onActionFindRectTriggered();
    // реакция на запуск потока
    void onThreadStarted();
    // реакция на завершение потока
    void onThreadFinished();    
private:
    Ui::MainWindow *ui;
    // компоненты для отображения
    QSplitter *m_pSplitter;
    QLabel *m_pLabelImage; // метка для исходного изображения
    QLabel *m_pLabelPatternImage; // метка для исходного изображения
    QLabel *m_pLabelFileName;
    QLabel *m_pLabelImageSize;
    QScrollArea *m_pAreaImage;
    //
    QString m_fileName;
    QSize m_sizeImage;
    //
    WorkerThread *m_pThreadWork;        
    //
    void loadFile(const QString &rcFileName, bool isPattern = false);
    //
    void updateStatusBar();
};

#endif // MAINWINDOW_H
