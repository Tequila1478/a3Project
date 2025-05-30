// Fill out your copyright notice in the Description page of Project Settings.


#include "ABacktraceMaze.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavigationSystem.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

// Sets default values
AABacktraceMaze::AABacktraceMaze()
{

//Lets the client's see the maze
bReplicates = true;

// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
PrimaryActorTick.bCanEverTick = false;

// Create and set Root Component if not already set
RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
/*
// Create floor instanced mesh component
m_floorStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FloorMeshComponent"));
m_floorStaticMeshComponent->SetupAttachment(RootComponent);
m_floorStaticMeshComponent->SetMobility(EComponentMobility::Static);  // Optimize for performance

// Create default wall instanced mesh component
m_defaultWallStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DefaultWallMeshComponent"));
m_defaultWallStaticMeshComponent->SetupAttachment(RootComponent);
m_defaultWallStaticMeshComponent->SetMobility(EComponentMobility::Static);

// Create rotated wall instanced mesh component
m_rotatedWallStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("RotatedWallMeshComponent"));
m_rotatedWallStaticMeshComponent->SetupAttachment(RootComponent);
m_rotatedWallStaticMeshComponent->SetMobility(EComponentMobility::Static);

// Create rotated wall instanced mesh component
m_doorWallStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DoorWallMeshComponent"));
m_doorWallStaticMeshComponent->SetupAttachment(RootComponent);
m_doorWallStaticMeshComponent->SetMobility(EComponentMobility::Static);
*/
}

/*===================
GenerateMazeMeshes

Sets up the maze meshes and materials for the maze generation process.
It assigns static meshes to floor and wall components and creates dynamic materials for instancing.
The maze grid is initialized with random rotations, and the maze is generated with a start and end point.
The start and end points are assigned openings in the walls, and the maze is visualized using the `VisualiseMaze` function.
Validations are performed to ensure components are correctly initialized before proceeding.
===================*/
void AABacktraceMaze::GenerateMazeMeshes()
{
	/*
	// Ensure components are valid before proceeding
	if (!m_floorStaticMeshComponent || !m_defaultWallStaticMeshComponent || !m_rotatedWallStaticMeshComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Failed: One or more static mesh components are null!"));
		return;
	}

	// Assign Static Meshes
	if (floorStaticMesh)
	{
		m_floorStaticMeshComponent->SetStaticMesh(floorStaticMesh);
	}

	if (wallStaticMesh)
	{
		m_defaultWallStaticMeshComponent->SetStaticMesh(wallStaticMesh);
		m_rotatedWallStaticMeshComponent->SetStaticMesh(wallStaticMesh);
		m_doorWallStaticMeshComponent->SetStaticMesh(doorWallStaticMesh);
	}

	// Create and Assign Materials
	m_floorInstancedMaterial = UMaterialInstanceDynamic::Create(floorMaterial, this);
	m_defaultWallInstancedMaterial = UMaterialInstanceDynamic::Create(defaultWallMaterial, this);
	m_rotatedWallInstancedMaterial = UMaterialInstanceDynamic::Create(rotatedWallMaterial, this);

	// Apply material to mesh component
	if (m_floorInstancedMaterial)
	{
		m_floorStaticMeshComponent->SetMaterial(0, m_floorInstancedMaterial);
	}

	if (m_defaultWallInstancedMaterial)
	{
		m_defaultWallStaticMeshComponent->SetMaterial(0, m_defaultWallInstancedMaterial);
		m_doorWallStaticMeshComponent->SetMaterial(0, m_defaultWallInstancedMaterial);
	}

	if (m_rotatedWallInstancedMaterial)
	{
		m_rotatedWallStaticMeshComponent->SetMaterial(0, m_rotatedWallInstancedMaterial);
	}
	*/


	/*NEW*/
	// Step 1: Init maze with random rotations

	grid.SetNum(levelWidth);
	for (int x = 0; x < levelWidth; x++) {
		grid[x].SetNum(levelHeight);
	}

	// Step 2: Generate the maze
	int startX = 0;
	int startY = 0;

	// Sets End point to rightEdge every time
	int endX, endY;

	GenerateMaze(startX, startY);

	// Step 4: Create openings at start and end
	grid[startX][startY].westWall = false; // Entrance opening at (0,0) on the left side

	// Remove the appropriate wall at the end point


	endX = levelWidth - 1; // Right edge
	endY = levelHeight - 1;//FMath::RandRange(0, levelHeight - 1);

	grid[endX][endY].eastWall = false; // Exit opening on the right side


	// Step 3: Visualize it
	VisualiseMaze();

	//SpawnNavMeshForLevelAI();

}

