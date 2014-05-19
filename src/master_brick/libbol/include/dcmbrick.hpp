/*
 * This file is part of the Brick-o-Lage project.
 *
 * Copyright (C) 2014 Stefan Wendler <sw@kaltpost.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DCMBRICK_HPP__
#define __DCMBRICK_HPP__

#include "brick.hpp"

namespace bol {

class DcmBrick : public GenericBrick
{
private:

    DcmBrick(const GenericBrick &brick);

    virtual ~DcmBrick();

    unsigned char dir;

    unsigned char speed_m1_cw;
    unsigned char speed_m1_ccw;
    unsigned char speed_m2_cw;
    unsigned char speed_m2_ccw;

public:

    static const int CW;
    static const int CCW;
    static const int STOP;

    static const char* DIR_M1;
    static const char* DIR_M2;

    static const char* SPEED_M1_CW;
    static const char* SPEED_M1_CCW;
    static const char* SPEED_M2_CW;
    static const char* SPEED_M2_CCW;

    void invalidate();

protected:

    void sync(bool out = true, bool in = true);

    friend class BrickBus;
};

}

#endif
