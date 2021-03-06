#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QProcess>

class RenderThread : public QThread
{
    Q_OBJECT
public:
    explicit RenderThread(QString parseTreeDotDescription): m_dot_descrption(parseTreeDotDescription) {}

private:
    void run() override {
        QString result = "";

        QProcess renderProcess;
        renderProcess.setProcessChannelMode(QProcess::MergedChannels);
        renderProcess.setProgram("dot");
        renderProcess.setArguments(QStringList() << "-Tsvg" << "-o" << "parsetree.svg");
        renderProcess.start(QIODevice::ReadWrite | QIODevice::Text);
        renderProcess.waitForStarted();
        renderProcess.write(m_dot_descrption.toUtf8());
        renderProcess.waitForBytesWritten();
        renderProcess.closeWriteChannel();

        if (!renderProcess.waitForFinished())
            result = renderProcess.errorString();
        else
            result = renderProcess.readAll();

        emit renderFinished(result);
    }

signals:
    void renderFinished(const QString &s);

private:
    QString m_dot_descrption;
};

#endif // RENDERTHREAD_H
