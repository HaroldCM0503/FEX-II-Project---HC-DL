import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# --- Datos ---
posicion_cm = np.array([0, 7, 14, 21, 28])   
recibidos = np.array([1, 21, 39, 42, 50])
ESPERADOS = 50
DIST_TX_CM = 600                             

distancia_cm = DIST_TX_CM - posicion_cm

prr = recibidos / ESPERADOS

# --- Intervalo de confianza de Wilson (95 %) para una proporción ---
def wilson(k, n, z=1.96):
    p = k / n
    denom = 1 + z**2 / n
    centro = (p + z**2 / (2 * n)) / denom
    mitad = z * np.sqrt(p * (1 - p) / n + z**2 / (4 * n**2)) / denom
    return centro - mitad, centro + mitad

lo, hi = wilson(recibidos, ESPERADOS)
yerr = np.array([prr - lo, hi - prr])

# --- Ajuste logístico (decreciente con la distancia) ---
def logistica(d, d50, s):
    return 1 / (1 + np.exp((d - d50) / s))

popt, pcov = curve_fit(logistica, distancia_cm, prr, p0=[586, 5], maxfev=10000)
d50, s = popt
err = np.sqrt(np.diag(pcov))

# --- Gráfico ---
x = np.linspace(distancia_cm.min() - 3, distancia_cm.max() + 3, 300)

fig, ax = plt.subplots(figsize=(8, 5))
ax.errorbar(distancia_cm, prr, yerr=yerr, fmt="o", capsize=4, label="Datos (IC 95 %)")
ax.plot(x, logistica(x, *popt), "--",
        label=f"Ajuste logístico\nd50 = {d50:.1f} ± {err[0]:.1f} cm, s = {s:.1f} ± {err[1]:.1f} cm")
ax.set_xlabel("Distancia transmisor-receptor (cm)")
ax.set_ylabel("Tasa de paquetes recibidos (PRR)")
ax.set_ylim(-0.1, 1.1)
ax.invert_xaxis()           # quita esta línea si prefieres distancia creciente hacia la derecha
ax.grid(True, alpha=0.3)
ax.legend()
plt.tight_layout()
plt.savefig("prr_vs_distancia.png", dpi=200)
plt.show()