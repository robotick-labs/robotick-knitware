
# 🐔MAGG.e™'s Narrative Brain: A Layered Architecture for Emotionally Expressive Robotics

## Abstract

This paper proposes a novel narrative cognition module for embodied robotics, designed forMAGG.e™ — a two-wheeled, LEGO-based balancing robot with depth sensing and character. The system introduces a layered control model that supplements traditional robotics behavior with expressive, narrative-aware decision-making. A slow-running "narrator brain" interprets the robot's actions, environment, and emotional state to produce poetic inner monologues, dynamic behavior scripting, and emotionally resonant stor...

We argue that this approach is not just expressive but potentially aligned with cognitive models of human decision-making. Emotional and narrative context may serve as functional scaffolds for planning and agency, with implications for explainability, HRI, and adaptive autonomy.

---

## 1. Introduction

As robotics enters human-facing domains — education, domestic assistance, expressive interaction — the ability to communicate meaningfully and interpretably becomes paramount. While sensor fusion, SLAM, and motion planning solve "what to do" and "how to do it," they rarely answer *"how should it feel?"* or *"how might this be told?"*

This work introduces a **narrative control layer**, allowing a robot to build an evolving, emotionally grounded story of its experiences. The robot gains not just function, but character — enabling new forms of transparency, education, companionship, and engagement.

We explore this through the development of *MAGG.e™**, a balancing chicken-themed robot powered by ROS2 and the modular Robotick Engine. Her “soul” is an LLM-assisted narrative planner, generating real-time or reflective descriptions, and informing behavior scripting with affective depth.

---

## 2. Theoretical Rationale

### 2.1 Narrative as a Cognitive Framework

Cognitive science increasingly supports the idea that humans interpret and plan their actions through narrative structures. Jerome Bruner (1990) proposed that narrative is not merely a communication strategy, but a mode of thought — on par with logical reasoning. Roger Schank and Robert Abelson introduced the concept of **scripts** and **plans** as internalized, story-like mechanisms that guide action selection in uncertain environments.

Gazzaniga’s “interpreter” hypothesis suggests that humans retrospectively assemble coherent stories about their actions, while Kahneman's work on *System 1* and *System 2* supports the view that we rationalize post-hoc. These narrative structures often embed emotional context, serving as a bias or reward signal that shapes memory and decision-making.

This proposal applies these insights to robotics: we hypothesize that giving robots narrative and emotional context — even as slow, auxiliary layers — may lead to **more interpretable, adaptive, and human-aligned autonomy**.

---

## 3. Related Work

### 3.1 Narrative Robotics and HRI

- Breazeal et al. (MIT) explored emotional expressivity in robots, showing increased engagement and trust
- The Kismet and Leonardo robots used expressive affordances to frame interaction but lacked reflective narration
- More recent work at Carnegie Mellon and TU Delft explores story-driven robot tutors, but typically with static scripts or centralized authoring

### 3.2 Cognitive Architectures

- Rodney Brooks’ subsumption architecture established layered control, but narrative/emotion layers remain rare
- Architectures like SOAR or ACT-R implement deliberative reasoning but not narrative awareness
- Some work in symbolic planners incorporates goal narratives but lacks affective tagging or expressive output

### 3.3 Affective Computing

- Picard’s foundational work introduced emotional state modeling into HCI, with increasing relevance in social robotics
- Emotion-as-modulator models are emerging in RL (e.g. intrinsic motivation or curiosity-driven exploration)
- These are typically numerical — this project frames emotion as **semantic and narrative**, not just scalar

### 3.4 LLM Integration

- Work by OpenAI, Microsoft, and Stanford has begun exploring LLMs in planning, code generation, and story composition
- This project uniquely embeds an LLM not for general reasoning, but for shaping a robot’s lived experience into a story

### 3.5 Explainable AI (XAI) and Embodied Agents

- Narrative has been proposed as an interface for XAI systems (e.g. Lipton 2017)
- Some mobile robots attempt to explain decisions ("I turned to avoid obstacle") — this work aims to generate not just justification, but *character*

---

## 4. Architecture

### 4.1 Layered System Overview

```plaintext
1. Motor Layer        → Balancing, motion (100–500Hz)
2. Behavior Layer     → Reactive actions (5–50Hz)
3. Script Runner      → Prewritten / dynamic action sequences (0.5–2Hz)
4. Narrative Planner  → Slow narrative scripting (0.01–0.2Hz)
5. Soul Layer (LLM)   → Introspective, poetic, memory-driven (0.001Hz)
```

Each layer operates independently but exchanges data via ROS2. The **Narrative Planner** selects or constructs scripts for the Script Runner. The **Soul Layer** (GPT-4o or similar) periodically updates the story arc or injects metaphorical framing.

### 4.2 ROS2 Integration

```plaintext
Node: magg_e_narrator
├── Subscribes:
│   ├── /detections           (vision_msgs)
│   ├── /odom or /tf          (geometry_msgs)
│   ├── /emotion_state        (std_msgs/String)
│   └── /collision_events     (custom_msgs/ImpactEvent)
├── Publishes:
│   ├── /narration_log        (std_msgs/String)
│   └── /action_script        (custom_msgs/ScriptAction)
```

---

## 5. Use Cases

- “Captain’s Logs” describing SLAM runs with metaphorical and emotional framing
- Dynamic emotional response to objects (“I fear the bin”, “the hallway feels lonely”)
- Affectionate behavior naming: `step_over_duck` → `tiptoe_past_sentinel`
- Interactive "choose your own adventure" scripts triggered by events

---

## 6. Tools and Assets

| File / Module | Purpose |
|---------------|---------|
| MAGG.e™-soul.yaml` | Maps emotions to behavior sets and triggers |
| `log_to_story.py` | Converts rosbag to story-form narrative |
| `voice-over.sh` | Synthesizes narration with TTS |
| `scripts/*.yaml` | Reusable narrative-action plans |

---

## 7. Future Work

- 🧠 **Semantic memory** of recurring spaces and feelings
- 🎓 **Educational scripting** for teaching robotics through story
- 🤖 **Narrative planning in functional robotics** (e.g. delivery bots)
- 🎭 **Mood-biased planning and perception**
- 🧠 **Robotick Engine integration** as cognitive runtime for emotional robotics

---

## 8. Conclusion

This paper introduces a control architecture that merges robotics with narrative cognition. Framed around the development ofMAGG.e™, the system demonstrates that story and emotion are not merely decorative — they may be *functional components* of intelligent behavior.

We believe this model, while showcased playfully, may inform future emotionally aware, explainable, and socially legible robots — from education to healthcare, domestic AI to exploration.

> *“There was a knock at the cabinet. I did not answer. I evolved.” –MAGG.e™*

---

## Acknowledging Robotick

While this work is focused onMAGG.e™, the architecture is implemented within the Robotick Engine — an open, modular robotics framework also under active development. Future work may explore Robotick as a standalone contribution or platform for broader research.

