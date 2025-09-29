/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the program and library             */
/*         SCIP --- Solving Constraint Integer Programs                      */
/*                                                                           */
/*  Copyright (c) 2002-2024 Zuse Institute Berlin (ZIB)                      */
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
/*  along with SCIP; see the file LICENSE. If not visit scipopt.org.         */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**@file   branch_generaldisjunction.cpp
 * @ingroup DEFPLUGINS_BRANCH
 * @brief  branching rule with general disjunction
 * @author Yugeng Zhou
 */

/*---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/


#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "scip/scip_var.h"
#include "scip/scip_branch.h"
#include "branch_generaldisjunction.h"
#include "scip/branch_relpscost.h"
#include "objbranchrule.h"
#include "scip/scipdefplugins.h"
#include "scip/struct_lp.h"
#include "scip/scip.h"
#include <iomanip>
#include "scip/prob.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <scip/tree.h>
#include <limits>
#include <cmath>


#define scip_name_            "general_disjunction"
#define scip_desc_           "branching rule with general disjunctions"
#define scip_priority_        10
#define scip_maxdepth_        -1
#define scip_maxbounddist_    1.0

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Data structures
 */
/** branching rule data */
static SCIP_Longint g_total_milp_nodes = 0;      // Fair nodes from MILP solving
static SCIP_Longint g_total_probing_lps = 0;     // Probing LP solves
static SCIP_Longint g_total_milps = 0;           // MILP solves (submodels)

class BranchruleGeneralDisjunction : public scip::ObjBranchrule {
public:
   // Parameters for submodel solving
   int M = 1;
   int k = 5;
   SCIP_Real base_delta = 0.1;
   const double TIME_LIMIT_SECONDS = 1000.0;

   explicit BranchruleGeneralDisjunction(SCIP* scip)
         : ObjBranchrule(scip, scip_name_, scip_desc_, scip_priority_, scip_maxdepth_, scip_maxbounddist_) {}
   virtual SCIP_DECL_BRANCHEXECLP(scip_execlp);

};

/*
 * Local methods
 */

 /* 
 * Methods for testing
 */

/* Create the small submodel for testing system (4)*/
static
SubmodelVars submodelsmall_create(
       SCIP* scip,
       CSRMatrix A,
       vector<SCIP_Real> b,
       vector<SCIP_Real> c,
       int M,
       int k,
       SCIP_Real delta,
       SCIP_Real zl,
       SCIP_Real matrix_range,
       SCIP_Real time_limit
){
   // Create the submodel
   size_t m = b.size();
   size_t n = c.size();
   SCIP *model_sub_s;
   SCIP_RETCODE retcode;
   retcode = SCIPcreate(&model_sub_s);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }
   retcode = SCIPincludeDefaultPlugins(model_sub_s);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub_s);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }  
   retcode = SCIPcreateProbBasic(model_sub_s, "sub_small");
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub_s);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }

   // Define vector variables
   vector<SCIP_VAR *> p(m);
   vector<SCIP_VAR *> q(m);
   vector<SCIP_VAR *> pi_plus(n);
   vector<SCIP_VAR *> pi_minus(n);
   SCIP_VAR *pi0;

   // Create variables

   for (size_t i = 0; i < m; ++i) {
      retcode = SCIPcreateVarBasic(model_sub_s, &p[i], ("p_" + to_string(i)).c_str(), 0.0, SCIPinfinity(model_sub_s), 0.0, SCIP_VARTYPE_CONTINUOUS);
      if (retcode != SCIP_OKAY) {
         SCIPprintError(retcode);
         SCIPfree(&model_sub_s);
         return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
      }
      SCIP_CALL_ABORT(SCIPaddVar(model_sub_s, p[i]));
      
   }

   for (size_t i = 0; i < m; ++i) {
      retcode = SCIPcreateVarBasic(model_sub_s, &q[i], ("q_" + to_string(i)).c_str(), 0.0, SCIPinfinity(model_sub_s), 0.0, SCIP_VARTYPE_CONTINUOUS);
      if (retcode != SCIP_OKAY) {
         SCIPprintError(retcode);
         SCIPfree(&model_sub_s);
         return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
      }
      SCIP_CALL_ABORT(SCIPaddVar(model_sub_s, q[i]));
   }

   for (size_t j = 0; j < n; ++j) {
      retcode = SCIPcreateVarBasic(model_sub_s, &pi_plus[j], ("pi_plus_" + to_string(j)).c_str(), 0, M, 0.0, SCIP_VARTYPE_INTEGER);
      if (retcode != SCIP_OKAY) {
         SCIPprintError(retcode);
         SCIPfree(&model_sub_s);
         return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
      }
      SCIP_CALL_ABORT(SCIPaddVar(model_sub_s, pi_plus[j]));

      retcode = SCIPcreateVarBasic(model_sub_s, &pi_minus[j], ("pi_minus_" + to_string(j)).c_str(), 0, M, 0.0, SCIP_VARTYPE_INTEGER);
      if (retcode != SCIP_OKAY) {
         SCIPprintError(retcode);
         SCIPfree(&model_sub_s);
         return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
      }
      SCIP_CALL_ABORT(SCIPaddVar(model_sub_s, pi_minus[j]));
   }

   retcode = SCIPcreateVarBasic(model_sub_s, &pi0, "pi0", -SCIPinfinity(model_sub_s), SCIPinfinity(model_sub_s), 0.0, SCIP_VARTYPE_INTEGER);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub_s);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }
   SCIP_CALL_ABORT(SCIPaddVar(model_sub_s, pi0));

   // Add constraints
   //pA - pi = 0
   CSRMatrix At = A.transpose();
   for (size_t j = 0; j < n; ++j ){
      SCIP_CONS* cons;
      SCIPcreateConsBasicLinear(model_sub_s, &cons, ("cons_p_" + to_string(j)).c_str(), 0, nullptr, nullptr, 0.0, 0.0);

      for (int i = At.row_ptr[j]; i < At.row_ptr[j + 1]; ++i) {
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, p[At.col_indices[i]], At.values[i]));
      }
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, pi_plus[j], -1.0));
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, pi_minus[j], 1.0));
      SCIP_CALL_ABORT(SCIPaddCons(model_sub_s, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_sub_s, &cons));
   }

   // qA + pi = 0
   for (size_t j = 0; j < n; ++j ){
      SCIP_CONS* cons;
      SCIPcreateConsBasicLinear(model_sub_s, &cons, ("cons_q_" + to_string(j)).c_str(), 0, nullptr, nullptr, 0.0, 0.0);

      for (int i = A.row_ptr[j]; i < A.row_ptr[j + 1]; ++i) {
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, q[A.col_indices[i]], A.values[i]));
      }
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, pi_plus[j], 1.0));
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, pi_minus[j], -1.0));
      SCIP_CALL_ABORT(SCIPaddCons(model_sub_s, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_sub_s, &cons));
   
   }

   //pb - pi0 >= delta
   SCIP_CONS* cons_p_b;
   SCIPcreateConsBasicLinear(model_sub_s, &cons_p_b, "cons_p_b", 0, nullptr, nullptr, delta, SCIPinfinity(model_sub_s));
   for (size_t i = 0; i < m; ++i) {
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons_p_b, p[i], b[i]));
   }
   SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons_p_b, pi0, -1.0));
   SCIP_CALL_ABORT(SCIPaddCons(model_sub_s, cons_p_b));
   SCIP_CALL_ABORT(SCIPreleaseCons(model_sub_s, &cons_p_b));

   //qb + pi0 >= delta - 1
   SCIP_CONS* cons_q_b;
   SCIPcreateConsBasicLinear(model_sub_s, &cons_q_b, "cons_q_b", 0, nullptr, nullptr, delta - 1, SCIPinfinity(model_sub_s));
   for (size_t i = 0; i < m; ++i) {
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons_q_b, q[i], b[i]));
   }
   SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons_q_b, pi0, 1.0));
   SCIP_CALL_ABORT(SCIPaddCons(model_sub_s, cons_q_b));
   SCIP_CALL_ABORT(SCIPreleaseCons(model_sub_s, &cons_q_b));

   // Add constraint that if var is continuous then the corresponding pi[i] = 0
   for (size_t i = 0; i < n; ++i) {
      SCIP_CONS* cons;
      SCIP_CONS* cons1;
      SCIPcreateConsBasicLinear(model_sub_s, &cons, ("cons_pi_p_" + to_string(i)).c_str(), 0, nullptr, nullptr, 0.0, 0.0);
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, pi_plus[i], 1.0));
      SCIP_CALL_ABORT(SCIPaddCons(model_sub_s, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_sub_s, &cons));

      SCIPcreateConsBasicLinear(model_sub_s, &cons1, ("cons_pi_m_" + to_string(i)).c_str(), 0, nullptr, nullptr, 0.0, 0.0);
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons1, pi_minus[i], 1.0));
      SCIP_CALL_ABORT(SCIPaddCons(model_sub_s, cons1));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_sub_s, &cons1));
   }

   // Check if the LP relaxation of the original problem is optimal
   SCIP_LPSOLSTAT status_LP = SCIPgetLPSolstat(scip);
   if (status_LP == SCIP_LPSOLSTAT_OPTIMAL) {
      vector<SCIP_Real> x_star(n);
      SCIP_Real epsilon = 1e-3;
      SCIP_COL **lp_cols = SCIPgetLPCols(scip);
      assert (lp_cols != nullptr);
      for (size_t j = 0; j < n; ++j) {
         x_star[j] = SCIPgetSolVal(scip, nullptr, SCIPcolGetVar(lp_cols[j]));
      }

      // Add constraints pi0 <= sum((pi_plus[i] - pi_minus[i]) * x_star[i]) - epsilon
      {
         SCIP_CONS *cons;
         // Lower constraint: pi0 <= sum((pi_plus[i] - pi_minus[i]) * x_star[i]) - epsilon
         SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub_s, &cons, "cons_pi0_lower", 0, nullptr, nullptr, epsilon, SCIPinfinity(model_sub_s)));
         for (size_t i = 0; i < n; ++i) {
            SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, pi_plus[i], x_star[i]));
            SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, pi_minus[i], -x_star[i]));
         }
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, pi0, -1.0));
         SCIP_CALL_ABORT(SCIPaddCons(model_sub_s, cons));
         SCIP_CALL_ABORT(SCIPreleaseCons(model_sub_s, &cons));
      }

      // Add constraints pi0 >= sum((pi_plus[i] - pi_minus[i]) * x_star[i]) + epsilon - 1
      {
         SCIP_CONS *cons;
         SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub_s, &cons, "cons_pi0_upper", 0, nullptr, nullptr, -SCIPinfinity(model_sub_s), 1 - epsilon));
         for (size_t i = 0; i < n; ++i) {
            SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, pi_plus[i], x_star[i]));
            SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, pi_minus[i], -x_star[i]));
         }
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub_s, cons, pi0, -1.0));
         SCIP_CALL_ABORT(SCIPaddCons(model_sub_s, cons));
         SCIP_CALL_ABORT(SCIPreleaseCons(model_sub_s, &cons));
      }
   }

   retcode = SCIPreadParams(model_sub_s, "/scratch/htc/yzhou/exp_scipmip/settings/default1.set");
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub_s);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }



   retcode = SCIPsetRealParam(model_sub_s, "limits/time", time_limit);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub_s);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }
   SCIPsetMessagehdlrQuiet(model_sub_s, TRUE);

   if (matrix_range < 1e-9){
      matrix_range = 1e-7;
      SCIPsetRealParam(model_sub_s, "numerics/feastol", matrix_range);
      SCIPsetRealParam(model_sub_s, "numerics/sumepsilon", matrix_range);
   }
   
   return SubmodelVars{model_sub_s, p, {}, q, {}, pi_plus, pi_minus, pi0};

}

