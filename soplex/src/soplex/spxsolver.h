/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the class library                   */
/*       SoPlex --- the Sequential object-oriented simPlex.                  */
/*                                                                           */
/*  Copyright (c) 1996-2025 Zuse Institute Berlin (ZIB)                      */
/*                                                                           */
/*  Licensed under the Apache License, Version 2.0 (the "License");          */
/*  you may not use this file except in compliance with the License.         */
/*  You may obtain a copy of the License at                                  */
/*                                                                           */
/*      http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                           */
/*  Unless required by applicable law or agreed to in writing, software      */
/*  distributed under the License is distributed on an "AS IS" BASIS,        */
/*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/*  See the License for the specific language governing permissions and      */
/*  limitations under the License.                                           */
/*                                                                           */
/*  You should have received a copy of the Apache-2.0 license                */
/*  along with SoPlex; see the file LICENSE. If not email to soplex@zib.de.  */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**@file  spxsolver.h
 * @brief main LP solver class
 */
#ifndef _SPXSOLVER_H_
#define _SPXSOLVER_H_

#include <assert.h>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "soplex/spxdefines.h"
#include "soplex/timer.h"
#include "soplex/timerfactory.h"
#include "soplex/spxlp.h"
#include "soplex/spxbasis.h"
#include "soplex/array.h"
#include "soplex/random.h"
#include "soplex/unitvector.h"
#include "soplex/updatevector.h"
#include "soplex/stablesum.h"

#include "soplex/spxlpbase.h"

#define SOPLEX_HYPERPRICINGTHRESHOLD 5000     /**< do (auto) hyper pricing only if problem size (cols+rows) is larger than SOPLEX_HYPERPRICINGTHRESHOLD */
#define SOPLEX_HYPERPRICINGSIZE      100      /**< size of initial candidate list for hyper pricing */
#define SOPLEX_SPARSITYFACTOR        0.6      /**< percentage of infeasibilities that is considered sparse */
#define SOPLEX_DENSEROUNDS           5        /**< number of refactorizations until sparsity is tested again */
#define SOPLEX_SPARSITY_TRADEOFF     0.8      /**< threshold to decide whether Ids or coIds are preferred to enter the basis;
                                              //  * coIds are more likely to enter if SOPLEX_SPARSITY_TRADEOFF is close to 0
                                              //  */
#define SOPLEX_MAXNCLCKSKIPS         32       /**< maximum number of clock skips (iterations without time measuring) */
#define SOPLEX_SAFETYFACTOR          1e-2     /**< the probability to skip the clock when the time limit has been reached */
#define SOPLEX_NINITCALLS            200      /**< the number of clock updates in isTimelimitReached() before clock skipping starts */
namespace soplex
{
template <class R>
class SPxPricer;
template <class R>
class SPxRatioTester;
template <class R>
class SPxStarter;
template <class R>
class SPxFastRT;
template <class R>
class SPxBoundFlippingRT;

/**@brief   Sequential object-oriented SimPlex.
   @ingroup Algo

   SPxSolverBase is an LP solver class using the revised Simplex algorithm. It
   provides two basis representations, namely a column basis and a row basis
   (see #Representation). For both representations, a primal and
   dual algorithm is available (see \ref Type).

   In addition, SPxSolverBase can be customized with various respects:
   - pricing algorithms using SPxPricer
   - ratio test using class SPxRatioTester
   - computation of a start basis using class SPxStarter
   - preprocessing of the LP using class SPxSimplifier
   - termination criteria by overriding

   SPxSolverBase is derived from SPxLPBase<R> that is used to store the LP to be solved.
   Hence, the LPs solved with SPxSolverBase have the general format

   \f[
   \begin{array}{rl}
   \hbox{max}   & \mbox{maxObj}^T x                 \\
   \hbox{s.t.}  & \mbox{lhs} \le Ax \le \mbox{rhs}  \\
   & \mbox{low} \le x  \le \mbox{up}
   \end{array}
   \f]

   Also, SPxLPBase<R> provide all manipulation methods for the LP. They allow
   SPxSolverBase to be used within cutting plane algorithms.
*/

template <class R>
class SPxSolverBase : public SPxLPBase<R>, protected SPxBasisBase<R>
{
   friend SPxFastRT<R>;
   friend SPxBoundFlippingRT<R>;

public:

   //-----------------------------
   /**@name Data Types */
   ///@{
   /// LP basis representation.
   /** Solving LPs with the Simplex algorithm requires the definition of a
    *  \em basis. A basis can be defined as a set of column vectors or a
    *  set of row vectors building a nonsingular matrix. We will refer to
    *  the first case as the \em columnwise representation and the latter
    *  case will be called the \em rowwise representation.
    *
    *  Type Representation determines the representation of SPxSolverBase, i.e.
    *  a columnwise (#COLUMN == 1) or rowwise (#ROW == -1) one.
    */
   enum Representation
   {
      ROW    = -1,  ///< rowwise representation.
      COLUMN =  1   ///< columnwise representation.
   };

   /// Algorithmic type.
   /** SPxSolverBase uses the reviesed Simplex algorithm to solve LPs.
    *  Mathematically, one distinguishes the \em primal from the
    *  \em dual algorihm. Algorithmically, these relate to the two
    *  types #ENTER or #LEAVE. How they relate, depends on the chosen
    *  basis representation. This is desribed by the following table:
    *
    *  <TABLE>
    *  <TR><TD>&nbsp;</TD><TD>ENTER </TD><TD>LEAVE </TD></TR>
    *  <TR><TD>ROW   </TD><TD>DUAL  </TD><TD>PRIMAL</TD></TR>
    *  <TR><TD>COLUMN</TD><TD>PRIMAL</TD><TD>DUAL  </TD></TR>
    *  </TABLE>
    */
   enum Type
   {
      /// Entering Simplex.
      /** The Simplex loop for the entering Simplex can be sketched
       *  as follows:
       *  - \em Pricing : Select a variable to #ENTER the basis.
       *  - \em Ratio-Test : Select variable to #LEAVE the
       *    basis such that the basis remains feasible.
       *  - Perform the basis update.
       */
      ENTER = -1,
      /// Leaving Simplex.
      /** The Simplex loop for the leaving Simplex can be sketched
       *  as follows:
       *  - \em Pricing: Select a variable to #LEAVE the basis.
       *  - \em Ratio-Test: Select variable to #ENTER the
       *    basis such that the basis remains priced.
       *  - Perform the basis update.
       */
      LEAVE = 1
   };

   /// Pricing type.
   /** In case of the #ENTER%ing Simplex algorithm, for performance
    *  reasons it may be advisable not to compute and maintain up to
    *  date vectors #pVec() and #test() and instead compute only some
    *  of its elements explicitely. This is controled by the #Pricing type.
    */
   enum Pricing
   {
      /// Full pricing.
      /** If #FULL pricing in selected for the #ENTER%ing Simplex,
       *  vectors #pVec() and #test() are kept up to date by
       *  SPxSolverBase. An SPxPricer only needs to select an Id such
       *  that the #test() or #coTest() value is < 0.
       */
      FULL,
      /// Partial pricing.
      /** When #PARTIAL pricing in selected for the #ENTER%ing
       *  Simplex, vectors #pVec() and #test() are not set up and
       *  updated by SPxSolverBase. However, vectors #coPvec() and
       *  #coTest() are still kept up to date by SPxSolverBase.
       *  An SPxPricer object needs to compute the values for
       *  #pVec() and #test() itself in order to select an
       *  appropriate pivot with #test() < 0. Methods \ref computePvec(int)
       *  "computePvec(i)" and \ref computeTest(int) "computeTest(i)"
       *  will assist the used to do so. Note
       *  that it may be feasible for a pricer to return an Id with
       *  #test() > 0; such will be rejected by SPxSolverBase.
       */
      PARTIAL
   };

   enum VarStatus
   {
      ON_UPPER,      ///< variable set to its upper bound.
      ON_LOWER,      ///< variable set to its lower bound.
      FIXED,         ///< variable fixed to identical bounds.
      ZERO,          ///< free variable fixed to zero.
      BASIC,         ///< variable is basic.
      UNDEFINED      ///< nothing known about basis status (possibly due to a singular basis in transformed problem)
   };

   /**@todo In spxchange, change the status to
      if (m_status > 0) m_status = REGULAR;
   */
   enum Status
   {
      ERROR          = -15, ///< an error occured.
      NO_RATIOTESTER = -14, ///< No ratiotester loaded
      NO_PRICER      = -13, ///< No pricer loaded
      NO_SOLVER      = -12, ///< No linear solver loaded
      NOT_INIT       = -11, ///< not initialised error
      ABORT_CYCLING  = -8,  ///< solve() aborted due to detection of cycling.
      ABORT_TIME     = -7,  ///< solve() aborted due to time limit.
      ABORT_ITER     = -6,  ///< solve() aborted due to iteration limit.
      ABORT_VALUE    = -5,  ///< solve() aborted due to objective limit.
      SINGULAR       = -4,  ///< Basis is singular, numerical troubles?
      NO_PROBLEM     = -3,  ///< No Problem has been loaded.
      REGULAR        = -2,  ///< LP has a usable Basis (maybe LP is changed).
      RUNNING        = -1,  ///< algorithm is running
      UNKNOWN        =  0,  ///< nothing known on loaded problem.
      OPTIMAL        =  1,  ///< LP has been solved to optimality.
      UNBOUNDED      =  2,  ///< LP has been proven to be primal unbounded.
      INFEASIBLE     =  3,  ///< LP has been proven to be primal infeasible.
      INForUNBD      =  4,   ///< LP is primal infeasible or unbounded.
      OPTIMAL_UNSCALED_VIOLATIONS =  5   ///< LP has beed solved to optimality but unscaled solution contains violations.
   };

   /// objective for solution polishing
   enum SolutionPolish
   {
      POLISH_OFF,           ///< don't perform modifications on optimal basis
      POLISH_INTEGRALITY,   ///< maximize number of basic slack variables, i.e. more variables on bounds
      POLISH_FRACTIONALITY  ///< minimize number of basic slack variables, i.e. more variables in between bounds
   };


   ///@}

private:

   //-----------------------------
   /**@name Private data */
   ///@{
   Type           theType;     ///< entering or leaving algortihm.
   Pricing        thePricing;  ///< full or partial pricing.
   Representation theRep;      ///< row or column representation.
   SolutionPolish polishObj;   ///< objective of solution polishing
   Timer*         theTime;     ///< time spent in last call to method solve()
   Timer::TYPE    timerType;   ///< type of timer (user or wallclock)
   Real           theCumulativeTime; ///< cumulative time spent in all calls to method solve()
   int            maxIters;    ///< maximum allowed iterations.
   Real           maxTime;     ///< maximum allowed time.
   int            nClckSkipsLeft; ///< remaining number of times the clock can be safely skipped
   long           nCallsToTimelim; /// < the number of calls to the method isTimeLimitReached()
   R              objLimit;    ///< objective value limit.
   bool           useTerminationValue; ///< true, if objective limit should be used in the next solve.
   Status         m_status;    ///< status of algorithm.

