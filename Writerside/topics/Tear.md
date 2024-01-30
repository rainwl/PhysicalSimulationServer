# Tear

![tear flow](Tear.png){thumbnail="true"}

- If the `tear_force` is greater than the `tear_force_threshold`, then the floccule tearing can be achieved.
- **So we should focus on `tear_force`**
- We assign the `tear_force` with `m_tearForce` from the `Instrument.h`
- The current `m_tearForce` comes from `SOFA` calculations

> Calculate the soft force ,from Rongeur.h 's ProcessContacts.
> When particles fixed on the rongeur,record this and when tearing , compute.
> {style="note"}


|   | TODO                                      | Status     |
|---|-------------------------------------------|------------|
| 0 | close mouse tear                          | [done](#0) |
| 1 | `m_tearForce` flow                        | [done](#1) |
| 2 | verify if give a const variable for force | [done](#2) |
| 3 | write a new function to calculate force   |            |


## 0 {collapsible="true"}

In `FlocculationSoftTearing.h`

```C++
bool mouse_tear = true;
```

Turn to `false`.

## 1 {collapsible="true"}

In `LoadScenes.h` , assign `the m_tear_force` with SOFA, we need to remove it.

![](computeForce.png)

## 2 {collapsible="true"}

In `LoadScenes.h`

```C++
//instrument_switch->m_tear_force = &sofa_scene->m_tearForce;
float temp_force = 7;
instrument_switch->m_tear_force = &temp_force;
```

The effect is OK.

## 3 {collapsible="true"}

`tear_force` is a variable from `FlocculationSurfaceMesh.h`.

`m_tear_force` is a variable from `InstrumentSwitch.h`.

In `FlocculationSoftTearing.h`,use `set_tear_force` to assign it.

And in `Rongeur.h` ,in `ProcessContacts`,can we modify here?