static
SCIP_Real getMagnitudeBase(SCIP_Real x) {
   x = std::abs(x);
   if (x < 1e-17) return 0.0;
   SCIP_Real mag = 1.0;
   while (x >= 10.0) {
      x /= 10.0;
      mag *= 10.0;
   }
   while (x < 1.0) {
      x *= 10.0;
      mag /= 10.0;
   }
   return mag;
}

/* Consult relpscost branching rule for variable evaluation */
static
SCIP_RESULT consultRelpscost(
    SCIP* scip,
    SCIP_VAR** lpcands,
    SCIP_Real* lpcandssol,
    SCIP_Real* lpcandsfrac,
    int nlpcands,
    SCIP_Bool executebranching = FALSE
) {

   SCIP_RESULT relpscost_result;
   
   // Call relpscost for evaluation (default: do not execute branching)
   SCIP_CALL_ABORT(SCIPexecRelpscostBranching(scip, lpcands, lpcandssol, lpcandsfrac,
                                             nlpcands, executebranching, &relpscost_result));
   
   return relpscost_result;
}

/* Check if not all elements are zero */
static
SCIP_Bool notallzero(const vector <int>& vec) {
   return std::any_of(vec.begin(), vec.end(), [](double val) { return abs(val) - 1e-6 > 0.0; });
}

/*
* Methods for solving
*/

/* get the LP constraint matrix A, vector b and objective vector c*/
static
MatrixData getConstraintMatrix(SCIP* scip) {
   SCIP_LPSOLSTAT status_LP = SCIPgetLPSolstat(scip);
   SCIP_COL** cols = SCIPgetLPCols(scip);
   SCIP_ROW** rows = SCIPgetLPRows(scip);
   size_t ncols = SCIPgetNLPCols(scip);
   size_t nrows = SCIPgetNLPRows(scip);

   MatrixData LP_data;
   LP_data.c.resize(ncols);
   LP_data.b.reserve(nrows * 2); // Reserve extra space for equality constraints

   // Extract objective coefficients
   for (size_t i = 0; i < ncols; ++i) {
      LP_data.c[i] = SCIPcolGetObj(cols[i]);
   }

   // Initialize row_ptr with the starting index of each row
   LP_data.A.row_ptr.push_back(0);
   int count_eq = 0;
   int count_range = 0;
   vector<SCIP_Real> x_star(LP_data.c.size());
   if (status_LP == SCIP_LPSOLSTAT_OPTIMAL) {

      SCIP_COL **lp_cols = SCIPgetLPCols(scip);

      for (size_t j = 0; j < LP_data.c.size(); ++j) {
         
         x_star[j] = SCIPgetSolVal(scip, nullptr, SCIPcolGetVar(lp_cols[j]));
      }
   }
   for (size_t i = 0; i < nrows; ++i) {
      SCIP_ROW* row = rows[i];
      SCIP_COL** rowcols = SCIProwGetCols(row); // Nonzero columns
      SCIP_Real* rowvals = SCIProwGetVals(row); // Nonzero values
      int num_nonz = SCIProwGetNNonz(row); // Number of nonzeros

      SCIP_Real constant = SCIProwGetConstant(row);

      for (int j = 0; j < num_nonz; ++j) {
         int colindex = SCIPcolGetLPPos(rowcols[j]);
         LP_data.A.values.push_back(rowvals[j]);
         LP_data.A.col_indices.push_back(colindex);
      }

      // Handle the constraint right-hand side
      SCIP_Real lhs = SCIProwGetLhs(row) - constant;
      SCIP_Real rhs = SCIProwGetRhs(row) - constant;

      if (abs(lhs - rhs) < 1e-06) { // Equality constraint: Convert to two inequalities
         LP_data.b.push_back(lhs); // Ax >= b
         LP_data.A.row_ptr.push_back(LP_data.A.values.size());
         // Convert Ax ≤ b to -Ax ≥ -b
         for (int j = 0; j < num_nonz; ++j) {
            int colindex = SCIPcolGetLPPos(rowcols[j]);
            LP_data.A.values.push_back(-rowvals[j]);
            LP_data.A.col_indices.push_back(colindex);
         }
         LP_data.b.push_back(-lhs);
         LP_data.A.row_ptr.push_back(LP_data.A.values.size());
         count_eq++;
      }
      else if (lhs != -SCIPinfinity(scip) && rhs != SCIPinfinity(scip)) {
         LP_data.b.push_back(lhs);
         LP_data.A.row_ptr.push_back(LP_data.A.values.size());
         for (int j = 0; j < num_nonz; ++j) {
            int colindex = SCIPcolGetLPPos(rowcols[j]);
            LP_data.A.values.push_back(-rowvals[j]);
            LP_data.A.col_indices.push_back(colindex);
         }
         LP_data.b.push_back(-rhs);
         LP_data.A.row_ptr.push_back(LP_data.A.values.size());
         count_range++;
      }
      else if (lhs == -SCIPinfinity(scip)) { // Ax ≤ rhs
         LP_data.b.push_back(-rhs);
         LP_data.A.row_ptr.push_back(LP_data.A.values.size());
         size_t abs_idx = i + count_eq + count_range;
         assert(abs_idx == LP_data.A.row_ptr.size() - 2);
         for (int j = LP_data.A.row_ptr[abs_idx]; j < LP_data.A.row_ptr[abs_idx + 1]; ++j) {
            LP_data.A.values[j] = -LP_data.A.values[j];
         }
      }
      else if (rhs == SCIPinfinity(scip)) { // Ax ≥ lhs
         LP_data.b.push_back(lhs);
         LP_data.A.row_ptr.push_back(LP_data.A.values.size());
      }
   }
   // Handle active variable bounds as constraints

   for (size_t i = 0; i < ncols; ++i) {

      SCIP_Real lb = SCIPcolGetLb(cols[i]);
      SCIP_Real ub = SCIPcolGetUb(cols[i]);
      if (lb > -SCIPinfinity(scip)) { // x >= lb
         LP_data.A.values.push_back(1.0);
         LP_data.A.col_indices.push_back(i);
         LP_data.b.push_back(lb);
         LP_data.A.row_ptr.push_back(LP_data.A.values.size());
      }
      if (ub < SCIPinfinity(scip)) { // x ≤ ub → -x ≥ -ub
         LP_data.A.values.push_back(-1.0);
         LP_data.A.col_indices.push_back(i);
         LP_data.b.push_back(-ub);
         LP_data.A.row_ptr.push_back(LP_data.A.values.size());
      }
   }
   assert(LP_data.c.size() == ncols);
   assert(LP_data.b.size() == LP_data.A.row_ptr.size() - 1);

   LP_data.A.num_rows = LP_data.b.size();
   LP_data.A.num_cols = ncols;

   return LP_data;
}


