#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

namespace
{
    QString strippedName(const QString &rcFullFileName)
    {
        return QFileInfo(rcFullFileName).fileName();
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // настройка элементов UI
    ui->setupUi(this);
    //
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(onActionOpenTriggered()));
    connect(ui->actionFind_Rect, SIGNAL(triggered()), this, SLOT(onActionFindRectTriggered()));
    //
    m_pSplitter = new QSplitter(this);
    //
    m_pLabelImage = new QLabel;    
    m_pLabelImage->setAlignment(Qt::AlignCenter);
    //
    m_pAreaImage = new QScrollArea(m_pSplitter);
    m_pAreaImage->setBackgroundRole(QPalette::Dark);
    m_pAreaImage->setWidget(m_pLabelImage);
    m_pAreaImage->setWidgetResizable(true);
    //
    m_pLabelPatternImage = new QLabel(m_pSplitter);
    m_pLabelPatternImage->setAlignment(Qt::AlignCenter);
    m_pLabelPatternImage->setBackgroundRole(QPalette::Shadow);
    //
    setCentralWidget(m_pSplitter);
    m_pSplitter->show();
    //
    m_pLabelFileName = new QLabel;
    m_pLabelImageSize = new QLabel("9999 x 9999");
    m_pLabelImageSize->setAlignment(Qt::AlignHCenter);
    m_pLabelImageSize->setMinimumSize(
    m_pLabelImageSize->sizeHint());
    //
    statusBar()->addWidget(m_pLabelFileName, 1);
    statusBar()->addWidget(m_pLabelImageSize);
    updateStatusBar();
    // загрузка настроек окна из внутреннего хранилища
    QSettings settings("kvv", "findRectObjects");
    restoreGeometry(settings.value("geometry").toByteArray());
    //
    m_pThreadWork = new WorkerThread;
    connect(m_pThreadWork, SIGNAL(started()), this, SLOT(onThreadStarted()));
    connect(m_pThreadWork, SIGNAL(finished()),this, SLOT(onThreadFinished()));    
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_pLabelImage;
    delete m_pAreaImage;
    delete m_pLabelFileName;
    delete m_pLabelImageSize;
    delete m_pSplitter;
    delete m_pThreadWork;
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    pEvent->accept();
    //
    m_pThreadWork->stop();
    // сохранение настроек окна во внутреннее хранилище
    QSettings settings("kvv", "findRectObjects");
    settings.setValue("geometry", saveGeometry());
}

void MainWindow::onActionOpenTriggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),
                                                    ":/image",
                                                    tr("Images (*.png *.jpg)"));
    if (!fileName.isEmpty())
        loadFile(fileName);
}

void MainWindow::onActionFindRectTriggered()
{
    if(!m_pLabelImage->pixmap())
        QMessageBox::warning(this,tr("Warning"),tr("No image to process"));
    else
    {
        QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),
                                                        ":/image",
                                                        tr("Images (*.png *.jpg)"));
        if (!fileName.isEmpty())
            loadFile(fileName,true);
        if(!m_pLabelPatternImage->pixmap())
            QMessageBox::warning(this,tr("Warning"),tr("No pattern image is loaded"));
        else
            m_pThreadWork->startImageMatching(m_pLabelImage->pixmap()->toImage(),
                                              m_pLabelPatternImage->pixmap()->toImage());
    }
}

void MainWindow::onThreadStarted()
{
    ui->menuBar->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //
    Sleep(2000);
}

void MainWindow::onThreadFinished()
{
    ui->menuBar->setEnabled(true);
    //
    QApplication::restoreOverrideCursor();
    //
    const QImage *pcImage = m_pThreadWork->getResultImage();
    if (pcImage)    
        m_pLabelImage->setPixmap(QPixmap::fromImage(*pcImage));    
    //
    statusBar()->showMessage(tr("Fragment found"),2000);
}

void MainWindow::loadFile(const QString &rcFileName, bool isPattern)
{
    QPixmap pixmap;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    const bool cbSuccess = pixmap.load(rcFileName);
    QApplication::restoreOverrideCursor();
    //
    if(cbSuccess)
    {
        if(!isPattern)
        {
            m_pLabelImage->setPixmap(pixmap);
            m_sizeImage = pixmap.rect().size();
            m_fileName = rcFileName;
            setWindowTitle(strippedName(rcFileName));
            updateStatusBar();
        }
        else
            m_pLabelPatternImage->setPixmap(pixmap);
        statusBar()->showMessage(tr("File loaded"), 2000);
    }
}

void MainWindow::updateStatusBar()
{
    QString strLabel;
    if(!m_sizeImage.isEmpty())
        strLabel = QString("%1 x %2")
            .arg(m_sizeImage.width())
            .arg(m_sizeImage.height());
    //
    m_pLabelFileName->setText(m_fileName);
    m_pLabelImageSize->setText(strLabel);
}
