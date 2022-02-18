#pragma once
#include "UeStuff.h"
#include "global.h"
#include "engine.h"
#include "wrappers.h"
#include "UeStuff.h"
#include "Pattern.h"

namespace ModObjects {


	/*
	static inline UE_UObject* StaticLoadObject(UE_UClass* uclass, UE_UObject* InOuter, const wchar_t* InName, const wchar_t* Filename, unsigned int LoadFlags, void* Sandbox, bool bAllowObjectReconciliation)
	{
		return reinterpret_cast<UE_UObject * (__fastcall*)(UE_UClass*, UE_UObject*, const wchar_t*, const wchar_t*, unsigned int, void*, bool)>((DWORD64)Pattern::Find("48 89 5C 24 ? 4C 89 4C 24 ? 48 89 54 24 ? 48 89 4C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 60 45 33 F6 49 8B F0 4C 89 75 E0 41 8B D6 89 55 E8 45 8B C6 44 89 75 EC 49 8D 7E FF 48 85 F6 74 51 66 39 16 74 4C 48 8B DF 66 90"))(uclass, InOuter, InName, Filename, LoadFlags, Sandbox, bAllowObjectReconciliation);
	}*/
	
	static inline UE_UObject* StaticConstructObject_Internal(UE_UClass* uclass, UE_UObject* InOuter, UE_FName Name, unsigned int SetFlags, int InternalSetFlags, UE_UObject* Template, bool bCopyTransientsFromClassDefaults, void* InstanceGraph, bool bAssumeTemplateIsArchetype)
	{
		return reinterpret_cast<UE_UObject * (__fastcall*)(UE_UClass*, UE_UObject*, UE_FName, unsigned int, int, UE_UObject*, bool, void*, bool)>((DWORD64)Pattern::Find("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B 31 4C 8D 25 ? ? ? ? 4C 8B 79 08 48 8B D9 8B 79 18 4C 8B 71 28 F7 86 ? ? ? ? ? ? ? ? 74 76 4D 85 F6 74 37 8B 49 10 33 D2 E8 ? ? ? ?"))(uclass, InOuter, Name, SetFlags, InternalSetFlags, Template, bCopyTransientsFromClassDefaults, InstanceGraph, bAssumeTemplateIsArchetype);
	}

	/*
	static inline UE_UObject* CreateDefaultObject(UE_UClass* ThisClass)
	{
		return reinterpret_cast<UE_UObject * (*)(class UE_UClass*)>((DWORD64)Pattern::Find("4C 8B DC 53 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 83 B9 ? ? ? ? ? 48 8B D9 0F 85 ? ? ? ?"))(ThisClass);
	}*/


	inline void ProcessEvent(void* obj, void* function, void* parms)
	{

		return reinterpret_cast<void(*)(void*, void*, void*)>((DWORD64)Pattern::Find("40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ?"))(obj, function, parms);
	}


	// Enum Engine.ESpawnActorCollisionHandlingMethod
	enum class ESpawnActorCollisionHandlingMethod : uint8_t
	{
		Undefined = 0,
		AlwaysSpawn = 1,
		AdjustIfPossibleButAlwaysSpawn = 2,
		AdjustIfPossibleButDontSpawnIfColliding = 3,
		DontSpawnIfColliding = 4,
		ESpawnActorCollisionHandlingMethod_MAX = 5
	};

	class FVector
	{
	public:
		float X, Y, Z;

	};

	class FRotator
	{
	public:
		float Pitch, Yaw, Roll;

	};

	class FScale
	{
	public:
		float X, Y, Z;

	};

	class FTransform
	{
	public:
		FVector Location;
		FRotator Rotation;
		FScale Scale;
	};

	//Session SDK


