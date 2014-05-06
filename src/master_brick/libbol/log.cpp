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

#include "log.hpp"

bol::Log* bol::Log::instance = NULL;

bol::Log::Log(int logLevel)
{
	level = logLevel;
}

bol::Log::~Log()
{
}

void bol::Log::initialize(bol::Log *logInstance, int logLevel)
{
	instance = logInstance;
	instance->level = logLevel;
}

void bol::Log::setLevel(int logLevel)
{
	instance->level = logLevel;
}

int bol::Log::getLevel()
{
	return instance->level;
}

void bol::Log::print(int priority, const char *format, ...)
{
	if(instance == NULL || priority > instance->level)
	{
		return;
	}

	va_list argptr;
	va_start(argptr, format);
	instance->_print(priority, format, argptr);
	va_end(argptr);
}

bool bol::Log::shouldPrint(int priority)
{
	return (priority <= level);
}

bol::LogSysLog bol::LogSysLog::instance;

bol::LogSysLog::LogSysLog(int logLevel) : bol::Log(logLevel)
{
	openlog("bol", LOG_PID | LOG_CONS, LOG_USER);
}

bol::Log *bol::LogSysLog::getInstance()
{
	return &instance;
}

bol::LogSysLog::~LogSysLog()
{
	closelog();
}

void bol::LogSysLog::_print(int priority, const char *format, va_list argptr)
{
	vsyslog(priority, format, argptr);
}


bol::LogStdOut bol::LogStdOut::instance;

bol::LogStdOut::LogStdOut(int logLevel) : bol::Log(logLevel)
{
}

bol::Log *bol::LogStdOut::getInstance()
{
	return &instance;
}

#define RESET		0
#define BRIGHT 		1
#define DIM			2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED			1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

void textcolor(int attr, int fg, int bg)
{
	printf("%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
}

void bol::LogStdOut::_print(int priority, const char *format, va_list argptr)
{
	textcolor(BRIGHT, WHITE, BLACK);
	printf("[BOL]");

	switch(priority)
	{
	case Log::ERR:
		textcolor(BRIGHT, RED, BLACK);
		printf("[E] ");
		break;
	case Log::WARNING:
		textcolor(BRIGHT, MAGENTA, BLACK);
		printf("[W] ");
		break;
	case Log::INFO:
		textcolor(BRIGHT, YELLOW, BLACK);
		printf("[I] ");
		break;
	case Log::DEBUG:
		textcolor(BRIGHT, GREEN, BLACK);
		printf("[D] ");
		break;
	}
	textcolor(RESET, WHITE, BLACK);

	vprintf(format, argptr);
	printf("\n");
}
