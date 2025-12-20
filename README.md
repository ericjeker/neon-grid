# Neon Grid

## Versioning

Git only tracks source code. Content is saved separately.

## Rule

All gameplay logic goes into C++ classes. All asset referencing (meshes, sounds, particle references) and tweaking (timeline curves) happens in Blueprint subclasses.

## Naming Convention 

Prefix C++ classes with standard Unreal prefixes (A for Actors, U for Components/Objects, S for Slate Widgets, E for Enums).

## Directory Structure

* `/Content/Art`: Geometry, Textures (Binary files).
* `/Content/Core`: Base GameModes, Controllers, Inputs.
* `/Content/Characters/Hero`: Specific BP subclasses.
* `/Content/Levels`: Levels

## Synchronize Content

Assuming `D:\` is the external drive.

```powershell
.\Scripts\SyncContent.ps1 -SourcePath .\Content -Destination "D:\Unreal\NeonGrid\Content" -ShowProgress
```