/* Create the sub MILP refer to system (6)*/
static
SubmodelVars submodel_create(
        SCIP* scip,
        CSRMatrix A,
        vector<SCIP_Real> b,
        vector<SCIP_Real> c,
        int M,
        int k,
        SCIP_Real delta,
        SCIP_Real zl,
        SCIP_Real matrix_range,
        SCIP_Real time_limit
){
   // Create the submodel 
   size_t m = b.size();
   size_t n = c.size();
   SCIP *model_sub;
   SCIP_RETCODE retcode;

   retcode = SCIPcreate(&model_sub);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }

   retcode = SCIPincludeDefaultPlugins(model_sub);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }

   retcode = SCIPcreateProbBasic(model_sub, "sub");
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub);

      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }
   // Define vector variables
   vector<SCIP_VAR *> p(m);
   SCIP_VAR *s_L;
   vector<SCIP_VAR *> q(m);
   SCIP_VAR *s_R;
   vector<SCIP_VAR *> pi_plus(n);
   vector<SCIP_VAR *> pi_minus(n);
   SCIP_VAR *pi0;

   // Create variables
   for (size_t i = 0; i < m; ++i) {
      retcode = SCIPcreateVarBasic(model_sub, &p[i], ("p_" + to_string(i)).c_str(), 0.0, SCIPinfinity(model_sub), 0.0, SCIP_VARTYPE_CONTINUOUS);
      if (retcode != SCIP_OKAY) {
         SCIPprintError(retcode);
         SCIPfree(&model_sub);
         return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
      }
      SCIP_CALL_ABORT(SCIPaddVar(model_sub, p[i]));
   }

   retcode = SCIPcreateVarBasic(model_sub, &s_L, "s_L", 0.0, SCIPinfinity(model_sub), 0.0, SCIP_VARTYPE_CONTINUOUS);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }
   SCIP_CALL_ABORT(SCIPaddVar(model_sub, s_L));

   for (size_t i = 0; i < m; ++i) {
      retcode = SCIPcreateVarBasic(model_sub, &q[i], ("q_" + to_string(i)).c_str(), 0.0, SCIPinfinity(model_sub), 0.0, SCIP_VARTYPE_CONTINUOUS);
      if (retcode != SCIP_OKAY) {
         SCIPprintError(retcode);
         SCIPfree(&model_sub);

         return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
      }
      SCIP_CALL_ABORT(SCIPaddVar(model_sub, q[i]));
   }

   retcode = SCIPcreateVarBasic(model_sub, &s_R, "s_R", 0.0, SCIPinfinity(model_sub), 0.0, SCIP_VARTYPE_CONTINUOUS);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub);

      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }
   SCIP_CALL_ABORT(SCIPaddVar(model_sub, s_R));

   for (size_t j = 0; j < n; ++j) {
      retcode = SCIPcreateVarBasic(model_sub, &pi_plus[j], ("pi_plus_" + to_string(j)).c_str(), 0, M, 0.0, SCIP_VARTYPE_INTEGER);
      if (retcode != SCIP_OKAY) {
         SCIPprintError(retcode);
         SCIPfree(&model_sub);
         return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
      }
      SCIP_CALL_ABORT(SCIPaddVar(model_sub, pi_plus[j]));

      retcode = SCIPcreateVarBasic(model_sub, &pi_minus[j], ("pi_minus_" + to_string(j)).c_str(), 0, M, 0.0, SCIP_VARTYPE_INTEGER);
      if (retcode != SCIP_OKAY) {
         SCIPprintError(retcode);
         SCIPfree(&model_sub);
         return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
      }
      SCIP_CALL_ABORT(SCIPaddVar(model_sub, pi_minus[j]));
   }

   retcode = SCIPcreateVarBasic(model_sub, &pi0, "pi0", -SCIPinfinity(model_sub), SCIPinfinity(model_sub), 0.0, SCIP_VARTYPE_INTEGER);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }
   SCIP_CALL_ABORT(SCIPaddVar(model_sub, pi0));
   // Add constraints
   CSRMatrix At = A.transpose();
   for (size_t j = 0; j < n; ++j ){
      SCIP_CONS* cons;
      SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, ("cons_pA_" + to_string(j)).c_str(), 0, nullptr, nullptr, 0.0, 0.0));
      
      for (int i = At.row_ptr[j]; i < At.row_ptr[j + 1]; ++i) {
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, p[At.col_indices[i]], At.values[i]));
      }
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, s_L, -c[j]));
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_plus[j], -1.0));
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_minus[j], 1.0));
      SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons));
   }

   {
      SCIP_CONS* cons;
      SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, "cons_pb", 0, nullptr, nullptr, delta, SCIPinfinity(model_sub)));
      for (size_t i = 0; i < m; ++i) {
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, p[i], b[i]));
      }
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, s_L, -zl));
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi0, -1.0));
      SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons));
   }

   for (size_t j = 0; j < n; ++j){
      SCIP_CONS* cons;
      SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, ("cons_qA_" + to_string(j)).c_str(), 0, nullptr, nullptr, 0.0, 0.0));
      for (int i = At.row_ptr[j]; i < At.row_ptr[j + 1]; ++i) {
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, q[At.col_indices[i]], At.values[i]));
      }
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, s_R, -c[j]));
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_plus[j], 1.0));
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_minus[j], -1.0));
      SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons));
   }

   {
      SCIP_CONS* cons;
      SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, "cons_qb", 0, nullptr, nullptr, delta - 1, SCIPinfinity(model_sub)));
      for (size_t i = 0; i < m; ++i) {
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, q[i], b[i]));
      }
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, s_R, -zl));
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi0, 1.0));
      SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons));
   }

   {
      SCIP_CONS* cons;
      SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, "cons_pi_sum", 0, nullptr, nullptr, -SCIPinfinity(model_sub), k));
      for (size_t i = 0; i < n; ++i) {
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_plus[i], 1.0));
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_minus[i], 1.0));
      }
      SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons));
   }

   // Add constraint that if var is continuous then the corresponding pi[i] = 0
   SCIP_COL **cols = SCIPgetLPCols(scip);
   for (size_t j = 0; j < n; ++j) {
      SCIP_VAR* var = SCIPcolGetVar(cols[j]);
      if (SCIPvarGetType(var) == SCIP_VARTYPE_CONTINUOUS) {
         SCIP_CONS* cons;
         SCIP_CONS* cons1;
         SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, ("cons_pi_p_continuous_" + to_string(j)).c_str(), 0, nullptr, nullptr, 0.0, 0.0));
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_plus[j], 1.0));
         SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons));
         SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons));

         SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons1, ("cons_pi_m_continuous_" + to_string(j)).c_str(), 0, nullptr, nullptr, 0.0, 0.0));
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons1, pi_minus[j], 1.0));
         SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons1));
         SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons1));
      }
   }


   // Check if the LP relaxation of the original problem is optimal
   SCIP_LPSOLSTAT status_LP = SCIPgetLPSolstat(scip);
   if (status_LP == SCIP_LPSOLSTAT_OPTIMAL) {
      vector<SCIP_Real> x_star(n);
      SCIP_Real epsilon = 1e-3;
      SCIP_COL **lp_cols = SCIPgetLPCols(scip);
      assert (lp_cols != nullptr);

      for (size_t j = 0; j < n; ++j) {
         x_star[j] = SCIPgetSolVal(scip, nullptr, SCIPcolGetVar(lp_cols[j]));
      }

      // Add constraints pi0 <= sum((pi_plus[i] - pi_minus[i]) * x_star[i]) - epsilon
      {
         SCIP_CONS *cons;
         // Lower constraint: pi0 <= sum((pi_plus[i] - pi_minus[i]) * x_star[i]) - epsilon
         SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, "cons_pi0_lower", 0, nullptr, nullptr, epsilon, SCIPinfinity(model_sub)));
         for (size_t i = 0; i < n; ++i) {
            SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_plus[i], x_star[i]));
            SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_minus[i], -x_star[i]));
         }
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi0, -1.0));
         SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons));
         SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons));
      }

      // Add constraints pi0 >= sum((pi_plus[i] - pi_minus[i]) * x_star[i]) + epsilon - 1
      {
         SCIP_CONS *cons;
         SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, "cons_pi0_upper", 0, nullptr, nullptr, -SCIPinfinity(model_sub), 1 - epsilon));
         for (size_t i = 0; i < n; ++i) {
            SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_plus[i], x_star[i]));
            SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_minus[i], -x_star[i]));
         }
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi0, -1.0 ));
         SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons));
         SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons));
      }
   }

   retcode = SCIPreadParams(model_sub, "/scratch/htc/yzhou/exp_scipmip/settings/default1.set");
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }


   
   retcode = SCIPsetRealParam(model_sub, "limits/time", time_limit);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }
   if (matrix_range < 1e-9){
      matrix_range = 1e-7;
      SCIPsetRealParam(model_sub, "numerics/feastol", matrix_range);
      SCIPsetRealParam(model_sub, "numerics/sumepsilon", matrix_range);
   }
   
   SCIPsetMessagehdlrQuiet(model_sub, TRUE);
   return SubmodelVars{model_sub, p, s_L, q, s_R, pi_plus, pi_minus, pi0};
}

