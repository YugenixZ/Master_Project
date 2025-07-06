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
class BranchruleGeneralDisjunction : public scip::ObjBranchrule {
public:
    int M = 1;
    int k = 2;
    SCIP_Real delta = 1e-4;

    explicit BranchruleGeneralDisjunction(SCIP* scip)
            : ObjBranchrule(scip, scip_name_, scip_desc_, scip_priority_, scip_maxdepth_, scip_maxbounddist_) {}
    virtual SCIP_DECL_BRANCHEXECLP(scip_execlp);

};

/*
 * Local methods
 */
static
SCIP_Real createTestModel(const CSRMatrix A, const vector<SCIP_Real>& b, const vector<SCIP_Real>& c) {
   SCIP* model_test = nullptr;
   SCIP_CALL_ABORT(SCIPcreate(&model_test));
   SCIP_CALL_ABORT(SCIPincludeDefaultPlugins(model_test));
   SCIP_CALL_ABORT(SCIPcreateProbBasic(model_test, "test_model"));
   SCIP_CALL_ABORT(SCIPreadParams(model_test, "/scratch/htc/yzhou/exp_scipmip/settings/default1.set"));
   int n = c.size();
   int m = b.size();
   vector<SCIP_VAR*> vars(n);

   // Create variables
   for (int i = 0; i < n; ++i) {
      SCIP_VAR* var;
      SCIP_CALL_ABORT(SCIPcreateVarBasic(model_test, &var, ("x_" + to_string(i)).c_str(), -SCIPinfinity(model_test), SCIPinfinity(model_test), c[i], SCIP_VARTYPE_CONTINUOUS));
      SCIP_CALL_ABORT(SCIPaddVar(model_test, var));
      vars[i] = var;
   }

   // Add constraints Ax >= b
   for (int i = 0; i < m; ++i) {
      SCIP_CONS* cons;
      SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_test, &cons, ("cons_" + to_string(i)).c_str(), 0, nullptr, nullptr, b[i], SCIPinfinity(model_test)));
      for (int j = A.row_ptr[i]; j < A.row_ptr[i + 1]; ++j) {
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_test, cons, vars[A.col_indices[j]], A.values[j]));
      }
      SCIP_CALL_ABORT(SCIPaddCons(model_test, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_test, &cons));
   }

   // Set objective function
   SCIP_CALL_ABORT(SCIPsetObjsense(model_test, SCIP_OBJSENSE_MINIMIZE));
   SCIPsetMessagehdlrQuiet(model_test, TRUE); 
   SCIP_RETCODE retcode = SCIPsolve(model_test);
   if (retcode != SCIP_OKAY) {
      for (int i = 0; i < n; ++i) {
         SCIP_CALL_ABORT(SCIPreleaseVar(model_test, &vars[i]));
      }
      SCIPfree(&model_test);
      return 1e+20; // Return a large value if the model cannot be solved
   }
   SCIP_Status status = SCIPgetStatus(model_test);
   if (status == SCIP_STATUS_OPTIMAL) {
      SCIP_Real sol_primal = SCIPgetPrimalbound(model_test);
      for (int i = 0; i < n; ++i) {
         SCIP_CALL_ABORT(SCIPreleaseVar(model_test, &vars[i]));
      }
      SCIPfree(&model_test);
      return sol_primal;
   } else {
      for (int i = 0; i < n; ++i) {
         SCIP_CALL_ABORT(SCIPreleaseVar(model_test, &vars[i]));
      }
      SCIPfree(&model_test);
      return 1e+20; // Return a large value if not optimal
   }
}
static
SCIP_Real createTestModel_2(const CSRMatrix A, const vector<SCIP_Real>& b, const vector<SCIP_Real>& c,
                            const vector<SCIP_Real>& pi_solution, SCIP_Real pi0_solution, bool is_left) {
   SCIP* model_test = nullptr;
   SCIP_CALL_ABORT(SCIPcreate(&model_test));
   SCIP_CALL_ABORT(SCIPincludeDefaultPlugins(model_test));
   SCIP_CALL_ABORT(SCIPcreateProbBasic(model_test, "test_model_2"));
   SCIP_CALL_ABORT(SCIPreadParams(model_test, "/scratch/htc/yzhou/exp_scipmip/settings/default1.set"));
   size_t n = c.size();
   size_t m = b.size();
   vector<SCIP_VAR*> vars(n);

   // Create variables
   if (is_left){
      for (size_t i = 0; i < n; ++i) {
         SCIP_VAR* var;
         SCIP_CALL_ABORT(SCIPcreateVarBasic(model_test, &var, ("x_" + to_string(i)).c_str(), -SCIPinfinity(model_test), SCIPinfinity(model_test), pi_solution[i], SCIP_VARTYPE_CONTINUOUS));
         SCIP_CALL_ABORT(SCIPaddVar(model_test, var));
         vars[i] = var;
      }
   } else {
      for (size_t i = 0; i < n; ++i) {
         SCIP_VAR* var;
         SCIP_CALL_ABORT(SCIPcreateVarBasic(model_test, &var, ("x_" + to_string(i)).c_str(), -SCIPinfinity(model_test), SCIPinfinity(model_test), -pi_solution[i], SCIP_VARTYPE_CONTINUOUS));
         SCIP_CALL_ABORT(SCIPaddVar(model_test, var));
         vars[i] = var;
      }
   }

   // Add constraints Ax >= b
   for (size_t i = 0; i < m; ++i) {
      SCIP_CONS* cons;
      SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_test, &cons, ("cons_" + to_string(i)).c_str(), 0, nullptr, nullptr, b[i], SCIPinfinity(model_test)));
      for (int j = A.row_ptr[i]; j < A.row_ptr[i + 1]; ++j) {
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_test, cons, vars[A.col_indices[j]], A.values[j]));
      }
      SCIP_CALL_ABORT(SCIPaddCons(model_test, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_test, &cons));
   }

   // Set objective function
   SCIP_CALL_ABORT(SCIPsetObjsense(model_test, SCIP_OBJSENSE_MINIMIZE));
   SCIPsetMessagehdlrQuiet(model_test, TRUE);

   SCIP_CALL_ABORT(SCIPsolve(model_test));
   SCIP_Status status = SCIPgetStatus(model_test);
   if (status == SCIP_STATUS_OPTIMAL) {
      SCIP_Real sol_primal = SCIPgetPrimalbound(model_test);
      for (size_t i = 0; i < n; ++i) {
         SCIP_CALL_ABORT(SCIPreleaseVar(model_test, &vars[i]));
      }
      SCIPfree(&model_test);
      return sol_primal;
   } else {
      for (size_t i = 0; i < n; ++i) {
         SCIP_CALL_ABORT(SCIPreleaseVar(model_test, &vars[i]));
      }
      SCIPfree(&model_test);
      return 1e+20; // Return a large value if not optimal
   }
}  
static
int getMagnitudeBase(int x) {
    x = std::abs(x);
    if (x == 0) return 1;
    int mag = 1;
    while (x >= 10) {
        x /= 10;
        mag *= 10;
    }
    return mag;
}