   R           m_nonbasicValue;         ///< nonbasic part of current objective value
   bool           m_nonbasicValueUpToDate; ///< true, if the stored objValue is up to date

   R           m_pricingViol;             ///< maximal feasibility violation of current solution
   bool           m_pricingViolUpToDate;     ///< true, if the stored violation is up to date

   R
   m_pricingViolCo;           ///< maximal feasibility violation of current solution in coDim
   bool           m_pricingViolCoUpToDate;   ///< true, if the stored violation in coDim is up to date
   int            m_numViol;     ///< number of violations of current solution

   R           entertolscale;    ///< factor to temporarily decrease the entering tolerance
   R           leavetolscale;    ///< factor to temporarily decrease the leaving tolerance
   R           theShift;      ///< sum of all shifts applied to any bound.
   R           lastShift;     ///< for forcing feasibility.
   int            m_maxCycle;    ///< maximum steps before cycling is detected.
   int            m_numCycle;    ///< actual number of degenerate steps so far.
   bool           initialized;   ///< true, if all vectors are setup.

   SSVectorBase<R>*
   solveVector2;      ///< when 2 systems are to be solved at a time; typically for speepest edge weights
   SSVectorBase<R>*
   solveVector2rhs;   ///< when 2 systems are to be solved at a time; typically for speepest edge weights
   SSVectorBase<R>*
   solveVector3;      ///< when 3 systems are to be solved at a time; typically reserved for bound flipping ratio test (basic solution will be modified!)
   SSVectorBase<R>*
   solveVector3rhs;   ///< when 3 systems are to be solved at a time; typically reserved for bound flipping ratio test (basic solution will be modified!)
   SSVectorBase<R>*
   coSolveVector2;    ///< when 2 systems are to be solved at a time; typically for speepest edge weights
   SSVectorBase<R>*
   coSolveVector2rhs; ///< when 2 systems are to be solved at a time; typically for speepest edge weights
   SSVectorBase<R>*
   coSolveVector3;    ///< when 3 systems are to be solved at a time; typically reserved for bound flipping ratio test (basic solution will be modified!)
   SSVectorBase<R>*
   coSolveVector3rhs; ///< when 3 systems are to be solved at a time; typically reserved for bound flipping ratio test (basic solution will be modified!)

   bool           freePricer;        ///< true iff thepricer should be freed inside of object
   bool           freeRatioTester;   ///< true iff theratiotester should be freed inside of object
   bool           freeStarter;       ///< true iff thestarter should be freed inside of object

   /* Store the index of a leaving variable if only an instable entering variable has been found.
      instableLeave == true iff this instable basis change should be performed.
      (see spxsolve.hpp and leave.hpp) */
   int            instableLeaveNum;
   bool           instableLeave;
   R           instableLeaveVal;

   /* Store the id of an entering row or column if only an instable pivot has been found.
      instableEnter == true iff this instable basis change should be performed.
      (see spxsolve.hpp and enter.hpp) */
   SPxId          instableEnterId;
   bool           instableEnter;
   R           instableEnterVal;

   bool
   recomputedVectors;      ///< flag to perform clean up step to reduce numerical errors only once

   int            displayLine;
   int            displayFreq;
   R           sparsePricingFactor;    ///< enable sparse pricing when viols < factor * dim()

   DataArray<VarStatus> oldBasisStatusRows;
   ///< stored stable basis met before a simplex pivot (used to warm start the solver)
   DataArray<VarStatus> oldBasisStatusCols;
   ///< They don't have setters because only the internal simplex method is meant to fill them

   bool solvingForBoosted; ///< is this solver involved in a higher precision solving scheme?
   int storeBasisSimplexFreq; ///< number of simplex pivots -1 to perform before storing stable basis

   bool
   fullPerturbation;       ///< whether to perturb the entire problem or just the bounds relevant for the current pivot
   int
   printBasisMetric;       ///< printing the current basis metric in the log (-1: off, 0: condition estimate, 1: trace, 2: determinant, 3: condition)

   ///@}

protected:

   //-----------------------------
   /**@name Protected data */
   ///@{
   Array < UnitVectorBase<R> > unitVecs; ///< array of unit vectors
   const SVSetBase<R>*   thevectors;   ///< the LP vectors according to representation
   const SVSetBase<R>*   thecovectors; ///< the LP coVectors according to representation

   VectorBase<R>        primRhs;     ///< rhs VectorBase<R> for computing the primal vector
   UpdateVector<R>   primVec;     ///< primal vector
   VectorBase<R>        dualRhs;     ///< rhs VectorBase<R> for computing the dual vector
   UpdateVector<R>   dualVec;     ///< dual vector
   UpdateVector<R>   addVec;      ///< storage for thePvec = &addVec

   VectorBase<R>        theURbound;  ///< Upper Row    Feasibility bound
   VectorBase<R>        theLRbound;  ///< Lower Row    Feasibility bound
   VectorBase<R>        theUCbound;  ///< Upper Column Feasibility bound
   VectorBase<R>        theLCbound;  ///< Lower Column Feasibility bound

   /** In entering Simplex algorithm, the ratio test must ensure that all
    *  \em basic variables remain within their feasibility bounds. To give fast
    *  acces to them, the bounds of basic variables are copied into the
    *  following two vectors.
    */
   VectorBase<R>        theUBbound;  ///< Upper Basic Feasibility bound
   VectorBase<R>        theLBbound;  ///< Lower Basic Feasibility bound

   /** The values of the rhs corresponding to the current basis.*/
   VectorBase<R>*       theFrhs;
   /** The values of all basis variables. */
   UpdateVector<R>*  theFvec;

   /* The Copricing rhs and VectorBase<R> */
   VectorBase<R>*       theCoPrhs;
   UpdateVector<R>*  theCoPvec;
   /** The pricing VectorBase<R> */
   UpdateVector<R>*  thePvec;

   UpdateVector<R>*  theRPvec;    ///< row pricing vector
   UpdateVector<R>*  theCPvec;    ///< column pricing vector

   // The following vectors serve for the virtualization of shift bounds
   //@todo In prinziple this schould be references.
   VectorBase<R>*       theUbound;      ///< Upper bound for vars
   VectorBase<R>*       theLbound;      ///< Lower bound for vars
   VectorBase<R>*       theCoUbound;    ///< Upper bound for covars
   VectorBase<R>*       theCoLbound;    ///< Lower bound for covars

   // The following vectors serve for the virtualization of testing vectors
   VectorBase<R>        theCoTest;
   VectorBase<R>        theTest;

   DSVectorBase<R>       primalRay;      ///< stores primal ray in case of unboundedness
   DSVectorBase<R>       dualFarkas;     ///< stores dual farkas proof in case of infeasibility

   int            leaveCount;    ///< number of LEAVE iterations
   int            enterCount;    ///< number of ENTER iterations
   int            primalCount;   ///< number of primal iterations
   int            polishCount;   ///< number of solution polishing iterations

   int            boundflips;          ///< number of performed bound flips
   int            totalboundflips;     ///< total number of bound flips

   int            enterCycles;      ///< the number of degenerate steps during the entering algorithm
   int            leaveCycles;      ///< the number of degenerate steps during the leaving algorithm
   int            enterDegenCand;   ///< the number of degenerate candidates in the entering algorithm
   int            leaveDegenCand;   ///< the number of degenerate candidates in the leaving algorithm
   R           primalDegenSum;   ///< the sum of the primal degeneracy percentage
   R           dualDegenSum;     ///< the sum of the dual degeneracy percentage

   SPxPricer<R>*      thepricer;
   SPxRatioTester<R>* theratiotester;
   SPxStarter<R>*     thestarter;

   R           boundrange;       ///< absolute range of all bounds in the problem
   R           siderange;        ///< absolute range of all side in the problem
   R           objrange;         ///< absolute range of all objective coefficients in the problem
   ///@}

   //-----------------------------
   /**@name Precision */
   ///@{
   /// is the solution precise enough, or should we increase delta() ?
   virtual bool precisionReached(R& newpricertol) const;

   /// determine ranges of problem values for bounds, sides and objective to assess numerical difficulties
   void calculateProblemRanges();
   ///@}

public:

   /// The random number generator used throughout the whole computation. Its seed can be modified.
   Random random;

   /** For the leaving Simplex algorithm this vector contains the indices of infeasible basic variables;
    *  for the entering Simplex algorithm this vector contains the indices of infeasible slack variables.
    */
   DIdxSet infeasibilities;
   /**For the entering Simplex algorithm these vectors contains the indices of infeasible basic variables.
    */
   DIdxSet infeasibilitiesCo;

   /// store indices that were changed in the previous iteration and must be checked in hyper pricing
   DIdxSet updateViols;
   DIdxSet updateViolsCo;

   /** Binary vectors to store whether basic indices are infeasible
    *  the i-th entry equals false, if the i-th basic variable is not infeasible
    *  the i-th entry equals true, if the i-th basic variable is infeasible
    */
   DataArray<int>
   isInfeasible;           ///< 0: index not violated, 1: index violated, 2: index violated and among candidate list
   DataArray<int>
   isInfeasibleCo;         ///< 0: index not violated, 1: index violated, 2: index violated and among candidate list

   /// These values enable or disable sparse pricing
   bool     sparsePricingLeave;        ///< true if sparsePricing is turned on in the leaving Simplex
   bool     sparsePricingEnter;        ///< true if sparsePricing is turned on in the entering Simplex for slack variables
   bool     sparsePricingEnterCo;      ///< true if sparsePricing is turned on in the entering Simplex
   bool     hyperPricingLeave;         ///< true if hyper sparse pricing is turned on in the leaving Simplex
   bool     hyperPricingEnter;         ///< true if hyper sparse pricing is turned on in the entering Simplex

   int      remainingRoundsLeave;      ///< number of dense rounds/refactorizations until sparsePricing is enabled again
   int      remainingRoundsEnter;
   int      remainingRoundsEnterCo;

   /// dual pricing norms
   VectorBase<R>     weights;                ///< store dual norms
   VectorBase<R>     coWeights;              ///< store dual norms
   bool        weightsAreSetup;        ///< are the dual norms already set up?


   Timer*   multTimeSparse;            ///< time spent in setupPupdate() exploiting sparsity
   Timer*   multTimeFull;              ///< time spent in setupPupdate() ignoring sparsity
   Timer*   multTimeColwise;           ///< time spent in setupPupdate(), columnwise multiplication
   Timer*   multTimeUnsetup;           ///< time spent in setupPupdate() w/o sparsity information
   int      multSparseCalls;           ///< number of products exploiting sparsity
   int      multFullCalls;             ///< number of products ignoring sparsity
   int      multColwiseCalls;          ///< number of products, columnwise multiplication
   int      multUnsetupCalls;          ///< number of products w/o sparsity information

   SPxOut* spxout;                     ///< message handler

   DataArray<int>
   integerVariables;    ///< supplementary variable information, 0: continous variable, 1: integer variable

