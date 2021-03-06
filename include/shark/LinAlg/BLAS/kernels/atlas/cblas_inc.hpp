/*
 *
 * Copyright (c) Kresimir Fresl 2002
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Author acknowledges the support of the Faculty of Civil Engineering,
 * University of Zagreb, Croatia.
 *
 */

//////////////////////////////////////////////////////////////////////////
//
// ATLAS (Automatically Tuned Linear Algebra Software)
//
// ''At present, it provides C and Fortran77 interfaces to a portably
// efficient BLAS implementation, as well as a few routines from LAPACK.''
//
// see: http://math-atlas.sourceforge.net/
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHARK_LINALG_IMPL_NUMERIC_BINDINGS_ATLAS_CBLAS_INC_H
#define SHARK_LINALG_IMPL_NUMERIC_BINDINGS_ATLAS_CBLAS_INC_H

extern "C" {
#include <cblas.h>
#include <clapack.h>
}

//all atlas using functions need this anyway...
//so we prevent multiple includes in all atlas using functions
//which should decrease compile time a small bit
#include <shark/Core/Exception.h>
#include <complex>
#include <boost/mpl/bool.hpp>
#include "../traits.hpp"

namespace shark {
namespace blas {
namespace bindings {

template <typename Ord> struct storage_order {};
template<> struct storage_order<row_major> {
	enum ename { value = CblasRowMajor };
};
template<> struct storage_order<column_major> {
	enum ename { value = CblasColMajor };
};

}

}
}


#endif
