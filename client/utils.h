/*
 * utils.h
 *
 *  Created on: 6 mar 2015
 *      Author: Tomek
 */

#ifndef UTILS_H_
#define UTILS_H_

#ifdef __UTILS__

void randstring(char * const data,int length);
void error(const char *msg);

#else

extern void randstring(char * const data,int length);
extern void error(const char *msg);

#endif



#endif /* UTILS_H_ */
