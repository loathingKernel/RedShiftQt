#ifndef QDOUBLESPINBOXLIST_H
#define QDOUBLESPINBOXLIST_H

#include <QDoubleSpinBox>

class QDoubleSpinBoxList : public QList<QDoubleSpinBox*>
{
public:
    QStringList value();
    void setValue(QStringList);
    void setEnabled(bool);
};
#endif // QDOUBLESPINBOXLIST_H
