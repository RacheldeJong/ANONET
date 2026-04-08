# ANO-NET
This repository contains code to measure anonymity and improve anonymity in networks using multiple anonymity measures and heuristic anonymization algorithms.
The repository accompanies the following research papers: [1, 2].

# What this code does:
* Measure (fuzzy-)$k$-anonymity in networks.
* Perform anonymity-cascade (described in [6]).
* Apply anonymization algorithms.

# How to run

## Installation
Before using this code, the nauty framework [7] should be downloaded from: https://users.cecs.anu.edu.au/~bdm/nauty/

Clone this repository (ANONET) in the downloaded nauty directory (nauty27r3).
To install Nauty run `./configure` and `./make` in the folder.

`Note:` This code is last tested with nauty version `nauty27r3`.

## Compilation and run code
To compile the code, navigate to the `ANONET` folder and use:
```
make
```

To run the code, at least one input graph should be given. Command line arguments are optional. Input graphs should be in Nauty format (.nauty files).
```
./bin/anonet [input] [-arguments]
```



## Command line arguments
Various command line arguments can be used to indicate what to run, and to adjust the settings of 1) the anonymity measure, 2) anonymization algorithm, and 3) cascading.
Most arguments are optional and have sensible defaults — the code can be run with default settings.

### Quick start


### 1. Assessing anonymity
Running the following command:
```
./bin/anonet ./examples/example4 [-arguments]
```

produces the following output:
```
|V|6 |E|: 8
Equivalence partition info: 
        Used measure: Count
        Maximum distance: 1
        Heuristic used: Degree
        Equivalence partition computed: true
[2, 2], [4, 4], 
Fraction unique: 0

```
This includes information on the graph, the settings used to compute the equivalence classes and finally the size of the resulting equivalence classes and uniqueness (fraction of unique nodes in the network).
Where [X,Y] indicates that there are X nodes in an equivalence class of size Y.

Optional output:
Adding the argument `-ep` prints nodes in each equivalence class. Each line denotes a new equivalence class.

```
Start equivalence classes:
0, 2, 3, 5, 
1, 4, 
End equivalence classes.
```

### 2. Fuzzy equivalence class.
To compute fuzzy-k-anonymity, add argument `-f [val]` with percentage in range `0-100`.
The following runs fuzzy-k-anonymity while allowing for 100% difference.
```
./bin/anonet ./examples/example4 -f 100 -ep
```
This results in the following output. Note: since the fuzzy measure for anonymity is not symmetric, this mode prints for each node to which set of nodes it is phi-similar.
> **Warning:** fuzziness is currently only implemented for the count measure. Running fuzzy with a different measure results in an error.
```
Equivalence partition info: 
        Used measure: Count
        Maximum distance: 1
        Heuristic used: Degree
        Equivalence partition computed: true
Fuzzy similarity info: 
        Phi value: 100%
[6,6],
0: 1, 2, 3, 4, 5, 
1: 0, 2, 3, 4, 5, 
2: 0, 1, 3, 4, 5, 
3: 0, 1, 2, 4, 5, 
4: 0, 1, 2, 3, 5, 
5: 0, 1, 2, 3, 4, 
```


### 3. Anonymity cascade
For cascading run:
```
./bin/anonet ./examples/example4 -C
```
Generates the following output.
```
Cascade info
        Starting measure: Count
        Cascade measure: Count
        Maximum cascade distance: -1
Cascading results:
Unique nodes per level:
Unique nodes level 0: 0
Unique nodes total: 0
```

### 4. Budgeted anonymization: run until a budget of $B$ edges is deleted:
By default, 5% of edges are deleted. Anonymity and node signatures are recomputed periodically (default: every 10% of the budget). Note: the recompute gap may affect the anonymization algorithm.
```
./bin/anonet ./datasets/sms.nauty -A 
```
This gives the following output. For each iteration, it prints statistics for anonymization. Adding the argument `-edges` additionally prints the edges that are deleted.
`Alter, source, target, 0`. 0 indicates that the edge is deleted.

```
Altered, uniqueness, twin-uniqueness, fuzzy-uniqueness, runtime, runtime-alg, runtime-update, edges_altered, perc_edges_altered
0, 0.0264085, 0.0299296, 0.0264085, 0, 0, 0, 0, 0, 
7, 0.0246479, 0.028169, 0.0246479, 0.006915, 0.000707, 1.85077, 7, 0.00502152, 
14, 0.0158451, 0.0193662, 0.0158451, 0.012099, 0.001165, 1.85549, 14, 0.010043, 
21, 0.0228873, 0.0264085, 0.0228873, 0.018062, 0.001938, 1.86068, 21, 0.0150646, 
...
```

### 5. Full anonymization, run until all nodes are $k$-anonymous:
Set the budget to 100 to run full anonymization. ANO-NET automatically stops when the network is fully anonymized, or all edges are deleted.
```
./bin/anonet ./datasets/sms.nauty -A -b 100 
```

### Main modes
| Flag | Description |
|------|-------------|
| `[file]` | Input file (required) |
| (none) | Equivalence partition of nodes |
| `-A` | Perform anonymization |
| `-C` | Perform anonymity-cascade |
| `-M` | Menu mode |

### General settings
| Flag | Description | Default |
|------|-------------|---------|
| `-d [arg]` | Set distance | 1 |
| `-k [arg]` | Set value for `k` (anonymous node threshold) | 2 |
| `-f [arg]` | Set level of uncertainty threshold [0-100] (for fuzzy-k-anonymity) | 0 |
| `-t [arg]` | Use anonymity (`0`) or anonymity%twinnodes (`1`) | 0 |

