// Copyright Gareth Cross. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.IO;
using UnrealBuildTool;

[SuppressMessage("ReSharper", "InconsistentNaming")]
public class CustomLogger : ModuleRules
{
	public bool TargetIsWindows()
	{
		return Target.Platform == UnrealTargetPlatform.Win64;
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
			return "libzmq-v143-mt-s-4_3_4.lib";
		}
		else
		{
			return "libzmq.a";
		}
	}

	public CustomLogger(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// Turn on exceptions and RTTI for zmq.hpp
		bEnableExceptions = true;
		bUseRTTI = true;

		var PluginDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../../"));
		var InstallDir = Path.Combine(PluginDir, "ThirdParty/install");
		var PlatformDir = Path.Combine(InstallDir, GetPlatformString());

		PrivateIncludePaths.Add(Path.Combine(PlatformDir, "include"));
		PublicAdditionalLibraries.Add(Path.Combine(PlatformDir, "lib", GetZmqLibraryName()));

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				// Our module that does the JSON encoding:
				"CustomLoggerJsonEncoding"
			}
		);
	}
}