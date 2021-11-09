// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class CustomLogger : ModuleRules
{
	public bool TargetIsWindows()
	{
		return Target.Platform == UnrealTargetPlatform.Win32 ||
		       Target.Platform == UnrealTargetPlatform.Win64;
	}

	public string GetPlatformString()
	{
		if (TargetIsWindows())
		{
			return "Windows";
		}
		else
		{
			return "Linux";
		}
	}

	public string GetZmqLibraryName()
	{
		if (TargetIsWindows())
		{
			return "libzmq-v142-mt-s-4_3_4.lib";
		}
		else
		{
			return "libzmq.a";
		}
	}

	public CustomLogger(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		bool bBuildOnLinux = true;
		var bIncludeZmqLibrary = (Target.Configuration != UnrealTargetConfiguration.Shipping) &&
		                         (bBuildOnLinux || TargetIsWindows());
		if (bIncludeZmqLibrary)
		{
			// Turn on exceptions and RTTI for zmq.hpp
			bEnableExceptions = true;
			bUseRTTI = true;

			var PluginDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));
			var InstallDir = Path.Combine(PluginDir, "ThirdParty/install");
			var PlatformDir = Path.Combine(InstallDir, GetPlatformString());

			PrivateDefinitions.Add("BUILD_CUSTOMER_LOGGER");

			PrivateIncludePaths.AddRange(
				new string[]
				{
					Path.Combine(PlatformDir, "include")
				}
			);
			// TODO(gareth): Determine version of the library automatically...
			PublicAdditionalLibraries.AddRange(new string[]
			{
				Path.Combine(PlatformDir, "lib", GetZmqLibraryName()),
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
				// Our module that does the JSON encoding:
				"CustomLoggerJsonEncoding",
			}
		);
	}
}