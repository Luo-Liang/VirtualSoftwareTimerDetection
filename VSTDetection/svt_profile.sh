#!/bin/bash
amplxe-cl -collect-with runsa -knob event-config=\
MEM_LOAD_UOPS_L3_HIT_RETIRED.XSNP_HITM_PS,\
INST_RETIRED.ANY,\
CPU_CLK_UNHALTED.THREAD,\
DTLB_STORE_MISSES.MISS_CAUSES_A_WALK,\
ICACHE.HIT,\
ICACHE.MISSES,\
ITLB_MISSES.MISS_CAUSES_A_WALK,\
BR_INST_EXEC.ALL_BRANCHES,\
BR_MISP_EXEC.ALL_BRANCHES,\
BR_INST_RETIRED.ALL_BRANCHES,\
MEM_UOPS_RETIRED.STLB_MISS_LOADS,\
MEM_UOPS_RETIRED.STLB_MISS_STORES,\
MEM_UOPS_RETIRED.ALL_LOADS,\
MEM_UOPS_RETIRED.ALL_STORES,\
MEM_LOAD_UOPS_RETIRED.L1_HIT,\
MEM_LOAD_UOPS_RETIRED.L2_HIT,\
MEM_LOAD_UOPS_RETIRED.L3_HIT,\
MEM_LOAD_UOPS_RETIRED.L1_MISS,\
MEM_LOAD_UOPS_RETIRED.L2_MISS,\
MEM_LOAD_UOPS_RETIRED.L3_MISS,\
DTLB_LOAD_MISSES.WALK_COMPLETED,\
DTLB_LOAD_MISSES.STLB_HIT,\
DTLB_STORE_MISSES.WALK_COMPLETED,\
DTLB_STORE_MISSES.STLB_HIT,\
ITLB_MISSES.WALK_COMPLETED,\
ITLB_MISSES.STLB_HIT,\
-- ./software_vt