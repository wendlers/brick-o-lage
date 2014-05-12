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

#ifndef __BRICK_HPP__
#define __BRICK_HPP__

#include <exception>

#include "brickbus.hpp"
#include "brickport.hpp"

namespace bol {

enum class BrickType
{
    UNKNOWN = 0x00, /**< UNKNOWN brick */
    DIO 	= 0x01, /**< Digital IO brick */
    DCM 	= 0x02, /**< DC motor control brick */
    SER 	= 0x03, /**< Servo control brick */
    SEN 	= 0x04, /**< Sensor brick */
    ANY 	= 0xFF, /**< Placeholder for any brick */
};

/**
 * \brief Proxy to a specific brick.
 *
 * This is the main interface to the user when working with bricks. Each brick keeps an
 * internal instance of the specific brick it proxies for.
 */
class Brick
{
private:

    GenericBrick *brick;	/**< Specific brick instance which is proxied by this Brick. */

public:

    static const char* DIO1;
    static const char* DIO2;
    static const char* DIO3;
    static const char* DIO4;

    static const char* DCM1;
    static const char* DCM2;
    static const char* DCM3;
    static const char* DCM4;

    /**
     * \brief Construct instance for brick at given I2C address.
     *
     * \warning throws bol::BrickException if brick at address is not found.
     *
     * \param slaveAddress	I2C 7-Bit slave address
     */
    Brick(int slaveAddress);

    /**
     * \brief Construct instance for brick at given I2C address, also check if brick is of given type.
     *
     * \warning throws bol::BrickException if brick at address is not found or brick has not specified bol::BrickType.
     *
     * \param slaveAddress	I2C 7-Bit slave address
     * \param type			bol::BrickType which the brick needs to match
     */
    Brick(int slaveAddress, BrickType type);

    /**
     * \brief Construct instance for brick with a given name.
     *
     * \warning throws bol::BrickException if brick name could not be resolved to implied I2C slaveAddress and bol::BrickType.
     *
     * \param name 	name of brick
     */
    Brick(const char *name);

    BrickType getType();

    unsigned char getFirmwareVersion();

    void reset();

    BrickPort *getPortByName(const char *name);

    BrickPortMap *getPorts();

    void setPortValue(const char *name, int value);

    int  getPortValue(const char *name);

    void setSyncPriority(int syncPriority);

    int getSyncPriority();

    std::string describe();

    BrickPort &operator[](const char *name);

    static BrickPort *get_port(const char *brickName, const char *portName);

    static void set_port_value(const char *brickName, const char *portName, const int value);

    static int get_port_value(const char *brickName, const char *portName);
};

class GenericBrick
{
protected:

    BrickBus *bbus;

    int address;

    int priority;

    int currentPriority;

    int	fwVersion;

    BrickType type;

    BrickPortMap pmap;

    GenericBrick(BrickBus *brickBus, int slaveAddress);

protected:

    GenericBrick(const GenericBrick &brick);

public:

    virtual ~GenericBrick();

    BrickType getType();

    unsigned char getFirmwareVersion();

    void reset();

    BrickPort *getPortByName(const char *name);

    BrickPortMap *getPorts();

    void setPortValue(const char *name, int value);

    int  getPortValue(const char *name);

    void setSyncPriority(int syncPriority);

    int getSyncPriority();

    std::string describe();

    BrickPort &operator[](const char *name);

protected:

    virtual void sync(bool out = true, bool in = true);

    bool shouldSync();

    void addPort(BrickPort *port);

    friend class BrickBus;
};

}

#endif
