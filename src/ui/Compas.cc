#include <QDebug>
#include <QPainter>
#include <qmath.h>
#include "qwt_compass_rose.h"
#include "qwt_dial_needle.h"

#include "Compas.h"

Compas::Compas(QWidget *parent) :
    QwtCompass(parent),
    uas(0),
    heading(0)
{
    setRose(new QwtSimpleCompassRose(4, 1));
    setNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow, false, Qt::red));
    setReadOnly(true);
}

Compas::~Compas()
{
}

void Compas::setActiveUAS(UASInterface* uas)
{
    if (this->uas != 0)
        disconnect(this->uas, SIGNAL(headingChanged(UASInterface*,double)), this, SLOT(updateHeading(UASInterface*,double)));

    connect(uas, SIGNAL(headingChanged(UASInterface*,double)), this, SLOT(updateHeading(UASInterface*,double)));
    this->uas = uas;
}

void Compas::updateHeading(UASInterface* uas, double heading)
{
    Q_UNUSED(uas);

    if (heading != this->heading)
    {
        this->heading = heading;
        setValue(heading + 180.0);
    }
}
