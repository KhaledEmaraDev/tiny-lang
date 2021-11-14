#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include "code-editor.h"
#include "tiny-highlighter.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    void loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif

private:
    void setupEditor();
    void setupActions();
    void setupStatusBar();
    bool shouldSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

    QTabWidget *tabber;
    CodeEditor *tinyEditor;
    TinyHighlighter *tinyHighlighter;
    CodeEditor *tokensEditor;
    QString curFile;
};

#endif // MAIN_WINDOW_H
