using UnrealBuildTool;

public class SafeSave : ModuleRules
{
	public SafeSave(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// Requirement for VS 2026 / MSVC v144+
		CppStandard = CppStandardVersion.Cpp20; 

		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Projects",
			"InputCore",
			"EditorFramework",
			"UnrealEd",
			"ToolMenus",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"SourceControl", 
			"EditorStyle"
		});
	}
}