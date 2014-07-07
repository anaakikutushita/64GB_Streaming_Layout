/**********************************************************************************

Copyright (c) 2012, Tan Yu Sheng
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**********************************************************************************/

#include "configwindow.h"
#include "ui_configwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QtDebug>

ConfigWindow::ConfigWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);

    ui->formatGroup->setId(ui->formatXML,SC_XML);
    ui->formatGroup->setId(ui->formatJSON,SC_JSON);
    ui->formatGroup->setId(ui->formatBoth,SC_Both);

    connect(ui->outputButton,SIGNAL( clicked() ),this,SLOT( findOutput() ));
    connect(ui->layoutButton,SIGNAL( clicked() ),this,SLOT( findLayout() ));
    connect(ui->aboutQtButton,SIGNAL( clicked() ),this,SLOT( abtQt() ));
    connect(ui->CDATACheckBox,SIGNAL( stateChanged(int) ),this, SLOT ( CDATAToggle( int ) ));
    connect(ui->altHotkeyCheckbox,SIGNAL( stateChanged(int) ),this, SLOT ( altHotkeyToggle( int ) ));
    connect(ui->formatGroup,SIGNAL( buttonClicked(int) ),this, SLOT ( formatChange( int ) ));

    ui->outputPathTB->setDisabled(true);
    ui->layoutPathTB->setDisabled(true);
    ui->streamControlLabel->setText(QString("StreamControl ") + QString(SC_VERSION) );
    ui->qtLabel->setText(QString("This program uses Qt version ")+ QString(qVersion() + QString(".")) );

}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}

void ConfigWindow::setConfig(QMap<QString, QString> settings) {

    configsettings = settings;
    ui->outputPathTB->setText(configsettings["outputPath"]);
    ui->layoutPathTB->setText(configsettings["layoutPath"]);
    if (configsettings["useCDATA"] == "1") {
        ui->CDATACheckBox->setCheckState(Qt::Checked);
    } else {
        ui->CDATACheckBox->setCheckState(Qt::Unchecked);
    }

    if (configsettings["format"] == QString::number(SC_Both)) {
        ui->formatBoth->setChecked(true);
    } else if (configsettings["format"] == QString::number(SC_JSON)) {
        ui->formatJSON->setChecked(true);
    } else {
        ui->formatXML->setChecked(true);
    }

    if (configsettings["altHotkeyHandling"] == "1") {
        ui->altHotkeyCheckbox->setCheckState(Qt::Checked);
    } else {
        ui->altHotkeyCheckbox->setCheckState(Qt::Unchecked);
    }

}

QMap<QString, QString> ConfigWindow::getConfig() {
    return configsettings;
}

void ConfigWindow::findOutput() {

    QString output = QFileDialog::getExistingDirectory(this, tr("Find Output Directory"), configsettings["outputPath"], QFileDialog::ShowDirsOnly);
    if (output != "") {
        output.replace("/","\\");
        output = output + "\\";
        ui->outputPathTB->setText(output);
        configsettings["outputPath"] = output;
    }
}

void ConfigWindow::findLayout() {

    QString layoutPath = QFileDialog::getOpenFileName(this, tr("Find Layout File"), QFileInfo(configsettings["layoutPath"]).path(), tr("XML Files (*.xml)"));
    if (layoutPath != "") {
        layoutPath.replace("/","\\");
        ui->layoutPathTB->setText(layoutPath);
        configsettings["layoutPath"] = layoutPath;
    } else {
        ui->layoutPathTB->setText(layoutPath);
        configsettings["layoutPath"] = layoutPath;
    }
}

void ConfigWindow::CDATAToggle( int state ) {
    if (state == Qt::Checked) {
        configsettings["useCDATA"] = "1";
    } else {
        configsettings["useCDATA"] = "0";
    }
}

void ConfigWindow::altHotkeyToggle( int state ) {
    if (state == Qt::Checked) {
        configsettings["altHotkeyHandling"] = "1";
    } else {
        configsettings["altHotkeyHandling"] = "0";
    }
}

void ConfigWindow::formatChange( int state ) {
    configsettings["format"] = QString::number(state);
}

void ConfigWindow::abtQt() {
    QMessageBox::aboutQt(this);
}
