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

    static const char* DIO1;	/**< Digital IO brick 1 (at I2C address 0x48) */
    static const char* DIO2;	/**< Digital IO brick 2 (at I2C address 0x49) */
    static const char* DIO3;	/**< Digital IO brick 3 (at I2C address 0x4A) */
    static const char* DIO4;	/**< Digital IO brick 4 (at I2C address 0x4B) */

    static const char* DCM1;	/**< DC Motor brick 1 (at I2C address 0x4C) */
    static const char* DCM2;	/**< DC Motor brick 2 (at I2C address 0x4D) */
    static const char* DCM3;	/**< DC Motor brick 3 (at I2C address 0x4E) */
    static const char* DCM4;	/**< DC Motor brick 4 (at I2C address 0x4F) */

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

    /**
     * \brief Get type for this brick.
     *
     *
     * \return 	Type of this brick, or bol::BrickType::UNKNOWN if brick could not be identified.
     */
    BrickType getType();

    /**
     * \brief Get firmware version of this brick (0..255).
     *
     *
     * \return 	Firmware version of this brick.
     */
    unsigned char getFirmwareVersion();

    /**
     * \brief Reset this brick.
     *
     * \warning throws bol::BrickException if brick is not reachable.
     */
    void reset();

    /**
     * \brief Get a port from this brick by its name.
     *
     * A port is represented by a string (e.g. "DO1" for digital output 1 on a DIO brick).
     * Note, that each brick defines some constants for the port names it knows
     * (e.g. "DO1" is bol::DioBrick::DO1.
     *
     * \warning throws bol::BrickException if brick has no port with the given name.
     *
     * \param name		Name of port to get.
     *
     * \return			The port definition for the given name.
     */
    BrickPort *getPortByName(const char *name);

    /**
     * \brief Get all available ports of this brick.
	 *
	 * \return			A map of ports defined by this brick.
     */
    BrickPortMap *getPorts();

    /**
     * \brief Set the value of a named port for this brick.
     *
     * A port is represented by a string (e.g. "DO1" for digital output 1 on a DIO brick).
     * Note, that each brick defines some constants for the port names it knows
     * (e.g. "DO1" is bol::DioBrick::DO1).
     *
     * A value is always an integer value (e.g. for a digital output 0 or 1). Note that
     * not all ports allow all values, and that some ports define constants for their
     * known values (e.g. bol::DioBrick::HIGH for 1, bol::DioBrick::LOW for 0).
     *
     * \warning throws bol::BrickException if brick has no port with the given name.
     *
     * \param name		Name of port to get.
     * \param value	Value which should be assigned to port.
     */
    void setPortValue(const char *name, int value);

    /**
     * \brief Get the value of a named port from this brick.
     *
     * A port is represented by a string (e.g. "DI1" for digital input 1 on a DIO brick).
     * Note, that each brick defines some constants for the port names it knows
     * (e.g. "DI1" is bol::DioBrick::DI1).
     *
     * A value is always an integer value (e.g. for a digital output 0 or 1). Note that
     * not all ports allow all values, and that some ports define constants for their
     * known values (e.g. bol::DioBrick::HIGH for 1, bol::DioBrick::LOW for 0).
     *
     * The value of every port type could be read. If the port is an input, this function
     * will return the port value read from the brick on last sync interval. If the port
     * is an output, this function will return the value send to the brick with the
     * last sync interval.
     *
     * \warning throws bol::BrickException if brick has no port with the given name.
     *
     * \param name		Name of port to get.
     *
     * \return			Current value of that port.
     */
    int  getPortValue(const char *name);

    /**
     * \brief Set the priority of this brick for sync intervall.
     *
     * Every brick is synced through the brick bus. With the sync interval
     * it is possible to thell the bus how often brick should be synced.
     * If sync piority is set to 1, the brick is synced on every time the
     * bus sync is processed. If set to 2, it is only synced every 2nd
     * time and so forth.
     *
     * Note: Output ports are only synced when their values changed on the master.
     * Inputs are always synced (in respect of the sync priority) since the master
     * does not know when a slave changed.
     *
     * \param syncPriority	sync priority for the brick (1 - always, >1 every nth time)
     */
    void setSyncPriority(int syncPriority);

    /**
     * \breif Get the sync priority set with bol::Brick::setSyncPriority.
     *
     * \return	Sync priority of this brick.
     */
    int getSyncPriority();

    /**
     * \brief Describe this brick in JSON.
     *
     * \return	JASON string describing this brick.
     */
    std::string describe();

    /**
     * \brief Shortcut to bol::Brick::getPortByName.
     *
     * Instead of using brick.getPortByName("portName"), one could use
     * brick["portName"].
     *
     * \warning throws bol::BrickException if brick has no port with the given name.
     *
     * \return			The port definition for the given name.
     */
    BrickPort &operator[](const char *name);

    /**
     * \brief Get a named port from a named brick.
     *
     * This version is mainly used by the BOL Python scripts generated from Blockly.
     * From C++ use bol::Brick::getPortByName.
     *
     * \warning throws bol::BrickException if brick/port for given name was not found.
     *
     * \param	brickName		Name of brick.
     * \param	portName		Name of port.
     *
     * \return			The port definition for the given brick/port names.
     */
    static BrickPort *get_port(const char *brickName, const char *portName);

    /**
     * \brief Set value of a named port from a named brick.
     *
     * This version is mainly used by the BOL Python scripts generated from Blockly.
     * From C++ use bol::Brick::setPortValue.
     *
     * \warning throws bol::BrickException if brick/port for given name was not found.
     *
     * \param	brickName		Name of brick.
     * \param	portName		Name of port.
     * \param	value			Value to assign to port.
     */
    static void set_port_value(const char *brickName, const char *portName, const int value);

    /**
     * \brief Get value of a named port from a named brick.
     *
     * This version is mainly used by the BOL Python scripts generated from Blockly.
     * From C++ use bol::Brick::getPortValue.
     *
     * \warning throws bol::BrickException if brick/port for given name was not found.
     *
     * \param	brickName		Name of brick.
     * \param	portName		Name of port.
     *
     * \return		Current value assign to the port.
     */
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

    GenericBrick(const GenericBrick &brick);

public:

    virtual ~GenericBrick();

    BrickType getType();

    unsigned char getFirmwareVersion();

    void reset();

    virtual void invalidate();

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