/*===================
SpawnNavMeshForLevelAI

Creates a nav mesh to the size of the maze for AI navigation. Then rebuilds the navigation system
===================*/

void AABacktraceMaze::SpawnNavMeshForLevelAI() {

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NavMesh starting creation"));

	//if (HasAuthority()) {
		UWorld* World = GetWorld();
		if (!World)
		{
			UE_LOG(LogTemp, Warning, TEXT("World not found!"));
			return;
		}

		/*
		// Create a new NavMeshBoundsVolume
		ANavMeshBoundsVolume* NavMeshBoundsVolume = GetWorld()->SpawnActor<ANavMeshBoundsVolume>(ANavMeshBoundsVolume::StaticClass(), FVector((levelWidth * positionScaling)/2, (levelHeight * positionScaling)/2, 0), FRotator::ZeroRotator);
		if (NavMeshBoundsVolume) {
			//print string to confirm its created
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("NavMeshBoundsVolume created!"));
		}

		// Set the size of the NavMeshBoundsVolume to cover the maze
		FVector VolumeSize = FVector(levelWidth * positionScaling, levelHeight * positionScaling, 1000.0f); // Adjust height as needed
		NavMeshBoundsVolume->GetRootComponent()->SetWorldScale3D(VolumeSize);
		// Set the navigation bounds
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

		if (NavSys)
		{
			//Print string to confirm nav mesh bounds volume creation
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("NavMeshBoundsVolume created!!"));
			NavSys->Build();
		}
		*/

		// Spawn the NavMesh Bounds Volume
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector center = FVector((levelWidth * positionScaling) / 2.0f, (levelHeight * positionScaling) / 2.0f, 50.0f);
		ANavMeshBoundsVolume* NavVolume = World->SpawnActor<ANavMeshBoundsVolume>(center, FRotator::ZeroRotator, spawnParams);
		if (NavVolume)
		{
			// Set the bounds
			NavVolume->GetRootComponent()->SetMobility(EComponentMobility::Static);
			NavVolume->SetActorScale3D(FVector(
				levelWidth * positionScaling / 100,  // idk why I divide by 100, otherwise the mesh is way too big, it has something to do with the "brush" being a 100,100,100 vector
				levelHeight * positionScaling / 100,
				1
			));

			// Register volume with nav system
			if (FNavigationSystem::GetCurrent<UNavigationSystemV1>(World))
			{
				FNavigationSystem::GetCurrent<UNavigationSystemV1>(World)->OnNavigationBoundsUpdated(NavVolume);
			}

			UE_LOG(LogTemp, Log, TEXT("NavMesh volume spawned at %s"), *center.ToString());
		}
		if (!NavVolume)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn NavMeshBoundsVolume"));
			return;
		}

		FNavigationSystem::Build(*GetWorld());


		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

		if (NavSys)
		{
			//Print string to confirm nav mesh bounds volume creation
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("NavMeshBoundsVolume created!!"));
			NavSys->Build();
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NavMeshBoundsVolume not created!!"));

		}
	//}
}

