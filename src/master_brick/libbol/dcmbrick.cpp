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

#include "dcmbrick.hpp"

/* Commands */
#define CMD_SET_SPEED   0x01				// set speed (DC %) for the motor channels
#define CMD_SET_DIR     0x02				// set motor direction (STOP, CW or CCW)

const int bol::DcmBrick::CW		= 0b01;
const int bol::DcmBrick::CCW	= 0b10;
const int bol::DcmBrick::STOP	= 0b11;

const char* bol::DcmBrick::DIR_M1			= "DIR_M1";
const char* bol::DcmBrick::DIR_M2			= "DIR_M2";

const char* bol::DcmBrick::SPEED_M1_CW		= "SPEED_M1_CW";
const char* bol::DcmBrick::SPEED_M1_CCW		= "SPEED_M1_CCW";
const char* bol::DcmBrick::SPEED_M2_CW		= "SPEED_M2_CW";
const char* bol::DcmBrick::SPEED_M2_CCW		= "SPEED_M2_CCW";

bol::DcmBrick::DcmBrick(const GenericBrick &brick) : bol::GenericBrick(brick)
{

    dir = 0x00;

    speed_m1_cw 	= 0x00;
    speed_m1_ccw	= 0x00;
    speed_m2_cw		= 0x00;
    speed_m2_ccw	= 0x00;

    addPort(new BrickPort(DIR_M1, BrickPortType::INPUT));
    addPort(new BrickPort(DIR_M2, BrickPortType::INPUT));

    addPort(new BrickPort(SPEED_M1_CW, BrickPortType::INPUT));
    addPort(new BrickPort(SPEED_M1_CCW, BrickPortType::INPUT));
    addPort(new BrickPort(SPEED_M2_CW, BrickPortType::INPUT));
    addPort(new BrickPort(SPEED_M2_CCW, BrickPortType::INPUT));

    reset();
}

bol::DcmBrick::~DcmBrick()
{
    reset();
}

void bol::DcmBrick::sync(bool out, bool in)
{
    if(!shouldSync())
    {
        return;
    }

    if(out)
    {
        unsigned char _dir = 0x00;

        unsigned char _speed_m1_cw 	= 0x00;
        unsigned char _speed_m1_ccw = 0x00;
        unsigned char _speed_m2_cw 	= 0x00;
        unsigned char _speed_m2_ccw = 0x00;

        _dir 			|= (getPortByName(DIR_M1)->getValue() << 2);
        _dir 			|= (getPortByName(DIR_M2)->getValue());

        _speed_m1_cw 	|= (getPortByName(SPEED_M1_CW)->getValue());
        _speed_m1_ccw 	|= (getPortByName(SPEED_M1_CCW)->getValue());
        _speed_m2_cw 	|= (getPortByName(SPEED_M2_CW)->getValue());
        _speed_m2_ccw 	|= (getPortByName(SPEED_M2_CCW)->getValue());

        // only sync dir when they changed
        if(_dir != dir)
        {
            std::vector<unsigned char> msg1 = {CMD_SET_DIR, _dir};
            bbus->write(address, msg1);
            dir = _dir;
        }

        // only sync speed when changed
        if(_speed_m1_cw != speed_m1_cw)
        {
            std::vector<unsigned char> msg1 = {CMD_SET_SPEED, _speed_m1_cw, 0b00001000};
            bbus->write(address, msg1);
            speed_m1_cw = _speed_m1_cw;
        }

        // only sync speed when changed
        if(_speed_m1_ccw != speed_m1_ccw)
        {
            std::vector<unsigned char> msg1 = {CMD_SET_SPEED, _speed_m1_ccw, 0b00000100};
            bbus->write(address, msg1);
            speed_m1_ccw = _speed_m1_ccw;
        }

        // only sync speed when changed
        if(_speed_m2_cw != speed_m2_cw)
        {
            std::vector<unsigned char> msg1 = {CMD_SET_SPEED, _speed_m2_cw, 0b00000010};
            bbus->write(address, msg1);
            speed_m2_cw = _speed_m2_cw;
        }

        // only sync speed when changed
        if(_speed_m2_ccw != speed_m2_ccw)
        {
            std::vector<unsigned char> msg1 = {CMD_SET_SPEED, _speed_m2_ccw, 0b00000001};
            bbus->write(address, msg1);
            speed_m2_ccw = _speed_m2_ccw;
        }
    }
}
