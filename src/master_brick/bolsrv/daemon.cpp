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
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include "daemon.hpp"

bool daemonize(bool closeStdOut, bool closeStdErr, const char *runDir, const char *lockFile)
{
    int ret;
    int lfp;
    char str[10];

    /* already a daemon? */
    if(getppid() == 1)
    {
        return true;
    }

    ret = fork();

    /* fork error? */
    if(ret < 0)
    {
        std::cerr << "fork failed" << std::endl;
        return false;
    }
    /* parent exit? */
    else if(ret > 0)
    {
        exit(0);
    }

    /* obtain a new process group */
    ret = setsid();

    if(ret < 0)
    {
        std::cerr << "failed to create a new session" << std::endl;
        return false;
    }

    // Close stdin, stdout and stderr
    close(STDIN_FILENO);

    if(closeStdErr)
    {
        close(STDERR_FILENO);
    }

    if(closeStdOut)
    {
        close(STDOUT_FILENO);
    }

    /* set newly created file permissions */
    umask(027);

    /* change running directory */
    ret = chdir(runDir);

    if(ret != 0)
    {
        std::cerr << "chdir to " << runDir << " failed" << std::endl;
        return false;
    }

    lfp = open(lockFile, O_RDWR|O_CREAT, 0640);

    if(lfp < 0)
    {
        std::cerr << "failed to open lockFile " << lockFile << std::endl;
        return false;
    }

    ret = lockf(lfp,F_TLOCK,0);

    if(ret < 0)
    {
        std::cerr << "failed to get lock for lockFile " << lockFile << std::endl;
        return false;
    }

    /* write LockFile */
    sprintf(str,"%d\n",getpid());

    ret = write(lfp, str, strlen(str));

    if(ret < 0)
    {
        std::cerr << "failed write PID to lockFile " << lockFile << std::endl;
        return false;
    }

    return true;
}
