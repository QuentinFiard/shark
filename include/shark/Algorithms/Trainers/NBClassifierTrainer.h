//===========================================================================
/*!
 *  \brief Trainer of Naive Bayes classifier
 *
 *
 *  \author  B. Li
 *  \date    2012
 *
 *  \par Copyright (c) 2012:
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
#ifndef SHARK_ALGORITHMS_TRAINERS_NB_CLASSIFIER_TRAINER_H
#define SHARK_ALGORITHMS_TRAINERS_NB_CLASSIFIER_TRAINER_H

#include "shark/Algorithms/Trainers/AbstractTrainer.h"
#include "shark/Algorithms/Trainers/Distribution/GenericDistTrainer.h"
#include "shark/Core/Exception.h"
#include "shark/Models/NBClassifier.h"

#include <boost/foreach.hpp>

#include <cmath>

namespace shark {

/// @brief Trainer for naive Bayes classifier
///
/// Basically NB trainer needs to figure out two things for NB classifier:
/// (1) Prior probability of each class
/// (2) Parameters for distributions of each feature given each class
///
/// @tparam InputType the type of feature vector
/// @tparam OutputType the type of class
template <class InputType = RealVector, class OutputType = unsigned int>
class NBClassifierTrainer
:public AbstractTrainer<NBClassifier<InputType, OutputType> >
{
private:

	typedef NBClassifier<InputType, OutputType> NBClassifierType;
	typedef typename InputType::value_type InputValueType;

public:

	/// @see AbstractModel::train
	void train(NBClassifierType& model, LabeledData<InputType, OutputType> const& dataset)
	{
		SIZE_CHECK(dataset.size() > 0u);

		// Get size of class/feature
		std::size_t classSize;
		std::size_t featureSize;
		boost::tie(classSize, featureSize) = model.getDistSize();
		THROW_IF(classSize != numberOfClasses(dataset), "Size of class in dataset and model should match.");
		THROW_IF(featureSize != inputDimension(dataset), "Size of feature in dataset and model should match.");

		// Initialize trainer & buffer
		std::vector<InputValueType> buffer;
		buffer.reserve(dataset.size() / classSize);

		// Train individual feature distribution
		for (std::size_t i = 0; i < classSize; ++i)
		{
			for (std::size_t j = 0; j < featureSize; ++j)
			{
				AbstractDistribution& dist = model.getFeatureDist(i, j);
				buffer.clear();
				getFeatureSample(buffer, dataset, i, j);
				m_distTrainer.train(dist, buffer);
			}
		}

		// Figure out class distribution and add it to the model
		std::vector<std::size_t> occuranceCounter = classSizes(dataset);

		double totalClassOccurances = dataset.numberOfElements();
		for (std::size_t i = 0; i < classSize; ++i) {
			model.setClassPrior(i,occuranceCounter[i]/ totalClassOccurances);
		}
	}

	/// Return the distribution trainer container which allows user to check or set individual distribution trainer
	DistTrainerContainer& getDistTrainerContainer() { return m_distTrainer; }

private:

	/// Get samples for a given feature in a given class
	/// @param samples[out] the container which will store the samples we want to get
	/// @param dataset the entire dataset
	/// @param classIndex the index of class we are interested in
	/// @param featureIndex the index of feature we are interested in
	///
	/// @note This can/should be optimized
	void getFeatureSample(
		std::vector<InputValueType>& samples,
		const LabeledData<InputType, OutputType>& dataset,
		OutputType classIndex,
		std::size_t featureIndex
	) const{
		typedef typename  LabeledData<InputType, OutputType>::const_element_reference reference;
		BOOST_FOREACH(reference elem,dataset.elements()){
			if (elem.label == classIndex)
				samples.push_back(elem.input(featureIndex));
		}
	}

	/// Generic distribution trainer
	GenericDistTrainer m_distTrainer;
};

} // namespace shark {

#endif // SHARK_ALGORITHMS_TRAINERS_NB_CLASSIFIER_TRAINER_H
