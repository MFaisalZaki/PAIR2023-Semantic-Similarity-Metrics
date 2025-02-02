

# Semantic Similarity Metrics

This is the implementation of the paper
```
@InProceedings{mabdelwahed-pair-2023,
  title =        "Bridging the Gap between Structural and Semantic Similarity in Diverse Planning",
  author =       "Abdelwahed, Mustafa F. and Espasa Arxer, Joan and Toniolo, Alice and P. Gent, Ian",
  booktitle = "ICAPS 2023 Workshop on Human-Aware Explainable Planning",
  year =         "2023"
}
```

## How to use
We have mainted the same usage found in [Diverse-score](https://github.com/IBM/diversescore) while including two new metrics:
1. *compute_sgo_metric*
2. *compute_flex_metric*



# Diverse-score is a code that computes diversity score of a set of plans (or its subset) for a given metric.
## Implemented metrics are defined by:
1. Pairwise measure: stability, state, uniqueness (and their linear combination, obtained by specifying several measures). The parameters are (default values are *false*)
   1. *compute_stability_metric*
   2. *compute_states_metric*
   3. *compute_uniqueness_metric* 
2. Aggregation method over the pairs in the set of plans: average or minimum. The parameter is *aggregator_metric*, with values {*avg*, *min*}, default: *avg*. 
3. Treating plans as multisets or sets of actions (LPG-d planner treats plans as multisets of actions). The parameter is *plans_as_multisets* , default: *false*.

## Subset selection
Additional functionality allows for selecting a subset of plans of required size from a larger set of plans, with the aim of finding subsets with better scores. The code allows to post-process sets of plans to obtain solutions for **satisificing** and **bounded** diverse planning problems.

## Building
For building the code please use 
```
./build.py
```

**Note that the computation of bounded diversity score requires enabling CPLEX support in Fast Downward (see http://www.fast-downward.org/) and building the code with LP support.**


## Running

### Score of a given set
An example specification is provided in the script compute_stability.sh
```
# ./compute_stability.sh <domain> <problem> <plans-folder> <number-of-plans>
./compute_stability.sh domain.pddl problem.pddl found_plans 1000
```

Additional example specifications:
* *--diversity-score "score(compute_stability_metric=true, aggregator_metric=min, plans_as_multisets=true)"* <br/> for minimal pairwise stability as computed by LPG-d.
* *--diversity-score "score(compute_stability_metric=true, compute_states_metric=true, aggregator_metric=avg,plans_as_multisets=false)"* <br/> for average over linear combination of stability and state.

### Selecting subset for satisficing diverse planning
```
# ./compute_stability_subset.sh <domain> <problem> <plans-folder> <number-of-plans-in-folder> <number-of-plans-to-select> 
./compute_stability_subset.sh domain.pddl problem.pddl found_plans 1000 100
```
### Selecting subset for bounded diverse planning
```
# ./compute_stability_subset_bounded.sh <domain> <problem> <plans-folder> <number-of-plans-in-folder> <number-of-plans-to-select> <bound>
./compute_stability_subset_bounded.sh domain.pddl problem.pddl found_plans 1000 100 0.25
```

### Selecting subset for optimal diversity diverse planning
```
# ./compute_stability_subset_optimal.sh <domain> <problem> <plans-folder> <number-of-plans-in-folder> <number-of-plans-to-select> 
./compute_stability_subset_bounded.sh domain.pddl problem.pddl found_plans 1000 100 
```

### Selecting subset with a discounted prefix similarity 
```
--diversity-score "subset(compute_stability_metric=true,aggregator_metric=avg,plans_as_multisets=false,plans_subset_size=3,exact_method=false,dump_plans=true,discounted_prefixes=true, discount_factor=0.9)" --internal-plan-files-path plans --internal-num-plans-to-read 10
```

### Selecting subset with a seed set (first x plans)
```
--diversity-score "subset(compute_stability_metric=true,aggregator_metric=avg,plans_as_multisets=false,plans_subset_size=3,exact_method=false,dump_plans=true,plans_seed_set_size=2)" --internal-plan-files-path plans --internal-num-plans-to-read 10
```

### Selecting subset using label reduction
All labels that are not matched are skipped. Note the following options:
* reduce_skip_unmentioned (default true) -- If a label is not mentioned, skip it. If this value is false, unmentioned are mapped to themselves.
* labels_lifted (default true) -- Treat labels as lifted actions names. If this value is false, labels are treated as ground actions names.
```
--diversity-score "subset(compute_stability_metric=true,aggregator_metric=avg,plans_as_multisets=false,plans_subset_size=3,exact_method=false,dump_plans=true,label_reduction_file=lr.csv, reduce_labels=true)" --internal-plan-files-path plans --internal-num-plans-to-read 10
```

### Dump the found subset to JSON 
Add option ```json_file_to_dump=filename.json```

<br><br><br>

## Citing

```
@InProceedings{katz-sohrabi-aaai2020,
  title =        "Reshaping diverse planning",
  author =       "Michael Katz and Shirin Sohrabi",
  booktitle =    "Proceedings of the Thirty-Fourth {AAAI} Conference on
                  Artificial Intelligence ({AAAI} 2020)",
  publisher =    "{AAAI} Press",
  pages =        "9892--9899",
  year =         "2020"
}

@InProceedings{katz-et-al-aaai2022,
  title =        "Bounding Quality in Diverse Planning",
  author =       "Michael Katz and Shirin Sohrabi and Octavian Udrea",
  booktitle =    "Proceedings of the Thirty-Sixth {AAAI} Conference on
                  Artificial Intelligence ({AAAI} 2022)",
  publisher =    "{AAAI} Press",
  year =         "2022"
}
```

## Licensing

DiverseScore is a Automated PDDL planning tool for computing the score
for a set of plans under specified diversity metrics.
Copyright (C) 2019  Michael Katz, IBM Research, USA.

Diverse score computation is built on top of Fast Downward. 
The computation code is located in the following folder:
* ./src/search/diversity_score

In addition, the code modifies the following files of Fast Downward:

* driver/run_components.py
* src/search/DownwardFiles.cmake
* src/search/command_line.{cc,h}
* src/search/search_space.{cc,h}
* src/search/plan_manager.{cc,h}
* src/search/planner.cc

The license for the extension of Fast Downward code is specified in the LICENSE file.

## Fast Downward
Fast Downward is a domain-independent planning system.

For documentation and contact information see http://www.fast-downward.org/.

The following directories are not part of Fast Downward as covered by this
license:

* ./src/search/ext

For the rest, the following license applies:

```
Fast Downward is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

Fast Downward is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <http://www.gnu.org/licenses/>.
```