static
pair<SCIP_Status, SCIP_Real> ckmodel_create_side(
   const CSRMatrix& A,
   const vector<SCIP_Real>& b,
   const vector<SCIP_Real>& c,
   int n,
   const vector<int>& pi_solution,
   int pi0_solution,
   const string& side // "left" or "right"
) {
   // Copy the original matrix and vectors
   CSRMatrix A_ext = A;
   vector<SCIP_Real> b_ext = b;
   vector<SCIP_Real> c_ext = c;

   // Add new row for the disjunction constraint
   if (side == "left") {
      // pi_solution * x <= pi0  <=>  pi_solution * x - pi0 <= 0  <=>  -pi_solution * x + pi0 >= 0
      for (int i = 0; i < n; ++i) {
         A_ext.values.push_back(-pi_solution[i]);
         A_ext.col_indices.push_back(i);
      }
      A_ext.row_ptr.push_back(A_ext.values.size());
      b_ext.push_back(-pi0_solution);
   } else if (side == "right") {
      // pi_solution * x >= pi0 + 1  <=>  pi_solution * x - (pi0 + 1) >= 0
      for (int i = 0; i < n; ++i) {
         A_ext.values.push_back(pi_solution[i]);
         A_ext.col_indices.push_back(i);
      }
      A_ext.row_ptr.push_back(A_ext.values.size());
      b_ext.push_back(pi0_solution + 1.0);
   } else {
      std::cerr << "Invalid side parameter: " << side << std::endl;
      return {SCIP_STATUS_INFEASIBLE, 1e+20};
   }
   A_ext.num_rows = b_ext.size();

   // Create and solve the model
   SCIP_Real sol_val = createTestModel(A_ext, b_ext, c_ext);
   if (sol_val < 1e+20) {
      SCIP_Status status = SCIP_STATUS_OPTIMAL;
      return {status, sol_val};
   } else {
      SCIP_Status status = SCIP_STATUS_INFEASIBLE;
      return {status, 1e+20};
   }
}

static
std::pair<double, double> computeScaledDelta(
    const std::vector<double>& b_vec,
    int z_L,
    double base_delta = 1e-6,
    double max_delta = 0.5
) {
    // Max b
    double max_b = 0.0; 
      for (const auto& b : b_vec) {
         if (std::abs(b) > max_b) {
               max_b = std::abs(b);
         }
      }
    
    // Compute the combined magnitude measure X = |z_L| + ||b||_1
    double X = std::abs(z_L) + max_b;

    // Get the magnitude base, e.g. 50139 -> 10000
    double scale = static_cast<double>(getMagnitudeBase(static_cast<int>(X)));

    // Scale delta proportionally based on the magnitude
    double delta = base_delta * scale;

    // Ensure delta is not smaller than the minimum allowed value
    if (delta - max_delta > 1e-6) {
        delta = max_delta;
    }

    // Compute delta_shifted as delta minus 1 (scaled accordingly)
    double delta_shifted = delta - 1.0;

    return std::make_pair(delta, delta_shifted);
}

