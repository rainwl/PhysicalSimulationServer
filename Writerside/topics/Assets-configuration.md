# Assets configuration

## Protocol between front-end

![](scn.png){width="300"}

### FleX flow

When FleX(SOFA) boot up
: Load Main.scn
: Load *.obj

if xxx_mirror00.obj exists
: Load xxx_mirror00
: **else**
: Load LPart0_mirror, LPart1_mirror, L_extra

> **Suggestions for front-end (like unity)**
>
> Each time before entering the endoscope scenario, overwrite or create `Main.scn` with `Main1.scn` or `Main3.scn` in the backup
> directory.
>
> Under customized mode, move `xxx_mirror00.obj` to the backup directory to prevent it from being read by FleX.
> 
> And return to the original position when the operation is complete.
> {style = "note"}

> **NOTICE**
>
> `transformation.txt` files cannot exist in lateral scenario directory.
> {style = "warning"}
