#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <vector>

using namespace std;

extern QString fileContent;

void AllLower(QString&);


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_File_triggered();

    void on_actionOpen_File_triggered();

    void on_actionSave_As_triggered();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionEmpty_File_triggered();

    void on_actionEncrypt_triggered();

    void on_actionDecrypt_triggered();

    void on_actionMerge_another_file_triggered();

    void on_textEdit_textChanged();

    void on_actionTurn_File_to_LowerCase_triggered();

    void on_actionTurn_File_to_UpperCase_triggered();

    void on_actionTurn_File_to_First_Cap_triggered();

    void on_actionCount_words_triggered();

    void on_actionCount_Characters_triggered();

    void on_actionCount_Lines_triggered();

    void on_actionWord_Count_triggered();

    void on_actionSearch_Word_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
};
#endif // MAINWINDOW_H
