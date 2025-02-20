// Universal DX11 Hook With ImGui - Using Kiero.
// Put together by GHFear @ IllusorySoftware for the purpose of making mod menus for Single Player UE4 games. :)
// This also features an example mod for the game Session.


//INCLUDES

#include "stdafx.h"
#include <stdint.h>
#include <stdio.h>
#include <thread>
#include "MinHook/MinHook.h"
#include "proxy.h"
#include "global.h"
#include "Timer.h"
#include "util.h"
#include "SigScans.h"
#include "Pattern.h"
#include <sstream>
#include <string>
#include "UeStuff.h"
#include "processtools.h"
#include "memhack.h"
#include "Mod_UObjects.h"
#include "include/fmt/core.h"
#include <filesystem>
#include "processtools.h"

namespace fs = std::filesystem;

//imgui & Kiero INCLUDES
#include "memory.h"
#include <algorithm>
#include <map>
#include <psapi.h>
#include <sys/types.h>
#include <atlstr.h>
#include <regex>
#include "includes.h"
#include "ImGuiFileDialog.h"

//---------------------------------------------------------------------------------------------------- END OF INCLUDES


// Setup for the Proxy
EXTERN_C IMAGE_DOS_HEADER __ImageBase; // We might need this some day. Don't remove.
HMODULE ourModule = 0; // Declare our "extern HMODULE ourModule" from proxy.cpp inside dllmain, so we can pass hModule to Proxy_Attach();
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
Present oPresent;
ResizeBuffers oResizeBuffers;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
IDXGIOutput* pTarget;
bool init = false;
bool init_hook = false;
float Viewport_Width;
float Viewport_Height;
ImVec2 ViewportSize;

//Some vector array we use for some things
std::vector<uint64_t> FNamesIndexList = {};
std::vector<uint32> UObjectsIndexList = {};
std::vector<std::string> FNamesList = {};
std::vector<std::string> UObjectsList = {};
std::vector<void*> FNamesAddressList = {};
std::vector<void*> UObjectsAddressList = {};
std::vector<std::string> SongChannelListString = {};
std::vector<std::string> SongListString = {};
std::vector<std::string> UMapNameList = {};


//Patterns from Russells Source.

//FNamePool 
auto FPoolPat = Pattern::Find("74 09 48 8D 15 ? ? ? ? EB 16");
auto FPoolPatoffset = *reinterpret_cast<uint32_t*>(FPoolPat + 5);
auto FPool = (DWORD64)(FPoolPat + 9 + FPoolPatoffset);

//GNames
auto GNamePat = Pattern::Find("E8 ? ? ? ? 48 8B C3 48 89 1D ? ? ? ? 48 8B 5C 24");
auto GNamesAddress = *reinterpret_cast<uint32_t*>(GNamePat + 11);
auto GName = (DWORD64)(GNamePat + 15 + GNamesAddress);

//GObjects
auto GObjectPat = Pattern::Find("8B 46 10 3B 46 3C 75 0F 48 8B D6 48 8D 0D ? ? ? ? E8");
auto GObjectOffset = *reinterpret_cast<uint32_t*>(GObjectPat + 14);
auto GObject = (DWORD64)(GObjectPat + 18 + GObjectOffset);

//GWorld
auto GWorldPat = Pattern::Find("0F 2E ? 74 ? 48 8B 1D ? ? ? ? 48 85 DB 74");
auto GWorldAddress = *reinterpret_cast<uint32_t*>(GWorldPat + 8);
auto Gworld = (ModObjects::UWorld**)(GWorldPat + 12 + GWorldAddress);


// UWorld
inline ModObjects::UWorld* GetWorld()
{
	return *Gworld;
};



//Function Defs
void InitImGui();
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
HRESULT hkResizeBuffers(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
DWORD WINAPI MainThread(HMODULE hmod);
void Init_Hooks();


//---------------------------------------------------------------------------------------------------- END OF Function Defs


// YOU DON'T NEED TO TOUCH ANY OF THIS --------------------------------------------------------------------------------------v
// YOU DON'T NEED TO TOUCH ANY OF THIS --------------------------------------------------------------------------------------v
// YOU DON'T NEED TO TOUCH ANY OF THIS --------------------------------------------------------------------------------------v

//Start routine
BOOL APIENTRY DllMain(HMODULE hModule, int ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		ourModule = hModule;
		Proxy_Attach();
		Init_Hooks();
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr);
	}
	if (ul_reason_for_call == DLL_PROCESS_DETACH)
		Proxy_Detach();

	return TRUE;
}


// Set our important addresses, offsets and other values.
char* exe_base = nullptr;