   //-----------------------------
   void setOutstream(SPxOut& newOutstream)
   {
      spxout = &newOutstream;
      SPxLPBase<R>::spxout = &newOutstream;
   }

   /// set the _tolerances member variable
   virtual void setTolerances(std::shared_ptr<Tolerances> newTolerances)
   {
      this->_tolerances = newTolerances;
      // set tolerances for all the UpdateVectors
      this->primVec.setTolerances(newTolerances);
      this->dualVec.setTolerances(newTolerances);
      this->addVec.setTolerances(newTolerances);
      this->theFvec->setTolerances(newTolerances);
      this->theCoPvec->setTolerances(newTolerances);
      this->thePvec->setTolerances(newTolerances);
      this->theRPvec->setTolerances(newTolerances);
      this->theCPvec->setTolerances(newTolerances);
   }

   /// returns current tolerances
   const std::shared_ptr<Tolerances>& tolerances() const
   {
      return this->_tolerances;
   }

   /// set refactor threshold for nonzeros in last factorized basis matrix compared to updated basis matrix
   void setNonzeroFactor(R f)
   {
      SPxBasisBase<R>::nonzeroFactor = f;
   }

   /// set refactor threshold for fill-in in current factor update compared to fill-in in last factorization
   void setFillFactor(R f)
   {
      SPxBasisBase<R>::fillFactor = f;
   }

   /// set refactor threshold for memory growth in current factor update compared to the last factorization
   void setMemFactor(R f)
   {
      SPxBasisBase<R>::memFactor = f;
   }

   /**@name Access */
   ///@{
   /// return the version of SPxSolverBase as number like 123 for 1.2.3
   int version() const
   {
      return SOPLEX_VERSION;
   }
   /// return the internal subversion of SPxSolverBase as number
   int subversion() const
   {
      return SOPLEX_SUBVERSION;
   }
   /// return the current basis representation.
   Representation rep() const
   {
      return theRep;
   }

   /// return current Type.
   Type type() const
   {
      return theType;
   }

   /// return current Pricing.
   Pricing pricing() const
   {
      return thePricing;
   }

   /// return current starter.
   SPxStarter<R>* starter() const
   {
      return thestarter;
   }
   ///@}

   //-----------------------------
   /**@name Setup
    *  Before solving an LP with an instance of SPxSolverBase,
    *  the following steps must be performed:
    *
    *  -# Load the LP by copying an external LP or reading it from an
    *     input stream.
    *  -# Setup the pricer to use by loading an \ref soplex::SPxPricer
    *     "SPxPricer" object (if not already done in a previous call).
    *  -# Setup the ratio test method to use by loading an
    *     \ref soplex::SPxRatioTester "SPxRatioTester" object
    *     (if not already done in a previous call).
    *  -# Setup the linear system solver to use by loading an
    *     \ref soplex::SLinSolver "SLinSolver" object
    *     (if not already done in a previous call).
    *  -# Optionally setup an start basis generation method by loading an
    *     \ref soplex::SPxStarter "SPxStarter" object.
    *  -# Optionally setup a start basis by loading a
    *     \ref soplex::SPxBasisBase<R>::Desc "SPxBasisBase<R>::Desc" object.
    *  -# Optionally switch to another basis
    *     \ref soplex::SPxSolverBase<R>::Representation "Representation"
    *     by calling method \ref soplex::SPxSolverBase<R>::setRep() "setRep()".
    *  -# Optionally switch to another algorithm
    *     \ref soplex::SPxSolverBase<R>::Type "Type"
    *     by calling method \ref soplex::SPxSolverBase<R>::setType() "setType()".
    *
    *  Now the solver is ready for execution. If the loaded LP is to be solved
    *  again from scratch, this can be done with method
    *  \ref soplex::SPxSolverBase<R>::reLoad() "reLoad()". Finally,
    *  \ref soplex::SPxSolverBase<R>::clear() "clear()" removes the LP from the solver.
    */
   ///@{
   /// read LP from input stream.
   virtual bool read(std::istream& in, NameSet* rowNames = nullptr,
                     NameSet* colNames = nullptr, DIdxSet* intVars = nullptr);

   /// copy LP.
   virtual void loadLP(const SPxLPBase<R>& LP, bool initSlackBasis = true);
   /// setup linear solver to use. If \p destroy is true, \p slusolver will be freed in destructor.
   virtual void setBasisSolver(SLinSolver<R>* slu, const bool destroy = false);
   /// setup pricer to use. If \p destroy is true, \p pricer will be freed in destructor.
   virtual void setPricer(SPxPricer<R>* pricer, const bool destroy = false);
   /// setup ratio-tester to use. If \p destroy is true, \p tester will be freed in destructor.
   virtual void setTester(SPxRatioTester<R>* tester, const bool destroy = false);
   /// setup starting basis generator to use. If \p destroy is true, \p starter will be freed in destructor.
   virtual void setStarter(SPxStarter<R>* starter, const bool destroy = false);
   /// set a start basis.
   virtual void loadBasis(const typename SPxBasisBase<R>::Desc&);

   /// initialize #ROW or #COLUMN representation.
   void initRep(Representation p_rep);
   /// switch to #ROW or #COLUMN representation if not already used.
   void setRep(Representation p_rep);
   /// set \ref soplex::SPxSolverBase<R>::LEAVE "LEAVE" or \ref soplex::SPxSolverBase<R>::ENTER "ENTER" algorithm.
   void setType(Type tp);
   /// set \ref soplex::SPxSolverBase<R>::FULL "FULL" or \ref soplex::SPxSolverBase<R>::PARTIAL "PARTIAL" pricing.
   void setPricing(Pricing pr);

   /// reload LP.
   virtual void reLoad();

   /// clear all data in solver.
   virtual void clear();

   /// unscales the LP and reloads the basis
   void unscaleLPandReloadBasis();

   /// invalidates the basis, triggers refactorization
   void invalidateBasis();

   /** Load basis from \p filename in MPS format. If \p rowNames and \p
    *  colNames are \c nullptr, default names are used for the constraints and
    *  variables.
    */
   virtual bool readBasisFile(const char* filename,
                              const NameSet* rowNames, const NameSet* colNames);

   /** Write basis to \p filename in MPS format. If \p rowNames and \p
    *  colNames are \c nullptr, default names are used for the constraints and
    *  variables.
    */
   virtual bool writeBasisFile(const char* filename,
                               const NameSet* rowNames, const NameSet* colNames, const bool cpxFormat = false) const;

   /** Write current LP, basis, and parameter settings.
    *  LP is written in MPS format to "\p filename".mps, basis is written in "\p filename".bas, and parameters
    *  are written to "\p filename".set. If \p rowNames and \p colNames are \c nullptr, default names are used for
    *  the constraints and variables.
    */
   virtual bool writeState(const char* filename, const NameSet* rowNames = nullptr,
                           const NameSet* colNames = nullptr, const bool cpxFormat = false,
                           const bool writeZeroObjective = false) const;

   ///@}

   /**@name Solving LPs */
   ///@{
   /// solve loaded LP.
   /** Solves the loaded LP by processing the Simplex iteration until
    *  the termination criteria is fullfilled (see #terminate()).
    *  The SPxStatus of the solver will indicate the reason for termination.
    *  @param interrupt can be set externally to interrupt the solve
    *  @param polish should solution polishing be considered
    *
    *  @throw SPxStatusException if either no problem, solver, pricer
    *  or ratiotester loaded or if solve is still running when it shouldn't be
    */
   virtual Status solve(volatile bool* interrupt = nullptr, bool polish = true);

   /** Identify primal basic variables that have zero reduced costs and
    * try to pivot them out of the basis to make them tight.
    * This is supposed to decrease the number of fractional variables
    * when solving LP relaxations of (mixed) integer programs.
    * The objective must not be modified during this procedure.
    * @return true, if objective was modified (due to numerics) and resolving is necessary
    */
   bool performSolutionPolishing();

   /// set objective of solution polishing (0: off, 1: max_basic_slack, 2: min_basic_slack)
   void setSolutionPolishing(SolutionPolish _polishObj)
   {
      polishObj = _polishObj;
   }

   /// return objective of solution polishing
   SolutionPolish getSolutionPolishing()
   {
      return polishObj;
   }

   /// true if objective limit should be used in the next solve
   bool isTerminationValueEnabled() const
   {
      return useTerminationValue;
   }

   /// toggle objective limit for next solve
   void toggleTerminationValue(bool enable)
   {
      useTerminationValue = enable;
   }

   /// Status of solution process.
   Status status() const;

   /// current objective value.
   /**@return Objective value of the current solution vector
    *         (see #getPrimalSol()).
    */
   virtual R value();

   // update nonbasic part of the objective value by the given amount
   /**@return whether nonbasic part of objective is reliable
    */
   bool updateNonbasicValue(R objChange);

   // trigger a recomputation of the nonbasic part of the objective value
   void forceRecompNonbasicValue()
   {
      m_nonbasicValue = 0.0;
      m_nonbasicValueUpToDate = false;
   }

   /** helper method that computes a fresh factorization of the basis matrix
    *  (if at least one update has been performed)
    *  and recomputes Frhs, Fvec, CoPrhs, Pvec, and the nonbasic values.
    *  In LEAVE the Ftest is recomputed, in ENTER the CoTest and Test are recomputed.
    *
    *  This method is called to eliminate accumulated errors from LU updates
    *  especially required before checking if the solver can terminate
    *  (optimality or objective limit)
    */
   virtual void factorizeAndRecompute();

   /// get solution vector for primal variables.
   /** This method returns the Status of the basis.
    *  If it is #REGULAR or better,
    *  the primal solution vector of the current basis will be copied
    *  to the argument \p vector. Hence, \p vector must be of dimension
    *  #nCols().
    *
    *  @throw SPxStatusException if not initialized
    */
   virtual Status getPrimalSol(VectorBase<R>& vector) const;

   /// get VectorBase<R> of slack variables.
   /** This method returns the Status of the basis.
    *  If it is #REGULAR or better,
    *  the slack variables of the current basis will be copied
    *  to the argument \p vector. Hence, \p VectorBase<R> must be of dimension
    *  #nRows().
    *
    *  @warning Because SPxSolverBase supports range constraints as its
    *     default, slack variables are defined in a nonstandard way:
    *     Let \em x be the current solution vector and \em A the constraint
    *     matrix. Then the vector of slack variables is defined as
    *     \f$s = Ax\f$.
    *
    *  @throw SPxStatusException if no problem loaded
    */
   virtual Status getSlacks(VectorBase<R>& vector) const;

