// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class CustomLoggerJsonEncoding : ModuleRules {
  public CustomLoggerJsonEncoding(ReadOnlyTargetRules Target) : base(Target) {
    PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

    PublicDependencyModuleNames.AddRange(new string[] {
      "Core",
    });

    PrivateDependencyModuleNames.AddRange(new string[] {
      "CoreUObject",
      "Engine",
      "Json",
      "JsonUtilities",
    });
  }
}