/* Main function for solving sub-model*/
static
vector<Submodel_sols> submodel_solve(
        SCIP* scip,
        SCIP_Real zl_low,
        SCIP_Real zl_high,
        int m,
        int n,
        SCIP_Real delta,
        CSRMatrix A,
        vector<SCIP_Real> b,
        vector<SCIP_Real> c,
        int M,
        int k,
        SCIP_Real node_ub,
        vector<SCIP_VAR*> lp_vars,
        SCIP_Real matrix_range,
        SCIP_Real time_limit
){
   // Search disjunction time tracking
   auto start_time = std::chrono::high_resolution_clock::now();

   queue<SCIP_Real> estL_list;
   queue<SCIP_Real> estR_list;
   queue<SCIP_Real> feasible_zl;
   queue<vector<int>> best_pi_solutions;
   queue<int> best_pi0_solutions;
   queue<string> Status_l;
   queue<string> Status_r;
   vector<Submodel_sols> final_results;


   // Simple check on system (4)
   SubmodelVars preck_submodel = submodelsmall_create(scip, A, b, c, M, k, delta, zl_low, matrix_range, time_limit);
   SCIP_RETCODE retcode = SCIPsolve(preck_submodel.model_sub);
   g_total_milps++;
   SCIP_Longint fairnodes_preck = 0;
   // Retrieve the number of fair nodes solved in precheck submodel
   if (retcode != SCIP_OKAY) {
      std::cerr << "Error solving preck submodel !" << std::endl;
   } else{
      SCIP_BRANCHRULE** branchrules = SCIPgetBranchrules(preck_submodel.model_sub);
      int nbranchrules = SCIPgetNBranchrules(preck_submodel.model_sub);
      SCIP_Longint ConssAdded = 0;
      SCIP_Longint DomReds = 0;
      SCIP_Longint Cutoffs = 0;
      SCIP_Longint Nodes_smallmodel = SCIPgetNTotalNodes(preck_submodel.model_sub);
      for (int i = 0; i < nbranchrules; ++i) {
         ConssAdded += SCIPbranchruleGetNConssFound(branchrules[i]);
         DomReds += SCIPbranchruleGetNDomredsFound(branchrules[i]);
         Cutoffs += SCIPbranchruleGetNCutoffs(branchrules[i]);
      }
      fairnodes_preck += ConssAdded + DomReds + Cutoffs + Nodes_smallmodel;
   }
   g_total_milp_nodes += fairnodes_preck;
   if (SCIPgetStatus(preck_submodel.model_sub) == SCIP_STATUS_OPTIMAL) {
      cout << "Preck submodel is solved to optimality" << endl;
      cout << "No branching possible in this node" << endl;
      SCIPreleaseVar(preck_submodel.model_sub, &preck_submodel.pi0);
      for (int i = 0; i < m; ++i) {
         SCIPreleaseVar(preck_submodel.model_sub, &preck_submodel.p[i]);
         SCIPreleaseVar(preck_submodel.model_sub, &preck_submodel.q[i]);
      }
      for (int i = 0; i < n; ++i) {
         SCIPreleaseVar(preck_submodel.model_sub, &preck_submodel.pi_plus[i]);
         SCIPreleaseVar(preck_submodel.model_sub, &preck_submodel.pi_minus[i]);
      }
      SCIPfree(&preck_submodel.model_sub);
      Submodel_sols result = {zl_low, {}, {}, 1e+20, 1e+20, "infeasible", "infeasible"};
      final_results.push_back(result);
      return final_results;
   } 
   else {
      cout << "Preck submodel is infeasible, continue branching" << endl;
      SCIPreleaseVar(preck_submodel.model_sub, &preck_submodel.pi0);
      for (int i = 0; i < m; ++i) {
         SCIPreleaseVar(preck_submodel.model_sub, &preck_submodel.p[i]);
         SCIPreleaseVar(preck_submodel.model_sub, &preck_submodel.q[i]);
      }
      for (int i = 0; i < n; ++i) {
         SCIPreleaseVar(preck_submodel.model_sub, &preck_submodel.pi_plus[i]);
         SCIPreleaseVar(preck_submodel.model_sub, &preck_submodel.pi_minus[i]);
      }
      SCIPfree(&preck_submodel.model_sub);
   }

   // Start the binary search
   while (abs(zl_high - zl_low) > 1e-4){
      // Check time limit at the beginning of each iteration
      auto current_time = std::chrono::high_resolution_clock::now();
      auto elapsed_seconds = std::chrono::duration<double>(current_time - start_time).count();

      if (elapsed_seconds > time_limit) {
         cout << "Time limit exceeded (" << elapsed_seconds << "s), stopping submodel solve loop" << endl;
         
         // Return the best result found so far, or a default result if none found
         if (!feasible_zl.empty()) {
            SCIP_Real best_zl = feasible_zl.back();
            vector<int> best_pi_solution = best_pi_solutions.back();
            int best_pi0_solution = best_pi0_solutions.back();
            string status_l = Status_l.back();
            string status_r = Status_r.back();
            SCIP_Real est_l = estL_list.back();
            SCIP_Real est_r = estR_list.back();
            Submodel_sols result = {best_zl, best_pi_solution, best_pi0_solution, est_l, est_r, status_l, status_r};
            final_results.push_back(result);
         } else {
            // No feasible solution found within time limit
            Submodel_sols result = {SCIP_INVALID, {}, {}, 1e+20, 1e+20, "time_limit", "time_limit"};
            final_results.push_back(result);
         }
         return final_results;
      }

      SCIP_Real zl = (zl_high + zl_low) / 2;
      SubmodelVars submodel_datas = submodel_create(scip, A, b, c, M, k, delta, zl, matrix_range, time_limit);
      SCIP_RETCODE retcode1 = SCIPsolve(submodel_datas.model_sub);
      g_total_milps++;
      
      // Check if the submodel was solved to optimality
      if (retcode1 != SCIP_OKAY) {
         std::cerr << "Error solving submodel with curr zl: " << zl << std::endl;
         zl_high = zl;
         continue;
      }

      //Retrieve the number of fair nodes solved in submodel
      SCIP_BRANCHRULE** branchrules = SCIPgetBranchrules(submodel_datas.model_sub);
      int nbranchrules = SCIPgetNBranchrules(submodel_datas.model_sub);
      int nodes_submodel = SCIPgetNTotalNodes(submodel_datas.model_sub);
      SCIP_Longint ConssAdded = 0;
      SCIP_Longint DomReds = 0;
      SCIP_Longint Cutoffs = 0;
      for (int i = 0; i < nbranchrules; ++i) {
         ConssAdded += SCIPbranchruleGetNConssFound(branchrules[i]);
         DomReds += SCIPbranchruleGetNDomredsFound(branchrules[i]);
         Cutoffs += SCIPbranchruleGetNCutoffs(branchrules[i]);
      }
      
      SCIP_Longint fairnodes_submodel = ConssAdded + DomReds + Cutoffs + nodes_submodel;
      g_total_milp_nodes += fairnodes_submodel;

      // Starting probing if the submodel is solved to optimality
      if (SCIPgetStatus(submodel_datas.model_sub) == SCIP_STATUS_OPTIMAL) {
         
         SCIP_Bool startprobing = TRUE;
         SCIP_Bool endprobing = FALSE;

         // Retrieve the solutions
         // cout << "Submodel solved with current zl: " << zl << endl; 
         SCIP_Sol *submodel_sol = SCIPgetBestSol(submodel_datas.model_sub);
         vector<SCIP_Real> pi_plus_solution(n);
         vector<SCIP_Real> pi_minus_solution(n);
         vector<int> pi_solution(n);
         vector<SCIP_Real> p_solution(m); 
         vector<SCIP_Real> q_solution(m);
         int pi0_solution; 
         for (int i = 0; i < m; ++i) {
            p_solution[i] = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.p[i]);
            q_solution[i] = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.q[i]);
         }
         SCIP_Real s_L_solution = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.s_L);
         SCIP_Real s_R_solution = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.s_R);
         pi0_solution = round(SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.pi0));

         for (int i = 0; i < n; ++i) {
            pi_plus_solution[i] = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.pi_plus[i]);
            pi_minus_solution[i] = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.pi_minus[i]);
            pi_solution[i] = round(pi_plus_solution[i] - pi_minus_solution[i]);
         }
         assert(SCIPisFeasIntegral(submodel_datas.model_sub, pi0_solution));
         for (SCIP_Real i : pi_solution) {
            assert(SCIPisFeasIntegral(submodel_datas.model_sub, i));
         }

         SCIP_VAR** leftvars = NULL;
         SCIP_VAR** rightvars = NULL;
         SCIP_Real* leftvarssols = NULL;
         SCIP_Real* rightvarssols = NULL;
         int nleftvars;
         int nrightvars;
         SCIP_Bool lperror = FALSE;
         SCIP_Bool leftinf;
         SCIP_Bool rightinf;
         SCIP_LPSOLSTAT solstatleft;
         SCIP_LPSOLSTAT solstatright;
         SCIP_Real leftobjval;
         SCIP_Real rightobjval;
         SCIP_Real estimate_left;
         SCIP_Real estimate_right;
         pair<string, SCIP_Real> result_l;
         pair<string, SCIP_Real> result_r;
         SCIP_CONS *probing_cons_left;
         SCIP_CONS *probing_cons_right;
         
         // Check if pi_solution is not all zero and if both s_L and s_R are zero
         if (notallzero(pi_solution)) {
            if (!SCIPisFeasZero(scip, s_L_solution) || !SCIPisFeasZero(scip, s_R_solution)) {
               // cout << "One of the s_L and s_R are non-zero, DEBUG" << endl;
            } else {
               // cout<< "Both s_L and s_R are zero, DEBUG" << endl;
               // Write out the current submodel problem to a file for debugging
               // std::ostringstream submodel_fname;
               // submodel_fname << "/scratch/htc/yzhou/exp_scipmip/instances/submodel_debug_zl_" << std::setprecision(8) << zl << ".lp";
               // SCIP_CALL_ABORT(SCIPwriteOrigProblem(submodel_datas.model_sub, submodel_fname.str().c_str(), "lp", FALSE));
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_L);
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_R);
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi0);
               for (int i = 0; i < m; ++i) {
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.p[i]);
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.q[i]);
               }
               for (int i = 0; i < n; ++i) {
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_minus[i]);
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_plus[i]);
               }
               SCIPfree(&submodel_datas.model_sub);
               zl_high = zl;
               continue;
            }
            if (startprobing) {
               // If the solution is feasible, we can start probing
               SCIP_CALL_ABORT(SCIPstartProbing(scip));
               startprobing = FALSE;
               endprobing = TRUE;

            }
            
            // Handle the right side probing constraint
            SCIPcreateConsLinear(scip, &probing_cons_left, "probing_cons_left" , 0, NULL, NULL, -SCIPinfinity(scip), pi0_solution,
                  TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE);
            for (int i = 0; i < n; ++i) {
               if( SCIPisFeasZero(scip, pi_solution[i]) ) {
                  continue;
               }
               SCIPaddCoefLinear(scip, probing_cons_left, lp_vars[i] , pi_solution[i]);
            }
            SCIP_CALL_ABORT(SCIPnewProbingNode(scip));
            SCIP_Node* prob_node = SCIPgetCurrentNode(scip);
            assert(prob_node != NULL);
            SCIP_CALL_ABORT(SCIPaddConsNode(scip, prob_node, probing_cons_left, NULL));
            SCIP_CALL_ABORT(SCIPreleaseCons(scip, &probing_cons_left));
            SCIP_CALL_ABORT(SCIPsolveProbingLP(scip, -1, &lperror, &leftinf));
            solstatleft = SCIPgetLPSolstat(scip);
            lperror = lperror || (solstatleft == SCIP_LPSOLSTAT_NOTSOLVED) || (solstatleft == SCIP_LPSOLSTAT_ITERLIMIT) ||
                     (solstatleft == SCIP_LPSOLSTAT_TIMELIMIT);
            assert(solstatleft != SCIP_LPSOLSTAT_UNBOUNDEDRAY);
            if ( lperror ) {
               cout << "Error in probing left side: " << solstatleft << endl;
               result_l = {"infeasible", 1e+20};
               leftobjval = 1e+20;
            } else{
               leftobjval = SCIPgetLPObjval(scip);
            }

            leftinf = leftinf || (SCIPisGE(scip, leftobjval, node_ub));
            assert (((solstatleft != SCIP_LPSOLSTAT_INFEASIBLE) && (solstatleft != SCIP_LPSOLSTAT_OBJLIMIT)) || leftinf);
            if ( !leftinf ) {
               estimate_left = SCIPnodeGetLowerbound(prob_node);
               SCIP_CALL_ABORT(SCIPgetLPBranchCands(scip, &leftvars, &leftvarssols, NULL, &nleftvars, NULL, NULL));
               for (int j = 0; j < nleftvars; ++j) {

                  SCIP_Real estimateincr;
                  SCIP_Real pscdown;
                  SCIP_Real pscup;

                  assert(leftvars != NULL);
                  assert(leftvars[j] != NULL);

                  pscdown = SCIPgetVarPseudocostVal(scip, leftvars[j], SCIPfeasFloor(scip, leftvarssols[j]) - leftvarssols[j]);
                  pscup = SCIPgetVarPseudocostVal(scip, leftvars[j], SCIPfeasCeil(scip, leftvarssols[j]) - leftvarssols[j]);
                  estimateincr = MIN(pscdown, pscup);
                  estimate_left += estimateincr;
               }
               if (leftobjval - zl > 1e-06) {
                  result_l = {"updated_zl", estimate_left};
               } else{
                  result_l = {"obj_val less than zl", estimate_left};
               }
            } else {
               result_l = {"infeasible", 1e+20};
            }
            g_total_probing_lps++;  // Count probing LP solves
            SCIP_CALL_ABORT( SCIPbacktrackProbing(scip, 0) );

            // Handle the right side probing constraint
            SCIPcreateConsLinear(scip, &probing_cons_right, "probing_cons_right", 0, NULL, NULL, pi0_solution + 1, SCIPinfinity(scip),
                  TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE);
            for (int i = 0; i < n; ++i) {
               if( SCIPisFeasZero(scip, pi_solution[i]) ) {
                  continue;
               }
               SCIPaddCoefLinear(scip, probing_cons_right, lp_vars[i], pi_solution[i]);
            }
            SCIP_CALL_ABORT(SCIPnewProbingNode(scip));
            SCIP_Node* prob_node_r = SCIPgetCurrentNode(scip);
            assert(prob_node_r != NULL);
            SCIP_CALL_ABORT(SCIPaddConsNode(scip, prob_node_r, probing_cons_right, NULL));
            SCIP_CALL_ABORT(SCIPreleaseCons(scip, &probing_cons_right));
            SCIP_RETCODE retcode1 = SCIPsolveProbingLP(scip, -1, &lperror, &rightinf);
            if (retcode1 != SCIP_OKAY) {
               std::cerr << "Error solving probing LP with curr zl: " << zl << endl;
            }
            solstatright = SCIPgetLPSolstat(scip);
            lperror = lperror || (solstatright == SCIP_LPSOLSTAT_NOTSOLVED) || (solstatright == SCIP_LPSOLSTAT_ITERLIMIT) ||
                     (solstatright== SCIP_LPSOLSTAT_TIMELIMIT);
            assert(solstatright != SCIP_LPSOLSTAT_UNBOUNDEDRAY);
            if ( lperror ) {
               cout << "Error in probing right side: " << static_cast<int>(solstatright) << endl;
               result_r = {"infeasible", 1e+20};
               rightobjval = 1e+20;
            } else{
               rightobjval = SCIPgetLPObjval(scip);
            }

            rightinf = rightinf || (SCIPisGE(scip, rightobjval, node_ub));
            assert (((solstatright != SCIP_LPSOLSTAT_INFEASIBLE) && (solstatright != SCIP_LPSOLSTAT_OBJLIMIT)) || rightinf);
            if ( !rightinf ) {
               estimate_right = SCIPnodeGetLowerbound(prob_node_r);
               SCIP_CALL_ABORT(SCIPgetLPBranchCands(scip, &rightvars, &rightvarssols, NULL, &nrightvars, NULL, NULL));
               for (int j = 0; j < nrightvars; ++j) {

                  SCIP_Real estimateincr;
                  SCIP_Real pscdown;
                  SCIP_Real pscup;

                  assert(rightvars != NULL);
                  assert(rightvars[j] != NULL);

                  pscdown = SCIPgetVarPseudocostVal(scip, rightvars[j], SCIPfeasFloor(scip, rightvarssols[j]) - rightvarssols[j]);
                  pscup = SCIPgetVarPseudocostVal(scip, rightvars[j], SCIPfeasCeil(scip, rightvarssols[j]) - rightvarssols[j]);
                  estimateincr = MIN(pscdown, pscup);
                  estimate_right += estimateincr;
               }
               if (rightobjval - zl > 1e-06) {
                  result_r = {"updated_zl", estimate_right};
               } else{
                  result_r = {"obj_val less than zl", estimate_right};
               }
            } else {
               result_r = {"infeasible", 1e+20};
            }
            g_total_probing_lps++;  // Count probing LP solves
            if( endprobing )
            {
               SCIP_CALL_ABORT( SCIPendProbing(scip) );
            }
            if (result_l.first == "updated_zl" || result_r.first == "updated_zl") {
               feasible_zl.push(zl);
               best_pi_solutions.push(pi_solution);
               best_pi0_solutions.push(pi0_solution);
               Status_l.push(result_l.first);
               Status_r.push(result_r.first);
               estL_list.push(result_l.second);
               estR_list.push(result_r.second);
               zl_low = zl;
               assert (feasible_zl.size() == estL_list.size());
               if (feasible_zl.size() > 1) {
                  feasible_zl.pop();
                  best_pi_solutions.pop();
                  best_pi0_solutions.pop();
                  Status_l.pop();
                  Status_r.pop();
                  estL_list.pop();
                  estR_list.pop();
               }
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_L);
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_R);
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi0);
               for (int i = 0; i < m; ++i) {
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.p[i]);
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.q[i]);
               }
               for (int i = 0; i < n; ++i) {
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_minus[i]);
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_plus[i]);
               }
               SCIPfree(&submodel_datas.model_sub);
               continue;
            }
            else if (result_l.first == "infeasible" && result_r.first == "infeasible") {
               
                  feasible_zl.push(zl);
                  best_pi_solutions.push(pi_solution);
                  best_pi0_solutions.push(pi0_solution);
                  Status_l.push(result_l.first);
                  Status_r.push(result_r.first);
                  estL_list.push(result_l.second);
                  estR_list.push(result_r.second);

               if (feasible_zl.size() > 1) {

                  feasible_zl.pop();
                  best_pi_solutions.pop();
                  best_pi0_solutions.pop();
                  Status_l.pop();
                  Status_r.pop();
                  estL_list.pop();
                  estR_list.pop();
               } 
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_L);
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_R);
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi0);
               for (int i = 0; i < m; ++i) {
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.p[i]);
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.q[i]);
               }
               for (int i = 0; i < n; ++i) {
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_minus[i]);
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_plus[i]);
               }
               SCIPfree(&submodel_datas.model_sub);
               zl_high = zl;
               continue;
            } 
            else if (result_l.first == "obj_val less than Best_zl" && result_r.first == "obj_val less than Best_zl") {
               cout << "Both sides have objective value less than Best_zl, DEBUG" << endl;
 
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_L);
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_R);
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi0);
               for (int i = 0; i < m; ++i) {
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.p[i]);
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.q[i]);
               }
               for (int i = 0; i < n; ++i) {
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_minus[i]);
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_plus[i]);
               }
               SCIPfree(&submodel_datas.model_sub);
               zl_high = zl;
               continue;
            }
            else {
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_L);
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_R);
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi0);
               for (int i = 0; i < m; ++i) {
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.p[i]);
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.q[i]);
               }
               for (int i = 0; i < n; ++i) {
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_minus[i]);
                  SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_plus[i]);
               }
               SCIPfree(&submodel_datas.model_sub);
               zl_high = zl;
               continue;
            }
         } else {
            SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_L);
            SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_R);
            SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi0);
            for (int i = 0; i < m; ++i) {
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.p[i]);
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.q[i]);
            }
            for (int i = 0; i < n; ++i) {
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_minus[i]);
               SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_plus[i]);
            }
            SCIPfree(&submodel_datas.model_sub);
            zl_high = zl;
            continue;
         }
      }
      else{
         SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_L);
         SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_R);
         SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi0);
         for (int i = 0; i < m; ++i) {
            SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.p[i]);
            SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.q[i]);
         }
         for (int i = 0; i < n; ++i) {
            SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_minus[i]);
            SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_plus[i]);
         }
         SCIPfree(&submodel_datas.model_sub);
         zl_high = zl;
         continue;
      }
   }
   assert (feasible_zl.size() == best_pi_solutions.size());
   assert (feasible_zl.size() == best_pi0_solutions.size());

   Submodel_sols result;

   if (feasible_zl.empty()){
      result = {SCIP_INVALID, {}, {}, 1e+20, 1e+20, "NULL", "NULL"};
   }
   else {
      SCIP_Real best_zl = feasible_zl.back();
      vector<int> best_pi_solution = best_pi_solutions.back();
      int best_pi0_solution = best_pi0_solutions.back();
      string status_l = Status_l.back();
      string status_r = Status_r.back();
      SCIP_Real est_l = estL_list.back();
      SCIP_Real est_r = estR_list.back();
      result = {best_zl, best_pi_solution, best_pi0_solution, est_l, est_r, status_l, status_r};

   }
   final_results.push_back(result);
   return final_results;
};