void Init_Hooks() //Good to have.
{
	exe_base = (char*)GetModuleHandleA(NULL); //Get exe base

	if (!exe_base)
	{
		return;
	}

	MH_Initialize(); // Initialize MinHook

	DWORD dwOld;

}

// SDK Dumper Init (This is UnrealDumper by guttir)
STATUS Dumper::Init(int argc, char* argv[]) {
	for (auto i = 1; i < argc; i++) {
		auto arg = argv[i];
		if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) {
			printf("'-p' - dump only names and objects\n'-w' - wait for input (it gives me time to inject mods)");
			return STATUS::FAILED;
		}
		else if (!strcmp(arg, "-p")) {
			Full = false;
		}
		else if (!strcmp(arg, "-w")) {
			Wait = true;
		}
		else if (!strcmp(arg, "--spacing")) {
			Spacing = true;
		}
	}

	if (Wait) {
		system("pause");
	}

	uint32_t pid = 0;

	{
		pid = GetCurrentProcessId();
		if (!pid) {
			return STATUS::PROCESS_NOT_FOUND;
		};
	}

	if (!ReaderInit(pid)) {
		return STATUS::READER_ERROR;
	};


	fs::path processName;
	TCHAR Buffer[MAX_PATH];

	{
		wchar_t processPath[MAX_PATH]{};
		if (!GetModuleFileName(NULL, Buffer, MAX_PATH)) { return STATUS::CANNOT_GET_PROCNAME; };
		processName = fs::path(Buffer).filename();

	}


	std::wstring test(Buffer); //convert to wstring
	std::string filename(test.begin(), test.end()); //and convert to string.
	const size_t last_slash_idx = filename.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		filename.erase(0, last_slash_idx + 1);
	}

	// Remove extension if present.
	const size_t period_idx = filename.rfind('.');
	if (std::string::npos != period_idx)
	{
		filename.erase(period_idx);
	}

	TCHAR szProxyAddr[MAX_PATH];
	_tcscpy_s(szProxyAddr, CA2T(filename.c_str()));


	{
		std::string root = "";

		auto game = filename;
		Directory = "Games/" + game;
		fs::create_directories(Directory);

		auto [base, size] = GetModuleInfo(pid, processName);
		if (!(base && size)) {
			return STATUS::MODULE_NOT_FOUND;
		}
		Base = (uint64)base;
		std::vector<uint8*> image(size);
		if (!Read(base, image.data(), size)) {
			return STATUS::CANNOT_READ;
		}
		return EngineInit(filename, image.data());
	}
}

// SDK Dumper Dump function (This is UnrealDumper by guttir)
STATUS Dumper::Dump() {
	/*
	 * Names dumping.
	 * We go through each block, except last, that is not fully filled.
	 * In each block we calculate next entry depending on previous entry size.
	 */
	{
		FNamesIndexList.clear();
		FNamesList.clear();
		File file(Directory / "NamesDump.txt", "w");
		if (!file) { return STATUS::FILE_NOT_OPEN; }
		size_t size = 0;
		NamePoolData.Dump([&file, &size](std::string_view name, uint32_t id) {
			fmt::print(file, "[{:0>6}] {}\n", id, name);
			//FNamesIndexList.push_back(id);
			//FNamesList.push_back(std::string{ name });
			size++;
			});
		//fmt::print("Names: {}\n", size);
	}
	{
		// Why we need to iterate all objects twice? We dumping objects and filling
		// packages simultaneously.
		std::unordered_map<uint8*, std::vector<UE_UObject>> packages;
		{
			File file(Directory / "ObjectsDump.txt", "w");
			if (!file) { return STATUS::FILE_NOT_OPEN; }
			size_t size = 0;
			if (Full) {

				ObjObjects.Dump([&file, &size, &packages](UE_UObject object) {
					fmt::print(file, "[{:0>6}] <{}> {}\n", object.GetIndex(), object.GetAddress(), object.GetFullName());
					//UObjectsIndexList.push_back(object.GetIndex());
					//UObjectsList.push_back(object.GetFullName());
					//UObjectsAddressList.push_back(object.GetAddress());

					size++;
					if (object.IsA<UE_UStruct>() || object.IsA<UE_UEnum>()) {
						auto packageObj = object.GetPackageObject();
						packages[packageObj].push_back(object);
					}
					});
			}
			else {
				ObjObjects.Dump([&file, &size](UE_UObject object) {
					fmt::print(file, "[{:0>6}] <{}> {}\n", object.GetIndex(), object.GetAddress(), object.GetFullName());
					//UObjectsList.push_back(object.GetFullName());
					size++;
					});
			}

			//fmt::print("Objects: {}\n", size);
		}

		if (!Full) {
			return STATUS::SUCCESS;
		}

		//{
		//    // Clearing all packages with small amount of objects (comment this if
		//    you need all packages to be dumped) size_t size = packages.size();
		//    size_t erased = std::erase_if(packages, [](std::pair<byte* const,
		//    std::vector<UE_UObject>>& package) { return package.second.size() < 2;
		//    });

		//    fmt::print("Wiped {} out of {}\n", erased, size);
		//}

		// Checking if we have any package after clearing.
		if (!packages.size()) {
			return STATUS::ZERO_PACKAGES;
		}

		//fmt::print("Packages: {}\n", packages.size());

		
		{
			auto path = Directory / "DUMP";
			fs::create_directories(path);

			int i = 1;
			int saved = 0;
			std::string unsaved{};
			
			for (UE_UPackage package : packages) {

				package.Process();
				if (package.Save(path, Spacing)) {
					saved++;
				}
				else {
					unsaved += (package.GetObject().GetName() + ", ");
				};
			}
			

			//fmt::print("\nSaved packages: {}", saved);
			
			if (unsaved.size()) {
				unsaved.erase(unsaved.size() - 2);
				//fmt::print("\nUnsaved empty packages: [ {} ]", unsaved);
			}
		}
	}
	return STATUS::SUCCESS;
}