   /// get current solution VectorBase<R> for dual variables.
   /** This method returns the Status of the basis.
    *  If it is #REGULAR or better,
    *  the VectorBase<R> of dual variables of the current basis will be copied
    *  to the argument \p vector. Hence, \p VectorBase<R> must be of dimension
    *  #nRows().
    *
    *  @warning Even though mathematically, each range constraint would
    *     account for two dual variables (one for each inequaility), only
    *     #nRows() dual variables are setup via the following
    *     construction: Given a range constraint, there are three possible
    *     situations:
    *     - None of its inequalities is tight: The dual variables
    *       for both are 0. However, when shifting (see below)
    *       occurs, it may be set to a value other than 0, which
    *       models a perturbed objective vector.
    *     - Both of its inequalities are tight: In this case the
    *       range constraint models an equality and we adopt the
    *       standard definition.
    *     - One of its inequalities is tight while the other is not:
    *       In this case only the dual variable for the tight
    *       constraint is given with the standard definition, while
    *       the other constraint is implicitely set to 0.
    *
    *  @throw SPxStatusException if no problem loaded
    */
   virtual Status getDualSol(VectorBase<R>& vector) const;

   /// get vector of reduced costs.
   /** This method returns the Status of the basis.
    *  If it is #REGULAR or better,
    *  the vector of reduced costs of the current basis will be copied
    *  to the argument \p vector. Hence, \p vector must be of dimension
    *  #nCols().
    *
    *  Let \em d denote the vector of dual variables, as defined above,
    *  and \em A the LPs constraint matrix. Then the reduced cost vector
    *  \em r is defined as \f$r^T = c^T - d^TA\f$.
    *
    *  @throw SPxStatusException if no problem loaded
    */
   virtual Status getRedCostSol(VectorBase<R>& vector) const;

   /// get primal ray in case of unboundedness.
   ///  @throw SPxStatusException if no problem loaded
   virtual Status getPrimalray(VectorBase<R>& vector) const;

   /// get dual farkas proof of infeasibility.
   ///  @throw SPxStatusException if no problem loaded
   virtual Status getDualfarkas(VectorBase<R>& vector) const;

   /// print display line of flying table
   virtual void printDisplayLine(const bool force = false, const bool forceHead = false);

   /// Termination criterion.
   /** This method is called in each Simplex iteration to determine, if
    *  the algorithm is to terminate. In this case a nonzero value is
    *  returned.
    *
    *  This method is declared virtual to allow for implementation of
    *  other stopping criteria or using it as callback method within the
    *  Simplex loop, by overriding the method in a derived class.
    *  However, all implementations must terminate with the
    *  statement \c return SPxSolverBase<R>::#terminate(), if no own termination
    *  criteria is encountered.
    *
    *  Note, that the Simplex loop stopped even when #terminate()
    *  returns 0, if the LP has been solved to optimality (i.e. no
    *  further pricing succeeds and no shift is present).
    */
   virtual bool terminate();
   ///@}

   //-----------------------------
   /**@name Control Parameters */
   ///@{
   /// values \f$|x| < \epsilon\f$ are considered to be 0.
   /** if you want another value for epsilon, use
    * \ref soplex::Tolerances::setEpsilon() "Tolerances::setEpsilon()".
    */
   R epsilon() const
   {
      return this->tolerances()->epsilon();
   }
   /// feasibility tolerance maintained by ratio test during ENTER algorithm.
   R entertol() const
   {
      if(theRep == COLUMN)
         return this->tolerances()->floatingPointFeastol() * this->entertolscale;
      else
         return this->tolerances()->floatingPointOpttol() * this->entertolscale;
   }
   /// feasibility tolerance maintained by ratio test during LEAVE algorithm.
   R leavetol() const
   {
      if(theRep == COLUMN)
         return this->tolerances()->floatingPointOpttol() * this->leavetolscale;
      else
         return this->tolerances()->floatingPointFeastol() * this->leavetolscale;
   }
   /// scale the entering tolerance
   void scaleEntertol(R d)
   {
      this->entertolscale = d;
   }
   /// scale the leaving tolerance
   void scaleLeavetol(R d)
   {
      this->leavetolscale = d;
   }
   void scaleTolerances(R d)
   {
      this->scaleEntertol(d);
      this->scaleLeavetol(d);
   }
   /// guaranteed primal and dual bound violation for optimal solution, returning the maximum of floatingPointFeastol() and floatingPointOpttol().
   R delta() const
   {
      return SOPLEX_MAX(this->tolerances()->floatingPointFeastol(),
                        this->tolerances()->floatingPointOpttol());
   }

   /// set timing type
   void setTiming(Timer::TYPE ttype)
   {
      theTime = TimerFactory::switchTimer(theTime, ttype);
      multTimeSparse = TimerFactory::switchTimer(multTimeSparse, ttype);
      multTimeFull = TimerFactory::switchTimer(multTimeFull, ttype);
      multTimeColwise = TimerFactory::switchTimer(multTimeColwise, ttype);
      multTimeUnsetup = TimerFactory::switchTimer(multTimeUnsetup, ttype);
      timerType = ttype;
   }

   /// set timing type
   Timer::TYPE getTiming()
   {
      assert(timerType == theTime->type());
      assert(timerType == multTimeSparse->type());
      assert(timerType == multTimeFull->type());
      assert(timerType == multTimeColwise->type());
      assert(timerType == multTimeUnsetup->type());
      return timerType;
   }

   /// set display frequency
   void setDisplayFreq(int freq)
   {
      displayFreq = freq;
   }

   /// get display frequency
   int getDisplayFreq()
   {
      return displayFreq;
   }

   /// print basis metric within the usual output
   void setMetricInformation(int type)
   {
      printBasisMetric = type;
   }

   // enable sparse pricing when viols < fac * dim()
   void setSparsePricingFactor(R fac)
   {
      sparsePricingFactor = fac;
   }
   /// enable or disable hyper sparse pricing
   void hyperPricing(bool h);

   // get old basis status rows
   DataArray<VarStatus>& getOldBasisStatusRows()
   {
      return oldBasisStatusRows;
   }

   // get old basis status cols
   DataArray<VarStatus>& getOldBasisStatusCols()
   {
      return oldBasisStatusCols;
   }

   // should the basis be stored for use in precision boosting?
   void setSolvingForBoosted(bool value)
   {
      solvingForBoosted = value;
   }

   // set frequency of storing the basis for use in precision boosting
   void setStoreBasisFreqForBoosting(int freq)
   {
      storeBasisSimplexFreq = freq;
   }

   /** SPxSolverBase considers a Simplex step as degenerate if the
    *  steplength does not exceed #epsilon(). Cycling occurs if only
    *  degenerate steps are taken. To prevent this situation, SPxSolverBase
    *  perturbs the problem such that nondegenerate steps are ensured.
    *
    *  maxCycle() controls how agressive such perturbation is
    *  performed, since no more than maxCycle() degenerate steps are
    *  accepted before perturbing the LP. The current number of consecutive
    *  degenerate steps is counted by numCycle().
    */
   /// maximum number of degenerate simplex steps before we detect cycling.
   int maxCycle() const
   {
      return m_maxCycle;
   }
   /// actual number of degenerate simplex steps encountered so far.
   int numCycle() const
   {
      return m_numCycle;
   }

   /// perturb entire problem or only the bounds relevant to the current pivot
   void useFullPerturbation(bool full)
   {
      fullPerturbation = full;
   }

   virtual R getBasisMetric(int type)
   {
      return basis().getMatrixMetric(type);
   }

   ///@}

private:

   //-----------------------------
   /**@name Private helpers */
   ///@{
   ///
   void localAddRows(int start);
   ///
   void localAddCols(int start);
   ///
   void setPrimal(VectorBase<R>& p_vector);
   ///
   void setSlacks(VectorBase<R>& p_vector);
   ///
   void setDual(VectorBase<R>& p_vector);
   ///
   void setRedCost(VectorBase<R>& p_vector);
   ///@}

protected:

   //-----------------------------
   /**@name Protected helpers */
   ///@{
   ///
   virtual void addedRows(int n);
   ///
   virtual void addedCols(int n);
   ///
   virtual void doRemoveRow(int i);
   ///
   virtual void doRemoveRows(int perm[]);
   ///
   virtual void doRemoveCol(int i);
   ///
   virtual void doRemoveCols(int perm[]);
   ///@}

public:

   //-----------------------------
   /**@name Modification */
   /// \p scale determines whether the new data needs to be scaled according to the existing LP (persistent scaling)
   ///@{
   ///
   virtual void changeObj(const VectorBase<R>& newObj, bool scale = false);
   ///
   virtual void changeObj(int i, const R& newVal, bool scale = false);
   ///
   using SPxLPBase<R>::changeObj; /// overloading a virtual function
   virtual void changeObj(SPxColId p_id, const R& p_newVal, bool scale = false)
   {
      changeObj(this->number(p_id), p_newVal, scale);
   }
   ///
   virtual void changeMaxObj(const VectorBase<R>& newObj, bool scale = false);
   ///
   virtual void changeMaxObj(int i, const R& newVal, bool scale = false);
   ///
   using SPxLPBase<R>::changeMaxObj; /// overloading a virtual function
   virtual void changeMaxObj(SPxColId p_id, const R& p_newVal, bool scale = false)
   {
      changeMaxObj(this->number(p_id), p_newVal, scale);
   }
   ///
   virtual void changeRowObj(const VectorBase<R>& newObj, bool scale = false);
   ///
   virtual void changeRowObj(int i, const R& newVal, bool scale = false);
   ///
   using SPxLPBase<R>::changeRowObj;
   virtual void changeRowObj(SPxRowId p_id, const R& p_newVal, bool scale = false)
   {
      changeRowObj(this->number(p_id), p_newVal);
   }
   ///
   virtual void clearRowObjs()
   {
      SPxLPBase<R>::clearRowObjs();
      unInit();
   }
   ///
   virtual void changeLowerStatus(int i, R newLower, R oldLower = 0.0);
   ///
   virtual void changeLower(const VectorBase<R>& newLower, bool scale = false);
   ///
   virtual void changeLower(int i, const R& newLower, bool scale = false);
   ///
   using SPxLPBase<R>::changeLower;
   virtual void changeLower(SPxColId p_id, const R& p_newLower, bool scale = false)
   {
      changeLower(this->number(p_id), p_newLower, scale);
   }
   ///
   virtual void changeUpperStatus(int i, R newUpper, R oldLower = 0.0);
   ///
   virtual void changeUpper(const VectorBase<R>& newUpper, bool scale = false);
   ///
   virtual void changeUpper(int i, const R& newUpper, bool scale = false);
   ///
   using SPxLPBase<R>::changeUpper; /// overloading virtual function
   virtual void changeUpper(SPxColId p_id, const R& p_newUpper, bool scale = false)
   {
      changeUpper(this->number(p_id), p_newUpper, scale);
   }
   ///
   virtual void changeBounds(const VectorBase<R>& newLower, const VectorBase<R>& newUpper,
                             bool scale = false);
   ///
   virtual void changeBounds(int i, const R& newLower, const R& newUpper, bool scale = false);
   ///
   using SPxLPBase<R>::changeBounds;
   virtual void changeBounds(SPxColId p_id, const R& p_newLower, const R& p_newUpper,
                             bool scale = false)
   {
      changeBounds(this->number(p_id), p_newLower, p_newUpper, scale);
   }
   ///
   virtual void changeLhsStatus(int i, R newLhs, R oldLhs = 0.0);
   ///
   virtual void changeLhs(const VectorBase<R>& newLhs, bool scale = false);
   ///
   virtual void changeLhs(int i, const R& newLhs, bool scale = false);
   ///
   using SPxLPBase<R>::changeLhs;
   virtual void changeLhs(SPxRowId p_id, const R& p_newLhs, bool scale = false)
   {
      changeLhs(this->number(p_id), p_newLhs, scale);
   }
   ///
   virtual void changeRhsStatus(int i, R newRhs, R oldRhs = 0.0);
   ///
   virtual void changeRhs(const VectorBase<R>& newRhs, bool scale = false);
   ///
   virtual void changeRhs(int i, const R& newRhs, bool scale = false);
   ///
   using SPxLPBase<R>::changeRhs;
   virtual void changeRhs(SPxRowId p_id, const R& p_newRhs, bool scale = false)
   {
      changeRhs(this->number(p_id), p_newRhs, scale);
   }
   ///
   virtual void changeRange(const VectorBase<R>& newLhs, const VectorBase<R>& newRhs,
                            bool scale = false);
   ///
   virtual void changeRange(int i, const R& newLhs, const R& newRhs, bool scale = false);
   ///
   using SPxLPBase<R>::changeRange;
   virtual void changeRange(SPxRowId p_id, const R& p_newLhs, const R& p_newRhs, bool scale = false)
   {
      changeRange(this->number(p_id), p_newLhs, p_newRhs, scale);
   }
   ///
   virtual void changeRow(int i, const LPRowBase<R>& newRow, bool scale = false);
   ///
   using SPxLPBase<R>::changeRow;
   virtual void changeRow(SPxRowId p_id, const LPRowBase<R>& p_newRow, bool scale = false)
   {
      changeRow(this->number(p_id), p_newRow, scale);
   }
   ///
   virtual void changeCol(int i, const LPColBase<R>& newCol, bool scale = false);
   ///
   using SPxLPBase<R>::changeCol;
   virtual void changeCol(SPxColId p_id, const LPColBase<R>& p_newCol, bool scale = false)
   {
      changeCol(this->number(p_id), p_newCol, scale);
   }
   ///
   virtual void changeElement(int i, int j, const R& val, bool scale = false);
   ///
   using SPxLPBase<R>::changeElement;
   virtual void changeElement(SPxRowId rid, SPxColId cid, const R& val, bool scale = false)
   {
      changeElement(this->number(rid), this->number(cid), val, scale);
   }
   ///
   virtual void changeSense(typename SPxLPBase<R>::SPxSense sns);
   ///@}

