import os

import pandas as pd
import matplotlib.pyplot as plt

# Paths are relative to the project directory (genetic-algorithm/)
OUTPUT_DIR = "output"
INPUT_CSV = os.path.join(OUTPUT_DIR, "output.csv")
OUTPUT_PLOT = os.path.join(OUTPUT_DIR, "plot.png")

if not os.path.exists(INPUT_CSV):
    raise SystemExit(
        f"{INPUT_CSV} not found. Run ./bin/ga from the genetic-algorithm/ directory first."
    )

df = pd.read_csv(INPUT_CSV)

plt.figure(figsize=(10, 6))
plt.plot(df["Generation"], df["Average"], label="Average")
plt.plot(df["Generation"], df["Best"], label="Best")

plt.title("Results (Chromosome: 20, Generation: 100, Crossover: 0.7, Mutation: 0.001)")
plt.xlabel("Generation")
plt.ylabel("Fitness")
plt.legend(loc="lower right")
plt.grid(True)
plt.tight_layout()

os.makedirs(OUTPUT_DIR, exist_ok=True)
plt.savefig(OUTPUT_PLOT, dpi=150)

plt.show()
