// based on https://github.com/nem0/LumixEngine/blob/master/external/imgui/imgui_dock.h

#pragma once

//namespace Lumix { namespace FS { class OsFile; } }
//struct lua_State;

#ifdef __cplusplus
extern "C" {
#else
typedef struct ImVec2 ImVec2;
typedef int ImGuiWindowFlags;
#endif

typedef enum ImGuiDockSlot
{
	ImGuiDockSlot_Left,
	ImGuiDockSlot_Right,
	ImGuiDockSlot_Top,
	ImGuiDockSlot_Bottom,
	ImGuiDockSlot_Tab,

	ImGuiDockSlot_Float,
	ImGuiDockSlot_None
} ImGuiDockSlot;

extern void BeginDockWorkspace();
extern void EndDockWorkspace();
extern void ShutdownDock();
extern void SetNextDock(ImGuiDockSlot slot);
extern bool BeginDock(const char* label, bool* opened, ImGuiWindowFlags extra_flags);
extern void EndDock();
extern void SetDockActive();
extern void DockDebugWindow();
//void SaveDock(Lumix::FS::OsFile& file);
//void LoadDock(lua_State* L);

#ifdef __cplusplus
}
#endif