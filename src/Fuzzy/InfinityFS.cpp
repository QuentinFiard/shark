
/**
 * \file InfinityFS.cpp
 *
 * \brief FuzzySet with a step function as membership function
 * 
 * \authors Marc Nunkesser
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * On Debian systems, the full text of the GNU General Public
 * License version 2 can be found in the file
 * `/usr/share/common-licenses/GPL-2'.
 *
 */

/* $log$ */

#include <Fuzzy/InfinityFS.h>

InfinityFS::InfinityFS(bool b,double d,double f):
		positiveInfinity(b),a(d),b(f) {};

double  InfinityFS::mu( double x ) const {
	if (a>b) {
		throw(FuzzyException(1,"Illegal parameters for triangular MF --> a > b"));
	};
	if (positiveInfinity)
		if (x>=b)
			return(1);
		else
			return((x-a)/(b-a)*(a<=x));
	else
		if (x<=a)
			return(1);
		else
			return((b-x)/(b-a)*(x<=b));
}

void InfinityFS::setParams(bool b,double d,double f) {
	positiveInfinity = b;
	this->a = d;
	this->b =f;
}