	// Class Engine.Actor
	// Size: 0x220 (Inherited: 0x28)
	class AActor : UE_UObject {
	public:
		//char pad_0[0x28]; // 0x0(0x28)
		char pad_28[0x30]; // 0x28(0x30)
		char bNetTemporary : 1; // 0x58(0x01)
		char bNetStartup : 1; // 0x58(0x01)
		char bOnlyRelevantToOwner : 1; // 0x58(0x01)
		char bAlwaysRelevant : 1; // 0x58(0x01)
		char bReplicateMovement : 1; // 0x58(0x01)
		char bHidden : 1; // 0x58(0x01)
		char bTearOff : 1; // 0x58(0x01)
		char bExchangedRoles : 1; // 0x58(0x01)
		char bNetLoadOnClient : 1; // 0x59(0x01)
		char bNetUseOwnerRelevancy : 1; // 0x59(0x01)
		char bRelevantForNetworkReplays : 1; // 0x59(0x01)
		char bRelevantForLevelBounds : 1; // 0x59(0x01)
		char bReplayRewindable : 1; // 0x59(0x01)
		char bAllowTickBeforeBeginPlay : 1; // 0x59(0x01)
		char bAutoDestroyWhenFinished : 1; // 0x59(0x01)
		char bCanBeDamaged : 1; // 0x59(0x01)
		char bBlockInput : 1; // 0x5a(0x01)
		char bCollideWhenPlacing : 1; // 0x5a(0x01)
		char bFindCameraComponentWhenViewTarget : 1; // 0x5a(0x01)
		char bGenerateOverlapEventsDuringLevelStreaming : 1; // 0x5a(0x01)
		char bIgnoresOriginShifting : 1; // 0x5a(0x01)
		char bEnableAutoLODGeneration : 1; // 0x5a(0x01)
		char bIsEditorOnlyActor : 1; // 0x5a(0x01)
		char bActorSeamlessTraveled : 1; // 0x5a(0x01)
		char bReplicates : 1; // 0x5b(0x01)
		char bCanBeInCluster : 1; // 0x5b(0x01)
		char bAllowReceiveTickEventOnDedicatedServer : 1; // 0x5b(0x01)
		char pad_5B_3 : 5; // 0x5b(0x01)
		char pad_5C_0 : 2; // 0x5c(0x01)
		char bActorEnableCollision : 1; // 0x5c(0x01)
		char bActorIsBeingDestroyed : 1; // 0x5c(0x01)
		char pad_5C_4 : 4; // 0x5c(0x01)
		char pad_5d[0x1]; // 0x5d(0x01)
		char pad_5e[0x1]; // 0x5e(0x01)
		char pad_5f[0x1]; // 0x5f(0x01)
		char pad_60[0x34]; // 0x60(0x34)
		float InitialLifeSpan; // 0x94(0x04)
		float CustomTimeDilation; // 0x98(0x04)
		char pad_9C[0x4]; // 0x9c(0x04)
		char pad_a0[0x40]; // 0xa0(0x40)
		AActor* Owner; // 0xe0(0x08)
		SDK::FName NetDriverName; // 0xe8(0x08)
		char pad_f0[0x1]; // 0xf0(0x01)
		char pad_f1[0x1]; // 0xf1(0x01)
		char pad_f2[0x1]; // 0xf2(0x01)
		char pad_f3[0x1]; // 0xf3(0x01)
		int32_t InputPriority; // 0xf4(0x04)
		struct UInputComponent* InputComponent; // 0xf8(0x08)
		float NetCullDistanceSquared; // 0x100(0x04)
		int32_t NetTag; // 0x104(0x04)
		float NetUpdateFrequency; // 0x108(0x04)
		float MinNetUpdateFrequency; // 0x10c(0x04)
		float NetPriority; // 0x110(0x04)
		char pad_114[0x4]; // 0x114(0x04)
		struct APawn* Instigator; // 0x118(0x08)
		SDK::TArray<AActor*> Children; // 0x120(0x10)
		struct USceneComponent* RootComponent; // 0x130(0x08)
		SDK::TArray<struct AMatineeActor*> ControllingMatineeActors; // 0x138(0x10)
		char pad_148[0x8]; // 0x148(0x08)
		SDK::TArray<SDK::FName> Layers; // 0x150(0x10)
		SDK::TWeakObjectPtr<struct UChildActorComponent> ParentComponent; // 0x160(0x08)
		char pad_168[0x8]; // 0x168(0x08)
		SDK::TArray<SDK::FName> Tags; // 0x170(0x10)
		char pad_180[0x1]; // 0x180(0x01)
		char pad_181[0x1]; // 0x181(0x01)
		char pad_182[0x1]; // 0x182(0x01)
		char pad_183[0x1]; // 0x183(0x01)
		char pad_184[0x1]; // 0x184(0x01)
		char pad_185[0x1]; // 0x185(0x01)
		char pad_186[0x1]; // 0x186(0x01)
		char pad_187[0x1]; // 0x187(0x01)
		char pad_188[0x1]; // 0x188(0x01)
		char pad_189[0x1]; // 0x189(0x01)
		char pad_18a[0x1]; // 0x18a(0x01)
		char pad_18b[0x1]; // 0x18b(0x01)
		char pad_18c[0x1]; // 0x18c(0x01)
		char pad_18d[0x1]; // 0x18d(0x01)
		char pad_18e[0x1]; // 0x18e(0x01)
		char pad_18f[0x1]; // 0x18f(0x01)
		char pad_190[0x60]; // 0x190(0x60)
		SDK::TArray<struct UActorComponent*> InstanceComponents; // 0x1f0(0x10)
		SDK::TArray<struct UActorComponent*> BlueprintCreatedComponents; // 0x200(0x10)
		char pad_210[0x10]; // 0x210(0x10)

