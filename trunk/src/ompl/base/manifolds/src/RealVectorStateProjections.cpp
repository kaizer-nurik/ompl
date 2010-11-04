/*********************************************************************
* Software License Agreement (BSD License)
* 
*  Copyright (c) 2010, Rice University
*  All rights reserved.
* 
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
* 
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Rice University nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Ioan Sucan */

#include "ompl/base/manifolds/RealVectorStateProjections.h"
#include "ompl/util/Exception.h"
#include <cstring>

ompl::base::RealVectorLinearProjectionEvaluator::RealVectorLinearProjectionEvaluator(const StateManifold *manifold,
										     const std::vector<double> &cellDimensions,
										     const ProjectionMatrix::Matrix &projection) :
    ProjectionEvaluator(manifold)
{
    if (!dynamic_cast<const RealVectorStateManifold*>(manifold_))
	throw Exception("Expected real vector manifold for projection");
    projection_.projection = projection;
    setCellDimensions(cellDimensions);
}

ompl::base::RealVectorLinearProjectionEvaluator::RealVectorLinearProjectionEvaluator(const StateManifoldPtr &manifold,
										     const std::vector<double> &cellDimensions,
										     const ProjectionMatrix::Matrix &projection) :
    ProjectionEvaluator(manifold.get())
{
    if (!dynamic_cast<const RealVectorStateManifold*>(manifold_))
	throw Exception("Expected real vector manifold for projection");
    projection_.projection = projection;
    setCellDimensions(cellDimensions);
}

ompl::base::RealVectorOrthogonalProjectionEvaluator::RealVectorOrthogonalProjectionEvaluator(const StateManifold *manifold,
											     const std::vector<double> &cellDimensions,
											     const std::vector<unsigned int> &components) : 
    ProjectionEvaluator(manifold), components_(components)
{
    if (!dynamic_cast<const RealVectorStateManifold*>(manifold_))
	throw Exception("Expected real vector manifold for projection");
    setCellDimensions(cellDimensions);
}

ompl::base::RealVectorOrthogonalProjectionEvaluator::RealVectorOrthogonalProjectionEvaluator(const StateManifoldPtr &manifold,
											     const std::vector<double> &cellDimensions,
											     const std::vector<unsigned int> &components) : 
    ProjectionEvaluator(manifold.get()), components_(components)
{
    if (!dynamic_cast<const RealVectorStateManifold*>(manifold_))
	throw Exception("Expected real vector manifold for projection");
    setCellDimensions(cellDimensions);
}

unsigned int ompl::base::RealVectorLinearProjectionEvaluator::getDimension(void) const
{
    return projection_.projection.size();
}

void ompl::base::RealVectorLinearProjectionEvaluator::project(const State *state, EuclideanProjection &projection) const
{
    projection_.project(state->as<RealVectorStateManifold::StateType>()->values, projection.values);
}

unsigned int ompl::base::RealVectorOrthogonalProjectionEvaluator::getDimension(void) const
{
    return components_.size();
}

void ompl::base::RealVectorOrthogonalProjectionEvaluator::project(const State *state, EuclideanProjection &projection) const
{
    for (unsigned int i = 0 ; i < components_.size() ; ++i)
	projection.values[i] = state->as<RealVectorStateManifold::StateType>()->values[components_[i]];
}

ompl::base::RealVectorIdentityProjectionEvaluator::RealVectorIdentityProjectionEvaluator(const StateManifold *manifold,
											 const std::vector<double> &cellDimensions) :
    ProjectionEvaluator(manifold)
{
    if (!dynamic_cast<const RealVectorStateManifold*>(manifold_))
	throw Exception("Expected real vector manifold for projection");
    setCellDimensions(cellDimensions);
    copySize_ = getDimension() * sizeof(double);
}

ompl::base::RealVectorIdentityProjectionEvaluator::RealVectorIdentityProjectionEvaluator(const StateManifoldPtr &manifold,
											 const std::vector<double> &cellDimensions) :
    ProjectionEvaluator(manifold.get())
{
    if (!dynamic_cast<const RealVectorStateManifold*>(manifold_))
	throw Exception("Expected real vector manifold for projection");
    setCellDimensions(cellDimensions);
    copySize_ = getDimension() * sizeof(double);
}

unsigned int ompl::base::RealVectorIdentityProjectionEvaluator::getDimension(void) const
{
    return manifold_->getDimension();
}

void ompl::base::RealVectorIdentityProjectionEvaluator::project(const State *state, EuclideanProjection &projection) const
{
    memcpy(projection.values, state->as<RealVectorStateManifold::StateType>()->values, copySize_);
}
