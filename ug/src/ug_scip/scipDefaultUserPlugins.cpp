/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*          This file is part of the program and software framework          */
/*                    UG --- Ubquity Generator Framework                     */
/*                                                                           */
/*  Copyright Written by Yuji Shinano <shinano@zib.de>,                      */
/*            Copyright (C) 2021-2024 by Zuse Institute Berlin,              */
/*            licensed under LGPL version 3 or later.                        */
/*            Commercial licenses are available through <licenses@zib.de>    */
/*                                                                           */
/* This code is free software; you can redistribute it and/or                */
/* modify it under the terms of the GNU Lesser General Public License        */
/* as published by the Free Software Foundation; either version 3            */
/* of the License, or (at your option) any later version.                    */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU Lesser General Public License for more details.                       */
/*                                                                           */
/* You should have received a copy of the GNU Lesser General Public License  */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>.     */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**@file    scipDefaultUserPlugins.cpp
 * @brief   Set SCIP default user plugins.
 * @author  Yuji Shinano
 *
 *
 *
 */

/*---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#include "scipUserPlugins.h"
#include "scipParaInstance.h"
#include "scipParaSolver.h"
#include "scipParaInitiator.h"
#include "objscip/branch_generaldisjunction.h"

using namespace UG;
using namespace ParaSCIP;

class GeneralBranchUserPlugins : public ScipUserPlugins {
    void operator()(SCIP *scip)
    {
       SCIP_CALL_ABORT( SCIPincludeBranchruleGeneralDisjunction(scip) );
       // SCIP_CALL_ABORT( SCIPsetIntParam(scip, "timing/clocktype", 2) );
       // SCIP_CALL_ABORT( SCIPsetIntParam(scip, "lp/solvefreq", -1) );
       // SCIP_CALL_ABORT( SCIPsetIntParam(scip, "relaxing/SDP/freq", 1) );
       // SCIP_CALL_ABORT( SCIPsetRealParam(scip, "numerics/epsilon", 1e-9) );
       // SCIP_CALL_ABORT( SCIPsetRealParam(scip, "numerics/sumepsilon", 1e-6) );
       // SCIP_CALL_ABORT( SCIPsetRealParam(scip, "numerics/feastol", 1e-6) );
       // SCIP_CALL_ABORT( SCIPsetBoolParam(scip, "lp/cleanuprows", FALSE) );
       // SCIP_CALL_ABORT( SCIPsetBoolParam(scip, "lp/cleanuprowsroot", FALSE) );
       // SCIP_CALL_ABORT( SCIPsetIntParam(scip, "nodeselection/hybridestim/stdpriority", 1000000) );
       // SCIP_CALL_ABORT( SCIPsetIntParam(scip, "nodeselection/hybridestim/maxplungedepth", 0) );
       // SCIP_CALL_ABORT( SCIPsetRealParam(scip, "nodeselection/hybridestim/estimweight", 0.0) );

    }
};

void
setUserPlugins(ParaInitiator *inInitiator)
{
   ScipParaInitiator *initiator = dynamic_cast<ScipParaInitiator *>(inInitiator);
   initiator->setUserPlugins(new GeneralBranchUserPlugins());
}

void
setUserPlugins(ParaInstance *inInstance)
{
   ScipParaInstance *instance = dynamic_cast<ScipParaInstance *>(inInstance);
   instance->setUserPlugins(new GeneralBranchUserPlugins());
}

void
setUserPlugins(ParaSolver *inSolver)
{
   ScipParaSolver *solver = dynamic_cast<ScipParaSolver *>(inSolver);
   solver->setUserPlugins(new GeneralBranchUserPlugins());
}