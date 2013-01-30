/*
* <BR><HR>
* This file is part of Shark. This library is free software;
* you can redistribute it and/or modify it under the terms of the
* GNU General Public License as published by the Free Software
* Foundation; either version 3, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this library; if not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SHARK_OBJECTIVEFUNCTIONS_BENCHMARK_DIFFPOWERS_H
#define SHARK_OBJECTIVEFUNCTIONS_BENCHMARK_DIFFPOWERS_H

#include <shark/ObjectiveFunctions/AbstractObjectiveFunction.h>
#include <shark/Core/SearchSpaces/VectorSpace.h>
#include <shark/Rng/GlobalRng.h>

namespace shark {
	struct DiffPowers : public AbstractObjectiveFunction< VectorSpace<double>,double > {

		DiffPowers(unsigned int numberOfVariables = 5) {
			m_name="DiffPowers";
			m_features |= CAN_PROPOSE_STARTING_POINT;
		}

		void configure( const PropertyTree & node ) {
		}

		void proposeStartingPoint( SearchPointType & x ) const {
			x.resize( m_numberOfVariables );

			for( unsigned int i = 0; i < x.size(); i++ ) {
				x( i ) = Rng::uni( 0, 1 );
			}
		}

		double eval( const SearchPointType & p ) const {
			m_evaluationCounter++;
			double sum = 0;
			for( unsigned int i = 0; i < p.size(); i++ ){
				sum += ::pow( ::fabs( p( i ) ), 2. + (10.*i) / (p.size() - 1.) );
			}
			return sum;
		}
	};

	ANNOUNCE_SINGLE_OBJECTIVE_FUNCTION( DiffPowers, shark::soo::RealValuedObjectiveFunctionFactory );
}

#endif // SHARK_EA_SPHERE_H