   //------------------------------------
   /**@name Dimension and codimension */
   ///@{
   /// dimension of basis matrix.
   int dim() const
   {
      return thecovectors->num();
   }
   /// codimension.
   int coDim() const
   {
      return thevectors->num();
   }
   ///@}

   //------------------------------------
   /**@name Variables and Covariables
    *  Class SPxLPBase<R> introduces \ref soplex::SPxId "SPxIds" to identify
    *  row or column data of an LP. SPxSolverBase uses this concept to
    *  access data with respect to the chosen representation.
    */
   ///@{
   /// id of \p i 'th vector.
   /** The \p i 'th Id is the \p i 'th SPxRowId for a rowwise and the
    *  \p i 'th SPxColId for a columnwise basis represenation. Hence,
    *  0 <= i < #coDim().
    */
   SPxId id(int i) const
   {
      if(rep() == ROW)
      {
         SPxRowId rid = SPxLPBase<R>::rId(i);
         return SPxId(rid);
      }
      else
      {
         SPxColId cid = SPxLPBase<R>::cId(i);
         return SPxId(cid);
      }
   }

   /// id of \p i 'th covector.
   /** The \p i 'th #coId() is the \p i 'th SPxColId for a rowwise and the
    *  \p i 'th SPxRowId for a columnwise basis represenation. Hence,
    *  0 <= i < #dim().
    */
   SPxId coId(int i) const
   {
      if(rep() == ROW)
      {
         SPxColId cid = SPxLPBase<R>::cId(i);
         return SPxId(cid);
      }
      else
      {
         SPxRowId rid = SPxLPBase<R>::rId(i);
         return SPxId(rid);
      }
   }

   /// Is \p p_id an SPxId ?
   /** This method returns wheather or not \p p_id identifies a vector
    *  with respect to the chosen representation.
    */
   bool isId(const SPxId& p_id) const
   {
      return p_id.info * theRep > 0;
   }

   /// Is \p p_id a CoId.
   /** This method returns wheather or not \p p_id identifies a coVector
    *  with respect to the chosen representation.
    */
   bool isCoId(const SPxId& p_id) const
   {
      return p_id.info * theRep < 0;
   }
   ///@}

   //------------------------------------
   /**@name Vectors and Covectors */
   ///@{
   /// \p i 'th vector.
   /**@return a reference to the \p i 'th, 0 <= i < #coDim(), vector of
    *         the loaded LP (with respect to the chosen representation).
    */
   const SVectorBase<R>& vector(int i) const
   {
      return (*thevectors)[i];
   }

   ///
   const SVectorBase<R>& vector(const SPxRowId& rid) const
   {
      assert(rid.isValid());
      return (rep() == ROW)
             ? (*thevectors)[this->number(rid)]
             : static_cast<const SVectorBase<R>&>(unitVecs[this->number(rid)]);
   }
   ///
   const SVectorBase<R>& vector(const SPxColId& cid) const
   {
      assert(cid.isValid());
      return (rep() == COLUMN)
             ? (*thevectors)[this->number(cid)]
             : static_cast<const SVectorBase<R>&>(unitVecs[this->number(cid)]);
   }

   /// VectorBase<R> associated to \p p_id.
   /**@return Returns a reference to the VectorBase<R> of the loaded LP corresponding
    *  to \p id (with respect to the chosen representation). If \p p_id is
    *  an id, a vector of the constraint matrix is returned, otherwise
    *  the corresponding unit vector (of the slack variable or bound
    *  inequality) is returned.
    *  @todo The implementation does not exactly look like it will do
    *        what is promised in the describtion.
    */
   const SVectorBase<R>& vector(const SPxId& p_id) const
   {
      assert(p_id.isValid());

      return p_id.isSPxRowId()
             ? vector(SPxRowId(p_id))
             : vector(SPxColId(p_id));
   }

   /// \p i 'th covector of LP.
   /**@return a reference to the \p i 'th, 0 <= i < #dim(), covector of
    *  the loaded LP (with respect to the chosen representation).
    */
   const SVectorBase<R>& coVector(int i) const
   {
      return (*thecovectors)[i];
   }
   ///
   const SVectorBase<R>& coVector(const SPxRowId& rid) const
   {
      assert(rid.isValid());
      return (rep() == COLUMN)
             ? (*thecovectors)[this->number(rid)]
             : static_cast<const SVector&>(unitVecs[this->number(rid)]);
   }
   ///
   const SVectorBase<R>& coVector(const SPxColId& cid) const
   {
      assert(cid.isValid());
      return (rep() == ROW)
             ? (*thecovectors)[this->number(cid)]
             : static_cast<const SVectorBase<R>&>(unitVecs[this->number(cid)]);
   }
   /// coVector associated to \p p_id.
   /**@return a reference to the covector of the loaded LP
    *  corresponding to \p p_id (with respect to the chosen
    *  representation). If \p p_id is a coid, a covector of the constraint
    *  matrix is returned, otherwise the corresponding unit vector is
    *  returned.
    */
   const SVectorBase<R>& coVector(const SPxId& p_id) const
   {
      assert(p_id.isValid());
      return p_id.isSPxRowId()
             ? coVector(SPxRowId(p_id))
             : coVector(SPxColId(p_id));
   }
   /// return \p i 'th unit vector.
   const SVectorBase<R>& unitVector(int i) const
   {
      return unitVecs[i];
   }
   ///@}

   //------------------------------------
   /**@name Variable status
    *  The Simplex basis assigns a \ref soplex::SPxBasisBase<R>::Desc::Status
    *  "Status" to each variable and covariable. Depending on the
    *  representation, the status indicates that the corresponding
    *  vector is in the basis matrix or not.
    */
   ///@{
   /// Status of \p i 'th variable.
   typename SPxBasisBase<R>::Desc::Status varStatus(int i) const
   {
      return this->desc().status(i);
   }

   /// Status of \p i 'th covariable.
   typename SPxBasisBase<R>::Desc::Status covarStatus(int i) const
   {
      return this->desc().coStatus(i);
   }

   /// does \p stat describe a basic index ?
   bool isBasic(typename SPxBasisBase<R>::Desc::Status stat) const
   {
      return (stat * rep() > 0);
   }

   /// is the \p p_id 'th vector basic ?
   bool isBasic(const SPxId& p_id) const
   {
      assert(p_id.isValid());
      return p_id.isSPxRowId()
             ? isBasic(SPxRowId(p_id))
             : isBasic(SPxColId(p_id));
   }

   /// is the \p rid 'th vector basic ?
   bool isBasic(const SPxRowId& rid) const
   {
      return isBasic(this->desc().rowStatus(this->number(rid)));
   }

   /// is the \p cid 'th vector basic ?
   bool isBasic(const SPxColId& cid) const
   {
      return isBasic(this->desc().colStatus(this->number(cid)));
   }

   /// is the \p i 'th row vector basic ?
   bool isRowBasic(int i) const
   {
      return isBasic(this->desc().rowStatus(i));
   }

   /// is the \p i 'th column vector basic ?
   bool isColBasic(int i) const
   {
      return isBasic(this->desc().colStatus(i));
   }

   /// is the \p i 'th vector basic ?
   bool isBasic(int i) const
   {
      return isBasic(this->desc().status(i));
   }

   /// is the \p i 'th covector basic ?
   bool isCoBasic(int i) const
   {
      return isBasic(this->desc().coStatus(i));
   }
   ///@}