/* Print information about the current branching node */
static
SCIP_NODE* get_information(SCIP* scip) {
   std::cout << "_____________________________________" << std::endl;
   std::cout << "Now starting branching with general disjunction" << std::endl;
   SCIP_NODE* curr_Node = SCIPgetCurrentNode(scip);
   std::cout << "Current branching Node number: " << SCIPnodeGetNumber(curr_Node) << std::endl;

   return curr_Node;
}

/* Create Constraint and Branching Children*/
static
SCIP_RETCODE createBranchingConstraint(
        SCIP* scip,
        SCIP_Bool createChild,
        SCIP_NODE* curr_Node,
        const vector<SCIP_VAR*>& vars_lp,
        const vector<int>& pi_solution,
        SCIP_Real pi0_solution,
        SCIP_Real est,
        const string& side
)
{
   if (side == "left") {
      SCIP_CONS *cons_l;
      SCIP_CALL(SCIPcreateConsLinear(scip, &cons_l, (side + std::to_string(SCIPnodeGetNumber(curr_Node))).c_str(), 0, nullptr, nullptr, -SCIPinfinity(scip), pi0_solution, 
         TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE));
      for (size_t i = 0; i < vars_lp.size(); ++i) {
         if( SCIPisFeasZero(scip, pi_solution[i]) ) {
            continue;
         }
         SCIP_CALL(SCIPaddCoefLinear(scip, cons_l, vars_lp[i], pi_solution[i]));
      }
      if (createChild == TRUE) {
         SCIP_NODE *child_node;
         SCIP_CALL(SCIPcreateChild(scip, &child_node, 1.0, est));
         SCIP_CALL(SCIPaddConsNode(scip, child_node, cons_l, nullptr));
         SCIP_CALL(SCIPreleaseCons(scip, &cons_l));
      }
      else {
         SCIP_CALL(SCIPaddConsNode(scip, curr_Node, cons_l, nullptr));
         SCIP_CALL(SCIPreleaseCons(scip, &cons_l));
      }
   }
   else if (side == "right") {
      SCIP_CONS *cons_r;
      SCIP_CALL(SCIPcreateConsLinear(scip, &cons_r, (side + std::to_string(SCIPnodeGetNumber(curr_Node))).c_str(), 0, nullptr, nullptr, pi0_solution, SCIPinfinity(scip), 
         TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE));
      for (int i = 0; i < pi_solution.size(); ++i) {
         if( SCIPisFeasZero(scip, pi_solution[i]) ) {
            continue;
         }
         SCIP_CALL(SCIPaddCoefLinear(scip, cons_r, vars_lp[i], pi_solution[i]));
      }
      if (createChild == TRUE){
         SCIP_NODE* child_node;
         SCIP_CALL(SCIPcreateChild(scip, &child_node, 1.0, est));
         SCIP_CALL(SCIPaddConsNode(scip, child_node, cons_r, nullptr));
         SCIP_CALL(SCIPreleaseCons(scip, &cons_r));
      }
      else{
         SCIP_CALL(SCIPaddConsNode(scip, curr_Node, cons_r, NULL));
         SCIP_CALL(SCIPreleaseCons(scip, &cons_r));
      }
   }
   else {
      cout << "Invalid side" << endl;
   }
   return SCIP_OKAY;
};

