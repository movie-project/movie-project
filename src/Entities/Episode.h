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

#ifndef EPISODE_H
#define EPISODE_H

#include "Entities/Movie.h"
#include "Entities/Show.h"

class Movie;
class Show;

/**
 * @brief The Episode class
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
class Episode
{
public:
    explicit Episode();
    int id() const;
    void setId(const int id);
    int number() const;
    void setNumber(const int number);
    Movie movie() const;
    void setMovie(const Movie &movie);
    int season() const;
    void setSeason(const int season);
    Show show() const;
    void setShow(const Show &show);
    bool operator== (const Episode&);
    bool operator!= (const Episode&);

private:
    int m_id;
    Show m_show;
    Movie m_movie;
    int m_season;
    int m_number;
};

#endif // EPISODE
