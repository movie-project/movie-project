/* Copyright (C) 2014 Macaw-Movies
 * (Olivier CHURLAUD, Sébastien TOUZÉ)
 *
 * This file is part of Macaw-Movies.
 *
 * Macaw-Movies is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Macaw-Movies is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Macaw-Movies.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Application.h"

#include <QDir>
#include <QIcon>
#include <QMessageBox>

#include "include_var.h"

#include "MacawDebug.h"
#include "MainWindow.h"
#include "ServicesManager.h"
#include "Entities/People.h"
#include "Entities/Tag.h"
#include "FetchMetadata/FetchMetadata.h"

/**
 * @brief Constructor
 */
Application::Application(int &argc, char **argv) :
    QApplication(argc, argv)
{
    Macaw::DEBUG_IN("[Application] Enters initialization");

    this->setApplicationName(APP_NAME);
    this->setApplicationVersion(APP_VERSION);
    this->setWindowIcon(QIcon(":/img/logov0_1.png"));
    this->definePaths();

    Macaw::DEBUG_OUT("[Application] Initialization done");
}

int Application::exec()
{
    Macaw::DEBUG_IN("[Application] Execution started");

    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    m_tmdbkey = "6e4cbac7861ad5b847ef8f60489dc04e";
    m_mainWindow = new MainWindow;
    m_fetchMetadata = NULL;

    connect(databaseManager, SIGNAL(orphanTagDetected(Tag)),
            this, SLOT(askForOrphanTagDeletion(Tag)));
    connect(databaseManager, SIGNAL(orphanPeopleDetected(People)),
            this, SLOT(askForOrphanPeopleDeletion(People)));
    connect(m_mainWindow, SIGNAL(startFetchingMetadata(QList<Movie>)),
            this, SLOT(on_startFetchingMetadata(QList<Movie>)));
    connect(this, SIGNAL(updateMainWindow()),
            m_mainWindow, SLOT(selfUpdate()));

    m_mainWindow->show();

    int l_execVal = QApplication::exec();

    Macaw::DEBUG_OUT("[Application] Execution exits");

    return l_execVal;
}

/**
 * @brief Destructor
 */
Application::~Application()
{
    Macaw::DEBUG("[Application] Destructed");
}

/**
 * @brief Slot triggered when DatabaseManager finds an orphan tag.
 * A QMessageBox asks the user if the tag should be delete or not.
 *
 * @param orphanTag concerned by the choice
 */
void Application::askForOrphanTagDeletion(const Tag &orphanTag)
{
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(QApplication::tr("The tag <b>%1</b> is not used in any movie now.").arg(orphanTag.name()));
    msgBox.setInformativeText(tr("Do you want to delete this tag?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec() == QMessageBox::Yes) {
        databaseManager->deleteTag(orphanTag);
    }
}

/**
 * @brief Slot triggered when DatabaseManager finds an orphan person.
 * A QMessageBox asks the user if the person should be delete or not.
 *
 * @param orphanPeople concerned by the choice
 */
void Application::askForOrphanPeopleDeletion(const People &orphanPeople)
{
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    QString l_name = orphanPeople.name();

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(QApplication::tr("The person <b>%1</b> is not linked to any movie now.").arg(l_name));
    msgBox.setInformativeText(tr("Do you want to delete it?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec() == QMessageBox::Yes) {
        databaseManager->deletePeople(orphanPeople);
    }
}

/**
 * @brief Slot triggered when the user wants to fetch metadata on the internet
 */
void Application::on_startFetchingMetadata(const QList<Movie> &movieList)
{
    Macaw::DEBUG("[Application] startFetchingMetadata called");
    if(m_fetchMetadata == NULL) {
        Macaw::DEBUG("[Application] Create new FetchingMetadata");
        m_fetchMetadata = new FetchMetadata();
        connect(m_fetchMetadata, SIGNAL(jobDone()),
                this, SLOT(on_fethMetadataJobDone()));
    }

    m_fetchMetadata->addMoviesToQueue(movieList);
}

/**
 * @brief Slot triggered when m_fetchMetadata has finished its job
 */
void Application::on_fethMetadataJobDone()
{
    delete m_fetchMetadata;
    m_fetchMetadata = NULL;
    emit updateMainWindow();
}

/**
 * @brief Slot triggered when m_fetchMetadata finished to update a Movie
 * Updates MainWindow
 */
void Application::on_fethMetadataUpdatedMovie()
{
    emit updateMainWindow();
}

/**
 * @brief Define the paths used in the app
 * Can be retrieved by `qApp->property("name").toString`
 */
void Application::definePaths()
{
    QString l_filesPath = "";

#ifdef Q_OS_LINUX
    // Files in ~/.local/share/macaw-movies and create the folder if not exists
    l_filesPath = QString(QDir::home().path().append(QDir::separator())
                                             .append(".local")
                                             .append(QDir::separator())
                                             .append("share"));
#endif

#ifdef Q_OS_WIN
    // File in $USER\AppData\Local
    l_filesPath = QString(QDir::home().path().append(QDir::separator())
                                            .append("AppData")
                                            .append(QDir::separator())
                                            .append("Local"));
#endif

#ifdef Q_OS_OSX
    // Files in ~/Library/Application Support
    l_filesPath = QString(QDir::home().path().append(QDir::separator())
                                            .append("Library")
                                            .append(QDir::separator())
                                            .append("Application Support"));
#endif

    l_filesPath.append(QDir::separator());
    l_filesPath = QDir::toNativeSeparators(l_filesPath);

    QFileInfo checkFolder(l_filesPath + APP_NAME_SMALL);
    if (!checkFolder.exists())
    {
        QDir(l_filesPath).mkdir(APP_NAME_SMALL);
    }

    l_filesPath.append(APP_NAME_SMALL).append(QDir::separator());

    l_filesPath = QDir::toNativeSeparators(l_filesPath);
    checkFolder = QString(l_filesPath + "posters");
    if (!checkFolder.exists())
    {
        QDir(l_filesPath).mkdir("posters");
    }

    QString l_postersPath = QDir::toNativeSeparators(l_filesPath
                                                     + "posters"
                                                     +  QDir::separator()
                                                     );
    this->setProperty("filesPath", l_filesPath);
    this->setProperty("postersPath", l_postersPath);
}
