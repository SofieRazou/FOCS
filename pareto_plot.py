import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# ------------------------
# 1. Load CSV
# ------------------------
csv_file = "gcd_times.csv"  # update with your file path
df = pd.read_csv(csv_file)

# Strip any whitespace from column names
df.columns = df.columns.str.strip()

# Detect Euclid and Binary columns automatically
euclid_col = [c for c in df.columns if "euclid" in c.lower()]
binary_col = [c for c in df.columns if "binary" in c.lower()]

if not euclid_col or not binary_col:
    raise ValueError("Could not find 'euclid' or 'binary' columns in CSV!")

euclid_col = euclid_col[0]
binary_col = binary_col[0]

print(f"Using columns -> Euclid: {euclid_col}, Binary: {binary_col}")

# ------------------------
# 2. Add number sizes (digits)
# ------------------------
# If your CSV has the numbers stored as strings in columns "num1","num2"
if 'num1' in df.columns and 'num2' in df.columns:
    df['size'] = df['num1'].astype(str).str.len() + df['num2'].astype(str).str.len()
else:
    # fallback: just use row index as proxy
    df['size'] = np.arange(1, len(df)+1)

# ------------------------
# 3. Sort by number size (ascending)
# ------------------------
df = df.sort_values(by='size').reset_index(drop=True)

# ------------------------
# 4. Plot runtime per test case
# ------------------------
plt.figure(figsize=(12,6))
plt.plot(df[euclid_col], label='Euclid', marker='o', markersize=4)
plt.plot(df[binary_col], label='Binary GCD', marker='x', markersize=4)

optimal = np.where(df[euclid_col] < df[binary_col], 'Euclid', 'Binary')
colors = np.where(optimal == 'Euclid', 'blue', 'orange')
plt.scatter(range(len(df)), np.minimum(df[euclid_col], df[binary_col]), c=colors, label='Optimal', s=50, alpha=0.5)

plt.xlabel('Test case (sorted by number size)')
plt.ylabel('Time (µs)')
plt.title('GCD runtime per test case with optimal algorithm')
plt.legend()
plt.grid(True)
plt.show()

# ------------------------
# 5. Runtime vs. number size
# ------------------------
df['size_bin'] = pd.cut(df['size'], bins=20)

grouped = df.groupby('size_bin').mean(numeric_only=True)

plt.figure(figsize=(12,6))
plt.plot(grouped.index.astype(str), grouped[euclid_col], 'o-', label='Euclid')
plt.plot(grouped.index.astype(str), grouped[binary_col], 'x-', label='Binary GCD')
plt.xticks(rotation=45)
plt.xlabel('Bit-size bin')
plt.ylabel('Average time (µs)')
plt.title('Average GCD runtime vs number size')
plt.legend()
plt.grid(True)
plt.show()


# ------------------------
# 6. Pareto-style total time
# ------------------------
total_time = pd.Series({
    'Euclid': df[euclid_col].sum(),
    'Binary GCD': df[binary_col].sum()
}).sort_values(ascending=False)

plt.figure(figsize=(6,4))
plt.bar(total_time.index, total_time.values, color=['blue','orange'])
plt.ylabel('Total time (µs)')
plt.title('Pareto-style total runtime')
plt.show()

# ------------------------
# 7. Pareto with cumulative %
# ------------------------
cumulative = total_time.cumsum() / total_time.sum() * 100
plt.figure(figsize=(6,4))
plt.bar(total_time.index, total_time.values, color=['blue','orange'])
plt.plot(total_time.index, cumulative, color='red', marker='o')
plt.ylabel('Total time (µs)')
plt.title('Pareto with cumulative %')
plt.show()

# ------------------------
# 8. Fraction of test cases where each algorithm is optimal
# ------------------------
frac_euclid = (optimal == 'Euclid').mean() * 100
frac_binary = (optimal == 'Binary').mean() * 100
print(f"Fraction of test cases where Euclid is faster: {frac_euclid:.2f}%")
print(f"Fraction of test cases where Binary GCD is faster: {frac_binary:.2f}%")
