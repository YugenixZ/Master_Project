# Install script for directory: /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/gcg" TYPE FILE FILES
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/branch_empty.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/branch_bpstrong.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/branch_generic.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/branch_orig.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/branch_relpsprob.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/branch_ryanfoster.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/class_conspartition.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/class_indexpartition.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/miscvisualization.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/class_pricingcontroller.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/class_pricingtype.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/class_partialdecomp.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/class_detprobdata.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/class_stabilization.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/class_varpartition.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/clscons.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/clsvar.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/colpool.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/cons_decomp.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/cons_decomp.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/cons_integralorig.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/cons_masterbranch.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/cons_origbranch.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_compgreedily.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_connected_noNewLinkingVars.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_connectedbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_consclass.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_constype.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_dbscan.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_densemasterconss.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_generalmastersetcover.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_generalmastersetpack.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_generalmastersetpart.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_mastersetcover.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_mastersetpack.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_mastersetpart.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_mst.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_neighborhoodmaster.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_postprocess.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_staircase_lsp.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_stairheur.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_varclass.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/decomp.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/def.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dialog_gcg.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dialog_graph.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dialog_master.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/disp_gcg.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/disp_master.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/event_bestsol.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/event_display.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/event_mastersol.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/event_relaxsol.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/event_solvingstats.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/gcg.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/gcgcol.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/gcg_general.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/gcggithash.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/gcgplugins.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/gcgpqueue.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/gcgsort.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgcoefdiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgdins.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgfeaspump.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgfracdiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgguideddiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcglinesdiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgpscostdiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgrens.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgrins.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgrounding.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgshifting.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgsimplerounding.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgveclendiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_gcgzirounding.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_greedycolsel.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_mastercoefdiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_masterdiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_masterfracdiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_masterlinesdiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_mastervecldiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_origdiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_relaxcolsel.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_restmaster.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_setcover.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_xpcrossover.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/heur_xprins.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/masterplugins.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/nodesel_master.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/objdialog.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/objpricer_gcg.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/params_visu.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/presol_roundbound.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pricer_gcg.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pricestore_gcg.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pricingjob.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pricingprob.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_clscons.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_clsvar.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_colpool.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_decomp.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_gcgcol.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_gcgheur.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_gcgsepa.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_gcgpqueue.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_gcgvar.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_pricingjob.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_pricingprob.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_score.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/pub_solver.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/reader_blk.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/reader_cls.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/reader_dec.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/reader_gp.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/reader_ref.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/reader_tex.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/relax_gcg.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/scip_misc.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/score_bender.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/score_border.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/score_classic.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/score_fawh.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/score_forswh.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/score_maxwhite.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/score_spfawh.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/score_spfwh.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/score_strong.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/score.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/sepa_basis.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/sepa_master.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/solver.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/solver_cliquer.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/solver_knapsack.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/solver_mip.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/solver_xyz.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/stat.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/struct_branchgcg.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/struct_colpool.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/struct_decomp.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/struct_detector.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/struct_gcgcol.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/struct_gcgpqueue.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/struct_pricestore_gcg.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/struct_pricingjob.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/struct_pricingprob.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/struct_score.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/struct_solver.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/struct_vardata.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_branchgcg.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_classifier.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_colpool.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_consclassifier.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_decomp.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_detector.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_gcgcol.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_gcgpqueue.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_masterdiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_origdiving.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_parameter.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_pricestore_gcg.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_pricingjob.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_pricingprob.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_pricingstatus.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_score.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_solver.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/type_varclassifier.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/wrapper_partialdecomp.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/symmetry/automorphism.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/symmetry/automorphism.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/symmetry/pub_automorphism.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/gcg/dec_isomorph.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/graph" TYPE FILE FILES
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/bipartitegraph.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/bipartitegraph_def.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/bridge.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/columngraph.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/columngraph_def.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/graph.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/graph_def.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/graph_gcg.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/graph_interface.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/graph_tclique.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/graphalgorithms.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/graphalgorithms_def.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/hypercolgraph.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/hypercolgraph_def.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/hypergraph.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/hypergraph_def.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/hyperrowcolgraph.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/hyperrowcolgraph_def.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/hyperrowgraph.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/hyperrowgraph_def.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/matrixgraph.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/matrixgraph_def.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/rowgraph.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/rowgraph_def.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src/graph/weights.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gcg" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gcg")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gcg"
         RPATH "/usr/local/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/bin/gcg")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gcg" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gcg")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gcg"
         OLD_RPATH "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/lib:"
         NEW_RPATH "/usr/local/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gcg")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgcg.so.3.7.2.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgcg.so.3.7"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/lib/libgcg.so.3.7.2.0"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/lib/libgcg.so.3.7"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgcg.so.3.7.2.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgcg.so.3.7"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/lib:"
           NEW_RPATH "")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgcg.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgcg.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgcg.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/lib/libgcg.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgcg.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgcg.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgcg.so"
         OLD_RPATH "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgcg.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/gcg/gcg-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/gcg/gcg-targets.cmake"
         "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/gcg/src/CMakeFiles/Export/55a8c2330c2af6b35ecce0f90032ca65/gcg-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/gcg/gcg-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/gcg/gcg-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/gcg" TYPE FILE FILES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/gcg/src/CMakeFiles/Export/55a8c2330c2af6b35ecce0f90032ca65/gcg-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/gcg" TYPE FILE FILES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/gcg/src/CMakeFiles/Export/55a8c2330c2af6b35ecce0f90032ca65/gcg-targets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/gcg" TYPE FILE FILES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/gcg/CMakeFiles/gcg-config.cmake")
endif()

