using System.Diagnostics.CodeAnalysis;
using System.IO;
using UnrealBuildTool;

[SuppressMessage("ReSharper", "InconsistentNaming")]
public class CustomLogger : ModuleRules {
  public bool TargetIsWindows() {
    return Target.Platform == UnrealTargetPlatform.Win64;
  }

  public string GetZmqLibraryName() {
    if (TargetIsWindows()) {
      return "libzmq-mt-4_3_5.lib";
    } else {
      return "libzmq.a";
    }
  }

  public CustomLogger(ReadOnlyTargetRules Target) : base(Target) {
    PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

    // Turn on exceptions and RTTI for zmq.hpp
    bEnableExceptions = true;
    bUseRTTI = true;
    CppStandard = CppStandardVersion.Cpp20;

    var PluginDir =
        Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../../"));
    var InstallDir = Path.Combine(PluginDir, "install");
    var ZmqLibraryName = GetZmqLibraryName();
    var ZmqLibPath = Path.Combine(InstallDir, "lib", ZmqLibraryName);
    var ZmqDllPath =
        Path.Combine(InstallDir, "bin", ZmqLibraryName.Replace(".lib", ".dll"));

    PrivateIncludePaths.Add(Path.Combine(InstallDir, "include"));
    PublicAdditionalLibraries.Add(ZmqLibPath);
    RuntimeDependencies.Add(
        "$(TargetOutputDir)\\" + Path.GetFileName(ZmqDllPath), ZmqDllPath);

    PublicDependencyModuleNames.AddRange(["Core"]);
    PrivateDependencyModuleNames.AddRange(
        ["CoreUObject", "Engine", "CustomLoggerJsonEncoding"]);
  }
}