		static UE_UClass StaticClass()
		{
			UE_UClass ptr = (UE_UClass)ObjObjects.FindObject("Class Engine.Actor");

			return ptr;
		}

	};

	


	// Class Engine.SkeletalMesh
	// Size: 0x398 (Inherited: 0x60)
	class USkeletalMesh {
	public:
		char pad_0[0x60]; // 0x0(0x60)
		char pad_60[0x20]; // 0x60(0x20)
		struct USkeleton* Skeleton; // 0x80(0x08)
		char pad_88[0x1c]; // 0x88(0x1c)
		char pad_a4[0x1c]; // 0xa4(0x1c)
		FVector PositiveBoundsExtension; // 0xc0(0x0c)
		FVector NegativeBoundsExtension; // 0xcc(0x0c)
		SDK::TArray<struct FSkeletalMaterial> Materials; // 0xd8(0x10)
		SDK::TArray<struct FBoneMirrorInfo> SkelMirrorTable; // 0xe8(0x10)
		SDK::TArray<struct FSkeletalMeshLODInfo> LODInfo; // 0xf8(0x10)
		char pad_108[0x50]; // 0x108(0x50)
		char pad_158[0x04]; // 0x158(0x04)
		char pad_15c[0x01]; // 0x15c(0x01)
		char pad_15d[0x01]; // 0x15d(0x01)
		char pad_15e[0x01]; // 0x15e(0x01)
		char bUseFullPrecisionUVs : 1; // 0x15f(0x01)
		char bUseHighPrecisionTangentBasis : 1; // 0x15f(0x01)
		char bHasBeenSimplified : 1; // 0x15f(0x01)
		char bHasVertexColors : 1; // 0x15f(0x01)
		char pad_15F_4 : 1; // 0x15f(0x01)
		char bEnablePerPolyCollision : 1; // 0x15f(0x01)
		char pad_15F_6 : 2; // 0x15f(0x01)
		struct UBodySetup* BodySetup; // 0x160(0x08)
		struct UPhysicsAsset* PhysicsAsset; // 0x168(0x08)
		struct UPhysicsAsset* ShadowPhysicsAsset; // 0x170(0x08)
		SDK::TArray<struct UNodeMappingContainer*> NodeMappingData; // 0x178(0x10)
		SDK::TArray<struct UMorphTarget*> MorphTargets; // 0x188(0x10)
		char pad_198[0x178]; // 0x198(0x178)
		struct UAnimInstance* PostProcessAnimBlueprint; // 0x310(0x08)
		SDK::TArray<struct UClothingAssetBase*> MeshClothingAssets; // 0x318(0x10)
		char pad_328[0x30]; // 0x328(0x30)
		SDK::TArray<struct UAssetUserData*> AssetUserData; // 0x358(0x10)
		SDK::TArray<struct USkeletalMeshSocket*> Sockets; // 0x368(0x10)
		char pad_378[0x10]; // 0x378(0x10)
		SDK::TArray<struct FSkinWeightProfileInfo> SkinWeightProfiles; // 0x388(0x10)

		static UE_UClass StaticClass()
		{
			UE_UClass ptr = (UE_UClass)ObjObjects.FindObject("Class Engine.SkeletalMesh");

			return ptr;
		}

	};


