#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QImage>

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    //
    const QImage *getResultImage() const;
    //
    void startLoadFile(const QString &rcFilePath);
    void startImageMatching(const QImage &rcImage);
    void stop();
signals:
    void canceled();
private:
    enum Operation
    {
        LoadFile,
        MatchImage
    };
    Operation m_nOperation;
    QString m_FilePath;
    QScopedPointer<QImage> m_ptrInput, m_ptrResult;
    //
    virtual void run();
};

#endif // WORKERTHREAD_H