// /* Method for get factor for scaling zl */
// static
// SCIP_Real get_factor(SCIP_Real lp_gap) {
//    SCIP_Real factor;
//    assert (lp_gap >= 0);
//    if (lp_gap < 0.1) {
//       factor = 1 + ceil(lp_gap*100)/100;
//    } else if (lp_gap >=0.1 && lp_gap < 1) {
//       factor = 1 + ceil(lp_gap*10)/10;
//    } else if (lp_gap == 1e+20) {
//       factor = 2;
//    } else {
//       factor = (ceil(lp_gap) + 1) * 2;
//    }
//    return factor;
// }

/* Helper function to analyze coefficients and output only
 matrix range to maintain the numerical stability */
static
pair<SCIP_Real, SCIP_Real> analyzeMatrixRange(
   const CSRMatrix& A,
   const vector<SCIP_Real>& b,
   const vector<SCIP_Real>& c,
   SCIP_Real zl,
   const vector<SCIP_VAR*>& vars_lp,
   SCIP_Real base_delta,
   SCIP* scip
) {
   // Find min and max coefficient values across all data
   SCIP_Real min_coef = SCIP_REAL_MAX;
   SCIP_Real max_coef = 0.0;
   // Analyze constraint matrix A
   for (size_t i = 0; i < A.values.size(); ++i) {
      if (std::abs(A.values[i]) > 1e-9) {
         SCIP_Real abs_val = std::abs(A.values[i]);
         min_coef = std::min(min_coef, abs_val);
         max_coef = std::max(max_coef, abs_val);
      }
   }
    
   // Analyze RHS vector b
   for (size_t i = 0; i < b.size(); ++i) {
      if (std::abs(b[i]) > 1e-9) {
         SCIP_Real abs_val = std::abs(b[i]);
         min_coef = std::min(min_coef, abs_val);
         max_coef = std::max(max_coef, abs_val);
      }
   }
    
   // Analyze objective coefficients c
   for (size_t i = 0; i < c.size(); ++i) {
      if (std::abs(c[i]) > 1e-9) {
         SCIP_Real abs_val = std::abs(c[i]);
         min_coef = std::min(min_coef, abs_val);
         max_coef = std::max(max_coef, abs_val);
      }
   }
   
   // Analyze dual bound zl
   if (!SCIPisInfinity(scip, std::abs(zl))) {
      SCIP_Real abs_val = std::abs(zl);
      if (abs_val > 1e-12) {
         min_coef = std::min(min_coef, abs_val);
         max_coef = std::max(max_coef, abs_val);
      }
   }
   
   // Analyze current LP solution x*
   for (size_t i = 0; i < vars_lp.size(); ++i) {
      SCIP_Real sol_val = SCIPgetSolVal(scip, nullptr, vars_lp[i]);
      if (std::abs(sol_val) > 1e-9) {
         SCIP_Real abs_val = std::abs(sol_val);
         min_coef = std::min(min_coef, abs_val);
         max_coef = std::max(max_coef, abs_val);
      }
   }
   SCIP_Real scaled_delta = base_delta;
   SCIP_Real range = 1e-6;
   
   // Calculate matrix range using getMagnitudeBase on min and max coefficients
   if (max_coef != SCIP_REAL_MAX && min_coef - 1e-9 > 0) {
      SCIP_Real min_magnitude = getMagnitudeBase(min_coef);
      SCIP_Real max_magnitude = getMagnitudeBase(std::round(max_coef));
      SCIP_Real matrix_range = (min_magnitude - 1e-9 > 0) ? max_magnitude / min_magnitude : max_magnitude;      
      
      if (matrix_range > 1e+8) {

         if (max_magnitude / 1e+5 > base_delta) {
            scaled_delta = base_delta;
         } else {
            scaled_delta = max_magnitude / 1e+5;
         }
      }     
      else if (matrix_range > 1e+5 && matrix_range <= 1e+8) {
         // For matrix range between 1e+5 and 1e+7, keep default delta
         scaled_delta = base_delta;
      }
      else if (matrix_range >= 1e+4) {
         // Scale delta proportionally to matrix range
         SCIP_Real scale_factor = matrix_range / 1e+4;  // Between 1 and 100
         scaled_delta = base_delta / std::sqrt(scale_factor);
         
         // Ensure minimum delta for numerical stability
         scaled_delta = std::max(scaled_delta, 1e-6);
      }
      else if (matrix_range >= 1e+2) {
         // Use base_delta or slightly larger
         scaled_delta = base_delta;
      }
      else if (matrix_range >= 1e-8) {
         // For small matrix ranges including 1e-6 and 1e-7 (your successful cases)
         // Keep the base_delta as it works well for these instances
         scaled_delta = base_delta;
      }
      else {
         // For very small matrix ranges, use larger delta for faster convergence
         scaled_delta = base_delta * 2.0;
         
         // Cap the maximum delta
         scaled_delta = std::min(scaled_delta, 1.0);
      }
      // Additional safety check based on coefficient magnitude
      if (max_coef >= 1e+2 && max_coef <= 1e+4) {
         // For your specific case: max_coef in range 1e+2 to 1e+4
         SCIP_Real magnitude_based_delta = max_coef / 1e+5;
         scaled_delta = std::min(scaled_delta, magnitude_based_delta);
      }
      range = getMagnitudeBase(1 / matrix_range);
   }

   return {scaled_delta, range};
}
/*
 * Callback methods of branching rule
 */