static
SCIP_Bool notallzero(const vector <SCIP_Real>& vec) {
   return std::any_of(vec.begin(), vec.end(), [](double val) { return abs(val) - 1e-6 > 0.0; });
}
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
   for (int i = 0; i < ncols; ++i) {
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
   for (int i = 0; i < nrows; ++i) {
      SCIP_ROW* row = rows[i];
//        SCIP_Bool rowiscut = SCIProwIsInGlobalCutpool(row);
//        cout << "rowiscut: " << rowiscut << endl;
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

   for (int i = 0; i < ncols; ++i) {
      // Skip if the column is not in the LP
      // SCIP_VAR* var = SCIPcolGetVar(cols[i]);
      // SCIP_Real lb = SCIPvarGetLbLocal(var);
      // SCIP_Real ub = SCIPvarGetUbLocal(var);
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

static
SubmodelVars submodel_create(
        SCIP* scip,
        CSRMatrix A,
        vector<SCIP_Real> b,
        vector<SCIP_Real> c,
        int M,
        int k,
        SCIP_Real delta,
        SCIP_Real zl
){          
   // SCIP_Real delta_scaled = compute_scaled_delta(b, zl, delta, 10000.0);
   // SCIP_Real delta_scaled_minus = compute_scaled_delta(b, zl, delta - 1, 10000.0);    
   pair<SCIP_Real, SCIP_Real> scaled_deltas = computeScaledDelta(b, zl, delta);
   SCIP_Real delta_scaled = 1e-4;
   SCIP_Real delta_scaled_minus = delta_scaled -  1;  
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

   for (int j = 0; j < n; ++j) {
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
   for (int j = 0; j < n; ++j ){
      SCIP_CONS* cons;
      SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, ("cons_pA_" + to_string(j)).c_str(), 0, nullptr, nullptr, 0.0, 0.0));
      CSRMatrix At = A.transpose();
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
      SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, "cons_pb", 0, nullptr, nullptr, delta_scaled, SCIPinfinity(model_sub)));
      for (int i = 0; i < m; ++i) {
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, p[i], b[i]));
      }
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, s_L, -zl));
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi0, -1.0));
      SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons));
   }

   for (int j = 0; j < n; ++j){
      SCIP_CONS* cons;
      SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, ("cons_qA_" + to_string(j)).c_str(), 0, nullptr, nullptr, 0.0, 0.0));
      CSRMatrix At = A.transpose();
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
      SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, "cons_qb", 0, nullptr, nullptr, delta_scaled_minus, SCIPinfinity(model_sub)));
      for (int i = 0; i < m; ++i) {
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, q[i], b[i]));
      }
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, s_R, -zl));
      SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi0, 1.0));
      SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons));
   }

   {
      SCIP_CONS* cons;
      SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, "cons_pi_sum", 0, nullptr, nullptr, 1, k));
      for (int i = 0; i < n; ++i) {
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_plus[i], 1.0));
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_minus[i], 1.0));
      }
      SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons));
      SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons));
   }
   // Add constraint that if var is continuous then the corresponding pi[i] = 0
   SCIP_COL **cols = SCIPgetLPCols(scip);
   for (int j = 0; j < n; ++j) {
      SCIP_VAR* var = SCIPcolGetVar(cols[j]);
      if (SCIPvarGetType(var) == SCIP_VARTYPE_CONTINUOUS) {
         SCIP_CONS* cons;
         SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, ("cons_pi_continuous_" + to_string(j)).c_str(), 0, nullptr, nullptr, 0.0, 0.0));
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_plus[j], 1.0));
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_minus[j], -1.0));
         SCIP_CALL_ABORT(SCIPaddCons(model_sub, cons));
         SCIP_CALL_ABORT(SCIPreleaseCons(model_sub, &cons));
      }
   }


   // Check if the LP relaxation of the original problem is optimal
   SCIP_LPSOLSTAT status_LP = SCIPgetLPSolstat(scip);
   if (status_LP == SCIP_LPSOLSTAT_OPTIMAL) {
      vector<SCIP_Real> x_star(n);
      SCIP_Real epsilon = 1e-4;
      SCIP_COL **lp_cols = SCIPgetLPCols(scip);
      for (int j = 0; j < n; ++j) {
         x_star[j] = SCIPgetSolVal(scip, nullptr, SCIPcolGetVar(lp_cols[j]));
      }

      // Add constraints pi0 <= sum((pi_plus[i] - pi_minus[i]) * x_star[i]) - epsilon
      {
         SCIP_CONS *cons;
         SCIP_CALL_ABORT(SCIPcreateConsBasicLinear(model_sub, &cons, "cons_pi0_lower", 0, nullptr, nullptr, epsilon, SCIPinfinity(model_sub)));
         for (int i = 0; i < n; ++i) {
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
         for (int i = 0; i < n; ++i) {
            SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_plus[i], x_star[i]));
            SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi_minus[i], -x_star[i]));
         }
         SCIP_CALL_ABORT(SCIPaddCoefLinear(model_sub, cons, pi0, -1.0));
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

   retcode = SCIPsetRealParam(model_sub, "limits/time", 1000);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_sub);
      return SubmodelVars{nullptr, {}, nullptr, {}, nullptr, {}, {}, nullptr};
   }
   SCIPsetMessagehdlrQuiet(model_sub, TRUE);
   return SubmodelVars{model_sub, p, s_L, q, s_R, pi_plus, pi_minus, pi0};
}