	// Class Engine.SkeletalMeshComponent
	// Size: 0xed0 (Inherited: 0x690)
	class USkeletalMeshComponent {
	public:
		char pad_0[0x690];
		SDK::UObject* AnimBlueprintGeneratedClass; // 0x690(0x08)
		struct UAnimInstance* AnimClass; // 0x698(0x08)
		struct UAnimInstance* AnimScriptInstance; // 0x6a0(0x08)
		struct UAnimInstance* PostProcessAnimInstance; // 0x6a8(0x08)
		char pad_6b0[0x18]; // 0x6b0(0x18)
		char pad_6C8[0x10]; // 0x6c8(0x10)
		char pad_6d8[0x0c]; // 0x6d8(0x0c)
		char pad_6e4[0x0c]; // 0x6e4(0x0c)
		char pad_6F0[0x30]; // 0x6f0(0x30)
		SDK::TArray<struct UAnimInstance*> LinkedInstances; // 0x720(0x10)
		SDK::TArray<struct FTransform> CachedBoneSpaceTransforms; // 0x730(0x10)
		SDK::TArray<struct FTransform> CachedComponentSpaceTransforms; // 0x740(0x10)
		char pad_750[0x150]; // 0x750(0x150)
		float GlobalAnimRateScale; // 0x8a0(0x04)
		char pad_8a4[0x01]; // 0x8a4(0x01)
		char pad_8a5[0x01]; // 0x8a5(0x01)
		char pad_8A6[0x1]; // 0x8a6(0x01)
		char pad_8a7[0x1]; // 0x8a7(0x01)
		char pad_8A8[0x1]; // 0x8a8(0x01)
		char bDisablePostProcessBlueprint : 1; // 0x8a9(0x01)
		char pad_8A9_1 : 1; // 0x8a9(0x01)
		char bUpdateOverlapsOnAnimationFinalize : 1; // 0x8a9(0x01)
		char pad_8A9_3 : 1; // 0x8a9(0x01)
		char bHasValidBodies : 1; // 0x8a9(0x01)
		char bBlendPhysics : 1; // 0x8a9(0x01)
		char bEnablePhysicsOnDedicatedServer : 1; // 0x8a9(0x01)
		char bUpdateJointsFromAnimation : 1; // 0x8a9(0x01)
		char bDisableClothSimulation : 1; // 0x8aa(0x01)
		char pad_8AA_1 : 7; // 0x8aa(0x01)
		char pad_8AB[0x5]; // 0x8ab(0x05)
		char pad_8B0_0 : 1; // 0x8b0(0x01)
		char bDisableRigidBodyAnimNode : 1; // 0x8b0(0x01)
		char bAllowAnimCurveEvaluation : 1; // 0x8b0(0x01)
		char bDisableAnimCurves : 1; // 0x8b0(0x01)
		char pad_8B0_4 : 3; // 0x8b0(0x01)
		char bCollideWithEnvironment : 1; // 0x8b0(0x01)
		char bCollideWithAttachedChildren : 1; // 0x8b1(0x01)
		char bLocalSpaceSimulation : 1; // 0x8b1(0x01)
		char bResetAfterTeleport : 1; // 0x8b1(0x01)
		char pad_8B1_3 : 1; // 0x8b1(0x01)
		char bDeferKinematicBoneUpdate : 1; // 0x8b1(0x01)
		char bNoSkeletonUpdate : 1; // 0x8b1(0x01)
		char bPauseAnims : 1; // 0x8b1(0x01)
		char bUseRefPoseOnInitAnim : 1; // 0x8b1(0x01)
		char bEnablePerPolyCollision : 1; // 0x8b2(0x01)
		char bForceRefpose : 1; // 0x8b2(0x01)
		char bOnlyAllowAutonomousTickPose : 1; // 0x8b2(0x01)
		char bIsAutonomousTickPose : 1; // 0x8b2(0x01)
		char bOldForceRefPose : 1; // 0x8b2(0x01)
		char bShowPrePhysBones : 1; // 0x8b2(0x01)
		char bRequiredBonesUpToDate : 1; // 0x8b2(0x01)
		char bAnimTreeInitialised : 1; // 0x8b2(0x01)
		char bIncludeComponentLocationIntoBounds : 1; // 0x8b3(0x01)
		char bEnableLineCheckWithBounds : 1; // 0x8b3(0x01)
		char bUseBendingElements : 1; // 0x8b3(0x01)
		char bUseTetrahedralConstraints : 1; // 0x8b3(0x01)
		char bUseThinShellVolumeConstraints : 1; // 0x8b3(0x01)
		char bUseSelfCollisions : 1; // 0x8b3(0x01)
		char bUseContinuousCollisionDetection : 1; // 0x8b3(0x01)
		char bPropagateCurvesToSlaves : 1; // 0x8b3(0x01)
		char bSkipKinematicUpdateWhenInterpolating : 1; // 0x8b4(0x01)
		char bSkipBoundsUpdateWhenInterpolating : 1; // 0x8b4(0x01)
		char pad_8B4_2 : 2; // 0x8b4(0x01)
		char bNeedsQueuedAnimEventsDispatched : 1; // 0x8b4(0x01)
		char pad_8B4_5 : 3; // 0x8b4(0x01)
		char pad_8B5[0x1]; // 0x8b5(0x01)
		uint16_t CachedAnimCurveUidVersion; // 0x8b6(0x02)
		char pad_8b8[0x1]; // 0x8b8(0x01)
		char pad_8B9[0x3]; // 0x8b9(0x03)
		float UniformMass; // 0x8bc(0x04)
		float TotalMass; // 0x8c0(0x04)
		float Density; // 0x8c4(0x04)
		float MinPerParticleMass; // 0x8c8(0x04)
		float ClothBlendWeight; // 0x8cc(0x04)
		float EdgeStiffness; // 0x8d0(0x04)
		float BendingStiffness; // 0x8d4(0x04)
		float AreaStiffness; // 0x8d8(0x04)
		float VolumeStiffness; // 0x8dc(0x04)
		float StrainLimitingStiffness; // 0x8e0(0x04)
		float ShapeTargetStiffness; // 0x8e4(0x04)
		bool bWaitForParallelClothTask; // 0x8e8(0x01)
		char pad_8E9[0x7]; // 0x8e9(0x07)
		SDK::TArray<SDK::FName> DisallowedAnimCurves; // 0x8f0(0x10)
		struct UBodySetup* BodySetup; // 0x900(0x08)
		char pad_908[0x8]; // 0x908(0x08)
		char pad_910[0x10]; // 0x910(0x10)
		char pad_920[0x08]; // 0x920(0x08)
		char pad_928[0xd0]; // 0x928(0xd0)
		float TeleportDistanceThreshold; // 0x9f8(0x04)
		float TeleportRotationThreshold; // 0x9fc(0x04)
		char pad_A00[0x8]; // 0xa00(0x08)
		uint32_t LastPoseTickFrame; // 0xa08(0x04)
		char pad_A0C[0x54]; // 0xa0c(0x54)
		char pad_a60[0x08]; // 0xa60(0x08)
		char pad_A68[0xc8]; // 0xa68(0xc8)
		char pad_b30[0x10]; // 0xb30(0x10)
		char pad_B40[0x390]; // 0xb40(0x390)

