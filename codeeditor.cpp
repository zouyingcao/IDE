#include <QtWidgets>
#include "codeeditor.h"
#include <QCompleter>

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
  , keyWordsCompleter{nullptr}
{
    keyWordsList << "auto" << "bool" << "break"
                 << "case" << "char" << "class"
                 << "const" << "continue" << "default"
                 << "do" << "double" << "else"
                 << "enum" << "explicit" << "extern"
                 << "float" << "for" << "friend"
                 << "goto" << "if" << "inline"
                 << "int" << "long" << "restrict"
                 << "return" << "namespace"<< "operator"
                 << "private" << "protected" << "public"
                 << "short" << "signals" << "signed"
                 << "sizeof" << "switch" << "while"
                 << "slots" << "static" << "struct"
                 << "template" << "typedef" << "typename"
                 << "union" << "unsigned" << "virtual"
                 << "void" << "volatile" << "#include" << "iostream";

    keyWordsCompleter = new QCompleter(keyWordsList);
    keyWordsCompleter->setWidget(this);
    keyWordsCompleter->setCaseSensitivity(Qt::CaseInsensitive); // 设置大小写区分，不区大小写
    keyWordsCompleter->setCompletionMode(QCompleter::PopupCompletion);// 设置向用户提供补全的方式
    keyWordsCompleter->setMaxVisibleItems(6);

    connect(keyWordsCompleter, SIGNAL(activated(QString)), this, SLOT(onCompleterActivated(QString)));

    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    this->setTabChangesFocus(false);
    this->setTabStopDistance(20);
}


//public slots
void CodeEditor::onCompleterActivated(const QString &completion)
{
    QString completionPrefix = wordUnderCursor(),
            shouldInertText = completion;
    curTextCursor = textCursor();
    if (!completion.contains(completionPrefix))
    {
        // delete the previously typed.
        curTextCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, completionPrefix.size());
        curTextCursor.clearSelection();
    }
    else
    {
        // 补全相应的字符
        shouldInertText = shouldInertText.replace(
            shouldInertText.indexOf(completionPrefix), completionPrefix.size(), "");
    }
    curTextCursor.insertText(shouldInertText);
}
QString CodeEditor::wordUnderCursor() const
{
    //不断向左移动cursor，并选中字符，并查看选中的单词中是否含有空格——空格作为单词的分隔符
    QTextCursor curTextCursor = textCursor();
    QString selectedString;
    while (curTextCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1))
    {
        selectedString = curTextCursor.selectedText();
        if (selectedString.startsWith(QString(" ")) || selectedString.startsWith(QChar(0x422029)))
        {
            break;
        }

    }
    if (selectedString.startsWith(QChar(0x422029)))
    {
        selectedString.replace(0, 1, QChar(' '));
    }
    return selectedString.trimmed();
}

//protected Events
void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if (keyWordsCompleter)
    {
        if (keyWordsCompleter->popup()->isVisible())
        {
            switch(e->key())
            {
                case Qt::Key_Escape:
                case Qt::Key_Enter:
                case Qt::Key_Return:
                case Qt::Key_Tab:
                    e->ignore();
                    return;
                default:
                    break;
            }
        }
        QPlainTextEdit::keyPressEvent(e);
        completerPrefix = this->wordUnderCursor();
        keyWordsCompleter->setCompletionPrefix(completerPrefix); // 通过设置QCompleter的前缀，来让Completer寻找关键词
        curTextCursorRect = cursorRect();
        if (completerPrefix == "")
        {
            return;
        }
        //qDebug() << "completerPrefix:" << completerPrefix << " match_count:" << keyWordsComplter->completionCount() << " completionColumn:"<<keyWordsComplter->completionColumn();
        if (keyWordsCompleter->completionCount() > 0)
        {
            keyWordsCompleter->complete(QRect(curTextCursorRect.left(), curTextCursorRect.top(), 60, 15));
        }
    }
}

void CodeEditor::onCurosPosChange()
{
    QString completionPrefix = wordUnderCursor();
    if (completionPrefix == "")
    {
        keyWordsCompleter->setCompletionPrefix("----");
        keyWordsCompleter->complete(QRect(curTextCursorRect.left(), curTextCursorRect.top(), 60, 15));
    }
    highlightCurrentLine();

}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}


void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
