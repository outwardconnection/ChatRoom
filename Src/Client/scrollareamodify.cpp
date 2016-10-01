#include "scrollareamodify.h"

ScrollAreaModify::ScrollAreaModify(QWidget *parent):QScrollArea(parent)
{

}

void ScrollAreaModify::resizeEvent(QResizeEvent *event)
{
    emit resizeSignal(width(),height());
    event->accept();
}
