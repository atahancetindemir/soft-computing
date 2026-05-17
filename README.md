# Soft Computing

Implementations of two classic soft computing algorithms in C, written from scratch as a learning exercise.

## What's inside

### Genetic Algorithm
Evolves a population of binary chromosomes over generations to maximize a fitness function (`255x - x²`). Uses roulette wheel selection, one-point crossover, and bit-flip mutation. Outputs per-generation stats to CSV and plots them with Python/matplotlib.

**Parameters:** 20 chromosomes · 8-bit genome · 100 generations · 70% crossover rate · 0.1% mutation rate (adjustable at the top of `main()`)

### Simulated Annealing
Minimizes a 2-variable function by wandering through the solution space and occasionally accepting worse solutions. The temperature controls how reckless it's willing to be. As it cools, it settles into a minimum.

**Parameters:** Metropolis criterion · geometric cooling (c = 0.9) · Boltzmann constant k = 1 (adjustable at the top of `main()`)

## Structure

```
genetic-algorithm/
  src/genetic_algorithm.c   # core algorithm
  src/visualise.py          # plots output CSV
  output/                   # generated CSV and plot

simulated-annealing/
  src/simulated_annealing.c # core algorithm
```

## Building & Running

**Genetic Algorithm** requires Python dependencies for the visualiser:

```bash
pip install matplotlib pandas
```

The binary must run from `genetic-algorithm/src/` because output paths are relative to there:

```bash
gcc genetic-algorithm/src/genetic_algorithm.c -o genetic-algorithm/src/ga -lm
cd genetic-algorithm/src
./ga                        # writes output/output.csv
python3 visualise.py        # reads output/output.csv, writes output/plot.png
```

**Simulated Annealing** just prints to stdout:

```bash
gcc simulated-annealing/src/simulated_annealing.c -o sa -lm
./sa
```