		static UE_UClass StaticClass()
		{
			UE_UClass ptr = (UE_UClass)ObjObjects.FindObject("Class Engine.SkeletalMeshComponent");

			return ptr;
		}

	};



	// Class Engine.LocalPlayer
	// Size: 0x258 (Inherited: 0x48)
	class ULocalPlayer {
	public:
		char pad_0[0x48]; // 0x48(0x28)
		char pad_48[0x28]; // 0x48(0x28)
		struct UGameViewportClient* ViewportClient; // 0x70(0x08)
		char pad_78[0x1c]; // 0x78(0x1c)
		char pad_94[0x1]; // 0x94(0x01)
		char pad_95[0x3]; // 0x95(0x03)
		class APlayerController* PendingLevelPlayerControllerClass; // 0x98(0x08)
		char bSentSplitJoin : 1; // 0xa0(0x01)
		char pad_A0_1 : 7; // 0xa0(0x01)
		char pad_A1[0x17]; // 0xa1(0x17)
		int32_t ControllerId; // 0xb8(0x04)
		char pad_BC[0x19c]; // 0xbc(0x19c)

		static UE_UClass StaticClass()
		{
			UE_UClass ptr = (UE_UClass)ObjObjects.FindObject("Class Engine.LocalPlayer");

			return ptr;
		}

	};



	// Class Engine.GameInstance
	// Size: 0x1a8 (Inherited: 0x28)
	class UGameInstance {
	public:
		char pad_0[0x28];
		char pad_28[0x10]; // 0x28(0x10)
		SDK::TArray<ULocalPlayer*> LocalPlayers; // 0x38(0x10)
		struct UOnlineSession* OnlineSession; // 0x48(0x08)
		SDK::TArray<UE_UObject*> ReferencedObjects; // 0x50(0x10)
		char pad_60[0x18]; // 0x60(0x18)
		char pad_78[0x10]; // 0x78(0x10)
		char pad_88[0x120]; // 0x88(0x120)

		static UE_UClass StaticClass()
		{
			UE_UClass ptr = (UE_UClass)ObjObjects.FindObject("Class Engine.GameInstance");

			return ptr;
		}

	};


