// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class QuickTween : ModuleRules
{
	public QuickTween(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				Path.Combine(ModuleDirectory, "Public"),
				Path.Combine(ModuleDirectory, "Public", "Tweens"),
				Path.Combine(ModuleDirectory, "Public", "Utils"),
				Path.Combine(ModuleDirectory, "Public", "Blueprint"),
				Path.Combine(ModuleDirectory, "Public", "Blueprint", "Latent")
			}
			);
				
		
		PrivateIncludePaths.AddRange(
				new string[] {
					// internal headers only used within this module
					Path.Combine(ModuleDirectory, "Private"),
					Path.Combine(ModuleDirectory, "Private", "Tweens"),
					Path.Combine(ModuleDirectory, "Private", "Blueprint"),
					Path.Combine(ModuleDirectory, "Private", "Blueprint", "Latent"),
				}
			);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				// ... add other public dependencies that you statically link with here ...
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"UMG",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
