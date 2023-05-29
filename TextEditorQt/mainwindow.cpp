#include "mainwindow.h"
#include "./ui_mainwindow.h"

QString fileContent = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);           // make text editor take all screen
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_File_triggered()
{
    currentFile.clear();                        // clear current file
    ui->textEdit->setText(QString());           // clear display
}


void MainWindow::on_actionOpen_File_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select File to open");           // select file to open
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly| QFile::Text)){
        QMessageBox::warning(this,"Error","Cannot open this file");             // error if it failed to open
        return;
    }
    currentFile = filename;                     // make it current file
    setWindowTitle(filename);                   // change window title
    QTextStream in(&file);
    fileContent = in.readAll();                 // read fileContent
    ui->textEdit->setText(fileContent);         // display content
    file.close();
}


void MainWindow::on_actionMerge_another_file_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select File to Merge");              // open window to select file to merge
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly| QFile::Text)){
        QMessageBox::warning(this,"Error","Cannot open this file");         // error if file failed to open
        return;
    }
    QTextStream in(&file);                  // open file
    int n = fileContent.length() - 1;
    fileContent.remove(n, 1);               // removes last null character
    fileContent += '\n' + in.readAll();     // append file to previous fileContent
    ui->textEdit->setText(fileContent);     // display fileContent
    file.close();
}


void MainWindow::on_actionSave_As_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Select File to Overwrite, or create new file");          // open window to select file
    QFile file(filename);
    if(!file.open(QFile::WriteOnly| QFile::Text)){                          //if file failed to open, terminate process.
        QMessageBox::warning(this,"Error","Cannot Save this file");
        return;
    }
    currentFile = filename;                                                 // make it currentFile
    setWindowTitle(filename);                                               // Change window title
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();     // convert editor to plain text to save it
    out << text;                            // save
    file.close();
}


