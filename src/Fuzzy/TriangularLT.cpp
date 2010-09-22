/**
 * \file TriangularLT.cpp
 *
 * \brief LinguisticTerm with triangular membership function
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


#include <Fuzzy/TriangularLT.h>

TriangularLT::TriangularLT(const std::string name,
                           const RCPtr<LinguisticVariable>& parent,
                           double                           p1,
                           double                           p2,
                           double                           p3):
		LinguisticTerm(name,parent),TriangularFS(p1,p2,p3) {}