   /// feasibility vector.
   /** This method return the \em feasibility vector. If it satisfies its
    *  bound, the basis is called feasible (independently of the chosen
    *  representation). The feasibility vector has dimension #dim().
    *
    *  For the entering Simplex, #fVec is kept within its bounds. In
    *  contrast to this, the pricing of the leaving Simplex selects an
    *  element of #fVec, that violates its bounds.
    */
   UpdateVector<R>& fVec() const
   {
      return *theFvec;
   }
   /// right-hand side vector for \ref soplex::SPxSolverBase<R>::fVec "fVec"
   /** The feasibility vector is computed by solving a linear system with the
    *  basis matrix. The right-hand side vector of this system is referred
    *  to as \em feasibility, \em right-hand \em side \em vector #fRhs().
    *
    *  For a row basis, #fRhs() is the objective vector (ignoring shifts).
    *  For a column basis, it is the sum of all nonbasic vectors scaled by
    *  the factor of their bound.
    */
   const VectorBase<R>& fRhs() const
   {
      return *theFrhs;
   }
   /// upper bound for \ref soplex::SPxSolverBase<R>::fVec "fVec".
   const VectorBase<R>& ubBound() const
   {
      return theUBbound;
   }
   /// upper bound for #fVec, writable.
   /** This method returns the upper bound for the feasibility vector.
    *  It may only be called for the #ENTER%ing Simplex.
    *
    *  For the #ENTER%ing Simplex algorithms, the feasibility vector is
    *  maintained to fullfill its bounds. As #fVec itself, also its
    *  bounds depend on the chosen representation. Further, they may
    *  need to be shifted (see below).
    */
   VectorBase<R>& ubBound()
   {
      return theUBbound;
   }
   /// lower bound for \ref soplex::SPxSolverBase<R>::fVec "fVec".
   const VectorBase<R>& lbBound() const
   {
      return theLBbound;
   }
   /// lower bound for #fVec, writable.
   /** This method returns the lower bound for the feasibility vector.
    *  It may only be called for the #ENTER%ing Simplex.
    *
    *  For the #ENTER%ing Simplex algorithms, the feasibility vector is
    *  maintained to fullfill its bounds. As #fVec itself, also its
    *  bound depend on the chosen representation. Further, they may
    *  need to be shifted (see below).
    */
   VectorBase<R>& lbBound()
   {
      return theLBbound;
   }

   /// Violations of \ref soplex::SPxSolverBase<R>::fVec "fVec"
   /** For the leaving Simplex algorithm, pricing involves selecting a
    *  variable from #fVec that violates its bounds that is to leave
    *  the basis. When a SPxPricer is called to select such a
    *  leaving variable, #fTest() contains the vector of violations:
    *  For #fTest()[i] < 0, the \c i 'th basic variable violates one of
    *  its bounds by the given value. Otherwise no bound is violated.
    */
   const VectorBase<R>& fTest() const
   {
      assert(type() == LEAVE);
      return theCoTest;
   }

   /// copricing vector.
   /** The copricing vector #coPvec along with the pricing vector
    *  #pVec are used for pricing in the #ENTER%ing Simplex algorithm,
    *  i.e. one variable is selected, that violates its bounds. In
    *  contrast to this, the #LEAVE%ing Simplex algorithm keeps both
    *  vectors within their bounds.
    */
   UpdateVector<R>& coPvec() const
   {
      return *theCoPvec;
   }

   /// Right-hand side vector for \ref soplex::SPxSolverBase<R>::coPvec "coPvec".
   /** The vector #coPvec is computed by solving a linear system with the
    *  basis matrix and #coPrhs as the right-hand side vector. For
    *  column basis representation, #coPrhs is build up of the
    *  objective vector elements of all basic variables. For a row
    *  basis, it consists of the tight bounds of all basic
    *  constraints.
    */
   const VectorBase<R>& coPrhs() const
   {
      return *theCoPrhs;
   }

   ///
   const VectorBase<R>& ucBound() const
   {
      assert(theType == LEAVE);
      return *theCoUbound;
   }
   /// upper bound for #coPvec.
   /** This method returns the upper bound for #coPvec. It may only be
    *  called for the leaving Simplex algorithm.
    *
    *  For the leaving Simplex algorithms #coPvec is maintained to
    *  fullfill its bounds. As #coPvec itself, also its bound depend
    *  on the chosen representation. Further, they may need to be
    *  shifted (see below).
    */
   VectorBase<R>& ucBound()
   {
      assert(theType == LEAVE);
      return *theCoUbound;
   }

   ///
   const VectorBase<R>& lcBound() const
   {
      assert(theType == LEAVE);
      return *theCoLbound;
   }
   /// lower bound for #coPvec.
   /** This method returns the lower bound for #coPvec. It may only be
    *  called for the leaving Simplex algorithm.
    *
    *  For the leaving Simplex algorithms #coPvec is maintained to
    *  fullfill its bounds. As #coPvec itself, also its bound depend
    *  on the chosen representation. Further, they may need to be
    *  shifted (see below).
    */
   VectorBase<R>& lcBound()
   {
      assert(theType == LEAVE);
      return *theCoLbound;
   }

   /// violations of \ref soplex::SPxSolverBase<R>::coPvec "coPvec".
   /** In entering Simplex pricing selects checks vectors #coPvec()
    *  and #pVec() for violation of its bounds. #coTest() contains
    *  the violations for #coPvec() which are indicated by a negative
    *  value. That is, if #coTest()[i] < 0, the \p i 'th element of #coPvec()
    *  is violated by -#coTest()[i].
    */
   const VectorBase<R>& coTest() const
   {
      assert(type() == ENTER);
      return theCoTest;
   }
   /// pricing vector.
   /** The pricing vector #pVec is the product of #coPvec with the
    *  constraint matrix. As #coPvec, also #pVec is maintained within
    *  its bound for the leaving Simplex algorithm, while the bounds
    *  are tested for the entering Simplex. #pVec is of dimension
    *  #coDim(). Vector #pVec() is only up to date for #LEAVE%ing
    *  Simplex or #FULL pricing in #ENTER%ing Simplex.
    */
   UpdateVector<R>& pVec() const
   {
      return *thePvec;
   }
   ///
   const VectorBase<R>& upBound() const
   {
      assert(theType == LEAVE);
      return *theUbound;
   }
   /// upper bound for #pVec.
   /** This method returns the upper bound for #pVec. It may only be
    *  called for the leaving Simplex algorithm.
    *
    *  For the leaving Simplex algorithms #pVec is maintained to
    *  fullfill its bounds. As #pVec itself, also its bound depend
    *  on the chosen representation. Further, they may need to be
    *  shifted (see below).
    */
   VectorBase<R>& upBound()
   {
      assert(theType == LEAVE);
      return *theUbound;
   }

   ///
   const VectorBase<R>& lpBound() const
   {
      assert(theType == LEAVE);
      return *theLbound;
   }
   /// lower bound for #pVec.
   /** This method returns the lower bound for #pVec. It may only be
    *  called for the leaving Simplex algorithm.
    *
    *  For the leaving Simplex algorithms #pVec is maintained to
    *  fullfill its bounds. As #pVec itself, also its bound depend
    *  on the chosen representation. Further, they may need to be
    *  shifted (see below).
    */
   VectorBase<R>& lpBound()
   {
      assert(theType == LEAVE);
      return *theLbound;
   }

   /// Violations of \ref soplex::SPxSolverBase<R>::pVec "pVec".
   /** In entering Simplex pricing selects checks vectors #coPvec()
    *  and #pVec() for violation of its bounds. Vector #test()
    *  contains the violations for #pVec(), i.e., if #test()[i] < 0,
    *  the i'th element of #pVec() is violated by #test()[i].
    *  Vector #test() is only up to date for #FULL pricing.
    */
   const VectorBase<R>& test() const
   {
      assert(type() == ENTER);
      return theTest;
   }

   /// compute and return \ref soplex::SPxSolverBase<R>::pVec() "pVec()"[i].
   R computePvec(int i);
   /// compute entire \ref soplex::SPxSolverBase<R>::pVec() "pVec()".
   void computePvec();
   /// compute and return \ref soplex::SPxSolverBase<R>::test() "test()"[i] in \ref soplex::SPxSolverBase<R>::ENTER "ENTER"ing Simplex.
   R computeTest(int i);
   /// compute test VectorBase<R> in \ref soplex::SPxSolverBase<R>::ENTER "ENTER"ing Simplex.
   void computeTest();

   //------------------------------------
   /**@name Shifting
    *  The task of the ratio test (implemented in SPxRatioTester classes)
    *  is to select a variable for the basis update, such that the basis
    *  remains priced (i.e. both, the pricing and copricing vectors satisfy
    *  their bounds) or feasible (i.e. the feasibility vector satisfies its
    *  bounds). However, this can lead to numerically instable basis matrices
    *  or -- after accumulation of various errors -- even to a singular basis
    *  matrix.
    *
    *  The key to overcome this problem is to allow the basis to become "a
    *  bit" infeasible or unpriced, in order provide a better choice for the
    *  ratio test to select a stable variable. This is equivalent to enlarging
    *  the bounds by a small amount. This is referred to as \em shifting.
    *
    *  These methods serve for shifting feasibility bounds, either in order
    *  to maintain numerical stability or initially for computation of
    *  phase 1. The sum of all shifts applied to any bound is stored in
    *  \ref soplex::SPxSolverBase<R>::theShift "theShift".
    *
    *  The following methods are used to shift individual bounds. They are
    *  mainly intended for stable implenentations of SPxRatioTester.
    */
   ///@{
   /// Perform initial shifting to optain an feasible or pricable basis.
   void shiftFvec();
   /// Perform initial shifting to optain an feasible or pricable basis.
   void shiftPvec();

   /// shift \p i 'th \ref soplex::SPxSolver::ubBound "ubBound" to \p to.
   void shiftUBbound(int i, R to)
   {
      assert(theType == ENTER);
      // use maximum to not count tightened bounds in case of equality shifts
      theShift += SOPLEX_MAX(to - theUBbound[i], 0.0);
      theUBbound[i] = to;
   }
   /// shift \p i 'th \ref soplex::SPxSolver::lbBound "lbBound" to \p to.
   void shiftLBbound(int i, R to)
   {
      assert(theType == ENTER);
      // use maximum to not count tightened bounds in case of equality shifts
      theShift += SOPLEX_MAX(theLBbound[i] - to, 0.0);
      theLBbound[i] = to;
   }
   /// shift \p i 'th \ref soplex::SPxSolver::upBound "upBound" to \p to.
   void shiftUPbound(int i, R to)
   {
      assert(theType == LEAVE);
      // use maximum to not count tightened bounds in case of equality shifts
      theShift += SOPLEX_MAX(to - (*theUbound)[i], 0.0);
      (*theUbound)[i] = to;
   }
   /// shift \p i 'th \ref soplex::SPxSolver::lpBound "lpBound" to \p to.
   void shiftLPbound(int i, R to)
   {
      assert(theType == LEAVE);
      // use maximum to not count tightened bounds in case of equality shifts
      theShift += SOPLEX_MAX((*theLbound)[i] - to, 0.0);
      (*theLbound)[i] = to;
   }
   /// shift \p i 'th \ref soplex::SPxSolver::ucBound "ucBound" to \p to.
   void shiftUCbound(int i, R to)
   {
      assert(theType == LEAVE);
      // use maximum to not count tightened bounds in case of equality shifts
      theShift += SOPLEX_MAX(to - (*theCoUbound)[i], 0.0);
      (*theCoUbound)[i] = to;
   }
   /// shift \p i 'th \ref soplex::SPxSolver::lcBound "lcBound" to \p to.
   void shiftLCbound(int i, R to)
   {
      assert(theType == LEAVE);
      // use maximum to not count tightened bounds in case of equality shifts
      theShift += SOPLEX_MAX((*theCoLbound)[i] - to, 0.0);
      (*theCoLbound)[i] = to;
   }
   ///
   void testBounds() const;

