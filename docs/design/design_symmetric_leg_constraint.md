# Symmetric Leg Linkage Constraint Derivation MAGG.e™)

## Purpose

To design a symmetric planar leg mechanism forMAGG.e™, we aim to ensure that the left and right leg joints satisfy:

```
θ₁ = θ₂
```

at all times. This simplifies control and helps maintain geometric symmetry during locomotion.

## Setup

We consider a planar leg linkage with a rear pantograph-style connector. Each leg consists of:

- **Upper leg (hip to knee):** length `l`
- **Lower leg (knee to foot):** also length `l`
- **Rear offset from knee to pantograph connector:** `c`
- **Pantograph link (connector to body):** fixed length `b`
- **Distance from hip to pantograph anchor on body:** `a`
- **Angle of pantograph anchor from hip:** `α` (relative to horizontal)

The goal is to find a constraint that ensures `θ₁ = θ₂` is preserved **throughout the motion**, not just at a single pose.

## Vector Derivation

Let:

- Hip joint be at the origin
- Knee vector:  
  ```math
  \vec{knee} = l[cos(θ), sin(θ)]
  ```
- Pantograph connector point:  
  ```math
  \vec{conn} = \vec{knee} + c[cos(θ), sin(θ)] = (l + c)[cos(θ), sin(θ)]
  ```
- Pantograph body anchor:  
  ```math
  \vec{anchor} = a[cos(α), sin(α)]
  ```

Then the pantograph link vector is:

```math
\vec{p} = \vec{conn} - \vec{anchor}
```

We require:

```math
\|\vec{p}\| = b
```

So:

```math
b^2 = \|\vec{p}\|^2 = a^2 + c^2 + l^2 + 2cl - 2a(c + l)cos(α - θ)
```

This is the core constraint equation.

## Interpretation

- This equation relates **leg angle `θ`** to the **geometry** of the pantograph system.
- If all parameters (`a, b, c, l, α`) are **fixed**, then this constraint only holds at specific `θ`.
- Therefore, **you cannot have continuous symmetry across all `θ`** unless further geometric constraints are imposed.

## Special Case: Parallelogram Solution

If you enforce:

- `a = c + l`
- `b = a`
- `α = θ`

Then all vectors align, and you form a parallelogram (or rhombus). The constraint simplifies and becomes independent of `θ`, allowing symmetry at all poses.

## Design Implications

To achieve `θ₁ = θ₂` over a **range** of motion:

### Options:

1. **Parallelogram linkage**  
   Enforce geometric conditions above for guaranteed symmetry.

2. **Compliance or slots**  
   Allow the pantograph rod to flex or slide to absorb geometric mismatch as `θ` changes.

3. **Accept limited symmetry**  
   Design so that the constraint is exactly satisfied at rest/mid-stance, and approximate elsewhere.

## Diagram

(Refer to whiteboard sketch or create a formal version with vector positions labeled.)

## Next Steps

- Visualize constraint satisfaction for different values of `a, b, c, α`
- Simulate in MuJoCo or Symbolic Python
- Consider using this constraint to drive mechanical synthesis or controller design