/*===================
VisualiseMaze

Visualizes the generated maze by instantiating floor and wall meshes at the correct positions.
Walls are placed based on the state of each grid cell, with north, south, east, and west walls added accordingly.
Materials are applied to the mesh components for proper rendering.
Instances are added in bulk for performance optimization.
===================*/
void AABacktraceMaze::VisualiseMaze()
{
	/*
	// Assume these are set up in the actor's constructor
	UInstancedStaticMeshComponent* floorComponent = m_floorStaticMeshComponent;
	UInstancedStaticMeshComponent* hWallComponent = m_defaultWallStaticMeshComponent;
	UInstancedStaticMeshComponent* vWallComponent = m_rotatedWallStaticMeshComponent;
	UInstancedStaticMeshComponent* doorWallComponent = m_doorWallStaticMeshComponent;


	// Assign Material
	m_floorStaticMeshComponent->SetMaterial(0, m_floorInstancedMaterial);
	m_defaultWallStaticMeshComponent->SetMaterial(0, m_defaultWallInstancedMaterial);
	m_rotatedWallStaticMeshComponent->SetMaterial(0, m_rotatedWallInstancedMaterial);
	m_doorWallStaticMeshComponent->SetMaterial(0, m_defaultWallInstancedMaterial);
	*/

	// Create arrays to hold the instances for floor and walls
	TArray<FTransform> floorInstances;
	TArray<FTransform> hWallInstances;
	TArray<FTransform> vWallInstances;
	TArray<FTransform> doorWallInstances;

	TArray<FTransform> doorBPInstances;
	TArray<FTransform> roomTypeBPInstances;

	//Add an extra south wall at start
	FVector addwallLocation = FVector(-positionScaling / 2, 0, 0);
	FVector addwallScale = FVector(1.0f * meshScaling.X, meshRotatedScalingWidth * meshScaling.Y, 1.0f * meshScaling.Z);
	FTransform addwallTransform(FRotator::ZeroRotator, addwallLocation, addwallScale);
	vWallInstances.Add(addwallTransform);
	//at end
	addwallLocation = FVector(positionScaling * levelWidth + positionScaling / 2, positionScaling * (levelHeight - 1), 0);
	addwallScale = FVector(1.0f * meshScaling.X, meshRotatedScalingWidth * meshScaling.Y, 1.0f * meshScaling.Z);
	FTransform add2wallTransform(FRotator::ZeroRotator, addwallLocation, addwallScale);
	vWallInstances.Add(add2wallTransform);

	//Add an extra West wall at start
	addwallLocation = FVector(-positionScaling, positionScaling / 2, 0);
	addwallScale = FVector(meshRotatedScalingWidth * meshScaling.X, 1.0f * meshScaling.Y, 1.0f * meshScaling.Z);
	FTransform nextAddwallTransform(FRotator::ZeroRotator, addwallLocation, addwallScale);
	hWallInstances.Add(nextAddwallTransform);

	//Add an extra East Wall at end
	addwallLocation = FVector(positionScaling * (levelWidth + 1), positionScaling * (levelHeight - 1) + positionScaling / 2, 0);
	addwallScale = FVector(meshRotatedScalingWidth * meshScaling.X, 1.0f * meshScaling.Y, 1.0f * meshScaling.Z);
	FTransform wall2Transform(FRotator::ZeroRotator, addwallLocation, addwallScale);
	vWallInstances.Add(wall2Transform);

	//Add an extra North wall at start
	addwallLocation = FVector(-positionScaling / 2, positionScaling, 0);
	addwallScale = FVector(1.0f * meshScaling.X, meshRotatedScalingWidth * meshScaling.Y, 1.0f * meshScaling.Z);
	FTransform newestwallTransform(FRotator::ZeroRotator, addwallLocation, addwallScale);
	vWallInstances.Add(newestwallTransform);
	//at end
	addwallLocation = FVector(positionScaling * levelWidth + positionScaling / 2, positionScaling * levelHeight, 0);
	addwallScale = FVector(1.0f * meshScaling.X, meshRotatedScalingWidth * meshScaling.Y, 1.0f * meshScaling.Z);
	FTransform newest2wallTransform(FRotator::ZeroRotator, addwallLocation, addwallScale);
	vWallInstances.Add(newest2wallTransform);

	//Add an extra floor
	FVector addlocation = FVector(-(positionScaling / 2), (positionScaling / 2), 0);


	// Floor (unchanged)
	FVector addfloorScale = FVector(1.0f * meshScaling.X, 1.0f * meshScaling.Y, 0.1f * meshScaling.Z);
	FTransform addfloorTransform(FRotator::ZeroRotator, addlocation, addfloorScale);
	floorInstances.Add(addfloorTransform);

	//Add an extra floor
	FVector addfloor2location = FVector(positionScaling * levelWidth + positionScaling / 2, positionScaling * levelHeight - (positionScaling / 2), 0);


	// Floor (unchanged)
	FVector addfloor2Scale = FVector(1.0f * meshScaling.X, 1.0f * meshScaling.Y, 0.1f * meshScaling.Z);
	FTransform addfloor2Transform(FRotator::ZeroRotator, addfloor2location, addfloor2Scale);
	floorInstances.Add(addfloor2Transform);

	if (endroomBlueprint)
	{
		GetWorld()->SpawnActor<AActor>(endroomBlueprint, FTransform(FRotator::ZeroRotator, FVector(positionScaling * levelWidth + positionScaling / 2, positionScaling * levelHeight - positionScaling / 2, 0), FVector(1, 1, 1)));
	}

	/*NEW*/
	for (int x = 0; x < levelWidth; x++) {
		for (int y = 0; y < levelHeight; y++) {
			FVector location = FVector(x * positionScaling + (positionScaling / 2), y * positionScaling + (positionScaling / 2), 0);

			// Floor (unchanged)
			FVector floorScale = FVector(1.0f * meshScaling.X, 1.0f * meshScaling.Y, 0.1f * meshScaling.Z);
			FTransform floorTransform(FRotator::ZeroRotator, location, floorScale);
			floorInstances.Add(floorTransform);

			FRotator wallRotation;
			wallRotation = FRotator(0.0f, 90.0f, 0.0f);

			// Walls adjusted to cell edges
			if (grid[x][y].northWall && y == levelHeight - 1) {
				FVector wallLocation = FVector((x * positionScaling + zOffset) + (positionScaling / 2), (y + 1) * positionScaling, 0);
				FVector wallScale = FVector(1.0f * meshScaling.X, meshRotatedScalingWidth * meshScaling.Y, 1.0f * meshScaling.Z);
				FTransform wallTransform(FRotator::ZeroRotator, wallLocation, wallScale);
				hWallInstances.Add(wallTransform);
			}
			else if (y == levelHeight - 1) {
				FVector wallLocation = FVector((x * positionScaling + zOffset) + (positionScaling / 2), (y + 1) * positionScaling, 0 + meshScaling.Z * 50);
				FVector wallScale = FVector(meshRotatedScalingWidth * meshScaling.X, 1.0f * meshScaling.Y, 1.0f * meshScaling.Z);
				FTransform wallTransform(wallRotation, wallLocation, wallScale);
				doorWallInstances.Add(wallTransform);

				wallLocation = FVector((x * positionScaling + zOffset) + (positionScaling / 2), (y + 1) * positionScaling, 0);
				FTransform doorTransform(FRotator::ZeroRotator, wallLocation, FVector(1.0f, 1.0f, 1.0f));
				doorBPInstances.Add(doorTransform);
			}

			if (grid[x][y].southWall) {
				FVector wallLocation = FVector((x * positionScaling + zOffset) + (positionScaling / 2), y * positionScaling, 0);
				FVector wallScale = FVector(1.0f * meshScaling.X, meshRotatedScalingWidth * meshScaling.Y, 1.0f * meshScaling.Z);
				FTransform wallTransform(FRotator::ZeroRotator, wallLocation, wallScale);
				hWallInstances.Add(wallTransform);
			}
			else {
				FVector wallLocation = FVector((x * positionScaling + zOffset) + (positionScaling / 2), y * positionScaling, 0 + meshScaling.Z * 50);
				FVector wallScale = FVector(meshRotatedScalingWidth * meshScaling.X, 1.0f * meshScaling.Y, 1.0f * meshScaling.Z);
				FTransform wallTransform(wallRotation, wallLocation, wallScale);
				doorWallInstances.Add(wallTransform);

				wallLocation = FVector((x * positionScaling + zOffset) + (positionScaling / 2), y * positionScaling, 0);
				FTransform doorTransform(FRotator::ZeroRotator, wallLocation, FVector(1.0f, 1.0f, 1.0f));
				doorBPInstances.Add(doorTransform);
			}

			if (grid[x][y].eastWall && x == levelWidth - 1) {
				FVector wallLocation = FVector((x + 1) * positionScaling, (y * positionScaling + zOffset) + (positionScaling / 2), 0);
				FVector wallScale = FVector(meshRotatedScalingWidth * meshScaling.X, 1.0f * meshScaling.Y, 1.0f * meshScaling.Z);
				FTransform wallTransform(FRotator::ZeroRotator, wallLocation, wallScale);
				vWallInstances.Add(wallTransform);
			}
			else if (x == levelWidth - 1) {
				FVector wallLocation = FVector((x + 1) * positionScaling, (y * positionScaling + zOffset) + (positionScaling / 2), 0 + meshScaling.Z * 50);
				FVector wallScale = FVector(meshRotatedScalingWidth * meshScaling.X, 1.0f * meshScaling.Y, 1.0f * meshScaling.Z);
				FTransform wallTransform(FRotator::ZeroRotator, wallLocation, wallScale);
				doorWallInstances.Add(wallTransform);

				wallLocation = FVector((x + 1) * positionScaling, (y * positionScaling + zOffset) + (positionScaling / 2), 0);
				FTransform doorTransform(wallRotation, wallLocation, FVector(1.0f, 1.0f, 1.0f));
				doorBPInstances.Add(doorTransform);
			}


			if (grid[x][y].westWall) {
				FVector wallLocation = FVector(x * positionScaling, (y * positionScaling + zOffset) + (positionScaling / 2), 0);
				FVector wallScale = FVector(meshRotatedScalingWidth * meshScaling.X, 1.0f * meshScaling.Y, 1.0f * meshScaling.Z);
				FTransform wallTransform(FRotator::ZeroRotator, wallLocation, wallScale);
				vWallInstances.Add(wallTransform);
			}
			else {
				FVector wallLocation = FVector(x * positionScaling, (y * positionScaling + zOffset) + (positionScaling / 2), 0 + meshScaling.Z * 50);
				FVector wallScale = FVector(meshRotatedScalingWidth * meshScaling.X, 1.0f * meshScaling.Y, 1.0f * meshScaling.Z);
				FTransform wallTransform(FRotator::ZeroRotator, wallLocation, wallScale);
				doorWallInstances.Add(wallTransform);

				wallLocation = FVector(x * positionScaling, (y * positionScaling + zOffset) + (positionScaling / 2), 0);
				FTransform doorTransform(wallRotation, wallLocation, FVector(0.0f, 0.0f, 0.0f));
				doorBPInstances.Add(doorTransform);
			}
		}
	}





	// Now add all the instances at once
	//floorComponent->AddInstances(floorInstances, true);
	//hWallComponent->AddInstances(hWallInstances, true);
	//vWallComponent->AddInstances(vWallInstances, true);

	//doorWallComponent->AddInstances(doorWallInstances, true);

	if (floorBP && HasAuthority()) {
		for (int x = 0; x < floorInstances.Num(); x++) {
			AActor* floor = GetWorld()->SpawnActor<AActor>(floorBP, floorInstances[x]);
			floor->SetActorScale3D(floorInstances[x].GetScale3D());
		}
	}

	if (wallBP && HasAuthority()) {
		for (int x = 0; x < hWallInstances.Num(); x++) {
			AActor* wall = GetWorld()->SpawnActor<AActor>(wallBP, hWallInstances[x]);
			wall->SetActorScale3D(hWallInstances[x].GetScale3D());
		}
	}

	if (wallBP && HasAuthority()) {
		for (int x = 0; x < vWallInstances.Num(); x++) {
			AActor* vWall = GetWorld()->SpawnActor<AActor>(wallBP, vWallInstances[x]);
			vWall->SetActorScale3D(vWallInstances[x].GetScale3D());
		}
	}

	if (doorWallBP && HasAuthority()) {
		for (int x = 0; x < doorWallInstances.Num(); x++) {
			AActor* doorWall = GetWorld()->SpawnActor<AActor>(doorWallBP, doorWallInstances[x]);
			doorWall->SetActorScale3D(doorWallInstances[x].GetScale3D());
		}
	}

	if (doorBlueprint && HasAuthority())
	{
		for (int x = 0; x < doorBPInstances.Num(); x++) {
			GetWorld()->SpawnActor<AActor>(doorBlueprint, doorBPInstances[x]);
		}

	}
	if (HasAuthority()) CreateRoomInfo();

}

