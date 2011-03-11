/*=====================================================================

QGroundControl Open Source Ground Control Station

(c) 2009, 2010 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>

This file is part of the QGROUNDCONTROL project

    QGROUNDCONTROL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QGROUNDCONTROL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QGROUNDCONTROL. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
 * @file
 *   @brief Implementation of QGCSensorSettingsWidget
 *   @author Lorenz Meier <mavteam@student.ethz.ch>
 *
 */

#include "QGCSensorSettingsWidget.h"
#include "ui_QGCSensorSettingsWidget.h"

QGCSensorSettingsWidget::QGCSensorSettingsWidget(UASInterface* uas, QWidget *parent) :
    QWidget(parent),
    mav(uas),
    ui(new Ui::QGCSensorSettingsWidget)
{
    ui->setupUi(this);

    // checkboxes< that enables/disables following spinboxes
    connect(ui->checkBox_rawSensor, SIGNAL(stateChanged(int)), this, SLOT(spinRawSensorEnable(int)));
    connect(ui->checkBox_extended, SIGNAL(stateChanged(int)), this, SLOT(spinExtendedEnable(int)));
    connect(ui->checkBox_position, SIGNAL(stateChanged(int)), this, SLOT(spinPositionEnable(int)));
    connect(ui->checkBox_controller, SIGNAL(stateChanged(int)), this, SLOT(spinControllerEnable(int)));
    connect(ui->checkBox_rc, SIGNAL(stateChanged(int)), this, SLOT(spinRcEnable(int)));
    connect(ui->checkBox_extra1, SIGNAL(stateChanged(int)), this, SLOT(spinExtra1Enable(int)));
    connect(ui->checkBox_extra2, SIGNAL(stateChanged(int)), this, SLOT(spinExtra2Enable(int)));
    connect(ui->checkBox_extra3, SIGNAL(stateChanged(int)), this, SLOT(spinExtra3Enable(int)));

    // FIXME James Goppert
    // XXX: This might be a bad idea sending a message every time the value changes
    connect(ui->spinBox_rawSensor, SIGNAL(valueChanged(int)), mav, SLOT(enableRawSensorDataTransmission(int)));
    connect(ui->spinBox_controller, SIGNAL(valueChanged(int)), mav, SLOT(enableRawControllerDataTransmission(int)));
    connect(ui->spinBox_extended, SIGNAL(valueChanged(int)), mav, SLOT(enableExtendedSystemStatusTransmission(int)));
    connect(ui->spinBox_rc, SIGNAL(valueChanged(int)), mav, SLOT(enableRCChannelDataTransmission(int)));
    connect(ui->spinBox_position, SIGNAL(valueChanged(int)), mav, SLOT(enablePositionTransmission(int)));
    connect(ui->spinBox_extra1, SIGNAL(valueChanged(int)), mav, SLOT(enableExtra1Transmission(int)));
    connect(ui->spinBox_extra2, SIGNAL(valueChanged(int)), mav, SLOT(enableExtra2Transmission(int)));
    connect(ui->spinBox_extra3, SIGNAL(valueChanged(int)), mav, SLOT(enableExtra3Transmission(int)));

    // Calibration
    connect(ui->rcCalButton, SIGNAL(clicked()), mav, SLOT(startRadioControlCalibration()));
    connect(ui->magCalButton, SIGNAL(clicked()), mav, SLOT(startMagnetometerCalibration()));
    connect(ui->pressureCalButton, SIGNAL(clicked()), mav, SLOT(startPressureCalibration()));
    connect(ui->gyroCalButton, SIGNAL(clicked()), mav, SLOT(startGyroscopeCalibration()));
}

QGCSensorSettingsWidget::~QGCSensorSettingsWidget()
{
    delete ui;
}

void QGCSensorSettingsWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QGCSensorSettingsWidget::spinRawSensorEnable(int state) // slot for checkbox
{
    if (state == Qt::Unchecked) // disabled
    {
        mav->enableRawSensorDataTransmission(0); // stop data stream
        ui->spinBox_rawSensor->setEnabled(false);
    }
    else
    {
        mav->enableRawSensorDataTransmission(ui->spinBox_rawSensor->value());
        ui->spinBox_rawSensor->setEnabled(true);
    }
}

void QGCSensorSettingsWidget::spinExtendedEnable(int state) // slot for checkbox
{
    if (state == Qt::Unchecked) // disabled
    {
        mav->enableExtendedSystemStatusTransmission(0); // stop data stream
        ui->spinBox_extended->setEnabled(false);
    }
    else
    {
        mav->enableExtendedSystemStatusTransmission(ui->spinBox_extended->value());
        ui->spinBox_extended->setEnabled(true);
    }
}

void QGCSensorSettingsWidget::spinPositionEnable(int state) // slot for checkbox
{
    if (state == Qt::Unchecked) // disabled
    {
        mav->enablePositionTransmission(0); // stop data stream
        ui->spinBox_position->setEnabled(false);
    }
    else
    {
        mav->enablePositionTransmission(ui->spinBox_position->value());
        ui->spinBox_position->setEnabled(true);
    }
}

void QGCSensorSettingsWidget::spinControllerEnable(int state) // slot for checkbox
{
    if (state == Qt::Unchecked) // disabled
    {
        mav->enableRawControllerDataTransmission(0); // stop data stream
        ui->spinBox_controller->setEnabled(false);
    }
    else
    {
        mav->enableRawControllerDataTransmission(ui->spinBox_controller->value());
        ui->spinBox_controller->setEnabled(true);
    }
}

void QGCSensorSettingsWidget::spinRcEnable(int state) // slot for checkbox
{
    if (state == Qt::Unchecked) // disabled
    {
        mav->enableRCChannelDataTransmission(0); // stop data stream
        ui->spinBox_rc->setEnabled(false);
    }
    else
    {
        mav->enableRCChannelDataTransmission(ui->spinBox_rc->value());
        ui->spinBox_rc->setEnabled(true);
    }
}

void QGCSensorSettingsWidget::spinExtra1Enable(int state) // slot for checkbox
{
    if (state == Qt::Unchecked) // disabled
    {
        mav->enableExtra1Transmission(0); // stop data stream
        ui->spinBox_extra1->setEnabled(false);
    }
    else
    {
        mav->enableExtra1Transmission(ui->spinBox_extra1->value());
        ui->spinBox_extra1->setEnabled(true);
    }
}

void QGCSensorSettingsWidget::spinExtra2Enable(int state) // slot for checkbox
{
    if (state == Qt::Unchecked) // disabled
    {
        mav->enableExtra2Transmission(0); // stop data stream
        ui->spinBox_extra2->setEnabled(false);
    }
    else
    {
        mav->enableExtra2Transmission(ui->spinBox_extra2->value());
        ui->spinBox_extra2->setEnabled(true);
    }
}

void QGCSensorSettingsWidget::spinExtra3Enable(int state) // slot for checkbox
{
    if (state == Qt::Unchecked) // disabled
    {
        mav->enableExtra3Transmission(0); // stop data stream
        ui->spinBox_extra3->setEnabled(false);
    }
    else
    {
        mav->enableExtra3Transmission(ui->spinBox_extra3->value());
        ui->spinBox_extra3->setEnabled(true);
    }
}
