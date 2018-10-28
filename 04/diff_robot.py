import numpy as np
import sys
import matplotlib.pyplot as plt

GAUGE = .234
G = GAUGE / 2


# === TRANSFORMATION MATRICES ===
def rotation(alpha):
    s = np.sin(alpha)
    c = np.cos(alpha)
    return np.array([
        [c, -s, 0],
        [s, c, 0],
        [0, 0, 1]
    ])


def translation(vector):
    dx, dy, _ = vector
    return np.array([
        [1, 0, dx],
        [0, 1, dy],
        [0, 0, 1]
    ])


# === HELPERS ===
# vector of the axle (normalized)
def get_axle(wheels):
    left, right = wheels
    return (right - left) / GAUGE


def get_axle_center(wheels):
    left, right = wheels
    return (left + right) / 2


# normal to the axis
def get_normal(axis):
    return np.array([-axis[1], axis[0], 0])


# Measured from the center of the axle
def get_radius(left_dist, right_dist):
    return G * ((left_dist + right_dist) / (right_dist - left_dist))


# angle of rotation from radius and wheel distances
def get_rotation_angle(radius, left_dist, right_dist):
    if (left_dist != 0):
        return left_dist / (radius - G)

    if (right_dist != 0):
        return right_dist / (radius + G)


# === MOVEMENT ===
def move_straight(wheels, left_dist, right_dist):
    if (left_dist != right_dist):
        print("[WARN]: Wrong Movement: move_straight")

    axle_vector = get_axle(wheels)
    axle_normal = get_normal(axle_vector)

    left, right = wheels
    move_vector = left_dist * axle_normal

    return np.array([left + move_vector, right + move_vector])


def move_general(wheels, left_dist, right_dist):
    if (left_dist == right_dist):
        print("[WARN]: Wrong Movement: move_general")

    left, right = wheels
    axle_vector = get_axle(wheels)
    axle_centre = get_axle_center(wheels)

    radius = get_radius(left_dist, right_dist)

    rotation_angle = get_rotation_angle(radius, left_dist, right_dist)
    rotation_centre = axle_centre - (radius * axle_vector)

    # Translate rotation_center to origin
    t = translation(-1 * rotation_centre)
    left, right = t @ left, t @ right

    # Rotate angle
    r = rotation(rotation_angle)
    left, right = r @ left, r @ right

    # Translate rotation center back where it was
    t = translation(rotation_centre)
    left, right = t @ left, t @ right

    return np.array([left, right])


# Initialize Wheels
left = np.array([-G, 0, 1])
right = np.array([G, 0, 1])
wheels = np.array([left, right])

# Plot labels
plt.xlabel("x")
plt.ylabel("y")

# Plot start marker
start = get_axle_center(wheels)
plt.plot(start[0], start[1], marker='o', linewidth=0, color="green", label="start")

trajectory = []
for line in sys.stdin:
    left_dist, right_dist = line.split(" ")
    left_dist, right_dist = float(left_dist), float(right_dist)

    if (left_dist == right_dist):
        wheels = move_straight(wheels, left_dist, right_dist)
    else:
        wheels = move_general(wheels, left_dist, right_dist)

    axle_center = get_axle_center(wheels)
    trajectory.append(axle_center)

# Plot trajectory
trajectory = np.array(trajectory)
plt.plot(trajectory[:, 0], trajectory[:, 1], linewidth=2, label="trajectory")

# Plot end marker
end = trajectory[-1]
plt.plot(end[0], end[1], marker='o', linewidth=0, color="red", label="end")

# Show plots
plt.legend(loc='best')
plt.show()
