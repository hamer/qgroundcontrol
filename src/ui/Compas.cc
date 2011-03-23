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
    setNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow, true, Qt::red));
    setReadOnly(true);
    setMinimumWidth(100);
    setMinimumHeight(100);
}

Compas::~Compas()
{
}

void Compas::setActiveUAS(UASInterface* uas)
{
    if (this->uas != 0)
    {
        disconnect(this->uas, SIGNAL(headingChanged(UASInterface*,double)), this, SLOT(updateHeading(UASInterface*,double)));
        disconnect(uas, SIGNAL(speedChanged(UASInterface*,double,double,double,quint64)), this, SLOT(updateSpeed(UASInterface*,double,double,double,quint64)));
    }

    connect(uas, SIGNAL(headingChanged(UASInterface*,double)), this, SLOT(updateHeading(UASInterface*,double)));
    connect(uas, SIGNAL(speedChanged(UASInterface*,double,double,double,quint64)), this, SLOT(updateSpeed(UASInterface*,double,double,double,quint64)));
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

void Compas::updateSpeed(UASInterface*, double x, double y, double z, quint64 usec)
{
    Q_UNUSED(usec);
    totalSpeed = qSqrt(x*x + y*y + z*z) / 10.0;

    update();
}

void Compas::paintEvent(QPaintEvent *event)
{
    QwtCompass::paintEvent(event);

    QPainter p(this);
    int w = width(), h = height();
    QRect rct = QRect(0, h/2, w, qMin(w, h)/2);

    p.setPen(Qt::black);
    p.setBackgroundMode(Qt::OpaqueMode);
    p.setBackground(QBrush(QColor(255, 255, 255, 64)));
    p.drawText(rct, QString("%1 m/s").arg(totalSpeed, 8, 'f', 3), QTextOption(Qt::AlignCenter));
}
