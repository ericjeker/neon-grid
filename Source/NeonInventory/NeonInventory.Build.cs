using UnrealBuildTool;

public class NeonInventory : ModuleRules
{
	public NeonInventory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			[
				"Core"
			]
		);

		PrivateDependencyModuleNames.AddRange(
			[
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
			]
		);
	}
}