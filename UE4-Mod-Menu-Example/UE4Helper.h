#pragma once
#include <Windows.h>
//#include "generic.h"
//#include "UeStuff.h"
#include <string>
#include "UE4/UE4.h"
#include "UeStuff.h"

#ifdef _MSC_VER
#pragma pack(push, 0x8)
#endif


namespace UE4Helper
{




	struct FNameEntry
	{
		uint32_t Index;
		uint32_t pad;
		FNameEntry* HashNext;
		char AnsiName[1024];

		const int GetIndex() const { return Index >> 1; }
		const char* GetAnsiName() const { return AnsiName; }
	};

	class TNameEntryArray
	{
	public:

		bool IsValidIndex(uint32_t index) const { return index < NumElements; }

		FNameEntry const* GetById(uint32_t index) const { return *GetItemPtr(index); }

		FNameEntry const* const* GetItemPtr(uint32_t Index) const {
			const auto ChunkIndex = Index / 16384;
			const auto WithinChunkIndex = Index % 16384;
			const auto Chunk = Chunks[ChunkIndex];
			return Chunk + WithinChunkIndex;
		}

		FNameEntry** Chunks[128];
		uint32_t NumElements = 0;
		uint32_t NumChunks = 0;
	};

	struct FName
	{
		int ComparisonIndex = 0;
		int Number = 0;

		static inline TNameEntryArray* GNames = nullptr;

		static const char* GetNameByIdFast(int Id) {
			auto NameEntry = GNames->GetById(Id);
			if (!NameEntry) return nullptr;
			return NameEntry->GetAnsiName();
		}

		static std::string GetNameById(int Id) {
			auto NameEntry = GNames->GetById(Id);
			if (!NameEntry) return std::string();
			return NameEntry->GetAnsiName();
		}

		const char* GetNameFast() const {
			auto NameEntry = GNames->GetById(ComparisonIndex);
			if (!NameEntry) return nullptr;
			return NameEntry->GetAnsiName();
		}

		const std::string GetName() const {
			auto NameEntry = GNames->GetById(ComparisonIndex);
			if (!NameEntry) return std::string();
			return NameEntry->GetAnsiName();
		};

		inline bool operator==(const FName& other) const {
			return ComparisonIndex == other.ComparisonIndex;
		};

		FName() {}

		FName(const char* find) {
			for (auto i = 6000u; i < GNames->NumElements; i++)
			{
				auto name = GetNameByIdFast(i);
				if (!name) continue;
				if (strcmp(name, find) == 0) {
					ComparisonIndex = i;
					return;
				};
			}
		}
	};

	struct FUObjectItem
	{
		class UObject* Object;
		int Flags;
		int ClusterIndex;
		int SerialNumber;
		int pad;
	};

	struct TUObjectArray
	{
		FUObjectItem* Objects;
		int MaxElements;
		int NumElements;

		class UObject* GetByIndex(int index) { return Objects[index].Object; }
	};

	class UClass;
	class UObject
	{
	public:
		UObject(UObject* addr) { *this = addr; }
		static inline TUObjectArray* GObjects = nullptr;
		void* Vtable; // 0x0
		int ObjectFlags; // 0x8
		int InternalIndex; // 0xC
		UClass* Class; // 0x10
		FName Name; // 0x18
		UObject* Outer; // 0x20

		std::string GetName() const;

		const char* GetNameFast() const;

		std::string GetFullName() const;

		template<typename T>
		static T* FindObject(const std::string& name)
		{
			for (int i = 0; i < GObjects->NumElements; ++i)
			{
				auto object = GObjects->GetByIndex(i);

				if (object == nullptr)
				{
					continue;
				}

				if (object->GetFullName() == name)
				{
					return static_cast<T*>(object);
				}
			}
			return nullptr;
		}

		static UClass* FindClass(const std::string& name)
		{
			return FindObject<UClass>(name);
		}

		template<typename T>
		static T* GetObjectCasted(uint32_t index)
		{
			return static_cast<T*>(GObjects->GetByIndex(index));
		}

		bool IsA(UClass* cmp) const;

		static UClass* StaticClass()
		{
			static auto ptr = UObject::FindObject<UClass>("Class CoreUObject.Object");
			return ptr;
		}
	};

	class UField : public UObject
	{
	public:
		using UObject::UObject;
		UField* Next;
	};

	class UProperty : public UField
	{
	public:
		int ArrayDim;
		int ElementSize;
		uint64_t PropertyFlags;
		char pad[0xC];
		int Offset_Internal;
		UProperty* PropertyLinkNext;
		UProperty* NextRef;
		UProperty* DestructorLinkNext;
		UProperty* PostConstructLinkNext;
	};