static
pair<SCIP_Status, SCIP_Real> ckmodel_create(
        const string& name,
        CSRMatrix A,
        vector<SCIP_Real> b,
        vector<SCIP_Real> c,
        int m,
        int n,
        vector<SCIP_Real> pi_solution,
        SCIP_Real pi0_solution,
        const string& condition
) {
   SCIP* model_ck;
   SCIP_RETCODE retcode;
   vector<SCIP_VAR*> x(n);


   retcode = SCIPcreate(&model_ck);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      return {SCIP_STATUS_INFEASIBLE,1e+20};
   }

   retcode = SCIPincludeDefaultPlugins(model_ck);
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      SCIPfree(&model_ck);
      return {SCIP_STATUS_INFEASIBLE,1e+20};
   }

   retcode = SCIPcreateProbBasic(model_ck, name.c_str());
   if (retcode != SCIP_OKAY) {
      SCIPprintError(retcode);
      return {SCIP_STATUS_INFEASIBLE,1e+20};
   }
   SCIPreadParams(model_ck, "/scratch/htc/yzhou/exp_scipmip/settings/default1.set");
   for (int i = 0; i < n; ++i) {
      SCIP_VAR * var;
      SCIPcreateVarBasic(model_ck, &var, ("x_" + to_string(i)).c_str(), -SCIPinfinity(model_ck), SCIPinfinity(model_ck), c[i], SCIP_VARTYPE_CONTINUOUS);
      SCIPaddVar(model_ck, var);
      x[i] = var;
   }

   for (int j = 0; j < m; ++j) {
      SCIP_CONS * cons;
      SCIPcreateConsBasicLinear(model_ck, &cons, ("cons_" + to_string(j)).c_str(), 0, NULL, NULL, b[j], SCIPinfinity(model_ck));
      for (int i = A.row_ptr[j]; i < A.row_ptr[j + 1]; ++i) {
         SCIPaddCoefLinear(model_ck, cons, x[A.col_indices[i]], A.values[i]);
      }
      SCIPaddCons(model_ck, cons);
      SCIPreleaseCons(model_ck, &cons);
   }

   if (condition == "pi0") {
      SCIP_CONS* cons;
      SCIPcreateConsBasicLinear(model_ck, &cons, "cons_pi0", 0, NULL, NULL, -SCIPinfinity(model_ck), pi0_solution);
      for (int i = 0; i < n; ++i) {
         if( SCIPisFeasZero(model_ck, pi_solution[i]) ) {
            continue;
         }
         SCIPaddCoefLinear(model_ck, cons, x[i], pi_solution[i]);
      }
      SCIPaddCons(model_ck, cons);
      SCIPreleaseCons(model_ck, &cons);
   }
   else if (condition == "pi0+1") {
      SCIP_CONS* cons;
      int pi0_plus = pi0_solution + 1;
      SCIPcreateConsBasicLinear(model_ck, &cons, "cons_pi0_plus_1", 0, NULL, NULL, pi0_plus, SCIPinfinity(model_ck));
      for (int i = 0; i < n; ++i) {
         if( SCIPisFeasZero(model_ck, pi_solution[i]) ) {
            continue;
         }
         SCIPaddCoefLinear(model_ck, cons, x[i], pi_solution[i]);
      }
      SCIPaddCons(model_ck, cons);
      SCIPreleaseCons(model_ck, &cons);
   }

   SCIPsetObjsense(model_ck, SCIP_OBJSENSE_MINIMIZE);
   SCIPsetMessagehdlrQuiet(model_ck, TRUE);
   SCIPsolve(model_ck);
   SCIP_Status status = SCIPgetStatus(model_ck);
   if (status == SCIP_STATUS_OPTIMAL) {
      SCIP_Real sol_primal = SCIPgetPrimalbound(model_ck);
      for (int i = 0; i < n; ++i) {
         SCIPreleaseVar(model_ck, &x[i]);
      }
      SCIPfree(&model_ck);
      return {status, sol_primal};
   }
   else {
      for (int i = 0; i < n; ++i) {
         SCIPreleaseVar(model_ck, &x[i]);
      }
      SCIPfree(&model_ck);
      return {status, 1e+20};
   }
};

