#include "main-window.h"

#include <QtWidgets>
#include <QTextStream>
#include <QDebug>

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "ui/render-thread.h"
#include "lang/tiny.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupEditor();
    setCentralWidget(tabber);
    setupActions();
    setupStatusBar();
    setWindowTitle("Tiny Editor[*]");

#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (shouldSave())
        event->accept();
    else
        event->ignore();
}

void MainWindow::newFile()
{
    if (shouldSave()) {
        tinyEditor->clearErrors();
        tinyEditor->clear();
        tokensEditor->clearErrors();
        tokensEditor->clear();
        setCurrentFile(QString());
    }
}

void MainWindow::open()
{
    if (shouldSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"),
                                                        "../tiny-lang/data",
                                                        "Tiny files (*.tiny)"
                                                        );

        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty())
        return saveAs();
    else
        return saveFile(curFile);
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this,
                       tr("Save File As"),
                       "../tiny-lang/data",
                       "Tiny files (*.tiny)"
                       );

    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if (dialog.exec() != QDialog::Accepted)
        return false;

    return saveFile(dialog.selectedFiles().constFirst());
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Tiny Editor"),
                       tr("<p>This <b>Tiny Editor</b> is part of a project under the " \
                          "<b>Systems Software</b> course.</p>"));
}

void MainWindow::documentWasModified()
{
    setWindowModified(tinyEditor->document()->isModified());
}

bool MainWindow::parseFile()
{
    tinyEditor->clearErrors();
    tokensEditor->clearErrors();

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    try {
        Tiny language = Tiny(tinyEditor->toPlainText().toStdString());
        auto tokens = language.get_tokens();

        std::stringstream tokenList;
        for (const auto &token: tokens) {
            tokenList << token << std::endl;
        }

        tokensEditor->setPlainText(QString::fromStdString(tokenList.str()));

#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif

        tabber->setCurrentIndex(1);

        return true;
    } catch (const std::vector<std::pair<int, std::string>> &ex) {
        bool modified = tinyEditor->document()->isModified();
        for (const auto &[line, error]: ex) {
            tinyEditor->displayError(line, QString::fromStdString(error));
        }
        tinyEditor->document()->setModified(modified);
        documentWasModified();
    } catch (std::pair<int, std::string> &ex) {
        bool modified = tinyEditor->document()->isModified();
        tinyEditor->displayError(ex.first, QString::fromStdString(ex.second));
        tinyEditor->document()->setModified(modified);
        documentWasModified();
    } catch (const std::exception &ex) {
        statusBar()->showMessage(tr(ex.what()));
    } catch (const std::string &ex) {
        statusBar()->showMessage(tr(ex.c_str()));
    } catch (const QString &ex) {
        statusBar()->showMessage(ex);
    } catch (...) {
        statusBar()->showMessage(tr("An unexpected error occurred"));
    }
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    return false;
}

void MainWindow::renderTree()
{
    tokensEditor->clearErrors();

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    auto tokens_lines = tokensEditor->toPlainText().split('\n');
    std::vector<Token> tokens_list;

    int i = 1;
    for (auto const &line : tokens_lines) {
        auto token_pair = line.split(',');
        if (token_pair.size() == 2) {
            try {
                tokens_list.push_back(
                            Token(
                                Token::type_from_string(token_pair[1].toStdString()),
                            token_pair[0].toStdString(),
                        i
                        )
                        );
            } catch (const std::string &ex) {
                statusBar()->showMessage(tr(ex.c_str()));
#ifndef QT_NO_CURSOR
                QGuiApplication::restoreOverrideCursor();
#endif
                return;
            }
        }
        i++;
    }

    try {
        Parser parser(tokens_list);
        auto root_node = parser.parse();

        if (root_node == nullptr) {
#ifndef QT_NO_CURSOR
            QGuiApplication::restoreOverrideCursor();
#endif
            return;
}

        RenderThread *renderThread = new RenderThread(root_node->dot_representation());
        connect(renderThread, &RenderThread::renderFinished, this, &MainWindow::handleRender);
        connect(renderThread, &RenderThread::finished, renderThread, &QObject::deleteLater);
        renderThread->start();
    } catch (const std::vector<std::pair<int, std::string>> &ex) {
        bool modified = tokensEditor->document()->isModified();
        for (const auto &[line, error]: ex) {
            tokensEditor->displayError(line, QString::fromStdString(error));
        }
        tokensEditor->document()->setModified(modified);
        documentWasModified();
    } catch (std::pair<int, std::string> &ex) {
        bool modified = tokensEditor->document()->isModified();
        tokensEditor->displayError(ex.first, QString::fromStdString(ex.second));
        tokensEditor->document()->setModified(modified);
        documentWasModified();
    } catch (const std::exception &ex) {
        statusBar()->showMessage(tr(ex.what()));
    } catch (const std::string &ex) {
        statusBar()->showMessage(tr(ex.c_str()));
    } catch (const QString &ex) {
        statusBar()->showMessage(ex);
    } catch (...) {
        statusBar()->showMessage(tr("An unexpected error occurred"));
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
}

bool MainWindow::handleRender(QString result) {
    Q_UNUSED(result)

    if (!QFileInfo::exists("parsetree.svg") || !parseTreeView->openFile("parsetree.svg")) {
        QMessageBox::critical(this, tr("Render Parse Tree"),
                              tr("Couldn't load tree."));
        return false;
    }

    tabber->setCurrentIndex(2);

    return true;
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("monospace");
    font.setFixedPitch(true);
    font.setPointSize(10);

    tinyEditor = new CodeEditor;
    tinyEditor->setFont(font);

    tinyHighlighter = new TinyHighlighter(tinyEditor->document());

    connect(tinyEditor->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);

    tokensEditor = new CodeEditor;
    tokensEditor->setFont(font);

    parseTreeView = new SvgView;
    parseTreeView->setAntialiasing(true);
#ifndef QT_NO_OPENGL
    parseTreeView->setRenderer(SvgView::OpenGL);
#else
    parseTreeView->setRenderer(SvgView::Native);
#endif

    tabber = new QTabWidget(this);
    tabber->addTab(tinyEditor, "Tiny File");
    tabber->addTab(tokensEditor, "Tokens");
    tabber->addTab(parseTreeView, "Parse Tree");
}

void MainWindow::setupActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));

    fileMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));
