using UnrealBuildTool;

public class NeonLoadout : ModuleRules
{
	public NeonLoadout(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			[
				"Core",
				"CoreUObject",
				"Engine",

				"NeonInventory"
			]
		);

		PrivateDependencyModuleNames.AddRange(
			[
				"Slate",
				"SlateCore"
			]
		);
	}
}