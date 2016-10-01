#ifndef SCROLLAREAMODIFY_H
#define SCROLLAREAMODIFY_H

#include <QScrollArea>
#include <QResizeEvent>

class ScrollAreaModify : public QScrollArea
{
    Q_OBJECT
public:
    ScrollAreaModify(QWidget *parent=0);
    void resizeEvent(QResizeEvent *event);

signals:
    void resizeSignal(int,int);
};

#endif // SCROLLAREAMODIFY_H