#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
    QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, &QAction::triggered, tinyEditor, &CodeEditor::cut);
    editMenu->addAction(cutAct);
    editToolBar->addAction(cutAct);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, &QAction::triggered, tinyEditor, &CodeEditor::copy);
    editMenu->addAction(copyAct);
    editToolBar->addAction(copyAct);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
    QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, &QAction::triggered, tinyEditor, &CodeEditor::paste);
    editMenu->addAction(pasteAct);
    editToolBar->addAction(pasteAct);

    menuBar()->addSeparator();
#endif

    QMenu *codeMenu = menuBar()->addMenu(tr("&Code"));
    QToolBar *codeToolBar = addToolBar(tr("Code"));

    const QIcon parseIcon = QIcon(":/images/scan.png");
    QAction *parseAct = new QAction(parseIcon, tr("&Parse"), this);
    parseAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_P));
    parseAct->setStatusTip(tr("Extraxt Tokens and Print Parse Tree"));
    connect(parseAct, &QAction::triggered, this, &MainWindow::parseFile);
    codeMenu->addAction(parseAct);
    codeToolBar->addAction(parseAct);

    const QIcon renderIcon = QIcon(":/images/parse.png");
    QAction *renderAct = new QAction(renderIcon, tr("&Render"), this);
    renderAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_R));
    renderAct->setStatusTip(tr("Render Parse Tree"));
    connect(renderAct, &QAction::triggered, this, &MainWindow::renderTree);
    codeMenu->addAction(renderAct);
    codeToolBar->addAction(renderAct);

    menuBar()->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

#ifndef QT_NO_CLIPBOARD
    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(tinyEditor, &CodeEditor::copyAvailable, cutAct, &QAction::setEnabled);
    connect(tinyEditor, &CodeEditor::copyAvailable, copyAct, &QAction::setEnabled);
#endif
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

bool MainWindow::shouldSave()
{
    if (!tinyEditor->document()->isModified())
        return true;

    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("Tiny Editor"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
                                   );

    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFileInfo fileInfo(fileName);

    if (fileInfo.suffix() == "tiny") {
        QFile file(fileName);

        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Tiny Editor"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
            return;
        }

        QTextStream in(&file);
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        tinyEditor->clearErrors();
        tinyEditor->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif
    } else {
        QMessageBox::warning(this, tr("Tiny Editor"), tr("Unkown file type"));
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"));
}

bool MainWindow::saveFile(const QString &fileName)
{
    QString errorMessage;

    QFileInfo fileInfo(fileName);

    if (fileInfo.suffix() == "tiny") {
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

        QSaveFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);

            tinyEditor->clearErrors();
            out << tinyEditor->toPlainText();

            if (!file.commit()) {
                errorMessage = tr("Cannot write file %1:\n%2.")
                        .arg(QDir::toNativeSeparators(fileName), file.errorString());
            }
        } else {
            errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                    .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }

#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif
    } else {
        errorMessage = tr("Unkown file type");
    }

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("Tiny Editor"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"));

    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    tinyEditor->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.tiny";
    setWindowFilePath(shownName);
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager)
{
    if (manager.allowsInteraction()) {
        if (!shouldSave())
            manager.cancel();
    } else {
        if (tinyEditor->document()->isModified())
            save();
    }
}
#endif
