#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class syntaxHighlighter : public QSyntaxHighlighter
{

public:
    explicit syntaxHighlighter(QTextDocument *parent = nullptr);

protected:
     void highlightBlock(const QString &text) override;

 private:
     struct HighlightingRule
     {
         QRegularExpression pattern;    //正则表达式模式串
         QTextCharFormat format;        //匹配的样式
     };
     QVector<HighlightingRule> highlightingRules;//规则的集合，可以定义多个高亮规则

     QRegularExpression commentStartExpression; //注释的高亮
     QRegularExpression commentEndExpression;

     QTextCharFormat punctuationFormat;         //标点符号
     QTextCharFormat headerFormat;              //头文件
     QTextCharFormat keywordFormat;             //关键词
     QTextCharFormat classFormat;               //类名
     QTextCharFormat singleLineCommentFormat;   //单行注释
     QTextCharFormat multiLineCommentFormat;    //多行注释
     QTextCharFormat quotationFormat;           //头文件引用&字符串
     QTextCharFormat functionFormat;            //函数
};

#endif // SYNTAXHIGHLIGHTER_H
