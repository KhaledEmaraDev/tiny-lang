#include "token-highlighter.h"

#include <QDebug>

TokenHighlighter::TokenHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    lexemeFormat.setForeground(Qt::darkBlue);
    rule.pattern = QRegularExpression(QStringLiteral("^(.+),"));
    rule.format = lexemeFormat;
    highlightingRules.append(rule);

    typeFormat.setFontWeight(QFont::Bold);
    typeFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral(",(.+)$"));
    rule.format = typeFormat;
    highlightingRules.append(rule);
}

void TokenHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(1), match.capturedLength(1), rule.format);
        }
    }
    setCurrentBlockState(0);
}
