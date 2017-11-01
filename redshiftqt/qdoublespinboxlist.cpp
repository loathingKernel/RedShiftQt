#include "qdoublespinboxlist.h"

QStringList QDoubleSpinBoxList::value(void) {
    QStringList gamma;
    QDoubleSpinBoxList::iterator i;
    for(i = this->begin(); i != this->end(); i++)
        gamma.append(QString("%1").arg((*i)->value()));
    return gamma;
}

void QDoubleSpinBoxList::setValue(QStringList v)
{
    for(int i = 0; i < this->count(); i++) {
        this->at(i)->setValue(v.value(i, v.value(0)).toDouble());
    }
}

void QDoubleSpinBoxList::setEnabled(bool b)
{
    QDoubleSpinBoxList::iterator i;
    for(i = this->begin(); i != this->end(); i++)
        (*i)->setEnabled(b);
}
