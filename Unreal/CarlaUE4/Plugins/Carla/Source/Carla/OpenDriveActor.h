// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SceneComponent.h"
#include "Components/BillboardComponent.h"

#include "Traffic/RoutePlanner.h"

#include "Vehicle/VehicleSpawnPoint.h"

#include <compiler/disable-ue4-macros.h>
#include <carla/opendrive/OpenDrive.h>
#include <compiler/enable-ue4-macros.h>

#include "OpenDriveActor.generated.h"

UCLASS()
class CARLA_API AOpenDriveActor : public AActor
{
  GENERATED_BODY()

protected:
  // A UBillboardComponent to hold Icon sprite
  UBillboardComponent* SpriteComponent;
  // Sprite for the Billboard Component
  UTexture2D* SpriteTexture;

private:

  UPROPERTY()
  TArray<ARoutePlanner *> RoutePlanners;

  UPROPERTY()
  TArray<AVehicleSpawnPoint *> VehicleSpawners;

#if WITH_EDITORONLY_DATA
  /// Generate the road network using an OpenDrive file (named as the current .umap)
  UPROPERTY(Category = "Generate", EditAnywhere)
  bool bGenerateRoutes = false;
#endif // WITH_EDITORONLY_DATA

  /// Distance between waypoints where the cars will drive
  UPROPERTY(Category = "Generate", EditAnywhere, meta = (ClampMin = "0.01", UIMin = "0.01"))
  float RoadAccuracy = 2.0f;

#if WITH_EDITORONLY_DATA
  /// Remove the previously generated road network. Also, it will remove spawners if necessary
  UPROPERTY(Category = "Generate", EditAnywhere)
  bool bRemoveRoutes = false;
#endif // WITH_EDITORONLY_DATA

  /// If true, spawners will be placed when generating the routes
  UPROPERTY(Category = "Spawners", EditAnywhere)
  bool bAddSpawners = true;

  /// Determine the height where the spawners will be placed, relative to each RoutePlanner
  UPROPERTY(Category = "Spawners", EditAnywhere)
  float SpawnersHeight = 300.0;

#if WITH_EDITORONLY_DATA
  /// Remove already placed spawners if necessary
  UPROPERTY(Category = "Spawners", EditAnywhere)
  bool bRemoveCurrentSpawners = false;
#endif // WITH_EDITORONLY_DATA

#if WITH_EDITORONLY_DATA
  /// Show / Hide additional debug information
  UPROPERTY(Category = "Debug", EditAnywhere)
  bool bShowDebug = true;
#endif // WITH_EDITORONLY_DATA

public:

  using RoadSegment = carla::road::element::RoadSegment;
  using DirectedPoint = carla::road::element::DirectedPoint;
  using LaneInfo = carla::road::element::LaneInfo;
  using RoadGeneralInfo = carla::road::element::RoadGeneralInfo;
  using RoadInfoLane = carla::road::element::RoadInfoLane;

  AOpenDriveActor(const FObjectInitializer& ObjectInitializer);

  void BuildRoutes();

  void RemoveRoutes();

  void DebugRoutes() const;

  void RemoveDebugRoutes() const;

#if WITH_EDITOR
  void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
#endif // WITH_EDITOR

  ARoutePlanner *GenerateRoutePlanner(const TArray<DirectedPoint> &waypoints);

  TArray<DirectedPoint> GenerateLaneZeroPoints(
      const RoadSegment *road);

  TArray<TArray<DirectedPoint>> GenerateRightLaneWaypoints(
      const RoadSegment *road,
      const TArray<DirectedPoint> &laneZeroPoints);

  TArray<TArray<DirectedPoint>> GenerateLeftLaneWaypoints(
      const RoadSegment *road,
      const TArray<DirectedPoint> &laneZeroPoints);

  void GenerateWaypointsJunction(
      const RoadSegment *road,
      TArray<TArray<DirectedPoint>> &waypoints);

  void GenerateWaypointsRoad(const RoadSegment *road);

  void AddSpawners();

  void RemoveSpawners();
};
