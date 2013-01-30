//===========================================================================
/*!
 *
 *  \brief unit test for kernel nearest neighbor regression
 *
 *
 *  \author  T. Glasmachers
 *  \date    2011
 *
 *  \par Copyright (c) 2011:
 *      Institut f&uuml;r Neuroinformatik<BR>
 *      Ruhr-Universit&auml;t Bochum<BR>
 *      D-44780 Bochum, Germany<BR>
 *      Phone: +49-234-32-25558<BR>
 *      Fax:   +49-234-32-14209<BR>
 *      eMail: Shark-admin@neuroinformatik.ruhr-uni-bochum.de<BR>
 *      www:   http://www.neuroinformatik.ruhr-uni-bochum.de<BR>
 *
 *
 *  <BR><HR>
 *  This file is part of Shark. This library is free software;
 *  you can redistribute it and/or modify it under the terms of the
 *  GNU General Public License as published by the Free Software
 *  Foundation; either version 3, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 */
//===========================================================================


#include <iostream>
#include <boost/numeric/ublas/io.hpp>
#define BOOST_TEST_MODULE MODELS_KERNEL_NEAREST_NEIGHBOR_REGRESSION
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <shark/Algorithms/NearestNeighbors/SimpleNearestNeighbors.h>
#include <shark/Models/NearestNeighborRegression.h>
#include <shark/Models/Kernels/GaussianRbfKernel.h>
#include <shark/Models/Trees/KHCTree.h>

using namespace shark;

BOOST_AUTO_TEST_CASE( KERNEL_NEAREST_NEIGHBOR_REGRESSION )
{
	std::vector<RealVector> input(6, RealVector(2));
	input[0](0)=1;
	input[0](1)=3;
	input[1](0)=-1;
	input[1](1)=3;
	input[2](0)=1;
	input[2](1)=0;
	input[3](0)=-1;
	input[3](1)=0;
	input[4](0)=1;
	input[4](1)=-3;
	input[5](0)=-1;
	input[5](1)=-3;
	std::vector<RealVector> target(6, RealVector(1));
	target[0](0)= -1.0;
	target[1](0)= +1.0;
	target[2](0)= -1.0;
	target[3](0)= +1.0;
	target[4](0)= -1.0;
	target[5](0)= +1.0;

	RegressionDataset dataset(input, target);

	DenseRbfKernel kernel(0.5);
	SimpleNearestNeighbors<RealVector,RealVector> algorithm(dataset, &kernel);
	NearestNeighborRegression<RealVector> model(&algorithm, 3);

	for (std::size_t i = 0; i<6; ++i)
	{
		RealVector prediction = model(input[i]);
		BOOST_CHECK_SMALL(target[i](0) - 3.0 * prediction(0), 1e-12);
	}

	RealVector param;
//~ #ifdef DEBUG
	//~ BOOST_CHECK_THROW(model.setParameterVector(param), Exception);
//~ #endif
	//~ param.resize(1);
	//~ param(0) = 3.0;
	//~ BOOST_CHECK_NO_THROW(model.setParameterVector(param));
//~ #ifdef DEBUG
	//~ param(0) = 4.5;
	//~ BOOST_CHECK_THROW(model.setParameterVector(param), Exception);
	//~ param(0) = -1.0;
	//~ BOOST_CHECK_THROW(model.setParameterVector(param), Exception);
//~ #endif
}