	// Class Engine.LevelActorContainer
	// Size: 0x38 (Inherited: 0x28)
	class ULevelActorContainer {
	public:
		char Pad_0[0x28];
		SDK::TArray<AActor*> Actors; // 0x28(0x10)

		static UE_UClass StaticClass()
		{
			UE_UClass ptr = (UE_UClass)ObjObjects.FindObject("Class Engine.LevelActorContainer");

			return ptr;
		}
	};

	// Class Engine.Level
	// Size: 0x298 (Inherited: 0x28)
	class ULevel {
	public:
		char pad_0x0000[0x98]; //0x0000
		SDK::TArray<AActor*> WorldActors; //0x0098 


		static UE_UClass StaticClass()
		{
			UE_UClass ptr = (UE_UClass)ObjObjects.FindObject("Class Engine.Level");

			return ptr;
		}

	};


	// Class Engine.World
	// Size: 0x788 (Inherited: 0x28)
	class UWorld {
	public:
		char pad_0[0x28]; // 0x0(0x28)
		char pad_28[0x8]; // 0x28(0x08)
		ULevel* PersistentLevel; // 0x30(0x08)
		struct UNetDriver* NetDriver; // 0x38(0x08)
		struct ULineBatchComponent* LineBatcher; // 0x40(0x08)
		struct ULineBatchComponent* PersistentLineBatcher; // 0x48(0x08)
		struct ULineBatchComponent* ForegroundLineBatcher; // 0x50(0x08)
		struct AGameNetworkManager* NetworkManager; // 0x58(0x08)
		struct UPhysicsCollisionHandler* PhysicsCollisionHandler; // 0x60(0x08)
		SDK::TArray<SDK::UObject*> ExtraReferencedObjects; // 0x68(0x10)
		SDK::TArray<SDK::UObject*> PerModuleDataObjects; // 0x78(0x10)
		SDK::TArray<struct ULevelStreaming*> StreamingLevels; // 0x88(0x10)
		char pad_98[0x28]; // 0x98(0x28)
		SDK::FString StreamingLevelsPrefix; // 0xc0(0x10)
		ULevel* CurrentLevelPendingVisibility; // 0xd0(0x08)
		ULevel* CurrentLevelPendingInvisibility; // 0xd8(0x08)
		struct UDemoNetDriver* DemoNetDriver; // 0xe0(0x08)
		struct AParticleEventManager* MyParticleEventManager; // 0xe8(0x08)
		struct APhysicsVolume* DefaultPhysicsVolume; // 0xf0(0x08)
		char pad_F8[0x16]; // 0xf8(0x16)
		char pad_10E_0 : 2; // 0x10e(0x01)
		char bAreConstraintsDirty : 1; // 0x10e(0x01)
		char pad_10E_3 : 5; // 0x10e(0x01)
		char pad_10F[0x1]; // 0x10f(0x01)
		struct UNavigationSystemBase* NavigationSystem; // 0x110(0x08)
		struct AGameModeBase* AuthorityGameMode; // 0x118(0x08)
		struct AGameStateBase* GameState; // 0x120(0x08)
		struct UAISystemBase* AISystem; // 0x128(0x08)
		struct UAvoidanceManager* AvoidanceManager; // 0x130(0x08)
		SDK::TArray<ULevel*> Levels; // 0x138(0x10)
		SDK::TArray<struct FLevelCollection> LevelCollections; // 0x148(0x10)
		char pad_158[0x28]; // 0x158(0x28)
		UGameInstance* OwningGameInstance; // 0x180(0x08)
		SDK::TArray<struct UMaterialParameterCollectionInstance*> ParameterCollectionInstances; // 0x188(0x10)
		struct UCanvas* CanvasForRenderingToTarget; // 0x198(0x08)
		struct UCanvas* CanvasForDrawMaterialToRenderTarget; // 0x1a0(0x08)
		char pad_1A8[0x50]; // 0x1a8(0x50)
		char pad_1f8[0x50]; // 0x1f8(0x50)
		SDK::TArray<struct UActorComponent*> ComponentsThatNeedEndOfFrameUpdate; // 0x248(0x10)
		SDK::TArray<struct UActorComponent*> ComponentsThatNeedEndOfFrameUpdate_OnGameThread; // 0x258(0x10)
		char pad_268[0x370]; // 0x268(0x370)
		struct UWorldComposition* WorldComposition; // 0x5d8(0x08)
		char pad_5E0[0x88]; // 0x5e0(0x88)
		char pad_668[0x50]; // 0x668(0x58)
		char pad_6C0[0xc8]; // 0x6c0(0xc8)