void MainWindow::on_actionSave_triggered()
{
    QFile file(currentFile);
    if(!file.open(QFile::WriteOnly| QFile::Text)){                  // if file doesn't open, create new file
        bool ok;
        QString word = QInputDialog::getText(this, "File name",
                "Enter File name to save in", QLineEdit::Normal,
                QDir::home().dirName(), &ok);                       // take name of new file
        word += ".txt";
        QFile file(word);
        file.open(QFile::WriteOnly);            // create new file
        currentFile = word;                 // make it currentFile
        setWindowTitle(word);               // change window title
        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();     // convert editor to plain text to save it
        out << text;                        // save
        file.close();
        return;             // terminate
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();         // convert editor to plain text to save it
    out << text;                            // save
    file.close();
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionEmpty_File_triggered()
{
    fileContent = "";                   // clear string
    ui->textEdit->clear();              // clear display
}


void MainWindow::on_actionEncrypt_triggered()
{
    for (QChar & i : fileContent) {
        if (i == 'z')
            i = 'a';
        else if (i == 'Z')
            i = 'A';
        else if (i.isLetter()){
            char letter = i.toLatin1() + 1;
            i = (QChar)letter;                      // add 1 to every alpha letter
        }
    }
    int n = fileContent.length() - 1;
    fileContent.remove(n, 1);                       // removes last null character
    ui->textEdit->setPlainText(fileContent);        // display changes
}


void MainWindow::on_actionDecrypt_triggered()
{
    for (QChar & i : fileContent) {
        if (i == 'a')
            i = 'z';
        else if (i == 'A')
            i = 'Z';
        else if (i.isLetter()){
            char letter = i.toLatin1() - 1;
            i = (QChar)letter;                      // substract 1 from every alpha letter
        }
    }
    int n = fileContent.length() - 1;
    fileContent.remove(n, 1);                       // removes last null character
    ui->textEdit->setPlainText(fileContent);        // display changes
}


void MainWindow::on_textEdit_textChanged()
{
    fileContent = ui->textEdit->toPlainText() + '\0';   // update fileContent every time a letter is pressed on keyboard (add null character in the end)
}


void AllLower(QString& str) {
    for (QChar& chr : str) {
        chr = chr.toLower();                    // changes every character to lower case
    }
}


void MainWindow::on_actionTurn_File_to_LowerCase_triggered()
{
    AllLower(fileContent);                      // convert file to lowercase
    ui->textEdit->setText(fileContent);         // display changes
}


void MainWindow::on_actionTurn_File_to_UpperCase_triggered()
{
    for (QChar& chr : fileContent) {         // make every char uppercase
        chr = chr.toUpper();
    }
    ui->textEdit->setText(fileContent);     // display changes
}


void MainWindow::on_actionTurn_File_to_First_Cap_triggered()
{
    AllLower(fileContent);       // turns entire string to lower
    fileContent[0] = fileContent[0].toUpper();       // makes first letter Cap
    for (int i = 0; i < fileContent.length(); i++) {
        if (fileContent[i] == ' ' || fileContent[i] == '\n') {
            fileContent[i + 1] = fileContent[i + 1].toUpper();   // if a space or newline is found
        }                                                       // the character after it is turned Cap
    }
    ui->textEdit->setText(fileContent);
}


void MainWindow::on_actionCount_words_triggered()
{
    int nWords = 0;

    for (int i = 0; i < fileContent.length() - 1; i++) {
        if ((fileContent[i] != ' ' && fileContent[i] != '\n' && fileContent[i] != '\0') && (
            fileContent[i + 1] == ' ' || fileContent[i + 1] == '\n' || fileContent[i + 1] == '\0'
            ) // if any non-space-non-newline is found to be
            ) // next to a space or a newline or end of string
        {     // then it is a word
            nWords++;
        }
    }
    QMessageBox::information(this, "Number of words", QString("The number of Words in this file is: %1").arg(nWords));
}


void MainWindow::on_actionCount_Characters_triggered()
{
    int nChars = 0;
    for (QChar chr : fileContent) {
        if(chr!='\0')                   //does not count null character
            nChars++;
    }
    QMessageBox::information(this, "Number of Characters", QString("The number of Characters in this file is: %1").arg(nChars));
}


void MainWindow::on_actionCount_Lines_triggered()
{
    int nLines = 0;
    for (int i = 0; i < fileContent.length(); i++) {
        if (fileContent[i] == '\n' || fileContent[i] == '\0') {
            nLines++; // if a newline or an end of string character is found
        }             // then it is the end of a line
    }
    QMessageBox::information(this, "Number of Lines", QString("The number of Lines in this file is: %1").arg(nLines));
}


void MainWindow::on_actionWord_Count_triggered()
{
    vector<QString> arr;                 // vector to store each word individually
    int count = 0; QString temp = "";
    for (int i = 0; i < fileContent.length(); i++) {       // loop to find each word
        if (fileContent[i].isLetter())
            temp += fileContent[i];
        else {
            temp = temp.toLower();              // store it in lower case
            arr.push_back(temp);
            temp = "";                  // reset the value of temp to store another word
        }
    }
    bool ok;
    QString word = QInputDialog::getText(this, "Word Count",
            "Enter word to count the number of times it occurs in the text: ", QLineEdit::Normal,
            QDir::home().dirName(), &ok);
    QString tmp = word;
    word = word.toLower();
    for (QString st : arr) {             // loop on vector
        if (st == word)                 // add 1 to count if the index = word
            count++;
    }
    QMessageBox::information(this, "Word Count", QString("The count of the word "+tmp+" is: %1 times!").arg(count));
}


void MainWindow::on_actionSearch_Word_triggered()
{
    QString wantedWord, word;
    bool ok;
    wantedWord = QInputDialog::getText(this, "Search Word",
                    "Enter word to search for: ", QLineEdit::Normal,
                    QDir::home().dirName(), &ok);
    QString tmp = wantedWord;
    wantedWord = wantedWord.toLower();
    for (int i = 0; i < fileContent.length(); i++) {       // loop to find each word
        if (fileContent[i].isLetter())
            word += fileContent[i];
        else {
            word = word.toLower();              // store it in lower case
            if (wantedWord == word) {
               QMessageBox::information(this, "Found", QString("Word %1 was found!").arg(tmp));
               return;                  // terminate function
            }
            word = "";                  // reset the value of temp to store another word
        }
    }
    QMessageBox::information(this, "Not Found", QString("Word %1 was not found!").arg(tmp));
}

