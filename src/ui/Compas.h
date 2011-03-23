#ifndef COMPAS_H
#define COMPAS_H

#include "qwt_compass.h"
#include "UASInterface.h"

class Compas : public QwtCompass
{
    Q_OBJECT
public:
    explicit Compas(QWidget *parent = 0);
    ~Compas();

signals:

public slots:
    virtual void setActiveUAS(UASInterface* uas);
    virtual void updateHeading(UASInterface* uas, double heading);
    virtual void updateSpeed(UASInterface*, double x, double y, double z, quint64 usec);

protected:
    UASInterface* uas;
    double heading, totalSpeed;

    void paintEvent(QPaintEvent *);
};

#endif // COMPAS_H
