---
apply: always
---

# Agents

## Project Description

We are building a top-down shooter game called Neon Grid. Similar games are RUINER, The Ascent, or Syndicate Wars. The 
goal of the project is to learn Unreal Engine along the way so we want to adopt industry best practices at all time.

We will use Gameplay Ability System, Behavior Tree, Blueprint Native Events and Multicast Delegates.

## Best Practices

* Write const correct C++ code.
* Use a C++ base class when you need logic, performance, or hard guarantees (spawn rules, game state flow, authoritative server logic, complex validation, reusable systems).
* Use a Blueprint-only class when it's just configuration/content (setting default pawn, tweaking values, wiring assets, quick iteration).
* Gameplay Ability System should be in code as well except for configuration/content that could be achieved by a game designer.
* We want to keep the Character and Controller as lean as possible and use composition with Components for reusable features.

## Code Conventions

* Apply early returns (gates) as much as possible to avoid deeply nested operators.