static
pair<string, SCIP_Real> check_feasibility(
        SCIP_Status sol_status,
        SCIP_Real sol_val,
        SCIP_Real Best_zl,
        SCIP_Real node_ub
)
{
   string status;
   SCIP_Real est;
   if (sol_status == SCIP_STATUS_OPTIMAL) {
      if (sol_val - node_ub > 1e-6) {
         status = "infeasible";  
         est = 1e+20;
      } 
      else{
         if (sol_val - Best_zl > 1e-6) {
            status = "updated_zl";
            //Retrieve the objective value of the solution.
            est = sol_val;
         } else {
            status = "obj_val less than Best_zl";
            est = sol_val;
            return {status, est};
         }
      }
   }
   else {
      status = "ckmodel infeasible";
      est = 1e+20;
      return {status, est};
   }
   return {status, est};
}

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
        SCIP_Real node_ub
){

   queue<SCIP_Real> estL_list;
   queue<SCIP_Real> estR_list;
   queue<SCIP_Real> feasible_zl;
   queue<vector<SCIP_Real>> best_pi_solutions;
   queue<SCIP_Real> best_pi0_solutions;
   queue<string> Status_l;
   queue<string> Status_r;
   vector<Submodel_sols> final_results;

   while (abs(zl_high - zl_low) > 1e-6){
      SCIP_Real zl = (zl_high + zl_low) / 2;
      SubmodelVars submodel_datas = submodel_create(scip, A, b, c, M, k, delta, zl);
      SCIP_RETCODE retcode = SCIPsolve(submodel_datas.model_sub);
      if (retcode != SCIP_OKAY) {
         std::cerr << "Error solving submodel: " << std::endl;
         Submodel_sols result = {SCIP_INVALID, {}, {}, NULL, NULL, "NULL", "NULL"};
         final_results.push_back(result);

         return final_results;
      }
      if (SCIPgetStatus(submodel_datas.model_sub) == SCIP_STATUS_OPTIMAL) {
         // Retrieve the solutions
         SCIP_Sol *submodel_sol = SCIPgetBestSol(submodel_datas.model_sub); // pi0_solution
         vector<SCIP_Real> pi_plus_solution(n);
         vector<SCIP_Real> pi_minus_solution(n);
         vector<SCIP_Real> pi_solution(n);
         vector<SCIP_Real> p_solution(m); 
         vector<SCIP_Real> q_solution(m); 
         for (int i = 0; i < m; ++i) {
            p_solution[i] = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.p[i]);
            q_solution[i] = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.q[i]);
         }
         SCIP_Real s_L_solution = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.s_L);
         SCIP_Real s_R_solution = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.s_R);
         int pi0_solution = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.pi0);

         for (int i = 0; i < n; ++i) {
            pi_plus_solution[i] = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.pi_plus[i]);
            pi_minus_solution[i] = SCIPgetSolVal(submodel_datas.model_sub, submodel_sol, submodel_datas.pi_minus[i]);
            pi_solution[i] = pi_plus_solution[i] - pi_minus_solution[i];
         }
         assert(SCIPisFeasIntegral(submodel_datas.model_sub, pi0_solution));
         for (SCIP_Real i : pi_solution) {
            assert(SCIPisFeasIntegral(submodel_datas.model_sub, i));
         }
         SCIP_Real pb = 0.0;
         for (int i = 0; i < m; ++i) {
            pb += p_solution[i] * b[i];
         }
         SCIP_Real qb = 0.0;     
         for (int i = 0; i < m; ++i) {
            qb += q_solution[i] * b[i];
         }
         vector<SCIP_Real> pA(n, 0.0);
         vector<SCIP_Real> qA(n, 0.0); 
         for (int j = 0; j < n; ++j) {
            for (int i =  A.transpose().row_ptr[j]; i < A.transpose().row_ptr[j + 1]; ++i) {
               pA[j] += p_solution[ A.transpose().col_indices[i]] *  A.transpose().values[i];
               qA[j] += q_solution[ A.transpose().col_indices[i]] *  A.transpose().values[i];
            }
         }                                    
         // Check if both pi and pi0 are all zero
         if (notallzero(pi_solution)) {
            if (!SCIPisFeasZero(scip, s_L_solution) || !SCIPisFeasZero(scip, s_R_solution)) {
               cout << "One of the s_L and s_R are non-zero, DEBUG" << endl;
            }
            // Check if the solution is feasible for the general disjunction
            pair<SCIP_Status, SCIP_Real> model_ck_l_info = ckmodel_create("check_model_left", A, b, c, m, n, pi_solution, pi0_solution, "pi0");
            pair<SCIP_Status, SCIP_Real> model_ck_r_info = ckmodel_create("check_model_right", A, b, c, m, n, pi_solution, pi0_solution, "pi0+1");

            // pair<SCIP_Status, SCIP_Real> model_ck_l_info = ckmodel_create_side(A, b, c, n, pi_solution, pi0_solution, "left");
            // pair<SCIP_Status, SCIP_Real> model_ck_r_info = ckmodel_create_side(A, b, c, n, pi_solution, pi0_solution, "right");
            
            pair<string, SCIP_Real> result_l = check_feasibility(model_ck_l_info.first, model_ck_l_info.second, zl, node_ub);
            pair<string, SCIP_Real> result_r = check_feasibility(model_ck_r_info.first, model_ck_r_info.second, zl, node_ub);

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
            } 
            // else if (result_l.first == "ckmodel infeasible" && result_r.first == "ckmodel infeasible") {

            //    pair <SCIP_Status, SCIP_Real> model_ck_l_info_t = ckmodel_create_side(A, b, c, n, pi_solution, pi0_solution, "left");  
            //    pair <SCIP_Status, SCIP_Real> model_ck_r_info_t = ckmodel_create_side(A, b, c, n, pi_solution, pi0_solution, "right");

            //    cout << "Left model status: " << model_ck_l_info_t.first << ", value: " << model_ck_l_info.second << endl;
            //    cout << "Right model status: " << model_ck_r_info_t.first << ", value: " << model_ck_r_info.second << endl;

            //    for (int i = 0; i < n; ++i) {
            //       cout << "pi_solution:" << pi_solution[i] << endl;  
            //    }
            //    cout << "pi0_solution: " << pi0_solution << endl;

            //    SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_L);
            //    SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.s_R);
            //    SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi0);
            //    for (int i = 0; i < m; ++i) {
            //       SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.p[i]);
            //       SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.q[i]);
            //    }
            //    for (int i = 0; i < n; ++i) {
            //       SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_minus[i]);
            //       SCIPreleaseVar(submodel_datas.model_sub, &submodel_datas.pi_plus[i]);
            //    }
            //    SCIPfree(&submodel_datas.model_sub);
            //    zl_high = zl;
            // } 
            else if (result_l.first == "infeasible" && result_r.first == "infeasible") {

               if (feasible_zl.size() > 1) {
                  feasible_zl.push(zl);
                  best_pi_solutions.push(pi_solution);
                  best_pi0_solutions.push(pi0_solution);
                  Status_l.push(result_l.first);
                  Status_r.push(result_r.first);
                  estL_list.push(result_l.second);
                  estR_list.push(result_r.second);

                  feasible_zl.pop();
                  best_pi_solutions.pop();
                  best_pi0_solutions.pop();
                  Status_l.pop();
                  Status_r.pop();
                  estL_list.pop();
                  estR_list.pop();
               } else if (feasible_zl.empty()){
                  feasible_zl.push(zl);
                  best_pi_solutions.push(pi_solution);
                  best_pi0_solutions.push(pi0_solution);
                  Status_l.push(result_l.first);
                  Status_r.push(result_r.first);
                  estL_list.push(result_l.second);
                  estR_list.push(result_r.second);
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
      }
   }
   assert (feasible_zl.size() == best_pi_solutions.size());
   assert (feasible_zl.size() == best_pi0_solutions.size());

   Submodel_sols result;

   if (feasible_zl.empty()){
      result = {SCIP_INVALID, {}, {}, NULL, NULL, "NULL", "NULL"};
   }
   else {
      SCIP_Real best_zl = feasible_zl.back();
      vector<SCIP_Real> best_pi_solution = best_pi_solutions.back();
      SCIP_Real best_pi0_solution = best_pi0_solutions.back();
      string status_l = Status_l.back();
      string status_r = Status_r.back();
      SCIP_Real est_l = estL_list.back();
      SCIP_Real est_r = estR_list.back();
      result = {best_zl, best_pi_solution, best_pi0_solution, est_l, est_r, status_l, status_r};
   }
   final_results.push_back(result);
   return final_results;
};