   /// total current shift amount.
   virtual R shift() const
   {
      return theShift;
   }
   /// remove shift as much as possible.
   virtual void unShift(void);

   /// get violation of constraints.
   virtual void qualConstraintViolation(R& maxviol, R& sumviol) const;
   /// get violations of bounds.
   virtual void qualBoundViolation(R& maxviol, R& sumviol) const;
   /// get the residuum |Ax-b|.
   virtual void qualSlackViolation(R& maxviol, R& sumviol) const;
   /// get violation of optimality criterion.
   virtual void qualRedCostViolation(R& maxviol, R& sumviol) const;
   ///@}

private:

   //------------------------------------
   /**@name Perturbation */
   ///@{
   ///
   void perturbMin(
      const UpdateVector<R>& vec, VectorBase<R>& low, VectorBase<R>& up, R eps, R delta,
      int start = 0, int incr = 1);
   ///
   void perturbMax(
      const UpdateVector<R>& vec, VectorBase<R>& low, VectorBase<R>& up, R eps, R delta,
      int start = 0, int incr = 1);
   ///
   R perturbMin(const UpdateVector<R>& uvec,
                VectorBase<R>& low, VectorBase<R>& up, R eps, R delta,
                const typename SPxBasisBase<R>::Desc::Status* stat, int start, int incr);
   ///
   R perturbMax(const UpdateVector<R>& uvec,
                VectorBase<R>& low, VectorBase<R>& up, R eps, R delta,
                const typename SPxBasisBase<R>::Desc::Status* stat, int start, int incr);
   ///@}

   //------------------------------------
   /**@name The Simplex Loop
    *  We now present a set of methods that may be usefull when implementing
    *  own SPxPricer or SPxRatioTester classes. Here is, how
    *  SPxSolverBase will call methods from its loaded SPxPricer and
    *  SPxRatioTester.
    *
    *  For the entering Simplex:
    *    -# \ref soplex::SPxPricer::selectEnter() "SPxPricer::selectEnter()"
    *    -# \ref soplex::SPxRatioTester::selectLeave() "SPxRatioTester::selectLeave()"
    *    -# \ref soplex::SPxPricer::entered4() "SPxPricer::entered4()"
    *
    *  For the leaving Simplex:
    *    -# \ref soplex::SPxPricer::selectLeave() "SPxPricer::selectLeave()"
    *    -# \ref soplex::SPxRatioTester::selectEnter() "SPxRatioTester::selectEnter()"
    *    -# \ref soplex::SPxPricer::left4() "SPxPricer::left4()"
    */
   ///@{
public:
   /// Setup vectors to be solved within Simplex loop.
   /** Load vector \p y to be #solve%d with the basis matrix during the
    *  #LEAVE Simplex. The system will be solved after #SPxSolverBase%'s call
    *  to SPxRatioTester.  The system will be solved along with
    *  another system. Solving two linear system at a time has
    *  performance advantages over solving the two linear systems
    *  seperately.
    */
   void setup4solve(SSVectorBase<R>* p_y, SSVectorBase<R>* p_rhs)
   {
      assert(type() == LEAVE);
      solveVector2    = p_y;
      solveVector2rhs = p_rhs;
   }
   /// Setup vectors to be solved within Simplex loop.
   /** Load a second additional vector \p y2 to be #solve%d with the
    *  basis matrix during the #LEAVE Simplex. The system will be
    *  solved after #SPxSolverBase%'s call to SPxRatioTester.
    *  The system will be solved along with at least one
    *  other system. Solving several linear system at a time has
    *  performance advantages over solving them seperately.
    */
   void setup4solve2(SSVectorBase<R>* p_y2, SSVectorBase<R>* p_rhs2)
   {
      assert(type() == LEAVE);
      solveVector3    = p_y2;
      solveVector3rhs = p_rhs2;
   }
   /// Setup vectors to be cosolved within Simplex loop.
   /** Load vector \p y to be #coSolve%d with the basis matrix during
    *  the #ENTER Simplex. The system will be solved after #SPxSolverBase%'s
    *  call to SPxRatioTester.  The system will be solved along
    *  with another system. Solving two linear system at a time has
    *  performance advantages over solving the two linear systems
    *  seperately.
    */
   void setup4coSolve(SSVectorBase<R>* p_y, SSVectorBase<R>* p_rhs)
   {
      assert(type() == ENTER);
      coSolveVector2    = p_y;
      coSolveVector2rhs = p_rhs;
   }
   /// Setup vectors to be cosolved within Simplex loop.
   /** Load a second vector \p z to be #coSolve%d with the basis matrix during
    *  the #ENTER Simplex. The system will be solved after #SPxSolverBase%'s
    *  call to SPxRatioTester. The system will be solved along
    *  with two other systems.
    */
   void setup4coSolve2(SSVectorBase<R>* p_z, SSVectorBase<R>* p_rhs)
   {
      assert(type() == ENTER);
      coSolveVector3    = p_z;
      coSolveVector3rhs = p_rhs;
   }

   /// maximal infeasibility of basis
   /** This method is called before concluding optimality. Since it is
    *  possible that some stable implementation of class
    *  SPxRatioTester yielded a slightly infeasible (or unpriced)
    *  basis, this must be checked before terminating with an optimal
    *  solution.
    */
   virtual R maxInfeas() const;

   /// check for violations above tol and immediately return false w/o checking the remaining values
   /** This method is useful for verifying whether an objective limit can be used as termination criterion
    */
   virtual bool noViols(R tol) const;

   /// Return current basis.
   /**@note The basis can be used to solve linear systems or use
    *  any other of its (const) methods.  It is, however, encuraged
    *  to use methods #setup4solve() and #setup4coSolve() for solving
    *  systems, since this is likely to have perfomance advantages.
    */
   const SPxBasisBase<R>& basis() const
   {
      return *this;
   }
   ///
   SPxBasisBase<R>& basis()
   {
      return *this;
   }
   /// return loaded SPxPricer.
   const SPxPricer<R>* pricer() const
   {
      return thepricer;
   }
   /// return loaded SLinSolver.
   const SLinSolver<R>* slinSolver() const
   {
      return SPxBasisBase<R>::factor;
   }
   /// return loaded SPxRatioTester.
   const SPxRatioTester<R>* ratiotester() const
   {
      return theratiotester;
   }

   /// Factorize basis matrix.
   /// @throw SPxStatusException if loaded matrix is singular
   virtual void factorize();

private:

   /** let index \p i leave the basis and manage entering of another one.
       @returns \c false if LP is unbounded/infeasible. */
   bool leave(int i, bool polish = false);
   /** let id enter the basis and manage leaving of another one.
       @returns \c false if LP is unbounded/infeasible. */
   bool enter(SPxId& id, bool polish = false);

   /// test coVector \p i with status \p stat.
   R coTest(int i, typename SPxBasisBase<R>::Desc::Status stat) const;
   /// compute coTest vector.
   void computeCoTest();
   /// recompute coTest vector.
   void updateCoTest();

   /// test VectorBase<R> \p i with status \p stat.
   R test(int i, typename SPxBasisBase<R>::Desc::Status stat) const;
   /// recompute test vector.
   void updateTest();

   /// compute basis feasibility test vector.
   void computeFtest();
   /// update basis feasibility test vector.
   void updateFtest();

   ///@}

   //------------------------------------
   /**@name Parallelization
    *  In this section we present the methods, that are provided in order to
    *  allow a parallel version to be implemented as a derived class, thereby
    *  inheriting most of the code of SPxSolverBase.
    *
    *  @par Initialization
    *  These methods are used to setup all the vectors used in the Simplex
    *  loop, that where described in the previous sectios.
    */
   ///@{
public:
   /// intialize data structures.
   /** If SPxSolverBase is not \ref isInitialized() "initialized", the method
    *  #solve() calls #init() to setup all vectors and internal data structures.
    *  Most of the other methods within this section are called by #init().
    *
    *  Derived classes should add the initialization of additional
    *  data structures by overriding this method. Don't forget,
    *  however, to call SPxSolverBase<R>::init().
    */
   virtual void init();

protected:

   /// has the internal data been initialized?
   /** As long as an instance of SPxSolverBase is not initialized, no member
    *  contains setup data. Initialization is performed via method
    *  #init().  Afterwards all data structures are kept up to date (even
    *  for all manipulation methods), until #unInit() is called. However,
    *  some manipulation methods call #unInit() themselfs.
    */
   bool isInitialized() const
   {
      return initialized;
   }

   /// resets clock average statistics
   void resetClockStats();

   /// uninitialize data structures.
   virtual void unInit()
   {
      initialized = false;
   }
   /// setup all vecs fresh
   virtual void reinitializeVecs();
   /// reset dimensions of vectors according to loaded LP.
   virtual void reDim();
   /// compute feasibility vector from scratch.
   void computeFrhs();
   ///
   virtual void computeFrhsXtra();
   ///
   virtual void computeFrhs1(const VectorBase<R>&, const VectorBase<R>&);
   ///
   void computeFrhs2(VectorBase<R>&, VectorBase<R>&);
   /// compute \ref soplex::SPxSolverBase<R>::theCoPrhs "theCoPrhs" for entering Simplex.
   virtual void computeEnterCoPrhs();
   ///
   void computeEnterCoPrhs4Row(int i, int n);
   ///
   void computeEnterCoPrhs4Col(int i, int n);
   /// compute \ref soplex::SPxSolverBase<R>::theCoPrhs "theCoPrhs" for leaving Simplex.
   virtual void computeLeaveCoPrhs();
   ///
   void computeLeaveCoPrhs4Row(int i, int n);
   ///
   void computeLeaveCoPrhs4Col(int i, int n);

   /// Compute part of objective value.
   /** This method is called from #value() in order to compute the part of
    *  the objective value resulting form nonbasic variables for #COLUMN
    *  Representation.
    */
   R nonbasicValue();

   /// Get pointer to the \p id 'th vector
   virtual const SVectorBase<R>* enterVector(const SPxId& p_id)
   {
      assert(p_id.isValid());
      return p_id.isSPxRowId()
             ? &vector(SPxRowId(p_id)) : &vector(SPxColId(p_id));
   }
   ///
   virtual void getLeaveVals(int i,
                             typename SPxBasisBase<R>::Desc::Status& leaveStat, SPxId& leaveId,
                             R& leaveMax, R& leavebound, int& leaveNum, StableSum<R>& objChange);
   ///
   virtual void getLeaveVals2(R leaveMax, SPxId enterId,
                              R& enterBound, R& newUBbound,
                              R& newLBbound, R& newCoPrhs, StableSum<R>& objChange);
   ///
   virtual void getEnterVals(SPxId id, R& enterTest,
                             R& enterUB, R& enterLB, R& enterVal, R& enterMax,
                             R& enterPric, typename SPxBasisBase<R>::Desc::Status& enterStat, R& enterRO,
                             StableSum<R>& objChange);
   ///
   virtual void getEnterVals2(int leaveIdx,
                              R enterMax, R& leaveBound, StableSum<R>& objChange);
   ///
   virtual void ungetEnterVal(SPxId enterId, typename SPxBasisBase<R>::Desc::Status enterStat,
                              R leaveVal, const SVectorBase<R>& vec, StableSum<R>& objChange);
   ///
   virtual void rejectEnter(SPxId enterId,
                            R enterTest, typename SPxBasisBase<R>::Desc::Status enterStat);
   ///
   virtual void rejectLeave(int leaveNum, SPxId leaveId,
                            typename SPxBasisBase<R>::Desc::Status leaveStat, const SVectorBase<R>* newVec = nullptr);
   ///
   virtual void setupPupdate(void);
   ///
   virtual void doPupdate(void);
   ///
   virtual void clearUpdateVecs(void);
   ///
   virtual void perturbMinEnter(void);
   /// perturb basis bounds.
   virtual void perturbMaxEnter(void);
   ///
   virtual void perturbMinLeave(void);
   /// perturb nonbasic bounds.
   virtual void perturbMaxLeave(void);
   ///@}

