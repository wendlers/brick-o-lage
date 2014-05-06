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

#include <stdio.h>
#include <unistd.h>

#include "diobrick.hpp"

/* Commands */
#define CMD_SET_POUT    0x01
#define CMD_GET_PIN     0x02

const int bol::DioBrick::LOW  = 0;
const int bol::DioBrick::HIGH = 1;

const char* bol::DioBrick::DI1 = "DI1";
const char* bol::DioBrick::DI2 = "DI2";
const char* bol::DioBrick::DI3 = "DI3";
const char* bol::DioBrick::DI4 = "DI4";

const char* bol::DioBrick::DO1 = "DO1";
const char* bol::DioBrick::DO2 = "DO2";
const char* bol::DioBrick::DO3 = "DO3";
const char* bol::DioBrick::DO4 = "DO4";

bol::DioBrick::DioBrick(const GenericBrick &brick) : bol::GenericBrick(brick)
{
    pout = 0x00;

    addPort(new BrickPort(DI1, BrickPortType::INPUT));
    addPort(new BrickPort(DI2, BrickPortType::INPUT));
    addPort(new BrickPort(DI3, BrickPortType::INPUT));
    addPort(new BrickPort(DI4, BrickPortType::INPUT));

    addPort(new BrickPort(DO1, BrickPortType::OUTPUT));
    addPort(new BrickPort(DO2, BrickPortType::OUTPUT));
    addPort(new BrickPort(DO3, BrickPortType::OUTPUT));
    addPort(new BrickPort(DO4, BrickPortType::OUTPUT));

    reset();
}

bol::DioBrick::~DioBrick()
{
    reset();
}

void bol::DioBrick::sync(bool out, bool in)
{
    if(!shouldSync())
    {
        return;
    }

    if(out)
    {
        unsigned char _pout = 0x00;

        _pout |= (getPortByName(DO1)->getValue() << 0);
        _pout |= (getPortByName(DO2)->getValue() << 1);
        _pout |= (getPortByName(DO3)->getValue() << 2);
        _pout |= (getPortByName(DO4)->getValue() << 3);

        // only sync outputs when they changed
        if(_pout != pout)
        {
            std::vector<unsigned char> msg1 = {CMD_SET_POUT, _pout};
            bbus->write(address, msg1);
            pout = _pout;
        }
    }

    if(in)
    {
        // always sync inputs
        std::vector<unsigned char> res = bbus->read(address, CMD_GET_PIN, 1);

        getPortByName(DI1)->setValue((res[0] >> 0) & 1);
        getPortByName(DI2)->setValue((res[0] >> 1) & 1);
        getPortByName(DI3)->setValue((res[0] >> 2) & 1);
        getPortByName(DI4)->setValue((res[0] >> 3) & 1);
    }
}