static
SCIP_NODE* get_information(SCIP* scip) {
   std::cout << "_____________________________________" << std::endl;
   std::cout << "Now starting branching with general disjunction`" << std::endl;

   SCIP_NODE* curr_Node = SCIPgetCurrentNode(scip);
   std::cout << "Current branching Node number: " << SCIPnodeGetNumber(curr_Node) << std::endl;

   // if (SCIPnodeGetDepth(curr_Node) > 0) {
   //    int num_addedCons = SCIPnodeGetNAddedConss(curr_Node);
   //    auto** addedconss = (SCIP_CONS**) malloc(num_addedCons * sizeof(SCIP_CONS*));
   //    if (num_addedCons == 0) {
   //       std::cout << "No constraints added" << std::endl;

   //    } else {
   //       int nconss;
   //       SCIPnodeGetAddedConss(curr_Node, addedconss, &nconss ,num_addedCons);
   //       assert(nconss == num_addedCons);
   //       vector<SCIP_CONS *> addedconss_vec(addedconss, addedconss + num_addedCons);
   //       std::cout << "Number of added constraints: " << num_addedCons << std::endl;
   //       for (int i = 0; i < num_addedCons; ++i) {
   //          SCIP_ROW* row = SCIPgetRowLinear(scip, addedconss_vec[i]);
   //          std::cout << "Added constraint: ";
   //          SCIP_Real* vals = SCIProwGetVals(row);
   //          for (int j = 0; j < SCIProwGetNNonz(row); ++j) {
   //             std::cout << vals[j] << " ";
   //          }
   //          std::cout << std::endl;
   //          std::cout << "Rhs: " << SCIProwGetRhs(row) << std::endl;
   //          std::cout << "Lhs: " << SCIProwGetLhs(row) << std::endl;
   //       }
   //    }
   // }
   return curr_Node;
}

static
SCIP_RETCODE createBranchingConstraint(
        SCIP* scip,
        SCIP_Bool createChild,
        SCIP_NODE* curr_Node,
        const vector<SCIP_VAR*>& vars_lp,
        const vector<SCIP_Real>& pi_solution,
        SCIP_Real pi0_solution,
        SCIP_Real est,
        const string& side
)
{
   if (side == "left") {
      SCIP_CONS *cons_l;
      SCIP_CALL(SCIPcreateConsLinear(scip, &cons_l, (side + std::to_string(SCIPnodeGetNumber(curr_Node))).c_str(), 0, nullptr, nullptr, -SCIPinfinity(scip), pi0_solution, 
         TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE));
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
         SCIP_CALL(SCIPaddConsLocal(scip, cons_l, NULL));
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
         SCIP_CALL(SCIPaddConsLocal(scip, cons_r, NULL));
         SCIP_CALL(SCIPreleaseCons(scip, &cons_r));
      }
   }
   else {
      cout << "Invalid side" << endl;
   }
   return SCIP_OKAY;
};

static
SCIP_Real get_factor(SCIP_Real lp_gap) {
   SCIP_Real factor;
   assert (lp_gap >= 0);
   if (lp_gap < 0.1) {
      factor = 1.5 + ceil(lp_gap*100)/100;
   } else if (lp_gap >=0.1 && lp_gap < 1) {
      factor = 1.5 + ceil(lp_gap*10)/10;
   } else if (lp_gap == 1e+20) {
      factor = 5;
   } else {
      factor = (ceil(lp_gap) + 1) * 5;
   }
   return factor;
}

