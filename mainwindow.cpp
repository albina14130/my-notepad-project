#i nclude "mainwindow.h"
#include "ui_mainwindow.h"
#include "notepad_exception.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFontDialog>
#include <QColorDialog>
#include <QRegularExpression>
#include <QMenu>
#include <map>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Ensure menu bar is visible on macOS
    menuBar()->setNativeMenuBar(false);

    // Live status bar with Word/Line count + Bonus (Line/Col)
    statusLabel = new QLabel("Words: 0  Lines: 0  Line: 1  Col: 1", this);
    ui->statusbar->addWidget(statusLabel);

    // Setup context menu for spelling suggestions
    ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textEdit, &QTextEdit::customContextMenuRequested, this, &MainWindow::showContextMenu);

    // Signals for live updates
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::update_status_bar);
    connect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::update_status_bar);

    load_dictionary();
    highlighter = new SpellHighlighter(ui->textEdit->document(), dictionary);

    update_title();
}

MainWindow::~MainWindow() {
    delete ui;
}

// --- FILE OPERATIONS WITH EXCEPTION HANDLING ---

void MainWindow::on_actionOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File");
    if (fileName.isEmpty()) return;

    try {
        QFile file(fileName);
        // Requirement: Distinct checks for exist and open
        if (!file.exists()) throw FileNotFoundException(fileName.toStdString());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) throw FileReadException(fileName.toStdString());

        ui->textEdit->setPlainText(file.readAll());
        update_title(fileName);
    } catch (const std::exception &e) {
        // Critical: Title must be "Error"
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::on_actionSave_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File");
    if (fileName.isEmpty()) return;

    try {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) throw FileWriteException(fileName.toStdString());

        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
        update_title(fileName);
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

// --- BONUS FEATURES: FONT & COLOR ---

void MainWindow::on_actionFont_triggered() {
    bool ok;
    QFont font = QFontDialog::getFont(&ok, ui->textEdit->currentFont(), this);
    if (ok) {
        ui->textEdit->setCurrentFont(font);
    }
}

void MainWindow::on_actionTextColor_triggered() {
    QColor color = QColorDialog::getColor(ui->textEdit->textColor(), this);
    if (color.isValid()) {
        ui->textEdit->setTextColor(color);
    }
}

// --- TOOLS & SPELL CHECK ---

// THIS WAS THE MISSING FUNCTION CAUSING YOUR ERROR
void MainWindow::on_actionCheck_Spelling_triggered() {
    if (highlighter) {
        highlighter->rehighlight();
    }
}

void MainWindow::on_actionWord_Frequency_triggered() {
    QString text = ui->textEdit->toPlainText().toLower();
    QStringList words = text.split(QRegularExpression("[^a-z]+"), Qt::SkipEmptyParts);

    std::map<QString, int> freq;
    for (const QString &w : words) freq[w]++;

    QString result = "Word\t\tCount\n----------------------------\n";
    for (auto const& [word, count] : freq) {
        result += word + "\t\t" + QString::number(count) + "\n";
    }

    // Requirement: Window title must be "Word Frequency"
    QMessageBox::information(this, "Word Frequency", result);
}

// --- UI UPDATES & DICTIONARY ---

void MainWindow::update_status_bar() {
    QString text = ui->textEdit->toPlainText();
    int words = text.split(QRegularExpression("(\\s|\\n|\\r)+"), Qt::SkipEmptyParts).count();
    int lines = ui->textEdit->document()->blockCount();
    if (text.isEmpty()) { words = 0; lines = 0; }

    QTextCursor cursor = ui->textEdit->textCursor();
    int curLine = cursor.blockNumber() + 1;
    int curCol = cursor.columnNumber() + 1;

    statusLabel->setText(QString("Words: %1  Lines: %2  Line: %3  Col: %4")
                                 .arg(words).arg(lines).arg(curLine).arg(curCol));
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QMenu *menu = ui->textEdit->createStandardContextMenu();
    QTextCursor cursor = ui->textEdit->cursorForPosition(pos);
    cursor.select(QTextCursor::WordUnderCursor);
    QString word = cursor.selectedText().toLower();

    // Context menu suggestions for misspelled words
    if (!word.isEmpty() && dictionary.find(word.toStdString()) == dictionary.end()) {
        menu->addSeparator();
        QMenu *suggestMenu = menu->addMenu("Suggestions");
        QStringList suggestions = { word + "s", "the " + word };
        for (const QString &s : suggestions) {
            QAction *act = suggestMenu->addAction(s);
            connect(act, &QAction::triggered, [this, cursor, s]() mutable {
                cursor.insertText(s);
            });
        }
    }
    menu->exec(ui->textEdit->mapToGlobal(pos));
    delete menu;
}

void MainWindow::load_dictionary() {
    QFile file("data/words.txt"); // Ensure Working Directory is set to $ProjectFileDir$
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            dictionary.insert(in.readLine().trimmed().toLower().toStdString());
        }
        file.close();
    }
}

void MainWindow::update_title(const QString &path) {
    if (path.isEmpty()) setWindowTitle("Notepad");
    else setWindowTitle("Notepad: " + path);
}

// Rich Text Formatting
void MainWindow::on_actionBold_triggered() {
    QTextCharFormat fmt;
    fmt.setFontWeight(ui->textEdit->fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::on_actionItalic_triggered() {
    ui->textEdit->setFontItalic(!ui->textEdit->fontItalic());
}

void MainWindow::on_actionUnderline_triggered() {
    ui->textEdit->setFontUnderline(!ui->textEdit->fontUnderline());
}

