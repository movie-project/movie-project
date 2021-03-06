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

#ifndef MAINPANNEL_H
#define MAINPANNEL_H

#include <QWidget>

class QFile;

class Movie;

/**
 * @brief The MainPannel class
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
class MainPannel : public QWidget
{
    Q_OBJECT

public:
    explicit MainPannel(QWidget *parent);
    virtual void fill(QList<Movie> const &movieList){ movieList.count(); }

signals:
    void fillMetadataPannel(const Movie&);
    void updatePannels();
    void startFetchingMetadata(const QList<Movie>&);

protected:
    bool permanentlyDeleteFile(QFile *movieFileToDelete);
    bool moveFileToTrash(QList<Movie> &movieList);

private:
    QString unix_findTrashFolder(QString movieFilePath);
    bool linux_moveFileToTrash(QString movieFilePath);
    bool windows_moveFileToTrash(QString movieFilePath);
    bool macosx_moveFileToTrash(QString movieFilePath);
};

#endif // MAINPANNEL_H
