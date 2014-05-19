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

#include <sstream>

#include "log.hpp"
#include "brick.hpp"
#include "brickexception.hpp"

#define CMD_RESET       0xA0
#define CMD_FW_TYPE		0xF0
#define CMD_FW_VERSION	0xF1

const char* bol::Brick::DIO1 = "DIO1";
const char* bol::Brick::DIO2 = "DIO2";
const char* bol::Brick::DIO3 = "DIO3";
const char* bol::Brick::DIO4 = "DIO4";

const char* bol::Brick::DCM1 = "DCM1";
const char* bol::Brick::DCM2 = "DCM2";
const char* bol::Brick::DCM3 = "DCM3";
const char* bol::Brick::DCM4 = "DCM4";

bol::GenericBrick::GenericBrick(BrickBus *brickBus, int slaveAddress)
{
    bbus = brickBus;
    address = slaveAddress;

    priority = 3;
    currentPriority = 0;

    fwVersion = -1;
    type = BrickType::UNKNOWN;
}

bol::GenericBrick::~GenericBrick()
{
    BrickPortMap::iterator it;

    for(it = pmap.begin(); it != pmap.end(); ++it)
    {
        BrickPort *p = it->second;
        delete p;
    }
}

void bol::GenericBrick::addPort(BrickPort *port)
{
    pmap[port->getName()] = port;
}

bol::GenericBrick::GenericBrick(const bol::GenericBrick &brick)
{
    bbus 		= brick.bbus;
    address 	= brick.address;
    pmap		= brick.pmap;

    priority 	= brick.priority;
    currentPriority	= 0;

    fwVersion = brick.fwVersion;
    type = brick.type;
}

bol::BrickType bol::GenericBrick::getType()
{
    if(type != BrickType::UNKNOWN)
    {
        return type;
    }

    std::vector<unsigned char> res = bbus->read(address, CMD_FW_TYPE, 1);

    switch(res[0]) {
    case 0x01:
        type = BrickType::DIO;
        break;
    case 0x02:
        type = BrickType::DCM;
        break;
    default:
        type = BrickType::UNKNOWN;
    }

    return type;
}

unsigned char bol::GenericBrick::getFirmwareVersion()
{
    if(fwVersion != -1)
    {
        return (unsigned char)fwVersion;
    }

    std::vector<unsigned char> res = bbus->read(address, CMD_FW_VERSION, 1);

    fwVersion = (int)res[0];

    return (unsigned char)fwVersion;
}

void bol::GenericBrick::reset()
{
    std::vector<unsigned char> msg = {CMD_RESET};
    bbus->write(address, msg);
    usleep(5000);
}

void bol::GenericBrick::invalidate()
{
}

bol::BrickPort *bol::GenericBrick::getPortByName(const char *name)
{
    if(pmap[name] == NULL)
    {
        BLOG_ERR("Invalid port name: %s", name);
        throw BrickException("Invalid port name");
    }

    return pmap[name];
}

bol::BrickPortMap *bol::GenericBrick::getPorts()
{
    return &pmap;
}

void bol::GenericBrick::setPortValue(const char *name, int value)
{
    getPortByName(name)->setValue(value);
}

int bol::GenericBrick::getPortValue(const char *name)
{
    return getPortByName(name)->getValue();
}

std::string bol::GenericBrick::describe()
{
    std::stringstream d;

    d << "{\"Brick\": " << std::endl;

    d << "{\"address\"=" << address;

    d << ", \"type\"=";

    BrickType type = getType();

    if(type == BrickType::DIO)
    {
        d << "\"DIO\"";
    }
    else if(type == BrickType::DCM)
    {
        d << "\"DCM\"";
    }
    else if(type == BrickType::SER)
    {
        d << "\"SER\"";
    }
    else if(type == BrickType::SEN)
    {
        d << "\"SEN\"";
    }
    else
    {
        d << "\"UNKNOWN\"";
    }

    d << ", \"fw-version\"=" << (int)getFirmwareVersion();
    d << ", " << std::endl << "\"ports\"=[" << std::endl;

    BrickPortMap::iterator it;

    for(it = pmap.begin(); it != pmap.end(); ++it)
    {
        BrickPort *p = it->second;
        d << p->describe();
        d << ", " << std::endl;
    }

    d << "]";

    d << "}}";

    return d.str();
}

void bol::GenericBrick::setSyncPriority(int syncPriority)
{
    priority 		= syncPriority;
    currentPriority = 0;
}

int bol::GenericBrick::getSyncPriority()
{
    return priority;
}

bol::BrickPort &bol::GenericBrick::operator [](const char* name)
{
    return *getPortByName(name);
}

void bol::GenericBrick::sync(bool out, bool in)
{
}

bool bol::GenericBrick::shouldSync()
{
    if(currentPriority == priority)
    {
        currentPriority = 0;
        return true;
    }

    currentPriority++;

    return false;
}

bol::Brick::Brick(int slaveAddress)
{
    brick = BrickBus::getInstance()->getBrickByAddress(slaveAddress);
}

bol::Brick::Brick(int slaveAddress, bol::BrickType type)
{
    brick = BrickBus::getInstance()->getBrickByAddress(slaveAddress, type);
}

bol::Brick::Brick(const char *name)
{
    brick = BrickBus::getInstance()->getBrickByName(name);
}

bol::BrickType bol::Brick::getType()
{
    return brick->getType();
}

unsigned char bol::Brick::getFirmwareVersion()
{
    return brick->getFirmwareVersion();
}

void bol::Brick::reset()
{
    brick->reset();
}

bol::BrickPort* bol::Brick::getPortByName(const char* name)
{
    return brick->getPortByName(name);
}

bol::BrickPortMap* bol::Brick::getPorts()
{
    return brick->getPorts();
}

void bol::Brick::setPortValue(const char* name, int value)
{
    brick->setPortValue(name, value);
}

int bol::Brick::getPortValue(const char* name)
{
    return brick->getPortValue(name);
}

void bol::Brick::setSyncPriority(int syncPriority)
{
    brick->setSyncPriority(syncPriority);
}

int bol::Brick::getSyncPriority()
{
    return brick->getSyncPriority();
}

std::string bol::Brick::describe()
{
    return brick->describe();
}

bol::BrickPort& bol::Brick::operator [](const char* name)
{
    return *(brick->getPortByName(name));
}

bol::BrickPort *bol::Brick::get_port(const char *brickName, const char *portName)
{
    return BrickBus::getInstance()->getBrickByName(brickName)->getPortByName(portName);
}

void bol::Brick::set_port_value(const char* brickName, const char* portName, const int value)
{
    BrickBus::getInstance()->getBrickByName(brickName)->getPortByName(portName)->setValue(value);
}

int bol::Brick::get_port_value(const char* brickName, const char* portName)
{
    return BrickBus::getInstance()->getBrickByName(brickName)->getPortByName(portName)->getValue();
}
