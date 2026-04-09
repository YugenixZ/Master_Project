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

/**@file   examples/MIPSolver/src/cppmain.cpp
 * @brief  main file for C++ example project using SCIP as a callable library
 * @author Tobias Achterberg
 */

/*--+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#include <iostream>
#include <cstdlib>
#include "objscip/objscip.h"
#include "objscip/objscipdefplugins.h"
#include "objscip/branch_generaldisjunction.h"

/** reads parameters */
static
SCIP_RETCODE readParams(
        SCIP*                      scip,               /**< SCIP data structure */
        const char*                filename            /**< parameter file name, or NULL */
)
{
   if( filename != NULL )
   {
      if( SCIPfileExists(filename) )
      {
         std::cout << "reading parameter file <" << filename << ">" << std::endl;
         SCIP_CALL( SCIPreadParams(scip, filename) );
      }
      else
         std::cout << "parameter file <" << filename << "> not found - using default parameters" << std::endl;
   }
   else if( SCIPfileExists("scipmip.set") )
   {
      std::cout << "reading parameter file <scipmip.set>" << std::endl;
      SCIP_CALL( SCIPreadParams(scip, "scipmip.set") );
   }

   return SCIP_OKAY;
}

/** starts SCIP */
static
SCIP_RETCODE fromCommandLine(
   SCIP*                 scip,               /**< SCIP data structure */
   const char*           filename,           /**< input file name */
   const char*           settingsfile,       /**< settings file name or NULL */
   const char*           solfilename,        /**< solution file name or NULL */
   double                objlimit,           /**< objective limit, or SCIP_DEFAULT_INFINITY if not set */
   int                   scipseed,           /**< SCIP seed */
   int                   permuteseed         /**< permutation seed */
   )
{
   /********************
    * Problem Creation *
    ********************/

   /* read problem */
   SCIP_CALL( SCIPreadProb(scip, filename, NULL) );


   /*********************************
    * Load parameters and solution *
    *********************************/

   if( settingsfile != NULL )
   {
      SCIP_CALL( SCIPreadParams(scip, settingsfile) );
   }

   if( solfilename != NULL )
   {
      SCIP_CALL( SCIPreadSol(scip, solfilename) );
   }

   if( objlimit < SCIP_DEFAULT_INFINITY )
   {
      SCIP_CALL( SCIPsetObjlimit(scip, objlimit) );
   }

   /* modify random seeds */
   SCIP_CALL( SCIPsetIntParam(scip, "randomization/randomseedshift", scipseed) );
   if( permuteseed >= 0 )
   {
      SCIP_CALL( SCIPsetIntParam(scip, "randomization/permutationseed", permuteseed) );
      SCIP_CALL( SCIPsetIntParam(scip, "randomization/lpseed", permuteseed) );
   }

   std::cout << "SCIP seed set to: " << scipseed << ", Permutation seed set to: " << permuteseed << std::endl;

   return SCIP_OKAY;
}

/** starts user interactive mode */
static
SCIP_RETCODE interactive(
        SCIP*                      scip                /**< SCIP data structure */
)
{
   SCIP_CALL( SCIPstartInteraction(scip) );

   return SCIP_OKAY;
}

/** creates a SCIP instance with default plugins, evaluates command line parameters, runs SCIP appropriately,
 *  and frees the SCIP instance
 */
static
SCIP_RETCODE runSCIP(
        int                        argc,               /**< number of shell parameters */
        char**                     argv                /**< array with shell parameters */
)
{
   SCIP* scip = NULL;

   /*********
    * Setup *
    *********/

   /* initialize SCIP */
   SCIP_CALL( SCIPcreate(&scip) );

   /***********************
    * Version information *
    ***********************/

   SCIPprintVersion(scip, NULL);
   std::cout << std::endl;

   /* include default SCIP plugins */
   SCIP_CALL( SCIPincludeDefaultPlugins(scip) );
   SCIP_CALL( SCIPincludeBranchruleGeneralDisjunction(scip) );

   /*******************
    * Comand line arguments *
    *******************/

   if( argc < 3 )
   {
      SCIPinfoMessage(scip, NULL, "usage: %s <prob_file> <settings_file> [sol_file] [objlimit] [scip_seed] [permute_seed]\n", argv[0]);
      SCIPinfoMessage(scip, NULL, "  (use 'none' for sol_file or a large number for objlimit if not needed)\n");
      return SCIP_OKAY;
   }

   const char* solfilename = NULL;
   double objlimit = SCIP_DEFAULT_INFINITY;
   int scipseed = 0;
   int permuteseed = 0;

   if (argc >= 4 && strcmp(argv[3], "none") != 0)
   {
      solfilename = argv[3];
   }

   if (argc >= 5)
   {
      char* endptr;
      objlimit = strtod(argv[4], &endptr);
      if (*endptr != '\0' && *endptr != '\n')
      {
         /* if the full string was not consumed, it's not a valid number, so reset to default */
         objlimit = SCIP_DEFAULT_INFINITY;
      }
   }

   scipseed = (argc >= 6) ? std::atoi(argv[5]) : 0;
   permuteseed = (argc >= 7) ? std::atoi(argv[6]) : -1;

   SCIP_CALL( fromCommandLine(scip, argv[1], argv[2], solfilename, objlimit, scipseed, permuteseed) );

   /*************
    * Solving *
    *************/

   /* solve problem */
   SCIP_CALL( SCIPsolve(scip) );

   /**************
    * Statistics *
    **************/

   /* print best solution */
   SCIP_CALL( SCIPprintBestSol(scip, NULL, FALSE) );

   /* print statistics */
   SCIP_CALL( SCIPprintStatistics(scip, NULL) );

   /********************
    * Deinitialization *
    ********************/

   SCIP_CALL( SCIPfree(&scip) );

   BMScheckEmptyMemory();

   return SCIP_OKAY;
}

/** main method starting SCIP */
int main(
        int                        argc,          /**< number of arguments from the shell */
        char**                     argv           /**< array of shell arguments */
)
{
   SCIP_RETCODE retcode;
   retcode = runSCIP(argc, argv);
   if( retcode != SCIP_OKAY )
   {
      SCIPprintError(retcode);
      return -1;
   }

   return 0;
}