uint64_t FindFNameIndex(std::string s, std::vector<std::string> v) {
	for (uint64_t i = 0; i < v.size(); i++) {
		if (s.compare(v[i]) == 0) {
			return i;
		}
	}
	return -1;
}


//DumpStart function (UnrealDumper by guttir)
int dumpStart()
{
	auto dumper = Dumper::GetInstance();

	switch (dumper->Init(0, 0))
	{
		//case STATUS::WINDOW_NOT_FOUND: { puts("Can't find UE4 window"); return 1; }
	case STATUS::PROCESS_NOT_FOUND: { puts("Can't find process"); return 1; }
	case STATUS::READER_ERROR: { puts("Can't init reader"); return 1; }
	case STATUS::CANNOT_GET_PROCNAME: { puts("Can't get process name"); return 1; }
	case STATUS::ENGINE_NOT_FOUND: { puts("Can't find offsets for this game"); return 1; }
	case STATUS::ENGINE_FAILED: { puts("Can't init engine for this game"); return 1; }
	case STATUS::MODULE_NOT_FOUND: { puts("Can't enumerate modules (protected process?)"); return 1; }
	case STATUS::CANNOT_READ: { puts("Can't read process memory"); return 1; }
	case STATUS::INVALID_IMAGE: { puts("Can't get executable sections"); return 1; }
	case STATUS::OBJECTS_NOT_FOUND: { puts("Can't find objects array"); return 1; }
	case STATUS::NAMES_NOT_FOUND: { puts("Can't find names array"); return 1; }
	case STATUS::SUCCESS: { break; };
	default: { return 1; }
	}

	switch (dumper->Dump())
	{
	case STATUS::FILE_NOT_OPEN: { puts("Can't open file"); return 1; }
	case STATUS::ZERO_PACKAGES: { puts("Size of packages is zero"); return 1; }
	case STATUS::SUCCESS: { break; }
	default: { return 1; }
	}

	return 0;
}

// Group Panel Stuff
#define IMGUI_DEFINE_MATH_OPERATORS


static ImVector<ImRect> s_GroupPanelLabelStack;

void ImGui::BeginGroupPanel(const char* name, const ImVec2& size)
{
	ImGui::BeginGroup();

	auto cursorPos = ImGui::GetCursorScreenPos();
	auto itemSpacing = ImGui::GetStyle().ItemSpacing;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

	auto frameHeight = ImGui::GetFrameHeight();
	ImGui::BeginGroup();

	ImVec2 effectiveSize = size;
	if (size.x < 0.0f)
		effectiveSize.x = ImGui::GetContentRegionAvailWidth();
	else
		effectiveSize.x = size.x;
	ImGui::Dummy(ImVec2(effectiveSize.x, 0.0f));

	ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
	ImGui::SameLine(0.0f, 0.0f);
	ImGui::BeginGroup();
	ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
	ImGui::SameLine(0.0f, 0.0f);
	ImGui::TextUnformatted(name);
	auto labelMin = ImGui::GetItemRectMin();
	auto labelMax = ImGui::GetItemRectMax();
	ImGui::SameLine(0.0f, 0.0f);
	ImGui::Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));
	ImGui::BeginGroup();

	//ImGui::GetWindowDrawList()->AddRect(labelMin, labelMax, IM_COL32(255, 0, 255, 255));

	ImGui::PopStyleVar(2);

