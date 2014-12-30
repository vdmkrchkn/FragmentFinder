#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QImage>

class WorkerThread : public QThread
{
    Q_OBJECT
public:    
    const QImage *getResultImage() const;
    //    
    void startImageMatching(const QImage &rcImage);
    void stop();
signals:
    void canceled();
private:            
    QScopedPointer<QImage> m_ptrInput, m_ptrResult;
    //
    virtual void run();
};

#endif // WORKERTHREAD_H
