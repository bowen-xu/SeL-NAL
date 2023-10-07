# SeL-NAL

This repo contains the source code of the paper *A Brain-inspired Sequential Learning Model based on Non-Axiomatic Logic*.

## Compilation

This project is python-C++ mixed.
The C++ code is compiled by `g++23`; older C++ standards (*e.g.*, `g++11`) and other compilers (*e.g.*, `clang`) are not ensured to work.

To compile the C++ code, run the commands as the following
```
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make
```
There would be two target folders, `./narsese` and `./SequentialGroup`, which are also two python modules.

## Run

To test the capacity of the model, run the command
```
python -m test_capacity
```

To test the catastrophic forgetting phenomenon of the model, run the command
```
python -m test_catastrophic_forgetting
```

Run the jupyter-notebook `tests.ipynb` to get all related figures from tests.

## Debug

Python-C++ mixed debugging is avaiable in VS-Code. To do so, the extension `Python C++ Debugger` is needed. Select the task "Python C++ Mixed" in `.vscode/launch.json`, and run and debug.

To enter a C++ file, a break point should be set up before debugging.

## Performance

### Capacity

![](Figures/Capacity/svgs/Accuracy-length=5-n_patterns=5-n_types=26-n_nodes=10.svg)
length=5, n_patterns=5, n_types=26

![](Figures/Capacity/svgs/Accuracy-length=14-n_patterns=20-n_types=26-n_nodes=10.svg)
length=14, n_patterns=20, n_types=26

![](Figures/Capacity/svgs/Accuracy-length=14-n_patterns=20-n_types=1000-n_nodes=10.svg)
length=14, n_patterns=20, n_types=1000

## Cite

bibtex:
TBA
<!-- ```bibtex
@software{Lisa_My_Research_Software_2017,
  author = {Lisa, Mona and Bot, Hew},
  doi = {10.5281/zenodo.1234},
  month = {12},
  title = {{My Research Software}},
  url = {https://github.com/github-linguist/linguist},
  version = {2.0.4},
  year = {2017}
}
``` -->