#if IMGUI_VERSION_NUM >= 17301
	ImGui::GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
	ImGui::GetCurrentWindow()->WorkRect.Max.x -= frameHeight * 0.5f;
	ImGui::GetCurrentWindow()->InnerRect.Max.x -= frameHeight * 0.5f;
#else

#endif
	ImGui::GetCurrentWindow()->Size.x -= frameHeight;

	auto itemWidth = ImGui::CalcItemWidth();
	ImGui::PushItemWidth(ImMax(0.0f, itemWidth - frameHeight));

	s_GroupPanelLabelStack.push_back(ImRect(labelMin, labelMax));
}

void ImGui::EndGroupPanel()
{
	ImGui::PopItemWidth();

	auto itemSpacing = ImGui::GetStyle().ItemSpacing;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

	auto frameHeight = ImGui::GetFrameHeight();

	ImGui::EndGroup();

	//ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 255, 0, 64), 4.0f);

	ImGui::EndGroup();

	ImGui::SameLine(0.0f, 0.0f);
	ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
	ImGui::Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));

	ImGui::EndGroup();

	auto itemMin = ImGui::GetItemRectMin();
	auto itemMax = ImGui::GetItemRectMax();
	//ImGui::GetWindowDrawList()->AddRectFilled(itemMin, itemMax, IM_COL32(255, 0, 0, 64), 4.0f);

	auto labelRect = s_GroupPanelLabelStack.back();
	s_GroupPanelLabelStack.pop_back();

	ImVec2 halfFrame = ImVec2(frameHeight * 0.25f, frameHeight);
	ImVec2 gg1 = ImVec2(itemMin.x + halfFrame.x, itemMin.y + halfFrame.y);
	ImVec2 gg2 = ImVec2(halfFrame.x, 0.0f);
	ImVec2 gg3 = ImVec2(itemMax.x - gg2.x, itemMax.y - gg2.y);
	ImRect frameRect = ImRect(gg1, gg3);
	labelRect.Min.x -= itemSpacing.x;
	labelRect.Max.x += itemSpacing.x;
	for (int i = 0; i < 4; ++i)
	{
		switch (i)
		{
			// left half-plane
		case 0: ImGui::PushClipRect(ImVec2(-FLT_MAX, -FLT_MAX), ImVec2(labelRect.Min.x, FLT_MAX), true); break;
			// right half-plane
		case 1: ImGui::PushClipRect(ImVec2(labelRect.Max.x, -FLT_MAX), ImVec2(FLT_MAX, FLT_MAX), true); break;
			// top
		case 2: ImGui::PushClipRect(ImVec2(labelRect.Min.x, -FLT_MAX), ImVec2(labelRect.Max.x, labelRect.Min.y), true); break;
			// bottom
		case 3: ImGui::PushClipRect(ImVec2(labelRect.Min.x, labelRect.Max.y), ImVec2(labelRect.Max.x, FLT_MAX), true); break;
		}

		ImGui::GetWindowDrawList()->AddRect(
			frameRect.Min, frameRect.Max,
			ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)),
			halfFrame.x);

		ImGui::PopClipRect();
	}

	ImGui::PopStyleVar(2);

#if IMGUI_VERSION_NUM >= 17301
	ImGui::GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
	ImGui::GetCurrentWindow()->WorkRect.Max.x += frameHeight * 0.5f;
	ImGui::GetCurrentWindow()->InnerRect.Max.x += frameHeight * 0.5f;
#else
	ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x += frameHeight * 0.5f;
#endif
	ImGui::GetCurrentWindow()->Size.x += frameHeight;

	ImGui::Dummy(ImVec2(0.0f, 0.0f));

	ImGui::EndGroup();
}



// Init ImGui
void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	//io.MouseDrawCursor = true;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);


}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}



//dx11 ResizeBuffers Hook
HRESULT hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{

	if (mainRenderTargetView) {
		pContext->OMSetRenderTargets(0, 0, 0);
		mainRenderTargetView->Release();
	}

	HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	ID3D11Texture2D* pBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
	// Perform error handling here!

	pDevice->CreateRenderTargetView(pBuffer, NULL, &mainRenderTargetView);
	// Perform error handling here!
	pBuffer->Release();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	// Set up the viewport.
	D3D11_VIEWPORT vp;
	vp.Width = Width;
	vp.Height = Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	//Set Global Vars Width and Height for ImGui
	Viewport_Height = Height;
	Viewport_Width = Width;
	ViewportSize = ImVec2(Width / 4, Height / 2);
	pContext->RSSetViewports(1, &vp);
	return hr;
}