   //------------------------------------
   /** The following methods serve for initializing the bounds for dual or
    *  primal Simplex algorithm of entering or leaving type.
    */
   ///@{
   ///
   void clearDualBounds(typename SPxBasisBase<R>::Desc::Status, R&, R&) const;
   ///
   void setDualColBounds();
   ///
   void setDualRowBounds();
   /// setup feasibility bounds for entering algorithm
   void setPrimalBounds();
   ///
   void setEnterBound4Col(int, int);
   ///
   void setEnterBound4Row(int, int);
   ///
   virtual void setEnterBounds();
   ///
   void setLeaveBound4Row(int i, int n);
   ///
   void setLeaveBound4Col(int i, int n);
   ///
   virtual void setLeaveBounds();
   ///@}

   //------------------------------------
   /** Compute the primal ray or the farkas proof in case of unboundedness
    *  or infeasibility.
    */
   ///@{
   ///
   void computePrimalray4Col(R direction, SPxId enterId);
   ///
   void computePrimalray4Row(R direction);
   ///
   void computeDualfarkas4Col(R direction);
   ///
   void computeDualfarkas4Row(R direction, SPxId enterId);
   ///@}

public:

   //------------------------------------
   /** Limits and status inquiry */
   ///@{
   /// set time limit.
   virtual void setTerminationTime(Real time = infinity);
   /// return time limit.
   virtual Real terminationTime() const;
   /// set iteration limit.
   virtual void setTerminationIter(int iteration = -1);
   /// return iteration limit.
   virtual int terminationIter() const;
   /// set objective limit.
   virtual void setTerminationValue(R value = R(infinity));
   /// return objective limit.
   virtual R terminationValue() const;
   /// get objective value of current solution.
   virtual R objValue()
   {
      return value();
   }
   /// get all results of last solve.
   Status
   getResult(R* value = 0, VectorBase<R>* primal = 0,
             VectorBase<R>* slacks = 0, VectorBase<R>* dual = 0,
             VectorBase<R>* reduCost = 0);

protected:

   /**@todo put the following basis methods near the variable status methods!*/
   /// converts basis status to VarStatus
   VarStatus basisStatusToVarStatus(typename SPxBasisBase<R>::Desc::Status stat) const;

   /// converts VarStatus to basis status for rows
   typename SPxBasisBase<R>::Desc::Status varStatusToBasisStatusRow(int row, VarStatus stat)
   const;

   /// converts VarStatus to basis status for columns
   typename SPxBasisBase<R>::Desc::Status varStatusToBasisStatusCol(int col, VarStatus stat)
   const;

public:

   /// gets basis status for a single row
   VarStatus getBasisRowStatus(int row) const;

   /// gets basis status for a single column
   VarStatus getBasisColStatus(int col) const;

   /// get current basis, and return solver status.
   Status getBasis(VarStatus rows[], VarStatus cols[], const int rowsSize = -1,
                   const int colsSize = -1) const;

   /// gets basis status
   typename SPxBasisBase<R>::SPxStatus getBasisStatus() const
   {
      return SPxBasisBase<R>::status();
   }

   /// check a given basis for validity.
   bool isBasisValid(DataArray<VarStatus> rows, DataArray<VarStatus> cols);

   /// set the lp solver's basis.
   void setBasis(const VarStatus rows[], const VarStatus cols[]);

   /// set the lp solver's basis status.
   void setBasisStatus(typename SPxBasisBase<R>::SPxStatus stat)
   {
      if(m_status == OPTIMAL)
         m_status = UNKNOWN;

      SPxBasisBase<R>::setStatus(stat);
   }

   /// setting the solver status external from the solve loop.
   void setSolverStatus(typename SPxSolverBase<R>::Status stat)
   {
      m_status = stat;
   }

   /// get level of dual degeneracy
   // this function is used for the improved dual simplex
   R getDegeneracyLevel(VectorBase<R> degenvec);

   /// get number of dual norms
   void getNdualNorms(int& nnormsRow, int& nnormsCol) const;

   /// get dual norms
   bool getDualNorms(int& nnormsRow, int& nnormsCol, R* norms) const;

   /// set dual norms
   bool setDualNorms(int nnormsRow, int nnormsCol, R* norms);

   /// pass integrality information about the variables to the solver
   void setIntegralityInformation(int ncols, int* intInfo);

   /// reset cumulative time counter to zero.
   void resetCumulativeTime()
   {
      theCumulativeTime = 0.0;
   }

   /// get number of bound flips.
   int boundFlips() const
   {
      return totalboundflips;
   }

   /// get number of dual degenerate pivots
   int dualDegeneratePivots()
   {
      return (rep() == ROW) ? enterCycles : leaveCycles;
   }

   /// get number of primal degenerate pivots
   int primalDegeneratePivots()
   {
      return (rep() == ROW) ? leaveCycles : enterCycles;
   }

   /// get the sum of dual degeneracy
   R sumDualDegeneracy()
   {
      return dualDegenSum;
   }

   /// get the sum of primal degeneracy
   R sumPrimalDegeneracy()
   {
      return primalDegenSum;
   }

   /// get number of iterations of current solution.
   int iterations() const
   {
      return basis().iteration();
   }

   /// return number of iterations done with primal algorithm
   int primalIterations()
   {
      assert(iterations() == 0 || primalCount <= iterations());
      return (iterations() == 0) ? 0 : primalCount;
   }

   /// return number of iterations done with primal algorithm
   int dualIterations()
   {
      return iterations() - primalIterations();
   }

   /// return number of iterations done with primal algorithm
   int polishIterations()
   {
      return polishCount;
   }

   /// time spent in last call to method solve().
   Real time() const
   {
      return theTime->time();
   }

   /// returns whether current time limit is reached; call to time() may be skipped unless \p forceCheck is true
   ///
   bool isTimeLimitReached(const bool forceCheck = false);

   /// the maximum runtime
   Real getMaxTime()
   {
      return maxTime;
   }

   /// cumulative time spent in all calls to method solve().
   Real cumulativeTime() const
   {
      return theCumulativeTime;
   }

   /// the maximum number of iterations
   int getMaxIters()
   {
      return maxIters;
   }

   /// return const lp's rows if available.
   const LPRowSetBase<R>& rows() const
   {
      return *this->lprowset();
   }

   /// return const lp's cols if available.
   const LPColSet& cols() const
   {
      return *this->lpcolset();
   }

   /// copy lower bound VectorBase<R> to \p p_low.
   void getLower(VectorBase<R>& p_low) const
   {
      p_low = SPxLPBase<R>::lower();
   }
   /// copy upper bound VectorBase<R> to \p p_up.
   void getUpper(VectorBase<R>& p_up) const
   {
      p_up = SPxLPBase<R>::upper();
   }

   /// copy lhs value VectorBase<R> to \p p_lhs.
   void getLhs(VectorBase<R>& p_lhs) const
   {
      p_lhs = SPxLPBase<R>::lhs();
   }

   /// copy rhs value VectorBase<R> to \p p_rhs.
   void getRhs(VectorBase<R>& p_rhs) const
   {
      p_rhs = SPxLPBase<R>::rhs();
   }

   /// optimization sense.
   typename SPxLPBase<R>::SPxSense sense() const
   {
      return this->spxSense();
   }

   /// returns statistical information in form of a string.
   std::string statistics() const
   {
      std::stringstream s;
      s << basis().statistics()
        << "Solution time      : " << std::setw(10) << std::fixed << std::setprecision(
           2) << time() << std::endl
        << "Iterations         : " << std::setw(10) << iterations() << std::endl;

      return s.str();
   }

   ///@}

   //------------------------------------
   /** Mapping between numbers and Ids */
   ///@{
   /// RowId of \p i 'th inequality.
   SPxRowId rowId(int i) const
   {
      return this->rId(i);
   }
   /// ColId of \p i 'th column.
   SPxColId colId(int i) const
   {
      return this->cId(i);
   }
   ///@}

   //------------------------------------
   /** Constructors / destructors */
   ///@{
   /// default constructor.
   explicit
   SPxSolverBase(Type            type  = LEAVE,
                 Representation  rep   = ROW,
                 Timer::TYPE     ttype = Timer::USER_TIME);
   // virtual destructor
   virtual ~SPxSolverBase();
   ///@}

   //------------------------------------
   /** Miscellaneous */
   ///@{
   /// check consistency.
   bool isConsistent() const;
   ///@}

   //------------------------------------
   /** assignment operator and copy constructor */
   ///@{
   /// assignment operator
   SPxSolverBase<R>& operator=(const SPxSolverBase<R>& base);
   /// copy constructor
   SPxSolverBase(const SPxSolverBase<R>& base);
   ///@}

   void testVecs();
};

//
// Auxiliary functions.
//

/// Pretty-printing of variable status.
template <class R>
std::ostream& operator<<(std::ostream& os,
                         const typename SPxSolverBase<R>::VarStatus& status);

/// Pretty-printing of solver status.
template <class R>
std::ostream& operator<<(std::ostream& os,
                         const typename SPxSolverBase<R>::Status& status);

/// Pretty-printing of algorithm.
template <class R>
std::ostream& operator<<(std::ostream& os,
                         const typename SPxSolverBase<R>::Type& status);

/// Pretty-printing of representation.
template <class R>
std::ostream& operator<<(std::ostream& os,
                         const typename SPxSolverBase<R>::Representation& status);

/* For Backwards compatibility */
typedef SPxSolverBase<Real> SPxSolver;

} // namespace soplex

// For general templated functions
#include "spxsolver.hpp"
#include "spxsolve.hpp"
#include "changesoplex.hpp"
#include "leave.hpp"
#include "enter.hpp"
#include "spxshift.hpp"
#include "spxbounds.hpp"
#include "spxchangebasis.hpp"
#include "spxvecs.hpp"
#include "spxwritestate.hpp"
#include "spxfileio.hpp"
#include "spxquality.hpp"

#endif // _SPXSOLVER_H_