/* branching execution method for fractional LP solutions */
SCIP_DECL_BRANCHEXECLP(BranchruleGeneralDisjunction::scip_execlp){
   {
      SCIP_Node *curr_Node = get_information(scip);
      MatrixData LP_data = getConstraintMatrix(scip);

      // Get LP branching candidates
      SCIP_VAR** lpcands;
      SCIP_Real* lpcandssol;
      SCIP_Real* lpcandsfrac;
      int nlpcands;
      SCIP_CALL(SCIPgetLPBranchCands(scip, &lpcands, &lpcandssol, &lpcandsfrac, NULL, &nlpcands, NULL));
      
      if (nlpcands == 0) {
         *result = SCIP_DIDNOTRUN;
         return SCIP_OKAY;
      }
      
      // First consult relpscost (evaluation only, no branching)
      SCIP_RESULT relpscost_result = consultRelpscost(scip, lpcands, lpcandssol, lpcandsfrac, nlpcands, FALSE);
      if (relpscost_result == SCIP_REDUCEDDOM) {
         *result = SCIP_REDUCEDDOM;
         std::cout << "Relpscost reduced domain, skip general disjunction branching" << std::endl;
         return SCIP_OKAY;
      } else if (relpscost_result == SCIP_CONSADDED) {
         *result = SCIP_CONSADDED;
         std::cout << "Relpscost added constraint, skip general disjunction branching" << std::endl;
         return SCIP_OKAY;
      }

      CSRMatrix A = LP_data.A;
      std::vector<SCIP_Real> b = LP_data.b;
      std::vector<SCIP_Real> c = LP_data.c;
      size_t m = b.size();
      size_t n = c.size();
      SCIP_LPSOLSTAT lpstat = SCIPgetLPSolstat(scip);
      if (lpstat != SCIP_LPSOLSTAT_OPTIMAL) {
         std::cout << "LP not solved optimally (status: " << static_cast<int>(lpstat) << "), skipping general disjunction branching" << std::endl;
         *result = SCIP_DIDNOTRUN;
         return SCIP_OKAY;
      }

      SCIP_Real LP_obj = SCIPgetLPObjval(scip);
      SCIP_Real node_lowerbound = SCIPgetNodeLowerbound(scip, curr_Node);
      SCIP_Real node_ub = SCIPgetPrimalbound(scip);

      cout << "LP objective value: " << LP_obj << endl;
      cout << "Node lower bound: " << node_lowerbound << endl;
      cout << "Node upper bound: " << node_ub << endl;

      SCIP_Real lp_gap = SCIPgetGap(scip);
      cout << "gap to the primal bound: " << lp_gap << endl;
      SCIP_COL** cols_lp = SCIPgetLPCols(scip);
      vector<SCIP_VAR*> vars_lp(c.size());
      for (size_t i = 0; i < c.size(); ++i) {
         vars_lp[i] = SCIPcolGetVar(cols_lp[i]);
      }

      SCIP_Real zl_init = node_lowerbound;
      SCIP_Real zl_low = zl_init;
      SCIP_Real zl_high;
      SCIP_Real factor = 2.5;
      if (zl_init > 0) {
         zl_high = zl_init * factor;
      } else if (zl_init < 0) {
         zl_high = zl_init / factor;
      } else {
         zl_high = 2;
      }
      // Analyze matrix range (only output)
      pair<SCIP_Real, SCIP_Real> numerics_pair = analyzeMatrixRange(A, b, c, (zl_low + zl_high) * 0.5, vars_lp, base_delta, scip);
      SCIP_Real scaled_delta = numerics_pair.first;
      SCIP_Real matrix_range = numerics_pair.second;
      // cout << "Matrix range: " << matrix_range << ", scaled delta: " << scaled_delta << endl;
      std::vector<Submodel_sols> final_results = submodel_solve(scip, zl_low, zl_high, m, n, scaled_delta, A, b, c, M, k, node_ub, vars_lp, matrix_range, TIME_LIMIT_SECONDS);
      SCIP_Real est_l = final_results[0].est_l;
      SCIP_Real est_r = final_results[0].est_r;
      string status_l = final_results[0].status_l;
      string status_r = final_results[0].status_r;

      if ( status_l == "NULL" || status_r == "NULL") {
         std::cout << "General disjunction: No feasible solution found, use SCIP default branching rule" << std::endl;
         *result = SCIP_DIDNOTFIND;
         return SCIP_OKAY;

      } else if (status_l == "updated_zl" && status_r == "updated_zl") {

         SCIP_Bool CreateChild = TRUE;
         int pi0_sol_plus = final_results[0].pi0_solution + 1; 
         SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, final_results[0].pi0_solution, est_l, "left"));
         SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, pi0_sol_plus, est_r, "right"));
         std::cout << "General disjunction: Both Children are added" << std::endl;
         *result = SCIP_BRANCHED;
         return SCIP_OKAY;

      }else if (status_l == "infeasible" && status_r != "updated_zl") {
         std::cout << "General disjunction: Infeasible disjunction" << std::endl;
         *result = SCIP_CUTOFF;
         return SCIP_OKAY;

      } else if (status_l != "updated_zl" && status_r == "infeasible") {
         std::cout << "General disjunction: Infeasible disjunction" << std::endl;
         *result = SCIP_CUTOFF;
         return SCIP_OKAY;

      } else if (status_l == "updated_zl" && status_r != "updated_zl") {

         if (status_r == "infeasible" || status_r == "ckmodel infeasible") {

            SCIP_Bool CreateChild = FALSE;
            SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, final_results[0].pi0_solution, est_l, "left"));
            std::cout << "General disjunction: Only Left constraint added:" << std::endl;
            *result = SCIP_CONSADDED;
         }
         else {
            int pi0_sol_plus = final_results[0].pi0_solution + 1;
            SCIP_Bool CreateChild = TRUE;
            SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, final_results[0].pi0_solution, est_l, "left"));
            SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, pi0_sol_plus, est_r, "right"));
            std::cout << "General disjunction: Both Children are added" << std::endl;
            *result = SCIP_BRANCHED;
         }
         return SCIP_OKAY;

      } else if (status_r == "updated_zl" && status_l != "updated_zl") {

         if (status_l == "infeasible" || status_l == "ckmodel infeasible") {
            SCIP_Bool CreateChild = FALSE;
            int pi0_sol_plus = final_results[0].pi0_solution + 1;
            SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, pi0_sol_plus, est_r, "right"));
            std::cout << "General disjunction: Only Right constraint added:" << std::endl;
            *result = SCIP_CONSADDED;
         }
         else {
            SCIP_Bool CreateChild = TRUE;
            int pi0_sol_plus = final_results[0].pi0_solution + 1;
            SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, pi0_sol_plus, est_r, "right"));
            SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, final_results[0].pi0_solution, est_l, "left"));
            std::cout << "General disjunction: Both Children are added" << std::endl;
            *result = SCIP_BRANCHED;
         }
         return SCIP_OKAY;

      } else {
         std::cout << "General disjunction: No feasible solution found, use SCIP default branching rule" << std::endl;
         *result = SCIP_DIDNOTFIND;
         return SCIP_OKAY;
      };
   };
};

/** creates the general disjunction branching rule and includes it in SCIP */

SCIP_Longint SCIPbranchruleGeneralDisjunctionGetMILPNodes(SCIP* scip) {
    return g_total_milp_nodes;
}

SCIP_Longint SCIPbranchruleGeneralDisjunctionGetProbingLPs(SCIP* scip) {
    return g_total_probing_lps;
}

SCIP_Longint SCIPbranchruleGeneralDisjunctionGetMILPs(SCIP* scip) {
    return g_total_milps;
}
void SCIPbranchruleGeneralDisjunctionResetCounters() {
    g_total_milp_nodes = 0;
    g_total_probing_lps = 0;
    g_total_milps = 0;
}
SCIP_RETCODE SCIPincludeBranchruleGeneralDisjunction(SCIP* scip) {

   SCIPbranchruleGeneralDisjunctionResetCounters();

   BranchruleGeneralDisjunction* mybranchrule = new BranchruleGeneralDisjunction(scip);
   SCIP_CALL(SCIPincludeObjBranchrule(scip,  mybranchrule, TRUE));

   return SCIP_OKAY;
}
#ifdef __cplusplus
}
#endif

