 #include "spell_highlighter.h"

SpellHighlighter::SpellHighlighter(QTextDocument *parent, const std::set<std::string> &dict)
        : QSyntaxHighlighter(parent), dictionary(dict) {
    errorFormat.setUnderlineColor(Qt::red);
    errorFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
}

void SpellHighlighter::highlightBlock(const QString &text) {
    QRegularExpression re("\\b[A-Za-z]+\\b");
    auto it = re.globalMatch(text);
    while (it.hasNext()) {
        auto match = it.next();
        QString word = match.captured().toLower();
        if (dictionary.find(word.toStdString()) == dictionary.end()) {
            setFormat(match.capturedStart(), match.capturedLength(), errorFormat);
        }
    }
}

void SpellHighlighter::rehighlightAll() { rehighlight(); }