static
std::vector<SCIP_Real> read_sol_file(const std::string& sol_filename, int n)
{
    std::vector<SCIP_Real> x(n, 0.0);
    std::ifstream infile(sol_filename);
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string varname;
        SCIP_Real value;
        if (!(iss >> varname >> value)) continue;
        // Expect variable names like x_0, x_1, ...
        if (varname.substr(0,2) == "x_") {
            int idx = std::stoi(varname.substr(2));
            if (idx >= 0 && idx < n)
                x[idx] = value;
        }
    }
    return x;
}

// Checks feasibility of (pi, pi0) for a given solution x
static
bool check_pi_side_feasibility(
    const std::vector<SCIP_Real>& pi,
    SCIP_Real pi0,
    const std::vector<SCIP_Real>& x,
    const std::string& side, // "left" or "right"
    SCIP_Real tol = 1e-6
)
{
    SCIP_Real pix = 0.0;
    for (size_t i = 0; i < pi.size(); ++i)
        pix += pi[i] * x[i];

    if (side == "left")
        return (pix <= pi0 + tol);
    else if (side == "right")
        return (pix >= pi0 + 1 - tol);
    else {
        std::cerr << "Invalid side parameter: " << side << std::endl;
        return false;
    }
}
/*
 * Callback methods of branching rule
 */