### Output settings
| Flag | Description | Default |
|------|-------------|---------|
| `-s [arg]` | Statistics mode (see below) | 0 |
| `-ep` | Print nodes per equivalence class / fuzzy: node and phi-similar nodes | off |
| `-edges` | Print deleted edges (anonymization only) | off |

**Statistics mode options:**
- `0` Print main result: anonymity distribution, cascade statistics or anonymization results
- `1` Additionally print anonymity distribution derived from the original equivalence partition
- `2` Additionally print info of the equivalence partition, and cascade or anonymization step


### Anonymity measures
| Flag | Description | Default |
|------|-------------|---------|
| `-me [arg]` | Main measure | 2 (count) |
| `-mh [arg]` | Heuristic measure | 1 (degree) |
| `-it [arg]` | Iterative computation (0 to turn off)| 1 |

> **Note:** a warning will be given if the chosen heuristic may influence the obtained 
> equivalence partition

### Anonymity cascade (`-C`)
| Flag | Description | Default |
|------|-------------|---------|
| `-me2 [arg]` | Cascade measure | 2 (count) |
| `-mh2 [arg]` | Cascade measure heuristic | 1 (degree) |

> **Note:** a warning will be given if the chosen heuristic may influence the obtained 
> equivalence partition

### Anonymization algorithm (`-A`)
| Flag | Description | Default |
|------|-------------|---------|
| `-a [arg]` | Anonymization algorithm | 0 (Edge sampling) |
| `-b [arg]` | Budget $[0-\|E	\|]$ | $\lceil 0.05|E| \rceil $ |
| `-u [arg]` | Recompute gap | -1 (0.05 × budget) |

Anonymity and node signatures are recomputed periodically (default: every 10% of the budget).

**Anonymization algorithm options:**
- `0` **Edge sampling** - Select each edge with the same probability
- `1` **Unique** - Select from set of edges connected to at least one unique node first. Otherwise select randomly.
- `2` **Degree** - Select preferential based on degree of nodes connected
- `3` **Affected** - Select preferential to the number of nodes affected by deleting the edge.
- `4` **Affected unique** - Select preferential based on number of unique nodes affected by edge deletion.
- `5` **Affected unique only** - Combines unique and affected unique. Select unique edges preferential on number of unique nodes affected. Otherwise select randomly.


### Measure options (for `-me`, `-mh`, `-me2`, `-mh2`)
| Value | Measure |
|-------|---------|
| 0 | None |
| 1 | Degree |
| 2 | Count |
| 3 | DegDist |
| 4 | d-k-anonymity |
| 5 | VRQ |
| 6 | Hybrid |
| 7 | EdgeCount |
</details>

## Menu
<details>
<summary> Menu mode</summary>

Run in menu mode with:
```bash
./bin/anonet -M
```

**Reading**
| Key | Description |
|-----|-------------|
| `I` | Read input graph |
| `P` | Print the input graph |
| `D` | Set statistics mode (0, 1, 2) |
| `e` | Exit program |

**Measure anonymity**
| Key | Description |
|-----|-------------|
| `E` | Compute partition (graph should be read first) |
| `C` | Perform anonymity-cascade (partition should be computed first) |

**Anonymization**
| Key | Description |
|-----|-------------|
| `A` | Perform anonymization (partition should be computed first) |

**Settings**
| Key | Description |
|-----|-------------|
| `OE` | Set anonymity measure and heuristic, distance, twin node options |
| `OC` | Set cascade measure and heuristic |
| `OA` | Set anonymization algorithm, budget, and recompute gap |

> For detailed settings options see the [command line arguments](#command-line-arguments) section — the same options apply.

</details>



# Citing ANO-NET
To cite ANO-NET, please use the following reference: 

 de Jong, Rachel G., Mark P. J. van der Loo, and Frank W. Takes. "The anonymization problem in social networks." arXiv preprint arXiv:2409.16163
 (2024). doi: https://doi.org/10.48550/arXiv.2409.16163 \
 
@article{dejong2024anonymization,
&nbsp;&nbsp;&nbsp;&nbsp;title={The anonymization problem in social networks},\
&nbsp;&nbsp;&nbsp;&nbsp;author={de Jong, Rachel G and van der Loo, Mark PJ and Takes, Frank W},\
&nbsp;&nbsp;&nbsp;&nbsp;journal={arXiv preprint arXiv:2409.16163},\
&nbsp;&nbsp;&nbsp;&nbsp;year={2024}
}


# References
[1] de Jong, Rachel G., Mark P. J. van der Loo, and Frank W. Takes. "The anonymization problem in social networks." Accepted at the 17th Workshop on Modelling and Mining Networks
 (2026). ArXiv preprint: https://doi.org/10.48550/arXiv.2409.16163 \
[2] de Jong, Rachel G., Mark P. J. van der Loo, and Frank W. Takes. "A systematic comparison of measures for k-anonymity in networks." arXiv preprint arXiv:2407.02290 (2024). doi:  	
https://doi.org/10.48550/arXiv.2407.02290 \
[3] Liu, Kun, and Evimaria Terzi. "Towards identity anonymization on graphs." Proceedings of the 2008 ACM SIGMOD international conference on Management of data (2008). \
[4] de Jong, Rachel G., Mark P. J. van der Loo, and Frank W. Takes. "Algorithms for Efficiently Computing Structural Anonymity in Complex Networks." ACM Journal of Experimental Algorithmics (2023).  \
[5] Hay, Michael, et al. "Anonymizing social networks." Computer science department faculty publication series (2007).\
[6] de Jong, Rachel G., Mark P. J. van der Loo, and Frank W. Takes. "The effect of distant connections on node anonymity in complex networks." Scientific Reports 14.1 (2024): 1156. \
[7] B. D. McKay and A. Piperno, "Practical graph isomorphism", Journal of Symbolic Computation, vol. 60, no. 0, pp. 94–112 (2014). 
