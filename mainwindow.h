#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <set>
#include <string>
#include "spell_highlighter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionFont_triggered();
    void on_actionTextColor_triggered();
    void on_actionBold_triggered();
    void on_actionItalic_triggered();
    void on_actionUnderline_triggered();
    void on_actionWord_Frequency_triggered();
    void on_actionCheck_Spelling_triggered();
    void update_status_bar();
    void showContextMenu(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    QLabel *statusLabel;
    SpellHighlighter *highlighter;
    std::set<std::string> dictionary;
    void load_dictionary();
    void update_title(const QString &path = "");
};
#endif