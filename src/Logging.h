/*
 * Logging.h
 *
 *  Created on: Oct 5, 2018
 *      Author: milites
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include <stdio.h>

#define IGNORE_PRINTF 1

#if IGNORE_PRINTF
#define printf(fmt, ...) (0)
#endif

#endif /* LOGGING_H_ */
