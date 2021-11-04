// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CustomLogger : ModuleRules
{
	public CustomLogger(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		var bIncludeZmqLibrary = Target.Configuration != UnrealTargetConfiguration.Shipping;
		if (bIncludeZmqLibrary)
		{
			// Turn on exceptions and RTTI for zmq.hpp
			bEnableExceptions = true;
			bUseRTTI = true;

			PrivateIncludePaths.AddRange(
				new string[]
				{
					"C:\\Users\\garet\\Documents\\zmq\\install\\include",
				}
			);

			PublicAdditionalLibraries.AddRange(new string[]
			{
				"C:\\Users\\garet\\Documents\\zmq\\install\\lib\\libzmq-v142-mt-s-4_3_5.lib",
			});
		}

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Json",
			}
		);
	}
}