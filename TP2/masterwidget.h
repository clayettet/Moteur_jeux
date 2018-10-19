#ifndef MASTERWIDGET_H
#define MASTERWIDGET_H

#include "mainwidget.h"
#include <QBasicTimer>
#include <string>

class MasterWidget : QWidget
{
    public:
        MasterWidget();

        MainWidget* fallWidget;
        MainWidget* summerWidget;
        MainWidget* winterWidget;
        MainWidget* springWidget;

        void update();
        std::string getMonthStr();

        void timerEvent(QTimerEvent *e) override;

    protected:
        QBasicTimer timer;
        int month;

};

#endif // MASTERWIDGET_H
