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

#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <unistd.h>
#include <syslog.h>
#include <stdarg.h>

#define BLOG_INIT_SYSLOG(LEVEL)	bol::Log::initialize(bol::LogSysLog::getInstance(), LEVEL)
#define BLOG_INIT_STDOUT(LEVEL)	bol::Log::initialize(bol::LogStdOut::getInstance(), LEVEL)
#define BLOG_ERR(...)			bol::Log::print(bol::Log::ERR, __VA_ARGS__)
#define BLOG_WARNING(...)		bol::Log::print(bol::Log::WARNING, __VA_ARGS__)
#define BLOG_INFO(...)			bol::Log::print(bol::Log::INFO, __VA_ARGS__)
#define BLOG_DEBUG(...)			bol::Log::print(bol::Log::DEBUG, __VA_ARGS__)

namespace bol
{

class Log
{
private:

    int level;

    static Log *instance;

protected:

    Log(int logLevel = Log::DEBUG);

public:


    enum {
        ERR=LOG_ERR,
        WARNING=LOG_WARNING,
        INFO=LOG_INFO,
        DEBUG=LOG_DEBUG,
    };

    virtual ~Log();

    static void initialize(Log *logInstance, int logLevel = Log::DEBUG);

    static void setLevel(int logLevel);

    static int getLevel();

    static void print(int priority, const char *format, ...);

protected:

    virtual void _print(int priority, const char *format, va_list argptr) = 0;

    virtual bool shouldPrint(int priority);
};

class LogSysLog : public Log
{
private:

    static LogSysLog instance;

public:

    static Log *getInstance();

    ~LogSysLog();

protected:

    LogSysLog(int logLevel = Log::DEBUG);

    void _print(int priority, const char *format, va_list argptr);
};

class LogStdOut : public Log
{
private:

    static LogStdOut instance;

public:

    static Log *getInstance();

protected:

    LogStdOut(int logLevel = Log::DEBUG);

    void _print(int priority, const char *format, va_list argptr);
};

}

#endif
