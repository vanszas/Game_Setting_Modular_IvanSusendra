// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SkripsiIvan : ModuleRules
{
    public SkripsiIvan(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicSystemLibraries.AddRange(new string[] { "dxgi.lib" });
            PublicSystemLibraries.Add("pdh.lib");
        }

        // --- PERBAIKAN DI SINI ---
        // Modul dasar yang aman untuk Shipping
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore",
                "ApplicationCore"
            }
        );

        // JANGAN masukkan DesktopPlatform untuk build Game/Shipping.
        // Kalau mau tetap dipakai di Editor saja, pakai ini:
        if (Target.Type == TargetRules.TargetType.Editor)
        {
            PrivateDependencyModuleNames.Add("DesktopPlatform");
        }

    }
}