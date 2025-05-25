import subprocess
import sys

def install_packages():
    required = ["numpy", "scipy", "matplotlib", "tabulate", "ipympl", "plotly", "nbformat", "pandas", "numba"]

    print(f"📦 Installing packages: {' '.join(required)}")
    subprocess.run(
        [sys.executable, "-m", "pip", "install"] + required,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    print("✅ All packages installed.")

install_packages() # do it now for convenience

# Group into contiguous non-NaN ranges (i.e., success ranges!)
def group_success_ranges(theta1_vals, theta2_vals):
    ranges = []
    start_idx = None

    for i, theta2 in enumerate(theta2_vals):
        if not np.isnan(theta2):
            if start_idx is None:
                start_idx = i
        else:
            if start_idx is not None:
                ranges.append((start_idx, i - 1))
                start_idx = None

    if start_idx is not None:
        ranges.append((start_idx, len(theta2_vals) - 1))

    return [
        (np.degrees(theta1_vals[start]), np.degrees(theta1_vals[end]))
        for start, end in ranges
    ]

import numpy as np
import matplotlib.pyplot as plt

def plot_angles(theta1_vals, theta2_vals):
    """
    Convert theta1 and theta2 arrays from radians to degrees, mask invalid results,
    and plot theta2 vs theta1 with an ideal symmetry line.
    """

    # Convert to degrees
    theta1_deg = np.degrees(theta1_vals)
    theta2_deg = np.array([np.degrees(t2) for t2 in theta2_vals])

    # Mask invalid (NaN) results
    valid_mask = ~np.isnan(theta2_deg)
    valid_theta1 = theta1_deg[valid_mask]
    valid_theta2 = theta2_deg[valid_mask]

    # Plot
    plt.figure(figsize=(8, 5))
    plt.plot(valid_theta1, valid_theta2, label=r"$\theta_2$ vs $\theta_1$", linewidth=2)
    plt.plot(valid_theta1, valid_theta1, 'k--', label=r"Ideal symmetry: $\theta_2 = \theta_1$", alpha=0.6)

    plt.xlabel(r"$\theta_1$ (degrees)")
    plt.ylabel(r"$\theta_2$ (degrees)")
    plt.title("Pantograph Constraint: Valid θ₂ vs θ₁")
    plt.xlim(0, 90)
    plt.ylim(0, 90)
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()


from tabulate import tabulate

def display_optimization_results(param_names, initial_vals, optimized_vals, mse):
    """
    Print a comparison table of initial vs optimized parameters,
    and display the final mean squared error.
    """
    table_data = [
        [name, f"{initial:.4f}", f"{opt:.4f}"]
        for name, initial, opt in zip(param_names, initial_vals, optimized_vals)
    ]
    table_data.append(["Mean Squared Error", "", f"{mse:.6f}"])

    print(tabulate(table_data, headers=["Parameter", "Initial", "Optimized"], tablefmt="github"))


import numpy as np

def generate_loss_surfaces(
    result,
    param_keys,
    bounds_dict,
    symmetry_error_fn,
    resolution=30,
    plot_pairs=None
):
    """
    Generate loss surface data for pairs of parameters around their optimized values.

    Parameters:
    - result: optimization result (e.g. from scipy.optimize.minimize)
    - param_keys: list of parameter names, matching order of result.x
    - bounds_dict: dict mapping parameter names to (min, max) bounds
    - symmetry_error_fn: function that takes a parameter vector and returns a scalar error
    - resolution: number of grid points per axis (default: 30)
    - plot_pairs: list of (param1, param2) tuples to plot (default: all pairs)

    Returns:
    - surface_data: list of tuples (X, Y, Z, x_key, y_key) for each parameter pair
    """
    if plot_pairs is None:
        # Default: all unique pairs
        from itertools import combinations
        plot_pairs = list(combinations(param_keys, 2))

    surface_data = []

    for x_key, y_key in plot_pairs:
        x_idx = param_keys.index(x_key)
        y_idx = param_keys.index(y_key)
        x_opt = result.x[x_idx]
        y_opt = result.x[y_idx]

        x_min, x_max = bounds_dict[x_key]
        y_min, y_max = bounds_dict[y_key]

        dx = (x_max - x_min) / (resolution - 1)
        dy = (y_max - y_min) / (resolution - 1)

        k_x = round((x_opt - x_min) / dx)
        k_y = round((y_opt - y_min) / dy)

        x_start = x_opt - k_x * dx
        y_start = y_opt - k_y * dy

        x_vals = np.linspace(x_start, x_start + dx * (resolution - 1), resolution)
        y_vals = np.linspace(y_start, y_start + dy * (resolution - 1), resolution)
        X, Y = np.meshgrid(x_vals, y_vals)
        Z = np.zeros_like(X)

        for i in range(resolution):
            for j in range(resolution):
                params = result.x.copy()
                params[x_idx] = X[i, j]
                params[y_idx] = Y[i, j]
                value = symmetry_error_fn(params, -0.01)
                Z[i, j] = value if np.isfinite(value) else -0.01

        Z[k_y, k_x] = result.fun  # Ensure the optimized value is exactly placed
        surface_data.append((X, Y, Z, x_key, y_key))

    return surface_data


import plotly.graph_objects as go
import numpy as np

def plot_loss_surfaces(surface_data, param_keys, result, highlight_delta=0.02):
    """
    Plot 3D loss surfaces with the optimized point highlighted and
    red-colored grid tiles where all Z values are within `highlight_delta` of the optimum.

    Parameters:
    - surface_data: list of (X, Y, Z, x_key, y_key) tuples
    - param_keys: list of parameter names in order
    - result: optimization result object (must have .x and .fun)
    - highlight_delta: float, maximum deviation from optimum to highlight
    """
    for X, Y, Z, x_key, y_key in surface_data:
        x_idx = param_keys.index(x_key)
        y_idx = param_keys.index(y_key)

        x_opt = result.x[x_idx]
        y_opt = result.x[y_idx]
        z_opt = result.fun

        fig = go.Figure()

        # Main surface
        fig.add_trace(go.Surface(
            x=X, y=Y, z=Z,
            colorscale="Viridis",
            opacity=0.9,
            showscale=False,
            hovertemplate=(
                f"{x_key} = %{{x:.4f}}<br>" +
                f"{y_key} = %{{y:.4f}}<br>" +
                "MSE = %{z:.6f}<extra></extra>"
            ),
            name="Loss Surface"
        ))

        # Highlight tiles
        i_max, j_max = Z.shape[0] - 1, Z.shape[1] - 1
        for i in range(i_max):
            for j in range(j_max):
                quad_z = [Z[i, j], Z[i+1, j], Z[i, j+1], Z[i+1, j+1]]
                if all(np.abs(z - z_opt) <= highlight_delta for z in quad_z):
                    
                    if any(z < z_opt for z in quad_z):
                        color = 'red'
                    else:
                        color = 'yellow'

                    quad_x = [X[i, j], X[i+1, j], X[i+1, j+1], X[i, j+1], X[i, j]]
                    quad_y = [Y[i, j], Y[i+1, j], Y[i+1, j+1], Y[i, j+1], Y[i, j]]
                    quad_z = [Z[i, j], Z[i+1, j], Z[i+1, j+1], Z[i, j+1], Z[i, j]]

                    fig.add_trace(go.Scatter3d(
                        x=quad_x,
                        y=quad_y,
                        z=quad_z,
                        mode='lines',
                        line=dict(color='red', width=4),
                        name="Near-Optimal Region",
                        showlegend=False
                    ))

        # Optimal point
        fig.add_trace(go.Scatter3d(
            x=[x_opt], y=[y_opt], z=[z_opt],
            mode='markers+text',
            marker=dict(size=7, color='lime', symbol='circle'),
            text=["Optimized"],
            textposition="top center",
            name="Optimized Point"
        ))

        zmin = 0.0
        zmax = np.nanmax(Z)

        fig.update_layout(
            title=f"Loss Surface: {x_key} vs {y_key}",
            scene=dict(
                zaxis=dict(range=[zmin, zmax]),
                xaxis_title=x_key,
                yaxis_title=y_key,
                zaxis_title="Mean Squared Error",
            ),
            width=800,
            height=600,
            showlegend=True
        )

        fig.show()


from IPython.display import HTML
from matplotlib.animation import FuncAnimation
import numpy as np
import matplotlib.pyplot as plt

def animate_leg(theta1_vals, theta2_vals, l, a_x, a_y, b, c):
    fig, ax = plt.subplots(figsize=(6, 6))
    ax.set_xlim(-0.5, 1.8)
    ax.set_ylim(-1.8, 0.5)
    ax.set_aspect('equal')
    ax.grid(True)
    title = ax.set_title("")

    hip = np.array([0, 0])
    anchor = np.array([a_x, a_y])

    line_upper, = ax.plot([], [], 'o-', lw=3, label="Upper leg")
    line_lower, = ax.plot([], [], 'o-', lw=3, label="Lower leg")
    line_panto, = ax.plot([], [], 'o-', lw=2, label="Pantograph rod", color='orange')
    ax.plot(anchor[0], anchor[1], 'rx', markersize=8, label="Anchor")
    ax.legend()

    def update(frame):
        theta1 = theta1_vals[frame]
        theta2 = theta2_vals[frame]
        if np.isnan(theta2):
            return line_upper, line_lower, line_panto, title  # Still return artists!

        knee = hip + l * np.array([np.cos(theta1), -np.sin(theta1)])
        foot = knee + l * np.array([-np.cos(-theta2), np.sin(-theta2)])
        connector = knee + c * np.array([np.cos(-theta2), -np.sin(-theta2)])

        line_upper.set_data([hip[0], knee[0]], [hip[1], knee[1]])
        line_lower.set_data([connector[0], foot[0]], [connector[1], foot[1]])
        line_panto.set_data([connector[0], anchor[0]], [connector[1], anchor[1]])
        title.set_text(
            f"θ₁ = {np.degrees(theta1):.1f}°, θ₂ = {np.degrees(theta2):.1f}°, "
            f"diff = {np.degrees(theta2 - theta1):.1f}°"
        )

        return line_upper, line_lower, line_panto, title

    valid_frames = [i for i, t2 in enumerate(theta2_vals) if not np.isnan(t2)]
    ani = FuncAnimation(fig, update, frames=valid_frames, interval=100, blit=True)
    plt.close(fig)
    return HTML(ani.to_jshtml())


def extract_ax_eq_ay_point_cloud(result, param_keys, bounds_dict, symmetry_error_fn, resolution=30, angle_range=range(20, 71, 10)):
    """
    Sample points along the a_x = a_y line for multiple angles (20° to 70°), and vary |A|, b, c.
    Returns: array of shape (N, 5): [a_mag, b, c, mse, angle_deg]
    """
    import numpy as np

    a_mags = np.linspace(0.1, 1.2, resolution)
    b_vals = np.linspace(bounds_dict["b"][0], bounds_dict["b"][1], resolution)
    c_vals = np.linspace(bounds_dict["c"][0], bounds_dict["c"][1], resolution)

    cloud = []

    for angle_deg in angle_range:
        a_angle = np.radians(angle_deg)

        for a_mag in a_mags:
            a_x = a_mag * np.cos(a_angle)
            a_y = a_mag * np.sin(a_angle)

            for b in b_vals:
                for c in c_vals:
                    params = result.x.copy()
                    param_map = dict(zip(param_keys, params))

                    param_map["a_x"] = a_x
                    param_map["a_y"] = a_y
                    param_map["b"]   = b
                    param_map["c"]   = c

                    vec = [param_map[k] for k in param_keys]
                    mse = symmetry_error_fn(vec, np.nan)

                    if np.isfinite(mse):
                        cloud.append((a_mag, b, c, mse))

    return np.array(cloud)


import plotly.express as px

def plot_universe_point_cloud(cloud, mse_limit=None):
    a_mag, b, c, mse = cloud.T

    if mse_limit:
        mask = mse < mse_limit
        a_mag, b, c, mse = a_mag[mask], b[mask], c[mask], mse[mask]

    fig = px.scatter_3d(
        x=a_mag, y=b, z=c,
        color=mse,
        labels={'x': '|A|', 'y': 'b', 'z': 'c', 'color': 'MSE'},
        title="Symmetry Landscape: Fixed a_angle = 45°"
    )
    fig.update_traces(marker=dict(size=3, opacity=0.6))
    fig.show()
