#include "masterwidget.h"
#include "mainwidget.h"
#include <QBasicTimer>
#include <iostream>
#include <string>

MasterWidget::MasterWidget()
{
     timer.start(1000, this);
     month = 0;
     std::cout << "Wouah on est au mois de " << getMonthStr() << std::endl;
}

void MasterWidget::timerEvent(QTimerEvent * event)
{
    this->update();
}

void MasterWidget::update()
{
    month = (month + 1) % 12;
    std::cout << "Wouah on est au mois de " << getMonthStr() << std::endl;
    if (month % 3 == 0)
    {
        std::cout << "On change de saison" << std::endl;
        winterWidget->nextSeason();
        fallWidget->nextSeason();
        summerWidget->nextSeason();
        springWidget->nextSeason();
    }
}

std::string MasterWidget::getMonthStr()
{
    switch (this->month) {
    case 0:
        return "Janvier";
    case 1:
        return "Février";
    case 2:
        return "Mars";
    case 3:
        return "Avril";
    case 4:
        return "Mai";
    case 5:
        return "Juin";
    case 6:
        return "Juillet";
    case 7:
        return "Aout";
    case 8:
        return "Septembre";
    case 9:
        return "Octobre";
    case 10:
        return "Novembre";
    case 11:
        return "Décembre";
    default:
        return std::to_string(month);
    }
}
