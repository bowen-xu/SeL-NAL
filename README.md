# SeL-NAL

This repo contains the source code of the paper *A Brain-inspired Sequential Learning Model based on Non-Axiomatic Logic*.

## Compilation

This project is python-C++ mixed.
The C++ code is compiled by `g++23`, older C++ standard (e.g., `g++11`) and other compilers (e.g., `clang`) are not ensured to work.
There would be two targets folders, `./narsese` and `./SequentialGroup`, which are also two python modules.

## Run

To test the capacity of the model, run the command
```
python -m test_capacity
```

To test the catastrophic forgetting phenomenon of the model, run the command
```
python -m test_catastrophic_forgetting
```

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
