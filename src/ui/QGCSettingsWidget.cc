#include <QSettings>

#include "QGCSettingsWidget.h"
#include "MainWindow.h"
#include "ui_QGCSettingsWidget.h"

#include "LinkManager.h"
#include "MAVLinkProtocol.h"
#include "MAVLinkSettingsWidget.h"

//, Qt::WindowFlags flags

QGCSettingsWidget::QGCSettingsWidget(QWidget *parent, Qt::WindowFlags flags) :
    QDialog(parent, flags),
    ui(new Ui::QGCSettingsWidget)
{
    ui->setupUi(this);

    QSettings s;
    ui->netBlocker->setChecked(s.value("netBlocked").toInt());

    // Add all protocols
    QList<ProtocolInterface*> protocols = LinkManager::instance()->getProtocols();
    foreach (ProtocolInterface* protocol, protocols)
    {
        MAVLinkProtocol* mavlink = dynamic_cast<MAVLinkProtocol*>(protocol);
        if (mavlink)
        {
            MAVLinkSettingsWidget* msettings = new MAVLinkSettingsWidget(mavlink, this);
            ui->tabWidget->addTab(msettings, "MAVLink");
        }
    }

    this->window()->setWindowTitle(tr("QGroundControl Settings"));

    // Reconnect
    ui->reconnectCheckBox->setChecked(MainWindow::instance()->autoReconnectEnabled());
    connect(ui->reconnectCheckBox, SIGNAL(clicked(bool)), MainWindow::instance(), SLOT(enableAutoReconnect(bool)));

    // Style
    MainWindow::QGC_MAINWINDOW_STYLE style = (MainWindow::QGC_MAINWINDOW_STYLE)MainWindow::instance()->getStyle();
    switch (style)
    {
    case MainWindow::QGC_MAINWINDOW_STYLE_NATIVE:
        ui->nativeStyle->setChecked(true);
        break;
    case MainWindow::QGC_MAINWINDOW_STYLE_INDOOR:
        ui->indoorStyle->setChecked(true);
        break;
    case MainWindow::QGC_MAINWINDOW_STYLE_OUTDOOR:
        ui->outdoorStyle->setChecked(true);
        break;
    }
    connect(ui->nativeStyle, SIGNAL(clicked()), MainWindow::instance(), SLOT(loadNativeStyle()));
    connect(ui->indoorStyle, SIGNAL(clicked()), MainWindow::instance(), SLOT(loadIndoorStyle()));
    connect(ui->outdoorStyle, SIGNAL(clicked()), MainWindow::instance(), SLOT(loadOutdoorStyle()));

    connect(ui->netBlocker, SIGNAL(stateChanged(int)), MapNetBlocker::instance(), SLOT(setBlock(int)));

    // Close / destroy
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(deleteLater()));

    // Set layout options
    //ui->generalPaneGridLayout->setAlignment(Qt::AlignTop);
}

QGCSettingsWidget::~QGCSettingsWidget()
{
    delete ui;
}
