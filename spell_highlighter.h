#ifndef SPELL_HIGHLIGHTER_H
#define SPELL_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <set>
#include <string>

class SpellHighlighter : public QSyntaxHighlighter {
Q_OBJECT
public:
    SpellHighlighter(QTextDocument *parent, const std::set<std::string> &dict);
    void rehighlightAll();

protected:
    void highlightBlock(const QString &text) override;

private:
    const std::set<std::string> &dictionary;
    QTextCharFormat errorFormat;
};

#endif