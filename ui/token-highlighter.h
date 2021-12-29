#ifndef TOKENHIGHLIGHTER_H
#define TOKENHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class TokenHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    TokenHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat lexemeFormat;
    QTextCharFormat typeFormat;
};

#endif // TOKENHIGHLIGHTER_H