		static UE_UClass StaticClass()
		{
			UE_UClass ptr = (UE_UClass)ObjObjects.FindObject("Class Engine.World");

			return ptr;
		}

	};


	// Class Engine.PlayerController
	// Size: 0x570 (Inherited: 0x298)
	class APlayerController {
	public:
		char pad_0[0x298]; // 0x0(0x298)
		struct UPlayer* Player; // 0x298(0x08)
		struct APawn* AcknowledgedPawn; // 0x2a0(0x08)
		struct UInterpTrackInstDirector* ControllingDirTrackInst; // 0x2a8(0x08)
		struct AHUD* MyHUD; // 0x2b0(0x08)
		struct APlayerCameraManager* PlayerCameraManager; // 0x2b8(0x08)
		struct APlayerCameraManager* PlayerCameraManagerClass; // 0x2c0(0x08)
		bool bAutoManageActiveCameraTarget; // 0x2c8(0x01)
		char pad_2C9[0x3]; // 0x2c9(0x03)
		FRotator TargetViewRotation; // 0x2cc(0x0c)
		char pad_2D8[0xc]; // 0x2d8(0x0c)
		float SmoothTargetViewRotationSpeed; // 0x2e4(0x04)
		char pad_2E8[0x8]; // 0x2e8(0x08)
		SDK::TArray<struct AActor*> HiddenActors; // 0x2f0(0x10)
		SDK::TArray<SDK::TWeakObjectPtr<struct UPrimitiveComponent>> HiddenPrimitiveComponents; // 0x300(0x10)
		char pad_310[0x4]; // 0x310(0x04)
		float LastSpectatorStateSynchTime; // 0x314(0x04)
		FVector LastSpectatorSyncLocation; // 0x318(0x0c)
		FRotator LastSpectatorSyncRotation; // 0x324(0x0c)
		int32_t ClientCap; // 0x330(0x04)
		char pad_334[0x4]; // 0x334(0x04)
		struct UCheatManager* CheatManager; // 0x338(0x08)
		struct UCheatManager* CheatClass; // 0x340(0x08)
		struct UPlayerInput* PlayerInput; // 0x348(0x08)
		SDK::TArray<struct FActiveForceFeedbackEffect> ActiveForceFeedbackEffects; // 0x350(0x10)
		char pad_360[0x70]; // 0x360(0x70)
		char pad_3D0_0 : 4; // 0x3d0(0x01)
		char bPlayerIsWaiting : 1; // 0x3d0(0x01)
		char pad_3D0_5 : 3; // 0x3d0(0x01)
		char pad_3D1[0x3]; // 0x3d1(0x03)
		char NetPlayerIndex; // 0x3d4(0x01)
		char pad_3D5[0x3b]; // 0x3d5(0x3b)
		struct UNetConnection* PendingSwapConnection; // 0x410(0x08)
		struct UNetConnection* NetConnection; // 0x418(0x08)
		char pad_420[0xc]; // 0x420(0x0c)
		float InputYawScale; // 0x42c(0x04)
		float InputPitchScale; // 0x430(0x04)
		float InputRollScale; // 0x434(0x04)
		char bShowMouseCursor : 1; // 0x438(0x01)
		char bEnableClickEvents : 1; // 0x438(0x01)
		char bEnableTouchEvents : 1; // 0x438(0x01)
		char bEnableMouseOverEvents : 1; // 0x438(0x01)
		char bEnableTouchOverEvents : 1; // 0x438(0x01)
		char bForceFeedbackEnabled : 1; // 0x438(0x01)
		char pad_438_6 : 2; // 0x438(0x01)
		char pad_439[0x3]; // 0x439(0x03)
		float ForceFeedbackScale; // 0x43c(0x04)
		SDK::TArray<struct FKey> ClickEventKeys; // 0x440(0x10)
		uint8_t DefaultMouseCursor; // 0x450(0x01)
		uint8_t CurrentMouseCursor; // 0x451(0x01)
		uint8_t DefaultClickTraceChannel; // 0x452(0x01)
		uint8_t CurrentClickTraceChannel; // 0x453(0x01)
		float HitResultTraceDistance; // 0x454(0x04)
		uint16_t SeamlessTravelCount; // 0x458(0x02)
		uint16_t LastCompletedSeamlessTravelCount; // 0x45a(0x02)
		char pad_45C[0x74]; // 0x45c(0x74)
		struct UInputComponent* InactiveStateInputComponent; // 0x4d0(0x08)
		char pad_4D8_0 : 2; // 0x4d8(0x01)
		char bShouldPerformFullTickWhenPaused : 1; // 0x4d8(0x01)
		char pad_4D8_3 : 5; // 0x4d8(0x01)
		char pad_4D9[0x17]; // 0x4d9(0x17)
		struct UTouchInterface* CurrentTouchInterface; // 0x4f0(0x08)
		char pad_4F8[0x50]; // 0x4f8(0x50)
		struct ASpectatorPawn* SpectatorPawn; // 0x548(0x08)
		char pad_550[0x4]; // 0x550(0x04)
		bool bIsLocalPlayerController; // 0x554(0x01)
		char pad_555[0x3]; // 0x555(0x03)
		FVector SpawnLocation; // 0x558(0x0c)
		char pad_564[0xc]; // 0x564(0x0c)

