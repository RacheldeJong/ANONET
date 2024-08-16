# ANONET
This repository contains code measuring anonymity with various anonymity measrues and anonymizing networks with various heuristic algorithms.
The repository accompanies the following research paper: [1].

# Measures:
Nodes are equivalent if the following is equal:
* `Degree`: the number of connections of the node [2]
* `Count`: the number of nodes and edges in the d-neighborhood of the node [1] 
* `Degree distribution`: the degree distribution in the d-neighborhood of the node [1]
* `d-k-Anonymity`: the structure of the $d$-neighborhood [3] (based on isomorphism)
* `vrq`: the degree distribution over all nodes at distance d of a node [4]

`Note:` each of these measures can be used as a heuristic for computing anonymity.

# Anonymity-cascade
Uniqueness and twin-uniqueness for anonymity-cascade as described in [5] can be computed with this framework.

# Anonymization algorithms
Anonymization can be done by deleting edges. 
The framework is implemented such that anonymity of nodes can be efficiently updated after edges are altered.

The following anonymization algorithms are implemented:
* `Random` or `Edge sampling`: Select each edge with the same probability.
* `Degree`: Select preferential based on degree of nodes connected:
* `Affected`: Select preferential to the number of nodes affected by deleting the edge.
* `Unique`: Select from set of edges connected to at least one unique node first. Otherwise select randomly.
* `Affected unique`: Select preferential based on number of unique nodes affected by edge deletion.
* `Affected unique only`: Combines unique and affected unique. Select unique edges preferential on number of unique nodes affected. Otherwise select randomly.



# How to run

## Installation
Before using this code, the nauty framework [6] should be downloaded from: https://users.cecs.anu.edu.au/~bdm/nauty/

Move this git-directory (ANONET) in the downloaded nauty directory (nauty27r3).
To install Nauty run `./configure` and `./make` in the folder.

`Note:` This code is last tested with nauty version `nauty27r3`.

## Compilation and run code
To compile the code use:
```
make
```

To run the code, at least one input graph should be given. Command line arguments are optional.
Use the following command to run the code:
```
./bin/anonet ./examples/example2 [-arguments]
```
For example:
```
./bin/anonet ./examples/example2 -d 2
```

## Menu
To run the code in menu mode, run
For example:
```
./bin/anonet -M
```
You can choose from the following options:
* `I`: Read input graph
* `E`: Compute partition (graph should be read)
* `C`: Perform anonymity-cascade (partition should be computed first)
* `A`: Perform anonymization (partition should be computed first)
* `P`: Print the input graph
* `OE`: Set anonymity measure related options:
	* Anonymity measure and heuristic: 
	[0 `None`, 1 `DEGREE`, 2 `COUNT`, 3 `DEGDIST`,  4 `d-k-ANONYMITY`, 5 `VRQ`, 6 `HYBRID`, 7 `EDGECOUNT`] (default 4, d-k-ANONYMITY)
	* `Note:` a warning will be given if the chosen heuristic may influence the obtained equivalence partition
	* Distance (default 1)
	* Option to not of count twin nodes in the anonymity results (default 0: take into account)
* `OC`: Set cascade measure and heuristic for computing partition
	* Anonymity measure and heuristic: 
	[0 `None`, 1 `DEGREE`, 2 `COUNT`, 3 `DEGDIST`,  4 `d-k-ANONYMITY`, 5 `VRQ`, 6 `HYBRID`, 7 `EDGECOUNT`] (default 4, d-k-ANONYMITY)
	* `Note:` a warning will be given if the chosen heuristic may influence the obtained equivalence partition
* `OA`: Set anonymization algorithm related options:
	* Algorithm: [0 `Edge sampling`, 1 `Unique`,  2`Degree`, 3  `Affected`, 4 `Affected unique`, 5 `Affected unique only`] (default 0: random)
	* Budget (for default, use -1: 0.05 * |E|)
	* Recompute gap (for default, use -1: 0.01 * budget)
* `D`: Set statistic mode [0, 1, 2]
	* 0 Print main result: anonymity distribution, cascade statistics or anonymization resutls (default)
	* 1 Additionally print anonymity distribution derived from the original equivalence partition
	* 2 Additionally print info of the equivalence partition, and cascade or anonymization step
* `e`: exit program

## Command line arguments
Various command line arguments can be used to indicate what to run, and to adjust the settings of the algorithm.

* What to run:
	* None: Equivalence class partition (default)
	* `-M`: Menu mode
	* `-C`: Perform anonymity-cascade
	* `-A`: Perform anonymization
	* `-i`: Give input file
* General settings:
	* `-d [d]`: set distance to arg
	* `-k [d]`: set value for `k`, when a node is anonymous (default 2)
	* `-t [t]`: set use of anonymity or anonymity%twinnodes (default 0, anonymity)
	* `-s [arg]`: set statistics mode
		* 0 Print main result: anonymity distribution, cascade statistics or anonymization resutls (default)
		* 1 Additionally print anonymity distribution derived from the original equivalence partition
		* 2    Additionally print info of the equivalence partition, and cascade or anonymization step
* Anonymity measures:
	* `-me [arg]`: set main measure (default d-k-anonymity)
	* `-mh [arg]`: set heuristic measur (default Degree)
		* Options: [0 `None`, 1 `DEGREE`, 2 `COUNT`, 3 `DEGDIST`,  4 `d-k-ANONYMITY`, 5 `VRQ`, 6 `HYBRID`, 7 `EDGECOUNT`]
		* `Note:` a warning will be given if the chosen heuristic may influence the obtained equivalence partition
	* `-it [arg]`: set iterative (default true)
* Anonymity-cascade
	* `me2 [arg]`: set cascade measure (default d-k-anonymity)
	* `me2 [arg]`: set cascade measure heuristic (default d-k-anonymity)
		* Options: [0 `None`, 1 `DEGREE`, 2 `COUNT`, 3 `DEGDIST`,  4 `d-k-ANONYMITY`, 5 `VRQ`, 6 `HYBRID`, 7 `EDGECOUNT`]
		* `Note:` a warning will be given if the chosen heuristic may influence the obtained equivalence partition
* Anonymization algorithm
	* `-a [arg]`: set anonymization algorithm (default 0: random)
		* [0 `Edge sampling`, 1 `Unique`,  2`Degree`, 3  `Affected`, 4 `Affected unique`, 5 `Affected unique only`]
	* `-b [arg]`: set budget to arg (default -1: 0.1*|E|)
	* `-u [arg]`: set recompute gap to arg (default -1: 0.1 * budget)

# References
[1] de Jong, Rachel G., Mark P. J. van der Loo, and Frank W. Takes. "A systematic comparison of measures for k-anonymity in networks." arXiv preprint arXiv:2407.02290 (2024). \
[2] Liu, Kun, and Evimaria Terzi. "Towards identity anonymization on graphs." Proceedings of the 2008 ACM SIGMOD international conference on Management of data (2008). \
[3] de Jong, Rachel G., Mark P. J. van der Loo, and Frank W. Takes. "Algorithms for Efficiently Computing Structural Anonymity in Complex Networks." ACM Journal of Experimental Algorithmics (2023).  \
[4] Hay, Michael, et al. "Anonymizing social networks." Computer science department faculty publication series (2007).\
[5] de Jong, Rachel G., Mark P. J. van der Loo, and Frank W. Takes. "The effect of distant connections on node anonymity in complex networks." Scientific Reports 14.1 (2024): 1156. \
[6] B. D. McKay and A. Piperno, "Practical graph isomorphism", Journal of Symbolic Computa-tion, vol. 60, no. 0, pp. 94–112 (2014). 