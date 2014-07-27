/* Copyright (C) 2014 Movie-Project
 * (Olivier CHURLAUD)
 *
 * This file is part of Movie-Project.
 *
 * Movie-Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Movie-Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Movie-Project.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QDialog *parent) : QDialog(parent)
{
    m_app = qobject_cast<Application *>(qApp);
    resize(400,300);
    setWindowTitle("Settings");
    m_mainLayout = new QVBoxLayout(this);
    m_filesPathLayout = new QHBoxLayout;
    m_filesPathMessage = new QLabel("");

    m_filesPathLabel = new QLabel("Files path");
    m_filesPathEdit = new QLineEdit;
    m_filesPathEdit->setText(m_app->getFilesPath());
    m_filesPathSearchButton = new QPushButton("Browse");
    QObject::connect(m_filesPathSearchButton, SIGNAL(clicked()), this, SLOT(browseFilesPathDialog()));

    m_knownPathList = new QListView;
    m_knownPathList->setModel(m_app->getDataBaseManager()->getMoviesPathModel());

    m_submitButton = new QPushButton("Submit");
    QObject::connect(m_submitButton, SIGNAL(clicked()), this, SLOT(applySetting()));

    m_mainLayout->addLayout(m_filesPathLayout);
    m_mainLayout->addWidget(m_filesPathMessage);
    m_mainLayout->addWidget(m_knownPathList);
    m_mainLayout->addWidget(m_submitButton);
    m_filesPathLayout->addWidget(m_filesPathLabel);
    m_filesPathLayout->addWidget(m_filesPathEdit);
    m_filesPathLayout->addWidget(m_filesPathSearchButton);
}

SettingsWindow::~SettingsWindow()
{
}

void SettingsWindow::applySetting()
{
    if(QDir(m_filesPathEdit->text()).exists())
    {
        Application *l_app = qobject_cast<Application *>(qApp);
        l_app->setFilesPath(m_filesPathEdit->text());
        close();
    }
    else
    {
        m_filesPathEdit->setText("");
        m_filesPathMessage->setText("Choose an existant path");
    }
}

void SettingsWindow::browseFilesPathDialog()
{
    QString l_folder = QFileDialog::getExistingDirectory();
    m_filesPathEdit->setText(l_folder);
}
void SettingsWindow::closeEvent(QCloseEvent *event)
{
  emit closed();
  event->accept();
}
