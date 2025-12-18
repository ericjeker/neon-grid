---
apply: always
---

# Agents

## Best Practices

* Use a C++ base class when you need logic, performance, or hard guarantees (spawn rules, game state flow, authoritative server logic, complex validation, reusable systems).
* Use a Blueprint-only class when it’s just configuration/content (setting default pawn, tweaking values, wiring assets, quick iteration).

