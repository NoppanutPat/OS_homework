import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

dt = pd.read_csv("time.csv")

dtime = dt["TIME"]
dthread = dt["NTHREAD"]

dtime = np.array(dtime)
dthread = np.array(dthread)

print(dtime)
print(dthread)

plt.figure()

plt.plot(dthread,dtime)
plt.xlabel('Number of thread')
plt.xticks(dthread)
plt.ylabel('time (s)')
plt.scatter(dthread,dtime)
plt.show()
