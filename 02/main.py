# Vladislav Vancak
# !/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# DH Matrices
def trans_x(d):
    return np.array([
        [1, 0, 0, 0],
        [0, 1, 0, 0],
        [0, 0, 1, d],
        [0, 0, 0, 1]
    ], dtype=float)


def trans_z(a):
    return np.array([
        [1, 0, 0, a],
        [0, 1, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1]
    ], dtype=float)


def rot_x(phi):
    s = np.sin(phi)
    c = np.cos(phi)
    return np.array([
        [c, -s, 0, 0],
        [s, c, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1]
    ], dtype=float)


def rot_z(alpha):
    s = np.sin(alpha)
    c = np.cos(alpha)
    return np.array([
        [1, 0, 0, 0],
        [0, c, -s, 0],
        [0, s, c, 0],
        [0, 0, 0, 1]
    ], dtype=float)

def get_dh(params):
    phi, d, a, alpha = params
    return rot_x(phi) @ trans_x(d) @ trans_z(a) @ rot_z(alpha)

# Assignment parameters
l0 = 0.2


def q1(t):
    return 0.5 * t


def q2(t):
    return 0.5 + 0.3 * t


def q3(t):
    return 0.5 + 0.4 * t

# Model Parameters - phi, d, a, alpha
def parameters(t):
    return [
        (0., l0, 0., 0.),
        (q1(t), 0., 0., 0.),
        (np.pi / 2, q2(t), 0., np.pi / 2),
        (0, q3(t), 0., 0.)
    ]


# Simulation
coordinates_over_time = []
for time in np.linspace(0.0, 3.0, 150):

    # Intialization - Origin and Parameters in time
    coordinates = np.array([0., 0., 0., 1.], dtype=float)
    time_params = parameters(time)

    # Applying DH transformations
    for dh_params in reversed(time_params):
        coordinates = get_dh(dh_params) @ coordinates

    coordinates_over_time.append(coordinates)

# Plots
plot_data = np.array(coordinates_over_time)
x = plot_data[:, 0]
y = plot_data[:, 1]
z = plot_data[:, 2]

# X-Y
plt.plot(x, y, linewidth=2.0)
plt.xlabel("x")
plt.ylabel("y")
plt.show()

# X-Z
plt.plot(x, z, linewidth=2.0)
plt.xlabel("x")
plt.ylabel("z")
plt.show()

# X-Y-Z
fig = plt.figure()
ax = fig.gca(projection='3d')
ax.plot(x, y, z, label='parametric curve', linewidth=2.0)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
plt.show()
