# JUMP
This is the code repo for the paper:
JUMP: A Fast Deterministic Algorithm to Find the Closest Pair of Subsequences
( accepted in SIAM International Conference on Data Mining 2018 (SDM18) )

----------------------------------------
JUMP.cpp: the code for proposed algorithm
N2Alg.cpp: the code for N2Alg used in the paper
N2Alg_top20.cpp: the code finding top20 using N2Alg algorithm
household_power_consumption_globalactive.txt: the sample datafile

To use, pass datafile, total length N, and l-mer length l, as three arguments and call the following:
./JUMP [datafile.txt] [N] [l]
./N2Alg [datafile.txt] [N] [l]