/*NEW*/
/*===================
GenerateMaze

Generates a maze using a recursive backtracking algorithm. It marks cells as visited,
identifies valid neighbors, shuffles them for randomness, and removes walls between
neighboring cells. The function recursively explores and backtracks until the maze is complete.
===================*/
void AABacktraceMaze::GenerateMaze(int x, int y)
{
	// Step 1: Mark the current cell as visited
	// Step 2: Identify valid neighbors (left, right, up, down)
	// Step 3: Shuffle the neighbors to ensure random path generation
	// Step 4: Visit each neighbor
	// Step 5: Recursively visit the neighbor
	// Step 6: Backtrack when no unvisited neighbors remain


	// Step 1: Mark the current cell as visited
	grid[x][y].visited = true;

	// Step 2: Identify valid neighbors (left, right, up, down)
	TArray<FIntPoint> neighbors;
	if (x > 0 && !grid[x - 1][y].visited)
	{
		neighbors.Add(FIntPoint(x - 1, y)); // Left
	}

	if (x < levelWidth - 1 && !grid[x + 1][y].visited) {
		neighbors.Add(FIntPoint(x + 1, y)); // Right
	}

	if (y > 0 && !grid[x][y - 1].visited) {
		neighbors.Add(FIntPoint(x, y - 1)); // Down
	}

	if (y < levelHeight - 1 && !grid[x][y + 1].visited) {
		neighbors.Add(FIntPoint(x, y + 1)); // Up
	}

	// Step 3: Shuffle the neighbors to ensure random path generation
	for (int i = neighbors.Num() - 1; i > 0; i--) {
		int j = FMath::RandRange(0, i);
		neighbors.Swap(i, j);
	}

	// Step 4: Visit each neighbor
	for (int i = 0; i < neighbors.Num(); i++) {
		int nx = neighbors[i].X;
		int ny = neighbors[i].Y;

		if (!grid[nx][ny].visited) {
			// Remove the wall between the current cell and the neighbor
			if (nx < x) { // Neighbor is to the left
				grid[x][y].westWall = false;
				grid[nx][ny].eastWall = false;
			}
			else if (nx > x) { // Neighbor is to the right
				grid[x][y].eastWall = false;
				grid[nx][ny].westWall = false;
			}
			else if (ny < y) { // Neighbor is below
				grid[x][y].southWall = false;
				grid[nx][ny].northWall = false;
			}
			else if (ny > y) { // Neighbor is above
				grid[x][y].northWall = false;
				grid[nx][ny].southWall = false;
			}

			// Step 5: Recursively visit the neighbor
			GenerateMaze(nx, ny);
		}
	}

	// Step 6: Backtrack when no unvisited neighbors remain
	// (Handled implicitly by the recursion: the function returns when there are no unvisited neighbors)
}

