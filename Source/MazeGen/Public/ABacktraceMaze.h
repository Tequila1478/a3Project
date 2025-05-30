// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABacktraceMaze.generated.h"

USTRUCT()
struct FMazeCell {
	GENERATED_BODY()
public:
	bool visited = false;
	bool northWall = true;
	bool southWall = true;
	bool eastWall = true;
	bool westWall = true;

};

UCLASS()
class MAZEGEN_API AABacktraceMaze : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AABacktraceMaze();
	void GenerateMazeMeshes();

	/*NEW*/
	void VisualiseMaze();
	void GenerateMaze(int x, int y);


	UFUNCTION(BlueprintCallable, Category = "Navigation Generation")
	void SpawnNavMeshForLevelAI();

	// Width of the maze in grid cells 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze Settings")
	int levelWidth = 128;

	// Height of the maze in grid cells 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze Settings")
	int levelHeight = 128;

	// Distance between cells (affects positioning) 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze Settings")
	int positionScaling = 200;

	// Wall rotation in degrees. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze Settings")
	float wallRotationDeg = 90.0f;

	// Scaling factor for wall and floor meshes 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze Settings")
	FVector meshScaling = FVector{ 1.0f, 1.0f, 1.0f };

	// Scaling factor for wall and floor meshes 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze Settings")
	float meshRotatedScalingWidth = 0.05f;

	/*NEW*/
	// Small offset value to add to remove z fighting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh ZOffset")
	float zOffset = 0.1f;

	// Static mesh for the floor 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Settings")
	TSubclassOf<AActor> floorBP;

	// Static mesh for the walls 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Settings")
	TSubclassOf<AActor> wallBP;

	// Static mesh for the doors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Settings")
	TSubclassOf<AActor> doorWallBP;

	// Default material for walls 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Settings")
	UMaterial* defaultWallMaterial;

	// Material for rotated walls 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Settings")
	UMaterial* rotatedWallMaterial;

	// Material for the floor 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Settings")
	UMaterial* floorMaterial;

	// Door Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings")
	TSubclassOf<AActor> doorBlueprint;

	//Room TYpe Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Type")
	TSubclassOf<AActor> roomBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "End Room")
	TSubclassOf<AActor> endroomBlueprint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DelayedSpawnLogic();

	void CreateRoomInfo();

	// Instanced Static Mesh for default walls (not exposed to editor)
	UPROPERTY()
	UInstancedStaticMeshComponent* m_defaultWallStaticMeshComponent;

	// Instanced Static Mesh for rotated walls (not exposed to editor)
	UPROPERTY()
	UInstancedStaticMeshComponent* m_rotatedWallStaticMeshComponent;

	// Instanced Static Mesh for the floor (not exposed to editor)
	UPROPERTY()
	UInstancedStaticMeshComponent* m_floorStaticMeshComponent;

	// Instanced Static Mesh for rotated walls (not exposed to editor)
	UPROPERTY()
	UInstancedStaticMeshComponent* m_doorWallStaticMeshComponent;

	// Dynamic material instance for rotated walls 
	UPROPERTY()
	UMaterialInstanceDynamic* m_rotatedWallInstancedMaterial;

	// Dynamic material instance for default walls 
	UPROPERTY()
	UMaterialInstanceDynamic* m_defaultWallInstancedMaterial;

	// Dynamic material instance for the floor 
	UPROPERTY()
	UMaterialInstanceDynamic* m_floorInstancedMaterial;

	/*NEW*/
	// Double Dynamic Array to hold our struct
	TArray<TArray<FMazeCell>> grid;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