//Convert wchar_t* to const char*
char* charbuffer;
const char* wchar_t_To_char(wchar_t* input) {
	// Count required buffer size (plus one for null-terminator).
	delete charbuffer;
	charbuffer = nullptr;
	size_t size = (wcslen(input) + 1) * sizeof(wchar_t);
	charbuffer = new char[size];
#ifdef __STDC_LIB_EXT1__
	// wcstombs_s is only guaranteed to be available if __STDC_LIB_EXT1__ is defined
	size_t convertedSize;
	std::wcstombs_s(&convertedSize, buffer, size, input, size);

#else
	std::wcstombs(charbuffer, input, size);
#endif

	/* Use the string stored in "buffer" variable */

	// Free allocated memory:
	return charbuffer;
}


// convert const char* to wchar_t*
wchar_t* GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

// YOU DON'T NEED TO TOUCH ANY OF THIS --------------------------------------------------------------------------------------^
// YOU DON'T NEED TO TOUCH ANY OF THIS --------------------------------------------------------------------------------------^
// YOU DON'T NEED TO TOUCH ANY OF THIS --------------------------------------------------------------------------------------^


//--------------------------------------------------------------------------------START MAKING MODS AFTER THIS POINT-----------------------------------------------------------------v
//--------------------------------------------------------------------------------START MAKING MODS AFTER THIS POINT-----------------------------------------------------------------v
//--------------------------------------------------------------------------------START MAKING MODS AFTER THIS POINT-----------------------------------------------------------------v
//--------------------------------------------------------------------------------START MAKING MODS AFTER THIS POINT-----------------------------------------------------------------v



//Declare our UObjects that we want to use for our mods
UE_UObject PlayerController;


//Declare our UFunctions that we want to use for our mods



//Our list of mods to update and keep valid (we can select which one we want to update by calling this function and giving it the case number corresponding to our mod UObject or UFunction)
bool ModUpdater(int i) 
{
	if (dumpStart() == 0)
	{
		switch (i) //you can add as many cases to this switch as you want
		{
		case 0: // Update ALL mods
			PlayerController = ObjObjects.FindObjectByTypeAndContainsString("PlayerController", ".PersistentLevel.", "PlayerController");
			return true;
		case 1:
			//Currently empty
			return true;
		case 2:
			//Currently empty
			return true;
		case 3:
			//Currently empty
			return true;
		case 4:
			PlayerController = ObjObjects.FindObjectByTypeAndContainsString("PlayerController", ".PersistentLevel.", "PlayerController");
			return true;
		case 5:
			//Currently empty
			return true;
		default:
			return false;
		}

	}
	else
	{
		MessageBox(NULL, L"SDK Dump / Reload Failed!", L"Illusory", MB_OK | MB_SYSTEMMODAL);
	}

}



//ImGui variables
bool Window1IsOpen = true;
bool Window2IsOpen = false;
bool bFirstStart = true;
bool bLoading = false;
bool bDoOnce = true;
float fov_float_buffer = 60.0f;


//ImGui Setttings
static bool no_titlebar = false;
static bool no_border = false;
static bool no_resize = false;
static bool auto_resize = false;
static bool no_move = false;
static bool no_close = true;
static bool no_scrollbar = false;
static bool no_collapse = true;
static bool no_menu = true;
static bool start_pos_set = false;
static bool start_pos2_set = false;


// Object callback for getting level actors
std::vector<UE_UObject> ActorsArray;
void LevelActorsObjectsCallback(uint8* callback)
{
	
	UE_UObject Level(callback);
	if (Level)
	{
		ModObjects::ULevel* OurLevel = (ModObjects::ULevel*)Level.GetAddress();

		for (size_t i = 0; i < OurLevel->WorldActors.Num(); i++)
		{

			UE_UObject LevelWrapper((byte*)OurLevel->WorldActors[i]);

			if (LevelWrapper)
			{

				ActorsArray.push_back(LevelWrapper);
			}
		}
		
	}
	
}



bool DumpActorList(std::string path)
{

	ActorsArray.clear();
	if (dumpStart() == 0)
	{
		auto UGetWorld = GetWorld();
		UE_UObject CurrentLevel((byte*)UGetWorld->PersistentLevel);
		ObjObjects.ForEachObjectOfClass(CurrentLevel.GetClass(), LevelActorsObjectsCallback); // Dump All actors from actor list in all persistent levels

		FILE* Log = NULL;
		fopen_s(&Log, path.c_str(), "w+");

		for (size_t i = 0; i < ActorsArray.size(); i++)
		{
			char buffer[256]; // <- danger, only storage for 256 characters.
			strncpy(buffer, "	|	", sizeof(buffer));
			strncat(buffer, ActorsArray[i].GetFullName().c_str(), sizeof(buffer));
			char* new_chars = reinterpret_cast<char*>(ActorsArray[i].GetAddress());
			fprintf(Log, "%p%s\n", ActorsArray[i].GetAddress(), buffer);
		}
		fclose(Log);
		return true;
	}
	else
	{
		
		return false;
	}


	return false;
}




