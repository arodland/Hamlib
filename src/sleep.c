/**
 * \addtogroup rig
 * @{
 */

/**
 * \file src/sleep.c
 * \brief Replacements for sleep and usleep
 * \author Michael Black W9MDB
 * \date 2020
 */
/*
 *  Hamlib Interface - Replacements for sleep and usleep
 *  Copyright (c) 2020 by Michael Black W9MDB
 *
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/**
 * \brief provide sleep and usleep replacements
 * \param same as man page for each
 *
 */
#ifdef HAVE_NANOSLEEP
#include <unistd.h>
#include <errno.h>
#include <time.h>

unsigned int sleep(unsigned int secs)
{
    int retval;
    struct timespec t;
    struct timespec tleft;
    t.tv_sec = secs;
    t.tv_nsec = 0;
    retval = nanosleep(&t, &tleft);

    if (retval == -1) { return tleft.tv_sec; }

    return 0;
}

// Does not have the same 1000000 limit as usleep
int usleep(useconds_t usec)
{
    int retval;
    unsigned long sec = usec / 1000000ul;
    unsigned long nsec = usec * 1000ul - (sec * 1000000000ul);
    struct timespec t;
    t.tv_sec = sec;
    t.tv_nsec = nsec;
    retval = nanosleep(&t, NULL);

    // EINTR is the only error return usleep should need
    // since EINVAL should not be a problem
    if (retval == -1) { return EINTR; }

    return 0;
}
#endif // HAVE_NANOSLEEP
