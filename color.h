/*************************************************************************
	> File Name: color.h
	> Author: 
	> Mail: 
	> Created Time: Tue 03 Aug 2021 11:07:40 PM CST
 ************************************************************************/

#ifndef _COLOR_H
#define _COLOR_H

#define NONE "\033[m"
#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"

#ifdef DEBUG
#define DBG(format, arg...) printf(format, ##arg)
#else
#define DBG
#endif

#endif