void IsMenuOpen()
{



		bLoading = true;
		/*if (PlayerController.IsA(ModObjects::APlayerController::StaticClass()))
		{
			ModObjects::APlayerController* PlayerControllerAddr = (ModObjects::APlayerController*)PlayerController.GetAddress();
			PlayerControllerAddr->Pause(PlayerController);
			Window1IsOpen = !Window1IsOpen;
			Window2IsOpen = false;
			bLoading = false;
		}
		else
		{
			if (ModUpdater(4) == true)
			{
				if (PlayerController.IsA(ModObjects::APlayerController::StaticClass()))
				{
					ModObjects::APlayerController* PlayerControllerAddr = (ModObjects::APlayerController*)PlayerController.GetAddress();
					PlayerControllerAddr->Pause(PlayerController);
					Window1IsOpen = !Window1IsOpen;
					Window2IsOpen = false;
					bLoading = false;
				}
				else
				{
					bLoading = false;
				}

			}
			else
			{
				bLoading = false;
			}
		}*/


		if (ModUpdater(4) == true)
		{
			if (PlayerController.IsA(ModObjects::APlayerController::StaticClass()))
			{
				ModObjects::APlayerController* PlayerControllerAddr = (ModObjects::APlayerController*)PlayerController.GetAddress();
				PlayerControllerAddr->Pause(PlayerController);
				Window1IsOpen = !Window1IsOpen;
				Window2IsOpen = false;
				bLoading = false;
			}
			else
			{
				bLoading = false;
			}

		}
		else
		{
			bLoading = false;
		}


		bLoading = false;

}