/** branching execution method for fractional LP solutions */
SCIP_DECL_BRANCHEXECLP(BranchruleGeneralDisjunction::scip_execlp){
   {  /*lint --e{715}*/
      SCIP_Node *curr_Node = get_information(scip);
      MatrixData LP_data = getConstraintMatrix(scip);
      CSRMatrix A = LP_data.A;
      std::vector<SCIP_Real> b = LP_data.b;
      std::vector<SCIP_Real> c = LP_data.c;
      size_t m = b.size();
      size_t n = c.size();
//      testSubmodelCreate();
      SCIP_LPSOLSTAT status_LP = SCIPgetLPSolstat(scip);
      if (status_LP == SCIP_LPSOLSTAT_OPTIMAL) {
         vector<SCIP_Real> x_star(n);
         SCIP_Real epsilon = 1e-4;
         SCIP_COL **cols = SCIPgetLPCols(scip);
         for (int j = 0; j < n; ++j) {
            x_star[j] = SCIPgetSolVal(scip, nullptr, SCIPcolGetVar(cols[j]));
         }
         size_t x_star_size = x_star.size();
         // Test if Ax >= b
         for (int i = 0; i < m; ++i) {
            SCIP_Real Ax = 0;
            for (int j = A.row_ptr[i]; j < A.row_ptr[i + 1]; ++j) {
               Ax += A.values[j] * x_star[A.col_indices[j]];
            }
            if (Ax < b[i] - epsilon) {
               std::cout << "Infeasible solution: Ax < b" << std::endl;
               std::cout << "Ax: " << Ax << ", b: " << b[i] << std::endl;  
               std::cout << "Row index: " << i << std::endl;
            }
         }
         SCIP_Real CX = 0;
         for (int i = 0; i < n; ++i) {
            SCIP_VAR* var = SCIPcolGetVar(cols[i]);
            if (SCIPvarIsActive(var)) {
               CX += c[i] * x_star[i];
            }
            else{
               std::cout << "Variable " << i << " is not active." << std::endl;
            }
         }
         std::cout << "Objective value: " << CX << std::endl;
      } else {
         std::cout << "Infeasible solution: LP not solved to optimality" << std::endl;
      }

      
      SCIP_Real LP_obj = SCIPgetLPObjval(scip);
      SCIP_Real node_lowerbound = SCIPgetNodeLowerbound(scip, curr_Node);
      SCIP_Real Primalsol = createTestModel(A, b, c); 
      SCIP_Real node_ub = SCIPgetPrimalbound(scip);      
      cout << "LP objective: " << LP_obj << endl;
      cout << "Primal solution: " << Primalsol << endl;
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
      SCIP_Real factor = get_factor(lp_gap);
      if (zl_init > 0) {
         zl_high = zl_init * factor;
      } else if (zl_init < 0) {
         zl_high = zl_init / factor ;
      } else {
         zl_high = 2;
      }
      std::vector<Submodel_sols> final_results = submodel_solve(scip, zl_low, zl_high, m, n, delta, A, b, c, M, k, node_ub);
      SCIP_Real est_l = final_results[0].est_l;
      SCIP_Real est_r = final_results[0].est_r;
      string status_l = final_results[0].status_l;
      string status_r = final_results[0].status_r;
      // std::vector<SCIP_Real> x = read_sol_file("/scratch/htc/yzhou/qoblib/qoblib-quantum-optimization-benchmarking-library/08-network/solutions/network06.opt.sol", final_results[0].pi_solution.size());

      if ( status_l == "NULL" || status_r == "NULL") {
         std::cout << "General disjunction: No feasible solution found, use SCIP default branching rule" << std::endl;
         *result = SCIP_DIDNOTRUN;
         return SCIP_OKAY;

      } else if (status_l == "updated_zl" && status_r == "updated_zl") {
         // bool left_feas = check_pi_side_feasibility(final_results[0].pi_solution, final_results[0].pi0_solution, x, "left");
         // bool right_feas = check_pi_side_feasibility(final_results[0].pi_solution, final_results[0].pi0_solution, x, "right");
         
         // cout << "Left feasibility: " << left_feas << "The est is: "<< final_results[0].est_l << std::endl;
         // cout << "Right feasibility: " << right_feas << "The est is: "<< final_results[0].est_r << std::endl;
         SCIP_Bool CreateChild = TRUE;
         int pi0_sol_plus = final_results[0].pi0_solution + 1; 
         SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, final_results[0].pi0_solution, est_l, "left"));
         SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, pi0_sol_plus, est_r, "right"));
         std::cout << "General disjunction: Both children are added" << std::endl;
         *result = SCIP_BRANCHED;
         return SCIP_OKAY;

      }else if (status_l == "infeasible" && status_r != "updated_zl") {
         std::cout << "General disjunction: Infeasible disjunction" << std::endl;
         if (SCIPnodeGetNumber(curr_Node) == 1) {
            std::cout << "Root node: No feasible solution found, use SCIP default branching rule" << std::endl;
            *result = SCIP_DIDNOTRUN;
         }
         return SCIP_OKAY;

      } else if (status_l != "updated_zl" && status_r == "infeasible") {
         std::cout << "General disjunction: Infeasible disjunction" << std::endl;
         if (SCIPnodeGetNumber(curr_Node) == 1) {
            std::cout << "Root node: No feasible solution found, use SCIP default branching rule" << std::endl;
            *result = SCIP_DIDNOTRUN;
         }
         return SCIP_OKAY;

      } else if (status_l == "updated_zl" && status_r != "updated_zl") {
         // bool left_feas = check_pi_side_feasibility(final_results[0].pi_solution, final_results[0].pi0_solution, x, "left");
         // bool right_feas = check_pi_side_feasibility(final_results[0].pi_solution, final_results[0].pi0_solution, x, "right");
         
         // cout << "Left feasibility: " << left_feas << "The est is: "<< final_results[0].est_l << std::endl;
         // cout << "Right feasibility: " << right_feas << "The est is: "<< final_results[0].est_r << std::endl;

         if (status_r == "infeasible" || status_r == "ckmodel infeasible") {     
               
         SCIP_Bool CreateChild = FALSE;
         SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, final_results[0].pi0_solution, est_l, "left"));
         std::cout << "General disjunction: Only Left constraint added:" << std::endl;
         *result = SCIP_CONSADDED;
         }
         else {
            if (est_r < node_lowerbound) {
               est_r = node_lowerbound;
            }
            int pi0_sol_plus = final_results[0].pi0_solution + 1; 
            SCIP_Bool CreateChild = TRUE;
            SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, final_results[0].pi0_solution, est_l, "left"));
            SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, pi0_sol_plus, est_r, "right"));
            std::cout << "General disjunction: Both Children are added:" << std::endl;
            *result = SCIP_BRANCHED;
         }
         return SCIP_OKAY;

      } else if (status_r == "updated_zl" && status_l != "updated_zl") {
         // bool left_feas = check_pi_side_feasibility(final_results[0].pi_solution, final_results[0].pi0_solution, x, "left");
         // bool right_feas = check_pi_side_feasibility(final_results[0].pi_solution, final_results[0].pi0_solution, x, "right");
         
         // cout << "Left feasibility: " << left_feas << "The est is: "<< final_results[0].est_l << std::endl;
         // cout << "Right feasibility: " << right_feas << "The est is: "<< final_results[0].est_r << std::endl;
         if (status_l == "infeasible" || status_l == "ckmodel infeasible") {  
         SCIP_Bool CreateChild = FALSE;
         int pi0_sol_plus = final_results[0].pi0_solution + 1; 
         SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, pi0_sol_plus, est_r, "right"));
         std::cout << "General disjunction: Only Right constraint added:" << std::endl;
         *result = SCIP_CONSADDED;
         }
         else {
            if (est_l < node_lowerbound) {
               est_l = node_lowerbound;
            }
            SCIP_Bool CreateChild = TRUE;
            int pi0_sol_plus = final_results[0].pi0_solution + 1; 
            SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, pi0_sol_plus, est_r, "right"));
            SCIP_CALL(createBranchingConstraint(scip, CreateChild, curr_Node, vars_lp, final_results[0].pi_solution, final_results[0].pi0_solution, est_l, "left"));
            std::cout << "General disjunction: Both Children are added:" << std::endl;
            *result = SCIP_BRANCHED;
         }
         return SCIP_OKAY;

      } else {
         std::cout << "General disjunction: Both children are not added" << std::endl;
         *result = SCIP_DIDNOTRUN;
         return SCIP_OKAY;
      };
   };
};

/** creates the general disjunction branching rule and includes it in SCIP */
extern "C" SCIP_RETCODE SCIPincludeBranchruleGeneralDisjunction(SCIP* scip) {
   BranchruleGeneralDisjunction* mybranchrule = new BranchruleGeneralDisjunction(scip);
   SCIP_CALL(SCIPincludeObjBranchrule(scip,  mybranchrule, TRUE));
   return SCIP_OKAY;
}
#ifdef __cplusplus
}
#endif

