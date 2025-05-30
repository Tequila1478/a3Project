using UnrealBuildTool;

public class MazeGen: ModuleRules
{
    public MazeGen(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "AIModule", "NavigationSystem"});
    }
}