//dx11 Present Hook (this is our User Interface and where we do most of our logic for our mods)
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{

			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;

		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	
	ImGui::NewFrame();

	// Our menu state
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	ImGuiWindowFlags	window_flags = 0;
	if (!no_titlebar)	window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_resize)		window_flags |= ImGuiWindowFlags_NoResize;
	if (auto_resize)	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	if (no_move)		window_flags |= ImGuiWindowFlags_NoMove;
	if (no_scrollbar)	window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (no_collapse)	window_flags |= ImGuiWindowFlags_NoCollapse;
	if (!no_menu)		window_flags |= ImGuiWindowFlags_MenuBar;

	if (Window1IsOpen == false)
	{
		ImGui::GetIO().MouseDrawCursor = false;
	}
	else
	{
		ImGui::GetIO().MouseDrawCursor = true;
	}


	if (GetAsyncKeyState(VK_RMENU) & 1) // Open IMGUI Menu
	{
		// Right ALT key is down
		IsMenuOpen();
	}

	float A_Button = ImGui::GetIO().NavInputs[0];
	float B_Button = ImGui::GetIO().NavInputs[1];
	float DPad_UP_Button = ImGui::GetIO().NavInputs[6];
	float LB_Button = ImGui::GetIO().NavInputs[12];
	float RB_Button = ImGui::GetIO().NavInputs[13];
	

	if (LB_Button == 1.0f && RB_Button == 1.0f && B_Button == 1.0f)
	{
		if (bDoOnce == true)
		{
			bDoOnce = false;
			IsMenuOpen();
		}

	}
	else
	{
		bDoOnce = true;
	}

	

	//Window 1 --------------------------------------------------------------------------------
	if (Window1IsOpen)
	{
		//ImGui::GetIO().MouseDrawCursor = true;

		if (!start_pos_set) { 
			ImGui::SetNextWindowPos(ImVec2(25, 25)); 
			ImGui::SetNextWindowSize(ImVec2(500.0f, 350.0f));
			start_pos_set = true;
		}
		ImGui::Begin("Session:Skate Sim | Mod Menu V3 - GHFear @ Illusory", &Window1IsOpen, window_flags);
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);
		ImGui::PushItemWidth(-240);

		if (bLoading)
		{
			ImGui::Text("Loading %c", "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
		}
		else
		{
			ImGui::Text("Session:Skate Sim | Mod Menu V3 - GHFear @ Illusory");
		}

		ImGui::Separator(); //Separating horizontal line

		if (ImGui::CollapsingHeader("Maps And levels"))
		{
			// BeginGroupPanel Starts Here -------------------------------------
			ImGui::BeginGroupPanel("Maps And Levels", ImVec2(0.0f, 0.0f));


			// Open custom maps window -----------------------------------------------------------------------------------------------------------------------
			if (ImGui::Button("Custom Maps", ImVec2(ImGui::GetWindowWidth() - 10.0f, 50.0f)))
			{
				Window2IsOpen = !Window2IsOpen;
			}
			// End of custom maps window ---------------------------------------------------------------------------------------------------------------------


			// Dump actor list -------------------------------------------------------------------------------------------------------------------------------
			if (ImGui::Button("Dump Level-Actor List", ImVec2(ImGui::GetWindowWidth() - 10.0f, 50.0f))) {
				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Set Filename", ".txt", "ActorList.", 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
			}

			// display our dialog
			if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", window_flags, ImVec2(ImGui::GetWindowWidth() / 2.0f, ImGui::GetWindowHeight() / 2.0f)))
			{
				// action if OK
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
					std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
					// action
					std::thread t([filePathName]() {
						bLoading = true;
						if (DumpActorList(filePathName) == true)
						{
							MessageBox(NULL, L"ActorList Dump Was Successful!", L"Illusory", MB_OK | MB_SYSTEMMODAL);

						}
						else
						{
							MessageBox(NULL, L"SDK Dump / Reload Failed!", L"Illusory", MB_OK | MB_SYSTEMMODAL);
						}
						bLoading = false;
					});
					t.detach();
				}
				// close
				ImGuiFileDialog::Instance()->Close();
			}
			// End Of Dump actor list -------------------------------------------------------------------------------------------------------------------------------

			ImGui::EndGroupPanel(); 
			// BeginGroupPanel Ends Here -------------------------------------
		}

		// Divider -------------------------------------


		ImGui::Separator(); //Separating horizontal line

		if (ImGui::CollapsingHeader("Universal Settings"))
		{
			// BeginGroupPanel Starts Here -------------------------------------
			ImGui::BeginGroupPanel("Universal Settings", ImVec2(0.0f, 0.0f));

			if (ImGui::InputFloat("FOV", &fov_float_buffer, 1.0f, 5.0f, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				if (ModUpdater(4) == true)
				{
					if (PlayerController.IsA(ModObjects::APlayerController::StaticClass()))
					{
						ModObjects::APlayerController* PlayerControllerAddr = (ModObjects::APlayerController*)PlayerController.GetAddress();
						PlayerControllerAddr->FOV(fov_float_buffer);
					}
				}
			}



			if (ImGui::Button("Pause / Unpause"))
			{
				if (ModUpdater(4) == true)
				{
					if (PlayerController.IsA(ModObjects::APlayerController::StaticClass()))
					{
						ModObjects::APlayerController* PlayerControllerAddr = (ModObjects::APlayerController*)PlayerController.GetAddress();
						PlayerControllerAddr->ServerPause();
					}
				}
			}


			ImGui::EndGroupPanel();
			// BeginGroupPanel Ends Here -------------------------------------
		}


		// Divider -------------------------------------


		if (ImGui::CollapsingHeader("SDK Dumping"))
		{
			ImGui::BeginGroupPanel("SDK Dumping", ImVec2(0.0f, 0.0f));
			if (ImGui::Button("Dump SDK", ImVec2(ImGui::GetWindowWidth() - 10.0f, 50.0f))) {
				//Do some code under here
				if (bLoading == false)
				{
					std::thread t([]() {
						bLoading = true;
						if (dumpStart() == 0)
						{
							ModUpdater(0); //Update All
							MessageBox(NULL, L"SDK Dump / Reload Success!", L"Illusory", MB_OK | MB_SYSTEMMODAL);
						}
						else
						{
							MessageBox(NULL, L"SDK Dump / Reload Failed!", L"Illusory", MB_OK | MB_SYSTEMMODAL);
						}
						bLoading = false;
						});

					t.detach();
				}
			}

			ImGui::EndGroupPanel();
			
			// Divider -------------------------------------

		}// Divider -------------------------------------

		ImGui::Separator(); //Separating horizontal line
		ImGui::Separator(); //Separating horizontal line
		ImGui::Separator(); //Separating horizontal line

		if (ImGui::Button("CLOSE MENU", ImVec2(ImGui::GetWindowWidth() - 10.0f, 50.0f)))
		{
			IsMenuOpen();
		}

		

		ImGui::End();
	}


	//Window 2 - Custom Maps ----------------------------------------------------------------
	if (Window2IsOpen)
	{
		
		ImGui::Begin("Custom Map Picker", &Window2IsOpen, window_flags);
		//ImGui::GetIO().MouseDrawCursor = true;

		if (bLoading)
		{
			ImGui::Text("Loading %c", "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
		}
		else
		{
			ImGui::Text("Our Custom Maps");
		}


		// Button to scan for maps starts here ------------------------------------------------------------------------------------------
		if (ImGui::Button("Scan For Maps!", ImVec2(ImGui::GetWindowWidth() - 10.0f, 50.0f)))
		{
			if (bLoading == false)
			{
				std::thread t([]() {
					bLoading = true;
					UMapNameList.clear();
					dumpStart(); //We need to do something better and check if failed later. Good for now.
					std::string path("../../Content/");
					std::string ext(".umap");

					for (auto& p : fs::recursive_directory_iterator(path))
					{
						if (p.path().extension() == ext)
						{
							std::string dir = p.path().stem().string();
							UMapNameList.push_back(dir);
						}
					}
					bLoading = false;
					});
				t.detach();
			}
		}

		// Button to scan for maps ends here ---------------------------------------------------------------------------------------------

		ImGui::NewLine();

		ImGui::Separator();

		// BeginGroupPanel Starts Here ---------------------------------------------------------------------------------------------
		ImGui::BeginGroupPanel("----List of our custom maps----", ImVec2(0.0f, 0.0f));

		for (int n = 0; n < UMapNameList.size(); n++)
		{
			if (ImGui::Button(UMapNameList[n].c_str(), ImVec2(ImGui::GetWindowWidth() - 10.0f, 50.0f)))
			{
				if (bLoading == false)
				{
					wchar_t* level = GetWC(UMapNameList[n].c_str());
					std::thread t([level]() {
						bLoading = true;
						/*if (PlayerController.IsA(ModObjects::APlayerController::StaticClass()))
						{
							ModObjects::APlayerController* PlayerControllerAddr = (ModObjects::APlayerController*)PlayerController.GetAddress();
							IsMenuOpen();
							PlayerControllerAddr->LocalTravel(level);
						}
						else
						{
							if (ModUpdater(4) == true)
							{
								if (PlayerController.IsA(ModObjects::APlayerController::StaticClass()))
								{
									ModObjects::APlayerController* PlayerControllerAddr = (ModObjects::APlayerController*)PlayerController.GetAddress();
									IsMenuOpen();
									PlayerControllerAddr->LocalTravel(level);
								}
							}
						}*/


						if (ModUpdater(4) == true)
						{
							if (PlayerController.IsA(ModObjects::APlayerController::StaticClass()))
							{
								ModObjects::APlayerController* PlayerControllerAddr = (ModObjects::APlayerController*)PlayerController.GetAddress();
								IsMenuOpen();
								PlayerControllerAddr->LocalTravel(level);
							}
						}


						bLoading = false;
						});
					t.detach();
				}
			}
			ImGui::Separator();
		}

		ImGui::EndGroupPanel();
		// BeginGroupPanel Ends Here ---------------------------------------------------------------------------------------------

		ImGui::End();
	}


	ImGui::Render();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (bFirstStart == true)
	{
		Window1IsOpen = false; //If we want our window closed or opened at launch
		bFirstStart = false; // First Start Bool
	}

	return oPresent(pSwapChain, SyncInterval, Flags);
}



//-----------------------------------------------------------------------------------------MODS END HERE-----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------MODS END HERE-----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------MODS END HERE-----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------MODS END HERE-----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------MODS END HERE-----------------------------------------------------------------------

// Not currently in use.
void UIControlsPolling() {

	EngineUtils::Timer* timer = EngineUtils::Timer::Instance();

	bool isRunning = true;
	float frameRate = 60.0f; // set the amount of ticks we want to poll our controller / keyboard / mouse for the UI inputs.


	while (isRunning) {

		timer->Tick();

		if (timer->DeltaTime() >= 1 / frameRate) {


			//Keypresses for controlling the imgui windows (we poll these controls in their own delta_time controller invironment)
			if (GetAsyncKeyState(VK_RMENU) & 1) // Open IMGUI Menu
			{
				// Right ALT key is down
				Window1IsOpen = !Window1IsOpen;
			}

			timer->Reset();
		}
	}

}



DWORD WINAPI MainThread(HMODULE hmod)
{

	bool init_hook = false;

	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
	{
		kiero::bind(8, (void**)&oPresent, hkPresent);
		kiero::bind(13, (void**)&oResizeBuffers, hkResizeBuffers);
		init_hook = true;
	}

	return TRUE;
}
