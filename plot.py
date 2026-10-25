import csv 
import matplotlib.pyplot as plt

rows = []
with open("trace.csv") as f:
    for row in csv.DictReader(f):
        if row["test"] == "CCRb" and row["start_gap"] == "12" and row["lead_decel"] == "-6":
            rows.append(row)

t = [float(r["t"]) for r in rows]
ego = [float(r["ego_v"])*3.6 for r in rows]   # m/s -> km/h
lead = [float(r["lead_v"])*3.6 for r in rows]
gap = [float(r["gap"]) for r in rows]
state = [int(r["state"]) for r in rows]

fig, (ax1, ax2, ax3) = plt.subplots(3, 1, sharex=True, figsize = (8, 7))

ax1.plot(t, ego, label = "Ego car")
ax1.plot (t, lead, label = "Car ahead")
ax1.set_ylabel("Speed [km/h]")
ax1.legend()

ax2.plot(t, gap)
ax2.set_ylabel("Gap [m]")

ax3.step(t, state, where="post")
ax3.set_yticks([0, 1, 2, 3], ["Idle", "Warning", "Partial", "Full"])
ax3.set_xlabel("Time [s]")
ax3.set_xlim(0, 8)
fig.suptitle("CCRb: car ahead brakes at -6 m/s², 12 m gap, 50 km/h")
fig.tight_layout()
fig.savefig("docs/aeb_ccrb.png", dpi=120)
print("saved", len(rows), "rows")