// Called when the game starts or when spawned
void AABacktraceMaze::BeginPlay()
{
	Super::BeginPlay();
	//if (HasAuthority()) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, "Generate Backtrace Maze");
	}
	GenerateMazeMeshes();


	//Delay by a fraction of a second to ensure world is ready
	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &AABacktraceMaze::DelayedSpawnLogic, 0.1f, false);

	//Generate ending
//}

}

void AABacktraceMaze::DelayedSpawnLogic()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found player character: %s"), *PlayerCharacter->GetName());
		FVector targetLocation = FVector(-positionScaling / 2, positionScaling / 2, 134);

		PlayerCharacter->SetActorLocationAndRotation(targetLocation, FRotator::ZeroRotator);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player character not found (null)!"));
	}
}

void AABacktraceMaze::CreateRoomInfo() {
	TArray<FTransform> roomLocations;

	for (int x = 0; x < levelWidth; x++) {
		for (int y = 0; y < levelHeight; y++) {
			FVector roomLocation = FVector(x * positionScaling + positionScaling / 2, y * positionScaling + positionScaling / 2, meshScaling.Z * 100 / 2);
			FTransform roomTransform(FRotator::ZeroRotator, roomLocation, FVector(1, 1, 1));
			roomLocations.Add(roomTransform);
		}
	}

	if (roomBlueprint)
	{
		for (int x = 0; x < roomLocations.Num(); x++) {
			GetWorld()->SpawnActor<AActor>(roomBlueprint, roomLocations[x]);

		}
	}
}

// Called every frame
void AABacktraceMaze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


