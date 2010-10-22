/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/*
 * SimpleRotation.cpp
 *
 * This is a super-simple example of optimizing a single rotation according to a single prior
 * yet it is quite painful (took 1.5 hours to code from scratch) and is overly complex
 * An example like this should be very easy to do, so let's work at it.
 *
 *  Created on: Jul 1, 2010
 *  @Author: Frank Dellaert
 *  @Author: Alex Cunningham
 */

#include <iostream>
#include <math.h>
#include <gtsam/slam/PriorFactor.h>
#include <gtsam/geometry/Rot2.h>
#include <gtsam/linear/NoiseModel.h>
#include <gtsam/nonlinear/Key.h>
#include <gtsam/nonlinear/LieValues-inl.h>
#include <gtsam/nonlinear/NonlinearFactorGraph-inl.h>
#include <gtsam/nonlinear/NonlinearOptimization-inl.h>
/*
 * TODO: make factors independent of Values
 * TODO: get rid of excessive shared pointer stuff: mostly gone
 * TODO: make toplevel documentation
 * TODO: investigate whether we can just use ints as keys: will occur for linear, not nonlinear
 */

using namespace std;
using namespace gtsam;

typedef TypedSymbol<Rot2, 'x'> Key;
typedef LieValues<Key> Values;
typedef NonlinearFactorGraph<Values> Graph;
typedef NonlinearOptimizer<Graph,Values> Optimizer;

const double degree = M_PI / 180;

int main() {

	// optimize a unary factor on rotation 1

	// Create a factor
	Rot2 prior1 = Rot2::fromAngle(30 * degree);
	prior1.print("goal angle");
	SharedDiagonal model1 = noiseModel::Isotropic::Sigma(1, 1 * degree);
	Key key1(1);
	PriorFactor<Values, Key> factor1(key1, prior1, model1);

	// Create a factor graph
	Graph graph;
	graph.add(factor1);
	graph.print("full graph") ;

	// and an initial estimate
	Values initial;
	initial.insert(key1, Rot2::fromAngle(20 * degree));
	initial.print("initial estimate");

	Values result = optimize<Graph, Values>(graph, initial);
	result.print("final result");

	return 0;
}