	class UStruct : public UField
	{
	public:
		using UField::UField;

		UStruct* SuperField;
		UField* Children;
		int PropertySize;
		int MinAlignment;
		SDK::TArray<uint8_t> Script;
		UProperty* PropertyLink;
		UProperty* RefLink;
		UProperty* DestructorLink;
		UProperty* PostConstructLink;
		SDK::TArray<UObject*> ScriptObjectReferences;
	};

	class UFunction : public UStruct
	{
	public:
		int FunctionFlags;
		uint16_t RepOffset;
		uint8_t NumParms;
		char pad;
		uint16_t ParmsSize;
		uint16_t ReturnValueOffset;
		uint16_t RPCId;
		uint16_t RPCResponseId;
		UProperty* FirstPropertyToInit;
		UFunction* EventGraphFunction; //0x00A0
		int EventGraphCallOffset;
		char pad_0x00AC[0x4]; //0x00AC
		void* Func; //0x00B0
	};


	inline void ProcessEvent(void* obj, UFunction* function, void* parms)
	{
		auto vtable = *reinterpret_cast<void***>(obj);
		reinterpret_cast<void(*)(void*, UFunction*, void*)>(vtable[59])(obj, function, parms);
	}

	class UClass : public UStruct
	{
	public:
		using UStruct::UStruct;
		unsigned char                                      UnknownData00[0x138];                                     // 0x0088(0x0138) MISSED OFFSET

		template<typename T>
		inline T* CreateDefaultObject()
		{
			return static_cast<T*>(CreateDefaultObject());
		}
	};

	





	struct FMinimalViewInfo {
		FVector Location;
		FRotator Rotation;
		char UnknownData00[0x10];
		float FOV;
	};

	struct FCameraCacheEntry {
		float TimeStamp;
		char pad[0xC];
		FMinimalViewInfo POV;
	};

	struct APlayerCameraManager {
		char pad[0x04D0];
		FCameraCacheEntry CameraCache;

		FVector GetCameraLocation() {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.PlayerCameraManager.GetCameraLocation");
			FVector location;
			ProcessEvent(this, fn, &location);
			return location;
		};
		FRotator GetCameraRotation() {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.PlayerCameraManager.GetCameraRotation");
			FRotator rotation;
			ProcessEvent(this, fn, &rotation);
			return rotation;
		}
	};


	struct FKey
	{
		FName KeyName;
		unsigned char UnknownData00[0x18] = {};

		FKey() {};
		FKey(const char* InName) : KeyName(FName(InName)) {}
	};

	struct AController {

		char pad_0000[0x0488]; //0x0000
		class ACharacter* Character; //0x0488
		char pad_0480[0x70]; // 0x490
		APlayerCameraManager* PlayerCameraManager; //0x0500
		char pad_04f8[0x1049]; // 0x0508
		bool IdleDisconnectEnabled; // 0x1551(0x0001)

		void SendToConsole(SDK::FString& cmd) {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.PlayerController.SendToConsole");
			ProcessEvent(this, fn, &cmd);
		}

		bool ProjectWorldLocationToScreen(const FVector& WorldLocation, FVector2D& ScreenLocation) {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.PlayerController.ProjectWorldLocationToScreen");
			struct
			{
				FVector WorldLocation;
				FVector2D ScreenLocation;
				bool ReturnValue = false;
			} params;

			params.WorldLocation = WorldLocation;
			ProcessEvent(this, fn, &params);
			ScreenLocation = params.ScreenLocation;
			return params.ReturnValue;
		}

		bool WasInputKeyJustPressed(const FKey& Key) {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.PlayerController.WasInputKeyJustPressed");
			struct
			{
				FKey Key;
				bool ReturnValue = false;
			} params;

			params.Key = Key;
			ProcessEvent(this, fn, &params);
			return params.ReturnValue;
		}

		FRotator GetControlRotation() {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.Pawn.GetControlRotation");
			struct FRotator rotation;
			ProcessEvent(this, fn, &rotation);
			return rotation;
		}

		FRotator GetDesiredRotation() {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.Pawn.GetDesiredRotation");
			struct FRotator rotation;
			ProcessEvent(this, fn, &rotation);
			return rotation;
		}

		void AddYawInput(float Val) {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.PlayerController.AddYawInput");
			ProcessEvent(this, fn, &Val);
		}

		void AddPitchInput(float Val) {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.PlayerController.AddPitchInput");
			ProcessEvent(this, fn, &Val);
		}

