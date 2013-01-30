//===========================================================================
/*!
 *  \brief cross-validation error for selection of hyper-parameters
 *
 *
 *  \author  T. Glasmachers, O. Krause
 *  \date    2007-2012
 *
 *  \par Copyright (c) 2007-2011:
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

#ifndef SHARK_OBJECTIVEFUNCTIONS_CROSSVALIDATIONERROR_H
#define SHARK_OBJECTIVEFUNCTIONS_CROSSVALIDATIONERROR_H

#include <shark/ObjectiveFunctions/DataObjectiveFunction.h>
#include <shark/Algorithms/Trainers/AbstractTrainer.h>
#include <shark/Algorithms/AbstractSingleObjectiveOptimizer.h>
#include <shark/ObjectiveFunctions/AbstractCost.h>
#include <shark/Data/CVDatasetTools.h>

namespace shark {


///
/// \brief cross-validation error for selection of hyper-parameters
///
/// \par
/// The cross-validation error is useful for evaluating
/// how well a model performs on a problem. It is regularly
/// used for model selection.
///
/// \par
/// In Shark, the cross-validation procedure is abstracted
/// as follows:
/// First, the given point is written into an IParameterizable
/// object (such as a regularizer or a trainer). Then a model
/// is trained with a trainer with the given settings on a
/// number of folds and evaluated on the corresponding validation
/// sets with a cost function. The average cost function value
/// over all folds is returned.
///
/// \par
/// Thus, the cross-validation procedure requires a "meta"
/// IParameterizable object, a model, a trainer, a data set,
/// and a cost function.
///
template<class ModelTypeT, class LabelTypeT = typename ModelTypeT::OutputType>
class CrossValidationError : public AbstractObjectiveFunction< VectorSpace<double>, double >
{
public:
	typedef typename ModelTypeT::InputType InputType;
	typedef typename ModelTypeT::OutputType OutputType;
	typedef LabelTypeT LabelType;
	typedef LabeledData<InputType, LabelType> DatasetType;
	typedef CVFolds<DatasetType> FoldsType;
	typedef ModelTypeT ModelType;
	typedef AbstractTrainer<ModelType, LabelType> TrainerType;
	typedef AbstractCost<LabelType, OutputType> CostType;
private:
	typedef AbstractObjectiveFunction< VectorSpace<double>, double > base_type;


	FoldsType m_folds;
	IParameterizable* mep_meta;
	ModelType* mep_model;
	TrainerType* mep_trainer;
	CostType* mep_cost;

public:

	CrossValidationError(
		FoldsType const& dataFolds,
		IParameterizable* meta,
		ModelType* model,
		TrainerType* trainer,
		CostType* cost)
	: m_folds(dataFolds)
	, mep_meta(meta)
	, mep_model(model)
	, mep_trainer(trainer)
	, mep_cost(cost)
	{
		base_type::m_name = "CrossValidationError<"
				+ mep_model->name() + ","
				+ mep_trainer->name() + ","
				+ mep_cost->name() + ">";
	}

	/// configure the cross validation
	void configure( const PropertyTree & node ) {
		//m_folds = node.get("folds", 1);
	}

	/// Evaluate the cross-validation error:
	/// train sub-models, evaluate objective,
	/// return the average.
	double eval(RealVector const& parameters) const {
		this->m_evaluationCounter++;
		mep_meta->setParameterVector(parameters);

		double ret = 0.0;
		for (size_t setID=0; setID != m_folds.size(); ++setID) {
			DatasetType train =  m_folds.training(setID);
			DatasetType validation =  m_folds.validation(setID);
			mep_trainer->train(*mep_model, train);
			Data<OutputType> output = (*mep_model)(validation.inputs());
			ret += mep_cost->eval(validation.labels(), output);
		}
		return ret / m_folds.size();
	}
};


}
#endif