	public:
		void RestartLevel() {

			UE_UFunction fn = (UE_UFunction)ObjObjects.FindObject("Function Engine.PlayerController.RestartLevel");
			struct
			{


			} params;

			UE_UObject PlayerController = ObjObjects.FindObjectByTypeAndContainsString("PlayerController", ".PersistentLevel.", "PlayerController");

			ProcessEvent(PlayerController.GetAddress(), fn.GetAddress(), &params);
		}

		void LocalTravel(SDK::FString URL) {

			UE_UFunction fn = (UE_UFunction)ObjObjects.FindObject("Function Engine.PlayerController.LocalTravel");
			struct
			{
				SDK::FString TravelURL;

			} params;

			params.TravelURL = URL;

			UE_UObject PlayerController = ObjObjects.FindObjectByTypeAndContainsString("PlayerController", ".PersistentLevel.", "PlayerController");

			ProcessEvent(PlayerController.GetAddress(), fn.GetAddress(), &params);
		}

		void Pause(UE_UObject PlayerControll) {

			UE_UFunction fn = (UE_UFunction)ObjObjects.FindObject("Function Engine.PlayerController.Pause");
			struct
			{

			} params;


			ProcessEvent(PlayerControll.GetAddress(), fn.GetAddress(), &params);
		}

		void StopMovement(UE_UObject PlayerControll) {

			UE_UFunction fn = (UE_UFunction)ObjObjects.FindObject("Function Engine.Controller.StopMovement");
			struct
			{

			} params;


			ProcessEvent(PlayerControll.GetAddress(), fn.GetAddress(), &params);
		}

		void ServerPause() // Function Engine.PlayerController.ServerPause // (Net|NetReliableNative|Event|Public|NetServer|NetValidate) // @ game+0x3175d20
		{
			UE_UFunction fn = (UE_UFunction)ObjObjects.FindObject("Function Engine.PlayerController.ServerPause");
			struct
			{

			} params;


			UE_UObject PlayerController = ObjObjects.FindObjectByTypeAndContainsString("PlayerController", ".PersistentLevel.", "PlayerController");

			ProcessEvent(PlayerController.GetAddress(), fn.GetAddress(), &params);

		}

		void SendToConsole(SDK::FString Command) // Function Engine.PlayerController.SendToConsole // (Exec|Native|Public) // @ game+0x3175620
		{
			UE_UFunction fn = (UE_UFunction)ObjObjects.FindObject("Function Engine.PlayerController.SendToConsole");
			struct
			{
				SDK::FString command;

			} params;

			params.command = Command;

			UE_UObject PlayerController = ObjObjects.FindObjectByTypeAndContainsString("PlayerController", ".PersistentLevel.", "PlayerController");

			ProcessEvent(PlayerController.GetAddress(), fn.GetAddress(), &params);
		}


		void FOV(float NewFOV) // Function Engine.PlayerController.FOV // (Exec|Native|Public) // @ game+0x3173060
		{
			UE_UFunction fn = (UE_UFunction)ObjObjects.FindObject("Function Engine.PlayerController.FOV");
			struct
			{
				float FOV;

			} params;

			params.FOV = NewFOV;

			UE_UObject PlayerController = ObjObjects.FindObjectByTypeAndContainsString("PlayerController", ".PersistentLevel.", "PlayerController");

			ProcessEvent(PlayerController.GetAddress(), fn.GetAddress(), &params);
		}



		static UE_UClass StaticClass()
		{
			UE_UClass ptr = (UE_UClass)ObjObjects.FindObject("Class Engine.PlayerController");

			return ptr;
		}

	};

}