		void FOV(float NewFOV) {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.PlayerController.FOV");
			ProcessEvent(this, fn, &NewFOV);
		}

		bool LineOfSightTo(ACharacter* Other, const FVector& ViewPoint, const bool bAlternateChecks) {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.Controller.LineOfSightTo");
			struct {
				ACharacter* Other = nullptr;
				FVector ViewPoint;
				bool bAlternateChecks = false;
				bool ReturnValue = false;
			} params;
			params.Other = Other;
			params.ViewPoint = ViewPoint;
			params.bAlternateChecks = bAlternateChecks;
			ProcessEvent(this, fn, &params);
			return params.ReturnValue;
		}
	};



	struct ASessionGameState {


	};

	class UKismetMathLibrary {
	private:
		static inline UClass* defaultObj;
	public:
		static bool Init() {
			return defaultObj = UObject::FindObject<UClass>("Class Engine.KismetMathLibrary");
		}
		static FRotator NormalizedDeltaRotator(const struct FRotator& A, const struct FRotator& B) {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.KismetMathLibrary.NormalizedDeltaRotator");

			struct
			{
				struct FRotator                A;
				struct FRotator                B;
				struct FRotator                ReturnValue;
			} params;

			params.A = A;
			params.B = B;

			ProcessEvent(defaultObj, fn, &params);

			return params.ReturnValue;

		};
		static FRotator FindLookAtRotation(const FVector& Start, const FVector& Target) {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.KismetMathLibrary.FindLookAtRotation");

			struct {
				FVector Start;
				FVector Target;
				FRotator ReturnValue;
			} params;
			params.Start = Start;
			params.Target = Target;
			ProcessEvent(defaultObj, fn, &params);
			return params.ReturnValue;
		}
		static void DrawDebugBox(UObject* WorldContextObject, const FVector& Center, const FVector& Extent, const FLinearColor& LineColor, const FRotator& Rotation, float Duration) {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.KismetSystemLibrary.DrawDebugBox");
			struct
			{
				UObject* WorldContextObject = nullptr;
				FVector Center;
				FVector Extent;
				FLinearColor LineColor;
				FRotator Rotation;
				float Duration = INFINITY;
			} params;

			params.WorldContextObject = WorldContextObject;
			params.Center = Center;
			params.Extent = Extent;
			params.LineColor = LineColor;
			params.Rotation = Rotation;
			params.Duration = Duration;
			ProcessEvent(defaultObj, fn, &params);
		}
		static void DrawDebugArrow(UObject* WorldContextObject, const FVector& LineStart, const FVector& LineEnd, float ArrowSize, const FLinearColor& LineColor, float Duration) {
			static auto fn = UObject::FindObject<UFunction>("Function Engine.KismetSystemLibrary.DrawDebugBox");
			struct
			{
				class UObject* WorldContextObject = nullptr;
				struct FVector LineStart;
				struct FVector LineEnd;
				float ArrowSize = 1.f;
				struct FLinearColor LineColor;
				float Duration = 1.f;
			} params;

			params.WorldContextObject = WorldContextObject;
			params.LineStart = LineStart;
			params.LineEnd = LineEnd;
			params.ArrowSize = ArrowSize;
			params.LineColor = LineColor;
			params.Duration = Duration;

			ProcessEvent(defaultObj, fn, &params);
		}
	};



	struct UPlayer {
		char UnknownData00[0x30];
		AController* PlayerController;
	};

	struct UGameInstance {
		char UnknownData00[0x38];
		SDK::TArray<UPlayer*> LocalPlayers; // 0x38
	};

	struct ULevelActorContainer {
		char UnknownData00[0x28];
		SDK::TArray<struct AActor*> Actors; // 0x28(0x10)
	};

	struct ULevel {
		char UnknownData00[0xD8];
		ULevelActorContainer* ActorCluster; // 0xd8(0x08)
	};

	struct UWorld {
		static inline UWorld** GWorld = nullptr;
		char pad_28[0x30]; // 0x28(0x08)
		ULevel* PersistentLevel; // 0x30(0x08)
		char pad_38[0xEA]; // 0x38(0x08)
		ASessionGameState* GameState; // 0x120(0x08)
		char pad_128[0x10]; // 0x128(0x08)
		SDK::TArray<ULevel*> Levels; //0x138(0x10)
		ULevel* CurrentLevel;; // 0x148(0x10)
		char pad_158[0x28]; // 0x158(0x28)
		UGameInstance* GameInstance; //0x180(0x08)
	};

}
#ifdef _MSC_VER
#pragma pack(pop)
#endif