// dear imgui, v1.60 WIP
// (main code and documentation)

// Call and read ImGui::ShowDemoWindow() in imgui_demo.cpp for demo code.
// Newcomers, read 'Programmer guide' below for notes on how to setup Dear ImGui in your codebase.
// Get latest version at https://github.com/ocornut/imgui
// Releases change-log at https://github.com/ocornut/imgui/releases
// Gallery (please post your screenshots/video there!): https://github.com/ocornut/imgui/issues/1269
// Developed by Omar Cornut and every direct or indirect contributors to the GitHub.
// This library is free but I need your support to sustain development and maintenance.
// If you work for a company, please consider financial support, see README. For individuals: https://www.patreon.com/imgui

// It is recommended that you don't modify imgui.cpp! It will become difficult for you to update the library.
// Note that 'ImGui::' is a namespace and so you can add functions into it from your own source files without modifying imgui.h or imgui.cpp.
// You may include imgui_internal.h to access internal data structures, but it doesn't come with any guarantee of forward compatibility.
// Discussing your changes on the GitHub Issue Tracker may lead you to a better solution or official support for them.

/*

 Index
 - MISSION STATEMENT
 - END-USER GUIDE
 - PROGRAMMER GUIDE (read me!)
   - Read first
   - How to update to a newer version of Dear ImGui
   - Getting started with integrating Dear ImGui in your code/engine
   - Using gamepad/keyboard navigation controls [BETA]
 - API BREAKING CHANGES (read me when you update!)
 - ISSUES & TODO LIST
 - FREQUENTLY ASKED QUESTIONS (FAQ), TIPS
   - How can I tell whether to dispatch mouse/keyboard to imgui or to my application?
   - How can I display an image? What is ImTextureID, how does it works?
   - How can I have multiple widgets with the same label? Can I have widget without a label? (Yes). A primer on labels and the ID stack.
   - How can I load a different font than the default?
   - How can I easily use icons in my application?
   - How can I load multiple fonts?
   - How can I display and input non-latin characters such as Chinese, Japanese, Korean, Cyrillic?
   - How can I use the drawing facilities without an ImGui window? (using ImDrawList API)
   - I integrated Dear ImGui in my engine and the text or lines are blurry..
   - I integrated Dear ImGui in my engine and some elements are clipping or disappearing when I move windows around..
   - How can I help?
 - ISSUES & TODO-LIST
 - CODE


 MISSION STATEMENT
 =================

 - Easy to use to create code-driven and data-driven tools
 - Easy to use to create ad hoc short-lived tools and long-lived, more elaborate tools
 - Easy to hack and improve
 - Minimize screen real-estate usage
 - Minimize setup and maintenance
 - Minimize state storage on user side
 - Portable, minimize dependencies, run on target (consoles, phones, etc.)
 - Efficient runtime and memory consumption (NB- we do allocate when "growing" content e.g. creating a window, opening a tree node
   for the first time, etc. but a typical frame won't allocate anything)

 Designed for developers and content-creators, not the typical end-user! Some of the weaknesses includes:
 - Doesn't look fancy, doesn't animate
 - Limited layout features, intricate layouts are typically crafted in code


 END-USER GUIDE
 ==============

 - Double-click on title bar to collapse window.
 - Click upper right corner to close a window, available when 'bool* p_open' is passed to ImGui::Begin().
 - Click and drag on lower right corner to resize window (double-click to auto fit window to its contents).
 - Click and drag on any empty space to move window.
 - TAB/SHIFT+TAB to cycle through keyboard editable fields.
 - CTRL+Click on a slider or drag box to input value as text.
 - Use mouse wheel to scroll.
 - Text editor:
   - Hold SHIFT or use mouse to select text.
   - CTRL+Left/Right to word jump.
   - CTRL+Shift+Left/Right to select words.
   - CTRL+A our Double-Click to select all.
   - CTRL+X,CTRL+C,CTRL+V to use OS clipboard/
   - CTRL+Z,CTRL+Y to undo/redo.
   - ESCAPE to revert text to its original value.
   - You can apply arithmetic operators +,*,/ on numerical values. Use +- to subtract (because - would set a negative value!)
   - Controls are automatically adjusted for OSX to match standard OSX text editing operations.
 - General Keyboard controls: enable with ImGuiConfigFlags_NavEnableKeyboard.
 - General Gamepad controls: enable with ImGuiConfigFlags_NavEnableGamepad. See suggested mappings in imgui.h ImGuiNavInput_ + download PNG/PSD at goo.gl/9LgVZW.


 PROGRAMMER GUIDE
 ================

 READ FIRST

 - Read the FAQ below this section!
 - Your code creates the UI, if your code doesn't run the UI is gone! == very dynamic UI, no construction/destructions steps, less data retention
   on your side, no state duplication, less sync, less bugs.
 - Call and read ImGui::ShowDemoWindow() for demo code demonstrating most features.
 - You can learn about immediate-mode gui principles at http://www.johno.se/book/imgui.html or watch http://mollyrocket.com/861

 HOW TO UPDATE TO A NEWER VERSION OF DEAR IMGUI

 - Overwrite all the sources files except for imconfig.h (if you have made modification to your copy of imconfig.h)
 - Read the "API BREAKING CHANGES" section (below). This is where we list occasional API breaking changes.
   If a function/type has been renamed / or marked obsolete, try to fix the name in your code before it is permanently removed from the public API.
   If you have a problem with a missing function/symbols, search for its name in the code, there will likely be a comment about it.
   Please report any issue to the GitHub page!
 - Try to keep your copy of dear imgui reasonably up to date.

 GETTING STARTED WITH INTEGRATING DEAR IMGUI IN YOUR CODE/ENGINE

 - Add the Dear ImGui source files to your projects, using your preferred build system.
   It is recommended you build the .cpp files as part of your project and not as a library.
 - You can later customize the imconfig.h file to tweak some compilation time behavior, such as integrating imgui types with your own maths types.
 - See examples/ folder for standalone sample applications.
 - You may be able to grab and copy a ready made imgui_impl_*** file from the examples/.
 - When using Dear ImGui, your programming IDE is your friend: follow the declaration of variables, functions and types to find comments about them.

 - Init: retrieve the ImGuiIO structure with ImGui::GetIO() and fill the fields marked 'Settings': at minimum you need to set io.DisplaySize
   (application resolution). Later on you will fill your keyboard mapping, clipboard handlers, and other advanced features but for a basic
   integration you don't need to worry about it all.
 - Init: call io.Fonts->GetTexDataAsRGBA32(...), it will build the font atlas texture, then load the texture pixels into graphics memory.
 - Every frame:
	- In your main loop as early a possible, fill the IO fields marked 'Input' (e.g. mouse position, buttons, keyboard info, etc.)
	- Call ImGui::NewFrame() to begin the frame
	- You can use any ImGui function you want between NewFrame() and Render()
	- Call ImGui::Render() as late as you can to end the frame and finalize render data. it will call your io.RenderDrawListFn handler.
	   (Even if you don't render, call Render() and ignore the callback, or call EndFrame() instead. Otherwhise some features will break)
 - All rendering information are stored into command-lists until ImGui::Render() is called.
 - Dear ImGui never touches or knows about your GPU state. the only function that knows about GPU is the RenderDrawListFn handler that you provide.
 - Effectively it means you can create widgets at any time in your code, regardless of considerations of being in "update" vs "render" phases
   of your own application.
 - Refer to the examples applications in the examples/ folder for instruction on how to setup your code.
 - A minimal application skeleton may be:

	 // Application init
	 ImGui::CreateContext();
	 ImGuiIO& io = ImGui::GetIO();
	 io.DisplaySize.x = 1920.0f;
	 io.DisplaySize.y = 1280.0f;
	 // TODO: Fill others settings of the io structure later.

	 // Load texture atlas (there is a default font so you don't need to care about choosing a font yet)
	 unsigned char* pixels;
	 int width, height;
	 io.Fonts->GetTexDataAsRGBA32(pixels, &width, &height);
	 // TODO: At this points you've got the texture data and you need to upload that your your graphic system:
	 MyTexture* texture = MyEngine::CreateTextureFromMemoryPixels(pixels, width, height, TEXTURE_TYPE_RGBA)
	 // TODO: Store your texture pointer/identifier (whatever your engine uses) in 'io.Fonts->TexID'. This will be passed back to your via the renderer.
	 io.Fonts->TexID = (void*)texture;

	 // Application main loop
	 while (true)
	 {
		// Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = 1.0f/60.0f;
		io.MousePos = mouse_pos;
		io.MouseDown[0] = mouse_button_0;
		io.MouseDown[1] = mouse_button_1;

		// Call NewFrame(), after this point you can use ImGui::* functions anytime
		ImGui::NewFrame();

		// Most of your application code here
		MyGameUpdate(); // may use any ImGui functions, e.g. ImGui::Begin("My window"); ImGui::Text("Hello, world!"); ImGui::End();
		MyGameRender(); // may use any ImGui functions as well!

		// Render & swap video buffers
		ImGui::Render();
		MyImGuiRenderFunction(ImGui::GetDrawData());
		SwapBuffers();
	 }

	 // Shutdown
	 ImGui::DestroyContext();


 - A minimal render function skeleton may be:

	void void MyRenderFunction(ImDrawData* draw_data)
	{
	   // TODO: Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
	   // TODO: Setup viewport, orthographic projection matrix
	   // TODO: Setup shader: vertex { float2 pos, float2 uv, u32 color }, fragment shader sample color from 1 texture, multiply by vertex color.
	   for (int n = 0; n < draw_data->CmdListsCount; n++)
	   {
		  const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;  // vertex buffer generated by ImGui
		  const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;   // index buffer generated by ImGui
		  for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		  {
			 const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			 if (pcmd->UserCallback)
			 {
				 pcmd->UserCallback(cmd_list, pcmd);
			 }
			 else
			 {
				 // The texture for the draw call is specified by pcmd->TextureId.
				 // The vast majority of draw calls with use the imgui texture atlas, which value you have set yourself during initialization.
				 MyEngineBindTexture(pcmd->TextureId);

				 // We are using scissoring to clip some objects. All low-level graphics API supports it.
				 // If your engine doesn't support scissoring yet, you will get some small glitches (some elements outside their bounds) which you can fix later.
				 MyEngineScissor((int)pcmd->ClipRect.x, (int)pcmd->ClipRect.y, (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));

				 // Render 'pcmd->ElemCount/3' indexed triangles.
				 // By default the indices ImDrawIdx are 16-bits, you can change them to 32-bits if your engine doesn't support 16-bits indices.
				 MyEngineDrawIndexedTriangles(pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer, vtx_buffer);
			 }
			 idx_buffer += pcmd->ElemCount;
		  }
	   }
	}

 - The examples/ folders contains many functional implementation of the pseudo-code above.
 - When calling NewFrame(), the 'io.WantCaptureMouse'/'io.WantCaptureKeyboard'/'io.WantTextInput' flags are updated.
   They tell you if ImGui intends to use your inputs. So for example, if 'io.WantCaptureMouse' is set you would typically want to hide
   mouse inputs from the rest of your application. Read the FAQ below for more information about those flags.

 USING GAMEPAD/KEYBOARD NAVIGATION CONTROLS [BETA]

 - The gamepad/keyboard navigation is in Beta. Ask questions and report issues at https://github.com/ocornut/imgui/issues/787
 - The initial focus was to support game controllers, but keyboard is becoming increasingly and decently usable.
 - Keyboard:
	- Set io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard to enable. NewFrame() will automatically fill io.NavInputs[] based on your io.KeyDown[] + io.KeyMap[] arrays.
	- When keyboard navigation is active (io.NavActive + ImGuiConfigFlags_NavEnableKeyboard), the io.WantCaptureKeyboard flag will be set.
	  For more advanced uses, you may want to read from:
	   - io.NavActive: true when a window is focused and it doesn't have the ImGuiWindowFlags_NoNavInputs flag set.
	   - io.NavVisible: true when the navigation cursor is visible (and usually goes false when mouse is used).
	   - or query focus information with e.g. IsWindowFocused(), IsItemFocused() etc. functions.
	  Please reach out if you think the game vs navigation input sharing could be improved.
 - Gamepad:
	- Set io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad to enable.
	- Backend: Set io.BackendFlags |= ImGuiBackendFlags_HasGamepad + fill the io.NavInputs[] fields before calling NewFrame(). Note that io.NavInputs[] is cleared by EndFrame().
	- See 'enum ImGuiNavInput_' in imgui.h for a description of inputs. For each entry of io.NavInputs[], set the following values:
		 0.0f= not held. 1.0f= fully held. Pass intermediate 0.0f..1.0f values for analog triggers/sticks.
	- We uses a simple >0.0f test for activation testing, and won't attempt to test for a dead-zone.
	  Your code will probably need to transform your raw inputs (such as e.g. remapping your 0.2..0.9 raw input range to 0.0..1.0 imgui range, etc.).
	- You can download PNG/PSD files depicting the gamepad controls for common controllers at: goo.gl/9LgVZW.
	- If you need to share inputs between your game and the imgui parts, the easiest approach is to go all-or-nothing, with a buttons combo to toggle the target.
	  Please reach out if you think the game vs navigation input sharing could be improved.
 - Mouse:
	- PS4 users: Consider emulating a mouse cursor with DualShock4 touch pad or a spare analog stick as a mouse-emulation fallback.
	- Consoles/Tablet/Phone users: Consider using a Synergy 1.x server (on your PC) + uSynergy.c (on your console/tablet/phone app) to share your PC mouse/keyboard.
	- On a TV/console system where readability may be lower or mouse inputs may be awkward, you may want to set the ImGuiConfigFlags_NavEnableSetMousePos flag.
	  Enabling ImGuiConfigFlags_NavEnableSetMousePos + ImGuiBackendFlags_HasSetMousePos instructs dear imgui to move your mouse cursor along with navigation movements.
	  When enabled, the NewFrame() function may alter 'io.MousePos' and set 'io.WantSetMousePos' to notify you that it wants the mouse cursor to be moved.
	  When that happens your back-end NEEDS to move the OS or underlying mouse cursor on the next frame. Some of the binding in examples/ do that.
	  (If you set the NavEnableSetMousePos flag but don't honor 'io.WantSetMousePos' properly, imgui will misbehave as it will see your mouse as moving back and forth!)
	  (In a setup when you may not have easy control over the mouse cursor, e.g. uSynergy.c doesn't expose moving remote mouse cursor, you may want
	   to set a boolean to ignore your other external mouse positions until the external source is moved again.)


 API BREAKING CHANGES
 ====================

 Occasionally introducing changes that are breaking the API. The breakage are generally minor and easy to fix.
 Here is a change-log of API breaking changes, if you are using one of the functions listed, expect to have to fix some code.
 Also read releases logs https://github.com/ocornut/imgui/releases for more details.

 - 2018/03/20 (1.60) - Renamed io.WantMoveMouse to io.WantSetMousePos for consistency and ease of understanding (was added in 1.52, _not_ used by core and only honored by some binding ahead of merging the Nav branch).
 - 2018/03/12 (1.60) - Removed ImGuiCol_CloseButton, ImGuiCol_CloseButtonActive, ImGuiCol_CloseButtonHovered as the closing cross uses regular button colors now.
 - 2018/03/08 (1.60) - Changed ImFont::DisplayOffset.y to default to 0 instead of +1. Fixed rounding of Ascent/Descent to match TrueType renderer. If you were adding or subtracting to ImFont::DisplayOffset check if your fonts are correctly aligned vertically.
 - 2018/03/03 (1.60) - Renamed ImGuiStyleVar_Count_ to ImGuiStyleVar_COUNT and ImGuiMouseCursor_Count_ to ImGuiMouseCursor_COUNT for consistency with other public enums.
 - 2018/02/18 (1.60) - BeginDragDropSource(): temporarily removed the optional mouse_button=0 parameter because it is not really usable in many situations at the moment.
 - 2018/02/16 (1.60) - obsoleted the io.RenderDrawListsFn callback, you can call your graphics engine render function after ImGui::Render(). Use ImGui::GetDrawData() to retrieve the ImDrawData* to display.
 - 2018/02/07 (1.60) - reorganized context handling to be more explicit,
					   - YOU NOW NEED TO CALL ImGui::CreateContext() AT THE BEGINNING OF YOUR APP, AND CALL ImGui::DestroyContext() AT THE END.
					   - removed Shutdown() function, as DestroyContext() serve this purpose.
					   - you may pass a ImFontAtlas* pointer to CreateContext() to share a font atlas between contexts. Otherwhise CreateContext() will create its own font atlas instance.
					   - removed allocator parameters from CreateContext(), they are now setup with SetAllocatorFunctions(), and shared by all contexts.
					   - removed the default global context and font atlas instance, which were confusing for users of DLL reloading and users of multiple contexts.
 - 2018/01/31 (1.60) - moved sample TTF files from extra_fonts/ to misc/fonts/. If you loaded files directly from the imgui repo you may need to update your paths.
 - 2018/01/11 (1.60) - obsoleted IsAnyWindowHovered() in favor of IsWindowHovered(ImGuiHoveredFlags_AnyWindow). Kept redirection function (will obsolete).
 - 2018/01/11 (1.60) - obsoleted IsAnyWindowFocused() in favor of IsWindowFocused(ImGuiFocusedFlags_AnyWindow). Kept redirection function (will obsolete).
 - 2018/01/03 (1.60) - renamed ImGuiSizeConstraintCallback to ImGuiSizeCallback, ImGuiSizeConstraintCallbackData to ImGuiSizeCallbackData.
 - 2017/12/29 (1.60) - removed CalcItemRectClosestPoint() which was weird and not really used by anyone except demo code. If you need it it's easy to replicate on your side.
 - 2017/12/24 (1.53) - renamed the emblematic ShowTestWindow() function to ShowDemoWindow(). Kept redirection function (will obsolete).
 - 2017/12/21 (1.53) - ImDrawList: renamed style.AntiAliasedShapes to style.AntiAliasedFill for consistency and as a way to explicitly break code that manipulate those flag at runtime. You can now manipulate ImDrawList::Flags
 - 2017/12/21 (1.53) - ImDrawList: removed 'bool anti_aliased = true' final parameter of ImDrawList::AddPolyline() and ImDrawList::AddConvexPolyFilled(). Prefer manipulating ImDrawList::Flags if you need to toggle them during the frame.
 - 2017/12/14 (1.53) - using the ImGuiWindowFlags_NoScrollWithMouse flag on a child window forwards the mouse wheel event to the parent window, unless either ImGuiWindowFlags_NoInputs or ImGuiWindowFlags_NoScrollbar are also set.
 - 2017/12/13 (1.53) - renamed GetItemsLineHeightWithSpacing() to GetFrameHeightWithSpacing(). Kept redirection function (will obsolete).
 - 2017/12/13 (1.53) - obsoleted IsRootWindowFocused() in favor of using IsWindowFocused(ImGuiFocusedFlags_RootWindow). Kept redirection function (will obsolete).
					 - obsoleted IsRootWindowOrAnyChildFocused() in favor of using IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows). Kept redirection function (will obsolete).
 - 2017/12/12 (1.53) - renamed ImGuiTreeNodeFlags_AllowOverlapMode to ImGuiTreeNodeFlags_AllowItemOverlap. Kept redirection enum (will obsolete).
 - 2017/12/10 (1.53) - removed SetNextWindowContentWidth(), prefer using SetNextWindowContentSize(). Kept redirection function (will obsolete).
 - 2017/11/27 (1.53) - renamed ImGuiTextBuffer::append() helper to appendf(), appendv() to appendfv(). If you copied the 'Log' demo in your code, it uses appendv() so that needs to be renamed.
 - 2017/11/18 (1.53) - Style, Begin: removed ImGuiWindowFlags_ShowBorders window flag. Borders are now fully set up in the ImGuiStyle structure (see e.g. style.FrameBorderSize, style.WindowBorderSize). Use ImGui::ShowStyleEditor() to look them up.
					   Please note that the style system will keep evolving (hopefully stabilizing in Q1 2018), and so custom styles will probably subtly break over time. It is recommended you use the StyleColorsClassic(), StyleColorsDark(), StyleColorsLight() functions.
 - 2017/11/18 (1.53) - Style: removed ImGuiCol_ComboBg in favor of combo boxes using ImGuiCol_PopupBg for consistency.
 - 2017/11/18 (1.53) - Style: renamed ImGuiCol_ChildWindowBg to ImGuiCol_ChildBg.
 - 2017/11/18 (1.53) - Style: renamed style.ChildWindowRounding to style.ChildRounding, ImGuiStyleVar_ChildWindowRounding to ImGuiStyleVar_ChildRounding.
 - 2017/11/02 (1.53) - obsoleted IsRootWindowOrAnyChildHovered() in favor of using IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
 - 2017/10/24 (1.52) - renamed IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS/IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS to IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS/IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS for consistency.
 - 2017/10/20 (1.52) - changed IsWindowHovered() default parameters behavior to return false if an item is active in another window (e.g. click-dragging item from another window to this window). You can use the newly introduced IsWindowHovered() flags to requests this specific behavior if you need it.
 - 2017/10/20 (1.52) - marked IsItemHoveredRect()/IsMouseHoveringWindow() as obsolete, in favor of using the newly introduced flags for IsItemHovered() and IsWindowHovered(). See https://github.com/ocornut/imgui/issues/1382 for details.
					   removed the IsItemRectHovered()/IsWindowRectHovered() names introduced in 1.51 since they were merely more consistent names for the two functions we are now obsoleting.
 - 2017/10/17 (1.52) - marked the old 5-parameters version of Begin() as obsolete (still available). Use SetNextWindowSize()+Begin() instead!
 - 2017/10/11 (1.52) - renamed AlignFirstTextHeightToWidgets() to AlignTextToFramePadding(). Kept inline redirection function (will obsolete).
 - 2017/09/25 (1.52) - removed SetNextWindowPosCenter() because SetNextWindowPos() now has the optional pivot information to do the same and more. Kept redirection function (will obsolete).
 - 2017/08/25 (1.52) - io.MousePos needs to be set to ImVec2(-FLT_MAX,-FLT_MAX) when mouse is unavailable/missing. Previously ImVec2(-1,-1) was enough but we now accept negative mouse coordinates. In your binding if you need to support unavailable mouse, make sure to replace "io.MousePos = ImVec2(-1,-1)" with "io.MousePos = ImVec2(-FLT_MAX,-FLT_MAX)".
 - 2017/08/22 (1.51) - renamed IsItemHoveredRect() to IsItemRectHovered(). Kept inline redirection function (will obsolete). -> (1.52) use IsItemHovered(ImGuiHoveredFlags_RectOnly)!
					 - renamed IsMouseHoveringAnyWindow() to IsAnyWindowHovered() for consistency. Kept inline redirection function (will obsolete).
					 - renamed IsMouseHoveringWindow() to IsWindowRectHovered() for consistency. Kept inline redirection function (will obsolete).
 - 2017/08/20 (1.51) - renamed GetStyleColName() to GetStyleColorName() for consistency.
 - 2017/08/20 (1.51) - added PushStyleColor(ImGuiCol idx, ImU32 col) overload, which _might_ cause an "ambiguous call" compilation error if you are using ImColor() with implicit cast. Cast to ImU32 or ImVec4 explicily to fix.
 - 2017/08/15 (1.51) - marked the weird IMGUI_ONCE_UPON_A_FRAME helper macro as obsolete. prefer using the more explicit ImGuiOnceUponAFrame.
 - 2017/08/15 (1.51) - changed parameter order for BeginPopupContextWindow() from (const char*,int buttons,bool also_over_items) to (const char*,int buttons,bool also_over_items). Note that most calls relied on default parameters completely.
 - 2017/08/13 (1.51) - renamed ImGuiCol_Columns*** to ImGuiCol_Separator***. Kept redirection enums (will obsolete).
 - 2017/08/11 (1.51) - renamed ImGuiSetCond_*** types and flags to ImGuiCond_***. Kept redirection enums (will obsolete).
 - 2017/08/09 (1.51) - removed ValueColor() helpers, they are equivalent to calling Text(label) + SameLine() + ColorButton().
 - 2017/08/08 (1.51) - removed ColorEditMode() and ImGuiColorEditMode in favor of ImGuiColorEditFlags and parameters to the various Color*() functions. The SetColorEditOptions() allows to initialize default but the user can still change them with right-click context menu.
					 - changed prototype of 'ColorEdit4(const char* label, float col[4], bool show_alpha = true)' to 'ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0)', where passing flags = 0x01 is a safe no-op (hello dodgy backward compatibility!). - check and run the demo window, under "Color/Picker Widgets", to understand the various new options.
					 - changed prototype of rarely used 'ColorButton(ImVec4 col, bool small_height = false, bool outline_border = true)' to 'ColorButton(const char* desc_id, ImVec4 col, ImGuiColorEditFlags flags = 0, ImVec2 size = ImVec2(0,0))'
 - 2017/07/20 (1.51) - removed IsPosHoveringAnyWindow(ImVec2), which was partly broken and misleading. ASSERT + redirect user to io.WantCaptureMouse
 - 2017/05/26 (1.50) - removed ImFontConfig::MergeGlyphCenterV in favor of a more multipurpose ImFontConfig::GlyphOffset.
 - 2017/05/01 (1.50) - renamed ImDrawList::PathFill() (rarely used directly) to ImDrawList::PathFillConvex() for clarity.
 - 2016/11/06 (1.50) - BeginChild(const char*) now applies the stack id to the provided label, consistently with other functions as it should always have been. It shouldn't affect you unless (extremely unlikely) you were appending multiple times to a same child from different locations of the stack id. If that's the case, generate an id with GetId() and use it instead of passing string to BeginChild().
 - 2016/10/15 (1.50) - avoid 'void* user_data' parameter to io.SetClipboardTextFn/io.GetClipboardTextFn pointers. We pass io.ClipboardUserData to it.
 - 2016/09/25 (1.50) - style.WindowTitleAlign is now a ImVec2 (ImGuiAlign enum was removed). set to (0.5f,0.5f) for horizontal+vertical centering, (0.0f,0.0f) for upper-left, etc.
 - 2016/07/30 (1.50) - SameLine(x) with x>0.0f is now relative to left of column/group if any, and not always to left of window. This was sort of always the intent and hopefully breakage should be minimal.
 - 2016/05/12 (1.49) - title bar (using ImGuiCol_TitleBg/ImGuiCol_TitleBgActive colors) isn't rendered over a window background (ImGuiCol_WindowBg color) anymore.
					   If your TitleBg/TitleBgActive alpha was 1.0f or you are using the default theme it will not affect you.
					   However if your TitleBg/TitleBgActive alpha was <1.0f you need to tweak your custom theme to readjust for the fact that we don't draw a WindowBg background behind the title bar.
					   This helper function will convert an old TitleBg/TitleBgActive color into a new one with the same visual output, given the OLD color and the OLD WindowBg color.
						   ImVec4 ConvertTitleBgCol(const ImVec4& win_bg_col, const ImVec4& title_bg_col)
						   {
							   float new_a = 1.0f - ((1.0f - win_bg_col.w) * (1.0f - title_bg_col.w)), k = title_bg_col.w / new_a;
							   return ImVec4((win_bg_col.x * win_bg_col.w + title_bg_col.x) * k, (win_bg_col.y * win_bg_col.w + title_bg_col.y) * k, (win_bg_col.z * win_bg_col.w + title_bg_col.z) * k, new_a);
						   }
					   If this is confusing, pick the RGB value from title bar from an old screenshot and apply this as TitleBg/TitleBgActive. Or you may just create TitleBgActive from a tweaked TitleBg color.
 - 2016/05/07 (1.49) - removed confusing set of GetInternalState(), GetInternalStateSize(), SetInternalState() functions. Now using CreateContext(), DestroyContext(), GetCurrentContext(), SetCurrentContext().
 - 2016/05/02 (1.49) - renamed SetNextTreeNodeOpened() to SetNextTreeNodeOpen(), no redirection.
 - 2016/05/01 (1.49) - obsoleted old signature of CollapsingHeader(const char* label, const char* str_id = NULL, bool display_frame = true, bool default_open = false) as extra parameters were badly designed and rarely used. You can replace the "default_open = true" flag in new API with CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen).
 - 2016/04/26 (1.49) - changed ImDrawList::PushClipRect(ImVec4 rect) to ImDraw::PushClipRect(Imvec2 min,ImVec2 max,bool intersect_with_current_clip_rect=false). Note that higher-level ImGui::PushClipRect() is preferable because it will clip at logic/widget level, whereas ImDrawList::PushClipRect() only affect your renderer.
 - 2016/04/03 (1.48) - removed style.WindowFillAlphaDefault setting which was redundant. Bake default BG alpha inside style.Colors[ImGuiCol_WindowBg] and all other Bg color values. (ref github issue #337).
 - 2016/04/03 (1.48) - renamed ImGuiCol_TooltipBg to ImGuiCol_PopupBg, used by popups/menus and tooltips. popups/menus were previously using ImGuiCol_WindowBg. (ref github issue #337)
 - 2016/03/21 (1.48) - renamed GetWindowFont() to GetFont(), GetWindowFontSize() to GetFontSize(). Kept inline redirection function (will obsolete).
 - 2016/03/02 (1.48) - InputText() completion/history/always callbacks: if you modify the text buffer manually (without using DeleteChars()/InsertChars() helper) you need to maintain the BufTextLen field. added an assert.
 - 2016/01/23 (1.48) - fixed not honoring exact width passed to PushItemWidth(), previously it would add extra FramePadding.x*2 over that width. if you had manual pixel-perfect alignment in place it might affect you.
 - 2015/12/27 (1.48) - fixed ImDrawList::AddRect() which used to render a rectangle 1 px too large on each axis.
 - 2015/12/04 (1.47) - renamed Color() helpers to ValueColor() - dangerously named, rarely used and probably to be made obsolete.
 - 2015/08/29 (1.45) - with the addition of horizontal scrollbar we made various fixes to inconsistencies with dealing with cursor position.
					   GetCursorPos()/SetCursorPos() functions now include the scrolled amount. It shouldn't affect the majority of users, but take note that SetCursorPosX(100.0f) puts you at +100 from the starting x position which may include scrolling, not at +100 from the window left side.
					   GetContentRegionMax()/GetWindowContentRegionMin()/GetWindowContentRegionMax() functions allow include the scrolled amount. Typically those were used in cases where no scrolling would happen so it may not be a problem, but watch out!
 - 2015/08/29 (1.45) - renamed style.ScrollbarWidth to style.ScrollbarSize
 - 2015/08/05 (1.44) - split imgui.cpp into extra files: imgui_demo.cpp imgui_draw.cpp imgui_internal.h that you need to add to your project.
 - 2015/07/18 (1.44) - fixed angles in ImDrawList::PathArcTo(), PathArcToFast() (introduced in 1.43) being off by an extra PI for no justifiable reason
 - 2015/07/14 (1.43) - add new ImFontAtlas::AddFont() API. For the old AddFont***, moved the 'font_no' parameter of ImFontAtlas::AddFont** functions to the ImFontConfig structure.
					   you need to render your textured triangles with bilinear filtering to benefit from sub-pixel positioning of text.
 - 2015/07/08 (1.43) - switched rendering data to use indexed rendering. this is saving a fair amount of CPU/GPU and enables us to get anti-aliasing for a marginal cost.
					   this necessary change will break your rendering function! the fix should be very easy. sorry for that :(
					 - if you are using a vanilla copy of one of the imgui_impl_XXXX.cpp provided in the example, you just need to update your copy and you can ignore the rest.
					 - the signature of the io.RenderDrawListsFn handler has changed!
					   old: ImGui_XXXX_RenderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count)
					   new: ImGui_XXXX_RenderDrawLists(ImDrawData* draw_data).
						 argument:   'cmd_lists' becomes 'draw_data->CmdLists', 'cmd_l`      .<     ä       €P2    ğâ-‡šÑÓ                < 4 9 7 1 d 7 1 0 0 6 9 0 0 3 b b     `      .<     ä      `€P2    ğâ-‡šÑÓ  €             < 4 9 7 1 d 7 1 0 0 6 9 0 0 3 b b     P      /<     .<     À€P2    œ1ÃˆšÑÓ                < S C E N E . i p c h P      /<     .<     P2    œ1ÃˆšÑÓ               < S C E N E . i p c h P      /<     .<     `P2    ×c\‹šÑÓ               < S C E N E . i p c h P      /<     .<     °P2    GÆ^‹šÑÓ €             < S C E N E . i p c h `      Ú      Õ       ‚P2    qr3›ÑÓ €            $ < S o l u t i o n . V C . d b - s h m `      Û      Õ      `‚P2    qr3›ÑÓ €        "     < B r o w s e . V C . o p e n d b     X      ×      Õ      À‚P2    qr3›ÑÓ  €             < B r o w s e . V C . d b           0<     *      ƒP2    âü&ÑÓ           &   N < 4 { 3 8 0 8 8 7 6 b - c 1 7 6 - 4 e 4 8 - b 7 a e - 0 4 0 4 6 e 6 c c 7 5 2 }             0<     *      ¨ƒP2    r‘&ÑÓ          &   N < 4 { 3 8 0 8 8 7 6 b - c 1 7 6 - 4 e 4 8 - b 7 a e - 0 4 0 4 6 e 6 c c 7 5 2 }       `      Ú      Õ      8„P2    
˜æ&ÑÓ               $ < S o l u t i o n . V C . d b - s h m `      Ø      Õ      ˜„P2    „ó&ÑÓ  €            $ < S o l u t i o n . V C . d b - w a l       0<     *      ø„P2    “·ó&ÑÓ €        &   N < 4 { 3 8 0 8 8 7 6 b - c 1 7 6 - 4 e 4 8 - b 7 a e - 0 4 0 4 6 e 6 c c 7 5 2 }       `      Ú      Õ      ˆ…P2    “·ó&ÑÓ  €            $ < S o l u t i o n . V C . d b - s h m       0<     *      è…P2    Ù!;ÑÓ  €        &   N < 4 { 3 8 0 8 8 7 6 b - c 1 7 6 - 4 e 4 8 - b 7 a e - 0 4 0 4 6 e 6 c c 7 5 2 }       X      ×      Õ      x†P2    	 ÑÓ                < B r o w s e . V C . d b     X      ×      Õ      Ğ†P2    Dˆ‡¾£ÑÓ  €             < B r o w s e . V C . d b     X      ×      Õ      (‡P2    Á‘p¨ÑÓ                < B r o w s e . V C . d b     X      ×      Õ      €‡P2    ‹¿M ¬ÑÓ  €             < B r o w s e . V C . d b     H      Ö      Õ      Ø‡P2    x)]­ÑÓ €          "    < . s u o     H      Ö      Õ       ˆP2    x)]­ÑÓ€          "    < . s u o     H      Ö      Õ      hˆP2    O]­ÑÓ€          "    < . s u o     H      Ö      Õ      °ˆP2    O]­ÑÓ€          "    < . s u o     H      Ö      Õ      øˆP2    hßF]­ÑÓ€ €        "    < . s u o     H      Ö      Õ      @‰P2    ebJ]­ÑÓ €          "    < . s u o     H      Ö      Õ      ˆ‰P2    ›ˆJ]­ÑÓ€          "    < . s u o     H      Ö      Õ      Ğ‰P2    šK]­ÑÓ€ €        "    < . s u o     H      Ö      Õ      ŠP2    ¯ó;^­ÑÓ €          "    < . s u o     H      Ö      Õ      `ŠP2    Ñ<^­ÑÓ€          "    < . s u o     H      Ö      Õ      ¨ŠP2    8f<^­ÑÓ€ €        "    < . s u o     H      Ö      Õ      ğŠP2    7N^­ÑÓ €          "    < . s u o     H      Ö      Õ      8‹P2    ¸N^­ÑÓ€          "    < . s u o     H      Ö      Õ      €‹P2    ,O^­ÑÓ€ €        "    < . s u o     `      Ú      Õ      È‹P2    æ¡X^­ÑÓ  €            $ < S o l u t i o n . V C . d b - s h m `      Ø      Õ      (ŒP2    TÈX^­ÑÓ  €            $ < S o l u t i o n . V C . d b - w a l `      Û      Õ      ˆŒP2    FZ^­ÑÓ  €        "     < B r o w s e . V C . o p e n d b     X      ×      Õ      èŒP2    ŸZ^­ÑÓ                < B r o w s e . V C . d b     X      ×      Õ      @P2    /Ÿc^­ÑÓ  €             < B r o w s e . V C . d b     X      Ø      R      ˜P2    PhûXÖÓ               < A u r o r a E n g i n e     X      Ø      R      ğP2    6¯YÖÓ  €            < A u r o r a E n g i n e     X      Ø      R      HP2    ¿*YÖÓ €              < A u r o r a E n g i n e     X      Ø      R       P2    ¿*YÖÓ € €            < A u r o r a E n g i n e     X      Ú      Ø      øP2    &ÏYÖÓ                < . g i t a t t r i b u t e s X      Ú      Ø      PP2    &ÏYÖÓ  €             < . g i t a t t r i b u t e s X      Ú      Ø      ¨P2    u=	YÖÓ                < . g i t a t t r i b u t e s X      Ú      Ø       P2    ®yYÖÓ                < . g i t a t t r i b u t e s X      Ú      Ø      XP2    ®yYÖÓ€               < . g i t a t t r i b u t e s X      Ú      Ø      °P2    $YÖÓ€ €             < . g i t a t t r i b u t e s P      Û      Ø      ‘P2    íÌYÖÓ                < . g i t i g n o r e P      Û      Ø      X‘P2    ÁàYÖÓ               < . g i t i g n o r e P      Û      Ø      ¨‘P2     YÖÓ               < . g i t i g n o r e P      Û      Ø      ø‘P2     YÖÓ               < . g i t i g n o r e P      Û      Ø      H’P2     YÖÓ €             < . g i t i g n o r e `      0<     Ø      ˜’P2    À«YÖÓ                 < A s s e t M a n a g e r . c p p     `      0<     Ø      ø’P2    À«YÖÓ                < A s s e t M a n a g e r . c p p     `      0<     Ø      X“P2    Ä(YÖÓ                < A s s e t M a n a g e r . c p p     `      0<     Ø      ¸“P2    Ä(YÖÓ                < A s s e t M a n a g e r . c p p     `      0<     Ø      ”P2    Ä(YÖÓ €              < A s s e t M a n a g e r . c p p     X      1<     Ø      x”P2    TYÖÓ                < A s s e t M a n a g e r . h X      1<     Ø      Ğ”P2    TYÖÓ               < A s s e t M a n a g e r . h X      1<     Ø      (•P2    ˆüYÖÓ               < A s s e t M a n a g e r . h X      1<     Ø      €•P2    ˆüYÖÓ               < A s s e t M a n a g e r . h X      1<     Ø      Ø•P2    YÖÓ €             < A s s e t M a n a g e r . h P      2<     Ø      0–P2    À
YÖÓ                < C a m e r a . c p p P      2<     Ø      €–P2    À
YÖÓ               < C a m e r a . c p p P      2<     Ø      Ğ–P2     YÖÓ               < C a m e r a . c p p P      2<     Ø       —P2     YÖÓ               < C a m e r a . c p p P      2<     Ø      p—P2     YÖÓ €             < C a m e r a . c p p P      3<     Ø      À—P2    aåYÖÓ                < C a m e r a . h     P      3<     Ø      ˜P2    aåYÖÓ               < C a m e r a . h     P      3<     Ø      `˜P2    aåYÖÓ               < C a m e r a . h     P      3<     Ø      °˜P2    aåYÖÓ               < C a m e r a . h     P      3<     Ø       ™P2    *0YÖÓ €             < C a m e r a . h     X      4<     Ø      P™P2    
jYÖÓ                < C o m p o n e n t . c p p   X      4<     Ø      ¨™P2    
jYÖÓ               < C o m p o n e n t . c p p   X      4<     Ø       šP2    
jYÖÓ               < C o m p o n e n t . c p p   X      4<     Ø      XšP2    
jYÖÓ               < C o m p o n e n t . c p p   X      4<     Ø      °šP2    §›YÖÓ €             < C o m p o n e n t . c p p   X      5<     Ø      ›P2    ¼ YÖÓ                < C o m p o n e n t . h     g X      5<     Ø      `›P2    ¼ YÖÓ               < C o m p o n e n t . h     g X      5<     Ø      ¸›P2    ¼ YÖÓ               < C o m p o n e n t . h     g X      5<     Ø      œP2    ¼ YÖÓ               < C o m p o n e n t . h     g X      5<     Ø      hœP2    ¼ YÖÓ €             < C o m p o n e n t . h     g P      6<     Ø      ÀœP2    «{!YÖÓ                < C o n s o l e . h   P      6<     Ø      P2    «{!YÖÓ               < C o n s o l e . h   P      6<     Ø      `P2    ¯Ê"YÖÓ               < C o n s o l e . h   P      6<     Ø      °P2    ¯Ê"YÖÓ               < C o n s o l e . h   P      6<     Ø       P2    î#YÖÓ €             < C o n s o l e . h   X      7<     Ø      PP2    J”#YÖÓ                < C o r e E n g i n e . c p p X      7<     Ø      ¨P2    J”#YÖÓ               < C o r e E n g i n e . c p p X      7<     Ø       ŸP2    ˆ&YÖÓ               < C o r e E n g i n e . c p p X      7<     Ø      XŸP2    ˆ&YÖÓ               < C o r e E n g i n e . c p p                                                                                 X      7<     Ø        P2    ˆ&YÖÓ €             < C o r e E n g i n e . c p p X      8<     Ø      X P2    Ğ(YÖÓ                < C o r e E n g i n e . h     X      8<     Ø      ° P2    Ğ(YÖÓ               < C o r e E n g i n e . h     X      8<     Ø      ¡P2    O>*YÖÓ               < C o r e E n g i n e . h     X      8<     Ø      `¡P2    O>*YÖÓ               < C o r e E n g i n e . h     X      8<     Ø      ¸¡P2    ÃS*YÖÓ €             < C o r e E n g i n e . h     X      9<     Ø      ¢P2    Nğ*YÖÓ                < D 3 D D e m o . s l n     g X      9<     Ø      h¢P2    ×+YÖÓ               < D 3 D D e m o . s l n     g X      9<     Ø      À¢P2    %¶+YÖÓ               < D 3 D D e m o . s l n     g X      9<     Ø      £P2    %¶+YÖÓ               < D 3 D D e m o . s l n     g X      9<     Ø      p£P2    %¶+YÖÓ €             < D 3 D D e m o . s l n     g P      :<     Ø      È£P2    “Ø,YÖÓ                < D e b u g . c p p   P      :<     Ø      ¤P2    “Ø,YÖÓ               < D e b u g . c p p   P      :<     Ø      h¤P2    “Ø,YÖÓ               < D e b u g . c p p   P      :<     Ø      ¸¤P2    “Ø,YÖÓ               < D e b u g . c p p   P      :<     Ø      ¥P2    “Ø,YÖÓ €             < D e b u g . c p p   P      ;<     Ø      X¥P2    ²-YÖÓ                < D e b u g . h       P      ;<     Ø      ¨¥P2    ²-YÖÓ               < D e b u g . h       P      ;<     Ø      ø¥P2    c.YÖÓ               < D e b u g . h       P      ;<     Ø      H¦P2    c.YÖÓ               < D e b u g . h       P      ;<     Ø      ˜¦P2    c.YÖÓ €             < D e b u g . h       X      <<     Ø      è¦P2    L0YÖÓ                < D e m o . v c x p r o j     X      <<     Ø      @§P2    L0YÖÓ               < D e m o . v c x p r o j     X      <<     Ø      ˜§P2    `3YÖÓ               < D e m o . v c x p r o j     X      <<     Ø      ğ§P2    `3YÖÓ               < D e m o . v c x p r o j     X      <<     Ø      H¨P2    ğƒ3YÖÓ €             < D e m o . v c x p r o j     h      =<     Ø       ¨P2    ’Q4YÖÓ               ( < D e m o . v c x p r o j . f i l t e r s     h      =<     Ø      ©P2    ’Q4YÖÓ              ( < D e m o . v c x p r o j . f i l t e r s     h      =<     Ø      p©P2    ğã6YÖÓ              ( < D e m o . v c x p r o j . f i l t e r s     h      =<     Ø      Ø©P2    ğã6YÖÓ              ( < D e m o . v c x p r o j . f i l t e r s     h      =<     Ø      @ªP2    ğã6YÖÓ €            ( < D e m o . v c x p r o j . f i l t e r s     `      ><     Ø      ¨ªP2    77YÖÓ               " < D e m o . v c x p r o j . u s e r   `      ><     Ø      «P2    77YÖÓ              " < D e m o . v c x p r o j . u s e r   `      ><     Ø      h«P2    77YÖÓ              " < D e m o . v c x p r o j . u s e r   `      ><     Ø      È«P2    77YÖÓ              " < D e m o . v c x p r o j . u s e r   `      ><     Ø      (¬P2    77YÖÓ €            " < D e m o . v c x p r o j . u s e r   P      ?<     Ø      ˆ¬P2    yI9YÖÓ                < E d i t o r . c p p P      ?<     Ø      Ø¬P2    
]9YÖÓ               < E d i t o r . c p p P      ?<     Ø      (­P2    -Ó:YÖÓ               < E d i t o r . c p p P      ?<     Ø      x­P2    †å:YÖÓ               < E d i t o r . c p p P      ?<     Ø      È­P2    †å:YÖÓ €             < E d i t o r . c p p P      @<     Ø      ®P2    Mª;YÖÓ                < E d i t o r . h     P      @<     Ø      h®P2    Mª;YÖÓ               < E d i t o r . h     P      @<     Ø      ¸®P2    Mª;YÖÓ               < E d i t o r . h     P      @<     Ø      ¯P2    Mª;YÖÓ               < E d i t o r . h     P      @<     Ø      X¯P2    Mª;YÖÓ €             < E d i t o r . h                                                                                             h      A<     Ø       °P2    h¸<YÖÓ               ( < E d i t o r M o v e S c r i p t . c p p     h      A<     Ø      h°P2    h¸<YÖÓ              ( < E d i t o r M o v e S c r i p t . c p p     h      A<     Ø      Ğ°P2    h¸<YÖÓ              ( < E d i t o r M o v e S c r i p t . c p p     h      A<     Ø      8±P2    t=YÖÓ              ( < E d i t o r M o v e S c r i p t . c p p     h      A<     Ø       ±P2    t=YÖÓ €            ( < E d i t o r M o v e S c r i p t . c p p     `      B<     Ø      ²P2    ½ª=YÖÓ               $ < E d i t o r M o v e S c r i p t . h `      B<     Ø      h²P2    ½ª=YÖÓ              $ < E d i t o r M o v e S c r i p t . h `      B<     Ø      È²P2    ½ª=YÖÓ              $ < E d i t o r M o v e S c r i p t . h `      B<     Ø      (³P2    ´ô=YÖÓ              $ < E d i t o r M o v e S c r i p t . h `      B<     Ø      ˆ³P2    ´ô=YÖÓ €            $ < E d i t o r M o v e S c r i p t . h x      C<     Ø      è³P2    ^|>YÖÓ               < < E d i t o r S c e n e S e l e c t i o n S c r i p t . c p p x      C<     Ø      `´P2    ^|>YÖÓ              < < E d i t o r S c e n e S e l e c t i o n S c r i p t . c p p x      C<     Ø      Ø´P2    ^|>YÖÓ              < < E d i t o r S c e n e S e l e c t i o n S c r i p t . c p p x      C<     Ø      PµP2    Ø­>YÖÓ              < < E d i t o r S c e n e S e l e c t i o n S c r i p t . c p p x      C<     Ø      ÈµP2    Ø­>YÖÓ €            < < E d i t o r S c e n e S e l e c t i o n S c r i p t . c p p x      D<     Ø      @¶P2    d‚?YÖÓ               8 < E d i t o r S c e n e S e l e c t i o n S c r i p t . h     x      D<     Ø      ¸¶P2    d‚?YÖÓ              8 < E d i t o r S c e n e S e l e c t i o n S c r i p t . h     x      D<     Ø      0·P2    ·ª?YÖÓ              8 < E d i t o r S c e n e S e l e c t i o n S c r i p t . h     x      D<     Ø      ¨·P2    ·ª?YÖÓ              8 < E d i t o r S c e n e S e l e c t i o n S c r i p t . h     x      D<     Ø       ¸P2    ·ª?YÖÓ €            8 < E d i t o r S c e n e S e l e c t i o n S c r i p t . h     X      E<     Ø      ˜¸P2    ˜ë@YÖÓ                < F r a g m e n t . h l s l   X      E<     Ø      ğ¸P2    ˜ë@YÖÓ               < F r a g m e n t . h l s l   X      E<     Ø      H¹P2    BAYÖÓ               < F r a g m e n t . h l s l   X      E<     Ø       ¹P2    BAYÖÓ               < F r a g m e n t . h l s l   X      E<     Ø      ø¹P2    BAYÖÓ €             < F r a g m e n t . h l s l   X      F<     Ø      PºP2    ê„BYÖÓ                < G a m e O b j e c t . c p p X      F<     Ø      ¨ºP2    ê„BYÖÓ               < G a m e O b j e c t . c p p X      F<     Ø       »P2    ¦CYÖÓ               < G a m e O b j e c t . c p p X      F<     Ø      X»P2    ¦CYÖÓ               < G a m e O b j e c t . c p p X      F<     Ø      °»P2    ¦CYÖÓ €             < G a m e O b j e c t . c p p X      G<     Ø      ¼P2    ÖwDYÖÓ                < G a m e O b j e c t . h     X      G<     Ø      `¼P2    ÖwDYÖÓ               < G a m e O b j e c t . h     X      G<     Ø      ¸¼P2    ÖwDYÖÓ               < G a m e O b j e c t . h     X      G<     Ø      ½P2    ÖwDYÖÓ               < G a m e O b j e c t . h     X      G<     Ø      h½P2    ÖwDYÖÓ €             < G a m e O b j e c t . h     P      H<     Ø      À½P2    ™€GYÖÓ                < G U I . c p p       P      H<     Ø      ¾P2    ™€GYÖÓ               < G U I . c p p       P      H<     Ø      `¾P2    ™€GYÖÓ               < G U I . c p p       P      H<     Ø      °¾P2    ™€GYÖÓ               < G U I . c p p       P      H<     Ø       ¿P2    öáHYÖÓ €             < G U I . c p p       H      I<     Ø      P¿P2    vkIYÖÓ               
 < G U I . h   H      I<     Ø      ˜¿P2    IYÖÓ              
 < G U I . h                                   H      I<     Ø       ÀP2    I–IYÖÓ              
 < G U I . h   H      I<     Ø      HÀP2    ™IYÖÓ              
 < G U I . h   H      I<     Ø      ÀP2    ¥IYÖÓ €            
 < G U I . h   `      J<     Ø      ØÀP2    ‹.KYÖÓ                < G U I _ C o n s o l e . c p p       `      J<     Ø      8ÁP2    ‹.KYÖÓ               < G U I _ C o n s o l e . c p p       `      J<     Ø      ˜ÁP2    ‹.KYÖÓ               < G U I _ C o n s o l e . c p p       `      J<     Ø      øÁP2    avKYÖÓ               < G U I _ C o n s o l e . c p p       `      J<     Ø      XÂP2    avKYÖÓ €             < G U I _ C o n s o l e . c p p       X      K<     Ø      ¸ÂP2    öKYÖÓ                < G U I _ C o n s o l e . h   X      K<     Ø      ÃP2    öKYÖÓ               < G U I _ C o n s o l e . h   X      K<     Ø      hÃP2    öKYÖÓ               < G U I _ C o n s o l e . h   X      K<     Ø      ÀÃP2    %LYÖÓ               < G U I _ C o n s o l e . h   X      K<     Ø      ÄP2    %LYÖÓ €             < G U I _ C o n s o l e . h   `      L<     Ø      pÄP2    ”×MYÖÓ               " < G U I _ H i e r a r c h y . c p p   `      L<     Ø      ĞÄP2    ”×MYÖÓ              " < G U I _ H i e r a r c h y . c p p   `      L<     Ø      0ÅP2    ŠOYÖÓ              " < G U I _ H i e r a r c h y . c p p   `      L<     Ø      ÅP2    ŠOYÖÓ              " < G U I _ H i e r a r c h y . c p p   `      L<     Ø      ğÅP2    ŠOYÖÓ €            " < G U I _ H i e r a r c h y . c p p   `      M<     Ø      PÆP2    PYÖÓ                < G U I _ H i e r a r c h y . h       `      M<     Ø      °ÆP2    PYÖÓ               < G U I _ H i e r a r c h y . h       `      M<     Ø      ÇP2    PYÖÓ               < G U I _ H i e r a r c h y . h       `      M<     Ø      pÇP2    PYÖÓ               < G U I _ H i e r a r c h y . h       `      M<     Ø      ĞÇP2    PYÖÓ €             < G U I _ H i e r a r c h y . h       `      N<     Ø      0ÈP2    ±‚SYÖÓ               " < G U I _ I n s p e c t o r . c p p   `      N<     Ø      ÈP2    &ªSYÖÓ              " < G U I _ I n s p e c t o r . c p p   `      N<     Ø      ğÈP2    £eUYÖÓ              " < G U I _ I n s p e c t o r . c p p   `      N<     Ø      PÉP2    £eUYÖÓ              " < G U I _ I n s p e c t o r . c p p   `      N<     Ø      °ÉP2    £eUYÖÓ €            " < G U I _ I n s p e c t o r . c p p   `      O<     Ø      ÊP2    )VYÖÓ                < G U I _ I n s p e c t o r . h       `      O<     Ø      pÊP2    )VYÖÓ               < G U I _ I n s p e c t o r . h       `      O<     Ø      ĞÊP2    )VYÖÓ               < G U I _ I n s p e c t o r . h       `      O<     Ø      0ËP2    «PVYÖÓ               < G U I _ I n s p e c t o r . h       `      O<     Ø      ËP2    «PVYÖÓ €             < G U I _ I n s p e c t o r . h       `      P<     Ø      ğËP2    ÿpZYÖÓ                < G U I _ M e n u B a r . c p p     l `      P<     Ø      PÌP2    ÿpZYÖÓ               < G U I _ M e n u B a r . c p p     l `      P<     Ø      °ÌP2    &˜ZYÖÓ               < G U I _ M e n u B a r . c p p     l `      P<     Ø      ÍP2    &˜ZYÖÓ               < G U I _ M e n u B a r . c p p     l `      P<     Ø      pÍP2    &˜ZYÖÓ €             < G U I _ M e n u B a r . c p p     l X      Q<     Ø      ĞÍP2    {æZYÖÓ                < G U I _ M e n u B a r . h   X      Q<     Ø      (ÎP2    {æZYÖÓ               < G U I _ M e n u B a r . h   X      Q<     Ø      €ÎP2    {æZYÖÓ               < G U I _ M e n u B a r . h   X      Q<     Ø      ØÎP2    {æZYÖÓ               < G U I _ M e n u B a r . h   X      Q<     Ø      0ÏP2    {æZYÖÓ €             < G U I _ M e n u B a r . h   `      R<     Ø      ˆÏP2    ä[[YÖÓ                 < G U I _ V i e w p o r t . c p p                             `      R<     Ø       ĞP2    ä[[YÖÓ                < G U I _ V i e w p o r t . c p p     `      R<     Ø      `ĞP2    "D^YÖÓ                < G U I _ V i e w p o r t . c p p     `      R<     Ø      ÀĞP2    "D^YÖÓ                < G U I _ V i e w p o r t . c p p     `      R<     Ø       ÑP2    =k^YÖÓ €              < G U I _ V i e w p o r t . c p p     X      S<     Ø      €ÑP2    =V_YÖÓ                < G U I _ V i e w p o r t . h X      S<     Ø      ØÑP2    l}_YÖÓ               < G U I _ V i e w p o r t . h X      S<     Ø      0ÒP2    l}_YÖÓ               < G U I _ V i e w p o r t . h X      S<     Ø      ˆÒP2    l}_YÖÓ               < G U I _ V i e w p o r t . h X      S<     Ø      àÒP2    l}_YÖÓ €             < G U I _ V i e w p o r t . h P      T<     Ø      8ÓP2    ÛbYÖÓ                < I G i z m o . h     P      T<     Ø      ˆÓP2    2cYÖÓ               < I G i z m o . h     P      T<     Ø      ØÓP2    H cYÖÓ               < I G i z m o . h     P      T<     Ø      (ÔP2    H cYÖÓ               < I G i z m o . h     P      T<     Ø      xÔP2    H cYÖÓ#               < I G i z m o . h     P      T<     Ø      ÈÔP2    H cYÖÓ3               < I G i z m o . h     P      T<     Ø      ÕP2    H cYÖÓ3               < I G i z m o . h     P      T<     Ø      hÕP2    :ícYÖÓ3 €             < I G i z m o . h     P      U<     Ø      ¸ÕP2    ddYÖÓ                < I L o g g e r . h   P      U<     Ø      ÖP2    ddYÖÓ               < I L o g g e r . h   P      U<     Ø      XÖP2    ddYÖÓ               < I L o g g e r . h   P      U<     Ø      ¨ÖP2    ddYÖÓ               < I L o g g e r . h   P      U<     Ø      øÖP2    —;dYÖÓ €             < I L o g g e r . h   P      V<     Ø      H×P2    ºbdYÖÓ                < i m c o n f i g . h P      V<     Ø      ˜×P2    ºbdYÖÓ               < i m c o n f i g . h P      V<     Ø      è×P2    ºMeYÖÓ               < i m c o n f i g . h P      V<     Ø      8ØP2    äteYÖÓ               < i m c o n f i g . h P      V<     Ø      ˆØP2    äteYÖÓ €             < i m c o n f i g . h P      W<     Ø      ØØP2    UÃeYÖÓ                < i m g u i . c p p   P      W<     Ø      (ÙP2    UÃeYÖÓ               < i m g u i . c p p   P      W<     Ø      xÙP2    ¤êeYÖÓ               < i m g u i . c p p   P      W<     Ø      ÈÙP2    7‡fYÖÓ               < i m g u i . c p p   P      W<     Ø      ÚP2    7‡fYÖÓ €             < i m g u i . c p p   P      X<     Ø      hÚP2    Ï½iYÖÓ                < i m g u i . h     C}P      X<     Ø      ¸ÚP2    Ï½iYÖÓ               < i m g u i . h     C}P      X<     Ø      ÛP2    Ï½iYÖÓ               < i m g u i . h     C}P      X<     Ø      XÛP2    (jYÖÓ               < i m g u i . h     C}P      X<     Ø      ¨ÛP2    (jYÖÓ €             < i m g u i . h     C}P      Y<     Ø      øÛP2    İ3jYÖÓ                < i m g u i . i n i   P      Y<     Ø      HÜP2    }ZjYÖÓ               < i m g u i . i n i   P      Y<     Ø      ˜ÜP2    Ñ-kYÖÓ               < i m g u i . i n i   P      Y<     Ø      èÜP2    Ñ-kYÖÓ               < i m g u i . i n i   P      Y<     Ø      8İP2    ¬SkYÖÓ €             < i m g u i . i n i   X      Z<     Ø      ˆİP2    Ø£kYÖÓ                < I m G u i z m o . c p p     X      Z<     Ø      àİP2    Ø£kYÖÓ               < I m G u i z m o . c p p     X      Z<     Ø      8ŞP2    ?;pYÖÓ               < I m G u i z m o . c p p     X      Z<     Ø      ŞP2    ƒëqYÖÓ               < I m G u i z m o . c p p     X      Z<     Ø      èŞP2    ƒëqYÖÓ#               < I m G u i z m o . c p p     X      Z<     Ø      @ßP2    ƒëqYÖÓ3               < I m G u i z m o . c p p     X      Z<     Ø      ˜ßP2    ƒëqYÖÓ3               < I m G u i z m o . c p p                     X      Z<     Ø       àP2    çsYÖÓ3 €             < I m G u i z m o . c p p     P      [<     Ø      XàP2    JtYÖÓ                < I m G u i z m o . h P      [<     Ø      ¨àP2    JtYÖÓ               < I m G u i z m o . h P      [<     Ø      øàP2    puYÖÓ               < I m G u i z m o . h P      [<     Ø      HáP2    puYÖÓ               < I m G u i z m o . h P      [<     Ø      ˜áP2    puYÖÓ#               < I m G u i z m o . h P      [<     Ø      èáP2    puYÖÓ3               < I m G u i z m o . h P      [<     Ø      8âP2    puYÖÓ3               < I m G u i z m o . h P      [<     Ø      ˆâP2    ×yYÖÓ3 €             < I m G u i z m o . h X      \<     Ø      ØâP2    ,•zYÖÓ                < i m g u i _ d e m o . c p p X      \<     Ø      0ãP2    •»zYÖÓ               < i m g u i _ d e m o . c p p X      \<     Ø      ˆãP2    8¾zYÖÓ               < i m g u i _ d e m o . c p p X      \<     Ø      àãP2    –{YÖÓ               < i m g u i _ d e m o . c p p X      \<     Ø      8äP2    –{YÖÓ €             < i m g u i _ d e m o . c p p X      ]<     Ø      äP2    ûZ{YÖÓ                < i m g u i _ d o c k . c p p X      ]<     Ø      èäP2    ûZ{YÖÓ               < i m g u i _ d o c k . c p p X      ]<     Ø      @åP2    ûZ{YÖÓ               < i m g u i _ d o c k . c p p X      ]<     Ø      ˜åP2    ûZ{YÖÓ               < i m g u i _ d o c k . c p p X      ]<     Ø      ğåP2    ‚{YÖÓ €             < i m g u i _ d o c k . c p p X      ^<     Ø      HæP2    <©{YÖÓ                < i m g u i _ d o c k . h     X      ^<     Ø       æP2    <©{YÖÓ               < i m g u i _ d o c k . h     X      ^<     Ø      øæP2    ÏÑ{YÖÓ               < i m g u i _ d o c k . h     X      ^<     Ø      PçP2    ÏÑ{YÖÓ               < i m g u i _ d o c k . h     X      ^<     Ø      ¨çP2    ÏÑ{YÖÓ €             < i m g u i _ d o c k . h     X      _<     Ø       èP2    lù{YÖÓ                < i m g u i _ d r a w . c p p X      _<     Ø      XèP2    lù{YÖÓ               < i m g u i _ d r a w . c p p X      _<     Ø      °èP2    0 |YÖÓ               < i m g u i _ d r a w . c p p X      _<     Ø      éP2    nG|YÖÓ               < i m g u i _ d r a w . c p p X      _<     Ø      `éP2    am|YÖÓ €             < i m g u i _ d r a w . c p p h      `<     Ø      ¸éP2    –|YÖÓ               & < i m g u i _ i m p l _ d x 1 1 . c p p       h      `<     Ø       êP2    –|YÖÓ              & < i m g u i _ i m p l _ d x 1 1 . c p p       h      `<     Ø      ˆêP2    5Á|YÖÓ              & < i m g u i _ i m p l _ d x 1 1 . c p p       h      `<     Ø      ğêP2    5Á|YÖÓ              & < i m g u i _ i m p l _ d x 1 1 . c p p       h      `<     Ø      XëP2    5Á|YÖÓ €            & < i m g u i _ i m p l _ d x 1 1 . c p p       `      a<     Ø      ÀëP2    ƒ}YÖÓ               " < i m g u i _ i m p l _ d x 1 1 . h   `      a<     Ø       ìP2    ƒ}YÖÓ              " < i m g u i _ i m p l _ d x 1 1 . h   `      a<     Ø      €ìP2    ƒ}YÖÓ              " < i m g u i _ i m p l _ d x 1 1 . h   `      a<     Ø      àìP2    ƒ}YÖÓ              " < i m g u i _ i m p l _ d x 1 1 . h   `      a<     Ø      @íP2    ƒ}YÖÓ €            " < i m g u i _ i m p l _ d x 1 1 . h   `      b<     Ø       íP2    ÈY}YÖÓ                 < i m g u i _ i n t e r n a l . h     `      b<     Ø       îP2    ÈY}YÖÓ                < i m g u i _ i n t e r n a l . h     `      b<     Ø      `îP2    ÈY}YÖÓ                < i m g u i _ i n t e r n a l . h     `      b<     Ø      ÀîP2    i¨}YÖÓ                < i m g u i _ i n t e r n a l . h     `      b<     Ø       ïP2    i¨}YÖÓ €              < i m g u i _ i n t e r n a l . h     `      c<     Ø      €ïP2    éÍ}YÖÓ                < I m S e q u e n c e r . c p p                                       `      c<     Ø       ğP2    éÍ}YÖÓ               < I m S e q u e n c e r . c p p       `      c<     Ø      `ğP2    5_YÖÓ               < I m S e q u e n c e r . c p p       `      c<     Ø      ÀğP2    9~YÖÓ               < I m S e q u e n c e r . c p p       `      c<     Ø       ñP2    9~YÖÓ#               < I m S e q u e n c e r . c p p       `      c<     Ø      €ñP2    9~YÖÓ3               < I m S e q u e n c e r . c p p       `      c<     Ø      àñP2    9~YÖÓ3               < I m S e q u e n c e r . c p p       `      c<     Ø      @òP2    _ÌYÖÓ3 €             < I m S e q u e n c e r . c p p       X      d<     Ø       òP2    ÆƒYÖÓ                < I m S e q u e n c e r . h   X      d<     Ø      øòP2    ÆƒYÖÓ               < I m S e q u e n c e r . h   X      d<     Ø      PóP2    ÆƒYÖÓ               < I m S e q u e n c e r . h   X      d<     Ø      ¨óP2    _6ƒYÖÓ               < I m S e q u e n c e r . h   X      d<     Ø       ôP2    _6ƒYÖÓ#               < I m S e q u e n c e r . h   X      d<     Ø      XôP2    _6ƒYÖÓ3               < I m S e q u e n c e r . h   X      d<     Ø      °ôP2    _6ƒYÖÓ3               < I m S e q u e n c e r . h   X      d<     Ø      õP2    ‘]ƒYÖÓ3 €             < I m S e q u e n c e r . h   `      e<     Ø      `õP2    èÀƒYÖÓ                 < I n p u t H a n d l e r . c p p     `      e<     Ø      ÀõP2    èÀƒYÖÓ                < I n p u t H a n d l e r . c p p     `      e<     Ø       öP2    èÀƒYÖÓ                < I n p u t H a n d l e r . c p p     `      e<     Ø      €öP2    èÀƒYÖÓ                < I n p u t H a n d l e r . c p p     `      e<     Ø      àöP2    èÀƒYÖÓ €              < I n p u t H a n d l e r . c p p     X      f<     Ø      @÷P2    I„YÖÓ                < I n p u t H a n d l e r . h X      f<     Ø      ˜÷P2    I„YÖÓ               < I n p u t H a n d l e r . h X      f<     Ø      ğ÷P2    ã7„YÖÓ               < I n p u t H a n d l e r . h X      f<     Ø      HøP2    ã7„YÖÓ               < I n p u t H a n d l e r . h X      f<     Ø       øP2    ã7„YÖÓ €             < I n p u t H a n d l e r . h x      g<     Ø      øøP2    ò^„YÖÓ               6 < I N P U T _ E L E M E N T _ D E S C R I P T I O N . h     \ x      g<     Ø      pùP2    †„YÖÓ              6 < I N P U T _ E L E M E N T _ D E S C R I P T I O N . h     \ x      g<     Ø      èùP2    †„YÖÓ              6 < I N P U T _ E L E M E N T _ D E S C R I P T I O N . h     \ x      g<     Ø      `úP2    †„YÖÓ              6 < I N P U T _ E L E M E N T _ D E S C R I P T I O N . h     \ x      g<     Ø      ØúP2    †„YÖÓ €            6 < I N P U T _ E L E M E N T _ D E S C R I P T I O N . h     \ P      h<     Ø      PûP2    ƒó„YÖÓ                < j s o n . h p p     P      h<     Ø       ûP2    ƒó„YÖÓ               < j s o n . h p p     P      h<     Ø      ğûP2    ‡…YÖÓ               < j s o n . h p p     P      h<     Ø      @üP2    ºo†YÖÓ               < j s o n . h p p     P      h<     Ø      üP2    À|†YÖÓ €             < j s o n . h p p     P      i<     Ø      àüP2    1Ê‡YÖÓ                < K e y C o d e . h   P      i<     Ø      0ıP2    1Ê‡YÖÓ               < K e y C o d e . h   P      i<     Ø      €ıP2    1Ê‡YÖÓ               < K e y C o d e . h   P      i<     Ø      ĞıP2    1Ê‡YÖÓ               < K e y C o d e . h   P      i<     Ø       şP2    1Ê‡YÖÓ €             < K e y C o d e . h   X      j<     Ø      pşP2    Ş,ˆYÖÓ                < L I G H T _ T Y P E S . h   X      j<     Ø      ÈşP2    Ş,ˆYÖÓ               < L I G H T _ T Y P E S . h   X      j<     Ø       ÿP2    Ş,ˆYÖÓ               < L I G H T _ T Y P E S . h   X      j<     Ø      xÿP2    Ş,ˆYÖÓ               < L I G H T _ T Y P E S . h                                                   X      j<     Ø        Q2    Ş,ˆYÖÓ €             < L I G H T _ T Y P E S . h   P      k<     Ø      X Q2    “|ˆYÖÓ                < m a i n . c p p     P      k<     Ø      ¨ Q2    “|ˆYÖÓ               < m a i n . c p p     P      k<     Ø      ø Q2    “|ˆYÖÓ               < m a i n . c p p     P      k<     Ø      HQ2    “|ˆYÖÓ               < m a i n . c p p     P      k<     Ø      ˜Q2    “|ˆYÖÓ €             < m a i n . c p p     X      l<     Ø      èQ2    ,ä‰YÖÓ                < M a t e r i a l . c p p     X      l<     Ø      @Q2    ,ä‰YÖÓ               < M a t e r i a l . c p p     X      l<     Ø      ˜Q2    vöŠYÖÓ               < M a t e r i a l . c p p     X      l<     Ø      ğQ2    (‹YÖÓ               < M a t e r i a l . c p p     X      l<     Ø      HQ2    (‹YÖÓ €             < M a t e r i a l . c p p     P      m<     Ø       Q2    KK‹YÖÓ                < M a t e r i a l . h P      m<     Ø      ğQ2    Ås‹YÖÓ               < M a t e r i a l . h P      m<     Ø      @Q2    Ås‹YÖÓ               < M a t e r i a l . h P      m<     Ø      Q2    Ås‹YÖÓ               < M a t e r i a l . h P      m<     Ø      àQ2    Ås‹YÖÓ €             < M a t e r i a l . h P      n<     Ø      0Q2    ÎŒYÖÓ                < M e s h . c p p     P      n<     Ø      €Q2    ÎŒYÖÓ               < M e s h . c p p     P      n<     Ø      ĞQ2    é¥YÖÓ               < M e s h . c p p     P      n<     Ø       Q2    é¥YÖÓ               < M e s h . c p p     P      n<     Ø      pQ2    é¥YÖÓ €             < M e s h . c p p     H      o<     Ø      ÀQ2    3D‘YÖÓ                < M e s h . h H      o<     Ø      Q2    ëj‘YÖÓ               < M e s h . h H      o<     Ø      PQ2    “R“YÖÓ               < M e s h . h H      o<     Ø      ˜Q2    “R“YÖÓ               < M e s h . h H      o<     Ø      àQ2    “R“YÖÓ €             < M e s h . h X      p<     Ø      (Q2    Ûó“YÖÓ                < M e s h F i l t e r . c p p X      p<     Ø      €Q2    ”YÖÓ               < M e s h F i l t e r . c p p X      p<     Ø      ØQ2    2/•YÖÓ               < M e s h F i l t e r . c p p X      p<     Ø      0	Q2    2/•YÖÓ               < M e s h F i l t e r . c p p X      p<     Ø      ˆ	Q2    TA•YÖÓ €             < M e s h F i l t e r . c p p X      q<     Ø      à	Q2    @ğ•YÖÓ                < M e s h F i l t e r . h     X      q<     Ø      8
Q2    á–YÖÓ               < M e s h F i l t e r . h     X      q<     Ø      
Q2    !–YÖÓ               < M e s h F i l t e r . h     X      q<     Ø      è
Q2    !–YÖÓ               < M e s h F i l t e r . h     X      q<     Ø      @Q2    c–YÖÓ €             < M e s h F i l t e r . h     `      r<     Ø      ˜Q2    Ô–YÖÓ                < p a c k a g e s . c o n f i g       `      r<     Ø      øQ2    Ô–YÖÓ               < p a c k a g e s . c o n f i g       `      r<     Ø      XQ2    Ô–YÖÓ               < p a c k a g e s . c o n f i g       `      r<     Ø      ¸Q2    Ô–YÖÓ               < p a c k a g e s . c o n f i g       `      r<     Ø      Q2    ìü–YÖÓ €             < p a c k a g e s . c o n f i g       X      s<     Ø      xQ2    âX—YÖÓ                < P h y s i c s . c p p     g X      s<     Ø      ĞQ2    âX—YÖÓ               < P h y s i c s . c p p     g X      s<     Ø      (Q2    pş—YÖÓ               < P h y s i c s . c p p     g X      s<     Ø      €Q2    pş—YÖÓ               < P h y s i c s . c p p     g X      s<     Ø      ØQ2    á#˜YÖÓ €             < P h y s i c s . c p p     g P      t<     Ø      0Q2    ³À˜YÖÓ                < P h y s i c s . h   P      t<     Ø      €Q2    ³À˜YÖÓ               < P h y s i c s . h                                                   P      t<     Ø       Q2    ³À˜YÖÓ               < P h y s i c s . h   P      t<     Ø      PQ2    ç˜YÖÓ               < P h y s i c s . h   P      t<     Ø       Q2    ç˜YÖÓ €             < P h y s i c s . h   h      u<     Ø      ğQ2    ‹–šYÖÓ               ( < P R I M I T I V E _ G E O M E T R Y . h     h      u<     Ø      XQ2    ‹–šYÖÓ              ( < P R I M I T I V E _ G E O M E T R Y . h     h      u<     Ø      ÀQ2    ÖšYÖÓ              ( < P R I M I T I V E _ G E O M E T R Y . h     h      u<     Ø      (Q2    ÖšYÖÓ              ( < P R I M I T I V E _ G E O M E T R Y . h     h      u<     Ø      Q2    ÖšYÖÓ €            ( < P R I M I T I V E _ G E O M E T R Y . h     `      v<     Ø      øQ2    »IœYÖÓ                 < Q u a d T r e e N o d e . c p p     `      v<     Ø      XQ2    »IœYÖÓ                < Q u a d T r e e N o d e . c p p     `      v<     Ø      ¸Q2    »IœYÖÓ                < Q u a d T r e e N o d e . c p p     `      v<     Ø      Q2    »IœYÖÓ                < Q u a d T r e e N o d e . c p p     `      v<     Ø      xQ2    æpœYÖÓ €              < Q u a d T r e e N o d e . c p p     X      w<     Ø      ØQ2    „YÖÓ                < Q u a d T r e e N o d e . h X      w<     Ø      0Q2    „YÖÓ               < Q u a d T r e e N o d e . h X      w<     Ø      ˆQ2    o3YÖÓ               < Q u a d T r e e N o d e . h X      w<     Ø      àQ2    o3YÖÓ               < Q u a d T r e e N o d e . h X      w<     Ø      8Q2    o3YÖÓ €             < Q u a d T r e e N o d e . h `      x<     Ø      Q2    —YÖÓ               " < R e n d e r M a n a g e r . c p p   `      x<     Ø      ğQ2    —YÖÓ              " < R e n d e r M a n a g e r . c p p   `      x<     Ø      PQ2    ¶àŸYÖÓ              " < R e n d e r M a n a g e r . c p p   `      x<     Ø      °Q2    ¶àŸYÖÓ              " < R e n d e r M a n a g e r . c p p   `      x<     Ø      Q2    ¶àŸYÖÓ €            " < R e n d e r M a n a g e r . c p p   `      y<     Ø      pQ2    È¥ YÖÓ                < R e n d e r M a n a g e r . h       `      y<     Ø      ĞQ2    È¥ YÖÓ               < R e n d e r M a n a g e r . h       `      y<     Ø      0Q2    Â» YÖÓ               < R e n d e r M a n a g e r . h       `      y<     Ø      Q2    Â» YÖÓ               < R e n d e r M a n a g e r . h       `      y<     Ø      ğQ2    Â» YÖÓ €             < R e n d e r M a n a g e r . h       P      z<     Ø      PQ2    †2¡YÖÓ                < s c e n e . a u r   P      z<     Ø       Q2    †2¡YÖÓ               < s c e n e . a u r   P      z<     Ø      ğQ2    d8¢YÖÓ               < s c e n e . a u r   P      z<     Ø      @Q2    ğ]¢YÖÓ               < s c e n e . a u r   P      z<     Ø      Q2    ğ]¢YÖÓ €             < s c e n e . a u r   P      {<     Ø      àQ2    àî¢YÖÓ                < S c e n e . c p p   P      {<     Ø      0Q2    £YÖÓ               < S c e n e . c p p   P      {<     Ø      €Q2    ¡ğ¥YÖÓ               < S c e n e . c p p   P      {<     Ø      ĞQ2    ¡ğ¥YÖÓ               < S c e n e . c p p   P      {<     Ø       Q2    ¡ğ¥YÖÓ €             < S c e n e . c p p   P      |<     Ø      pQ2    $ƒ¦YÖÓ                < S c e n e . h     e P      |<     Ø      ÀQ2    $ƒ¦YÖÓ               < S c e n e . h     e P      |<     Ø      Q2    rø§YÖÓ               < S c e n e . h     e P      |<     Ø      `Q2    ¨YÖÓ               < S c e n e . h     e P      |<     Ø      °Q2    ¨YÖÓ €             < S c e n e . h     e X      }<     Ø       Q2    ²¨YÖÓ                < s c e n e _ u . a u r     g X      }<     Ø      XQ2    ²¨YÖÓ               < s c e n e _ u . a u r     g                                                                                 X      }<     Ø        Q2    ğ#ªYÖÓ               < s c e n e _ u . a u r     g X      }<     Ø      X Q2    ğ#ªYÖÓ               < s c e n e _ u . a u r     g X      }<     Ø      ° Q2    ğ#ªYÖÓ €             < s c e n e _ u . a u r     g `      ~<     Ø      !Q2    	¦«YÖÓ               " < S h a d e r P r o g r a m . c p p   `      ~<     Ø      h!Q2    YË«YÖÓ              " < S h a d e r P r o g r a m . c p p   `      ~<     Ø      È!Q2    YË«YÖÓ              " < S h a d e r P r o g r a m . c p p   `      ~<     Ø      ("Q2    YË«YÖÓ              " < S h a d e r P r o g r a m . c p p   `      ~<     Ø      ˆ"Q2    ô«YÖÓ €            " < S h a d e r P r o g r a m . c p p   `      <     Ø      è"Q2    µ{¬YÖÓ                < S h a d e r P r o g r a m . h       `      <     Ø      H#Q2    Ø¢¬YÖÓ               < S h a d e r P r o g r a m . h       `      <     Ø      ¨#Q2    B±¬YÖÓ               < S h a d e r P r o g r a m . h       `      <     Ø      $Q2    B±¬YÖÓ               < S h a d e r P r o g r a m . h       `      <     Ø      h$Q2    B±¬YÖÓ €             < S h a d e r P r o g r a m . h       X      €<     Ø      È$Q2    ³O­YÖÓ                < S H A D O W _ T Y P E . h   X      €<     Ø       %Q2    ³O­YÖÓ               < S H A D O W _ T Y P E . h   X      €<     Ø      x%Q2    Æv­YÖÓ               < S H A D O W _ T Y P E . h   X      €<     Ø      Ğ%Q2    Æv­YÖÓ               < S H A D O W _ T Y P E . h   X      €<     Ø      (&Q2    Æv­YÖÓ €             < S H A D O W _ T Y P E . h   `      <     Ø      €&Q2    ÿ®YÖÓ                < s t b _ r e c t _ p a c k . h       `      <     Ø      à&Q2    ÿ®YÖÓ               < s t b _ r e c t _ p a c k . h       `      <     Ø      @'Q2    V°YÖÓ               < s t b _ r e c t _ p a c k . h       `      <     Ø       'Q2    V°YÖÓ               < s t b _ r e c t _ p a c k . h       `      <     Ø       (Q2    V°YÖÓ €             < s t b _ r e c t _ p a c k . h       X      ‚<     Ø      `(Q2    -ˆ°YÖÓ                < s t b _ t e x t e d i t . h X      ‚<     Ø      ¸(Q2    -ˆ°YÖÓ               < s t b _ t e x t e d i t . h X      ‚<     Ø      )Q2    £°°YÖÓ               < s t b _ t e x t e d i t . h X      ‚<     Ø      h)Q2    £°°YÖÓ               < s t b _ t e x t e d i t . h X      ‚<     Ø      À)Q2    £°°YÖÓ €             < s t b _ t e x t e d i t . h X      ƒ<     Ø      *Q2    ã$±YÖÓ                < s t b _ t r u e t y p e . h X      ƒ<     Ø      p*Q2    ã$±YÖÓ               < s t b _ t r u e t y p e . h X      ƒ<     Ø      È*Q2    L±YÖÓ               < s t b _ t r u e t y p e . h X      ƒ<     Ø       +Q2    ‰±YÖÓ               < s t b _ t r u e t y p e . h X      ƒ<     Ø      x+Q2    "À±YÖÓ €             < s t b _ t r u e t y p e . h h      „<     Ø      Ğ+Q2    £"²YÖÓ               ( < T e r r a i n G e n e r a t o r . c p p     h      „<     Ø      8,Q2    £"²YÖÓ              ( < T e r r a i n G e n e r a t o r . c p p     h      „<     Ø       ,Q2    £"²YÖÓ              ( < T e r r a i n G e n e r a t o r . c p p     h      „<     Ø      -Q2    ÒI²YÖÓ              ( < T e r r a i n G e n e r a t o r . c p p     h      „<     Ø      p-Q2    ÒI²YÖÓ €            ( < T e r r a i n G e n e r a t o r . c p p     `      …<     Ø      Ø-Q2    ëÂ³YÖÓ               $ < T e r r a i n G e n e r a t o r . h `      …<     Ø      8.Q2    ëÂ³YÖÓ              $ < T e r r a i n G e n e r a t o r . h `      …<     Ø      ˜.Q2    -Õ³YÖÓ              $ < T e r r a i n G e n e r a t o r . h `      …<     Ø      ø.Q2    -Õ³YÖÓ              $ < T e r r a i n G e n e r a t o r . h `      …<     Ø      X/Q2    -Õ³YÖÓ €            $ < T e r r a i n G e n e r a t o r . h                                                                         X      †<     Ø       0Q2    o8´YÖÓ                < T e x t u r e . c p p     g X      †<     Ø      X0Q2    o8´YÖÓ               < T e x t u r e . c p p     g X      †<     Ø      °0Q2    o8´YÖÓ               < T e x t u r e . c p p     g X      †<     Ø      1Q2    ìE´YÖÓ               < T e x t u r e . c p p     g X      †<     Ø      `1Q2    ©J´YÖÓ €             < T e x t u r e . c p p     g P      ‡<     Ø      ¸1Q2    ¡]µYÖÓ                < T e x t u r e . h   P      ‡<     Ø      2Q2    ¡]µYÖÓ               < T e x t u r e . h   P      ‡<     Ø      X2Q2    ¡]µYÖÓ               < T e x t u r e . h   P      ‡<     Ø      ¨2Q2    ¡]µYÖÓ               < T e x t u r e . h   P      ‡<     Ø      ø2Q2    ¡]µYÖÓ €             < T e x t u r e . h   h      ˆ<     Ø      H3Q2    É¶YÖÓ               * < T E X T U R E _ R E S O L U T I O N S . h   h      ˆ<     Ø      °3Q2    5¶YÖÓ              * < T E X T U R E _ R E S O L U T I O N S . h   h      ˆ<     Ø      4Q2    5¶YÖÓ              * < T E X T U R E _ R E S O L U T I O N S . h   h      ˆ<     Ø      €4Q2    5¶YÖÓ              * < T E X T U R E _ R E S O L U T I O N S . h   h      ˆ<     Ø      è4Q2    5¶YÖÓ €            * < T E X T U R E _ R E S O L U T I O N S . h   P      ‰<     Ø      P5Q2    ·YÖÓ                < T i m e . c p p     P      ‰<     Ø       5Q2    ;=·YÖÓ               < T i m e . c p p     P      ‰<     Ø      ğ5Q2    ;=·YÖÓ               < T i m e . c p p     P      ‰<     Ø      @6Q2    ;=·YÖÓ               < T i m e . c p p     P      ‰<     Ø      6Q2    ;=·YÖÓ €             < T i m e . c p p     H      Š<     Ø      à6Q2    µ·YÖÓ                < T i m e . h H      Š<     Ø      (7Q2    µ·YÖÓ               < T i m e . h H      Š<     Ø      p7Q2    µ·YÖÓ               < T i m e . h H      Š<     Ø      ¸7Q2    µ·YÖÓ               < T i m e . h H      Š<     Ø       8Q2    µ·YÖÓ €             < T i m e . h X      ‹<     Ø      H8Q2    vºYÖÓ                < t i n y - g i z m o . c p p X      ‹<     Ø       8Q2    >ºYÖÓ               < t i n y - g i z m o . c p p X      ‹<     Ø      ø8Q2    ÚØ½YÖÓ               < t i n y - g i z m o . c p p X      ‹<     Ø      P9Q2    ÚØ½YÖÓ               < t i n y - g i z m o . c p p X      ‹<     Ø      ¨9Q2    ÚØ½YÖÓ#               < t i n y - g i z m o . c p p X      ‹<     Ø       :Q2    ÚØ½YÖÓ3               < t i n y - g i z m o . c p p X      ‹<     Ø      X:Q2    k	¾YÖÓ3               < t i n y - g i z m o . c p p X      ‹<     Ø      °:Q2     R¾YÖÓ3 €             < t i n y - g i z m o . c p p X      Œ<     Ø      ;Q2    ¿É¾YÖÓ                < t i n y - g i z m o . h p p X      Œ<     Ø      `;Q2    ¿É¾YÖÓ               < t i n y - g i z m o . h p p X      Œ<     Ø      ¸;Q2    ø¹ÁYÖÓ               < t i n y - g i z m o . h p p X      Œ<     Ø      <Q2    ?áÁYÖÓ               < t i n y - g i z m o . h p p X      Œ<     Ø      h<Q2    ?áÁYÖÓ#               < t i n y - g i z m o . h p p X      Œ<     Ø      À<Q2    ?áÁYÖÓ3               < t i n y - g i z m o . h p p X      Œ<     Ø      =Q2    ]ÂYÖÓ3               < t i n y - g i z m o . h p p X      Œ<     Ø      p=Q2    &XÂYÖÓ3 €             < t i n y - g i z m o . h p p X      <     Ø      È=Q2    ?áÂYÖÓ                < T r a n s f o r m . c p p   X      <     Ø       >Q2    °óÂYÖÓ               < T r a n s f o r m . c p p   X      <     Ø      x>Q2    ‚ÏÄYÖÓ               < T r a n s f o r m . c p p   X      <     Ø      Ğ>Q2    ‚ÏÄYÖÓ               < T r a n s f o r m . c p p   X      <     Ø      (?Q2    ‚ÏÄYÖÓ €             < T r a n s f o r m . c p p   X      <     Ø      €?Q2    à¤ÆYÖÓ                < T r a n s f o r m . h     g                                         X      <     Ø       @Q2    à¤ÆYÖÓ               < T r a n s f o r m . h     g X      <     Ø      X@Q2    à¤ÆYÖÓ               < T r a n s f o r m . h     g X      <     Ø      °@Q2    ãÊÆYÖÓ               < T r a n s f o r m . h     g X      <     Ø      AQ2    ãÊÆYÖÓ €             < T r a n s f o r m . h     g X      <     Ø      `AQ2    ÒYÈYÖÓ                < V e c t o r 2 . c p p     g X      <     Ø      ¸AQ2    ÒYÈYÖÓ               < V e c t o r 2 . c p p     g X      <     Ø      BQ2    ÒYÈYÖÓ               < V e c t o r 2 . c p p     g X      <     Ø      hBQ2    ÒYÈYÖÓ               < V e c t o r 2 . c p p     g X      <     Ø      ÀBQ2    ÒYÈYÖÓ €             < V e c t o r 2 . c p p     g P      <     Ø      CQ2    UÿÉYÖÓ                < V e c t o r 2 . h   P      <     Ø      hCQ2    UÿÉYÖÓ               < V e c t o r 2 . h   P      <     Ø      ¸CQ2    UÿÉYÖÓ               < V e c t o r 2 . h   P      <     Ø      DQ2    UÿÉYÖÓ               < V e c t o r 2 . h   P      <     Ø      XDQ2    UÿÉYÖÓ €             < V e c t o r 2 . h   X      ‘<     Ø      ¨DQ2    yËYÖÓ                < V e c t o r 3 . c p p     g X      ‘<     Ø       EQ2    yËYÖÓ               < V e c t o r 3 . c p p     g X      ‘<     Ø      XEQ2    ¬’ÌYÖÓ               < V e c t o r 3 . c p p     g X      ‘<     Ø      °EQ2    ¬’ÌYÖÓ               < V e c t o r 3 . c p p     g X      ‘<     Ø      FQ2    ¬’ÌYÖÓ €             < V e c t o r 3 . c p p     g P      ’<     Ø      `FQ2    }ÎYÖÓ                < V e c t o r 3 . h   P      ’<     Ø      °FQ2    ë@ÎYÖÓ               < V e c t o r 3 . h   P      ’<     Ø       GQ2    ë@ÎYÖÓ               < V e c t o r 3 . h   P      ’<     Ø      PGQ2    úIÎYÖÓ               < V e c t o r 3 . h   P      ’<     Ø       GQ2    úIÎYÖÓ €             < V e c t o r 3 . h   X      “<     Ø      ğGQ2    íãÎYÖÓ                < V e r t e x . h l s l     g X      “<     Ø      HHQ2    íãÎYÖÓ               < V e r t e x . h l s l     g X      “<     Ø       HQ2    ÏÏYÖÓ               < V e r t e x . h l s l     g X      “<     Ø      øHQ2    ÏÏYÖÓ               < V e r t e x . h l s l     g X      “<     Ø      PIQ2    ÏÏYÖÓ €             < V e r t e x . h l s l     g P      ”<     Ø      ¨IQ2    T˜ÏYÖÓ                < W i n d o w . h     P      ”<     Ø      øIQ2    T˜ÏYÖÓ               < W i n d o w . h     P      ”<     Ø      HJQ2    ÒÏYÖÓ               < W i n d o w . h     P      ”<     Ø      ˜JQ2    ÒÏYÖÓ               < W i n d o w . h     P      ”<     Ø      èJQ2    ÒÏYÖÓ €             < W i n d o w . h     H      •<     Ø      8KQ2    ë˜ĞYÖÓ               < . g i t     H      •<     Ø      €KQ2    ë˜ĞYÖÓ  €            < . g i t     H      •<     Ø      ÈKQ2    "ÀĞYÖÓ €              < . g i t     H      •<     Ø      LQ2    "ÀĞYÖÓ € €            < . g i t     H      –<     •<     XLQ2    ¬=ÑYÖÓ                < c o n f i g H      –<     •<      LQ2    ¬=ÑYÖÓ               < c o n f i g H      –<     •<     èLQ2    ¬=ÑYÖÓ               < c o n f i g H      –<     •<     0MQ2    ¬=ÑYÖÓ               < c o n f i g H      –<     •<     xMQ2    ¬=ÑYÖÓ €             < c o n f i g X      —<     •<     ÀMQ2    	ÅÓYÖÓ                < d e s c r i p t i o n     F7X      —<     •<     NQ2    	ÅÓYÖÓ               < d e s c r i p t i o n     F7X      —<     •<     pNQ2    	ÅÓYÖÓ               < d e s c r i p t i o n     F7X      —<     •<     ÈNQ2    	ÅÓYÖÓ               < d e s c r i p t i o n     F7X      —<     •<      OQ2    	ÅÓYÖÓ €             < d e s c r i p t i o n     F7P      ˜<     •<     xOQ2    øêÔYÖÓ                < F E T C H _ H E A D                                                         P      ˜<     •<      PQ2    øêÔYÖÓ               < F E T C H _ H E A D P      ˜<     •<     PPQ2    øêÔYÖÓ               < F E T C H _ H E A D P      ˜<     •<      PQ2    øêÔYÖÓ               < F E T C H _ H E A D P      ˜<     •<     ğPQ2    øêÔYÖÓ €             < F E T C H _ H E A D H      ™<     •<     @QQ2    ÕYÖÓ                < H E A D     H      ™<     •<     ˆQQ2    ÕYÖÓ               < H E A D     H      ™<     •<     ĞQQ2    ÕYÖÓ               < H E A D     H      ™<     •<     RQ2    ÕYÖÓ               < H E A D     H      ™<     •<     `RQ2    ÕYÖÓ €             < H E A D     H      š<     •<     ¨RQ2    şÖYÖÓ               
 < i n d e x   H      š<     •<     ğRQ2    şÖYÖÓ              
 < i n d e x   H      š<     •<     8SQ2    ËzØYÖÓ              
 < i n d e x   H      š<     •<     €SQ2    óØYÖÓ              
 < i n d e x   H      š<     •<     ÈSQ2    óØYÖÓ €            
 < i n d e x   X      ›<     •<     TQ2    ›)ÙYÖÓ                < m s - p e r s i s t . x m l X      ›<     •<     hTQ2    ›)ÙYÖÓ               < m s - p e r s i s t . x m l X      ›<     •<     ÀTQ2    ›)ÙYÖÓ               < m s - p e r s i s t . x m l X      ›<     •<     UQ2    ›)ÙYÖÓ               < m s - p e r s i s t . x m l X      ›<     •<     pUQ2    ÆPÙYÖÓ €             < m s - p e r s i s t . x m l P      œ<     •<     ÈUQ2    hOÛYÖÓ                < O R I G _ H E A D   P      œ<     •<     VQ2    hOÛYÖÓ               < O R I G _ H E A D   P      œ<     •<     hVQ2    hOÛYÖÓ               < O R I G _ H E A D   P      œ<     •<     ¸VQ2    hOÛYÖÓ               < O R I G _ H E A D   P      œ<     •<     WQ2    hOÛYÖÓ €             < O R I G _ H E A D   X      <     •<     XWQ2    óÈÛYÖÓ                < p a c k e d - r e f s       X      <     •<     °WQ2    óÈÛYÖÓ               < p a c k e d - r e f s       X      <     •<     XQ2    óÈÛYÖÓ               < p a c k e d - r e f s       X      <     •<     `XQ2    ²îÛYÖÓ               < p a c k e d - r e f s       X      <     •<     ¸XQ2    ²îÛYÖÓ €             < p a c k e d - r e f s       H      <     •<     YQ2    eÜYÖÓ              
 < h o o k s   H      <     •<     XYQ2    eÜYÖÓ  €           
 < h o o k s   H      <     •<      YQ2    U‹ÜYÖÓ €             
 < h o o k s   H      <     •<     èYQ2    U‹ÜYÖÓ € €           
 < h o o k s   h      Ÿ<     <     0ZQ2    ˜İYÖÓ               * < a p p l y p a t c h - m s g . s a m p l e   h      Ÿ<     <     ˜ZQ2    ˜İYÖÓ              * < a p p l y p a t c h - m s g . s a m p l e   h      Ÿ<     <      [Q2    ˜İYÖÓ              * < a p p l y p a t c h - m s g . s a m p l e   h      Ÿ<     <     h[Q2    ˜İYÖÓ              * < a p p l y p a t c h - m s g . s a m p l e   h      Ÿ<     <     Ğ[Q2    (İYÖÓ €            * < a p p l y p a t c h - m s g . s a m p l e   `       <     <     8\Q2     İYÖÓ               " < c o m m i t - m s g . s a m p l e   `       <     <     ˜\Q2     İYÖÓ              " < c o m m i t - m s g . s a m p l e   `       <     <     ø\Q2    ’íßYÖÓ              " < c o m m i t - m s g . s a m p l e   `       <     <     X]Q2    ÄàYÖÓ              " < c o m m i t - m s g . s a m p l e   `       <     <     ¸]Q2    ‘dàYÖÓ €            " < c o m m i t - m s g . s a m p l e   p      ¡<     <     ^Q2    áYÖÓ               2 < f s m o n i t o r - w a t c h m a n . s a m p l e   p      ¡<     <     ˆ^Q2    'áYÖÓ              2 < f s m o n i t o r - w a t c h m a n . s a m p l e   p      ¡<     <     ø^Q2    ›œâYÖÓ              2 < f s m o n i t o r - w a t c h m a n . s a m p l e   p      ¡<     <     h_Q2    ›œâYÖÓ              2 < f s m o n i t o r - w a t c h m a n . s a m p l e                                           p      ¡<     <      `Q2    ½®âYÖÓ €            2 < f s m o n i t o r - w a t c h m a n . s a m p l e   `      ¢<     <     p`Q2    ıâYÖÓ               $ < p o s t - u p d a t e . s a m p l e `      ¢<     <     Ğ`Q2    ıâYÖÓ              $ < p o s t - u p d a t e . s a m p l e `      ¢<     <     0aQ2    ıâYÖÓ              $ < p o s t - u p d a t e . s a m p l e `      ¢<     <     aQ2    ıâYÖÓ              $ < p o s t - u p d a t e . s a m p l e `      ¢<     <     ğaQ2    ıâYÖÓ €            $ < p o s t - u p d a t e . s a m p l e h      £<     <     PbQ2    ªŠãYÖÓ               * < p r e - a p p l y p a t c h . s a m p l e   h      £<     <     ¸bQ2    ªŠãYÖÓ              * < p r e - a p p l y p a t c h . s a m p l e   h      £<     <      cQ2    ªŠãYÖÓ              * < p r e - a p p l y p a t c h . s a m p l e   h      £<     <     ˆcQ2    ªŠãYÖÓ              * < p r e - a p p l y p a t c h . s a m p l e   h      £<     <     ğcQ2    ªŠãYÖÓ €            * < p r e - a p p l y p a t c h . s a m p l e   `      ¤<     <     XdQ2    BäYÖÓ               " < p r e - c o m m i t . s a m p l e   `      ¤<     <     ¸dQ2    BäYÖÓ              " < p r e - c o m m i t . s a m p l e   `      ¤<     <     eQ2    JAåYÖÓ              " < p r e - c o m m i t . s a m p l e   `      ¤<     <     xeQ2    JAåYÖÓ              " < p r e - c o m m i t . s a m p l e   `      ¤<     <     ØeQ2    JAåYÖÓ €            " < p r e - c o m m i t . s a m p l e   `      ¥<     <     8fQ2    ØJæYÖÓ                < p r e - p u s h . s a m p l e     4}`      ¥<     <     ˜fQ2    àræYÖÓ               < p r e - p u s h . s a m p l e     4}`      ¥<     <     øfQ2    éÑçYÖÓ               < p r e - p u s h . s a m p l e     4}`      ¥<     <     XgQ2    éÑçYÖÓ               < p r e - p u s h . s a m p l e     4}`      ¥<     <     ¸gQ2    ùçYÖÓ €             < p r e - p u s h . s a m p l e     4}`      ¦<     <     hQ2    eGèYÖÓ               " < p r e - r e b a s e . s a m p l e   `      ¦<     <     xhQ2    eGèYÖÓ              " < p r e - r e b a s e . s a m p l e   `      ¦<     <     ØhQ2    “êYÖÓ              " < p r e - r e b a s e . s a m p l e   `      ¦<     <     8iQ2    “êYÖÓ              " < p r e - r e b a s e . s a m p l e   `      ¦<     <     ˜iQ2    ºêYÖÓ €            " < p r e - r e b a s e . s a m p l e   `      §<     <     øiQ2    õ}ëYÖÓ               $ < p r e - r e c e i v e . s a m p l e `      §<     <     XjQ2    õ}ëYÖÓ              $ < p r e - r e c e i v e . s a m p l e `      §<     <     ¸jQ2    ¥ëYÖÓ              $ < p r e - r e c e i v e . s a m p l e `      §<     <     kQ2    ¥ëYÖÓ              $ < p r e - r e c e i v e . s a m p l e `      §<     <     xkQ2    ¥ëYÖÓ €            $ < p r e - r e c e i v e . s a m p l e p      ¨<     <     ØkQ2    ¡ôëYÖÓ               2 < p r e p a r e - c o m m i t - m s g . s a m p l e   p      ¨<     <     HlQ2    ¡ôëYÖÓ              2 < p r e p a r e - c o m m i t - m s g . s a m p l e   p      ¨<     <     ¸lQ2    øíYÖÓ              2 < p r e p a r e - c o m m i t - m s g . s a m p l e   p      ¨<     <     (mQ2    øíYÖÓ              2 < p r e p a r e - c o m m i t - m s g . s a m p l e   p      ¨<     <     ˜mQ2    øíYÖÓ €            2 < p r e p a r e - c o m m i t - m s g . s a m p l e   X      ©<     <     nQ2    MUíYÖÓ                < u p d a t e . s a m p l e   X      ©<     <     `nQ2    `|íYÖÓ               < u p d a t e . s a m p l e   X      ©<     <     ¸nQ2    n´îYÖÓ               < u p d a t e . s a m p l e   X      ©<     <     oQ2    n´îYÖÓ               < u p d a t e . s a m p l e   X      ©<     <     hoQ2    n´îYÖÓ €             < u p d a t e . s a m p l e                                                                   H      ª<     •<      pQ2    òğğYÖÓ               < i n f o     H      ª<     •<     HpQ2    òğğYÖÓ  €            < i n f o     H      ª<     •<     pQ2    G?ñYÖÓ €              < i n f o     H      ª<     •<     ØpQ2    G?ñYÖÓ € €            < i n f o     P      «<     ª<      qQ2    hÈñYÖÓ                < e x c l u d e       P      «<     ª<     pqQ2    hÈñYÖÓ               < e x c l u d e       P      «<     ª<     ÀqQ2    ÜñYÖÓ               < e x c l u d e       P      «<     ª<     rQ2    ÜñYÖÓ               < e x c l u d e       P      «<     ª<     `rQ2    “ïñYÖÓ €             < e x c l u d e       H      ¬<     •<     °rQ2    ÀxòYÖÓ               < l o g s     H      ¬<     •<     ørQ2    yòYÖÓ  €            < l o g s     H      ¬<     •<     @sQ2    yòYÖÓ €              < l o g s     H      ¬<     •<     ˆsQ2    yòYÖÓ € €            < l o g s     H      ­<     ¬<     ĞsQ2    ÈòYÖÓ                < H E A D     H      ­<     ¬<     tQ2    ÈòYÖÓ               < H E A D     H      ­<     ¬<     `tQ2    ÈòYÖÓ               < H E A D     H      ­<     ¬<     ¨tQ2    ÈòYÖÓ               < H E A D     H      ­<     ¬<     ğtQ2    ÈòYÖÓ €             < H E A D     H      ®<     ¬<     8uQ2    LóYÖÓ               < r e f s     H      ®<     ¬<     €uQ2    LóYÖÓ  €            < r e f s     H      ®<     ¬<     ÈuQ2    ‚=óYÖÓ €              < r e f s     H      ®<     ¬<     vQ2    ‚=óYÖÓ € €            < r e f s     H      ¯<     ®<     XvQ2    (óYÖÓ              
 < h e a d s   H      ¯<     ®<      vQ2    (óYÖÓ  €           
 < h e a d s   H      ¯<     ®<     èvQ2    û²óYÖÓ €             
 < h e a d s   H      ¯<     ®<     0wQ2    û²óYÖÓ € €           
 < h e a d s   H      °<     ¯<     xwQ2    ã/ôYÖÓ                < m a s t e r H      °<     ¯<     ÀwQ2    OôYÖÓ               < m a s t e r H      °<     ¯<     xQ2    OôYÖÓ               < m a s t e r H      °<     ¯<     PxQ2    OôYÖÓ               < m a s t e r H      °<     ¯<     ˜xQ2    OôYÖÓ €             < m a s t e r P      ±<     ®<     àxQ2    ôYÖÓ               < r e m o t e s     ÓP      ±<     ®<     0yQ2    ôYÖÓ  €            < r e m o t e s     ÓP      ±<     ®<     €yQ2    )ÅôYÖÓ €              < r e m o t e s     ÓP      ±<     ®<     ĞyQ2    )ÅôYÖÓ € €            < r e m o t e s     ÓH      ²<     ±<      zQ2    wõYÖÓ               < o r i g i n H      ²<     ±<     hzQ2    wõYÖÓ  €            < o r i g i n H      ²<     ±<     °zQ2    ­:õYÖÓ €              < o r i g i n H      ²<     ±<     øzQ2    ­:õYÖÓ € €            < o r i g i n H      ³<     ²<     @{Q2    #ŠõYÖÓ                < H E A D     H      ³<     ²<     ˆ{Q2    #ŠõYÖÓ               < H E A D     H      ³<     ²<     Ğ{Q2    #ŠõYÖÓ               < H E A D     H      ³<     ²<     |Q2    #ŠõYÖÓ               < H E A D     H      ³<     ²<     `|Q2    #ŠõYÖÓ €             < H E A D     H      ´<     ²<     ¨|Q2    ]ØõYÖÓ                < m a s t e r H      ´<     ²<     ğ|Q2    ]ØõYÖÓ               < m a s t e r H      ´<     ²<     8}Q2    ]ØõYÖÓ               < m a s t e r H      ´<     ²<     €}Q2    ]ØõYÖÓ               < m a s t e r H      ´<     ²<     È}Q2    ]ØõYÖÓ €             < m a s t e r P      µ<     •<     ~Q2    ØLöYÖÓ               < o b j e c t s       P      µ<     •<     `~Q2    ØLöYÖÓ  €            < o b j e c t s       P      µ<     •<     °~Q2    töYÖÓ €              < o b j e c t s       P      µ<     •<      Q2    töYÖÓ € €            < o b j e c t s       @      ¶<     µ<     PQ2    hÂöYÖÓ               < 3 2 @      ¶<     µ<     Q2    hÂöYÖÓ  €            < 3 2                                                 @      ¶<     µ<      €Q2    hÂöYÖÓ €              < 3 2 @      ¶<     µ<     @€Q2    hÂöYÖÓ € €            < 3 2 ˆ      ·<     ¶<     €€Q2    ¹÷YÖÓ           !   L < 4 f 0 d 4 d a 6 4 d b c 2 c 0 4 7 0 f 6 3 c e f 5 6 2 d 4 a 7 f b e 2 f f 3 ˆ      ·<     ¶<     Q2    ¹÷YÖÓ          !   L < 4 f 0 d 4 d a 6 4 d b c 2 c 0 4 7 0 f 6 3 c e f 5 6 2 d 4 a 7 f b e 2 f f 3 ˆ      ·<     ¶<     Q2    ¹÷YÖÓ          !   L < 4 f 0 d 4 d a 6 4 d b c 2 c 0 4 7 0 f 6 3 c e f 5 6 2 d 4 a 7 f b e 2 f f 3 ˆ      ·<     ¶<     ‚Q2    ¹÷YÖÓ          !   L < 4 f 0 d 4 d a 6 4 d b c 2 c 0 4 7 0 f 6 3 c e f 5 6 2 d 4 a 7 f b e 2 f f 3 ˆ      ·<     ¶<      ‚Q2    ¹÷YÖÓ €        !   L < 4 f 0 d 4 d a 6 4 d b c 2 c 0 4 7 0 f 6 3 c e f 5 6 2 d 4 a 7 f b e 2 f f 3 @      ¸<     µ<     (ƒQ2    _÷YÖÓ               < 6 4 @      ¸<     µ<     hƒQ2    _÷YÖÓ  €            < 6 4 @      ¸<     µ<     ¨ƒQ2    A†÷YÖÓ €              < 6 4 @      ¸<     µ<     èƒQ2    A†÷YÖÓ € €            < 6 4 ˆ      ¹<     ¸<     („Q2    `­÷YÖÓ           !   L < 5 8 4 b e c b 5 1 5 0 0 0 b 3 c 1 8 9 a a 1 9 9 4 8 e 7 5 a 3 c 1 8 7 7 1 b ˆ      ¹<     ¸<     °„Q2    `­÷YÖÓ          !   L < 5 8 4 b e c b 5 1 5 0 0 0 b 3 c 1 8 9 a a 1 9 9 4 8 e 7 5 a 3 c 1 8 7 7 1 b ˆ      ¹<     ¸<     8…Q2    "GúYÖÓ          !   L < 5 8 4 b e c b 5 1 5 0 0 0 b 3 c 1 8 9 a a 1 9 9 4 8 e 7 5 a 3 c 1 8 7 7 1 b ˆ      ¹<     ¸<     À…Q2    "GúYÖÓ          !   L < 5 8 4 b e c b 5 1 5 0 0 0 b 3 c 1 8 9 a a 1 9 9 4 8 e 7 5 a 3 c 1 8 7 7 1 b ˆ      ¹<     ¸<     H†Q2    LnúYÖÓ €        !   L < 5 8 4 b e c b 5 1 5 0 0 0 b 3 c 1 8 9 a a 1 9 9 4 8 e 7 5 a 3 c 1 8 7 7 1 b @      º<     µ<     Ğ†Q2    _äúYÖÓ               < 6 e @      º<     µ<     ‡Q2    _äúYÖÓ  €            < 6 e @      º<     µ<     P‡Q2    û
ûYÖÓ €              < 6 e @      º<     µ<     ‡Q2    û
ûYÖÓ € €            < 6 e ˆ      »<     º<     Ğ‡Q2    QYûYÖÓ           !   L < 6 5 6 8 e d 7 6 2 6 2 e 0 7 5 c 2 f 0 8 0 0 9 c b 9 0 f 0 8 a 5 e 5 9 7 0 b ˆ      »<     º<     XˆQ2    QYûYÖÓ          !   L < 6 5 6 8 e d 7 6 2 6 2 e 0 7 5 c 2 f 0 8 0 0 9 c b 9 0 f 0 8 a 5 e 5 9 7 0 b ˆ      »<     º<     àˆQ2    g÷ûYÖÓ          !   L < 6 5 6 8 e d 7 6 2 6 2 e 0 7 5 c 2 f 0 8 0 0 9 c b 9 0 f 0 8 a 5 e 5 9 7 0 b ˆ      »<     º<     h‰Q2    "üYÖÓ          !   L < 6 5 6 8 e d 7 6 2 6 2 e 0 7 5 c 2 f 0 8 0 0 9 c b 9 0 f 0 8 a 5 e 5 9 7 0 b ˆ      »<     º<     ğ‰Q2    "üYÖÓ €        !   L < 6 5 6 8 e d 7 6 2 6 2 e 0 7 5 c 2 f 0 8 0 0 9 c b 9 0 f 0 8 a 5 e 5 9 7 0 b @      ¼<     µ<     xŠQ2    ¢’üYÖÓ               < 8 0 @      ¼<     µ<     ¸ŠQ2    ¢’üYÖÓ  €            < 8 0 @      ¼<     µ<     øŠQ2    ÷ËıYÖÓ €              < 8 0 @      ¼<     µ<     8‹Q2    ÷ËıYÖÓ € €            < 8 0 ˆ      ½<     ¼<     x‹Q2    MşYÖÓ           !   L < a 6 0 7 3 5 b 6 8 4 b b b 3 f b 8 f e 2 1 e 2 e d 0 f 6 7 2 d 8 3 e b 5 c 0 ˆ      ½<     ¼<      ŒQ2    MşYÖÓ          !   L < a 6 0 7 3 5 b 6 8 4 b b b 3 f b 8 f e 2 1 e 2 e d 0 f 6 7 2 d 8 3 e b 5 c 0 ˆ      ½<     ¼<     ˆŒQ2    ûBşYÖÓ          !   L < a 6 0 7 3 5 b 6 8 4 b b b 3 f b 8 f e 2 1 e 2 e d 0 f 6 7 2 d 8 3 e b 5 c 0 ˆ      ½<     ¼<     Q2    ûBşYÖÓ          !   L < a 6 0 7 3 5 b 6 8 4 b b b 3 f b 8 f e 2 1 e 2 e d 0 f 6 7 2 d 8 3 e b 5 c 0 ˆ      ½<     ¼<     ˜Q2    ûBşYÖÓ €        !   L < a 6 0 7 3 5 b 6 8 4 b b b 3 f b 8 f e 2 1 e 2 e d 0 f 6 7 2 d 8 3 e b 5 c 0 @      ¾<     µ<      Q2    ‘şYÖÓ               < 8 5 @      ¾<     µ<     `Q2    ®¸şYÖÓ  €            < 8 5 @      ¾<     µ<      Q2    ®¸şYÖÓ €              < 8 5 @      ¾<     µ<     àQ2    ®¸şYÖÓ € €            < 8 5 ˆ      ¿<     ¾<      Q2    ÿÿYÖÓ           !   L < 5 e 5 6 4 0 4 7 f 4 6 6 6 2 c 3 b 0 9 7 d 8 0 6 2 a 6 5 d d 1 6 4 e 2 c f b                                                                                         ˆ      ¿<     ¾<      Q2    ÿÿYÖÓ          !   L < 5 e 5 6 4 0 4 7 f 4 6 6 6 2 c 3 b 0 9 7 d 8 0 6 2 a 6 5 d d 1 6 4 e 2 c f b ˆ      ¿<     ¾<     ˆQ2    ÿÿYÖÓ          !   L < 5 e 5 6 4 0 4 7 f 4 6 6 6 2 c 3 b 0 9 7 d 8 0 6 2 a 6 5 d d 1 6 4 e 2 c f b ˆ      ¿<     ¾<     ‘Q2    ÿÿYÖÓ          !   L < 5 e 5 6 4 0 4 7 f 4 6 6 6 2 c 3 b 0 9 7 d 8 0 6 2 a 6 5 d d 1 6 4 e 2 c f b ˆ      ¿<     ¾<     ˜‘Q2    Ğ.ÿYÖÓ €        !   L < 5 e 5 6 4 0 4 7 f 4 6 6 6 2 c 3 b 0 9 7 d 8 0 6 2 a 6 5 d d 1 6 4 e 2 c f b @      À<     µ<      ’Q2    &~ÿYÖÓ               < 8 7 @      À<     µ<     `’Q2    öÿYÖÓ  €            < 8 7 @      À<     µ<      ’Q2    l¤ÿYÖÓ €              < 8 7 @      À<     µ<     à’Q2    l¤ÿYÖÓ € €            < 8 7 ˆ      Á<     À<      “Q2    0ßÿYÖÓ           !   L < 6 a f b c 2 c 5 e 2 9 0 c 1 e d 1 0 b 8 3 7 c 4 f a 2 e 0 d 9 d 5 8 4 5 6 5 ˆ      Á<     À<     ¨“Q2    ÁòÿYÖÓ          !   L < 6 a f b c 2 c 5 e 2 9 0 c 1 e d 1 0 b 8 3 7 c 4 f a 2 e 0 d 9 d 5 8 4 5 6 5 ˆ      Á<     À<     0”Q2    ¤ ZÖÓ          !   L < 6 a f b c 2 c 5 e 2 9 0 c 1 e d 1 0 b 8 3 7 c 4 f a 2 e 0 d 9 d 5 8 4 5 6 5 ˆ      Á<     À<     ¸”Q2    –¶ ZÖÓ          !   L < 6 a f b c 2 c 5 e 2 9 0 c 1 e d 1 0 b 8 3 7 c 4 f a 2 e 0 d 9 d 5 8 4 5 6 5 ˆ      Á<     À<     @•Q2    –¶ ZÖÓ €        !   L < 6 a f b c 2 c 5 e 2 9 0 c 1 e d 1 0 b 8 3 7 c 4 f a 2 e 0 d 9 d 5 8 4 5 6 5 @      Â<     µ<     È•Q2    wZÖÓ               < 9 3 @      Â<     µ<     –Q2    wZÖÓ  €            < 9 3 @      Â<     µ<     H–Q2    <ZÖÓ €              < 9 3 @      Â<     µ<     ˆ–Q2    <ZÖÓ € €            < 9 3 ˆ      Ã<     Â<     È–Q2    ÄTZÖÓ           !   L < c 6 c c f 9 0 8 4 8 a d 3 e 7 c 9 3 9 3 0 7 7 5 6 8 7 5 7 9 4 1 6 9 6 2 6 e ˆ      Ã<     Â<     P—Q2    ÄTZÖÓ          !   L < c 6 c c f 9 0 8 4 8 a d 3 e 7 c 9 3 9 3 0 7 7 5 6 8 7 5 7 9 4 1 6 9 6 2 6 e ˆ      Ã<     Â<     Ø—Q2    bhZÖÓ          !   L < c 6 c c f 9 0 8 4 8 a d 3 e 7 c 9 3 9 3 0 7 7 5 6 8 7 5 7 9 4 1 6 9 6 2 6 e ˆ      Ã<     Â<     `˜Q2    bhZÖÓ          !   L < c 6 c c f 9 0 8 4 8 a d 3 e 7 c 9 3 9 3 0 7 7 5 6 8 7 5 7 9 4 1 6 9 6 2 6 e ˆ      Ã<     Â<     è˜Q2    bhZÖÓ €        !   L < c 6 c c f 9 0 8 4 8 a d 3 e 7 c 9 3 9 3 0 7 7 5 6 8 7 5 7 9 4 1 6 9 6 2 6 e @      Ä<     µ<     p™Q2    -·ZÖÓ               < 9 9 @      Ä<     µ<     °™Q2    -·ZÖÓ  €            < 9 9 @      Ä<     µ<     ğ™Q2    õÜZÖÓ €              < 9 9 @      Ä<     µ<     0šQ2    õÜZÖÓ € €            < 9 9 ˆ      Å<     Ä<     pšQ2    8¸ZÖÓ           !   L < 3 f c 9 1 c 1 8 6 5 7 e e 6 0 1 6 c e 9 d f c 9 3 7 9 b c c d f 3 5 4 d 4 e ˆ      Å<     Ä<     øšQ2    8¸ZÖÓ          !   L < 3 f c 9 1 c 1 8 6 5 7 e e 6 0 1 6 c e 9 d f c 9 3 7 9 b c c d f 3 5 4 d 4 e ˆ      Å<     Ä<     €›Q2    8¸ZÖÓ          !   L < 3 f c 9 1 c 1 8 6 5 7 e e 6 0 1 6 c e 9 d f c 9 3 7 9 b c c d f 3 5 4 d 4 e ˆ      Å<     Ä<     œQ2    8¸ZÖÓ          !   L < 3 f c 9 1 c 1 8 6 5 7 e e 6 0 1 6 c e 9 d f c 9 3 7 9 b c c d f 3 5 4 d 4 e ˆ      Å<     Ä<     œQ2    8¸ZÖÓ €        !   L < 3 f c 9 1 c 1 8 6 5 7 e e 6 0 1 6 c e 9 d f c 9 3 7 9 b c c d f 3 5 4 d 4 e @      Æ<     µ<     Q2    <­ZÖÓ               < b 9 @      Æ<     µ<     XQ2    <­ZÖÓ  €            < b 9 @      Æ<     µ<     ˜Q2    \ÖZÖÓ €              < b 9 @      Æ<     µ<     ØQ2    \ÖZÖÓ € €            < b 9 ˆ      Ç<     Æ<     Q2    ÿZÖÓ           !   L < 8 e d 3 1 6 6 5 0 e 4 0 1 3 8 d d c 2 a 2 1 5 b 4 9 f 0 f 9 d 9 1 6 e 1 1 9 ˆ      Ç<     Æ<      Q2    ÿZÖÓ          !   L < 8 e d 3 1 6 6 5 0 e 4 0 1 3 8 d d c 2 a 2 1 5 b 4 9 f 0 f 9 d 9 1 6 e 1 1 9 ˆ      Ç<     Æ<     (ŸQ2    ÿZÖÓ          !   L < 8 e d 3 1 6 6 5 0 e 4 0 1 3 8 d d c 2 a 2 1 5 b 4 9 f 0 f 9 d 9 1 6 e 1 1 9                                                                                 ˆ      Ç<     Æ<       Q2    ÿZÖÓ          !   L < 8 e d 3 1 6 6 5 0 e 4 0 1 3 8 d d c 2 a 2 1 5 b 4 9 f 0 f 9 d 9 1 6 e 1 1 9 ˆ      Ç<     Æ<     ˆ Q2    ®$ZÖÓ €        !   L < 8 e d 3 1 6 6 5 0 e 4 0 1 3 8 d d c 2 a 2 1 5 b 4 9 f 0 f 9 d 9 1 6 e 1 1 9 @      È<     µ<     ¡Q2    uZÖÓ               < c 5 @      È<     µ<     P¡Q2    uZÖÓ  €            < c 5 @      È<     µ<     ¡Q2    uZÖÓ €              < c 5 @      È<     µ<     Ğ¡Q2    uZÖÓ € €            < c 5 ˆ      É<     È<     ¢Q2    `ÁZÖÓ           !   L < e 5 8 0 a 0 b d 9 c 8 8 6 0 e 1 1 a 7 8 9 e 2 5 9 6 b 2 6 6 3 5 6 9 3 b f 5 ˆ      É<     È<     ˜¢Q2    êZÖÓ          !   L < e 5 8 0 a 0 b d 9 c 8 8 6 0 e 1 1 a 7 8 9 e 2 5 9 6 b 2 6 6 3 5 6 9 3 b f 5 ˆ      É<     È<      £Q2    à!ZÖÓ          !   L < e 5 8 0 a 0 b d 9 c 8 8 6 0 e 1 1 a 7 8 9 e 2 5 9 6 b 2 6 6 3 5 6 9 3 b f 5 ˆ      É<     È<     ¨£Q2    à!ZÖÓ          !   L < e 5 8 0 a 0 b d 9 c 8 8 6 0 e 1 1 a 7 8 9 e 2 5 9 6 b 2 6 6 3 5 6 9 3 b f 5 ˆ      É<     È<     0¤Q2    à!ZÖÓ €        !   L < e 5 8 0 a 0 b d 9 c 8 8 6 0 e 1 1 a 7 8 9 e 2 5 9 6 b 2 6 6 3 5 6 9 3 b f 5 @      Ê<     µ<     ¸¤Q2    d—ZÖÓ               < c 6 @      Ê<     µ<     ø¤Q2    d—ZÖÓ  €            < c 6 @      Ê<     µ<     8¥Q2    ÂåZÖÓ €              < c 6 @      Ê<     µ<     x¥Q2    ÂåZÖÓ € €            < c 6 ˆ      Ë<     Ê<     ¸¥Q2    34ZÖÓ           !   L < 2 5 8 b 3 8 2 d d 9 e d 1 1 c c e 8 9 7 3 f f 3 e 7 9 1 1 4 0 3 1 3 8 b 5 f ˆ      Ë<     Ê<     @¦Q2    34ZÖÓ          !   L < 2 5 8 b 3 8 2 d d 9 e d 1 1 c c e 8 9 7 3 f f 3 e 7 9 1 1 4 0 3 1 3 8 b 5 f ˆ      Ë<     Ê<     È¦Q2    34ZÖÓ          !   L < 2 5 8 b 3 8 2 d d 9 e d 1 1 c c e 8 9 7 3 f f 3 e 7 9 1 1 4 0 3 1 3 8 b 5 f ˆ      Ë<     Ê<     P§Q2    34ZÖÓ          !   L < 2 5 8 b 3 8 2 d d 9 e d 1 1 c c e 8 9 7 3 f f 3 e 7 9 1 1 4 0 3 1 3 8 b 5 f ˆ      Ë<     Ê<     Ø§Q2    B[ZÖÓ €        !   L < 2 5 8 b 3 8 2 d d 9 e d 1 1 c c e 8 9 7 3 f f 3 e 7 9 1 1 4 0 3 1 3 8 b 5 f @      Ì<     µ<     `¨Q2    ›©ZÖÓ               < c d @      Ì<     µ<      ¨Q2    ›©ZÖÓ  €            < c d @      Ì<     µ<     à¨Q2    áĞZÖÓ €              < c d @      Ì<     µ<      ©Q2    áĞZÖÓ € €            < c d ˆ      Í<     Ì<     `©Q2    z	ZÖÓ           !   L < 2 c 4 9 9 6 5 2 9 0 e f 1 b a 2 2 3 f a d d e e 6 9 4 e a a 2 4 e c 5 6 c b ˆ      Í<     Ì<     è©Q2    z	ZÖÓ          !   L < 2 c 4 9 9 6 5 2 9 0 e f 1 b a 2 2 3 f a d d e e 6 9 4 e a a 2 4 e c 5 6 c b ˆ      Í<     Ì<     pªQ2    z	ZÖÓ          !   L < 2 c 4 9 9 6 5 2 9 0 e f 1 b a 2 2 3 f a d d e e 6 9 4 e a a 2 4 e c 5 6 c b ˆ      Í<     Ì<     øªQ2    z	ZÖÓ          !   L < 2 c 4 9 9 6 5 2 9 0 e f 1 b a 2 2 3 f a d d e e 6 9 4 e a a 2 4 e c 5 6 c b ˆ      Í<     Ì<     €«Q2    z	ZÖÓ €        !   L < 2 c 4 9 9 6 5 2 9 0 e f 1 b a 2 2 3 f a d d e e 6 9 4 e a a 2 4 e c 5 6 c b @      Î<     µ<     ¬Q2    7o	ZÖÓ               < d b @      Î<     µ<     H¬Q2    7o	ZÖÓ  €            < d b @      Î<     µ<     ˆ¬Q2    ş”	ZÖÓ €              < d b @      Î<     µ<     È¬Q2    ş”	ZÖÓ € €            < d b ˆ      Ï<     Î<     ­Q2    7â	ZÖÓ           !   L < 9 a a d a 1 a 6 c d c b 8 0 5 0 8 8 6 c 4 e b 4 f 1 3 4 d 6 7 7 9 7 d 5 1 6 ˆ      Ï<     Î<     ­Q2    7â	ZÖÓ          !   L < 9 a a d a 1 a 6 c d c b 8 0 5 0 8 8 6 c 4 e b 4 f 1 3 4 d 6 7 7 9 7 d 5 1 6 ˆ      Ï<     Î<     ®Q2    mõ	ZÖÓ          !   L < 9 a a d a 1 a 6 c d c b 8 0 5 0 8 8 6 c 4 e b 4 f 1 3 4 d 6 7 7 9 7 d 5 1 6 ˆ      Ï<     Î<      ®Q2    mõ	ZÖÓ          !   L < 9 a a d a 1 a 6 c d c b 8 0 5 0 8 8 6 c 4 e b 4 f 1 3 4 d 6 7 7 9 7 d 5 1 6 ˆ      Ï<     Î<     (¯Q2    mõ	ZÖÓ €        !   L < 9 a a d a 1 a 6 c d c b 8 0 5 0 8 8 6 c 4 e b 4 f 1 3 4 d 6 7 7 9 7 d 5 1 6 @      Ğ<     µ<     °¯Q2    ûl
ZÖÓ               < d f                 @      Ğ<     µ<      °Q2    ûl
ZÖÓ  €            < d f @      Ğ<     µ<     @°Q2    —“
ZÖÓ €              < d f @      Ğ<     µ<     €°Q2    —“
ZÖÓ € €            < d f ˆ      Ñ<     Ğ<     À°Q2    ÔÌ
ZÖÓ           !   L < 3 2 0 1 6 d 7 e 2 7 8 b 9 0 0 c f b a 7 9 d b 7 8 9 9 f 3 f d c a e 1 4 e 5 ˆ      Ñ<     Ğ<     H±Q2    ÔÌ
ZÖÓ          !   L < 3 2 0 1 6 d 7 e 2 7 8 b 9 0 0 c f b a 7 9 d b 7 8 9 9 f 3 f d c a e 1 4 e 5 ˆ      Ñ<     Ğ<     Ğ±Q2    –â
ZÖÓ          !   L < 3 2 0 1 6 d 7 e 2 7 8 b 9 0 0 c f b a 7 9 d b 7 8 9 9 f 3 f d c a e 1 4 e 5 ˆ      Ñ<     Ğ<     X²Q2    –â
ZÖÓ          !   L < 3 2 0 1 6 d 7 e 2 7 8 b 9 0 0 c f b a 7 9 d b 7 8 9 9 f 3 f d c a e 1 4 e 5 ˆ      Ñ<     Ğ<     à²Q2    –â
ZÖÓ €        !   L < 3 2 0 1 6 d 7 e 2 7 8 b 9 0 0 c f b a 7 9 d b 7 8 9 9 f 3 f d c a e 1 4 e 5 @      Ò<     µ<     h³Q2    ×CZÖÓ               < e 2 @      Ò<     µ<     ¨³Q2    ×CZÖÓ  €            < e 2 @      Ò<     µ<     è³Q2    „kZÖÓ €              < e 2 @      Ò<     µ<     (´Q2    „kZÖÓ € €            < e 2 ˆ      Ó<     Ò<     h´Q2    ¾¥ZÖÓ           !   L < 0 3 a 0 4 5 1 c 5 c 0 0 a a d f b e e c d 5 e 6 8 1 9 0 3 7 f f 3 e 8 7 6 3 ˆ      Ó<     Ò<     ğ´Q2    ¾¥ZÖÓ          !   L < 0 3 a 0 4 5 1 c 5 c 0 0 a a d f b e e c d 5 e 6 8 1 9 0 3 7 f f 3 e 8 7 6 3 ˆ      Ó<     Ò<     xµQ2    ¾¥ZÖÓ          !   L < 0 3 a 0 4 5 1 c 5 c 0 0 a a d f b e e c d 5 e 6 8 1 9 0 3 7 f f 3 e 8 7 6 3 ˆ      Ó<     Ò<      ¶Q2    Ô·ZÖÓ          !   L < 0 3 a 0 4 5 1 c 5 c 0 0 a a d f b e e c d 5 e 6 8 1 9 0 3 7 f f 3 e 8 7 6 3 ˆ      Ó<     Ò<     ˆ¶Q2    Ô·ZÖÓ €        !   L < 0 3 a 0 4 5 1 c 5 c 0 0 a a d f b e e c d 5 e 6 8 1 9 0 3 7 f f 3 e 8 7 6 3 @      Ô<     µ<     ·Q2    QZÖÓ               < e 3 @      Ô<     µ<     P·Q2    QZÖÓ  €            < e 3 @      Ô<     µ<     ·Q2    I.ZÖÓ €              < e 3 @      Ô<     µ<     Ğ·Q2    I.ZÖÓ € €            < e 3 ˆ      Õ<     Ô<     ¸Q2    }ZÖÓ           !   L < 5 c 5 a 1 9 4 2 0 1 7 5 b 1 1 1 b e 6 f b a 5 3 7 6 5 d f e 8 f 9 d c e 0 c ˆ      Õ<     Ô<     ˜¸Q2    }ZÖÓ          !   L < 5 c 5 a 1 9 4 2 0 1 7 5 b 1 1 1 b e 6 f b a 5 3 7 6 5 d f e 8 f 9 d c e 0 c ˆ      Õ<     Ô<      ¹Q2    }ZÖÓ          !   L < 5 c 5 a 1 9 4 2 0 1 7 5 b 1 1 1 b e 6 f b a 5 3 7 6 5 d f e 8 f 9 d c e 0 c ˆ      Õ<     Ô<     ¨¹Q2    }ZÖÓ          !   L < 5 c 5 a 1 9 4 2 0 1 7 5 b 1 1 1 b e 6 f b a 5 3 7 6 5 d f e 8 f 9 d c e 0 c ˆ      Õ<     Ô<     0ºQ2    }ZÖÓ €        !   L < 5 c 5 a 1 9 4 2 0 1 7 5 b 1 1 1 b e 6 f b a 5 3 7 6 5 d f e 8 f 9 d c e 0 c @      Ö<     µ<     ¸ºQ2    &ñZÖÓ               < e b @      Ö<     µ<     øºQ2    &ñZÖÓ  €            < e b @      Ö<     µ<     8»Q2    ZÖÓ €              < e b @      Ö<     µ<     x»Q2    \ZÖÓ € €            < e b ˆ      ×<     Ö<     ¸»Q2    7zZÖÓ           !   L < 9 2 6 c f c 5 a c e 7 f f 5 f 8 8 4 1 1 e 2 3 c d e b 6 9 4 b 4 7 a 5 9 e 3 ˆ      ×<     Ö<     @¼Q2    7zZÖÓ          !   L < 9 2 6 c f c 5 a c e 7 f f 5 f 8 8 4 1 1 e 2 3 c d e b 6 9 4 b 4 7 a 5 9 e 3 ˆ      ×<     Ö<     È¼Q2    ÔZÖÓ          !   L < 9 2 6 c f c 5 a c e 7 f f 5 f 8 8 4 1 1 e 2 3 c d e b 6 9 4 b 4 7 a 5 9 e 3 ˆ      ×<     Ö<     P½Q2    ÔZÖÓ          !   L < 9 2 6 c f c 5 a c e 7 f f 5 f 8 8 4 1 1 e 2 3 c d e b 6 9 4 b 4 7 a 5 9 e 3 ˆ      ×<     Ö<     Ø½Q2    ÔZÖÓ €        !   L < 9 2 6 c f c 5 a c e 7 f f 5 f 8 8 4 1 1 e 2 3 c d e b 6 9 4 b 4 7 a 5 9 e 3 H      Ø<     µ<     `¾Q2    ©İZÖÓ               < i n f o     H      Ø<     µ<     ¨¾Q2    ñZÖÓ  €            < i n f o     H      Ø<     µ<     ğ¾Q2    ´ZÖÓ €              < i n f o     H      Ø<     µ<     8¿Q2    ´ZÖÓ € €            < i n f o     H      Ù<     µ<     €¿Q2    g>ZÖÓ               < p a c k                                                             H      Ù<     µ<      ÀQ2    g>ZÖÓ  €            < p a c k     H      Ù<     µ<     HÀQ2    ²QZÖÓ €              < p a c k     H      Ù<     µ<     ÀQ2    ?eZÖÓ € €            < p a c k            Ú<     Ù<     ØÀQ2    ÍZÖÓ           !   b < p a c k - 4 3 d 5 4 5 9 8 c d 8 2 8 9 2 e 5 f 4 8 6 d 3 7 3 9 e 3 9 f 0 5 d 7 7 4 f 1 c 3 . i d x          Ú<     Ù<     xÁQ2    ÍZÖÓ          !   b < p a c k - 4 3 d 5 4 5 9 8 c d 8 2 8 9 2 e 5 f 4 8 6 d 3 7 3 9 e 3 9 f 0 5 d 7 7 4 f 1 c 3 . i d x          Ú<     Ù<     ÂQ2    Ù9ZÖÓ          !   b < p a c k - 4 3 d 5 4 5 9 8 c d 8 2 8 9 2 e 5 f 4 8 6 d 3 7 3 9 e 3 9 f 0 5 d 7 7 4 f 1 c 3 . i d x          Ú<     Ù<     ¸ÂQ2    Ù9ZÖÓ          !   b < p a c k - 4 3 d 5 4 5 9 8 c d 8 2 8 9 2 e 5 f 4 8 6 d 3 7 3 9 e 3 9 f 0 5 d 7 7 4 f 1 c 3 . i d x          Ú<     Ù<     XÃQ2    Ù9ZÖÓ €        !   b < p a c k - 4 3 d 5 4 5 9 8 c d 8 2 8 9 2 e 5 f 4 8 6 d 3 7 3 9 e 3 9 f 0 5 d 7 7 4 f 1 c 3 . i d x          Û<     Ù<     øÃQ2    ‰ZÖÓ           !   d < p a c k - 4 3 d 5 4 5 9 8 c d 8 2 8 9 2 e 5 f 4 8 6 d 3 7 3 9 e 3 9 f 0 5 d 7 7 4 f 1 c 3 . p a c k        Û<     Ù<     ˜ÄQ2    ‰ZÖÓ          !   d < p a c k - 4 3 d 5 4 5 9 8 c d 8 2 8 9 2 e 5 f 4 8 6 d 3 7 3 9 e 3 9 f 0 5 d 7 7 4 f 1 c 3 . p a c k        Û<     Ù<     8ÅQ2    ÿ>ZÖÓ          !   d < p a c k - 4 3 d 5 4 5 9 8 c d 8 2 8 9 2 e 5 f 4 8 6 d 3 7 3 9 e 3 9 f 0 5 d 7 7 4 f 1 c 3 . p a c k        Û<     Ù<     ØÅQ2    ¿æ‚ZÖÓ          !   d < p a c k - 4 3 d 5 4 5 9 8 c d 8 2 8 9 2 e 5 f 4 8 6 d 3 7 3 9 e 3 9 f 0 5 d 7 7 4 f 1 c 3 . p a c k        Û<     Ù<     xÆQ2    ¿æ‚ZÖÓ €        !   d < p a c k - 4 3 d 5 4 5 9 8 c d 8 2 8 9 2 e 5 f 4 8 6 d 3 7 3 9 e 3 9 f 0 5 d 7 7 4 f 1 c 3 . p a c k H      Ü<     •<     ÇQ2    uƒƒZÖÓ               < r e f s     H      Ü<     •<     `ÇQ2    uƒƒZÖÓ  €            < r e f s     H      Ü<     •<     ¨ÇQ2    —ƒZÖÓ €              < r e f s     H      Ü<     •<     ğÇQ2    —ƒZÖÓ € €            < r e f s     H      İ<     Ü<     8ÈQ2    íøƒZÖÓ              
 < h e a d s   H      İ<     Ü<     €ÈQ2    íøƒZÖÓ  €           
 < h e a d s   H      İ<     Ü<     ÈÈQ2    ƒ„ZÖÓ €             
 < h e a d s   H      İ<     Ü<     ÉQ2    – „ZÖÓ € €           
 < h e a d s   H      Ş<     İ<     XÉQ2    ³¼„ZÖÓ                < m a s t e r H      Ş<     İ<      ÉQ2    ³¼„ZÖÓ               < m a s t e r H      Ş<     İ<     èÉQ2    ³¼„ZÖÓ               < m a s t e r H      Ş<     İ<     0ÊQ2    ³¼„ZÖÓ               < m a s t e r H      Ş<     İ<     xÊQ2    ³¼„ZÖÓ €             < m a s t e r P      ß<     Ü<     ÀÊQ2    ¥…ZÖÓ               < r e m o t e s       P      ß<     Ü<     ËQ2    /2…ZÖÓ  €            < r e m o t e s       P      ß<     Ü<     `ËQ2    ÄE…ZÖÓ €              < r e m o t e s       P      ß<     Ü<     °ËQ2    ÄE…ZÖÓ € €            < r e m o t e s       H      à<     ß<      ÌQ2    €…ZÖÓ               < o r i g i n H      à<     ß<     HÌQ2    €…ZÖÓ  €            < o r i g i n H      à<     ß<     ÌQ2    &”…ZÖÓ €              < o r i g i n H      à<     ß<     ØÌQ2    &”…ZÖÓ € €            < o r i g i n H      á<     à<      ÍQ2    íÎ…ZÖÓ                < H E A D     H      á<     à<     hÍQ2    íÎ…ZÖÓ               < H E A D     H      á<     à<     °ÍQ2    Í×…ZÖÓ               < H E A D     H      á<     à<     øÍQ2    Í×…ZÖÓ               < H E A D     H      á<     à<     @ÎQ2    Í×…ZÖÓ €             < H E A D     H      â<     à<     ˆÎQ2    ­	†ZÖÓ                < m a s t e r H      â<     à<     ĞÎQ2    ­	†ZÖÓ               < m a s t e r H      â<     à<     ÏQ2    ­	†ZÖÓ               < m a s t e r H      â<     à<     `ÏQ2    ­	†ZÖÓ               < m a s t e r H      â<     à<     ¨ÏQ2    ­	†ZÖÓ €             < m a s t e r                 H      ã<     Ü<      ĞQ2    ÿW†ZÖÓ               < t a g s     H      ã<     Ü<     HĞQ2    ÿW†ZÖÓ  €            < t a g s     H      ã<     Ü<     ĞQ2    ˆk†ZÖÓ €              < t a g s     H      ã<     Ü<     ØĞQ2    ˆk†ZÖÓ € €            < t a g s     H      ä<     Ø       ÑQ2    sÍ†ZÖÓ               < . v s     N H      ä<     Ø      hÑQ2    sÍ†ZÖÓ  €            < . v s     N H      ä<     Ø      °ÑQ2    sÍ†ZÖÓ €              < . v s     N H      ä<     Ø      øÑQ2    sÍ†ZÖÓ € €            < . v s     N P      å<     ä<     @ÒQ2    Ì‡ZÖÓ               < D 3 D D e m o       P      å<     ä<     ÒQ2    Ì‡ZÖÓ  €            < D 3 D D e m o       P      å<     ä<     àÒQ2    ^/‡ZÖÓ €              < D 3 D D e m o       P      å<     ä<     0ÓQ2    ^/‡ZÖÓ € €            < D 3 D D e m o       H      æ<     å<     €ÓQ2    "j‡ZÖÓ               < v 1 5       H      æ<     å<     ÈÓQ2    "j‡ZÖÓ  €            < v 1 5       H      æ<     å<     ÔQ2    ³}‡ZÖÓ €              < v 1 5       H      æ<     å<     XÔQ2    ³}‡ZÖÓ € €            < v 1 5       H      ç<     æ<      ÔQ2    â¤‡ZÖÓ           "    < . s u o     H      ç<     æ<     èÔQ2    â¤‡ZÖÓ          "    < . s u o     H      ç<     æ<     0ÕQ2    â¤‡ZÖÓ          "    < . s u o     H      ç<     æ<     xÕQ2    ÜˆZÖÓ          "    < . s u o     H      ç<     æ<     ÀÕQ2    ÜˆZÖÓ €        "    < . s u o     X      è<     æ<     ÖQ2    ÷-ˆZÖÓ                < B r o w s e . V C . d b     X      è<     æ<     `ÖQ2    ÷-ˆZÖÓ               < B r o w s e . V C . d b     X      è<     æ<     ¸ÖQ2    I(ŒZÖÓ               < B r o w s e . V C . d b     X      è<     æ<     ×Q2     6[ÖÓ               < B r o w s e . V C . d b     X      è<     æ<     h×Q2    86[ÖÓ €             < B r o w s e . V C . d b     H      é<     æ<     À×Q2    Ò6[ÖÓ               < i p c h     H      é<     æ<     ØQ2    Ôå6[ÖÓ  €            < i p c h     H      é<     æ<     PØQ2    —7[ÖÓ €              < i p c h     H      é<     æ<     ˜ØQ2    $7[ÖÓ € €            < i p c h     P      ê<     é<     àØQ2    E8[ÖÓ               < A u t o P C H       P      ê<     é<     0ÙQ2    X8[ÖÓ  €            < A u t o P C H       P      ê<     é<     €ÙQ2    °8[ÖÓ €              < A u t o P C H       P      ê<     é<     ĞÙQ2    6“8[ÖÓ € €            < A u t o P C H       `      ë<     ê<      ÚQ2    ,E9[ÖÓ                < 1 8 9 a 2 8 d 6 3 a c 3 f 4 c 5     `      ë<     ê<     €ÚQ2    ,E9[ÖÓ  €             < 1 8 9 a 2 8 d 6 3 a c 3 f 4 c 5     `      ë<     ê<     àÚQ2    6~9[ÖÓ €               < 1 8 9 a 2 8 d 6 3 a c 3 f 4 c 5     `      ë<     ê<     @ÛQ2    ëŒ9[ÖÓ € €             < 1 8 9 a 2 8 d 6 3 a c 3 f 4 c 5     P      ì<     ë<      ÛQ2    **=[ÖÓ                < S C E N E . i p c h P      ì<     ë<     ğÛQ2    Ï==[ÖÓ               < S C E N E . i p c h P      ì<     ë<     @ÜQ2    ¥6I[ÖÓ               < S C E N E . i p c h P      ì<     ë<     ÜQ2    á\ÖÓ               < S C E N E . i p c h P      ì<     ë<     àÜQ2    /\ÖÓ €             < S C E N E . i p c h `      í<     ê<     0İQ2    a§\ÖÓ               < 1 a 1 f b 3 b a b 3 d 2 0 3 1     0 `      í<     ê<     İQ2    a§\ÖÓ  €            < 1 a 1 f b 3 b a b 3 d 2 0 3 1     0 `      í<     ê<     ğİQ2    ùş\ÖÓ €              < 1 a 1 f b 3 b a b 3 d 2 0 3 1     0 `      í<     ê<     PŞQ2    ùş\ÖÓ € €            < 1 a 1 f b 3 b a b 3 d 2 0 3 1     0 X      î<     í<     °ŞQ2    ®Æ\ÖÓ                < V E C T O R 3 . i p c h     X      î<     í<     ßQ2    ï\ÖÓ               < V E C T O R 3 . i p c h     X      î<     í<     `ßQ2    X…\ÖÓ               < V E C T O R 3 . i p c h                                                                             X      î<     í<      àQ2    SjC\ÖÓ               < V E C T O R 3 . i p c h     X      î<     í<     XàQ2    “C\ÖÓ €             < V E C T O R 3 . i p c h     `      ï<     ê<     °àQ2    qD\ÖÓ                < 1 b 6 6 7 9 6 4 f e d 6 0 e c c     `      ï<     ê<     áQ2    qD\ÖÓ  €             < 1 b 6 6 7 9 6 4 f e d 6 0 e c c     `      ï<     ê<     páQ2    ƒjD\ÖÓ €               < 1 b 6 6 7 9 6 4 f e d 6 0 e c c     `      ï<     ê<     ĞáQ2    ƒjD\ÖÓ € €             < 1 b 6 6 7 9 6 4 f e d 6 0 e c c     X      ğ<     ï<     0âQ2    ÇìD\ÖÓ                < T E X T U R E . i p c h     X      ğ<     ï<     ˆâQ2    ÇìD\ÖÓ               < T E X T U R E . i p c h     X      ğ<     ï<     àâQ2    mƒN\ÖÓ               < T E X T U R E . i p c h     X      ğ<     ï<     8ãQ2    /N·\ÖÓ               < T E X T U R E . i p c h     X      ğ<     ï<     ãQ2    Ru·\ÖÓ €             < T E X T U R E . i p c h     `      ñ<     ê<     èãQ2    *ì·\ÖÓ                < 2 a 8 2 6 f 1 5 8 6 f 2 d 5 2 0     `      ñ<     ê<     HäQ2    *ì·\ÖÓ  €             < 2 a 8 2 6 f 1 5 8 6 f 2 d 5 2 0     `      ñ<     ê<     ¨äQ2    ñ¸\ÖÓ €               < 2 a 8 2 6 f 1 5 8 6 f 2 d 5 2 0     `      ñ<     ê<     åQ2    ñ¸\ÖÓ € €             < 2 a 8 2 6 f 1 5 8 6 f 2 d 5 2 0     `      ò<     ñ<     håQ2    1‹¸\ÖÓ               $ < G U I _ H I E R A R C H Y . i p c h `      ò<     ñ<     ÈåQ2    1‹¸\ÖÓ              $ < G U I _ H I E R A R C H Y . i p c h `      ò<     ñ<     (æQ2    &¶¿\ÖÓ              $ < G U I _ H I E R A R C H Y . i p c h `      ò<     ñ<     ˆæQ2    ŞQy]ÖÓ              $ < G U I _ H I E R A R C H Y . i p c h `      ò<     ñ<     èæQ2    ŞQy]ÖÓ €            $ < G U I _ H I E R A R C H Y . i p c h `      ó<     ê<     HçQ2    |]ÖÓ                < 4 0 6 4 5 6 3 c 9 2 e d 9 9 3 8     `      ó<     ê<     ¨çQ2    |]ÖÓ  €             < 4 0 6 4 5 6 3 c 9 2 e d 9 9 3 8     `      ó<     ê<     èQ2    C|]ÖÓ €               < 4 0 6 4 5 6 3 c 9 2 e d 9 9 3 8     `      ó<     ê<     hèQ2    C|]ÖÓ € €             < 4 0 6 4 5 6 3 c 9 2 e d 9 9 3 8     P      ô<     ó<     ÈèQ2    s}]ÖÓ                < M E S H . i p c h   P      ô<     ó<     éQ2    s}]ÖÓ               < M E S H . i p c h   P      ô<     ó<     héQ2    ç±ƒ]ÖÓ               < M E S H . i p c h   P      ô<     ó<     ¸éQ2    ó¬^ÖÓ               < M E S H . i p c h   P      ô<     ó<     êQ2    ó¬^ÖÓ €             < M E S H . i p c h   `      õ<     ê<     XêQ2    ¢w^ÖÓ                < 4 2 5 3 9 9 e 7 d 5 5 6 f e 1 4     `      õ<     ê<     ¸êQ2    ¢w^ÖÓ  €             < 4 2 5 3 9 9 e 7 d 5 5 6 f e 1 4     `      õ<     ê<     ëQ2    ÷œ^ÖÓ €               < 4 2 5 3 9 9 e 7 d 5 5 6 f e 1 4     `      õ<     ê<     xëQ2    ûÅ^ÖÓ € €             < 4 2 5 3 9 9 e 7 d 5 5 6 f e 1 4     `      ö<     õ<     ØëQ2    =¯^ÖÓ               $ < G U I _ I N S P E C T O R . i p c h `      ö<     õ<     8ìQ2    =¯^ÖÓ              $ < G U I _ I N S P E C T O R . i p c h `      ö<     õ<     ˜ìQ2    øq^ÖÓ              $ < G U I _ I N S P E C T O R . i p c h `      ö<     õ<     øìQ2    <Ìğ^ÖÓ              $ < G U I _ I N S P E C T O R . i p c h `      ö<     õ<     XíQ2    <Ìğ^ÖÓ €            $ < G U I _ I N S P E C T O R . i p c h `      ÷<     ê<     ¸íQ2    ŒKñ^ÖÓ                < 4 3 9 c 4 e e 8 2 b 6 f 0 3 c 0     `      ÷<     ê<     îQ2    ŒKñ^ÖÓ  €             < 4 3 9 c 4 e e 8 2 b 6 f 0 3 c 0     `      ÷<     ê<     xîQ2    Lîñ^ÖÓ €               < 4 3 9 c 4 e e 8 2 b 6 f 0 3 c 0     `      ÷<     ê<     ØîQ2    Lò^ÖÓ € €             < 4 3 9 c 4 e e 8 2 b 6 f 0 3 c 0     X      ø<     ÷<     8ïQ2    Ãò^ÖÓ                < M A T E R I A L . i p c h   X      ø<     ÷<     ïQ2    6êò^ÖÓ               < M A T E R I A L . i p c h                           X      ø<     ÷<      ğQ2    æú^ÖÓ               < M A T E R I A L . i p c h   X      ø<     ÷<     XğQ2    uî…_ÖÓ               < M A T E R I A L . i p c h   X      ø<     ÷<     °ğQ2    Ï†_ÖÓ €             < M A T E R I A L . i p c h   `      ù<     ê<     ñQ2    £Ü†_ÖÓ                < 6 3 2 0 2 0 f 2 0 9 c 0 a d 9 c     `      ù<     ê<     hñQ2    £Ü†_ÖÓ  €             < 6 3 2 0 2 0 f 2 0 9 c 0 a d 9 c     `      ù<     ê<     ÈñQ2    €2‡_ÖÓ €               < 6 3 2 0 2 0 f 2 0 9 c 0 a d 9 c     `      ù<     ê<     (òQ2    ŞY‡_ÖÓ € €             < 6 3 2 0 2 0 f 2 0 9 c 0 a d 9 c     X      ú<     ù<     ˆòQ2    I´‡_ÖÓ                < C O M P O N E N T . i p c h X      ú<     ù<     àòQ2    I´‡_ÖÓ               < C O M P O N E N T . i p c h X      ú<     ù<     8óQ2    «*’_ÖÓ               < C O M P O N E N T . i p c h X      ú<     ù<     óQ2    æ›*`ÖÓ               < C O M P O N E N T . i p c h X      ú<     ù<     èóQ2    1Ã*`ÖÓ €             < C O M P O N E N T . i p c h `      û<     ê<     @ôQ2    „s+`ÖÓ                < 6 b 9 a 6 1 6 b 7 0 0 3 1 6 3 c     `      û<     ê<      ôQ2    „s+`ÖÓ  €             < 6 b 9 a 6 1 6 b 7 0 0 3 1 6 3 c     `      û<     ê<      õQ2    îÁ+`ÖÓ €               < 6 b 9 a 6 1 6 b 7 0 0 3 1 6 3 c     `      û<     ê<     `õQ2    ,é+`ÖÓ € €             < 6 b 9 a 6 1 6 b 7 0 0 3 1 6 3 c     X      ü<     û<     ÀõQ2    ·…,`ÖÓ                < I M G U I Z M O . i p c h   X      ü<     û<     öQ2    Ş¬,`ÖÓ               < I M G U I Z M O . i p c h   X      ü<     û<     pöQ2    äş1`ÖÓ               < I M G U I Z M O . i p c h   X      ü<     û<     ÈöQ2    )dA`ÖÓ               < I M G U I Z M O . i p c h   X      ü<     û<      ÷Q2    T‹A`ÖÓ €             < I M G U I Z M O . i p c h   `      ı<     ê<     x÷Q2    ®îA`ÖÓ                < 7 4 e b 9 9 7 7 e 2 c 8 d 5 2 6     `      ı<     ê<     Ø÷Q2    i_B`ÖÓ  €             < 7 4 e b 9 9 7 7 e 2 c 8 d 5 2 6     `      ı<     ê<     8øQ2    i_B`ÖÓ €               < 7 4 e b 9 9 7 7 e 2 c 8 d 5 2 6     `      ı<     ê<     ˜øQ2    «†B`ÖÓ € €             < 7 4 e b 9 9 7 7 e 2 c 8 d 5 2 6     `      ş<     ı<     øøQ2    ¹KC`ÖÓ                < C O R E E N G I N E . i p c h       `      ş<     ı<     XùQ2    ¹KC`ÖÓ               < C O R E E N G I N E . i p c h       `      ş<     ı<     ¸ùQ2    “J`ÖÓ               < C O R E E N G I N E . i p c h       `      ş<     ı<     úQ2    hWaÖÓ               < C O R E E N G I N E . i p c h       `      ş<     ı<     xúQ2    hWaÖÓ €             < C O R E E N G I N E . i p c h       `      ÿ<     ê<     ØúQ2    fhaÖÓ                < 7 a b d e e 4 9 d 3 5 3 6 3 3 0     `      ÿ<     ê<     8ûQ2    fhaÖÓ  €             < 7 a b d e e 4 9 d 3 5 3 6 3 3 0     `      ÿ<     ê<     ˜ûQ2    ‘aÖÓ €               < 7 a b d e e 4 9 d 3 5 3 6 3 3 0     `      ÿ<     ê<     øûQ2    ‘aÖÓ € €             < 7 a b d e e 4 9 d 3 5 3 6 3 3 0     `       =     ÿ<     XüQ2    6aÖÓ                < G A M E O B J E C T . i p c h     8}`       =     ÿ<     ¸üQ2    6aÖÓ               < G A M E O B J E C T . i p c h     8}`       =     ÿ<     ıQ2    ÿ¶$aÖÓ               < G A M E O B J E C T . i p c h     8}`       =     ÿ<     xıQ2    ò›¶aÖÓ               < G A M E O B J E C T . i p c h     8}`       =     ÿ<     ØıQ2    ò›¶aÖÓ €             < G A M E O B J E C T . i p c h     8}`      =     ê<     8şQ2    Ë·aÖÓ                < 7 b 3 e a d 4 e 4 a 5 d c a a 4     `      =     ê<     ˜şQ2    Ë·aÖÓ  €             < 7 b 3 e a d 4 e 4 a 5 d c a a 4     `      =     ê<     øşQ2    #ò·aÖÓ €               < 7 b 3 e a d 4 e 4 a 5 d c a a 4     `      =     ê<     XÿQ2    ¦¸aÖÓ € €             < 7 b 3 e a d 4 e 4 a 5 d c a a 4                                                                             P      =     =       R2    •i¸aÖÓ                < I M G U I . i p c h P      =     =     P R2    •i¸aÖÓ               < I M G U I . i p c h P      =     =       R2    ØÖ½aÖÓ               < I M G U I . i p c h P      =     =     ğ R2    scÍaÖÓ               < I M G U I . i p c h P      =     =     @R2    scÍaÖÓ €             < I M G U I . i p c h `      =     ê<     R2    ²ÚÍaÖÓ                < 8 a f f 8 2 e 0 d 1 b 0 4 3 e 3     `      =     ê<     ğR2    ²ÚÍaÖÓ  €             < 8 a f f 8 2 e 0 d 1 b 0 4 3 e 3     `      =     ê<     PR2    ÜÎaÖÓ €               < 8 a f f 8 2 e 0 d 1 b 0 4 3 e 3     `      =     ê<     °R2    ÜÎaÖÓ € €             < 8 a f f 8 2 e 0 d 1 b 0 4 3 e 3     x      =     =     R2    &PÎaÖÓ               8 < G I Z M O T R A N S F O R M R E N D E R D X 9 . i p c h     x      =     =     ˆR2    TwÎaÖÓ              8 < G I Z M O T R A N S F O R M R E N D E R D X 9 . i p c h     x      =     =      R2    æSÓaÖÓ              8 < G I Z M O T R A N S F O R M R E N D E R D X 9 . i p c h     x      =     =     xR2    Š?HbÖÓ              8 < G I Z M O T R A N S F O R M R E N D E R D X 9 . i p c h     x      =     =     ğR2    jfHbÖÓ €            8 < G I Z M O T R A N S F O R M R E N D E R D X 9 . i p c h     `      =     ê<     hR2    >2IbÖÓ                < 8 b 5 5 0 8 d 3 5 9 b 7 7 7 1 2     `      =     ê<     ÈR2    >2IbÖÓ  €             < 8 b 5 5 0 8 d 3 5 9 b 7 7 7 1 2     `      =     ê<     (R2    iZIbÖÓ €               < 8 b 5 5 0 8 d 3 5 9 b 7 7 7 1 2     `      =     ê<     ˆR2    iZIbÖÓ € €             < 8 b 5 5 0 8 d 3 5 9 b 7 7 7 1 2     `      =     =     èR2    "mJbÖÓ                < T I N Y - G I Z M O . i p c h     5}`      =     =     HR2    "mJbÖÓ               < T I N Y - G I Z M O . i p c h     5}`      =     =     ¨R2    eìMbÖÓ               < T I N Y - G I Z M O . i p c h     5}`      =     =     R2    p¬ŠbÖÓ               < T I N Y - G I Z M O . i p c h     5}`      =     =     hR2    šÓŠbÖÓ €             < T I N Y - G I Z M O . i p c h     5}`      =     ê<     ÈR2    ®¯‹bÖÓ                < 9 3 2 a 0 1 2 6 e 2 4 1 9 7 a b     `      =     ê<     (	R2    áÖ‹bÖÓ  €             < 9 3 2 a 0 1 2 6 e 2 4 1 9 7 a b     `      =     ê<     ˆ	R2    áÖ‹bÖÓ €               < 9 3 2 a 0 1 2 6 e 2 4 1 9 7 a b     `      =     ê<     è	R2    ş‹bÖÓ € €             < 9 3 2 a 0 1 2 6 e 2 4 1 9 7 a b     `      =     =     H
R2    “ÃŒbÖÓ               " < G U I _ V I E W P O R T . i p c h   `      =     =     ¨
R2    “ÃŒbÖÓ              " < G U I _ V I E W P O R T . i p c h   `      =     =     R2    Î3”bÖÓ              " < G U I _ V I E W P O R T . i p c h   `      =     =     hR2    ;dNcÖÓ              " < G U I _ V I E W P O R T . i p c h   `      =     =     ÈR2    ;dNcÖÓ €            " < G U I _ V I E W P O R T . i p c h   `      	=     ê<     (R2    ŸZOcÖÓ                < a 8 6 e 0 3 2 b 6 8 6 8 4 a 2 f     `      	=     ê<     ˆR2    ŸZOcÖÓ  €             < a 8 6 e 0 3 2 b 6 8 6 8 4 a 2 f     `      	=     ê<     èR2    ^€OcÖÓ €               < a 8 6 e 0 3 2 b 6 8 6 8 4 a 2 f     `      	=     ê<     HR2    ^€OcÖÓ € €             < a 8 6 e 0 3 2 b 6 8 6 8 4 a 2 f     X      
=     	=     ¨R2    ÑGRcÖÓ                < T R A N S F O R M . i p c h X      
=     	=      R2    ÑGRcÖÓ               < T R A N S F O R M . i p c h X      
=     	=     XR2    K]YcÖÓ               < T R A N S F O R M . i p c h X      
=     	=     °R2    )›cÖÓ               < T R A N S F O R M . i p c h X      
=     	=     R2    ïO›cÖÓ €             < T R A N S F O R M . i p c h `      =     ê<     `R2    Â×œcÖÓ                < c 7 7 8 c a 3 a 4 6 8 6 7 6 2 5                                                                     `      =     ê<      R2    Â×œcÖÓ  €             < c 7 7 8 c a 3 a 4 6 8 6 7 6 2 5     `      =     ê<     `R2    ÙşœcÖÓ €               < c 7 7 8 c a 3 a 4 6 8 6 7 6 2 5     `      =     ê<     ÀR2    ¢ÂcÖÓ € €             < c 7 7 8 c a 3 a 4 6 8 6 7 6 2 5     `      =     =      R2    8cÖÓ               " < I N P U T H A N D L E R . i p c h   `      =     =     €R2    8cÖÓ              " < I N P U T H A N D L E R . i p c h   `      =     =     àR2    …ü¦cÖÓ              " < I N P U T H A N D L E R . i p c h   `      =     =     @R2    †ãdÖÓ              " < I N P U T H A N D L E R . i p c h   `      =     =      R2    ±
dÖÓ €            " < I N P U T H A N D L E R . i p c h   `      =     ê<      R2    jÜdÖÓ                < d 1 4 3 6 2 7 9 c 7 1 e f c 4 0     `      =     ê<     `R2    jÜdÖÓ  €             < d 1 4 3 6 2 7 9 c 7 1 e f c 4 0     `      =     ê<     ÀR2    udÖÓ €               < d 1 4 3 6 2 7 9 c 7 1 e f c 4 0     `      =     ê<      R2    udÖÓ € €             < d 1 4 3 6 2 7 9 c 7 1 e f c 4 0     `      =     =     €R2    RdÖÓ               " < A S S E T M A N A G E R . i p c h   `      =     =     àR2    RdÖÓ              " < A S S E T M A N A G E R . i p c h   `      =     =     @R2    PD'dÖÓ              " < A S S E T M A N A G E R . i p c h   `      =     =      R2    GeÖÓ              " < A S S E T M A N A G E R . i p c h   `      =     =      R2    ZaeÖÓ €            " < A S S E T M A N A G E R . i p c h   `      =     ê<     `R2    ¥yeÖÓ                < f 0 a 6 b f 0 c 3 c 1 5 9 f c 4     `      =     ê<     ÀR2    ¥yeÖÓ  €             < f 0 a 6 b f 0 c 3 c 1 5 9 f c 4     `      =     ê<      R2    ¥ eÖÓ €               < f 0 a 6 b f 0 c 3 c 1 5 9 f c 4     `      =     ê<     €R2    ÛÇeÖÓ € €             < f 0 a 6 b f 0 c 3 c 1 5 9 f c 4     P      =     =     àR2    K—eÖÓ                < A E S . i p c h     P      =     =     0R2    K—eÖÓ               < A E S . i p c h     P      =     =     €R2    ÔõeÖÓ               < A E S . i p c h     P      =     =     ĞR2    ¦GeÖÓ               < A E S . i p c h     P      =     =      R2    ¦GeÖÓ €             < A E S . i p c h     `      =     ê<     pR2    eÖÓ                < f 2 6 7 3 2 7 a 5 6 7 8 6 3 4 8     `      =     ê<     ĞR2    ?;eÖÓ  €             < f 2 6 7 3 2 7 a 5 6 7 8 6 3 4 8     `      =     ê<     0R2    ?;eÖÓ €               < f 2 6 7 3 2 7 a 5 6 7 8 6 3 4 8     `      =     ê<     R2    ?;eÖÓ € €             < f 2 6 7 3 2 7 a 5 6 7 8 6 3 4 8     h      =     =     ğR2    Â¯eÖÓ               ( < I M G U I _ I M P L _ D X 1 1 . i p c h     h      =     =     XR2    Â¯eÖÓ              ( < I M G U I _ I M P L _ D X 1 1 . i p c h     h      =     =     ÀR2    Ö$eÖÓ              ( < I M G U I _ I M P L _ D X 1 1 . i p c h     h      =     =     (R2    éeÖÓ              ( < I M G U I _ I M P L _ D X 1 1 . i p c h     h      =     =     R2    Ë€eÖÓ €            ( < I M G U I _ I M P L _ D X 1 1 . i p c h     `      =     ê<     øR2    OpeÖÓ                < f 2 f 3 d 1 d f 1 e 4 d f c 8 3     `      =     ê<     XR2    OpeÖÓ  €             < f 2 f 3 d 1 d f 1 e 4 d f c 8 3     `      =     ê<     ¸R2    r—eÖÓ €               < f 2 f 3 d 1 d f 1 e 4 d f c 8 3     `      =     ê<     R2    r—eÖÓ € €             < f 2 f 3 d 1 d f 1 e 4 d f c 8 3     `      =     =     xR2    ÏåeÖÓ                < I M G U I _ D R A W . i p c h     F}`      =     =     ØR2    ÏåeÖÓ               < I M G U I _ D R A W . i p c h     F}`      =     =     8R2    Fº…eÖÓ               < I M G U I _ D R A W . i p c h     F}`      =     =     ˜R2     ™eÖÓ               < I M G U I _ D R A W . i p c h     F}        `      =     =       R2     ™eÖÓ €             < I M G U I _ D R A W . i p c h     F}`      =     ê<     ` R2    ¥á™eÖÓ                < f 6 0 c a b 5 8 a 0 c 1 0 c e 2     `      =     ê<     À R2    ¥á™eÖÓ  €             < f 6 0 c a b 5 8 a 0 c 1 0 c e 2     `      =     ê<      !R2    pšeÖÓ €               < f 6 0 c a b 5 8 a 0 c 1 0 c e 2     `      =     ê<     €!R2    pšeÖÓ € €             < f 6 0 c a b 5 8 a 0 c 1 0 c e 2     X      =     =     à!R2    =WšeÖÓ                < C A M E R A . i p c h       X      =     =     8"R2    X~šeÖÓ               < C A M E R A . i p c h       X      =     =     "R2    :6eÖÓ               < C A M E R A . i p c h       X      =     =     è"R2    vfÖÓ               < C A M E R A . i p c h       X      =     =     @#R2    vfÖÓ €             < C A M E R A . i p c h       `      =     ê<     ˜#R2    º­vfÖÓ                < f f 0 5 7 9 7 2 1 a 6 a 7 b 3 8     `      =     ê<     ø#R2    º­vfÖÓ  €             < f f 0 5 7 9 7 2 1 a 6 a 7 b 3 8     `      =     ê<     X$R2    ,:wfÖÓ €               < f f 0 5 7 9 7 2 1 a 6 a 7 b 3 8     `      =     ê<     ¸$R2    ,:wfÖÓ € €             < f f 0 5 7 9 7 2 1 a 6 a 7 b 3 8     X      =     =     %R2    ÈwfÖÓ                < V E C T O R 2 . i p c h     X      =     =     p%R2    XxfÖÓ               < V E C T O R 2 . i p c h     X      =     =     È%R2    ·€fÖÓ               < V E C T O R 2 . i p c h     X      =     =      &R2    ø£fÖÓ               < V E C T O R 2 . i p c h     X      =     =     x&R2    ø£fÖÓ €             < V E C T O R 2 . i p c h     H      =     Ø      Ğ&R2    Vï¦fÖÓ               < A s s e t s H      =     Ø      'R2    Vï¦fÖÓ  €            < A s s e t s H      =     Ø      `'R2    Vï¦fÖÓ €              < A s s e t s H      =     Ø      ¨'R2    Vï¦fÖÓ € €            < A s s e t s h      =     =     ğ'R2    6¨fÖÓ               , < B m p M A P T E S T 1 0 0 x 1 0 0 2 . b m p h      =     =     X(R2    6¨fÖÓ              , < B m p M A P T E S T 1 0 0 x 1 0 0 2 . b m p h      =     =     À(R2    ªfÖÓ              , < B m p M A P T E S T 1 0 0 x 1 0 0 2 . b m p h      =     =     ()R2    ªfÖÓ              , < B m p M A P T E S T 1 0 0 x 1 0 0 2 . b m p h      =     =     )R2    {$ªfÖÓ €            , < B m p M A P T E S T 1 0 0 x 1 0 0 2 . b m p X      =     =     ø)R2    ÉJ«fÖÓ                < b r i c k w o r k . j p g   X      =     =     P*R2    ÉJ«fÖÓ               < b r i c k w o r k . j p g   X      =     =     ¨*R2    ÉJ«fÖÓ               < b r i c k w o r k . j p g   X      =     =      +R2    qŒ®fÖÓ               < b r i c k w o r k . j p g   X      =     =     X+R2    <¯fÖÓ €             < b r i c k w o r k . j p g   P      =     =     °+R2    àÌ¯fÖÓ                < C u b e . o b j     P      =     =      ,R2    ô¯fÖÓ               < C u b e . o b j     P      =     =     P,R2    ±fÖÓ               < C u b e . o b j     P      =     =      ,R2    ±fÖÓ               < C u b e . o b j     P      =     =     ğ,R2    A-±fÖÓ €             < C u b e . o b j     X      =     =     @-R2    ïÉ±fÖÓ                < c u b e t r i . o b j     d X      =     =     ˜-R2    ïÉ±fÖÓ               < c u b e t r i . o b j     d X      =     =     ğ-R2    µ²fÖÓ               < c u b e t r i . o b j     d X      =     =     H.R2    µ²fÖÓ               < c u b e t r i . o b j     d X      =     =      .R2    µ²fÖÓ €             < c u b e t r i . o b j     d `      =     =     ø.R2    Üx³fÖÓ               " < d o c k _ s t a n d a r d . a u r   `      =     =     X/R2    Üx³fÖÓ              " < d o c k _ s t a n d a r d . a u r                                                                           `      =     =      0R2    od´fÖÓ              " < d o c k _ s t a n d a r d . a u r   `      =     =     `0R2    od´fÖÓ              " < d o c k _ s t a n d a r d . a u r   `      =     =     À0R2    od´fÖÓ €            " < d o c k _ s t a n d a r d . a u r   P      =     =      1R2    ¿µfÖÓ                < k u d . p n g       P      =     =     p1R2    ¿µfÖÓ               < k u d . p n g       P      =     =     À1R2    j¶fÖÓ               < k u d . p n g       P      =     =     2R2    h¿¹fÖÓ               < k u d . p n g       P      =     =     `2R2    h¿¹fÖÓ €             < k u d . p n g       `       =     =     °2R2    h¿¹fÖÓ                < N o r m a l B r i c k . p n g       `       =     =     3R2    h¿¹fÖÓ               < N o r m a l B r i c k . p n g       `       =     =     p3R2    ²Î¼fÖÓ               < N o r m a l B r i c k . p n g       `       =     =     Ğ3R2    S¾fÖÓ               < N o r m a l B r i c k . p n g       `       =     =     04R2    S¾fÖÓ €             < N o r m a l B r i c k . p n g       P      !=     =     4R2    ˆğ¿fÖÓ                < p b r . j p g       P      !=     =     à4R2    ˆğ¿fÖÓ               < p b r . j p g       P      !=     =     05R2    PQÂfÖÓ               < p b r . j p g       P      !=     =     €5R2    I=ÃfÖÓ               < p b r . j p g       P      !=     =     Ğ5R2    I=ÃfÖÓ €             < p b r . j p g       P      "=     =      6R2    [øÃfÖÓ                < p b r a . p n g     P      "=     =     p6R2    [øÃfÖÓ               < p b r a . p n g     P      "=     =     À6R2    ŞÄfÖÓ               < p b r a . p n g     P      "=     =     7R2    ÖÇfÖÓ               < p b r a . p n g     P      "=     =     `7R2    ÖÇfÖÓ €             < p b r a . p n g     P      #=     =     °7R2    %ÈfÖÓ                < p b r a o . p n g   P      #=     =      8R2    %ÈfÖÓ               < p b r a o . p n g   P      #=     =     P8R2    şMÊfÖÓ               < p b r a o . p n g   P      #=     =      8R2    9mÎfÖÓ               < p b r a o . p n g   P      #=     =     ğ8R2    9mÎfÖÓ €             < p b r a o . p n g   P      $=     =     @9R2    [ÏfÖÓ                < p b r n . p n g     P      $=     =     9R2    [ÏfÖÓ               < p b r n . p n g     P      $=     =     à9R2    HKĞfÖÓ               < p b r n . p n g     P      $=     =     0:R2    …ÒfÖÓ               < p b r n . p n g     P      $=     =     €:R2    …ÒfÖÓ €             < p b r n . p n g     P      %=     =     Ğ:R2    GÀÒfÖÓ                < S T S P . o b j     P      %=     =      ;R2    GÀÒfÖÓ               < S T S P . o b j     P      %=     =     p;R2    Ú,ÕfÖÓ               < S T S P . o b j     P      %=     =     À;R2    V×fÖÓ               < S T S P . o b j     P      %=     =     <R2    V×fÖÓ €             < S T S P . o b j     x      &=     =     `<R2    Õ°×fÖÓ               : < S T S P _ S h a d o w T e a m _ B a s e C o l o r . p n g   x      &=     =     Ø<R2    Õ°×fÖÓ              : < S T S P _ S h a d o w T e a m _ B a s e C o l o r . p n g   x      &=     =     P=R2    >MÚfÖÓ              : < S T S P _ S h a d o w T e a m _ B a s e C o l o r . p n g   x      &=     =     È=R2    ŞHÜfÖÓ              : < S T S P _ S h a d o w T e a m _ B a s e C o l o r . p n g   x      &=     =     @>R2    ŞHÜfÖÓ €            : < S T S P _ S h a d o w T e a m _ B a s e C o l o r . p n g   p      '=     =     ¸>R2    ı'ŞfÖÓ               4 < S T S P _ S h a d o w T e a m _ N o r m a l . p n g p      '=     =     (?R2    ı'ŞfÖÓ              4 < S T S P _ S h a d o w T e a m _ N o r m a l . p n g                                                                                                         p      '=     =      @R2    åàfÖÓ              4 < S T S P _ S h a d o w T e a m _ N o r m a l . p n g p      '=     =     p@R2    òÄæfÖÓ              4 < S T S P _ S h a d o w T e a m _ N o r m a l . p n g p      '=     =     à@R2    ìæfÖÓ €            4 < S T S P _ S h a d o w T e a m _ N o r m a l . p n g ˜      (=     =     PAR2    OçfÖÓ               \ < S T S P _ S h a d o w T e a m _ O c c l u s i o n R o u g h n e s s M e t a l l i c . p n g ˜      (=     =     èAR2    OçfÖÓ              \ < S T S P _ S h a d o w T e a m _ O c c l u s i o n R o u g h n e s s M e t a l l i c . p n g ˜      (=     =     €BR2    ÒùèfÖÓ              \ < S T S P _ S h a d o w T e a m _ O c c l u s i o n R o u g h n e s s M e t a l l i c . p n g ˜      (=     =     CR2    HîfÖÓ              \ < S T S P _ S h a d o w T e a m _ O c c l u s i o n R o u g h n e s s M e t a l l i c . p n g ˜      (=     =     °CR2    HîfÖÓ €            \ < S T S P _ S h a d o w T e a m _ O c c l u s i o n R o u g h n e s s M e t a l l i c . p n g P      )=     =     HDR2    ¬4ğfÖÓ                < t e m p . o b j     P      )=     =     ˜DR2    Ë[ğfÖÓ               < t e m p . o b j     P      )=     =     èDR2    {xğfÖÓ               < t e m p . o b j     P      )=     =     8ER2    {xğfÖÓ               < t e m p . o b j     P      )=     =     ˆER2    {xğfÖÓ €             < t e m p . o b j     H      *=     =     ØER2    ŒñfÖÓ               < E d i t o r H      *=     =      FR2    ŒñfÖÓ  €            < E d i t o r H      *=     =     hFR2    §³ñfÖÓ €              < E d i t o r H      *=     =     °FR2    §³ñfÖÓ € €            < E d i t o r X      +=     *=     øFR2    $lòfÖÓ                < E y e - O r b i t . p n g   X      +=     *=     PGR2    $lòfÖÓ               < E y e - O r b i t . p n g   X      +=     *=     ¨GR2    ©/õfÖÓ               < E y e - O r b i t . p n g   X      +=     *=      HR2    ©/õfÖÓ               < E y e - O r b i t . p n g   X      +=     *=     XHR2    4EõfÖÓ €             < E y e - O r b i t . p n g   H      ,=     =     °HR2    QçõfÖÓ              
 < F o n t s   H      ,=     =     øHR2    QçõfÖÓ  €           
 < F o n t s   H      ,=     =     @IR2    ƒIöfÖÓ €             
 < F o n t s   H      ,=     =     ˆIR2    ¦pöfÖÓ € €           
 < F o n t s   p      -=     ,=     ĞIR2     ÷fÖÓ               . < L i b e r a t i o n S a n s - B o l d . t t f       p      -=     ,=     @JR2     ÷fÖÓ              . < L i b e r a t i o n S a n s - B o l d . t t f       p      -=     ,=     °JR2    ñùfÖÓ              . < L i b e r a t i o n S a n s - B o l d . t t f       p      -=     ,=      KR2    HÛúfÖÓ              . < L i b e r a t i o n S a n s - B o l d . t t f       p      -=     ,=     KR2    _ûfÖÓ €            . < L i b e r a t i o n S a n s - B o l d . t t f       x      .=     ,=      LR2    ;FüfÖÓ               : < L i b e r a t i o n S a n s - B o l d I t a l i c . t t f   x      .=     ,=     xLR2    ;FüfÖÓ              : < L i b e r a t i o n S a n s - B o l d I t a l i c . t t f   x      .=     ,=     ğLR2    »)ıfÖÓ              : < L i b e r a t i o n S a n s - B o l d I t a l i c . t t f   x      .=     ,=     hMR2    &äıfÖÓ              : < L i b e r a t i o n S a n s - B o l d I t a l i c . t t f   x      .=     ,=     àMR2    şfÖÓ €            : < L i b e r a t i o n S a n s - B o l d I t a l i c . t t f   p      /=     ,=     XNR2    ú›şfÖÓ               2 < L i b e r a t i o n S a n s - I t a l i c . t t f   p      /=     ,=     ÈNR2    ú›şfÖÓ              2 < L i b e r a t i o n S a n s - I t a l i c . t t f   p      /=     ,=     8OR2    %ÃşfÖÓ              2 < L i b e r a t i o n S a n s - I t a l i c . t t f                                                                                           p      /=     ,=      PR2    BœÿfÖÓ              2 < L i b e r a t i o n S a n s - I t a l i c . t t f   p      /=     ,=     pPR2    qÃÿfÖÓ €            2 < L i b e r a t i o n S a n s - I t a l i c . t t f   p      0=     ,=     àPR2    PWgÖÓ               4 < L i b e r a t i o n S a n s - R e g u l a r . t t f p      0=     ,=     PQR2    c~gÖÓ              4 < L i b e r a t i o n S a n s - R e g u l a r . t t f p      0=     ,=     ÀQR2    š¥gÖÓ              4 < L i b e r a t i o n S a n s - R e g u l a r . t t f p      0=     ,=     0RR2    d3gÖÓ              4 < L i b e r a t i o n S a n s - R e g u l a r . t t f p      0=     ,=      RR2    d3gÖÓ €            4 < L i b e r a t i o n S a n s - R e g u l a r . t t f H      1=     Ø      SR2    PgÖÓ              
 < D e b u g   H      1=     Ø      XSR2    PgÖÓ  €           
 < D e b u g   H      1=     Ø       SR2    _gÖÓ €             
 < D e b u g   H      1=     Ø      èSR2    …gÖÓ € €           
 < D e b u g   P      2=     1=     0TR2    ¢$gÖÓ                < a e s . o b j       P      2=     1=     €TR2    ¢$gÖÓ               < a e s . o b j       P      2=     1=     ĞTR2    Ì‡	gÖÓ               < a e s . o b j       P      2=     1=      UR2    Ì‡	gÖÓ               < a e s . o b j       P      2=     1=     pUR2    Û®	gÖÓ €             < a e s . o b j       `      3=     1=     ÀUR2    _
gÖÓ                 < A s s e t M a n a g e r . o b j     `      3=     1=      VR2    _
gÖÓ                < A s s e t M a n a g e r . o b j     `      3=     1=     €VR2    òªgÖÓ                < A s s e t M a n a g e r . o b j     `      3=     1=     àVR2    !`gÖÓ                < A s s e t M a n a g e r . o b j     `      3=     1=     @WR2    !`gÖÓ €              < A s s e t M a n a g e r . o b j     P      4=     1=      WR2    Ó¦gÖÓ                < C a m e r a . o b j P      4=     1=     ğWR2    Ó¦gÖÓ               < C a m e r a . o b j P      4=     1=     @XR2     ©gÖÓ               < C a m e r a . o b j P      4=     1=     XR2    ×?gÖÓ               < C a m e r a . o b j P      4=     1=     àXR2    ×?gÖÓ €             < C a m e r a . o b j X      5=     1=     0YR2    )gÖÓ                < C o m p o n e n t . o b j   X      5=     1=     ˆYR2    )gÖÓ               < C o m p o n e n t . o b j   X      5=     1=     àYR2    ‘ÕgÖÓ               < C o m p o n e n t . o b j   X      5=     1=     8ZR2    gEgÖÓ               < C o m p o n e n t . o b j   X      5=     1=     ZR2    gEgÖÓ €             < C o m p o n e n t . o b j   X      6=     1=     èZR2    6“gÖÓ                < C o r e E n g i n e . o b j X      6=     1=     @[R2    6“gÖÓ               < C o r e E n g i n e . o b j X      6=     1=     ˜[R2    4ÍgÖÓ               < C o r e E n g i n e . o b j X      6=     1=     ğ[R2    WËgÖÓ               < C o r e E n g i n e . o b j X      6=     1=     H\R2    WËgÖÓ €             < C o r e E n g i n e . o b j P      7=     1=      \R2    fµgÖÓ                < D e b u g . o b j   P      7=     1=     ğ\R2    fµgÖÓ               < D e b u g . o b j   P      7=     1=     @]R2    Ü\#gÖÓ               < D e b u g . o b j   P      7=     1=     ]R2    Í…#gÖÓ               < D e b u g . o b j   P      7=     1=     à]R2    Í…#gÖÓ €             < D e b u g . o b j   P      8=     1=     0^R2    Ôñ#gÖÓ                < D e m o . e x e     P      8=     1=     €^R2    Ôñ#gÖÓ               < D e m o . e x e     P      8=     1=     Ğ^R2    Úm'gÖÓ               < D e m o . e x e     P      8=     1=      _R2    +µ)gÖÓ               < D e m o . e x e     P      8=     1=     p_R2    +µ)gÖÓ €             < D e m o . e x e                                                                     P      9=     1=      `R2    «**gÖÓ                < D e m o . i l k     P      9=     1=     P`R2    «**gÖÓ               < D e m o . i l k     P      9=     1=      `R2    ×,gÖÓ               < D e m o . i l k     P      9=     1=     ğ`R2    q¢4gÖÓ               < D e m o . i l k     P      9=     1=     @aR2    q¢4gÖÓ €             < D e m o . i l k     P      :=     1=     aR2    /5gÖÓ                < D e m o . l o g     P      :=     1=     àaR2    /5gÖÓ               < D e m o . l o g     P      :=     1=     0bR2    ˜ 6gÖÓ               < D e m o . l o g     P      :=     1=     €bR2    ˜ 6gÖÓ               < D e m o . l o g     P      :=     1=     ĞbR2    ˜ 6gÖÓ €             < D e m o . l o g     P      ;=     1=      cR2    İ7gÖÓ                < D e m o . p d b     P      ;=     1=     pcR2    İ7gÖÓ               < D e m o . p d b     P      ;=     1=     ÀcR2    WÁ>gÖÓ               < D e m o . p d b     P      ;=     1=     dR2    0ÉSgÖÓ               < D e m o . p d b     P      ;=     1=     `dR2    0ÉSgÖÓ €             < D e m o . p d b     P      <=     1=     °dR2    —7TgÖÓ                < E d i t o r . o b j P      <=     1=      eR2    —7TgÖÓ               < E d i t o r . o b j P      <=     1=     PeR2    é­UgÖÓ               < E d i t o r . o b j P      <=     1=      eR2    ¢zVgÖÓ               < E d i t o r . o b j P      <=     1=     ğeR2    ¢zVgÖÓ €             < E d i t o r . o b j h      ==     1=     @fR2    .;WgÖÓ               ( < E d i t o r M o v e S c r i p t . o b j     h      ==     1=     ¨fR2    .;WgÖÓ              ( < E d i t o r M o v e S c r i p t . o b j     h      ==     1=     gR2    3YgÖÓ              ( < E d i t o r M o v e S c r i p t . o b j     h      ==     1=     xgR2    iµYgÖÓ              ( < E d i t o r M o v e S c r i p t . o b j     h      ==     1=     àgR2    iµYgÖÓ €            ( < E d i t o r M o v e S c r i p t . o b j     x      >=     1=     HhR2    áWZgÖÓ               < < E d i t o r S c e n e S e l e c t i o n S c r i p t . o b j x      >=     1=     ÀhR2    áWZgÖÓ              < < E d i t o r S c e n e S e l e c t i o n S c r i p t . o b j x      >=     1=     8iR2    y”\gÖÓ              < < E d i t o r S c e n e S e l e c t i o n S c r i p t . o b j x      >=     1=     °iR2    öG]gÖÓ              < < E d i t o r S c e n e S e l e c t i o n S c r i p t . o b j x      >=     1=     (jR2    öG]gÖÓ €            < < E d i t o r S c e n e S e l e c t i o n S c r i p t . o b j X      ?=     1=      jR2    Ì^gÖÓ                < F r a g m e n t . c s o     X      ?=     1=     øjR2    Ì^gÖÓ               < F r a g m e n t . c s o     X      ?=     1=     PkR2    "Ò_gÖÓ               < F r a g m e n t . c s o     X      ?=     1=     ¨kR2    "Ò_gÖÓ               < F r a g m e n t . c s o     X      ?=     1=      lR2    "Ò_gÖÓ €             < F r a g m e n t . c s o     X      @=     1=     XlR2    \`gÖÓ                < G a m e O b j e c t . o b j X      @=     1=     °lR2    \`gÖÓ               < G a m e O b j e c t . o b j X      @=     1=     mR2    ÷cdgÖÓ               < G a m e O b j e c t . o b j X      @=     1=     `mR2    (}fgÖÓ               < G a m e O b j e c t . o b j X      @=     1=     ¸mR2    (}fgÖÓ €             < G a m e O b j e c t . o b j p      A=     1=     nR2    ĞhgÖÓ               0 < G i z m o T r a n s f o r m R e n d e r . o b j     p      A=     1=     €nR2    ĞhgÖÓ              0 < G i z m o T r a n s f o r m R e n d e r . o b j     p      A=     1=     ğnR2    èÕkgÖÓ              0 < G i z m o T r a n s f o r m R e n d e r . o b j     p      A=     1=     `oR2    #ıkgÖÓ              0 < G i z m o T r a n s f o r m R e n d e r . o b j                                                     p      A=     1=      pR2    #ıkgÖÓ €            0 < G i z m o T r a n s f o r m R e n d e r . o b j     P      B=     1=     ppR2    lgÖÓ                < G U I . o b j       P      B=     1=     ÀpR2    lgÖÓ               < G U I . o b j       P      B=     1=     qR2    tpgÖÓ               < G U I . o b j       P      B=     1=     `qR2    9ØpgÖÓ               < G U I . o b j       P      B=     1=     °qR2    9ØpgÖÓ €             < G U I . o b j       `      C=     1=      rR2    ƒ&qgÖÓ                < G U I _ C o n s o l e . o b j       `      C=     1=     `rR2    ƒ&qgÖÓ               < G U I _ C o n s o l e . o b j       `      C=     1=     ÀrR2    ½«rgÖÓ               < G U I _ C o n s o l e . o b j       `      C=     1=      sR2    »usgÖÓ               < G U I _ C o n s o l e . o b j       `      C=     1=     €sR2    »usgÖÓ €             < G U I _ C o n s o l e . o b j       `      D=     1=     àsR2    itgÖÓ               " < G U I _ H i e r a r c h y . o b j   `      D=     1=     @tR2    itgÖÓ              " < G U I _ H i e r a r c h y . o b j   `      D=     1=      tR2    ÖhugÖÓ              " < G U I _ H i e r a r c h y . o b j   `      D=     1=      uR2    ¿,vgÖÓ              " < G U I _ H i e r a r c h y . o b j   `      D=     1=     `uR2    ¿,vgÖÓ €            " < G U I _ H i e r a r c h y . o b j   `      E=     1=     ÀuR2    ZÉvgÖÓ               " < G U I _ I n s p e c t o r . o b j   `      E=     1=      vR2    ZÉvgÖÓ              " < G U I _ I n s p e c t o r . o b j   `      E=     1=     €vR2    fwgÖÓ              " < G U I _ I n s p e c t o r . o b j   `      E=     1=     àvR2    õPxgÖÓ              " < G U I _ I n s p e c t o r . o b j   `      E=     1=     @wR2    xxgÖÓ €            " < G U I _ I n s p e c t o r . o b j   `      F=     1=      wR2    ”íxgÖÓ                < G U I _ M e n u B a r . o b j     3 `      F=     1=      xR2    ÃygÖÓ               < G U I _ M e n u B a r . o b j     3 `      F=     1=     `xR2    &(zgÖÓ               < G U I _ M e n u B a r . o b j     3 `      F=     1=     ÀxR2    ÃzgÖÓ               < G U I _ M e n u B a r . o b j     3 `      F=     1=      yR2    «êzgÖÓ €             < G U I _ M e n u B a r . o b j     3 `      G=     1=     €yR2    36}gÖÓ                 < G U I _ V i e w p o r t . o b j     `      G=     1=     àyR2    36}gÖÓ                < G U I _ V i e w p o r t . o b j     `      G=     1=     @zR2    _A}gÖÓ                < G U I _ V i e w p o r t . o b j     `      G=     1=      zR2    ~gÖÓ                < G U I _ V i e w p o r t . o b j     `      G=     1=      {R2    ~gÖÓ €              < G U I _ V i e w p o r t . o b j     P      H=     1=     `{R2    ÛZ~gÖÓ                < i m g u i . o b j   P      H=     1=     °{R2    ÛZ~gÖÓ               < i m g u i . o b j   P      H=     1=      |R2    J+gÖÓ               < i m g u i . o b j   P      H=     1=     P|R2    øVƒgÖÓ               < i m g u i . o b j   P      H=     1=      |R2    øVƒgÖÓ €             < i m g u i . o b j   X      I=     1=     ğ|R2    µ6…gÖÓ                < I m G u i z m o . o b j     X      I=     1=     H}R2    ã]…gÖÓ               < I m G u i z m o . o b j     X      I=     1=      }R2    º‹gÖÓ               < I m G u i z m o . o b j     X      I=     1=     ø}R2    ‘ü‹gÖÓ               < I m G u i z m o . o b j     X      I=     1=     P~R2    Û
ŒgÖÓ €             < I m G u i z m o . o b j     X      J=     1=     ¨~R2    w€ŒgÖÓ                < i m g u i _ d e m o . o b j X      J=     1=      R2    Á§ŒgÖÓ               < i m g u i _ d e m o . o b j X      J=     1=     XR2    …•gÖÓ               < i m g u i _ d e m o . o b j                                                                                 X      J=     1=      €R2    ‹ZgÖÓ               < i m g u i _ d e m o . o b j X      J=     1=     X€R2    †gÖÓ €             < i m g u i _ d e m o . o b j X      K=     1=     °€R2    ıgÖÓ                < i m g u i _ d o c k . o b j X      K=     1=     R2    °$gÖÓ               < i m g u i _ d o c k . o b j X      K=     1=     `R2    Lr‘gÖÓ               < i m g u i _ d o c k . o b j X      K=     1=     ¸R2    “gÖÓ               < i m g u i _ d o c k . o b j X      K=     1=     ‚R2    D8“gÖÓ €             < i m g u i _ d o c k . o b j X      L=     1=     h‚R2    Í‡“gÖÓ                < i m g u i _ d r a w . o b j X      L=     1=     À‚R2    Í‡“gÖÓ               < i m g u i _ d r a w . o b j X      L=     1=     ƒR2    J§”gÖÓ               < i m g u i _ d r a w . o b j X      L=     1=     pƒR2    B•gÖÓ               < i m g u i _ d r a w . o b j X      L=     1=     ÈƒR2    B•gÖÓ €             < i m g u i _ d r a w . o b j h      M=     1=      „R2    W•gÖÓ               & < i m g u i _ i m p l _ d x 1 1 . o b j     u h      M=     1=     ˆ„R2    W•gÖÓ              & < i m g u i _ i m p l _ d x 1 1 . o b j     u h      M=     1=     ğ„R2    Z—gÖÓ              & < i m g u i _ i m p l _ d x 1 1 . o b j     u h      M=     1=     X…R2    Õ?™gÖÓ              & < i m g u i _ i m p l _ d x 1 1 . o b j     u h      M=     1=     À…R2    Õ?™gÖÓ €            & < i m g u i _ i m p l _ d x 1 1 . o b j     u `      N=     1=     (†R2    ;¼™gÖÓ                < I m S e q u e n c e r . o b j       `      N=     1=     ˆ†R2    ;¼™gÖÓ               < I m S e q u e n c e r . o b j       `      N=     1=     è†R2    •'œgÖÓ               < I m S e q u e n c e r . o b j       `      N=     1=     H‡R2    •'œgÖÓ               < I m S e q u e n c e r . o b j       `      N=     1=     ¨‡R2    •'œgÖÓ €             < I m S e q u e n c e r . o b j       `      O=     1=     ˆR2    DÄœgÖÓ                 < I n p u t H a n d l e r . o b j     `      O=     1=     hˆR2    sëœgÖÓ                < I n p u t H a n d l e r . o b j     `      O=     1=     ÈˆR2    )N gÖÓ                < I n p u t H a n d l e r . o b j     `      O=     1=     (‰R2    9u¡gÖÓ                < I n p u t H a n d l e r . o b j     `      O=     1=     ˆ‰R2    9u¡gÖÓ €              < I n p u t H a n d l e r . o b j     P      P=     1=     è‰R2    Y¢gÖÓ                < m a i n . o b j     P      P=     1=     8ŠR2    Y¢gÖÓ               < m a i n . o b j     P      P=     1=     ˆŠR2    lW¥gÖÓ               < m a i n . o b j     P      P=     1=     ØŠR2    :¦gÖÓ               < m a i n . o b j     P      P=     1=     (‹R2    h=¦gÖÓ €             < m a i n . o b j     X      Q=     1=     x‹R2    ²‹¦gÖÓ                < M a t e r i a l . o b j     X      Q=     1=     Ğ‹R2    ²‹¦gÖÓ               < M a t e r i a l . o b j     X      Q=     1=     (ŒR2    ¶©gÖÓ               < M a t e r i a l . o b j     X      Q=     1=     €ŒR2    @®gÖÓ               < M a t e r i a l . o b j     X      Q=     1=     ØŒR2    @®gÖÓ €             < M a t e r i a l . o b j     P      R=     1=     0R2    #Ç®gÖÓ                < M e s h . o b j     P      R=     1=     €R2    #Ç®gÖÓ               < M e s h . o b j     P      R=     1=     ĞR2    #Ç®gÖÓ               < M e s h . o b j     P      R=     1=      R2    ‡~³gÖÓ               < M e s h . o b j     P      R=     1=     pR2    ‡~³gÖÓ €             < M e s h . o b j     X      S=     1=     ÀR2    &´gÖÓ                < M e s h F i l t e r . o b j X      S=     1=     R2    &´gÖÓ               < M e s h F i l t e r . o b j X      S=     1=     pR2    ·gÖÓ               < M e s h F i l t e r . o b j                                                         X      S=     1=      R2    Ö~·gÖÓ               < M e s h F i l t e r . o b j X      S=     1=     XR2    Ö~·gÖÓ €             < M e s h F i l t e r . o b j X      T=     1=     °R2    ŠY¸gÖÓ                < P h y s i c s . o b j       X      T=     1=     ‘R2    ŠY¸gÖÓ               < P h y s i c s . o b j       X      T=     1=     `‘R2    ò»gÖÓ               < P h y s i c s . o b j       X      T=     1=     ¸‘R2    ½gÖÓ               < P h y s i c s . o b j       X      T=     1=     ’R2    ½gÖÓ €             < P h y s i c s . o b j       `      U=     1=     h’R2    bD½gÖÓ                 < Q u a d T r e e N o d e . o b j     `      U=     1=     È’R2    bD½gÖÓ                < Q u a d T r e e N o d e . o b j     `      U=     1=     (“R2    îŞÀgÖÓ                < Q u a d T r e e N o d e . o b j     `      U=     1=     ˆ“R2    ¦ÂgÖÓ                < Q u a d T r e e N o d e . o b j     `      U=     1=     è“R2    ¦ÂgÖÓ €              < Q u a d T r e e N o d e . o b j     `      V=     1=     H”R2    ÷jÂgÖÓ               " < R e n d e r M a n a g e r . o b j   `      V=     1=     ¨”R2    ÷jÂgÖÓ              " < R e n d e r M a n a g e r . o b j   `      V=     1=     •R2    ¶QÄgÖÓ              " < R e n d e r M a n a g e r . o b j   `      V=     1=     h•R2    VÈgÖÓ              " < R e n d e r M a n a g e r . o b j   `      V=     1=     È•R2    B}ÈgÖÓ €            " < R e n d e r M a n a g e r . o b j   P      W=     1=     (–R2    CÊgÖÓ                < S c e n e . o b j   P      W=     1=     x–R2    CÊgÖÓ               < S c e n e . o b j   P      W=     1=     È–R2    0ÅÍgÖÓ               < S c e n e . o b j   P      W=     1=     —R2    ã,ãgÖÓ               < S c e n e . o b j   P      W=     1=     h—R2    ã,ãgÖÓ €             < S c e n e . o b j   `      X=     1=     ¸—R2    `ägÖÓ               " < S h a d e r P r o g r a m . o b j   `      X=     1=     ˜R2    ]1ägÖÓ              " < S h a d e r P r o g r a m . o b j   `      X=     1=     x˜R2    ğVçgÖÓ              " < S h a d e r P r o g r a m . o b j   `      X=     1=     Ø˜R2    —©ëgÖÓ              " < S h a d e r P r o g r a m . o b j   `      X=     1=     8™R2    —©ëgÖÓ €            " < S h a d e r P r o g r a m . o b j   P      Y=     1=     ˜™R2    µìgÖÓ                < s t d a f x . o b j P      Y=     1=     è™R2    µìgÖÓ               < s t d a f x . o b j P      Y=     1=     8šR2    ÆûîgÖÓ               < s t d a f x . o b j P      Y=     1=     ˆšR2    ÆûîgÖÓ               < s t d a f x . o b j P      Y=     1=     ØšR2    é"ïgÖÓ €             < s t d a f x . o b j h      Z=     1=     (›R2    ıšïgÖÓ               ( < T e r r a i n G e n e r a t o r . o b j     h      Z=     1=     ›R2    ıšïgÖÓ              ( < T e r r a i n G e n e r a t o r . o b j     h      Z=     1=     ø›R2    hcñgÖÓ              ( < T e r r a i n G e n e r a t o r . o b j     h      Z=     1=     `œR2    JOògÖÓ              ( < T e r r a i n G e n e r a t o r . o b j     h      Z=     1=     ÈœR2    JOògÖÓ €            ( < T e r r a i n G e n e r a t o r . o b j     X      [=     1=     0R2    ‹ògÖÓ                < T e x t u r e . o b j       X      [=     1=     ˆR2    ‹ògÖÓ               < T e x t u r e . o b j       X      [=     1=     àR2    XôgÖÓ               < T e x t u r e . o b j       X      [=     1=     8R2    ¸,ögÖÓ               < T e x t u r e . o b j       X      [=     1=     R2    ¸,ögÖÓ €             < T e x t u r e . o b j       P      \=     1=     èR2    ßSögÖÓ                < T i m e . o b j     P      \=     1=     8ŸR2    ßSögÖÓ               < T i m e . o b j     P      \=     1=     ˆŸR2    D‹ùgÖÓ               < T i m e . o b j                                             P      \=     1=       R2    D‹ùgÖÓ               < T i m e . o b j     P      \=     1=     P R2    †±ùgÖÓ €             < T i m e . o b j     X      ]=     1=       R2    ª-ügÖÓ                < t i n y - g i z m o . o b j X      ]=     1=     ø R2    ª-ügÖÓ               < t i n y - g i z m o . o b j X      ]=     1=     P¡R2    Û–şgÖÓ               < t i n y - g i z m o . o b j X      ]=     1=     ¨¡R2    o£hÖÓ               < t i n y - g i z m o . o b j X      ]=     1=      ¢R2    o£hÖÓ €             < t i n y - g i z m o . o b j X      ^=     1=     X¢R2    @hÖÓ                < T r a n s f o r m . o b j   X      ^=     1=     °¢R2    @hÖÓ               < T r a n s f o r m . o b j   X      ^=     1=     £R2    hÖÓ               < T r a n s f o r m . o b j   X      ^=     1=     `£R2    Ì´hÖÓ               < T r a n s f o r m . o b j   X      ^=     1=     ¸£R2    Ì´hÖÓ €             < T r a n s f o r m . o b j   P      _=     1=     ¤R2    ¦(	hÖÓ                < v c 1 4 1 . i d b   P      _=     1=     `¤R2    ¦(	hÖÓ               < v c 1 4 1 . i d b   P      _=     1=     °¤R2    ¨QhÖÓ               < v c 1 4 1 . i d b   P      _=     1=      ¥R2    ø¡hÖÓ               < v c 1 4 1 . i d b   P      _=     1=     P¥R2    ø¡hÖÓ €             < v c 1 4 1 . i d b   P      `=     1=      ¥R2    §hÖÓ                < v c 1 4 1 . p d b   P      `=     1=     ğ¥R2    §hÖÓ               < v c 1 4 1 . p d b   P      `=     1=     @¦R2    ÍPhÖÓ               < v c 1 4 1 . p d b   P      `=     1=     ¦R2    Æo)hÖÓ               < v c 1 4 1 . p d b   P      `=     1=     à¦R2    Æo)hÖÓ €             < v c 1 4 1 . p d b   X      a=     1=     0§R2    #¾)hÖÓ                < V e c t o r 2 . o b j     °X      a=     1=     ˆ§R2    Jå)hÖÓ               < V e c t o r 2 . o b j     °X      a=     1=     à§R2    +*hÖÓ               < V e c t o r 2 . o b j     °X      a=     1=     8¨R2    õ,hÖÓ               < V e c t o r 2 . o b j     °X      a=     1=     ¨R2    4,hÖÓ €             < V e c t o r 2 . o b j     °X      b=     1=     è¨R2    ‰‚,hÖÓ                < V e c t o r 3 . o b j     ¹ŸX      b=     1=     @©R2    ‰‚,hÖÓ               < V e c t o r 3 . o b j     ¹ŸX      b=     1=     ˜©R2    –1hÖÓ               < V e c t o r 3 . o b j     ¹ŸX      b=     1=     ğ©R2    êÉ2hÖÓ               < V e c t o r 3 . o b j     ¹ŸX      b=     1=     HªR2    êÉ2hÖÓ €             < V e c t o r 3 . o b j     ¹ŸP      c=     1=      ªR2    mC3hÖÓ                < V e r t e x . c s o P      c=     1=     ğªR2    ”j3hÖÓ               < V e r t e x . c s o P      c=     1=     @«R2    ”j3hÖÓ               < V e r t e x . c s o P      c=     1=     «R2    ”j3hÖÓ               < V e r t e x . c s o P      c=     1=     à«R2    ”j3hÖÓ €             < V e r t e x . c s o X      d=     1=     0¬R2    _	4hÖÓ                < Z B a s e M a t h s . o b j X      d=     1=     ˆ¬R2    _	4hÖÓ               < Z B a s e M a t h s . o b j X      d=     1=     à¬R2    {â7hÖÓ               < Z B a s e M a t h s . o b j X      d=     1=     8­R2    }/8hÖÓ               < Z B a s e M a t h s . o b j X      d=     1=     ­R2    °V8hÖÓ €             < Z B a s e M a t h s . o b j X      e=     1=     è­R2    €Î8hÖÓ                < Z M a t h s F u n c . o b j X      e=     1=     @®R2    €Î8hÖÓ               < Z M a t h s F u n c . o b j X      e=     1=     ˜®R2    “Ğ:hÖÓ               < Z M a t h s F u n c . o b j X      e=     1=     ğ®R2    “Ğ:hÖÓ               < Z M a t h s F u n c . o b j X      e=     1=     H¯R2    “Ğ:hÖÓ €             < Z M a t h s F u n c . o b j P      f=     1=      ¯R2    õs;hÖÓ               < D e m o . t l o g                   P      f=     1=      °R2    õs;hÖÓ  €            < D e m o . t l o g   P      f=     1=     P°R2    ×Æ;hÖÓ €              < D e m o . t l o g   P      f=     1=      °R2    ê
<hÖÓ € €            < D e m o . t l o g   `      g=     f=     ğ°R2    x2=hÖÓ               " < C L . c o m m a n d . 1 . t l o g   `      g=     f=     P±R2    Ê|=hÖÓ              " < C L . c o m m a n d . 1 . t l o g   `      g=     f=     °±R2    ¥BhÖÓ              " < C L . c o m m a n d . 1 . t l o g   `      g=     f=     ²R2    ¥BhÖÓ              " < C L . c o m m a n d . 1 . t l o g   `      g=     f=     p²R2    ¥BhÖÓ €            " < C L . c o m m a n d . 1 . t l o g   X      h=     f=     Ğ²R2    ÔBhÖÓ                < C L . r e a d . 1 . t l o g X      h=     f=     (³R2    ÔBhÖÓ               < C L . r e a d . 1 . t l o g X      h=     f=     €³R2    €:HhÖÓ               < C L . r e a d . 1 . t l o g X      h=     f=     Ø³R2     ™MhÖÓ               < C L . r e a d . 1 . t l o g X      h=     f=     0´R2    ÒÀMhÖÓ €             < C L . r e a d . 1 . t l o g `      i=     f=     ˆ´R2    öˆNhÖÓ                < C L . w r i t e . 1 . t l o g       `      i=     f=     è´R2    öˆNhÖÓ               < C L . w r i t e . 1 . t l o g       `      i=     f=     HµR2    ÒUhÖÓ               < C L . w r i t e . 1 . t l o g       `      i=     f=     ¨µR2    6VhÖÓ               < C L . w r i t e . 1 . t l o g       `      i=     f=     ¶R2    6VhÖÓ €             < C L . w r i t e . 1 . t l o g       h      j=     f=     h¶R2    Õ–VhÖÓ               & < D e m o . l a s t b u i l d s t a t e     u h      j=     f=     Ğ¶R2    \íVhÖÓ              & < D e m o . l a s t b u i l d s t a t e     u h      j=     f=     8·R2    \íVhÖÓ              & < D e m o . l a s t b u i l d s t a t e     u h      j=     f=      ·R2    \íVhÖÓ              & < D e m o . l a s t b u i l d s t a t e     u h      j=     f=     ¸R2    ‡WhÖÓ €            & < D e m o . l a s t b u i l d s t a t e     u `      k=     f=     p¸R2    ì”WhÖÓ               $ < f x c . c o m m a n d . 1 . t l o g `      k=     f=     Ğ¸R2    ì”WhÖÓ              $ < f x c . c o m m a n d . 1 . t l o g `      k=     f=     0¹R2    ì”WhÖÓ              $ < f x c . c o m m a n d . 1 . t l o g `      k=     f=     ¹R2    ì”WhÖÓ              $ < f x c . c o m m a n d . 1 . t l o g `      k=     f=     ğ¹R2    ì”WhÖÓ €            $ < f x c . c o m m a n d . 1 . t l o g `      l=     f=     PºR2    úYhÖÓ                < f x c . r e a d . 1 . t l o g     D `      l=     f=     °ºR2    úYhÖÓ               < f x c . r e a d . 1 . t l o g     D `      l=     f=     »R2    T!ZhÖÓ               < f x c . r e a d . 1 . t l o g     D `      l=     f=     p»R2    T!ZhÖÓ               < f x c . r e a d . 1 . t l o g     D `      l=     f=     Ğ»R2    T!ZhÖÓ €             < f x c . r e a d . 1 . t l o g     D `      m=     f=     0¼R2    ÎòZhÖÓ                 < f x c . w r i t e . 1 . t l o g     `      m=     f=     ¼R2    ÎòZhÖÓ                < f x c . w r i t e . 1 . t l o g     `      m=     f=     ğ¼R2    ÎòZhÖÓ                < f x c . w r i t e . 1 . t l o g     `      m=     f=     P½R2    ı[hÖÓ                < f x c . w r i t e . 1 . t l o g     `      m=     f=     °½R2    ı[hÖÓ €              < f x c . w r i t e . 1 . t l o g     h      n=     f=     ¾R2    ë·[hÖÓ               & < l i n k . c o m m a n d . 1 . t l o g     u h      n=     f=     x¾R2    ë·[hÖÓ              & < l i n k . c o m m a n d . 1 . t l o g     u h      n=     f=     à¾R2    R“]hÖÓ              & < l i n k . c o m m a n d . 1 . t l o g     u h      n=     f=     H¿R2    R“]hÖÓ              & < l i n k . c o m m a n d . 1 . t l o g     u                                                                                 h      n=     f=      ÀR2    R“]hÖÓ €            & < l i n k . c o m m a n d . 1 . t l o g     u `      o=     f=     hÀR2    å^hÖÓ                 < l i n k . r e a d . 1 . t l o g     `      o=     f=     ÈÀR2    Ì¥^hÖÓ                < l i n k . r e a d . 1 . t l o g     `      o=     f=     (ÁR2    Wd`hÖÓ                < l i n k . r e a d . 1 . t l o g     `      o=     f=     ˆÁR2    Îˆ`hÖÓ                < l i n k . r e a d . 1 . t l o g     `      o=     f=     èÁR2    Îˆ`hÖÓ €              < l i n k . r e a d . 1 . t l o g     `      p=     f=     HÂR2    ı@ahÖÓ               " < l i n k . w r i t e . 1 . t l o g   `      p=     f=     ¨ÂR2    ı@ahÖÓ              " < l i n k . w r i t e . 1 . t l o g   `      p=     f=     ÃR2    öchÖÓ              " < l i n k . w r i t e . 1 . t l o g   `      p=     f=     hÃR2    öchÖÓ              " < l i n k . w r i t e . 1 . t l o g   `      p=     f=     ÈÃR2    öchÖÓ €            " < l i n k . w r i t e . 1 . t l o g   `      q=     Ø      (ÄR2    Î¹ehÖÓ                < D i r e c t X T K - m a s t e r     `      q=     Ø      ˆÄR2    Î¹ehÖÓ  €             < D i r e c t X T K - m a s t e r     `      q=     Ø      èÄR2    ÓçehÖÓ €               < D i r e c t X T K - m a s t e r     `      q=     Ø      HÅR2    ÓçehÖÓ € €             < D i r e c t X T K - m a s t e r     X      r=     q=     ¨ÅR2    şFfhÖÓ                < . g i t a t t r i b u t e s X      r=     q=      ÆR2    şFfhÖÓ               < . g i t a t t r i b u t e s X      r=     q=     XÆR2    şFfhÖÓ               < . g i t a t t r i b u t e s X      r=     q=     °ÆR2    Õ²fhÖÓ               < . g i t a t t r i b u t e s X      r=     q=     ÇR2    Õ²fhÖÓ €             < . g i t a t t r i b u t e s P      s=     q=     `ÇR2    ÔUghÖÓ                < . g i t i g n o r e P      s=     q=     °ÇR2    ÔUghÖÓ               < . g i t i g n o r e P      s=     q=      ÈR2    ç|ghÖÓ               < . g i t i g n o r e P      s=     q=     PÈR2    ç|ghÖÓ               < . g i t i g n o r e P      s=     q=      ÈR2    È‘ghÖÓ €             < . g i t i g n o r e p      t=     q=     ğÈR2    thhÖÓ               4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . s l n p      t=     q=     `ÉR2    thhÖÓ              4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . s l n p      t=     q=     ĞÉR2    ÷òihÖÓ              4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . s l n p      t=     q=     @ÊR2    ÷òihÖÓ              4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . s l n p      t=     q=     °ÊR2    ljhÖÓ €            4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . s l n x      u=     q=      ËR2    àòjhÖÓ               < < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . v c x p r o j x      u=     q=     ˜ËR2    àòjhÖÓ              < < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . v c x p r o j x      u=     q=     ÌR2    °lhÖÓ              < < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . v c x p r o j x      u=     q=     ˆÌR2    °lhÖÓ              < < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . v c x p r o j x      u=     q=      ÍR2    °lhÖÓ €            < < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . v c x p r o j ˆ      v=     q=     xÍR2    ÿ{mhÖÓ               L < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . v c x p r o j . f i l t e r s ˆ      v=     q=      ÎR2    ÿ{mhÖÓ              L < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . v c x p r o j . f i l t e r s ˆ      v=     q=     ˆÎR2    ¤mhÖÓ              L < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . v c x p r o j . f i l t e r s ˆ      v=     q=     ÏR2    «çnhÖÓ              L < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . v c x p r o j . f i l t e r s                                                                                                         ˆ      v=     q=      ĞR2    «çnhÖÓ €            L < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 . v c x p r o j . f i l t e r s €      w=     q=     ˆĞR2    ŸúohÖÓ               @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 _ D X S D K . s l n     €      w=     q=     ÑR2    ŸúohÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 _ D X S D K . s l n     €      w=     q=     ˆÑR2    ÖıphÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 _ D X S D K . s l n     €      w=     q=     ÒR2    ÖıphÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 _ D X S D K . s l n     €      w=     q=     ˆÒR2    ÖıphÖÓ €            @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 3 _ D X S D K . s l n     p      x=     q=     ÓR2    ÔqhÖÓ               4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . s l n p      x=     q=     xÓR2    ÔqhÖÓ              4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . s l n p      x=     q=     èÓR2    °ùqhÖÓ              4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . s l n p      x=     q=     XÔR2    °ùqhÖÓ              4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . s l n p      x=     q=     ÈÔR2    °ùqhÖÓ €            4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . s l n x      y=     q=     8ÕR2    ·KshÖÓ               < < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . v c x p r o j x      y=     q=     °ÕR2    ·KshÖÓ              < < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . v c x p r o j x      y=     q=     (ÖR2    ÊÕthÖÓ              < < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . v c x p r o j x      y=     q=      ÖR2    ÊÕthÖÓ              < < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . v c x p r o j x      y=     q=     ×R2    ÊÕthÖÓ €            < < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . v c x p r o j ˆ      z=     q=     ×R2    ´¾uhÖÓ               L < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . v c x p r o j . f i l t e r s ˆ      z=     q=     ØR2    ÓåuhÖÓ              L < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . v c x p r o j . f i l t e r s ˆ      z=     q=      ØR2    Å×vhÖÓ              L < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . v c x p r o j . f i l t e r s ˆ      z=     q=     (ÙR2    Å×vhÖÓ              L < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . v c x p r o j . f i l t e r s ˆ      z=     q=     °ÙR2    Å×vhÖÓ €            L < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 . v c x p r o j . f i l t e r s €      {=     q=     8ÚR2    
=xhÖÓ               @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ D X S D K . s l n     €      {=     q=     ¸ÚR2    
=xhÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ D X S D K . s l n     €      {=     q=     8ÛR2    O½xhÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ D X S D K . s l n     €      {=     q=     ¸ÛR2    O½xhÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ D X S D K . s l n     €      {=     q=     8ÜR2    O½xhÖÓ €            @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ D X S D K . s l n     €      |=     q=     ¸ÜR2    ı{yhÖÓ               @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . s l n     €      |=     q=     8İR2    ı{yhÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . s l n     €      |=     q=     ¸İR2    •zhÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . s l n     €      |=     q=     8ŞR2    •zhÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . s l n     €      |=     q=     ¸ŞR2    
€{hÖÓ €            @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . s l n     ˆ      }=     q=     8ßR2    võ{hÖÓ               H < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . v c x p r o j                                                                     ˆ      }=     q=      àR2    võ{hÖÓ              H < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . v c x p r o j     ˆ      }=     q=     ˆàR2    võ{hÖÓ              H < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . v c x p r o j     ˆ      }=     q=     áR2    võ{hÖÓ              H < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . v c x p r o j     ˆ      }=     q=     ˜áR2    võ{hÖÓ €            H < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . v c x p r o j     ˜      ~=     q=      âR2    {%~hÖÓ               X < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . v c x p r o j . f i l t e r s     ˜      ~=     q=     ¸âR2    L~hÖÓ              X < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . v c x p r o j . f i l t e r s     ˜      ~=     q=     PãR2    Œ¨hÖÓ              X < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . v c x p r o j . f i l t e r s     ˜      ~=     q=     èãR2    ¯ÏhÖÓ              X < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . v c x p r o j . f i l t e r s     ˜      ~=     q=     €äR2    E€hÖÓ €            X < D i r e c t X T K _ D e s k t o p _ 2 0 1 5 _ W i n 1 0 . v c x p r o j . f i l t e r s     p      =     q=     åR2    æ€hÖÓ               4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . s l n p      =     q=     ˆåR2    æ€hÖÓ              4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . s l n p      =     q=     øåR2    ‡ÑhÖÓ              4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . s l n p      =     q=     hæR2    ‡ÑhÖÓ              4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . s l n p      =     q=     ØæR2    ‡ÑhÖÓ €            4 < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . s l n x      €=     q=     HçR2    ä‚hÖÓ               < < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . v c x p r o j x      €=     q=     ÀçR2    ä‚hÖÓ              < < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . v c x p r o j x      €=     q=     8èR2    _ƒhÖÓ              < < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . v c x p r o j x      €=     q=     °èR2    _ƒhÖÓ              < < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . v c x p r o j x      €=     q=     (éR2    _ƒhÖÓ €            < < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . v c x p r o j ˆ      =     q=      éR2    *D„hÖÓ               L < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . v c x p r o j . f i l t e r s ˆ      =     q=     (êR2    *D„hÖÓ              L < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . v c x p r o j . f i l t e r s ˆ      =     q=     °êR2    *D„hÖÓ              L < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . v c x p r o j . f i l t e r s ˆ      =     q=     8ëR2    *D„hÖÓ              L < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . v c x p r o j . f i l t e r s ˆ      =     q=     ÀëR2    Tk„hÖÓ €            L < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 . v c x p r o j . f i l t e r s €      ‚=     q=     HìR2    Øà„hÖÓ               @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ D X S D K . s l n     €      ‚=     q=     ÈìR2    Øà„hÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ D X S D K . s l n     €      ‚=     q=     HíR2    Øà„hÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ D X S D K . s l n     €      ‚=     q=     ÈíR2    Øà„hÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ D X S D K . s l n     €      ‚=     q=     HîR2    û…hÖÓ €            @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ D X S D K . s l n     €      ƒ=     q=     ÈîR2    LV…hÖÓ               @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . s l n     €      ƒ=     q=     HïR2    LV…hÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . s l n                                                             €      ƒ=     q=      ğR2    …¨…hÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . s l n     €      ƒ=     q=     €ğR2    …¨…hÖÓ              @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . s l n     €      ƒ=     q=      ñR2    …¨…hÖÓ €            @ < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . s l n     ˆ      „=     q=     €ñR2    "†hÖÓ               H < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . v c x p r o j     ˆ      „=     q=     òR2    "†hÖÓ              H < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . v c x p r o j     ˆ      „=     q=     òR2    ç †hÖÓ              H < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . v c x p r o j     ˆ      „=     q=     óR2    §¾†hÖÓ              H < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . v c x p r o j     ˆ      „=     q=      óR2    §¾†hÖÓ €            H < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . v c x p r o j     ˜      …=     q=     (ôR2    ù‡hÖÓ               X < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . v c x p r o j . f i l t e r s     ˜      …=     q=     ÀôR2    ù‡hÖÓ              X < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . v c x p r o j . f i l t e r s     ˜      …=     q=     XõR2    ZÃˆhÖÓ              X < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . v c x p r o j . f i l t e r s     ˜      …=     q=     ğõR2    ZÃˆhÖÓ              X < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . v c x p r o j . f i l t e r s     ˜      …=     q=     ˆöR2    ZÃˆhÖÓ €            X < D i r e c t X T K _ D e s k t o p _ 2 0 1 7 _ W i n 1 0 . v c x p r o j . f i l t e r s     p      †=     q=      ÷R2    B1‰hÖÓ               . < D i r e c t X T K _ W i n d o w s 1 0 . s l n       p      †=     q=     ÷R2    B1‰hÖÓ              . < D i r e c t X T K _ W i n d o w s 1 0 . s l n       p      †=     q=      øR2    :‹hÖÓ              . < D i r e c t X T K _ W i n d o w s 1 0 . s l n       p      †=     q=     pøR2    :‹hÖÓ              . < D i r e c t X T K _ W i n d o w s 1 0 . s l n       p      †=     q=     àøR2    :‹hÖÓ €            . < D i r e c t X T K _ W i n d o w s 1 0 . s l n       x      ‡=     q=     PùR2    ïU‹hÖÓ               6 < D i r e c t X T K _ W i n d o w s 1 0 . v c x p r o j     s x      ‡=     q=     ÈùR2    ïU‹hÖÓ              6 < D i r e c t X T K _ W i n d o w s 1 0 . v c x p r o j     s x      ‡=     q=     @úR2    ŒhÖÓ              6 < D i r e c t X T K _ W i n d o w s 1 0 . v c x p r o j     s x      ‡=     q=     ¸úR2    ŒhÖÓ              6 < D i r e c t X T K _ W i n d o w s 1 0 . v c x p r o j     s x      ‡=     q=     0ûR2    ŒhÖÓ €            6 < D i r e c t X T K _ W i n d o w s 1 0 . v c x p r o j     s ˆ      ˆ=     q=     ¨ûR2    ³9hÖÓ               F < D i r e c t X T K _ W i n d o w s 1 0 . v c x p r o j . f i l t e r s     l ˆ      ˆ=     q=     0üR2    îChÖÓ              F < D i r e c t X T K _ W i n d o w s 1 0 . v c x p r o j . f i l t e r s     l ˆ      ˆ=     q=     ¸üR2    îChÖÓ              F < D i r e c t X T K _ W i n d o w s 1 0 . v c x p r o j . f i l t e r s     l ˆ      ˆ=     q=     @ıR2    îChÖÓ              F < D i r e c t X T K _ W i n d o w s 1 0 . v c x p r o j . f i l t e r s     l ˆ      ˆ=     q=     ÈıR2    khÖÓ €            F < D i r e c t X T K _ W i n d o w s 1 0 . v c x p r o j . f i l t e r s     l x      ‰=     q=     PşR2    èÇhÖÓ               8 < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . s l n     x      ‰=     q=     ÈşR2    «íhÖÓ              8 < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . s l n     x      ‰=     q=     @ÿR2    «íhÖÓ              8 < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . s l n                                                                             x      ‰=     q=       S2    «íhÖÓ              8 < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . s l n     x      ‰=     q=     x S2    ÎhÖÓ €            8 < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . s l n     €      Š=     q=     ğ S2    FhÖÓ               @ < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . v c x p r o j     €      Š=     q=     pS2    FhÖÓ              @ < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . v c x p r o j     €      Š=     q=     ğS2    Ë"hÖÓ              @ < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . v c x p r o j     €      Š=     q=     pS2    Ë"hÖÓ              @ < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . v c x p r o j     €      Š=     q=     ğS2    Ë"hÖÓ €            @ < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . v c x p r o j           ‹=     q=     pS2    õÀhÖÓ               P < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . v c x p r o j . f i l t e r s           ‹=     q=      S2    õÀhÖÓ              P < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . v c x p r o j . f i l t e r s           ‹=     q=     S2    õÀhÖÓ              P < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . v c x p r o j . f i l t e r s           ‹=     q=      S2    /èhÖÓ              P < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . v c x p r o j . f i l t e r s           ‹=     q=     °S2    /èhÖÓ €            P < D i r e c t X T K _ W i n d o w s 1 0 _ 2 0 1 5 . v c x p r o j . f i l t e r s     p      Œ=     q=     @S2    ¬•‘hÖÓ               . < D i r e c t X T K _ W i n d o w s 8 1 . s l n     &0p      Œ=     q=     °S2    Ø¸‘hÖÓ              . < D i r e c t X T K _ W i n d o w s 8 1 . s l n     &0p      Œ=     q=      S2    ğ’hÖÓ              . < D i r e c t X T K _ W i n d o w s 8 1 . s l n     &0p      Œ=     q=     S2    ğ’hÖÓ              . < D i r e c t X T K _ W i n d o w s 8 1 . s l n     &0p      Œ=     q=      S2    g<’hÖÓ €            . < D i r e c t X T K _ W i n d o w s 8 1 . s l n     &0x      =     q=     pS2    &·’hÖÓ               6 < D i r e c t X T K _ W i n d o w s 8 1 . v c x p r o j     2 x      =     q=     èS2    &·’hÖÓ              6 < D i r e c t X T K _ W i n d o w s 8 1 . v c x p r o j     2 x      =     q=     `	S2    ê“hÖÓ              6 < D i r e c t X T K _ W i n d o w s 8 1 . v c x p r o j     2 x      =     q=     Ø	S2    ê“hÖÓ              6 < D i r e c t X T K _ W i n d o w s 8 1 . v c x p r o j     2 x      =     q=     P
S2    ê“hÖÓ €            6 < D i r e c t X T K _ W i n d o w s 8 1 . v c x p r o j     2 ˆ      =     q=     È
S2    ´œ“hÖÓ               F < D i r e c t X T K _ W i n d o w s 8 1 . v c x p r o j . f i l t e r s     l ˆ      =     q=     PS2    ÚÃ“hÖÓ              F < D i r e c t X T K _ W i n d o w s 8 1 . v c x p r o j . f i l t e r s     l ˆ      =     q=     ØS2    ŞÍ“hÖÓ              F < D i r e c t X T K _ W i n d o w s 8 1 . v c x p r o j . f i l t e r s     l ˆ      =     q=     `S2    ŞÍ“hÖÓ              F < D i r e c t X T K _ W i n d o w s 8 1 . v c x p r o j . f i l t e r s     l ˆ      =     q=     èS2    ŞÍ“hÖÓ €            F < D i r e c t X T K _ W i n d o w s 8 1 . v c x p r o j . f i l t e r s     l x      =     q=     pS2    ,•hÖÓ               8 < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . s l n     x      =     q=     èS2    b.•hÖÓ              8 < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . s l n     x      =     q=     `S2    b.•hÖÓ              8 < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . s l n     x      =     q=     ØS2    b.•hÖÓ              8 < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . s l n     x      =     q=     PS2    }U•hÖÓ €            8 < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . s l n                                                             €      =     q=      S2    ‚ì•hÖÓ               @ < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . v c x p r o j     €      =     q=     €S2    ‚ì•hÖÓ              @ < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . v c x p r o j     €      =     q=      S2    Z –hÖÓ              @ < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . v c x p r o j     €      =     q=     €S2    Z –hÖÓ              @ < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . v c x p r o j     €      =     q=      S2    Z –hÖÓ €            @ < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . v c x p r o j           ‘=     q=     €S2    šy–hÖÓ               P < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . v c x p r o j . f i l t e r s           ‘=     q=     S2    šy–hÖÓ              P < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . v c x p r o j . f i l t e r s           ‘=     q=      S2    ÌD—hÖÓ              P < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . v c x p r o j . f i l t e r s           ‘=     q=     0S2    ÌD—hÖÓ              P < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . v c x p r o j . f i l t e r s           ‘=     q=     ÀS2    ÌD—hÖÓ €            P < D i r e c t X T K _ W i n d o w s P h o n e 8 1 . v c x p r o j . f i l t e r s     ˜      ’=     q=     PS2    ®¸—hÖÓ               X < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . s l n     ˜      ’=     q=     èS2    ®¸—hÖÓ              X < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . s l n     ˜      ’=     q=     €S2    Ğß—hÖÓ              X < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . s l n     ˜      ’=     q=     S2    Ğß—hÖÓ              X < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . s l n     ˜      ’=     q=     °S2    Ğß—hÖÓ €            X < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . s l n            “=     q=     HS2    „=˜hÖÓ               ` < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . v c x p r o j            “=     q=     èS2    „=˜hÖÓ              ` < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . v c x p r o j            “=     q=     ˆS2    +Å™hÖÓ              ` < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . v c x p r o j            “=     q=     (S2    +Å™hÖÓ              ` < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . v c x p r o j            “=     q=     ÈS2    +Å™hÖÓ €            ` < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . v c x p r o j     °      ”=     q=     hS2    ¦VšhÖÓ               p < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . v c x p r o j . f i l t e r s     °      ”=     q=     S2    ¦VšhÖÓ              p < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . v c x p r o j . f i l t e r s     °      ”=     q=     ÈS2    ÷šhÖÓ              p < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . v c x p r o j . f i l t e r s     °      ”=     q=     xS2    ÷šhÖÓ              p < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . v c x p r o j . f i l t e r s     °      ”=     q=     (S2    ÷šhÖÓ €            p < D i r e c t X T K _ X A M L S i l v e r l i g h t _ W i n d o w s P h o n e 8 1 . v c x p r o j . f i l t e r s     x      •=     q=     ØS2    ìù›hÖÓ               : < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . s l n   x      •=     q=     PS2    ìù›hÖÓ              : < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . s l n                                                           x      •=     q=       S2    ìù›hÖÓ              : < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . s l n   x      •=     q=     x S2    ìù›hÖÓ              : < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . s l n   x      •=     q=     ğ S2    ìù›hÖÓ €            : < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . s l n   €      –=     q=     h!S2    Ù•œhÖÓ               B < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . v c x p r o j   €      –=     q=     è!S2    Ù•œhÖÓ              B < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . v c x p r o j   €      –=     q=     h"S2    >èœhÖÓ              B < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . v c x p r o j   €      –=     q=     è"S2    >èœhÖÓ              B < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . v c x p r o j   €      –=     q=     h#S2    >èœhÖÓ €            B < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . v c x p r o j         —=     q=     è#S2    Æ˜hÖÓ               R < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . v c x p r o j . f i l t e r s         —=     q=     x$S2    Æ˜hÖÓ              R < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . v c x p r o j . f i l t e r s         —=     q=     %S2    å¿hÖÓ              R < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . v c x p r o j . f i l t e r s         —=     q=     ˜%S2    å¿hÖÓ              R < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . v c x p r o j . f i l t e r s         —=     q=     (&S2    å¿hÖÓ €            R < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 5 . v c x p r o j . f i l t e r s   x      ˜=     q=     ¸&S2    —Í hÖÓ               : < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . s l n   x      ˜=     q=     0'S2    —Í hÖÓ              : < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . s l n   x      ˜=     q=     ¨'S2    —Í hÖÓ              : < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . s l n   x      ˜=     q=      (S2    —Í hÖÓ              : < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . s l n   x      ˜=     q=     ˜(S2    —Í hÖÓ €            : < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . s l n   €      ™=     q=     )S2    C¡hÖÓ               B < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . v c x p r o j   €      ™=     q=     )S2    C¡hÖÓ              B < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . v c x p r o j   €      ™=     q=     *S2    C¡hÖÓ              B < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . v c x p r o j   €      ™=     q=     *S2    C¡hÖÓ              B < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . v c x p r o j   €      ™=     q=     +S2    ?€¡hÖÓ €            B < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . v c x p r o j         š=     q=     +S2    Ã†¢hÖÓ               R < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . v c x p r o j . f i l t e r s         š=     q=      ,S2    Ã†¢hÖÓ              R < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . v c x p r o j . f i l t e r s         š=     q=     °,S2    _¤hÖÓ              R < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . v c x p r o j . f i l t e r s         š=     q=     @-S2    _¤hÖÓ              R < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . v c x p r o j . f i l t e r s         š=     q=     Ğ-S2    _¤hÖÓ €            R < D i r e c t X T K _ X b o x O n e X D K _ 2 0 1 7 . v c x p r o j . f i l t e r s   P      ›=     q=     `.S2    ™•¤hÖÓ                < L I C E N S E       P      ›=     q=     °.S2    ™•¤hÖÓ               < L I C E N S E       P      ›=     q=      /S2    }¥hÖÓ               < L I C E N S E       P      ›=     q=     P/S2    }¥hÖÓ               < L I C E N S E       P      ›=     q=      /S2    }¥hÖÓ €             < L I C E N S E                       P      œ=     q=      0S2    6¿¦hÖÓ                < R e a d m e . t x t P      œ=     q=     P0S2    6¿¦hÖÓ               < R e a d m e . t x t P      œ=     q=      0S2    ƒ§hÖÓ               < R e a d m e . t x t P      œ=     q=     ğ0S2    ƒ§hÖÓ               < R e a d m e . t x t P      œ=     q=     @1S2    ƒ§hÖÓ €             < R e a d m e . t x t H      =     q=     1S2    ‡Î¨hÖÓ              
 < A u d i o   H      =     q=     Ø1S2    ‡Î¨hÖÓ  €           
 < A u d i o   H      =     q=      2S2    ÇÚ¨hÖÓ €             
 < A u d i o   H      =     q=     h2S2    ÇÚ¨hÖÓ € €           
 < A u d i o   `      =     =     °2S2    ùx©hÖÓ                < A u d i o E n g i n e . c p p     I `      =     =     3S2    ùx©hÖÓ               < A u d i o E n g i n e . c p p     I `      =     =     p3S2    ícªhÖÓ               < A u d i o E n g i n e . c p p     I `      =     =     Ğ3S2    ‰ªhÖÓ               < A u d i o E n g i n e . c p p     I `      =     =     04S2    ‰ªhÖÓ €             < A u d i o E n g i n e . c p p     I       Ÿ=     =     4S2    -«hÖÓ               R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ D X S D K . v c x p r o j         Ÿ=     =      5S2    -«hÖÓ              R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ D X S D K . v c x p r o j         Ÿ=     =     °5S2    Œ‹¬hÖÓ              R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ D X S D K . v c x p r o j         Ÿ=     =     @6S2    Œ‹¬hÖÓ              R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ D X S D K . v c x p r o j         Ÿ=     =     Ğ6S2    ¿²¬hÖÓ €            R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ D X S D K . v c x p r o j           =     =     `7S2    RO­hÖÓ               b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ D X S D K . v c x p r o j . f i l t e r s           =     =      8S2    RO­hÖÓ              b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ D X S D K . v c x p r o j . f i l t e r s           =     =      8S2    :5®hÖÓ              b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ D X S D K . v c x p r o j . f i l t e r s           =     =     @9S2    :5®hÖÓ              b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ D X S D K . v c x p r o j . f i l t e r s           =     =     à9S2    :5®hÖÓ €            b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ D X S D K . v c x p r o j . f i l t e r s         ¡=     =     €:S2    +°hÖÓ               P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ W i n 8 . v c x p r o j           ¡=     =     ;S2    +°hÖÓ              P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ W i n 8 . v c x p r o j           ¡=     =      ;S2    ±hÖÓ              P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ W i n 8 . v c x p r o j           ¡=     =     0<S2    ±hÖÓ              P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ W i n 8 . v c x p r o j           ¡=     =     À<S2    ±hÖÓ €            P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ W i n 8 . v c x p r o j            ¢=     =     P=S2    •²hÖÓ               ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ W i n 8 . v c x p r o j . f i l t e r s            ¢=     =     ğ=S2    <|³hÖÓ              ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ W i n 8 . v c x p r o j . f i l t e r s            ¢=     =     >S2    <|³hÖÓ              ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ W i n 8 . v c x p r o j . f i l t e r s            ¢=     =     0?S2    <|³hÖÓ              ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ W i n 8 . v c x p r o j . f i l t e r s                                                            ¢=     =      @S2    Ò¤³hÖÓ €            ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 3 _ W i n 8 . v c x p r o j . f i l t e r s           £=     =      @S2    ´hÖÓ               R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ D X S D K . v c x p r o j         £=     =     0AS2    ´hÖÓ              R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ D X S D K . v c x p r o j         £=     =     ÀAS2    ´hÖÓ              R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ D X S D K . v c x p r o j         £=     =     PBS2    ´hÖÓ              R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ D X S D K . v c x p r o j         £=     =     àBS2    ´hÖÓ €            R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ D X S D K . v c x p r o j          ¤=     =     pCS2    °µhÖÓ               b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ D X S D K . v c x p r o j . f i l t e r s          ¤=     =     DS2    şóµhÖÓ              b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ D X S D K . v c x p r o j . f i l t e r s          ¤=     =     °DS2    şóµhÖÓ              b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ D X S D K . v c x p r o j . f i l t e r s          ¤=     =     PES2    şóµhÖÓ              b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ D X S D K . v c x p r o j . f i l t e r s          ¤=     =     ğES2    )¶hÖÓ €            b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ D X S D K . v c x p r o j . f i l t e r s         ¥=     =     FS2    n†¶hÖÓ               P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ W i n 8 . v c x p r o j           ¥=     =      GS2    n†¶hÖÓ              P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ W i n 8 . v c x p r o j           ¥=     =     °GS2    úÄ·hÖÓ              P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ W i n 8 . v c x p r o j           ¥=     =     @HS2    úÄ·hÖÓ              P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ W i n 8 . v c x p r o j           ¥=     =     ĞHS2    úÄ·hÖÓ €            P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ W i n 8 . v c x p r o j            ¦=     =     `IS2    êó¹hÖÓ               ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ W i n 8 . v c x p r o j . f i l t e r s            ¦=     =      JS2    êó¹hÖÓ              ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ W i n 8 . v c x p r o j . f i l t e r s            ¦=     =      JS2     `ºhÖÓ              ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ W i n 8 . v c x p r o j . f i l t e r s            ¦=     =     @KS2     `ºhÖÓ              ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ W i n 8 . v c x p r o j . f i l t e r s            ¦=     =     àKS2     `ºhÖÓ €            ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 5 _ W i n 8 . v c x p r o j . f i l t e r s           §=     =     €LS2    xÕºhÖÓ               R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ D X S D K . v c x p r o j         §=     =     MS2    ŸüºhÖÓ              R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ D X S D K . v c x p r o j         §=     =      MS2    (¼hÖÓ              R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ D X S D K . v c x p r o j         §=     =     0NS2    (¼hÖÓ              R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ D X S D K . v c x p r o j         §=     =     ÀNS2    (¼hÖÓ €            R < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ D X S D K . v c x p r o j          ¨=     =     POS2    4ª¼hÖÓ               b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ D X S D K . v c x p r o j . f i l t e r s                          ¨=     =      PS2    4ª¼hÖÓ              b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ D X S D K . v c x p r o j . f i l t e r s          ¨=     =      PS2    4ª¼hÖÓ              b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ D X S D K . v c x p r o j . f i l t e r s          ¨=     =     @QS2    4ª¼hÖÓ              b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ D X S D K . v c x p r o j . f i l t e r s          ¨=     =     àQS2    [Ñ¼hÖÓ €            b < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ D X S D K . v c x p r o j . f i l t e r s         ©=     =     €RS2    ¿2½hÖÓ               P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ W i n 8 . v c x p r o j           ©=     =     SS2    ¿2½hÖÓ              P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ W i n 8 . v c x p r o j           ©=     =      SS2    ë½hÖÓ              P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ W i n 8 . v c x p r o j           ©=     =     0TS2    ë½hÖÓ              P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ W i n 8 . v c x p r o j           ©=     =     ÀTS2    ë½hÖÓ €            P < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ W i n 8 . v c x p r o j            ª=     =     PUS2    ~µ¾hÖÓ               ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ W i n 8 . v c x p r o j . f i l t e r s            ª=     =     ğUS2    ~µ¾hÖÓ              ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ W i n 8 . v c x p r o j . f i l t e r s            ª=     =     VS2    eÖ¾hÖÓ              ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ W i n 8 . v c x p r o j . f i l t e r s            ª=     =     0WS2    eÖ¾hÖÓ              ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ W i n 8 . v c x p r o j . f i l t e r s            ª=     =     ĞWS2    eÖ¾hÖÓ €            ` < D i r e c t X T K A u d i o _ D e s k t o p _ 2 0 1 7 _ W i n 8 . v c x p r o j . f i l t e r s     x      «=     =     pXS2    ‡a¿hÖÓ               < < D y n a m i c S o u n d E f f e c t I n s t a n c e . c p p x      «=     =     èXS2    ‡a¿hÖÓ              < < D y n a m i c S o u n d E f f e c t I n s t a n c e . c p p x      «=     =     `YS2    „LÀhÖÓ              < < D y n a m i c S o u n d E f f e c t I n s t a n c e . c p p x      «=     =     ØYS2    „LÀhÖÓ              < < D y n a m i c S o u n d E f f e c t I n s t a n c e . c p p x      «=     =     PZS2    „LÀhÖÓ €            < < D y n a m i c S o u n d E f f e c t I n s t a n c e . c p p `      ¬=     =     ÈZS2    ,×ÀhÖÓ                < S o u n d C o m m o n . c p p       `      ¬=     =     ([S2    ,×ÀhÖÓ               < S o u n d C o m m o n . c p p       `      ¬=     =     ˆ[S2    GRÂhÖÓ               < S o u n d C o m m o n . c p p       `      ¬=     =     è[S2    GRÂhÖÓ               < S o u n d C o m m o n . c p p       `      ¬=     =     H\S2    GRÂhÖÓ €             < S o u n d C o m m o n . c p p       X      ­=     =     ¨\S2    ôÉÂhÖÓ                < S o u n d C o m m o n . h   X      ­=     =      ]S2    ôÉÂhÖÓ               < S o u n d C o m m o n . h   X      ­=     =     X]S2    M?ÃhÖÓ               < S o u n d C o m m o n . h   X      ­=     =     °]S2    ÉBÃhÖÓ               < S o u n d C o m m o n . h   X      ­=     =     ^S2    ÉBÃhÖÓ €             < S o u n d C o m m o n . h   `      ®=     =     `^S2    ¶-ÄhÖÓ                < S o u n d E f f e c t . c p p       `      ®=     =     À^S2    ¶-ÄhÖÓ               < S o u n d E f f e c t . c p p       `      ®=     =      _S2    êÅhÖÓ               < S o u n d E f f e c t . c p p       `      ®=     =     €_S2    êÅhÖÓ               < S o u n d E f f e c t . c p p                                       `      ®=     =      `S2    êÅhÖÓ €             < S o u n d E f f e c t . c p p       p      ¯=     =     ``S2    ÀÇhÖÓ               . < S o u n d E f f e c t I n s t a n c e . c p p       p      ¯=     =     Ğ`S2    ÀÇhÖÓ              . < S o u n d E f f e c t I n s t a n c e . c p p       p      ¯=     =     @aS2    çÈhÖÓ              . < S o u n d E f f e c t I n s t a n c e . c p p       p      ¯=     =     °aS2    çÈhÖÓ              . < S o u n d E f f e c t I n s t a n c e . c p p       p      ¯=     =      bS2    çÈhÖÓ €            . < S o u n d E f f e c t I n s t a n c e . c p p       X      °=     =     bS2    <°ÉhÖÓ                < W a v e B a n k . c p p     X      °=     =     èbS2    <°ÉhÖÓ               < W a v e B a n k . c p p     X      °=     =     @cS2    }œÊhÖÓ               < W a v e B a n k . c p p     X      °=     =     ˜cS2    }œÊhÖÓ               < W a v e B a n k . c p p     X      °=     =     ğcS2    }œÊhÖÓ €             < W a v e B a n k . c p p     `      ±=     =     HdS2    )ÌhÖÓ               $ < W a v e B a n k R e a d e r . c p p `      ±=     =     ¨dS2    )ÌhÖÓ              $ < W a v e B a n k R e a d e r . c p p `      ±=     =     eS2    "˜ÎhÖÓ              $ < W a v e B a n k R e a d e r . c p p `      ±=     =     heS2    "˜ÎhÖÓ              $ < W a v e B a n k R e a d e r . c p p `      ±=     =     ÈeS2    +ÂÎhÖÓ €            $ < W a v e B a n k R e a d e r . c p p `      ²=     =     (fS2    }ÏhÖÓ                 < W a v e B a n k R e a d e r . h     `      ²=     =     ˆfS2    }ÏhÖÓ                < W a v e B a n k R e a d e r . h     `      ²=     =     èfS2    }ÏhÖÓ                < W a v e B a n k R e a d e r . h     `      ²=     =     HgS2    «7ÏhÖÓ                < W a v e B a n k R e a d e r . h     `      ²=     =     ¨gS2    «7ÏhÖÓ €              < W a v e B a n k R e a d e r . h     `      ³=     =     hS2    Å˜ÏhÖÓ               " < W A V F i l e R e a d e r . c p p   `      ³=     =     hhS2    Å˜ÏhÖÓ              " < W A V F i l e R e a d e r . c p p   `      ³=     =     ÈhS2    ¸ĞhÖÓ              " < W A V F i l e R e a d e r . c p p   `      ³=     =     (iS2    ¸ĞhÖÓ              " < W A V F i l e R e a d e r . c p p   `      ³=     =     ˆiS2    ¸ĞhÖÓ €            " < W A V F i l e R e a d e r . c p p   `      ´=     =     èiS2    0îĞhÖÓ                < W A V F i l e R e a d e r . h       `      ´=     =     HjS2    0îĞhÖÓ               < W A V F i l e R e a d e r . h       `      ´=     =     ¨jS2    ¾$ÑhÖÓ               < W A V F i l e R e a d e r . h       `      ´=     =     kS2    ¾$ÑhÖÓ               < W A V F i l e R e a d e r . h       `      ´=     =     hkS2    ¾$ÑhÖÓ €             < W A V F i l e R e a d e r . h       H      µ=     q=     ÈkS2    Í¯ÑhÖÓ               < I n c       H      µ=     q=     lS2    Í¯ÑhÖÓ  €            < I n c       H      µ=     q=     XlS2    ¢ÓÑhÖÓ €              < I n c       H      µ=     q=      lS2    ÑúÑhÖÓ € €            < I n c       P      ¶=     µ=     èlS2    IÒhÖÓ                < A u d i o . h       P      ¶=     µ=     8mS2    IÒhÖÓ               < A u d i o . h       P      ¶=     µ=     ˆmS2    ÙËÓhÖÓ               < A u d i o . h       P      ¶=     µ=     ØmS2    ÙËÓhÖÓ               < A u d i o . h       P      ¶=     µ=     (nS2     ñÓhÖÓ €             < A u d i o . h       X      ·=     µ=     xnS2    HNÔhÖÓ                < C o m m o n S t a t e s . h X      ·=     µ=     ĞnS2    HNÔhÖÓ               < C o m m o n S t a t e s . h X      ·=     µ=     (oS2    HNÔhÖÓ               < C o m m o n S t a t e s . h X      ·=     µ=     €oS2    HNÔhÖÓ               < C o m m o n S t a t e s . h                                         X      ·=     µ=      pS2    pwÔhÖÓ €             < C o m m o n S t a t e s . h `      ¸=     µ=     XpS2    ÁÅÔhÖÓ               $ < D D S T e x t u r e L o a d e r . h `      ¸=     µ=     ¸pS2    ÁÅÔhÖÓ              $ < D D S T e x t u r e L o a d e r . h `      ¸=     µ=     qS2    9&ÖhÖÓ              $ < D D S T e x t u r e L o a d e r . h `      ¸=     µ=     xqS2    9&ÖhÖÓ              $ < D D S T e x t u r e L o a d e r . h `      ¸=     µ=     ØqS2    9&ÖhÖÓ €            $ < D D S T e x t u r e L o a d e r . h `      ¹=     µ=     8rS2    ŸtÖhÖÓ                 < D i r e c t X H e l p e r s . h     `      ¹=     µ=     ˜rS2    ŸtÖhÖÓ                < D i r e c t X H e l p e r s . h     `      ¹=     µ=     ørS2    ½›ÖhÖÓ                < D i r e c t X H e l p e r s . h     `      ¹=     µ=     XsS2    ½›ÖhÖÓ                < D i r e c t X H e l p e r s . h     `      ¹=     µ=     ¸sS2    ½›ÖhÖÓ €              < D i r e c t X H e l p e r s . h     P      º=     µ=     tS2    êÖhÖÓ                < E f f e c t s . h   P      º=     µ=     htS2    êÖhÖÓ               < E f f e c t s . h   P      º=     µ=     ¸tS2    ß`×hÖÓ               < E f f e c t s . h   P      º=     µ=     uS2    ß`×hÖÓ               < E f f e c t s . h   P      º=     µ=     XuS2    ß`×hÖÓ €             < E f f e c t s . h   P      »=     µ=     ¨uS2    º®×hÖÓ                < G a m e P a d . h   P      »=     µ=     øuS2    º®×hÖÓ               < G a m e P a d . h   P      »=     µ=     HvS2    U;ÙhÖÓ               < G a m e P a d . h   P      »=     µ=     ˜vS2    U;ÙhÖÓ               < G a m e P a d . h   P      »=     µ=     èvS2    U;ÙhÖÓ €             < G a m e P a d . h   h      ¼=     µ=     8wS2    ±ÙhÖÓ               ( < G e o m e t r i c P r i m i t i v e . h     h      ¼=     µ=      wS2    ±ÙhÖÓ              ( < G e o m e t r i c P r i m i t i v e . h     h      ¼=     µ=     xS2    ±ÙhÖÓ              ( < G e o m e t r i c P r i m i t i v e . h     h      ¼=     µ=     pxS2    ±ÙhÖÓ              ( < G e o m e t r i c P r i m i t i v e . h     h      ¼=     µ=     ØxS2    ±ÙhÖÓ €            ( < G e o m e t r i c P r i m i t i v e . h     `      ½=     µ=     @yS2    ÿÙhÖÓ                 < G r a p h i c s M e m o r y . h     `      ½=     µ=      yS2    ÿÙhÖÓ                < G r a p h i c s M e m o r y . h     `      ½=     µ=      zS2    ™ÛhÖÓ                < G r a p h i c s M e m o r y . h     `      ½=     µ=     `zS2    ™ÛhÖÓ                < G r a p h i c s M e m o r y . h     `      ½=     µ=     ÀzS2    ™ÛhÖÓ €              < G r a p h i c s M e m o r y . h     P      ¾=     µ=      {S2    Í\İhÖÓ                < K e y b o a r d . h P      ¾=     µ=     p{S2    Í\İhÖÓ               < K e y b o a r d . h P      ¾=     µ=     À{S2    S"àhÖÓ               < K e y b o a r d . h P      ¾=     µ=     |S2    ¼àhÖÓ               < K e y b o a r d . h P      ¾=     µ=     `|S2    ¼àhÖÓ €             < K e y b o a r d . h P      ¿=     µ=     °|S2    Ù!áhÖÓ                < M o d e l . h     B P      ¿=     µ=      }S2    Ù!áhÖÓ               < M o d e l . h     B P      ¿=     µ=     P}S2    Ù!áhÖÓ               < M o d e l . h     B P      ¿=     µ=      }S2     IáhÖÓ               < M o d e l . h     B P      ¿=     µ=     ğ}S2     IáhÖÓ €             < M o d e l . h     B P      À=     µ=     @~S2    -‡áhÖÓ                < M o u s e . h       P      À=     µ=     ~S2    -‡áhÖÓ               < M o u s e . h       P      À=     µ=     à~S2    X®áhÖÓ               < M o u s e . h       P      À=     µ=     0S2    X®áhÖÓ               < M o u s e . h       P      À=     µ=     €S2    X®áhÖÓ €             < M o u s e . h                                                       X      Á=     µ=      €S2    ‚ÕáhÖÓ                < P o s t P r o c e s s . h   X      Á=     µ=     X€S2    ¥üáhÖÓ               < P o s t P r o c e s s . h   X      Á=     µ=     °€S2    ¥üáhÖÓ               < P o s t P r o c e s s . h   X      Á=     µ=     S2    ¥üáhÖÓ               < P o s t P r o c e s s . h   X      Á=     µ=     `S2    öâhÖÓ €             < P o s t P r o c e s s . h   `      Â=     µ=     ¸S2    !;âhÖÓ                 < P r i m i t i v e B a t c h . h     `      Â=     µ=     ‚S2    !;âhÖÓ                < P r i m i t i v e B a t c h . h     `      Â=     µ=     x‚S2    !;âhÖÓ                < P r i m i t i v e B a t c h . h     `      Â=     µ=     Ø‚S2    !;âhÖÓ                < P r i m i t i v e B a t c h . h     `      Â=     µ=     8ƒS2    !;âhÖÓ €              < P r i m i t i v e B a t c h . h     X      Ã=     µ=     ˜ƒS2    º™âhÖÓ                < S c r e e n G r a b . h     X      Ã=     µ=     ğƒS2    º™âhÖÓ               < S c r e e n G r a b . h     X      Ã=     µ=     H„S2    º™âhÖÓ               < S c r e e n G r a b . h     X      Ã=     µ=      „S2    º™âhÖÓ               < S c r e e n G r a b . h     X      Ã=     µ=     ø„S2    º™âhÖÓ €             < S c r e e n G r a b . h     X      Ä=     µ=     P…S2    èâhÖÓ                < S i m p l e M a t h . h     X      Ä=     µ=     ¨…S2    èâhÖÓ               < S i m p l e M a t h . h     X      Ä=     µ=      †S2    ¦fçhÖÓ               < S i m p l e M a t h . h     X      Ä=     µ=     X†S2    ¦fçhÖÓ               < S i m p l e M a t h . h     X      Ä=     µ=     °†S2    ¦fçhÖÓ €             < S i m p l e M a t h . h     X      Å=     µ=     ‡S2    –ÎçhÖÓ                < S i m p l e M a t h . i n l X      Å=     µ=     `‡S2    –ÎçhÖÓ               < S i m p l e M a t h . i n l X      Å=     µ=     ¸‡S2    “eéhÖÓ               < S i m p l e M a t h . i n l X      Å=     µ=     ˆS2    îKêhÖÓ               < S i m p l e M a t h . i n l X      Å=     µ=     hˆS2    îKêhÖÓ €             < S i m p l e M a t h . i n l X      Æ=     µ=     ÀˆS2    ™èêhÖÓ                < S p r i t e B a t c h . h   X      Æ=     µ=     ‰S2    ™èêhÖÓ               < S p r i t e B a t c h . h   X      Æ=     µ=     p‰S2    \şëhÖÓ               < S p r i t e B a t c h . h   X      Æ=     µ=     È‰S2    \şëhÖÓ               < S p r i t e B a t c h . h   X      Æ=     µ=      ŠS2    \şëhÖÓ €             < S p r i t e B a t c h . h   X      Ç=     µ=     xŠS2    ÙtìhÖÓ                < S p r i t e F o n t . h     X      Ç=     µ=     ĞŠS2    ÙtìhÖÓ               < S p r i t e F o n t . h     X      Ç=     µ=     (‹S2    œìhÖÓ               < S p r i t e F o n t . h     X      Ç=     µ=     €‹S2    `¨ìhÖÓ               < S p r i t e F o n t . h     X      Ç=     µ=     Ø‹S2    `¨ìhÖÓ €             < S p r i t e F o n t . h     X      È=     µ=     0ŒS2    ƒÏìhÖÓ                < V e r t e x T y p e s . h   X      È=     µ=     ˆŒS2    ƒÏìhÖÓ               < V e r t e x T y p e s . h   X      È=     µ=     àŒS2    ™îhÖÓ               < V e r t e x T y p e s . h   X      È=     µ=     8S2    ™îhÖÓ               < V e r t e x T y p e s . h   X      È=     µ=     S2    ™îhÖÓ €             < V e r t e x T y p e s . h   `      É=     µ=     èS2    +èïhÖÓ               $ < W I C T e x t u r e L o a d e r . h `      É=     µ=     HS2    +èïhÖÓ              $ < W I C T e x t u r e L o a d e r . h `      É=     µ=     ¨S2    ï!ñhÖÓ              $ < W I C T e x t u r e L o a d e r . h `      É=     µ=     S2    ï!ñhÖÓ              $ < W I C T e x t u r e L o a d e r . h `      É=     µ=     hS2    ï!ñhÖÓ €            $ < W I C T e x t u r e L o a d e r . h                                                         h      Ê=     µ=      S2    ±¾ñhÖÓ               , < X b o x D D S T e x t u r e L o a d e r . h h      Ê=     µ=     hS2    ±¾ñhÖÓ              , < X b o x D D S T e x t u r e L o a d e r . h h      Ê=     µ=     ĞS2    ŞÎñhÖÓ              , < X b o x D D S T e x t u r e L o a d e r . h h      Ê=     µ=     8‘S2    ŞÎñhÖÓ              , < X b o x D D S T e x t u r e L o a d e r . h h      Ê=     µ=      ‘S2    ŞÎñhÖÓ €            , < X b o x D D S T e x t u r e L o a d e r . h X      Ë=     q=     ’S2    DòhÖÓ               < M a k e S p r i t e F o n t X      Ë=     q=     `’S2    DòhÖÓ  €            < M a k e S p r i t e F o n t X      Ë=     q=     ¸’S2    €oòhÖÓ €              < M a k e S p r i t e F o n t X      Ë=     q=     “S2    €oòhÖÓ € €            < M a k e S p r i t e F o n t `      Ì=     Ë=     h“S2    †óhÖÓ               " < B i t m a p I m p o r t e r . c s   `      Ì=     Ë=     È“S2    †óhÖÓ              " < B i t m a p I m p o r t e r . c s   `      Ì=     Ë=     (”S2    e3óhÖÓ              " < B i t m a p I m p o r t e r . c s   `      Ì=     Ë=     ˆ”S2    e3óhÖÓ              " < B i t m a p I m p o r t e r . c s   `      Ì=     Ë=     è”S2    e3óhÖÓ €            " < B i t m a p I m p o r t e r . c s   X      Í=     Ë=     H•S2    Í¨óhÖÓ                < B i t m a p U t i l s . c s X      Í=     Ë=      •S2    Í¨óhÖÓ               < B i t m a p U t i l s . c s X      Í=     Ë=     ø•S2    Í¨óhÖÓ               < B i t m a p U t i l s . c s X      Í=     Ë=     P–S2    Í¨óhÖÓ               < B i t m a p U t i l s . c s X      Í=     Ë=     ¨–S2    Í¨óhÖÓ €             < B i t m a p U t i l s . c s `      Î=     Ë=      —S2    MôhÖÓ               $ < C h a r a c t e r R e g i o n . c s `      Î=     Ë=     `—S2    MôhÖÓ              $ < C h a r a c t e r R e g i o n . c s `      Î=     Ë=     À—S2    5_ôhÖÓ              $ < C h a r a c t e r R e g i o n . c s `      Î=     Ë=      ˜S2    5_ôhÖÓ              $ < C h a r a c t e r R e g i o n . c s `      Î=     Ë=     €˜S2    5_ôhÖÓ €            $ < C h a r a c t e r R e g i o n . c s h      Ï=     Ë=     à˜S2    ƒ­ôhÖÓ               * < C o m m a n d L i n e O p t i o n s . c s   h      Ï=     Ë=     H™S2    ƒ­ôhÖÓ              * < C o m m a n d L i n e O p t i o n s . c s   h      Ï=     Ë=     °™S2    ƒ­ôhÖÓ              * < C o m m a n d L i n e O p t i o n s . c s   h      Ï=     Ë=     šS2    ¶øôhÖÓ              * < C o m m a n d L i n e O p t i o n s . c s   h      Ï=     Ë=     €šS2    ¶øôhÖÓ €            * < C o m m a n d L i n e O p t i o n s . c s   h      Ğ=     Ë=     èšS2    2nõhÖÓ               ( < C o m m a n d L i n e P a r s e r . c s     h      Ğ=     Ë=     P›S2    2nõhÖÓ              ( < C o m m a n d L i n e P a r s e r . c s     h      Ğ=     Ë=     ¸›S2    2nõhÖÓ              ( < C o m m a n d L i n e P a r s e r . c s     h      Ğ=     Ë=      œS2    2nõhÖÓ              ( < C o m m a n d L i n e P a r s e r . c s     h      Ğ=     Ë=     ˆœS2    Y•õhÖÓ €            ( < C o m m a n d L i n e P a r s e r . c s     P      Ñ=     Ë=     ğœS2    «ãõhÖÓ                < G l y p h . c s     P      Ñ=     Ë=     @S2    «ãõhÖÓ               < G l y p h . c s     P      Ñ=     Ë=     S2    «ãõhÖÓ               < G l y p h . c s     P      Ñ=     Ë=     àS2    «ãõhÖÓ               < G l y p h . c s     P      Ñ=     Ë=     0S2    «ãõhÖÓ €             < G l y p h . c s     `      Ò=     Ë=     €S2     2öhÖÓ                < G l y p h C r o p p e r . c s       `      Ò=     Ë=     àS2    /YöhÖÓ               < G l y p h C r o p p e r . c s       `      Ò=     Ë=     @ŸS2    ³¹÷hÖÓ               < G l y p h C r o p p e r . c s       `      Ò=     Ë=      ŸS2    ³¹÷hÖÓ               < G l y p h C r o p p e r . c s       `      Ò=     Ë=       S2    ³¹÷hÖÓ €             < G l y p h C r o p p e r . c s       X      Ó=     Ë=     ` S2    ùLúhÖÓ                < G l y p h P a c k e r . c s X      Ó=     Ë=     ¸ S2    ùLúhÖÓ               < G l y p h P a c k e r . c s X      Ó=     Ë=     ¡S2    äıhÖÓ               < G l y p h P a c k e r . c s X      Ó=     Ë=     h¡S2    äıhÖÓ               < G l y p h P a c k e r . c s X      Ó=     Ë=     À¡S2    äıhÖÓ €             < G l y p h P a c k e r . c s `      Ô=     Ë=     ¢S2    9nıhÖÓ                 < I F o n t I m p o r t e r . c s     `      Ô=     Ë=     x¢S2    9nıhÖÓ                < I F o n t I m p o r t e r . c s     `      Ô=     Ë=     Ø¢S2    9nıhÖÓ                < I F o n t I m p o r t e r . c s     `      Ô=     Ë=     8£S2    9nıhÖÓ                < I F o n t I m p o r t e r . c s     `      Ô=     Ë=     ˜£S2    9nıhÖÓ €              < I F o n t I m p o r t e r . c s     h      Õ=     Ë=     ø£S2    ¼ıhÖÓ               * < M a k e S p r i t e F o n t . c s p r o j   h      Õ=     Ë=     `¤S2    ¼ıhÖÓ              * < M a k e S p r i t e F o n t . c s p r o j   h      Õ=     Ë=     È¤S2    ¼ıhÖÓ              * < M a k e S p r i t e F o n t . c s p r o j   h      Õ=     Ë=     0¥S2    ¼ıhÖÓ              * < M a k e S p r i t e F o n t . c s p r o j   h      Õ=     Ë=     ˜¥S2    ¼ıhÖÓ €            * < M a k e S p r i t e F o n t . c s p r o j   P      Ö=     Ë=      ¦S2    ä
şhÖÓ                < P r o g r a m . c s P      Ö=     Ë=     P¦S2    ä
şhÖÓ               < P r o g r a m . c s P      Ö=     Ë=      ¦S2    ä
şhÖÓ               < P r o g r a m . c s P      Ö=     Ë=     ğ¦S2    ä
şhÖÓ               < P r o g r a m . c s P      Ö=     Ë=     @§S2    ä
şhÖÓ €             < P r o g r a m . c s h      ×=     Ë=     §S2    =YşhÖÓ               & < S p r i t e F o n t W r i t e r . c s     . h      ×=     Ë=     ø§S2    =YşhÖÓ              & < S p r i t e F o n t W r i t e r . c s     . h      ×=     Ë=     `¨S2    =YşhÖÓ              & < S p r i t e F o n t W r i t e r . c s     . h      ×=     Ë=     È¨S2    =YşhÖÓ              & < S p r i t e F o n t W r i t e r . c s     . h      ×=     Ë=     0©S2    =YşhÖÓ €            & < S p r i t e F o n t W r i t e r . c s     . h      Ø=     Ë=     ˜©S2    “§şhÖÓ               & < T r u e T y p e I m p o r t e r . c s     p h      Ø=     Ë=      ªS2    “§şhÖÓ              & < T r u e T y p e I m p o r t e r . c s     p h      Ø=     Ë=     hªS2    “§şhÖÓ              & < T r u e T y p e I m p o r t e r . c s     p h      Ø=     Ë=     ĞªS2    ùÎşhÖÓ              & < T r u e T y p e I m p o r t e r . c s     p h      Ø=     Ë=     8«S2    ùÎşhÖÓ €            & < T r u e T y p e I m p o r t e r . c s     p P      Ù=     Ë=      «S2    ÿhÖÓ               < P r o p e r t i e s P      Ù=     Ë=     ğ«S2    ÿhÖÓ  €            < P r o p e r t i e s P      Ù=     Ë=     @¬S2    =DÿhÖÓ €              < P r o p e r t i e s P      Ù=     Ë=     ¬S2    =DÿhÖÓ € €            < P r o p e r t i e s `      Ú=     Ù=     à¬S2    ú’ÿhÖÓ                < A s s e m b l y I n f o . c s       `      Ú=     Ù=     @­S2    ú’ÿhÖÓ               < A s s e m b l y I n f o . c s       `      Ú=     Ù=      ­S2    ¶¹ÿhÖÓ               < A s s e m b l y I n f o . c s       `      Ú=     Ù=      ®S2    ¶¹ÿhÖÓ               < A s s e m b l y I n f o . c s       `      Ú=     Ù=     `®S2    ¶¹ÿhÖÓ €             < A s s e m b l y I n f o . c s       H      Û=     q=     À®S2    >/ iÖÓ               < S r c       H      Û=     q=     ¯S2    >/ iÖÓ  €            < S r c       H      Û=     q=     P¯S2    >/ iÖÓ €              < S r c       H      Û=     q=     ˜¯S2    >/ iÖÓ € €            < S r c                                       X      Ü=     Û=      °S2    } iÖÓ                < A l i g n e d N e w . h     X      Ü=     Û=     X°S2    } iÖÓ               < A l i g n e d N e w . h     X      Ü=     Û=     °°S2    } iÖÓ               < A l i g n e d N e w . h     X      Ü=     Û=     ±S2    } iÖÓ               < A l i g n e d N e w . h     X      Ü=     Û=     `±S2    } iÖÓ €             < A l i g n e d N e w . h     h      İ=     Û=     ¸±S2    èË iÖÓ               & < A l p h a T e s t E f f e c t . c p p     . h      İ=     Û=      ²S2    èË iÖÓ              & < A l p h a T e s t E f f e c t . c p p     . h      İ=     Û=     ˆ²S2    >iÖÓ              & < A l p h a T e s t E f f e c t . c p p     . h      İ=     Û=     ğ²S2    >iÖÓ              & < A l p h a T e s t E f f e c t . c p p     . h      İ=     Û=     X³S2    >iÖÓ €            & < A l p h a T e s t E f f e c t . c p p     . `      Ş=     Û=     À³S2    ğ¡iÖÓ                < B a s i c E f f e c t . c p p     i `      Ş=     Û=      ´S2    ğ¡iÖÓ               < B a s i c E f f e c t . c p p     i `      Ş=     Û=     €´S2    RÑiÖÓ               < B a s i c E f f e c t . c p p     i `      Ş=     Û=     à´S2    RÑiÖÓ               < B a s i c E f f e c t . c p p     i `      Ş=     Û=     @µS2    RÑiÖÓ €             < B a s i c E f f e c t . c p p     i h      ß=     Û=      µS2    1ğiÖÓ               ( < B a s i c P o s t P r o c e s s . c p p     h      ß=     Û=     ¶S2    1ğiÖÓ              ( < B a s i c P o s t P r o c e s s . c p p     h      ß=     Û=     p¶S2    ˆÜiÖÓ              ( < B a s i c P o s t P r o c e s s . c p p     h      ß=     Û=     Ø¶S2    ˆÜiÖÓ              ( < B a s i c P o s t P r o c e s s . c p p     h      ß=     Û=     @·S2    ˆÜiÖÓ €            ( < B a s i c P o s t P r o c e s s . c p p     P      à=     Û=     ¨·S2    gíiÖÓ                < B e z i e r . h     P      à=     Û=     ø·S2    gíiÖÓ               < B e z i e r . h     P      à=     Û=     H¸S2    gíiÖÓ               < B e z i e r . h     P      à=     Û=     ˜¸S2    gíiÖÓ               < B e z i e r . h     P      à=     Û=     è¸S2    gíiÖÓ €             < B e z i e r . h     `      á=     Û=     8¹S2    0=iÖÓ                 < B i n a r y R e a d e r . c p p     `      á=     Û=     ˜¹S2    0=iÖÓ                < B i n a r y R e a d e r . c p p     `      á=     Û=     ø¹S2    0=iÖÓ                < B i n a r y R e a d e r . c p p     `      á=     Û=     XºS2    0=iÖÓ                < B i n a r y R e a d e r . c p p     `      á=     Û=     ¸ºS2    0=iÖÓ €              < B i n a r y R e a d e r . c p p     X      â=     Û=     »S2    ŠiÖÓ                < B i n a r y R e a d e r . h X      â=     Û=     p»S2    ŠiÖÓ               < B i n a r y R e a d e r . h X      â=     Û=     È»S2    ŠiÖÓ               < B i n a r y R e a d e r . h X      â=     Û=      ¼S2    ŠiÖÓ               < B i n a r y R e a d e r . h X      â=     Û=     x¼S2    ŠiÖÓ €             < B i n a r y R e a d e r . h `      ã=     Û=     Ğ¼S2    [ØiÖÓ                 < C o m m o n S t a t e s . c p p     `      ã=     Û=     0½S2    [ØiÖÓ                < C o m m o n S t a t e s . c p p     `      ã=     Û=     ½S2    [ØiÖÓ                < C o m m o n S t a t e s . c p p     `      ã=     Û=     ğ½S2    [ØiÖÓ                < C o m m o n S t a t e s . c p p     `      ã=     Û=     P¾S2    ‚ÿiÖÓ €              < C o m m o n S t a t e s . c p p     `      ä=     Û=     °¾S2    ÛM	iÖÓ                 < C o n s t a n t B u f f e r . h     `      ä=     Û=     ¿S2    ÛM	iÖÓ                < C o n s t a n t B u f f e r . h     `      ä=     Û=     p¿S2    ÛM	iÖÓ                < C o n s t a n t B u f f e r . h                                                     `      ä=     Û=      ÀS2    ÛM	iÖÓ                < C o n s t a n t B u f f e r . h     `      ä=     Û=     `ÀS2    u	iÖÓ €              < C o n s t a n t B u f f e r . h     H      å=     Û=     ÀÀS2    _Ã	iÖÓ               
 < d d s . h   H      å=     Û=     ÁS2    _Ã	iÖÓ              
 < d d s . h   H      å=     Û=     PÁS2    ›h
iÖÓ              
 < d d s . h   H      å=     Û=     ˜ÁS2    ›h
iÖÓ              
 < d d s . h   H      å=     Û=     àÁS2    ›h
iÖÓ €            
 < d d s . h   h      æ=     Û=     (ÂS2    Ÿ®
iÖÓ               ( < D D S T e x t u r e L o a d e r . c p p     h      æ=     Û=     ÂS2    ’Õ
iÖÓ              ( < D D S T e x t u r e L o a d e r . c p p     h      æ=     Û=     øÂS2    ’Õ
iÖÓ              ( < D D S T e x t u r e L o a d e r . c p p     h      æ=     Û=     `ÃS2    ’Õ
iÖÓ              ( < D D S T e x t u r e L o a d e r . c p p     h      æ=     Û=     ÈÃS2    ’Õ
iÖÓ €            ( < D D S T e x t u r e L o a d e r . c p p     `      ç=     Û=     0ÄS2    Ü#iÖÓ                < D e b u g E f f e c t . c p p     I `      ç=     Û=     ÄS2    
KiÖÓ               < D e b u g E f f e c t . c p p     I `      ç=     Û=     ğÄS2    
KiÖÓ               < D e b u g E f f e c t . c p p     I `      ç=     Û=     PÅS2    
KiÖÓ               < D e b u g E f f e c t . c p p     I `      ç=     Û=     °ÅS2    
KiÖÓ €             < D e b u g E f f e c t . c p p     I X      è=     Û=     ÆS2    `™iÖÓ                < D e m a n d C r e a t e . h X      è=     Û=     hÆS2    `™iÖÓ               < D e m a n d C r e a t e . h X      è=     Û=     ÀÆS2    †ÀiÖÓ               < D e m a n d C r e a t e . h X      è=     Û=     ÇS2    †ÀiÖÓ               < D e m a n d C r e a t e . h X      è=     Û=     pÇS2    †ÀiÖÓ €             < D e m a n d C r e a t e . h X      é=     Û=     ÈÇS2    ÜiÖÓ                < D G S L E f f e c t . c p p X      é=     Û=      ÈS2    ÜiÖÓ               < D G S L E f f e c t . c p p X      é=     Û=     xÈS2    ¾¬iÖÓ               < D G S L E f f e c t . c p p X      é=     Û=     ĞÈS2    ¾¬iÖÓ               < D G S L E f f e c t . c p p X      é=     Û=     (ÉS2    ­ÒiÖÓ €             < D G S L E f f e c t . c p p h      ê=     Û=     €ÉS2    %iÖÓ               * < D G S L E f f e c t F a c t o r y . c p p   h      ê=     Û=     èÉS2    P)iÖÓ              * < D G S L E f f e c t F a c t o r y . c p p   h      ê=     Û=     PÊS2    ÿÅiÖÓ              * < D G S L E f f e c t F a c t o r y . c p p   h      ê=     Û=     ¸ÊS2    ÿÅiÖÓ              * < D G S L E f f e c t F a c t o r y . c p p   h      ê=     Û=      ËS2    ÿÅiÖÓ €            * < D G S L E f f e c t F a c t o r y . c p p   h      ë=     Û=     ˆËS2    XiÖÓ               & < D u a l P o s t P r o c e s s . c p p     L h      ë=     Û=     ğËS2    XiÖÓ              & < D u a l P o s t P r o c e s s . c p p     L h      ë=     Û=     XÌS2    MiÖÓ              & < D u a l P o s t P r o c e s s . c p p     L h      ë=     Û=     ÀÌS2    MiÖÓ              & < D u a l P o s t P r o c e s s . c p p     L h      ë=     Û=     (ÍS2    MiÖÓ €            & < D u a l P o s t P r o c e s s . c p p     L h      ì=     Û=     ÍS2    œiÖÓ               * < D u a l T e x t u r e E f f e c t . c p p   h      ì=     Û=     øÍS2    œiÖÓ              * < D u a l T e x t u r e E f f e c t . c p p   h      ì=     Û=     `ÎS2    ©8iÖÓ              * < D u a l T e x t u r e E f f e c t . c p p   h      ì=     Û=     ÈÎS2    ©8iÖÓ              * < D u a l T e x t u r e E f f e c t . c p p   h      ì=     Û=     0ÏS2    ©8iÖÓ €            * < D u a l T e x t u r e E f f e c t . c p p   `      í=     Û=     ˜ÏS2    ÷†iÖÓ                 < E f f e c t C o m m o n . c p p             `      í=     Û=      ĞS2    ÷†iÖÓ                < E f f e c t C o m m o n . c p p     `      í=     Û=     `ĞS2    ¤ÁiÖÓ                < E f f e c t C o m m o n . c p p     `      í=     Û=     ÀĞS2    ‡çiÖÓ                < E f f e c t C o m m o n . c p p     `      í=     Û=      ÑS2    ‡çiÖÓ €              < E f f e c t C o m m o n . c p p     X      î=     Û=     €ÑS2    JiÖÓ                < E f f e c t C o m m o n . h X      î=     Û=     ØÑS2    JiÖÓ               < E f f e c t C o m m o n . h X      î=     Û=     0ÒS2    ÃÇiÖÓ               < E f f e c t C o m m o n . h X      î=     Û=     ˆÒS2    ÃÇiÖÓ               < E f f e c t C o m m o n . h X      î=     Û=     àÒS2    ÃÇiÖÓ €             < E f f e c t C o m m o n . h `      ï=     Û=     8ÓS2    QiÖÓ               " < E f f e c t F a c t o r y . c p p   `      ï=     Û=     ˜ÓS2    ¹xiÖÓ              " < E f f e c t F a c t o r y . c p p   `      ï=     Û=     øÓS2    ¹xiÖÓ              " < E f f e c t F a c t o r y . c p p   `      ï=     Û=     XÔS2    KˆiÖÓ              " < E f f e c t F a c t o r y . c p p   `      ï=     Û=     ¸ÔS2    KˆiÖÓ €            " < E f f e c t F a c t o r y . c p p   p      ğ=     Û=     ÕS2    ì×iÖÓ               0 < E n v i r o n m e n t M a p E f f e c t . c p p     p      ğ=     Û=     ˆÕS2    ì×iÖÓ              0 < E n v i r o n m e n t M a p E f f e c t . c p p     p      ğ=     Û=     øÕS2    TİiÖÓ              0 < E n v i r o n m e n t M a p E f f e c t . c p p     p      ğ=     Û=     hÖS2    TİiÖÓ              0 < E n v i r o n m e n t M a p E f f e c t . c p p     p      ğ=     Û=     ØÖS2    ~iÖÓ €            0 < E n v i r o n m e n t M a p E f f e c t . c p p     X      ñ=     Û=     H×S2    è5iÖÓ                < G a m e P a d . c p p       X      ñ=     Û=      ×S2    è5iÖÓ               < G a m e P a d . c p p       X      ñ=     Û=     ø×S2    F€iÖÓ               < G a m e P a d . c p p       X      ñ=     Û=     PØS2    F€iÖÓ               < G a m e P a d . c p p       X      ñ=     Û=     ¨ØS2    F€iÖÓ €             < G a m e P a d . c p p       h      ò=     Û=      ÙS2    éiÖÓ               , < G e o m e t r i c P r i m i t i v e . c p p h      ò=     Û=     hÙS2    éiÖÓ              , < G e o m e t r i c P r i m i t i v e . c p p h      ò=     Û=     ĞÙS2    Z=iÖÓ              , < G e o m e t r i c P r i m i t i v e . c p p h      ò=     Û=     8ÚS2    Z=iÖÓ              , < G e o m e t r i c P r i m i t i v e . c p p h      ò=     Û=      ÚS2    Z=iÖÓ €            , < G e o m e t r i c P r i m i t i v e . c p p X      ó=     Û=     ÛS2    „‹iÖÓ                < G e o m e t r y . c p p     X      ó=     Û=     `ÛS2    „‹iÖÓ               < G e o m e t r y . c p p     X      ó=     Û=     ¸ÛS2    )ÆiÖÓ               < G e o m e t r y . c p p     X      ó=     Û=     ÜS2    )ÆiÖÓ               < G e o m e t r y . c p p     X      ó=     Û=     hÜS2    )ÆiÖÓ €             < G e o m e t r y . c p p     P      ô=     Û=     ÀÜS2    +iÖÓ                < G e o m e t r y . h P      ô=     Û=     İS2    +iÖÓ               < G e o m e t r y . h P      ô=     Û=     `İS2    R:iÖÓ               < G e o m e t r y . h P      ô=     Û=     °İS2    R:iÖÓ               < G e o m e t r y . h P      ô=     Û=      ŞS2    R:iÖÓ €             < G e o m e t r y . h `      õ=     Û=     PŞS2    ¯ˆiÖÓ               $ < G r a p h i c s M e m o r y . c p p `      õ=     Û=     °ŞS2    ¯ˆiÖÓ              $ < G r a p h i c s M e m o r y . c p p `      õ=     Û=     ßS2    V%iÖÓ              $ < G r a p h i c s M e m o r y . c p p `      õ=     Û=     pßS2    V%iÖÓ              $ < G r a p h i c s M e m o r y . c p p                                                 `      õ=     Û=      àS2    }LiÖÓ €            $ < G r a p h i c s M e m o r y . c p p X      ö=     Û=     `àS2    ÒšiÖÓ                < K e y b o a r d . c p p     X      ö=     Û=     ¸àS2    ÒšiÖÓ               < K e y b o a r d . c p p     X      ö=     Û=     áS2    *`iÖÓ               < K e y b o a r d . c p p     X      ö=     Û=     háS2    *`iÖÓ               < K e y b o a r d . c p p     X      ö=     Û=     ÀáS2    *`iÖÓ €             < K e y b o a r d . c p p     `      ÷=     Û=     âS2    Ş[iÖÓ                < L o a d e r H e l p e r s . h       `      ÷=     Û=     xâS2    Ş[iÖÓ               < L o a d e r H e l p e r s . h       `      ÷=     Û=     ØâS2    âF iÖÓ               < L o a d e r H e l p e r s . h       `      ÷=     Û=     8ãS2    âF iÖÓ               < L o a d e r H e l p e r s . h       `      ÷=     Û=     ˜ãS2    âF iÖÓ €             < L o a d e r H e l p e r s . h       P      ø=     Û=     øãS2    +• iÖÓ                < M o d e l . c p p   P      ø=     Û=     HäS2    Z¼ iÖÓ               < M o d e l . c p p   P      ø=     Û=     ˜äS2    Z¼ iÖÓ               < M o d e l . c p p   P      ø=     Û=     èäS2    Z¼ iÖÓ               < M o d e l . c p p   P      ø=     Û=     8åS2    Z¼ iÖÓ €             < M o d e l . c p p   `      ù=     Û=     ˆåS2    ¨
!iÖÓ                 < M o d e l L o a d C M O . c p p     `      ù=     Û=     èåS2    ¨
!iÖÓ                < M o d e l L o a d C M O . c p p     `      ù=     Û=     HæS2    ¨
!iÖÓ                < M o d e l L o a d C M O . c p p     `      ù=     Û=     ¨æS2    ¨
!iÖÓ                < M o d e l L o a d C M O . c p p     `      ù=     Û=     çS2    ¨
!iÖÓ €              < M o d e l L o a d C M O . c p p     h      ú=     Û=     hçS2    Y!iÖÓ               ( < M o d e l L o a d S D K M E S H . c p p     h      ú=     Û=     ĞçS2    Y!iÖÓ              ( < M o d e l L o a d S D K M E S H . c p p     h      ú=     Û=     8èS2    Ìx"iÖÓ              ( < M o d e l L o a d S D K M E S H . c p p     h      ú=     Û=      èS2    Ìx"iÖÓ              ( < M o d e l L o a d S D K M E S H . c p p     h      ú=     Û=     éS2    Æ“"iÖÓ €            ( < M o d e l L o a d S D K M E S H . c p p     `      û=     Û=     péS2    Sh$iÖÓ                 < M o d e l L o a d V B O . c p p     `      û=     Û=     ĞéS2    Sh$iÖÓ                < M o d e l L o a d V B O . c p p     `      û=     Û=     0êS2    1%iÖÓ                < M o d e l L o a d V B O . c p p     `      û=     Û=     êS2    1%iÖÓ                < M o d e l L o a d V B O . c p p     `      û=     Û=     ğêS2    1%iÖÓ €              < M o d e l L o a d V B O . c p p     P      ü=     Û=     PëS2    W>&iÖÓ                < M o u s e . c p p   P      ü=     Û=      ëS2    W>&iÖÓ               < M o u s e . c p p   P      ü=     Û=     ğëS2     =(iÖÓ               < M o u s e . c p p   P      ü=     Û=     @ìS2     =(iÖÓ               < M o u s e . c p p   P      ü=     Û=     ìS2     =(iÖÓ €             < M o u s e . c p p   h      ı=     Û=     àìS2    n‹(iÖÓ               & < N o r m a l M a p E f f e c t . c p p       h      ı=     Û=     HíS2    n‹(iÖÓ              & < N o r m a l M a p E f f e c t . c p p       h      ı=     Û=     °íS2    ”²(iÖÓ              & < N o r m a l M a p E f f e c t . c p p       h      ı=     Û=     îS2    ”²(iÖÓ              & < N o r m a l M a p E f f e c t . c p p       h      ı=     Û=     €îS2    ”²(iÖÓ €            & < N o r m a l M a p E f f e c t . c p p       X      ş=     Û=     èîS2    EÉ)iÖÓ                < P B R E f f e c t . c p p   X      ş=     Û=     @ïS2    EÉ)iÖÓ               < P B R E f f e c t . c p p   X      ş=     Û=     ˜ïS2    TG-iÖÓ               < P B R E f f e c t . c p p                   X      ş=     Û=      ğS2    TG-iÖÓ               < P B R E f f e c t . c p p   X      ş=     Û=     XğS2    TG-iÖÓ €             < P B R E f f e c t . c p p   P      ÿ=     Û=     °ğS2    {.iÖÓ                < p c h . c p p       P      ÿ=     Û=      ñS2    {.iÖÓ               < p c h . c p p       P      ÿ=     Û=     PñS2    {.iÖÓ               < p c h . c p p       P      ÿ=     Û=      ñS2    cŸ.iÖÓ               < p c h . c p p       P      ÿ=     Û=     ğñS2    cŸ.iÖÓ €             < p c h . c p p       H       >     Û=     @òS2    ±í.iÖÓ               
 < p c h . h   H       >     Û=     ˆòS2    Û/iÖÓ              
 < p c h . h   H       >     Û=     ĞòS2    Û/iÖÓ              
 < p c h . h   H       >     Û=     óS2    Û/iÖÓ              
 < p c h . h   H       >     Û=     `óS2    Û/iÖÓ €            
 < p c h . h   `      >     Û=     ¨óS2    Y'0iÖÓ               " < P l a t f o r m H e l p e r s . h   `      >     Û=     ôS2    ÜK0iÖÓ              " < P l a t f o r m H e l p e r s . h   `      >     Û=     hôS2    †1iÖÓ              " < P l a t f o r m H e l p e r s . h   `      >     Û=     ÈôS2    P¬1iÖÓ              " < P l a t f o r m H e l p e r s . h   `      >     Û=     (õS2    P¬1iÖÓ €            " < P l a t f o r m H e l p e r s . h   `      >     Û=     ˆõS2    ı12iÖÓ               $ < P r i m i t i v e B a t c h . c p p `      >     Û=     èõS2    ı12iÖÓ              $ < P r i m i t i v e B a t c h . c p p `      >     Û=     HöS2    p3iÖÓ              $ < P r i m i t i v e B a t c h . c p p `      >     Û=     ¨öS2    p3iÖÓ              $ < P r i m i t i v e B a t c h . c p p `      >     Û=     ÷S2    p3iÖÓ €            $ < P r i m i t i v e B a t c h . c p p X      >     Û=     h÷S2    }’3iÖÓ                < S c r e e n G r a b . c p p X      >     Û=     À÷S2    }’3iÖÓ               < S c r e e n G r a b . c p p X      >     Û=     øS2    ‚(5iÖÓ               < S c r e e n G r a b . c p p X      >     Û=     pøS2    ‚(5iÖÓ               < S c r e e n G r a b . c p p X      >     Û=     ÈøS2    ‚(5iÖÓ €             < S c r e e n G r a b . c p p P      >     Û=      ùS2    •‡5iÖÓ                < S D K M e s h . h   P      >     Û=     pùS2    •‡5iÖÓ               < S D K M e s h . h   P      >     Û=     ÀùS2    3Â6iÖÓ               < S D K M e s h . h   P      >     Û=     úS2    3Â6iÖÓ               < S D K M e s h . h   P      >     Û=     `úS2    3Â6iÖÓ €             < S D K M e s h . h   h      >     Û=     °úS2    @7iÖÓ               ( < S h a r e d R e s o u r c e P o o l . h     h      >     Û=     ûS2    c67iÖÓ              ( < S h a r e d R e s o u r c e P o o l . h     h      >     Û=     €ûS2    ÖI8iÖÓ              ( < S h a r e d R e s o u r c e P o o l . h     h      >     Û=     èûS2    ÖI8iÖÓ              ( < S h a r e d R e s o u r c e P o o l . h     h      >     Û=     PüS2    ÖI8iÖÓ €            ( < S h a r e d R e s o u r c e P o o l . h     X      >     Û=     ¸üS2    h\:iÖÓ                < S i m p l e M a t h . c p p X      >     Û=     ıS2    h\:iÖÓ               < S i m p l e M a t h . c p p X      >     Û=     hıS2    h\:iÖÓ               < S i m p l e M a t h . c p p X      >     Û=     ÀıS2    h\:iÖÓ               < S i m p l e M a t h . c p p X      >     Û=     şS2    h\:iÖÓ €             < S i m p l e M a t h . c p p `      >     Û=     pşS2    Ö«:iÖÓ               " < S k i n n e d E f f e c t . c p p   `      >     Û=     ĞşS2    Ö«:iÖÓ              " < S k i n n e d E f f e c t . c p p   `      >     Û=     0ÿS2    =Ô:iÖÓ              " < S k i n n e d E f f e c t . c p p   `      >     Û=     ÿS2    ®İ:iÖÓ              " < S k i n n e d E f f e c t . c p p                   `      >     Û=       T2    ®İ:iÖÓ €            " < S k i n n e d E f f e c t . c p p   `      >     Û=     ` T2    2S;iÖÓ                < S p r i t e B a t c h . c p p     g `      >     Û=     À T2    2S;iÖÓ               < S p r i t e B a t c h . c p p     g `      >     Û=      T2    2S;iÖÓ               < S p r i t e B a t c h . c p p     g `      >     Û=     €T2    2S;iÖÓ               < S p r i t e B a t c h . c p p     g `      >     Û=     àT2    2S;iÖÓ €             < S p r i t e B a t c h . c p p     g X      	>     Û=     @T2    ÊÈ;iÖÓ                < S p r i t e F o n t . c p p X      	>     Û=     ˜T2    ÊÈ;iÖÓ               < S p r i t e F o n t . c p p X      	>     Û=     ğT2    ÊÈ;iÖÓ               < S p r i t e F o n t . c p p X      	>     Û=     HT2    «ç;iÖÓ               < S p r i t e F o n t . c p p X      	>     Û=      T2    «ç;iÖÓ €             < S p r i t e F o n t . c p p X      
>     Û=     øT2    Ö<iÖÓ                < T e a p o t D a t a . i n c X      
>     Û=     PT2    Ö<iÖÓ               < T e a p o t D a t a . i n c X      
>     Û=     ¨T2    ù5<iÖÓ               < T e a p o t D a t a . i n c X      
>     Û=      T2    7<iÖÓ               < T e a p o t D a t a . i n c X      
>     Û=     XT2    7<iÖÓ €             < T e a p o t D a t a . i n c h      >     Û=     °T2    ‰k<iÖÓ               , < T o n e M a p P o s t P r o c e s s . c p p h      >     Û=     T2    ¸’<iÖÓ              , < T o n e M a p P o s t P r o c e s s . c p p h      >     Û=     €T2    ¸’<iÖÓ              , < T o n e M a p P o s t P r o c e s s . c p p h      >     Û=     èT2    ¸’<iÖÓ              , < T o n e M a p P o s t P r o c e s s . c p p h      >     Û=     PT2    ¸’<iÖÓ €            , < T o n e M a p P o s t P r o c e s s . c p p H      >     Û=     ¸T2    á<iÖÓ               
 < v b o . h   H      >     Û=      T2    á<iÖÓ              
 < v b o . h   H      >     Û=     HT2    á<iÖÓ              
 < v b o . h   H      >     Û=     T2    8=iÖÓ              
 < v b o . h   H      >     Û=     ØT2    8=iÖÓ €            
 < v b o . h   `      >     Û=      	T2    _/=iÖÓ                < V e r t e x T y p e s . c p p       `      >     Û=     €	T2    _/=iÖÓ               < V e r t e x T y p e s . c p p       `      >     Û=     à	T2    ‰V=iÖÓ               < V e r t e x T y p e s . c p p       `      >     Û=     @
T2    ‰V=iÖÓ               < V e r t e x T y p e s . c p p       `      >     Û=      
T2    ‰V=iÖÓ €             < V e r t e x T y p e s . c p p       h      >     Û=      T2    *¥=iÖÓ               ( < W I C T e x t u r e L o a d e r . c p p     h      >     Û=     hT2    *¥=iÖÓ              ( < W I C T e x t u r e L o a d e r . c p p     h      >     Û=     ĞT2    *¥=iÖÓ              ( < W I C T e x t u r e L o a d e r . c p p     h      >     Û=     8T2    *¥=iÖÓ              ( < W I C T e x t u r e L o a d e r . c p p     h      >     Û=      T2    *¥=iÖÓ €            ( < W I C T e x t u r e L o a d e r . c p p     p      >     Û=     T2    <ó=iÖÓ               0 < X b o x D D S T e x t u r e L o a d e r . c p p     p      >     Û=     xT2    <ó=iÖÓ              0 < X b o x D D S T e x t u r e L o a d e r . c p p     p      >     Û=     èT2    <ó=iÖÓ              0 < X b o x D D S T e x t u r e L o a d e r . c p p     p      >     Û=     XT2    [>iÖÓ              0 < X b o x D D S T e x t u r e L o a d e r . c p p     p      >     Û=     ÈT2    [>iÖÓ €            0 < X b o x D D S T e x t u r e L o a d e r . c p p     P      >     Û=     8T2    ‰A>iÖÓ               < S h a d e r s       P      >     Û=     ˆT2    ‰A>iÖÓ  €            < S h a d e r s                                               P      >     Û=      T2    °h>iÖÓ €              < S h a d e r s       P      >     Û=     PT2    °h>iÖÓ € €            < S h a d e r s       `      >     >      T2    ß>iÖÓ               $ < A l p h a T e s t E f f e c t . f x `      >     >      T2    ß>iÖÓ              $ < A l p h a T e s t E f f e c t . f x `      >     >     `T2    ·>iÖÓ              $ < A l p h a T e s t E f f e c t . f x `      >     >     ÀT2    ·>iÖÓ              $ < A l p h a T e s t E f f e c t . f x `      >     >      T2    ·>iÖÓ €            $ < A l p h a T e s t E f f e c t . f x X      >     >     €T2    4Ş>iÖÓ                < B a s i c E f f e c t . f x X      >     >     ØT2    4Ş>iÖÓ               < B a s i c E f f e c t . f x X      >     >     0T2    4Ş>iÖÓ               < B a s i c E f f e c t . f x X      >     >     ˆT2    ~?iÖÓ               < B a s i c E f f e c t . f x X      >     >     àT2    ~?iÖÓ €             < B a s i c E f f e c t . f x P      >     >     8T2    ßz?iÖÓ                < C o m m o n . f x h P      >     >     ˆT2    ßz?iÖÓ               < C o m m o n . f x h P      >     >     ØT2    »ˆ?iÖÓ               < C o m m o n . f x h P      >     >     (T2    »ˆ?iÖÓ               < C o m m o n . f x h P      >     >     xT2    »ˆ?iÖÓ €             < C o m m o n . f x h `      >     >     ÈT2    ×?iÖÓ               $ < C o m p i l e S h a d e r s . c m d `      >     >     (T2    ×?iÖÓ              $ < C o m p i l e S h a d e r s . c m d `      >     >     ˆT2    Ç†AiÖÓ              $ < C o m p i l e S h a d e r s . c m d `      >     >     èT2    Ç†AiÖÓ              $ < C o m p i l e S h a d e r s . c m d `      >     >     HT2    Ç†AiÖÓ €            $ < C o m p i l e S h a d e r s . c m d X      >     >     ¨T2    ç×AiÖÓ                < D e b u g E f f e c t . f x X      >     >      T2    ç×AiÖÓ               < D e b u g E f f e c t . f x X      >     >     XT2    ú9CiÖÓ               < D e b u g E f f e c t . f x X      >     >     °T2    _CiÖÓ               < D e b u g E f f e c t . f x X      >     >     T2    _CiÖÓ €             < D e b u g E f f e c t . f x X      >     >     `T2    /‡CiÖÓ                < D G S L E f f e c t . f x   X      >     >     ¸T2    /‡CiÖÓ               < D G S L E f f e c t . f x   X      >     >     T2    LDiÖÓ               < D G S L E f f e c t . f x   X      >     >     hT2    LDiÖÓ               < D G S L E f f e c t . f x   X      >     >     ÀT2    LDiÖÓ €             < D G S L E f f e c t . f x   `      >     >     T2    uÔDiÖÓ                 < D G S L L a m b e r t . h l s l     `      >     >     xT2    uÔDiÖÓ                < D G S L L a m b e r t . h l s l     `      >     >     ØT2    uÔDiÖÓ                < D G S L L a m b e r t . h l s l     `      >     >     8T2    uÔDiÖÓ                < D G S L L a m b e r t . h l s l     `      >     >     ˜T2    µşDiÖÓ €              < D G S L L a m b e r t . h l s l     X      >     >     øT2    š&EiÖÓ                < D G S L P h o n g . h l s l X      >     >     PT2    MEiÖÓ               < D G S L P h o n g . h l s l X      >     >     ¨T2    MEiÖÓ               < D G S L P h o n g . h l s l X      >     >      T2    MEiÖÓ               < D G S L P h o n g . h l s l X      >     >     XT2    MEiÖÓ €             < D G S L P h o n g . h l s l X      >     >     °T2    ‡†EiÖÓ                < D G S L U n l i t . h l s l X      >     >     T2    ‡†EiÖÓ               < D G S L U n l i t . h l s l X      >     >     `T2    ‡†EiÖÓ               < D G S L U n l i t . h l s l                                                                         X      >     >       T2    z£EiÖÓ               < D G S L U n l i t . h l s l X      >     >     X T2    z£EiÖÓ €             < D G S L U n l i t . h l s l h      >     >     ° T2    AÎEiÖÓ               ( < D u a l T e x t u r e E f f e c t . f x     h      >     >     !T2    AÎEiÖÓ              ( < D u a l T e x t u r e E f f e c t . f x     h      >     >     €!T2    AÎEiÖÓ              ( < D u a l T e x t u r e E f f e c t . f x     h      >     >     è!T2    AÎEiÖÓ              ( < D u a l T e x t u r e E f f e c t . f x     h      >     >     P"T2    põEiÖÓ €            ( < D u a l T e x t u r e E f f e c t . f x     p      >     >     ¸"T2    "FiÖÓ               . < E n v i r o n m e n t M a p E f f e c t . f x       p      >     >     (#T2    "FiÖÓ              . < E n v i r o n m e n t M a p E f f e c t . f x       p      >     >     ˜#T2    "FiÖÓ              . < E n v i r o n m e n t M a p E f f e c t . f x       p      >     >     $T2    "FiÖÓ              . < E n v i r o n m e n t M a p E f f e c t . f x       p      >     >     x$T2    "FiÖÓ €            . < E n v i r o n m e n t M a p E f f e c t . f x       X      >     >     è$T2    SpFiÖÓ                < L i g h t i n g . f x h     X      >     >     @%T2    SpFiÖÓ               < L i g h t i n g . f x h     X      >     >     ˜%T2    ÇFiÖÓ               < L i g h t i n g . f x h     X      >     >     ğ%T2    ÇFiÖÓ               < L i g h t i n g . f x h     X      >     >     H&T2    ÇFiÖÓ €             < L i g h t i n g . f x h     `      >     >      &T2    À˜IiÖÓ               $ < N o r m a l M a p E f f e c t . f x `      >     >      'T2    À˜IiÖÓ              $ < N o r m a l M a p E f f e c t . f x `      >     >     `'T2    À˜IiÖÓ              $ < N o r m a l M a p E f f e c t . f x `      >     >     À'T2    À˜IiÖÓ              $ < N o r m a l M a p E f f e c t . f x `      >     >      (T2    À˜IiÖÓ €            $ < N o r m a l M a p E f f e c t . f x X      >     >     €(T2    çIiÖÓ                < P B R C o m m o n . f x h   X      >     >     Ø(T2    çIiÖÓ               < P B R C o m m o n . f x h   X      >     >     0)T2    çIiÖÓ               < P B R C o m m o n . f x h   X      >     >     ˆ)T2    çIiÖÓ               < P B R C o m m o n . f x h   X      >     >     à)T2    8JiÖÓ €             < P B R C o m m o n . f x h   X      >     >     8*T2    .8JiÖÓ                < P B R E f f e c t . f x     X      >     >     *T2    .8JiÖÓ               < P B R E f f e c t . f x     X      >     >     è*T2    .8JiÖÓ               < P B R E f f e c t . f x     X      >     >     @+T2    .8JiÖÓ               < P B R E f f e c t . f x     X      >     >     ˜+T2    U_JiÖÓ €             < P B R E f f e c t . f x     x       >     >     ğ+T2    H‹JiÖÓ               6 < P i x e l P a c k i n g _ V e l o c i t y . h l s l i     \ x       >     >     h,T2    H‹JiÖÓ              6 < P i x e l P a c k i n g _ V e l o c i t y . h l s l i     \ x       >     >     à,T2    RLiÖÓ              6 < P i x e l P a c k i n g _ V e l o c i t y . h l s l i     \ x       >     >     X-T2    RLiÖÓ              6 < P i x e l P a c k i n g _ V e l o c i t y . h l s l i     \ x       >     >     Ğ-T2    RLiÖÓ €            6 < P i x e l P a c k i n g _ V e l o c i t y . h l s l i     \ X      !>     >     H.T2    `íLiÖÓ                < P o s t P r o c e s s . f x X      !>     >      .T2    `íLiÖÓ               < P o s t P r o c e s s . f x X      !>     >     ø.T2    `íLiÖÓ               < P o s t P r o c e s s . f x X      !>     >     P/T2    `íLiÖÓ               < P o s t P r o c e s s . f x X      !>     >     ¨/T2    `íLiÖÓ €             < P o s t P r o c e s s . f x `      ">     >      0T2    ¹;MiÖÓ                 < S k i n n e d E f f e c t . f x     `      ">     >     `0T2    ¹;MiÖÓ                < S k i n n e d E f f e c t . f x     `      ">     >     À0T2    ¹;MiÖÓ                < S k i n n e d E f f e c t . f x     `      ">     >      1T2    fcMiÖÓ                < S k i n n e d E f f e c t . f x     `      ">     >     €1T2    fcMiÖÓ €              < S k i n n e d E f f e c t . f x     `      #>     >     à1T2    ŠMiÖÓ                < S p r i t e E f f e c t . f x     I `      #>     >     @2T2    ŠMiÖÓ               < S p r i t e E f f e c t . f x     I `      #>     >      2T2    —ÇNiÖÓ               < S p r i t e E f f e c t . f x     I `      #>     >      3T2    —ÇNiÖÓ               < S p r i t e E f f e c t . f x     I `      #>     >     `3T2    ZíNiÖÓ €             < S p r i t e E f f e c t . f x     I X      $>     >     À3T2    "AOiÖÓ                < S t r u c t u r e s . f x h X      $>     >     4T2    "AOiÖÓ               < S t r u c t u r e s . f x h X      $>     >     p4T2    "AOiÖÓ               < S t r u c t u r e s . f x h X      $>     >     È4T2    "AOiÖÓ               < S t r u c t u r e s . f x h X      $>     >      5T2    "AOiÖÓ €             < S t r u c t u r e s . f x h P      %>     >     x5T2    {OiÖÓ                < T o n e M a p . f x P      %>     >     È5T2    {OiÖÓ               < T o n e M a p . f x P      %>     >     6T2    {OiÖÓ               < T o n e M a p . f x P      %>     >     h6T2    {OiÖÓ               < T o n e M a p . f x P      %>     >     ¸6T2    {OiÖÓ €             < T o n e M a p . f x X      &>     >     7T2    ÔİOiÖÓ                < U t i l i t i e s . f x h   X      &>     >     `7T2    ÔİOiÖÓ               < U t i l i t i e s . f x h   X      &>     >     ¸7T2    [ñPiÖÓ               < U t i l i t i e s . f x h   X      &>     >     8T2    [ñPiÖÓ               < U t i l i t i e s . f x h   X      &>     >     h8T2    [ñPiÖÓ €             < U t i l i t i e s . f x h   P      '>     >     À8T2    ûÚQiÖÓ               < C o m p i l e d     P      '>     >     9T2    ûÚQiÖÓ  €            < C o m p i l e d     P      '>     >     `9T2    BRiÖÓ €              < C o m p i l e d     P      '>     >     °9T2    BRiÖÓ € €            < C o m p i l e d     ˆ      (>     '>      :T2    URiÖÓ               F < A l p h a T e s t E f f e c t _ P S A l p h a T e s t E q N e . i n c     c ˆ      (>     '>     ˆ:T2    URiÖÓ              F < A l p h a T e s t E f f e c t _ P S A l p h a T e s t E q N e . i n c     c ˆ      (>     '>     ;T2    URiÖÓ              F < A l p h a T e s t E f f e c t _ P S A l p h a T e s t E q N e . i n c     c ˆ      (>     '>     ˜;T2    URiÖÓ              F < A l p h a T e s t E f f e c t _ P S A l p h a T e s t E q N e . i n c     c ˆ      (>     '>      <T2    URiÖÓ €            F < A l p h a T e s t E f f e c t _ P S A l p h a T e s t E q N e . i n c     c       )>     '>     ¨<T2    ÜßRiÖÓ               P < A l p h a T e s t E f f e c t _ P S A l p h a T e s t E q N e N o F o g . i n c           )>     '>     8=T2    ÜßRiÖÓ              P < A l p h a T e s t E f f e c t _ P S A l p h a T e s t E q N e N o F o g . i n c           )>     '>     È=T2    ÜßRiÖÓ              P < A l p h a T e s t E f f e c t _ P S A l p h a T e s t E q N e N o F o g . i n c           )>     '>     X>T2    ÿSiÖÓ              P < A l p h a T e s t E f f e c t _ P S A l p h a T e s t E q N e N o F o g . i n c           )>     '>     è>T2    ÿSiÖÓ €            P < A l p h a T e s t E f f e c t _ P S A l p h a T e s t E q N e N o F o g . i n c     ˆ      *>     '>     x?T2    –ŠSiÖÓ               F < A l p h a T e s t E f f e c t _ P S A l p h a T e s t L t G t . i n c     c ˆ      *>     '>      @T2    –ŠSiÖÓ              F < A l p h a T e s t E f f e c t _ P S A l p h a T e s t L t G t . i n c     c ˆ      *>     '>     ˆ@T2    _ÚSiÖÓ              F < A l p h a T e s t E f f e c t _ P S A l p h a T e s t L t G t . i n c     c ˆ      *>     '>     AT2    _ÚSiÖÓ              F < A l p h a T e s t E f f e c t _ P S A l p h a T e s t L t G t . i n c     c ˆ      *>     '>     ˜AT2    _ÚSiÖÓ €            F < A l p h a T e s t E f f e c t _ P S A l p h a T e s t L t G t . i n c     c       +>     '>      BT2    XUiÖÓ               P < A l p h a T e s t E f f e c t _ P S A l p h a T e s t L t G t N o F o g . i n c           +>     '>     °BT2    8UiÖÓ              P < A l p h a T e s t E f f e c t _ P S A l p h a T e s t L t G t N o F o g . i n c           +>     '>     @CT2    ³’ViÖÓ              P < A l p h a T e s t E f f e c t _ P S A l p h a T e s t L t G t N o F o g . i n c           +>     '>     ĞCT2    ³’ViÖÓ              P < A l p h a T e s t E f f e c t _ P S A l p h a T e s t L t G t N o F o g . i n c           +>     '>     `DT2    ³’ViÖÓ €            P < A l p h a T e s t E f f e c t _ P S A l p h a T e s t L t G t N o F o g . i n c     €      ,>     '>     ğDT2    .WiÖÓ               > < A l p h a T e s t E f f e c t _ V S A l p h a T e s t . i n c       €      ,>     '>     pET2    .WiÖÓ              > < A l p h a T e s t E f f e c t _ V S A l p h a T e s t . i n c       €      ,>     '>     ğET2    ŒAXiÖÓ              > < A l p h a T e s t E f f e c t _ V S A l p h a T e s t . i n c       €      ,>     '>     pFT2    ŒAXiÖÓ              > < A l p h a T e s t E f f e c t _ V S A l p h a T e s t . i n c       €      ,>     '>     ğFT2    _gXiÖÓ €            > < A l p h a T e s t E f f e c t _ V S A l p h a T e s t . i n c       ˆ      ->     '>     pGT2    œÃXiÖÓ               H < A l p h a T e s t E f f e c t _ V S A l p h a T e s t N o F o g . i n c     ˆ      ->     '>     øGT2    œÃXiÖÓ              H < A l p h a T e s t E f f e c t _ V S A l p h a T e s t N o F o g . i n c     ˆ      ->     '>     €HT2    RğYiÖÓ              H < A l p h a T e s t E f f e c t _ V S A l p h a T e s t N o F o g . i n c     ˆ      ->     '>     IT2    RğYiÖÓ              H < A l p h a T e s t E f f e c t _ V S A l p h a T e s t N o F o g . i n c     ˆ      ->     '>     IT2    RğYiÖÓ €            H < A l p h a T e s t E f f e c t _ V S A l p h a T e s t N o F o g . i n c     €      .>     '>     JT2    Õ¥ZiÖÓ               B < A l p h a T e s t E f f e c t _ V S A l p h a T e s t V c . i n c   €      .>     '>     ˜JT2    Õ¥ZiÖÓ              B < A l p h a T e s t E f f e c t _ V S A l p h a T e s t V c . i n c   €      .>     '>     KT2    [iÖÓ              B < A l p h a T e s t E f f e c t _ V S A l p h a T e s t V c . i n c   €      .>     '>     ˜KT2    [iÖÓ              B < A l p h a T e s t E f f e c t _ V S A l p h a T e s t V c . i n c   €      .>     '>     LT2    [iÖÓ €            B < A l p h a T e s t E f f e c t _ V S A l p h a T e s t V c . i n c   ˆ      />     '>     ˜LT2    ¼[iÖÓ               L < A l p h a T e s t E f f e c t _ V S A l p h a T e s t V c N o F o g . i n c ˆ      />     '>      MT2    ¼[iÖÓ              L < A l p h a T e s t E f f e c t _ V S A l p h a T e s t V c N o F o g . i n c ˆ      />     '>     ¨MT2    1¦[iÖÓ              L < A l p h a T e s t E f f e c t _ V S A l p h a T e s t V c N o F o g . i n c ˆ      />     '>     0NT2    1¦[iÖÓ              L < A l p h a T e s t E f f e c t _ V S A l p h a T e s t V c N o F o g . i n c ˆ      />     '>     ¸NT2    TÍ[iÖÓ €            L < A l p h a T e s t E f f e c t _ V S A l p h a T e s t V c N o F o g . i n c p      0>     '>     @OT2    nW\iÖÓ               . < B a s i c E f f e c t _ P S B a s i c . i n c                                                                                       p      0>     '>      PT2    nW\iÖÓ              . < B a s i c E f f e c t _ P S B a s i c . i n c       p      0>     '>     pPT2    nW\iÖÓ              . < B a s i c E f f e c t _ P S B a s i c . i n c       p      0>     '>     àPT2    nW\iÖÓ              . < B a s i c E f f e c t _ P S B a s i c . i n c       p      0>     '>     PQT2    nW\iÖÓ €            . < B a s i c E f f e c t _ P S B a s i c . i n c       x      1>     '>     ÀQT2    Ç¥\iÖÓ               8 < B a s i c E f f e c t _ P S B a s i c N o F o g . i n c     x      1>     '>     8RT2    Ç¥\iÖÓ              8 < B a s i c E f f e c t _ P S B a s i c N o F o g . i n c     x      1>     '>     °RT2     ô\iÖÓ              8 < B a s i c E f f e c t _ P S B a s i c N o F o g . i n c     x      1>     '>     (ST2     ô\iÖÓ              8 < B a s i c E f f e c t _ P S B a s i c N o F o g . i n c     x      1>     '>      ST2    G]iÖÓ €            8 < B a s i c E f f e c t _ P S B a s i c N o F o g . i n c     ˆ      2>     '>     TT2    nB]iÖÓ               H < B a s i c E f f e c t _ P S B a s i c P i x e l L i g h t i n g . i n c     ˆ      2>     '>      TT2    nB]iÖÓ              H < B a s i c E f f e c t _ P S B a s i c P i x e l L i g h t i n g . i n c     ˆ      2>     '>     (UT2    {Ê^iÖÓ              H < B a s i c E f f e c t _ P S B a s i c P i x e l L i g h t i n g . i n c     ˆ      2>     '>     °UT2    {Ê^iÖÓ              H < B a s i c E f f e c t _ P S B a s i c P i x e l L i g h t i n g . i n c     ˆ      2>     '>     8VT2    {Ê^iÖÓ €            H < B a s i c E f f e c t _ P S B a s i c P i x e l L i g h t i n g . i n c     ˆ      3>     '>     ÀVT2    v_iÖÓ               L < B a s i c E f f e c t _ P S B a s i c P i x e l L i g h t i n g T x . i n c ˆ      3>     '>     HWT2    v_iÖÓ              L < B a s i c E f f e c t _ P S B a s i c P i x e l L i g h t i n g T x . i n c ˆ      3>     '>     ĞWT2    “İ_iÖÓ              L < B a s i c E f f e c t _ P S B a s i c P i x e l L i g h t i n g T x . i n c ˆ      3>     '>     XXT2    “İ_iÖÓ              L < B a s i c E f f e c t _ P S B a s i c P i x e l L i g h t i n g T x . i n c ˆ      3>     '>     àXT2    “İ_iÖÓ €            L < B a s i c E f f e c t _ P S B a s i c P i x e l L i g h t i n g T x . i n c p      4>     '>     hYT2    haiÖÓ               2 < B a s i c E f f e c t _ P S B a s i c T x . i n c   p      4>     '>     ØYT2    haiÖÓ              2 < B a s i c E f f e c t _ P S B a s i c T x . i n c   p      4>     '>     HZT2    4”aiÖÓ              2 < B a s i c E f f e c t _ P S B a s i c T x . i n c   p      4>     '>     ¸ZT2    4”aiÖÓ              2 < B a s i c E f f e c t _ P S B a s i c T x . i n c   p      4>     '>     ([T2    4”aiÖÓ €            2 < B a s i c E f f e c t _ P S B a s i c T x . i n c   x      5>     '>     ˜[T2    ÍbiÖÓ               < < B a s i c E f f e c t _ P S B a s i c T x N o F o g . i n c x      5>     '>     \T2    ÍbiÖÓ              < < B a s i c E f f e c t _ P S B a s i c T x N o F o g . i n c x      5>     '>     ˆ\T2    5ôbiÖÓ              < < B a s i c E f f e c t _ P S B a s i c T x N o F o g . i n c x      5>     '>      ]T2    5ôbiÖÓ              < < B a s i c E f f e c t _ P S B a s i c T x N o F o g . i n c x      5>     '>     x]T2    5ôbiÖÓ €            < < B a s i c E f f e c t _ P S B a s i c T x N o F o g . i n c ˆ      6>     '>     ğ]T2    hciÖÓ               J < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g . i n c   ˆ      6>     '>     x^T2    hciÖÓ              J < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g . i n c   ˆ      6>     '>      _T2    Œ™ciÖÓ              J < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g . i n c                                                                                                                           ˆ      6>     '>      `T2    Œ™ciÖÓ              J < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g . i n c   ˆ      6>     '>     ˆ`T2    ÁciÖÓ €            J < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g . i n c         7>     '>     aT2    ĞóciÖÓ               T < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g N o F o g . i n c       7>     '>      aT2    ĞóciÖÓ              T < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g N o F o g . i n c       7>     '>     0bT2    IdiÖÓ              T < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g N o F o g . i n c       7>     '>     ÀbT2    IdiÖÓ              T < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g N o F o g . i n c       7>     '>     PcT2    IdiÖÓ €            T < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g N o F o g . i n c       8>     '>     àcT2    A"fiÖÓ               N < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g T x . i n c             8>     '>     pdT2    A"fiÖÓ              N < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g T x . i n c             8>     '>      eT2    gIfiÖÓ              N < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g T x . i n c             8>     '>     eT2    gIfiÖÓ              N < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g T x . i n c             8>     '>      fT2    gIfiÖÓ €            N < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g T x . i n c       ˜      9>     '>     °fT2    Á—fiÖÓ               X < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g T x N o F o g . i n c     ˜      9>     '>     HgT2    Á—fiÖÓ              X < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g T x N o F o g . i n c     ˜      9>     '>     àgT2    ˜¤fiÖÓ              X < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g T x N o F o g . i n c     ˜      9>     '>     xhT2    ˜¤fiÖÓ              X < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g T x N o F o g . i n c     ˜      9>     '>     iT2    ˜¤fiÖÓ €            X < B a s i c E f f e c t _ P S B a s i c V e r t e x L i g h t i n g T x N o F o g . i n c     p      :>     '>     ¨iT2    giÖÓ               . < B a s i c E f f e c t _ V S B a s i c . i n c       p      :>     '>     jT2    giÖÓ              . < B a s i c E f f e c t _ V S B a s i c . i n c       p      :>     '>     ˆjT2    XñhiÖÓ              . < B a s i c E f f e c t _ V S B a s i c . i n c       p      :>     '>     øjT2    XñhiÖÓ              . < B a s i c E f f e c t _ V S B a s i c . i n c       p      :>     '>     hkT2    XñhiÖÓ €            . < B a s i c E f f e c t _ V S B a s i c . i n c       x      ;>     '>     ØkT2    m>iiÖÓ               8 < B a s i c E f f e c t _ V S B a s i c N o F o g . i n c     x      ;>     '>     PlT2    m>iiÖÓ              8 < B a s i c E f f e c t _ V S B a s i c N o F o g . i n c     x      ;>     '>     ÈlT2     £jiÖÓ              8 < B a s i c E f f e c t _ V S B a s i c N o F o g . i n c     x      ;>     '>     @mT2     £jiÖÓ              8 < B a s i c E f f e c t _ V S B a s i c N o F o g . i n c     x      ;>     '>     ¸mT2     £jiÖÓ €            8 < B a s i c E f f e c t _ V S B a s i c N o F o g . i n c     €      <>     '>     0nT2    ˆkiÖÓ               > < B a s i c E f f e c t _ V S B a s i c O n e L i g h t . i n c       €      <>     '>     °nT2    ˆkiÖÓ              > < B a s i c E f f e c t _ V S B a s i c O n e L i g h t . i n c       €      <>     '>     0oT2    á‚liÖÓ              > < B a s i c E f f e c t _ V S B a s i c O n e L i g h t . i n c                                                                                       €      <>     '>      pT2    ‹¬liÖÓ              > < B a s i c E f f e c t _ V S B a s i c O n e L i g h t . i n c       €      <>     '>     €pT2    ‹¬liÖÓ €            > < B a s i c E f f e c t _ V S B a s i c O n e L i g h t . i n c       €      =>     '>      qT2    ²©niÖÓ               B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t B n . i n c   €      =>     '>     €qT2    ²©niÖÓ              B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t B n . i n c   €      =>     '>      rT2    ÙĞniÖÓ              B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t B n . i n c   €      =>     '>     €rT2    ÙĞniÖÓ              B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t B n . i n c   €      =>     '>      sT2    ÙĞniÖÓ €            B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t B n . i n c   €      >>     '>     €sT2    6oiÖÓ               B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x . i n c   €      >>     '>      tT2    6oiÖÓ              B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x . i n c   €      >>     '>     €tT2    6@piÖÓ              B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x . i n c   €      >>     '>      uT2    6@piÖÓ              B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x . i n c   €      >>     '>     €uT2    6@piÖÓ €            B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x . i n c   ˆ      ?>     '>      vT2    ~âpiÖÓ               F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x B n . i n c       ˆ      ?>     '>     ˆvT2    ~âpiÖÓ              F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x B n . i n c       ˆ      ?>     '>     wT2    ¥	qiÖÓ              F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x B n . i n c       ˆ      ?>     '>     ˜wT2    ¥	qiÖÓ              F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x B n . i n c       ˆ      ?>     '>      xT2    ¥	qiÖÓ €            F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x B n . i n c       ˆ      @>     '>     ¨xT2    ‘riÖÓ               F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x V c . i n c     t ˆ      @>     '>     0yT2    ‘riÖÓ              F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x V c . i n c     t ˆ      @>     '>     ¸yT2    KsiÖÓ              F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x V c . i n c     t ˆ      @>     '>     @zT2    Ğ’siÖÓ              F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x V c . i n c     t ˆ      @>     '>     ÈzT2    Ğ’siÖÓ €            F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x V c . i n c     t ˆ      A>     '>     P{T2    GósiÖÓ               J < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x V c B n . i n c   ˆ      A>     '>     Ø{T2    GósiÖÓ              J < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x V c B n . i n c   ˆ      A>     '>     `|T2    £ßtiÖÓ              J < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x V c B n . i n c   ˆ      A>     '>     è|T2    £ßtiÖÓ              J < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x V c B n . i n c   ˆ      A>     '>     p}T2    £ßtiÖÓ €            J < B a s i c E f f e c t _ V S B a s i c O n e L i g h t T x V c B n . i n c   €      B>     '>     ø}T2    òzuiÖÓ               B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t V c . i n c   €      B>     '>     x~T2    òzuiÖÓ              B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t V c . i n c   €      B>     '>     ø~T2    @uiÖÓ              B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t V c . i n c   €      B>     '>     xT2    @uiÖÓ              B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t V c . i n c           €      B>     '>      €T2    @uiÖÓ €            B < B a s i c E f f e c t _ V S B a s i c O n e L i g h t V c . i n c   ˆ      C>     '>     €€T2    ’ÜuiÖÓ               F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t V c B n . i n c     L ˆ      C>     '>     T2    ’ÜuiÖÓ              F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t V c B n . i n c     L ˆ      C>     '>     T2    $‘viÖÓ              F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t V c B n . i n c     L ˆ      C>     '>     ‚T2    $‘viÖÓ              F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t V c B n . i n c     L ˆ      C>     '>      ‚T2    $‘viÖÓ €            F < B a s i c E f f e c t _ V S B a s i c O n e L i g h t V c B n . i n c     L ˆ      D>     '>     (ƒT2    7lwiÖÓ               H < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g . i n c     ˆ      D>     '>     °ƒT2    ^“wiÖÓ              H < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g . i n c     ˆ      D>     '>     8„T2    Ğ¦xiÖÓ              H < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g . i n c     ˆ      D>     '>     À„T2    Ğ¦xiÖÓ              H < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g . i n c     ˆ      D>     '>     H…T2    Ğ¦xiÖÓ €            H < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g . i n c     ˆ      E>     '>     Ğ…T2    ·ùxiÖÓ               L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g B n . i n c ˆ      E>     '>     X†T2    ·ùxiÖÓ              L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g B n . i n c ˆ      E>     '>     à†T2    ¢$yiÖÓ              L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g B n . i n c ˆ      E>     '>     h‡T2    ¢$yiÖÓ              L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g B n . i n c ˆ      E>     '>     ğ‡T2    ¢$yiÖÓ €            L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g B n . i n c ˆ      F>     '>     xˆT2    syiÖÓ               L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x . i n c ˆ      F>     '>      ‰T2    syiÖÓ              L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x . i n c ˆ      F>     '>     ˆ‰T2    $8ziÖÓ              L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x . i n c ˆ      F>     '>     ŠT2    $8ziÖÓ              L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x . i n c ˆ      F>     '>     ˜ŠT2    ó]ziÖÓ €            L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x . i n c       G>     '>      ‹T2    K:|iÖÓ               P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x B n . i n c           G>     '>     °‹T2    K:|iÖÓ              P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x B n . i n c           G>     '>     @ŒT2    »Á}iÖÓ              P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x B n . i n c           G>     '>     ĞŒT2    »Á}iÖÓ              P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x B n . i n c           G>     '>     `T2    »Á}iÖÓ €            P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x B n . i n c           H>     '>     ğT2    ?~iÖÓ               P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x V c . i n c           H>     '>     €T2    ?~iÖÓ              P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x V c . i n c           H>     '>     T2    q6~iÖÓ              P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x V c . i n c                                                                                                           H>     '>      T2    q6~iÖÓ              P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x V c . i n c           H>     '>     T2    q6~iÖÓ €            P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x V c . i n c           I>     '>      ‘T2    Ã„~iÖÓ               T < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x V c B n . i n c       I>     '>     °‘T2    é«~iÖÓ              T < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x V c B n . i n c       I>     '>     @’T2    é«~iÖÓ              T < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x V c B n . i n c       I>     '>     Ğ’T2    é«~iÖÓ              T < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x V c B n . i n c       I>     '>     `“T2    é«~iÖÓ €            T < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g T x V c B n . i n c ˆ      J>     '>     ğ“T2    Kú~iÖÓ               L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g V c . i n c ˆ      J>     '>     x”T2    Kú~iÖÓ              L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g V c . i n c ˆ      J>     '>      •T2    u!iÖÓ              L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g V c . i n c ˆ      J>     '>     ˆ•T2    u!iÖÓ              L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g V c . i n c ˆ      J>     '>     –T2    u!iÖÓ €            L < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g V c . i n c       K>     '>     ˜–T2    ÇoiÖÓ               P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g V c B n . i n c           K>     '>     (—T2    ÇoiÖÓ              P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g V c B n . i n c           K>     '>     ¸—T2    ‚iÖÓ              P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g V c B n . i n c           K>     '>     H˜T2    ‚iÖÓ              P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g V c B n . i n c           K>     '>     Ø˜T2    ‚iÖÓ €            P < B a s i c E f f e c t _ V S B a s i c P i x e l L i g h t i n g V c B n . i n c     p      L>     '>     h™T2    äĞiÖÓ               2 < B a s i c E f f e c t _ V S B a s i c T x . i n c   p      L>     '>     Ø™T2    äĞiÖÓ              2 < B a s i c E f f e c t _ V S B a s i c T x . i n c   p      L>     '>     HšT2    9
iÖÓ              2 < B a s i c E f f e c t _ V S B a s i c T x . i n c   p      L>     '>     ¸šT2    9
iÖÓ              2 < B a s i c E f f e c t _ V S B a s i c T x . i n c   p      L>     '>     (›T2    9
iÖÓ €            2 < B a s i c E f f e c t _ V S B a s i c T x . i n c   x      M>     '>     ˜›T2    YiÖÓ               < < B a s i c E f f e c t _ V S B a s i c T x N o F o g . i n c x      M>     '>     œT2    YiÖÓ              < < B a s i c E f f e c t _ V S B a s i c T x N o F o g . i n c x      M>     '>     ˆœT2    c{iÖÓ              < < B a s i c E f f e c t _ V S B a s i c T x N o F o g . i n c x      M>     '>      T2    c{iÖÓ              < < B a s i c E f f e c t _ V S B a s i c T x N o F o g . i n c x      M>     '>     xT2    c{iÖÓ €            < < B a s i c E f f e c t _ V S B a s i c T x N o F o g . i n c x      N>     '>     ğT2    ¢iÖÓ               6 < B a s i c E f f e c t _ V S B a s i c T x V c . i n c     \ x      N>     '>     hT2    ¢iÖÓ              6 < B a s i c E f f e c t _ V S B a s i c T x V c . i n c     \ x      N>     '>     àT2    }@‚iÖÓ              6 < B a s i c E f f e c t _ V S B a s i c T x V c . i n c     \ x      N>     '>     XŸT2    }@‚iÖÓ              6 < B a s i c E f f e c t _ V S B a s i c T x V c . i n c     \                                                 x      N>     '>       T2    _f‚iÖÓ €            6 < B a s i c E f f e c t _ V S B a s i c T x V c . i n c     \ €      O>     '>     x T2    :‘‚iÖÓ               @ < B a s i c E f f e c t _ V S B a s i c T x V c N o F o g . i n c     €      O>     '>     ø T2    ]¸‚iÖÓ              @ < B a s i c E f f e c t _ V S B a s i c T x V c N o F o g . i n c     €      O>     '>     x¡T2    ¶ƒiÖÓ              @ < B a s i c E f f e c t _ V S B a s i c T x V c N o F o g . i n c     €      O>     '>     ø¡T2    ¶ƒiÖÓ              @ < B a s i c E f f e c t _ V S B a s i c T x V c N o F o g . i n c     €      O>     '>     x¢T2    ¶ƒiÖÓ €            @ < B a s i c E f f e c t _ V S B a s i c T x V c N o F o g . i n c     p      P>     '>     ø¢T2    Õ‚ƒiÖÓ               2 < B a s i c E f f e c t _ V S B a s i c V c . i n c   p      P>     '>     h£T2    Õ‚ƒiÖÓ              2 < B a s i c E f f e c t _ V S B a s i c V c . i n c   p      P>     '>     Ø£T2    Õ‚ƒiÖÓ              2 < B a s i c E f f e c t _ V S B a s i c V c . i n c   p      P>     '>     H¤T2    ü©ƒiÖÓ              2 < B a s i c E f f e c t _ V S B a s i c V c . i n c   p      P>     '>     ¸¤T2    ü©ƒiÖÓ €            2 < B a s i c E f f e c t _ V S B a s i c V c . i n c   x      Q>     '>     (¥T2    #ÑƒiÖÓ               < < B a s i c E f f e c t _ V S B a s i c V c N o F o g . i n c x      Q>     '>      ¥T2    #ÑƒiÖÓ              < < B a s i c E f f e c t _ V S B a s i c V c N o F o g . i n c x      Q>     '>     ¦T2    QøƒiÖÓ              < < B a s i c E f f e c t _ V S B a s i c V c N o F o g . i n c x      Q>     '>     ¦T2    QøƒiÖÓ              < < B a s i c E f f e c t _ V S B a s i c V c N o F o g . i n c x      Q>     '>     §T2    QøƒiÖÓ €            < < B a s i c E f f e c t _ V S B a s i c V c N o F o g . i n c ˆ      R>     '>     €§T2    £F„iÖÓ               J < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g . i n c   ˆ      R>     '>     ¨T2    £F„iÖÓ              J < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g . i n c   ˆ      R>     '>     ¨T2    £F„iÖÓ              J < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g . i n c   ˆ      R>     '>     ©T2    £F„iÖÓ              J < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g . i n c   ˆ      R>     '>      ©T2    £F„iÖÓ €            J < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g . i n c         S>     '>     (ªT2    ü”„iÖÓ               N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g B n . i n c             S>     '>     ¸ªT2    ü”„iÖÓ              N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g B n . i n c             S>     '>     H«T2    ü”„iÖÓ              N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g B n . i n c             S>     '>     Ø«T2    ü”„iÖÓ              N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g B n . i n c             S>     '>     h¬T2    ü”„iÖÓ €            N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g B n . i n c             T>     '>     ø¬T2    Qã„iÖÓ               N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x . i n c             T>     '>     ˆ­T2    Qã„iÖÓ              N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x . i n c             T>     '>     ®T2    Qã„iÖÓ              N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x . i n c             T>     '>     ¨®T2    Qã„iÖÓ              N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x . i n c             T>     '>     8¯T2    Qã„iÖÓ €            N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x . i n c                                                                     U>     '>      °T2    «1…iÖÓ               R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x B n . i n c         U>     '>     °T2    «1…iÖÓ              R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x B n . i n c         U>     '>      ±T2    !E†iÖÓ              R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x B n . i n c         U>     '>     °±T2    !E†iÖÓ              R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x B n . i n c         U>     '>     @²T2    !E†iÖÓ €            R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x B n . i n c         V>     '>     Ğ²T2    ¢»†iÖÓ               R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x V c . i n c         V>     '>     `³T2    ¢»†iÖÓ              R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x V c . i n c         V>     '>     ğ³T2    HÏ‡iÖÓ              R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x V c . i n c         V>     '>     €´T2    HÏ‡iÖÓ              R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x V c . i n c         V>     '>     µT2    HÏ‡iÖÓ €            R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x V c . i n c   ˜      W>     '>      µT2    "ˆiÖÓ               V < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x V c B n . i n c     - ˜      W>     '>     8¶T2    "ˆiÖÓ              V < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x V c B n . i n c     - ˜      W>     '>     Ğ¶T2    ]H‰iÖÓ              V < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x V c B n . i n c     - ˜      W>     '>     h·T2    ]H‰iÖÓ              V < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x V c B n . i n c     - ˜      W>     '>      ¸T2    ]H‰iÖÓ €            V < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g T x V c B n . i n c     -       X>     '>     ˜¸T2    â–‰iÖÓ               N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g V c . i n c             X>     '>     (¹T2    â–‰iÖÓ              N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g V c . i n c             X>     '>     ¸¹T2    Så‰iÖÓ              N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g V c . i n c             X>     '>     HºT2    D‹iÖÓ              N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g V c . i n c             X>     '>     ØºT2    D‹iÖÓ €            N < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g V c . i n c             Y>     '>     h»T2    Ê‹iÖÓ               R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g V c B n . i n c         Y>     '>     ø»T2    Ê‹iÖÓ              R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g V c B n . i n c         Y>     '>     ˆ¼T2    Ê‹iÖÓ              R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g V c B n . i n c         Y>     '>     ½T2    }Â‹iÖÓ              R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g V c B n . i n c         Y>     '>     ¨½T2    }Â‹iÖÓ €            R < B a s i c E f f e c t _ V S B a s i c V e r t e x L i g h t i n g V c B n . i n c   x      Z>     '>     8¾T2    ÎŒiÖÓ               : < D e b u g E f f e c t _ P S H e m i A m b i e n t . i n c   x      Z>     '>     °¾T2    ÎŒiÖÓ              : < D e b u g E f f e c t _ P S H e m i A m b i e n t . i n c   x      Z>     '>     (¿T2    ÎŒiÖÓ              : < D e b u g E f f e c t _ P S H e m i A m b i e n t . i n c                                                                                                   x      Z>     '>      ÀT2    ÎŒiÖÓ              : < D e b u g E f f e c t _ P S H e m i A m b i e n t . i n c   x      Z>     '>     xÀT2    ÎŒiÖÓ €            : < D e b u g E f f e c t _ P S H e m i A m b i e n t . i n c   €      [>     '>     ğÀT2    #_ŒiÖÓ               > < D e b u g E f f e c t _ P S R G B B i T a n g e n t s . i n c     9 €      [>     '>     pÁT2    #_ŒiÖÓ              > < D e b u g E f f e c t _ P S R G B B i T a n g e n t s . i n c     9 €      [>     '>     ğÁT2    yŒiÖÓ              > < D e b u g E f f e c t _ P S R G B B i T a n g e n t s . i n c     9 €      [>     '>     pÂT2    yŒiÖÓ              > < D e b u g E f f e c t _ P S R G B B i T a n g e n t s . i n c     9 €      [>     '>     ğÂT2    yŒiÖÓ €            > < D e b u g E f f e c t _ P S R G B B i T a n g e n t s . i n c     9 x      \>     '>     pÃT2    rÇŒiÖÓ               8 < D e b u g E f f e c t _ P S R G B N o r m a l s . i n c     x      \>     '>     èÃT2    rÇŒiÖÓ              8 < D e b u g E f f e c t _ P S R G B N o r m a l s . i n c     x      \>     '>     `ÄT2    rÇŒiÖÓ              8 < D e b u g E f f e c t _ P S R G B N o r m a l s . i n c     x      \>     '>     ØÄT2    rÇŒiÖÓ              8 < D e b u g E f f e c t _ P S R G B N o r m a l s . i n c     x      \>     '>     PÅT2    rÇŒiÖÓ €            8 < D e b u g E f f e c t _ P S R G B N o r m a l s . i n c     x      ]>     '>     ÈÅT2    ÇiÖÓ               : < D e b u g E f f e c t _ P S R G B T a n g e n t s . i n c   x      ]>     '>     @ÆT2    ÇiÖÓ              : < D e b u g E f f e c t _ P S R G B T a n g e n t s . i n c   x      ]>     '>     ¸ÆT2    —.iÖÓ              : < D e b u g E f f e c t _ P S R G B T a n g e n t s . i n c   x      ]>     '>     0ÇT2    —.iÖÓ              : < D e b u g E f f e c t _ P S R G B T a n g e n t s . i n c   x      ]>     '>     ¨ÇT2    —.iÖÓ €            : < D e b u g E f f e c t _ P S R G B T a n g e n t s . i n c   p      ^>     '>      ÈT2    å|iÖÓ               . < D e b u g E f f e c t _ V S D e b u g . i n c     4 p      ^>     '>     ÈT2    å|iÖÓ              . < D e b u g E f f e c t _ V S D e b u g . i n c     4 p      ^>     '>      ÉT2    AtiÖÓ              . < D e b u g E f f e c t _ V S D e b u g . i n c     4 p      ^>     '>     pÉT2    AtiÖÓ              . < D e b u g E f f e c t _ V S D e b u g . i n c     4 p      ^>     '>     àÉT2    AtiÖÓ €            . < D e b u g E f f e c t _ V S D e b u g . i n c     4 p      _>     '>     PÊT2    NèiÖÓ               2 < D e b u g E f f e c t _ V S D e b u g B n . i n c   p      _>     '>     ÀÊT2    NèiÖÓ              2 < D e b u g E f f e c t _ V S D e b u g B n . i n c   p      _>     '>     0ËT2    NèiÖÓ              2 < D e b u g E f f e c t _ V S D e b u g B n . i n c   p      _>     '>      ËT2    NèiÖÓ              2 < D e b u g E f f e c t _ V S D e b u g B n . i n c   p      _>     '>     ÌT2    'iÖÓ €            2 < D e b u g E f f e c t _ V S D e b u g B n . i n c   p      `>     '>     €ÌT2    Ê]iÖÓ               2 < D e b u g E f f e c t _ V S D e b u g V c . i n c   p      `>     '>     ğÌT2    Ê]iÖÓ              2 < D e b u g E f f e c t _ V S D e b u g V c . i n c   p      `>     '>     `ÍT2    ı„iÖÓ              2 < D e b u g E f f e c t _ V S D e b u g V c . i n c   p      `>     '>     ĞÍT2    ı„iÖÓ              2 < D e b u g E f f e c t _ V S D e b u g V c . i n c   p      `>     '>     @ÎT2    ı„iÖÓ €            2 < D e b u g E f f e c t _ V S D e b u g V c . i n c   x      a>     '>     °ÎT2    NÓiÖÓ               6 < D e b u g E f f e c t _ V S D e b u g V c B n . i n c     c x      a>     '>     (ÏT2    NÓiÖÓ              6 < D e b u g E f f e c t _ V S D e b u g V c B n . i n c     c                                                                                                 x      a>     '>      ĞT2    NÓiÖÓ              6 < D e b u g E f f e c t _ V S D e b u g V c B n . i n c     c x      a>     '>     xĞT2    NÓiÖÓ              6 < D e b u g E f f e c t _ V S D e b u g V c B n . i n c     c x      a>     '>     ğĞT2    NÓiÖÓ €            6 < D e b u g E f f e c t _ V S D e b u g V c B n . i n c     c h      b>     '>     hÑT2    ¨!iÖÓ               & < D G S L E f f e c t _ m a i n . i n c     g h      b>     '>     ĞÑT2    ¨!iÖÓ              & < D G S L E f f e c t _ m a i n . i n c     g h      b>     '>     8ÒT2    p\‘iÖÓ              & < D G S L E f f e c t _ m a i n . i n c     g h      b>     '>      ÒT2    p\‘iÖÓ              & < D G S L E f f e c t _ m a i n . i n c     g h      b>     '>     ÓT2    p\‘iÖÓ €            & < D G S L E f f e c t _ m a i n . i n c     g p      c>     '>     pÓT2    N©‘iÖÓ               2 < D G S L E f f e c t _ m a i n 1 B o n e s . i n c   p      c>     '>     àÓT2    N©‘iÖÓ              2 < D G S L E f f e c t _ m a i n 1 B o n e s . i n c   p      c>     '>     PÔT2    uĞ‘iÖÓ              2 < D G S L E f f e c t _ m a i n 1 B o n e s . i n c   p      c>     '>     ÀÔT2    uĞ‘iÖÓ              2 < D G S L E f f e c t _ m a i n 1 B o n e s . i n c   p      c>     '>     0ÕT2    uĞ‘iÖÓ €            2 < D G S L E f f e c t _ m a i n 1 B o n e s . i n c   x      d>     '>      ÕT2    Î’iÖÓ               6 < D G S L E f f e c t _ m a i n 1 B o n e s V c . i n c     3 x      d>     '>     ÖT2    Î’iÖÓ              6 < D G S L E f f e c t _ m a i n 1 B o n e s V c . i n c     3 x      d>     '>     ÖT2    Î’iÖÓ              6 < D G S L E f f e c t _ m a i n 1 B o n e s V c . i n c     3 x      d>     '>     ×T2    Î’iÖÓ              6 < D G S L E f f e c t _ m a i n 1 B o n e s V c . i n c     3 x      d>     '>     €×T2    \F’iÖÓ €            6 < D G S L E f f e c t _ m a i n 1 B o n e s V c . i n c     3 p      e>     '>     ø×T2    V”’iÖÓ               2 < D G S L E f f e c t _ m a i n 2 B o n e s . i n c   p      e>     '>     hØT2    V”’iÖÓ              2 < D G S L E f f e c t _ m a i n 2 B o n e s . i n c   p      e>     '>     ØØT2    E”iÖÓ              2 < D G S L E f f e c t _ m a i n 2 B o n e s . i n c   p      e>     '>     HÙT2    E”iÖÓ              2 < D G S L E f f e c t _ m a i n 2 B o n e s . i n c   p      e>     '>     ¸ÙT2    E”iÖÓ €            2 < D G S L E f f e c t _ m a i n 2 B o n e s . i n c   x      f>     '>     (ÚT2    $C”iÖÓ               6 < D G S L E f f e c t _ m a i n 2 B o n e s V c . i n c       x      f>     '>      ÚT2    Vj”iÖÓ              6 < D G S L E f f e c t _ m a i n 2 B o n e s V c . i n c       x      f>     '>     ÛT2    Ÿ_—iÖÓ              6 < D G S L E f f e c t _ m a i n 2 B o n e s V c . i n c       x      f>     '>     ÛT2    Ÿ_—iÖÓ              6 < D G S L E f f e c t _ m a i n 2 B o n e s V c . i n c       x      f>     '>     ÜT2    Ÿ_—iÖÓ €            6 < D G S L E f f e c t _ m a i n 2 B o n e s V c . i n c       p      g>     '>     €ÜT2    vàšiÖÓ               2 < D G S L E f f e c t _ m a i n 4 B o n e s . i n c   p      g>     '>     ğÜT2    ›iÖÓ              2 < D G S L E f f e c t _ m a i n 4 B o n e s . i n c   p      g>     '>     `İT2    ›iÖÓ              2 < D G S L E f f e c t _ m a i n 4 B o n e s . i n c   p      g>     '>     ĞİT2    ¯.›iÖÓ              2 < D G S L E f f e c t _ m a i n 4 B o n e s . i n c   p      g>     '>     @ŞT2    ¯.›iÖÓ €            2 < D G S L E f f e c t _ m a i n 4 B o n e s . i n c   x      h>     '>     °ŞT2    H ›iÖÓ               6 < D G S L E f f e c t _ m a i n 4 B o n e s V c . i n c     4 x      h>     '>     (ßT2    H ›iÖÓ              6 < D G S L E f f e c t _ m a i n 4 B o n e s V c . i n c     4                                                                                                 x      h>     '>      àT2    WÇ›iÖÓ              6 < D G S L E f f e c t _ m a i n 4 B o n e s V c . i n c     4 x      h>     '>     xàT2    WÇ›iÖÓ              6 < D G S L E f f e c t _ m a i n 4 B o n e s V c . i n c     4 x      h>     '>     ğàT2    WÇ›iÖÓ €            6 < D G S L E f f e c t _ m a i n 4 B o n e s V c . i n c     4 h      i>     '>     háT2    ¬œiÖÓ               * < D G S L E f f e c t _ m a i n V c . i n c   h      i>     '>     ĞáT2    ¬œiÖÓ              * < D G S L E f f e c t _ m a i n V c . i n c   h      i>     '>     8âT2    ¬œiÖÓ              * < D G S L E f f e c t _ m a i n V c . i n c   h      i>     '>      âT2    ¬œiÖÓ              * < D G S L E f f e c t _ m a i n V c . i n c   h      i>     '>     ãT2    ¬œiÖÓ €            * < D G S L E f f e c t _ m a i n V c . i n c   h      j>     '>     pãT2    şcœiÖÓ               ( < D G S L L a m b e r t _ m a i n . i n c     h      j>     '>     ØãT2    şcœiÖÓ              ( < D G S L L a m b e r t _ m a i n . i n c     h      j>     '>     @äT2    [²œiÖÓ              ( < D G S L L a m b e r t _ m a i n . i n c     h      j>     '>     ¨äT2    [²œiÖÓ              ( < D G S L L a m b e r t _ m a i n . i n c     h      j>     '>     åT2    [²œiÖÓ €            ( < D G S L L a m b e r t _ m a i n . i n c     h      k>     '>     xåT2    µiÖÓ               , < D G S L L a m b e r t _ m a i n T k . i n c h      k>     '>     àåT2    µiÖÓ              , < D G S L L a m b e r t _ m a i n T k . i n c h      k>     '>     HæT2    Ü(iÖÓ              , < D G S L L a m b e r t _ m a i n T k . i n c h      k>     '>     °æT2    Ü(iÖÓ              , < D G S L L a m b e r t _ m a i n T k . i n c h      k>     '>     çT2    Ü(iÖÓ €            , < D G S L L a m b e r t _ m a i n T k . i n c h      l>     '>     €çT2    =wiÖÓ               , < D G S L L a m b e r t _ m a i n T x . i n c h      l>     '>     èçT2    =wiÖÓ              , < D G S L L a m b e r t _ m a i n T x . i n c h      l>     '>     PèT2    =wiÖÓ              , < D G S L L a m b e r t _ m a i n T x . i n c h      l>     '>     ¸èT2    =wiÖÓ              , < D G S L L a m b e r t _ m a i n T x . i n c h      l>     '>      éT2    =wiÖÓ €            , < D G S L L a m b e r t _ m a i n T x . i n c p      m>     '>     ˆéT2    ÅiÖÓ               0 < D G S L L a m b e r t _ m a i n T x T k . i n c     p      m>     '>     øéT2    ÅiÖÓ              0 < D G S L L a m b e r t _ m a i n T x T k . i n c     p      m>     '>     hêT2    ÅiÖÓ              0 < D G S L L a m b e r t _ m a i n T x T k . i n c     p      m>     '>     ØêT2    ÅiÖÓ              0 < D G S L L a m b e r t _ m a i n T x T k . i n c     p      m>     '>     HëT2    ÅiÖÓ €            0 < D G S L L a m b e r t _ m a i n T x T k . i n c     `      n>     '>     ¸ëT2    àiÖÓ               $ < D G S L P h o n g _ m a i n . i n c `      n>     '>     ìT2    àiÖÓ              $ < D G S L P h o n g _ m a i n . i n c `      n>     '>     xìT2    ó:iÖÓ              $ < D G S L P h o n g _ m a i n . i n c `      n>     '>     ØìT2    ó:iÖÓ              $ < D G S L P h o n g _ m a i n . i n c `      n>     '>     8íT2    ó:iÖÓ €            $ < D G S L P h o n g _ m a i n . i n c h      o>     '>     ˜íT2    X‰iÖÓ               ( < D G S L P h o n g _ m a i n T k . i n c     h      o>     '>      îT2    X‰iÖÓ              ( < D G S L P h o n g _ m a i n T k . i n c     h      o>     '>     hîT2    °iÖÓ              ( < D G S L P h o n g _ m a i n T k . i n c     h      o>     '>     ĞîT2    °iÖÓ              ( < D G S L P h o n g _ m a i n T k . i n c     h      o>     '>     8ïT2    °iÖÓ €            ( < D G S L P h o n g _ m a i n T k . i n c                                                                                                     h      p>     '>      ğT2    àşiÖÓ               ( < D G S L P h o n g _ m a i n T x . i n c     h      p>     '>     hğT2    àşiÖÓ              ( < D G S L P h o n g _ m a i n T x . i n c     h      p>     '>     ĞğT2    àşiÖÓ              ( < D G S L P h o n g _ m a i n T x . i n c     h      p>     '>     8ñT2    àşiÖÓ              ( < D G S L P h o n g _ m a i n T x . i n c     h      p>     '>      ñT2    àşiÖÓ €            ( < D G S L P h o n g _ m a i n T x . i n c     h      q>     '>     òT2    1MŸiÖÓ               , < D G S L P h o n g _ m a i n T x T k . i n c h      q>     '>     pòT2    1MŸiÖÓ              , < D G S L P h o n g _ m a i n T x T k . i n c h      q>     '>     ØòT2    1MŸiÖÓ              , < D G S L P h o n g _ m a i n T x T k . i n c h      q>     '>     @óT2    `tŸiÖÓ              , < D G S L P h o n g _ m a i n T x T k . i n c h      q>     '>     ¨óT2    `tŸiÖÓ €            , < D G S L P h o n g _ m a i n T x T k . i n c `      r>     '>     ôT2    w›ŸiÖÓ               $ < D G S L U n l i t _ m a i n . i n c `      r>     '>     pôT2    w›ŸiÖÓ              $ < D G S L U n l i t _ m a i n . i n c `      r>     '>     ĞôT2    ¢ÂŸiÖÓ              $ < D G S L U n l i t _ m a i n . i n c `      r>     '>     0õT2    ¢ÂŸiÖÓ              $ < D G S L U n l i t _ m a i n . i n c `      r>     '>     õT2    ¢ÂŸiÖÓ €            $ < D G S L U n l i t _ m a i n . i n c h      s>     '>     ğõT2    ÜéŸiÖÓ               ( < D G S L U n l i t _ m a i n T k . i n c     h      s>     '>     XöT2    ÜéŸiÖÓ              ( < D G S L U n l i t _ m a i n T k . i n c     h      s>     '>     ÀöT2    ÜéŸiÖÓ              ( < D G S L U n l i t _ m a i n T k . i n c     h      s>     '>     (÷T2    ÜéŸiÖÓ              ( < D G S L U n l i t _ m a i n T k . i n c     h      s>     '>     ÷T2    ÜéŸiÖÓ €            ( < D G S L U n l i t _ m a i n T k . i n c     h      t>     '>     ø÷T2    .8 iÖÓ               ( < D G S L U n l i t _ m a i n T x . i n c     h      t>     '>     `øT2    .8 iÖÓ              ( < D G S L U n l i t _ m a i n T x . i n c     h      t>     '>     ÈøT2    .8 iÖÓ              ( < D G S L U n l i t _ m a i n T x . i n c     h      t>     '>     0ùT2    .8 iÖÓ              ( < D G S L U n l i t _ m a i n T x . i n c     h      t>     '>     ˜ùT2    X_ iÖÓ €            ( < D G S L U n l i t _ m a i n T x . i n c     h      u>     '>      úT2    ‡† iÖÓ               , < D G S L U n l i t _ m a i n T x T k . i n c h      u>     '>     húT2    ‡† iÖÓ              , < D G S L U n l i t _ m a i n T x T k . i n c h      u>     '>     ĞúT2    ‡† iÖÓ              , < D G S L U n l i t _ m a i n T x T k . i n c h      u>     '>     8ûT2    ‡† iÖÓ              , < D G S L U n l i t _ m a i n T x T k . i n c h      u>     '>      ûT2    ‡† iÖÓ €            , < D G S L U n l i t _ m a i n T x T k . i n c ˆ      v>     '>     üT2    ÜÔ iÖÓ               F < D u a l T e x t u r e E f f e c t _ P S D u a l T e x t u r e . i n c     . ˆ      v>     '>     üT2    ÜÔ iÖÓ              F < D u a l T e x t u r e E f f e c t _ P S D u a l T e x t u r e . i n c     . ˆ      v>     '>     ıT2    zè¡iÖÓ              F < D u a l T e x t u r e E f f e c t _ P S D u a l T e x t u r e . i n c     . ˆ      v>     '>      ıT2    zè¡iÖÓ              F < D u a l T e x t u r e E f f e c t _ P S D u a l T e x t u r e . i n c     . ˆ      v>     '>     (şT2    zè¡iÖÓ €            F < D u a l T e x t u r e E f f e c t _ P S D u a l T e x t u r e . i n c     .       w>     '>     °şT2    ›5¢iÖÓ               P < D u a l T e x t u r e E f f e c t _ P S D u a l T e x t u r e N o F o g . i n c           w>     '>     @ÿT2    ›5¢iÖÓ              P < D u a l T e x t u r e E f f e c t _ P S D u a l T e x t u r e N o F o g . i n c                                                           w>     '>       U2    ²\¢iÖÓ              P < D u a l T e x t u r e E f f e c t _ P S D u a l T e x t u r e N o F o g . i n c           w>     '>      U2    ²\¢iÖÓ              P < D u a l T e x t u r e E f f e c t _ P S D u a l T e x t u r e N o F o g . i n c           w>     '>      U2    ²\¢iÖÓ €            P < D u a l T e x t u r e E f f e c t _ P S D u a l T e x t u r e N o F o g . i n c     ˆ      x>     '>     °U2    Ôª¢iÖÓ               F < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e . i n c     n ˆ      x>     '>     8U2    Ôª¢iÖÓ              F < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e . i n c     n ˆ      x>     '>     ÀU2    Ôª¢iÖÓ              F < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e . i n c     n ˆ      x>     '>     HU2    ¡Ò¢iÖÓ              F < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e . i n c     n ˆ      x>     '>     ĞU2    ¡Ò¢iÖÓ €            F < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e . i n c     n       y>     '>     XU2    yù¢iÖÓ               P < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e N o F o g . i n c           y>     '>     èU2    yù¢iÖÓ              P < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e N o F o g . i n c           y>     '>     xU2    X £iÖÓ              P < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e N o F o g . i n c           y>     '>     U2    X £iÖÓ              P < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e N o F o g . i n c           y>     '>     ˜U2    X £iÖÓ €            P < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e N o F o g . i n c     ˆ      z>     '>     (U2    ¶n£iÖÓ               J < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e V c . i n c   ˆ      z>     '>     °U2    ¶n£iÖÓ              J < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e V c . i n c   ˆ      z>     '>     8U2    ¶n£iÖÓ              J < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e V c . i n c   ˆ      z>     '>     ÀU2    ¶n£iÖÓ              J < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e V c . i n c   ˆ      z>     '>     H	U2    Ü•£iÖÓ €            J < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e V c . i n c         {>     '>     Ğ	U2    Úç£iÖÓ               T < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e V c N o F o g . i n c       {>     '>     `
U2    Úç£iÖÓ              T < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e V c N o F o g . i n c       {>     '>     ğ
U2    Úç£iÖÓ              T < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e V c N o F o g . i n c       {>     '>     €U2    Úç£iÖÓ              T < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e V c N o F o g . i n c       {>     '>     U2    Úç£iÖÓ €            T < D u a l T e x t u r e E f f e c t _ V S D u a l T e x t u r e V c N o F o g . i n c €      |>     '>      U2    76¤iÖÓ               B < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p . i n c   €      |>     '>      U2    76¤iÖÓ              B < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p . i n c   €      |>     '>      U2    76¤iÖÓ              B < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p . i n c   €      |>     '>      U2    76¤iÖÓ              B < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p . i n c   €      |>     '>      U2    f]¤iÖÓ €            B < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p . i n c   ˆ      }>     '>      U2    Aƒ¤iÖÓ               L < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p N o F o g . i n c                                                                                         ˆ      }>     '>      U2    hª¤iÖÓ              L < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p N o F o g . i n c ˆ      }>     '>     ˆU2    hª¤iÖÓ              L < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p N o F o g . i n c ˆ      }>     '>     U2    hª¤iÖÓ              L < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p N o F o g . i n c ˆ      }>     '>     ˜U2    hª¤iÖÓ €            L < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p N o F o g . i n c ˜      ~>     '>      U2    ’Ñ¤iÖÓ               \ < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g . i n c ˜      ~>     '>     ¸U2    ’Ñ¤iÖÓ              \ < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g . i n c ˜      ~>     '>     PU2    ®¦iÖÓ              \ < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g . i n c ˜      ~>     '>     èU2    ®¦iÖÓ              \ < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g . i n c ˜      ~>     '>     €U2    ®¦iÖÓ €            \ < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g . i n c ¨      >     '>     U2    ×!§iÖÓ               j < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g F r e s n e l . i n c   ¨      >     '>     ÀU2    ×!§iÖÓ              j < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g F r e s n e l . i n c   ¨      >     '>     hU2    ×!§iÖÓ              j < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g F r e s n e l . i n c   ¨      >     '>     U2    )I§iÖÓ              j < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g F r e s n e l . i n c   ¨      >     '>     ¸U2    )I§iÖÓ €            j < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g F r e s n e l . i n c   °      €>     '>     `U2    Ò˜§iÖÓ               t < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g F r e s n e l N o F o g . i n c °      €>     '>     U2    Ò˜§iÖÓ              t < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g F r e s n e l N o F o g . i n c °      €>     '>     ÀU2    ‚¾§iÖÓ              t < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g F r e s n e l N o F o g . i n c °      €>     '>     pU2    ‚¾§iÖÓ              t < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g F r e s n e l N o F o g . i n c °      €>     '>      U2    ‚¾§iÖÓ €            t < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g F r e s n e l N o F o g . i n c ¨      >     '>     ĞU2    ×¨iÖÓ               f < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g N o F o g . i n c     e ¨      >     '>     xU2    ×¨iÖÓ              f < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g N o F o g . i n c     e ¨      >     '>      U2    ×¨iÖÓ              f < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g N o F o g . i n c     e ¨      >     '>     ÈU2    ×¨iÖÓ              f < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g N o F o g . i n c     e ¨      >     '>     pU2    ×¨iÖÓ €            f < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p P i x e l L i g h t i n g N o F o g . i n c     e       ‚>     '>     U2    1\¨iÖÓ               R < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p S p e c u l a r . i n c                                                                                                 ‚>     '>       U2    1\¨iÖÓ              R < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p S p e c u l a r . i n c         ‚>     '>      U2    1\¨iÖÓ              R < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p S p e c u l a r . i n c         ‚>     '>      !U2    Xƒ¨iÖÓ              R < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p S p e c u l a r . i n c         ‚>     '>     °!U2    Xƒ¨iÖÓ €            R < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p S p e c u l a r . i n c   ˜      ƒ>     '>     @"U2    ƒª¨iÖÓ               \ < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p S p e c u l a r N o F o g . i n c ˜      ƒ>     '>     Ø"U2    ƒª¨iÖÓ              \ < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p S p e c u l a r N o F o g . i n c ˜      ƒ>     '>     p#U2    ±Ñ¨iÖÓ              \ < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p S p e c u l a r N o F o g . i n c ˜      ƒ>     '>     $U2    ±Ñ¨iÖÓ              \ < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p S p e c u l a r N o F o g . i n c ˜      ƒ>     '>      $U2    ±Ñ¨iÖÓ €            \ < E n v i r o n m e n t M a p E f f e c t _ P S E n v M a p S p e c u l a r N o F o g . i n c €      „>     '>     8%U2     ©iÖÓ               B < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p . i n c   €      „>     '>     ¸%U2     ©iÖÓ              B < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p . i n c   €      „>     '>     8&U2     ©iÖÓ              B < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p . i n c   €      „>     '>     ¸&U2     ©iÖÓ              B < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p . i n c   €      „>     '>     8'U2     ©iÖÓ €            B < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p . i n c   ˆ      …>     '>     ¸'U2    Xn©iÖÓ               F < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p B n . i n c     4 ˆ      …>     '>     @(U2    Xn©iÖÓ              F < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p B n . i n c     4 ˆ      …>     '>     È(U2    Xn©iÖÓ              F < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p B n . i n c     4 ˆ      …>     '>     P)U2    Xn©iÖÓ              F < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p B n . i n c     4 ˆ      …>     '>     Ø)U2    Xn©iÖÓ €            F < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p B n . i n c     4       †>     '>     `*U2    @½©iÖÓ               P < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p F r e s n e l . i n c           †>     '>     ğ*U2    @½©iÖÓ              P < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p F r e s n e l . i n c           †>     '>     €+U2    @½©iÖÓ              P < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p F r e s n e l . i n c           †>     '>     ,U2    àã©iÖÓ              P < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p F r e s n e l . i n c           †>     '>      ,U2    Âå©iÖÓ €            P < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p F r e s n e l . i n c           ‡>     '>     0-U2    íªiÖÓ               T < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p F r e s n e l B n . i n c       ‡>     '>     À-U2    4ªiÖÓ              T < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p F r e s n e l B n . i n c       ‡>     '>     P.U2    4ªiÖÓ              T < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p F r e s n e l B n . i n c       ‡>     '>     à.U2    4ªiÖÓ              T < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p F r e s n e l B n . i n c       ‡>     '>     p/U2    4ªiÖÓ €            T < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p F r e s n e l B n . i n c       ˆ>     '>      0U2    q‚ªiÖÓ               R < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t . i n c         ˆ>     '>     0U2    q‚ªiÖÓ              R < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t . i n c         ˆ>     '>      1U2    q‚ªiÖÓ              R < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t . i n c         ˆ>     '>     °1U2    q‚ªiÖÓ              R < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t . i n c         ˆ>     '>     @2U2    —©ªiÖÓ €            R < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t . i n c   ˜      ‰>     '>     Ğ2U2    ÂĞªiÖÓ               V < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t B n . i n c     d ˜      ‰>     '>     h3U2    ÂĞªiÖÓ              V < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t B n . i n c     d ˜      ‰>     '>      4U2    ñ÷ªiÖÓ              V < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t B n . i n c     d ˜      ‰>     '>     ˜4U2    ñ÷ªiÖÓ              V < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t B n . i n c     d ˜      ‰>     '>     05U2    ñ÷ªiÖÓ €            V < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t B n . i n c     d        Š>     '>     È5U2    ^F«iÖÓ               ` < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t F r e s n e l . i n c            Š>     '>     h6U2    ^F«iÖÓ              ` < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t F r e s n e l . i n c            Š>     '>     7U2    ^F«iÖÓ              ` < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t F r e s n e l . i n c            Š>     '>     ¨7U2    ^F«iÖÓ              ` < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t F r e s n e l . i n c            Š>     '>     H8U2    ^F«iÖÓ €            ` < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t F r e s n e l . i n c            ‹>     '>     è8U2    B­iÖÓ               d < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t F r e s n e l B n . i n c        ‹>     '>     ˆ9U2    B­iÖÓ              d < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t F r e s n e l B n . i n c        ‹>     '>     (:U2    qC­iÖÓ              d < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t F r e s n e l B n . i n c        ‹>     '>     È:U2    qC­iÖÓ              d < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t F r e s n e l B n . i n c        ‹>     '>     h;U2    qC­iÖÓ €            d < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p O n e L i g h t F r e s n e l B n . i n c ˜      Œ>     '>     <U2    Æ‘­iÖÓ               \ < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p P i x e l L i g h t i n g . i n c ˜      Œ>     '>      <U2    Æ‘­iÖÓ              \ < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p P i x e l L i g h t i n g . i n c ˜      Œ>     '>     8=U2    Æ‘­iÖÓ              \ < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p P i x e l L i g h t i n g . i n c ˜      Œ>     '>     Ğ=U2    é¸­iÖÓ              \ < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p P i x e l L i g h t i n g . i n c ˜      Œ>     '>     h>U2    é¸­iÖÓ €            \ < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p P i x e l L i g h t i n g . i n c        >     '>      ?U2    >®iÖÓ               ` < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p P i x e l L i g h t i n g B n . i n c                                                                                                            >     '>      @U2    >®iÖÓ              ` < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p P i x e l L i g h t i n g B n . i n c            >     '>      @U2    >®iÖÓ              ` < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p P i x e l L i g h t i n g B n . i n c            >     '>     @AU2    >®iÖÓ              ` < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p P i x e l L i g h t i n g B n . i n c            >     '>     àAU2    >®iÖÓ €            ` < E n v i r o n m e n t M a p E f f e c t _ V S E n v M a p P i x e l L i g h t i n g B n . i n c     ˜      >     '>     €BU2    ˜U®iÖÓ               V < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x . i n c     o ˜      >     '>     CU2    ˜U®iÖÓ              V < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x . i n c     o ˜      >     '>     °CU2    ˜U®iÖÓ              V < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x . i n c     o ˜      >     '>     HDU2    ˜U®iÖÓ              V < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x . i n c     o ˜      >     '>     àDU2    Æ|®iÖÓ €            V < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x . i n c     o        >     '>     xEU2    ñ£®iÖÓ               ` < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o F o g . i n c            >     '>     FU2    Ë®iÖÓ              ` < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o F o g . i n c            >     '>     ¸FU2    Ë®iÖÓ              ` < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o F o g . i n c            >     '>     XGU2    Ë®iÖÓ              ` < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o F o g . i n c            >     '>     øGU2    Ë®iÖÓ €            ` < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o F o g . i n c     ¨      >     '>     ˜HU2    m¯iÖÓ               h < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o F o g S p e c . i n c     ¨      >     '>     @IU2    m¯iÖÓ              h < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o F o g S p e c . i n c     ¨      >     '>     èIU2    œ@¯iÖÓ              h < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o F o g S p e c . i n c     ¨      >     '>     JU2    œ@¯iÖÓ              h < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o F o g S p e c . i n c     ¨      >     '>     8KU2    œ@¯iÖÓ €            h < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o F o g S p e c . i n c            ‘>     '>     àKU2    Æg¯iÖÓ               b < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o S p e c . i n c          ‘>     '>     €LU2    Æg¯iÖÓ              b < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o S p e c . i n c          ‘>     '>      MU2    ñ¯iÖÓ              b < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o S p e c . i n c          ‘>     '>     ÀMU2    ñ¯iÖÓ              b < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o S p e c . i n c          ‘>     '>     `NU2    ñ¯iÖÓ €            b < N o r m a l M a p E f f e c t _ P S N o r m a l P i x e l L i g h t i n g T x N o S p e c . i n c   ˜      ’>     '>      OU2    $¶¯iÖÓ               V < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x . i n c     s                                                                                                         ˜      ’>     '>      PU2    $¶¯iÖÓ              V < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x . i n c     s ˜      ’>     '>     ˜PU2    $¶¯iÖÓ              V < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x . i n c     s ˜      ’>     '>     0QU2    Bİ¯iÖÓ              V < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x . i n c     s ˜      ’>     '>     ÈQU2    Bİ¯iÖÓ €            V < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x . i n c     s ˜      “>     '>     `RU2    m°iÖÓ               Z < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x B n . i n c   ˜      “>     '>     øRU2    m°iÖÓ              Z < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x B n . i n c   ˜      “>     '>     SU2    m°iÖÓ              Z < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x B n . i n c   ˜      “>     '>     (TU2    m°iÖÓ              Z < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x B n . i n c   ˜      “>     '>     ÀTU2    ”+°iÖÓ €            Z < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x B n . i n c   ˜      ”>     '>     XUU2    ÆR°iÖÓ               Z < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x V c . i n c   ˜      ”>     '>     ğUU2    ÆR°iÖÓ              Z < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x V c . i n c   ˜      ”>     '>     ˆVU2    íy°iÖÓ              Z < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x V c . i n c   ˜      ”>     '>      WU2    íy°iÖÓ              Z < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x V c . i n c   ˜      ”>     '>     ¸WU2    íy°iÖÓ €            Z < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x V c . i n c          •>     '>     PXU2    ¡°iÖÓ               ^ < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x V c B n . i n c              •>     '>     ğXU2    ¡°iÖÓ              ^ < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x V c B n . i n c              •>     '>     YU2    ¡°iÖÓ              ^ < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x V c B n . i n c              •>     '>     0ZU2    ¡°iÖÓ              ^ < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x V c B n . i n c              •>     '>     ĞZU2    ôÈ°iÖÓ €            ^ < N o r m a l M a p E f f e c t _ V S N o r m a l P i x e l L i g h t i n g T x V c B n . i n c       p      –>     '>     p[U2    mï°iÖÓ               0 < P B R E f f e c t _ P S C o n s t a n t . i n c     p      –>     '>     à[U2    mï°iÖÓ              0 < P B R E f f e c t _ P S C o n s t a n t . i n c     p      –>     '>     P\U2    Æ²iÖÓ              0 < P B R E f f e c t _ P S C o n s t a n t . i n c     p      –>     '>     À\U2    Æ²iÖÓ              0 < P B R E f f e c t _ P S C o n s t a n t . i n c     p      –>     '>     0]U2    œì²iÖÓ €            0 < P B R E f f e c t _ P S C o n s t a n t . i n c     p      —>     '>      ]U2    é:³iÖÓ               0 < P B R E f f e c t _ P S T e x t u r e d . i n c     p      —>     '>     ^U2    é:³iÖÓ              0 < P B R E f f e c t _ P S T e x t u r e d . i n c     p      —>     '>     €^U2    ³c³iÖÓ              0 < P B R E f f e c t _ P S T e x t u r e d . i n c     p      —>     '>     ğ^U2    ³c³iÖÓ              0 < P B R E f f e c t _ P S T e x t u r e d . i n c     p      —>     '>     `_U2    ³c³iÖÓ €            0 < P B R E f f e c t _ P S T e x t u r e d . i n c                                                     €      ˜>     '>      `U2     ×³iÖÓ               @ < P B R E f f e c t _ P S T e x t u r e d E m i s s i v e . i n c     €      ˜>     '>     €`U2     ×³iÖÓ              @ < P B R E f f e c t _ P S T e x t u r e d E m i s s i v e . i n c     €      ˜>     '>      aU2     ×³iÖÓ              @ < P B R E f f e c t _ P S T e x t u r e d E m i s s i v e . i n c     €      ˜>     '>     €aU2    ÿ³iÖÓ              @ < P B R E f f e c t _ P S T e x t u r e d E m i s s i v e . i n c     €      ˜>     '>      bU2    ÿ³iÖÓ €            @ < P B R E f f e c t _ P S T e x t u r e d E m i s s i v e . i n c           ™>     '>     €bU2    M´iÖÓ               P < P B R E f f e c t _ P S T e x t u r e d E m i s s i v e V e l o c i t y . i n c           ™>     '>     cU2    M´iÖÓ              P < P B R E f f e c t _ P S T e x t u r e d E m i s s i v e V e l o c i t y . i n c           ™>     '>      cU2    M´iÖÓ              P < P B R E f f e c t _ P S T e x t u r e d E m i s s i v e V e l o c i t y . i n c           ™>     '>     0dU2    M´iÖÓ              P < P B R E f f e c t _ P S T e x t u r e d E m i s s i v e V e l o c i t y . i n c           ™>     '>     ÀdU2    M´iÖÓ €            P < P B R E f f e c t _ P S T e x t u r e d E m i s s i v e V e l o c i t y . i n c     €      š>     '>     PeU2    n›´iÖÓ               @ < P B R E f f e c t _ P S T e x t u r e d V e l o c i t y . i n c     €      š>     '>     ĞeU2    n›´iÖÓ              @ < P B R E f f e c t _ P S T e x t u r e d V e l o c i t y . i n c     €      š>     '>     PfU2    œÂ´iÖÓ              @ < P B R E f f e c t _ P S T e x t u r e d V e l o c i t y . i n c     €      š>     '>     ĞfU2    œÂ´iÖÓ              @ < P B R E f f e c t _ P S T e x t u r e d V e l o c i t y . i n c     €      š>     '>     PgU2    œÂ´iÖÓ €            @ < P B R E f f e c t _ P S T e x t u r e d V e l o c i t y . i n c     p      ›>     '>     ĞgU2    õµiÖÓ               0 < P B R E f f e c t _ V S C o n s t a n t . i n c     p      ›>     '>     @hU2    õµiÖÓ              0 < P B R E f f e c t _ V S C o n s t a n t . i n c     p      ›>     '>     °hU2    8µiÖÓ              0 < P B R E f f e c t _ V S C o n s t a n t . i n c     p      ›>     '>      iU2    8µiÖÓ              0 < P B R E f f e c t _ V S C o n s t a n t . i n c     p      ›>     '>     iU2    8µiÖÓ €            0 < P B R E f f e c t _ V S C o n s t a n t . i n c     p      œ>     '>      jU2    r†µiÖÓ               4 < P B R E f f e c t _ V S C o n s t a n t B n . i n c p      œ>     '>     pjU2    r†µiÖÓ              4 < P B R E f f e c t _ V S C o n s t a n t B n . i n c p      œ>     '>     àjU2    r†µiÖÓ              4 < P B R E f f e c t _ V S C o n s t a n t B n . i n c p      œ>     '>     PkU2    r†µiÖÓ              4 < P B R E f f e c t _ V S C o n s t a n t B n . i n c p      œ>     '>     ÀkU2    r†µiÖÓ €            4 < P B R E f f e c t _ V S C o n s t a n t B n . i n c €      >     '>     0lU2    ÏÔµiÖÓ               @ < P B R E f f e c t _ V S C o n s t a n t V e l o c i t y . i n c     €      >     '>     °lU2    ÏÔµiÖÓ              @ < P B R E f f e c t _ V S C o n s t a n t V e l o c i t y . i n c     €      >     '>     0mU2    ÏÔµiÖÓ              @ < P B R E f f e c t _ V S C o n s t a n t V e l o c i t y . i n c     €      >     '>     °mU2    ÏÔµiÖÓ              @ < P B R E f f e c t _ V S C o n s t a n t V e l o c i t y . i n c     €      >     '>     0nU2    ÏÔµiÖÓ €            @ < P B R E f f e c t _ V S C o n s t a n t V e l o c i t y . i n c     €      >     '>     °nU2    #¶iÖÓ               D < P B R E f f e c t _ V S C o n s t a n t V e l o c i t y B n . i n c €      >     '>     0oU2    #¶iÖÓ              D < P B R E f f e c t _ V S C o n s t a n t V e l o c i t y B n . i n c                                                                                 €      >     '>      pU2    #¶iÖÓ              D < P B R E f f e c t _ V S C o n s t a n t V e l o c i t y B n . i n c €      >     '>     €pU2    CJ¶iÖÓ              D < P B R E f f e c t _ V S C o n s t a n t V e l o c i t y B n . i n c €      >     '>      qU2    CJ¶iÖÓ €            D < P B R E f f e c t _ V S C o n s t a n t V e l o c i t y B n . i n c x      Ÿ>     '>     €qU2    rq¶iÖÓ               6 < P o s t P r o c e s s _ P S B l o o m B l u r . i n c     b x      Ÿ>     '>     øqU2    rq¶iÖÓ              6 < P o s t P r o c e s s _ P S B l o o m B l u r . i n c     b x      Ÿ>     '>     prU2    ¤˜¶iÖÓ              6 < P o s t P r o c e s s _ P S B l o o m B l u r . i n c     b x      Ÿ>     '>     èrU2    ¤˜¶iÖÓ              6 < P o s t P r o c e s s _ P S B l o o m B l u r . i n c     b x      Ÿ>     '>     `sU2    ¤˜¶iÖÓ €            6 < P o s t P r o c e s s _ P S B l o o m B l u r . i n c     b x       >     '>     ØsU2    qÀ¶iÖÓ               < < P o s t P r o c e s s _ P S B l o o m C o m b i n e . i n c x       >     '>     PtU2    êæ¶iÖÓ              < < P o s t P r o c e s s _ P S B l o o m C o m b i n e . i n c x       >     '>     ÈtU2    Øí¶iÖÓ              < < P o s t P r o c e s s _ P S B l o o m C o m b i n e . i n c x       >     '>     @uU2    Øí¶iÖÓ              < < P o s t P r o c e s s _ P S B l o o m C o m b i n e . i n c x       >     '>     ¸uU2    Øí¶iÖÓ €            < < P o s t P r o c e s s _ P S B l o o m C o m b i n e . i n c x      ¡>     '>     0vU2    -<·iÖÓ               < < P o s t P r o c e s s _ P S B l o o m E x t r a c t . i n c x      ¡>     '>     ¨vU2    -<·iÖÓ              < < P o s t P r o c e s s _ P S B l o o m E x t r a c t . i n c x      ¡>     '>      wU2    -<·iÖÓ              < < P o s t P r o c e s s _ P S B l o o m E x t r a c t . i n c x      ¡>     '>     ˜wU2    -<·iÖÓ              < < P o s t P r o c e s s _ P S B l o o m E x t r a c t . i n c x      ¡>     '>     xU2    -<·iÖÓ €            < < P o s t P r o c e s s _ P S B l o o m E x t r a c t . i n c h      ¢>     '>     ˆxU2    ‚Š·iÖÓ               , < P o s t P r o c e s s _ P S C o p y . i n c h      ¢>     '>     ğxU2    ‚Š·iÖÓ              , < P o s t P r o c e s s _ P S C o p y . i n c h      ¢>     '>     XyU2    ‚Š·iÖÓ              , < P o s t P r o c e s s _ P S C o p y . i n c h      ¢>     '>     ÀyU2     ²·iÖÓ              , < P o s t P r o c e s s _ P S C o p y . i n c h      ¢>     '>     (zU2     ²·iÖÓ €            , < P o s t P r o c e s s _ P S C o p y . i n c x      £>     '>     zU2    ÔØ·iÖÓ               < < P o s t P r o c e s s _ P S D o w n S c a l e 2 x 2 . i n c x      £>     '>     {U2    ÔØ·iÖÓ              < < P o s t P r o c e s s _ P S D o w n S c a l e 2 x 2 . i n c x      £>     '>     €{U2    úÿ·iÖÓ              < < P o s t P r o c e s s _ P S D o w n S c a l e 2 x 2 . i n c x      £>     '>     ø{U2    úÿ·iÖÓ              < < P o s t P r o c e s s _ P S D o w n S c a l e 2 x 2 . i n c x      £>     '>     p|U2    úÿ·iÖÓ €            < < P o s t P r o c e s s _ P S D o w n S c a l e 2 x 2 . i n c x      ¤>     '>     è|U2    -'¸iÖÓ               < < P o s t P r o c e s s _ P S D o w n S c a l e 4 x 4 . i n c x      ¤>     '>     `}U2    -'¸iÖÓ              < < P o s t P r o c e s s _ P S D o w n S c a l e 4 x 4 . i n c x      ¤>     '>     Ø}U2    PN¸iÖÓ              < < P o s t P r o c e s s _ P S D o w n S c a l e 4 x 4 . i n c x      ¤>     '>     P~U2    PN¸iÖÓ              < < P o s t P r o c e s s _ P S D o w n S c a l e 4 x 4 . i n c x      ¤>     '>     È~U2    PN¸iÖÓ €            < < P o s t P r o c e s s _ P S D o w n S c a l e 4 x 4 . i n c €      ¥>     '>     @U2    ©œ¸iÖÓ               B < P o s t P r o c e s s _ P S G a u s s i a n B l u r 5 x 5 . i n c                                                                   €      ¥>     '>      €U2    ©œ¸iÖÓ              B < P o s t P r o c e s s _ P S G a u s s i a n B l u r 5 x 5 . i n c   €      ¥>     '>     €€U2    ©œ¸iÖÓ              B < P o s t P r o c e s s _ P S G a u s s i a n B l u r 5 x 5 . i n c   €      ¥>     '>      U2    ©œ¸iÖÓ              B < P o s t P r o c e s s _ P S G a u s s i a n B l u r 5 x 5 . i n c   €      ¥>     '>     €U2    ©œ¸iÖÓ €            B < P o s t P r o c e s s _ P S G a u s s i a n B l u r 5 x 5 . i n c   p      ¦>     '>      ‚U2    şê¸iÖÓ               . < P o s t P r o c e s s _ P S M e r g e . i n c     o p      ¦>     '>     p‚U2    şê¸iÖÓ              . < P o s t P r o c e s s _ P S M e r g e . i n c     o p      ¦>     '>     à‚U2    şê¸iÖÓ              . < P o s t P r o c e s s _ P S M e r g e . i n c     o p      ¦>     '>     PƒU2    şê¸iÖÓ              . < P o s t P r o c e s s _ P S M e r g e . i n c     o p      ¦>     '>     ÀƒU2    şê¸iÖÓ €            . < P o s t P r o c e s s _ P S M e r g e . i n c     o x      §>     '>     0„U2    X9¹iÖÓ               8 < P o s t P r o c e s s _ P S M o n o c h r o m e . i n c     x      §>     '>     ¨„U2    X9¹iÖÓ              8 < P o s t P r o c e s s _ P S M o n o c h r o m e . i n c     x      §>     '>      …U2    ±]»iÖÓ              8 < P o s t P r o c e s s _ P S M o n o c h r o m e . i n c     x      §>     '>     ˜…U2    ±]»iÖÓ              8 < P o s t P r o c e s s _ P S M o n o c h r o m e . i n c     x      §>     '>     †U2    ±]»iÖÓ €            8 < P o s t P r o c e s s _ P S M o n o c h r o m e . i n c     p      ¨>     '>     ˆ†U2    
¬»iÖÓ               . < P o s t P r o c e s s _ P S S e p i a . i n c       p      ¨>     '>     ø†U2    
¬»iÖÓ              . < P o s t P r o c e s s _ P S S e p i a . i n c       p      ¨>     '>     h‡U2    )Ó»iÖÓ              . < P o s t P r o c e s s _ P S S e p i a . i n c       p      ¨>     '>     Ø‡U2    )Ó»iÖÓ              . < P o s t P r o c e s s _ P S S e p i a . i n c       p      ¨>     '>     HˆU2    )Ó»iÖÓ €            . < P o s t P r o c e s s _ P S S e p i a . i n c       h      ©>     '>     ¸ˆU2    Xú»iÖÓ               , < P o s t P r o c e s s _ V S Q u a d . i n c h      ©>     '>      ‰U2    Xú»iÖÓ              , < P o s t P r o c e s s _ V S Q u a d . i n c h      ©>     '>     ˆ‰U2    !¼iÖÓ              , < P o s t P r o c e s s _ V S Q u a d . i n c h      ©>     '>     ğ‰U2    !¼iÖÓ              , < P o s t P r o c e s s _ V S Q u a d . i n c h      ©>     '>     XŠU2    !¼iÖÓ €            , < P o s t P r o c e s s _ V S Q u a d . i n c       ª>     '>     ÀŠU2    Ôo¼iÖÓ               P < S k i n n e d E f f e c t _ P S S k i n n e d P i x e l L i g h t i n g . i n c           ª>     '>     P‹U2    Ôo¼iÖÓ              P < S k i n n e d E f f e c t _ P S S k i n n e d P i x e l L i g h t i n g . i n c           ª>     '>     à‹U2    —¼iÖÓ              P < S k i n n e d E f f e c t _ P S S k i n n e d P i x e l L i g h t i n g . i n c           ª>     '>     pŒU2    —¼iÖÓ              P < S k i n n e d E f f e c t _ P S S k i n n e d P i x e l L i g h t i n g . i n c           ª>     '>      U2    —¼iÖÓ €            P < S k i n n e d E f f e c t _ P S S k i n n e d P i x e l L i g h t i n g . i n c           «>     '>     U2    Xå¼iÖÓ               R < S k i n n e d E f f e c t _ P S S k i n n e d V e r t e x L i g h t i n g . i n c         «>     '>      U2    Xå¼iÖÓ              R < S k i n n e d E f f e c t _ P S S k i n n e d V e r t e x L i g h t i n g . i n c         «>     '>     °U2    ‹½iÖÓ              R < S k i n n e d E f f e c t _ P S S k i n n e d V e r t e x L i g h t i n g . i n c         «>     '>     @U2    ‹½iÖÓ              R < S k i n n e d E f f e c t _ P S S k i n n e d V e r t e x L i g h t i n g . i n c                                                         «>     '>      U2    ‹½iÖÓ €            R < S k i n n e d E f f e c t _ P S S k i n n e d V e r t e x L i g h t i n g . i n c   ˜      ¬>     '>     U2    ‚½iÖÓ               \ < S k i n n e d E f f e c t _ P S S k i n n e d V e r t e x L i g h t i n g N o F o g . i n c ˜      ¬>     '>     (‘U2    -©½iÖÓ              \ < S k i n n e d E f f e c t _ P S S k i n n e d V e r t e x L i g h t i n g N o F o g . i n c ˜      ¬>     '>     À‘U2    ª	¿iÖÓ              \ < S k i n n e d E f f e c t _ P S S k i n n e d V e r t e x L i g h t i n g N o F o g . i n c ˜      ¬>     '>     X’U2    ª	¿iÖÓ              \ < S k i n n e d E f f e c t _ P S S k i n n e d V e r t e x L i g h t i n g N o F o g . i n c ˜      ¬>     '>     ğ’U2    ª	¿iÖÓ €            \ < S k i n n e d E f f e c t _ P S S k i n n e d V e r t e x L i g h t i n g N o F o g . i n c ˜      ­>     '>     ˆ“U2    j€¿iÖÓ               X < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t F o u r B o n e s . i n c     ˜      ­>     '>      ”U2    j€¿iÖÓ              X < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t F o u r B o n e s . i n c     ˜      ­>     '>     ¸”U2    åßÀiÖÓ              X < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t F o u r B o n e s . i n c     ˜      ­>     '>     P•U2    åßÀiÖÓ              X < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t F o u r B o n e s . i n c     ˜      ­>     '>     è•U2    åßÀiÖÓ €            X < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t F o u r B o n e s . i n c     ˜      ®>     '>     €–U2    ÿ-ÁiÖÓ               \ < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t F o u r B o n e s B n . i n c ˜      ®>     '>     —U2    ÿ-ÁiÖÓ              \ < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t F o u r B o n e s B n . i n c ˜      ®>     '>     °—U2    ¥ÁiÖÓ              \ < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t F o u r B o n e s B n . i n c ˜      ®>     '>     H˜U2    ¥ÁiÖÓ              \ < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t F o u r B o n e s B n . i n c ˜      ®>     '>     à˜U2    ¥ÁiÖÓ €            \ < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t F o u r B o n e s B n . i n c       ¯>     '>     x™U2    ØñÁiÖÓ               T < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t O n e B o n e . i n c       ¯>     '>     šU2    ØñÁiÖÓ              T < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t O n e B o n e . i n c       ¯>     '>     ˜šU2    ‹ÃiÖÓ              T < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t O n e B o n e . i n c       ¯>     '>     (›U2    ‹ÃiÖÓ              T < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t O n e B o n e . i n c       ¯>     '>     ¸›U2    ‹ÃiÖÓ €            T < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t O n e B o n e . i n c ˜      °>     '>     HœU2    |ÃiÖÓ               X < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t O n e B o n e B n . i n c     ˜      °>     '>     àœU2    |ÃiÖÓ              X < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t O n e B o n e B n . i n c     ˜      °>     '>     xU2    ÌÃiÖÓ              X < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t O n e B o n e B n . i n c     ˜      °>     '>     U2    ÌÃiÖÓ              X < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t O n e B o n e B n . i n c     ˜      °>     '>     ¨U2    ÌÃiÖÓ €            X < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t O n e B o n e B n . i n c     ˜      ±>     '>     @ŸU2    ºÄiÖÓ               V < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t T w o B o n e s . i n c     e                                         ˜      ±>     '>       U2    ºÄiÖÓ              V < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t T w o B o n e s . i n c     e ˜      ±>     '>     ˜ U2    ÃTÅiÖÓ              V < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t T w o B o n e s . i n c     e ˜      ±>     '>     0¡U2    ÃTÅiÖÓ              V < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t T w o B o n e s . i n c     e ˜      ±>     '>     È¡U2    ~zÅiÖÓ €            V < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t T w o B o n e s . i n c     e ˜      ²>     '>     `¢U2    ÇÈÅiÖÓ               Z < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t T w o B o n e s B n . i n c   ˜      ²>     '>     ø¢U2    ÇÈÅiÖÓ              Z < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t T w o B o n e s B n . i n c   ˜      ²>     '>     £U2    ]ñÅiÖÓ              Z < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t T w o B o n e s B n . i n c   ˜      ²>     '>     (¤U2    ]ñÅiÖÓ              Z < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t T w o B o n e s B n . i n c   ˜      ²>     '>     À¤U2    ]ñÅiÖÓ €            Z < S k i n n e d E f f e c t _ V S S k i n n e d O n e L i g h t T w o B o n e s B n . i n c          ³>     '>     X¥U2    K>ÆiÖÓ               b < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g F o u r B o n e s . i n c          ³>     '>     ø¥U2    K>ÆiÖÓ              b < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g F o u r B o n e s . i n c          ³>     '>     ˜¦U2    ÆiÖÓ              b < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g F o u r B o n e s . i n c          ³>     '>     8§U2    ÆiÖÓ              b < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g F o u r B o n e s . i n c          ³>     '>     Ø§U2    Ç³ÆiÖÓ €            b < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g F o u r B o n e s . i n c   ¨      ´>     '>     x¨U2    =àÆiÖÓ               f < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g F o u r B o n e s B n . i n c     e ¨      ´>     '>      ©U2    dÇiÖÓ              f < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g F o u r B o n e s B n . i n c     e ¨      ´>     '>     È©U2    ZÉiÖÓ              f < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g F o u r B o n e s B n . i n c     e ¨      ´>     '>     pªU2    ZÉiÖÓ              f < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g F o u r B o n e s B n . i n c     e ¨      ´>     '>     «U2    ZÉiÖÓ €            f < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g F o u r B o n e s B n . i n c     e        µ>     '>     À«U2    [ÌÉiÖÓ               ^ < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g O n e B o n e . i n c              µ>     '>     `¬U2    [ÌÉiÖÓ              ^ < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g O n e B o n e . i n c              µ>     '>      ­U2    ÙóÉiÖÓ              ^ < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g O n e B o n e . i n c              µ>     '>      ­U2    ÙóÉiÖÓ              ^ < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g O n e B o n e . i n c              µ>     '>     @®U2    ÙóÉiÖÓ €            ^ < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g O n e B o n e . i n c              ¶>     '>     à®U2    v¸ÌiÖÓ               b < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g O n e B o n e B n . i n c                                                                                                                                          ¶>     '>      °U2    v¸ÌiÖÓ              b < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g O n e B o n e B n . i n c          ¶>     '>      °U2    v¸ÌiÖÓ              b < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g O n e B o n e B n . i n c          ¶>     '>     @±U2    v¸ÌiÖÓ              b < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g O n e B o n e B n . i n c          ¶>     '>     à±U2    ˜ßÌiÖÓ €            b < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g O n e B o n e B n . i n c          ·>     '>     €²U2    2HÍiÖÓ               ` < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g T w o B o n e s . i n c            ·>     '>      ³U2    2HÍiÖÓ              ` < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g T w o B o n e s . i n c            ·>     '>     À³U2    şJÎiÖÓ              ` < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g T w o B o n e s . i n c            ·>     '>     `´U2    şJÎiÖÓ              ` < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g T w o B o n e s . i n c            ·>     '>      µU2    şJÎiÖÓ €            ` < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g T w o B o n e s . i n c            ¸>     '>      µU2    7ÀÎiÖÓ               d < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g T w o B o n e s B n . i n c        ¸>     '>     @¶U2    7ÀÎiÖÓ              d < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g T w o B o n e s B n . i n c        ¸>     '>     à¶U2    qçÎiÖÓ              d < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g T w o B o n e s B n . i n c        ¸>     '>     €·U2    qçÎiÖÓ              d < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g T w o B o n e s B n . i n c        ¸>     '>      ¸U2    qçÎiÖÓ €            d < S k i n n e d E f f e c t _ V S S k i n n e d P i x e l L i g h t i n g T w o B o n e s B n . i n c        ¹>     '>     À¸U2    »5ÏiÖÓ               d < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g F o u r B o n e s . i n c        ¹>     '>     `¹U2    »5ÏiÖÓ              d < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g F o u r B o n e s . i n c        ¹>     '>      ºU2    €8ÑiÖÓ              d < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g F o u r B o n e s . i n c        ¹>     '>      ºU2    €8ÑiÖÓ              d < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g F o u r B o n e s . i n c        ¹>     '>     @»U2    ò`ÑiÖÓ €            d < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g F o u r B o n e s . i n c ¨      º>     '>     à»U2    £sÒiÖÓ               h < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g F o u r B o n e s B n . i n c     ¨      º>     '>     ˆ¼U2    £sÒiÖÓ              h < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g F o u r B o n e s B n . i n c     ¨      º>     '>     0½U2    M7ÓiÖÓ              h < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g F o u r B o n e s B n . i n c     ¨      º>     '>     Ø½U2    ¸ŒÓiÖÓ              h < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g F o u r B o n e s B n . i n c     ¨      º>     '>     €¾U2    ¸ŒÓiÖÓ €            h < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g F o u r B o n e s B n . i n c            »>     '>     (¿U2    DÔiÖÓ               ` < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g O n e B o n e . i n c                                                                    »>     '>      ÀU2    DÔiÖÓ              ` < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g O n e B o n e . i n c            »>     '>      ÀU2    f)ÔiÖÓ              ` < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g O n e B o n e . i n c            »>     '>     @ÁU2    f)ÔiÖÓ              ` < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g O n e B o n e . i n c            »>     '>     àÁU2    f)ÔiÖÓ €            ` < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g O n e B o n e . i n c            ¼>     '>     €ÂU2    ãÔiÖÓ               d < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g O n e B o n e B n . i n c        ¼>     '>      ÃU2    ãÔiÖÓ              d < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g O n e B o n e B n . i n c        ¼>     '>     ÀÃU2    Å
ÕiÖÓ              d < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g O n e B o n e B n . i n c        ¼>     '>     `ÄU2    Å
ÕiÖÓ              d < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g O n e B o n e B n . i n c        ¼>     '>      ÅU2    QËÕiÖÓ €            d < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g O n e B o n e B n . i n c        ½>     '>      ÅU2    Õ@ÖiÖÓ               b < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g T w o B o n e s . i n c          ½>     '>     @ÆU2    Õ@ÖiÖÓ              b < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g T w o B o n e s . i n c          ½>     '>     àÆU2    Õ@ÖiÖÓ              b < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g T w o B o n e s . i n c          ½>     '>     €ÇU2    Õ@ÖiÖÓ              b < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g T w o B o n e s . i n c          ½>     '>      ÈU2    hÖiÖÓ €            b < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g T w o B o n e s . i n c   ¨      ¾>     '>     ÀÈU2    z×iÖÓ               f < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g T w o B o n e s B n . i n c     e ¨      ¾>     '>     hÉU2    z×iÖÓ              f < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g T w o B o n e s B n . i n c     e ¨      ¾>     '>     ÊU2    /PÙiÖÓ              f < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g T w o B o n e s B n . i n c     e ¨      ¾>     '>     ¸ÊU2    /PÙiÖÓ              f < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g T w o B o n e s B n . i n c     e ¨      ¾>     '>     `ËU2    |ÙiÖÓ €            f < S k i n n e d E f f e c t _ V S S k i n n e d V e r t e x L i g h t i n g T w o B o n e s B n . i n c     e €      ¿>     '>     ÌU2    ôÚiÖÓ               D < S p r i t e E f f e c t _ S p r i t e P i x e l S h a d e r . i n c €      ¿>     '>     ˆÌU2    ôÚiÖÓ              D < S p r i t e E f f e c t _ S p r i t e P i x e l S h a d e r . i n c €      ¿>     '>     ÍU2    ôÚiÖÓ              D < S p r i t e E f f e c t _ S p r i t e P i x e l S h a d e r . i n c €      ¿>     '>     ˆÍU2    ôÚiÖÓ              D < S p r i t e E f f e c t _ S p r i t e P i x e l S h a d e r . i n c €      ¿>     '>     ÎU2    ÙcÚiÖÓ €            D < S p r i t e E f f e c t _ S p r i t e P i x e l S h a d e r . i n c ˆ      À>     '>     ˆÎU2    nÂÚiÖÓ               F < S p r i t e E f f e c t _ S p r i t e V e r t e x S h a d e r . i n c       ˆ      À>     '>     ÏU2    nÂÚiÖÓ              F < S p r i t e E f f e c t _ S p r i t e V e r t e x S h a d e r . i n c                                                                                                               ˆ      À>     '>      ĞU2    nÂÚiÖÓ              F < S p r i t e E f f e c t _ S p r i t e V e r t e x S h a d e r . i n c       ˆ      À>     '>     ˆĞU2    nÂÚiÖÓ              F < S p r i t e E f f e c t _ S p r i t e V e r t e x S h a d e r . i n c       ˆ      À>     '>     ÑU2    nÂÚiÖÓ €            F < S p r i t e E f f e c t _ S p r i t e V e r t e x S h a d e r . i n c       p      Á>     '>     ˜ÑU2    ÀÛiÖÓ               0 < T o n e M a p _ P S A C E S F i l m i c . i n c     p      Á>     '>     ÒU2    ÀÛiÖÓ              0 < T o n e M a p _ P S A C E S F i l m i c . i n c     p      Á>     '>     xÒU2    ÀÛiÖÓ              0 < T o n e M a p _ P S A C E S F i l m i c . i n c     p      Á>     '>     èÒU2    ÀÛiÖÓ              0 < T o n e M a p _ P S A C E S F i l m i c . i n c     p      Á>     '>     XÓU2    ÀÛiÖÓ €            0 < T o n e M a p _ P S A C E S F i l m i c . i n c     x      Â>     '>     ÈÓU2    _ÛiÖÓ               : < T o n e M a p _ P S A C E S F i l m i c _ S R G B . i n c   x      Â>     '>     @ÔU2    _ÛiÖÓ              : < T o n e M a p _ P S A C E S F i l m i c _ S R G B . i n c   x      Â>     '>     ¸ÔU2    _ÛiÖÓ              : < T o n e M a p _ P S A C E S F i l m i c _ S R G B . i n c   x      Â>     '>     0ÕU2    _ÛiÖÓ              : < T o n e M a p _ P S A C E S F i l m i c _ S R G B . i n c   x      Â>     '>     ¨ÕU2    @†ÛiÖÓ €            : < T o n e M a p _ P S A C E S F i l m i c _ S R G B . i n c   `      Ã>     '>      ÖU2    O²ÛiÖÓ               $ < T o n e M a p _ P S C o p y . i n c `      Ã>     '>     €ÖU2    O²ÛiÖÓ              $ < T o n e M a p _ P S C o p y . i n c `      Ã>     '>     àÖU2    O²ÛiÖÓ              $ < T o n e M a p _ P S C o p y . i n c `      Ã>     '>     @×U2    O²ÛiÖÓ              $ < T o n e M a p _ P S C o p y . i n c `      Ã>     '>      ×U2    O²ÛiÖÓ €            $ < T o n e M a p _ P S C o p y . i n c h      Ä>     '>      ØU2    .ÜiÖÓ               & < T o n e M a p _ P S H D R 1 0 . i n c       h      Ä>     '>     hØU2    .ÜiÖÓ              & < T o n e M a p _ P S H D R 1 0 . i n c       h      Ä>     '>     ĞØU2    .ÜiÖÓ              & < T o n e M a p _ P S H D R 1 0 . i n c       h      Ä>     '>     8ÙU2    .ÜiÖÓ              & < T o n e M a p _ P S H D R 1 0 . i n c       h      Ä>     '>      ÙU2    .ÜiÖÓ €            & < T o n e M a p _ P S H D R 1 0 . i n c       h      Å>     '>     ÚU2    „OÜiÖÓ               , < T o n e M a p _ P S R e i n h a r d . i n c h      Å>     '>     pÚU2    „OÜiÖÓ              , < T o n e M a p _ P S R e i n h a r d . i n c h      Å>     '>     ØÚU2    „OÜiÖÓ              , < T o n e M a p _ P S R e i n h a r d . i n c h      Å>     '>     @ÛU2    „OÜiÖÓ              , < T o n e M a p _ P S R e i n h a r d . i n c h      Å>     '>     ¨ÛU2    „OÜiÖÓ €            , < T o n e M a p _ P S R e i n h a r d . i n c x      Æ>     '>     ÜU2    áÜiÖÓ               6 < T o n e M a p _ P S R e i n h a r d _ S R G B . i n c       x      Æ>     '>     ˆÜU2    áÜiÖÓ              6 < T o n e M a p _ P S R e i n h a r d _ S R G B . i n c       x      Æ>     '>      İU2    ÅÜiÖÓ              6 < T o n e M a p _ P S R e i n h a r d _ S R G B . i n c       x      Æ>     '>     xİU2    ÅÜiÖÓ              6 < T o n e M a p _ P S R e i n h a r d _ S R G B . i n c       x      Æ>     '>     ğİU2    ÅÜiÖÓ €            6 < T o n e M a p _ P S R e i n h a r d _ S R G B . i n c       h      Ç>     '>     hŞU2    „:İiÖÓ               , < T o n e M a p _ P S S a t u r a t e . i n c h      Ç>     '>     ĞŞU2    „:İiÖÓ              , < T o n e M a p _ P S S a t u r a t e . i n c h      Ç>     '>     8ßU2    „:İiÖÓ              , < T o n e M a p _ P S S a t u r a t e . i n c                                                                                                 h      Ç>     '>      àU2    „:İiÖÓ              , < T o n e M a p _ P S S a t u r a t e . i n c h      Ç>     '>     hàU2    „:İiÖÓ €            , < T o n e M a p _ P S S a t u r a t e . i n c x      È>     '>     ĞàU2    †ØİiÖÓ               6 < T o n e M a p _ P S S a t u r a t e _ S R G B . i n c     4 x      È>     '>     HáU2    †ØİiÖÓ              6 < T o n e M a p _ P S S a t u r a t e _ S R G B . i n c     4 x      È>     '>     ÀáU2    †ØİiÖÓ              6 < T o n e M a p _ P S S a t u r a t e _ S R G B . i n c     4 x      È>     '>     8âU2    †ØİiÖÓ              6 < T o n e M a p _ P S S a t u r a t e _ S R G B . i n c     4 x      È>     '>     °âU2    †ØİiÖÓ €            6 < T o n e M a p _ P S S a t u r a t e _ S R G B . i n c     4 h      É>     '>     (ãU2    aÔßiÖÓ               & < T o n e M a p _ P S _ S R G B . i n c     y h      É>     '>     ãU2    aÔßiÖÓ              & < T o n e M a p _ P S _ S R G B . i n c     y h      É>     '>     øãU2    aÔßiÖÓ              & < T o n e M a p _ P S _ S R G B . i n c     y h      É>     '>     `äU2    aÔßiÖÓ              & < T o n e M a p _ P S _ S R G B . i n c     y h      É>     '>     ÈäU2    aÔßiÖÓ €            & < T o n e M a p _ P S _ S R G B . i n c     y `      Ê>     '>     0åU2    ¶"àiÖÓ               $ < T o n e M a p _ V S Q u a d . i n c `      Ê>     '>     åU2    ¶"àiÖÓ              $ < T o n e M a p _ V S Q u a d . i n c `      Ê>     '>     ğåU2    ¶"àiÖÓ              $ < T o n e M a p _ V S Q u a d . i n c `      Ê>     '>     PæU2    ¶"àiÖÓ              $ < T o n e M a p _ V S Q u a d . i n c `      Ê>     '>     °æU2    ¶"àiÖÓ €            $ < T o n e M a p _ V S Q u a d . i n c P      Ë>     q=     çU2    3˜àiÖÓ               < X W B T o o l       P      Ë>     q=     `çU2    3˜àiÖÓ  €            < X W B T o o l       P      Ë>     q=     °çU2    ©¬àiÖÓ €              < X W B T o o l       P      Ë>     q=      èU2    ©¬àiÖÓ € €            < X W B T o o l       X      Ì>     Ë>     PèU2    şúàiÖÓ                < x w b t o o l . c p p     U X      Ì>     Ë>     ¨èU2    şúàiÖÓ               < x w b t o o l . c p p     U X      Ì>     Ë>      éU2    şúàiÖÓ               < x w b t o o l . c p p     U X      Ì>     Ë>     XéU2    şúàiÖÓ               < x w b t o o l . c p p     U X      Ì>     Ë>     °éU2    şúàiÖÓ €             < x w b t o o l . c p p     U x      Í>     Ë>     êU2    LIáiÖÓ               8 < x w b t o o l _ D e s k t o p _ 2 0 1 3 . v c x p r o j     x      Í>     Ë>     €êU2    LIáiÖÓ              8 < x w b t o o l _ D e s k t o p _ 2 0 1 3 . v c x p r o j     x      Í>     Ë>     øêU2    LIáiÖÓ              8 < x w b t o o l _ D e s k t o p _ 2 0 1 3 . v c x p r o j     x      Í>     Ë>     pëU2    LIáiÖÓ              8 < x w b t o o l _ D e s k t o p _ 2 0 1 3 . v c x p r o j     x      Í>     Ë>     èëU2    LIáiÖÓ €            8 < x w b t o o l _ D e s k t o p _ 2 0 1 3 . v c x p r o j     ˆ      Î>     Ë>     `ìU2    Ô¾áiÖÓ               H < x w b t o o l _ D e s k t o p _ 2 0 1 3 . v c x p r o j . f i l t e r s     ˆ      Î>     Ë>     èìU2    Ô¾áiÖÓ              H < x w b t o o l _ D e s k t o p _ 2 0 1 3 . v c x p r o j . f i l t e r s     ˆ      Î>     Ë>     píU2    Ô¾áiÖÓ              H < x w b t o o l _ D e s k t o p _ 2 0 1 3 . v c x p r o j . f i l t e r s     ˆ      Î>     Ë>     øíU2    Ô¾áiÖÓ              H < x w b t o o l _ D e s k t o p _ 2 0 1 3 . v c x p r o j . f i l t e r s     ˆ      Î>     Ë>     €îU2    ûåáiÖÓ €            H < x w b t o o l _ D e s k t o p _ 2 0 1 3 . v c x p r o j . f i l t e r s     x      Ï>     Ë>     ïU2    T4âiÖÓ               8 < x w b t o o l _ D e s k t o p _ 2 0 1 5 . v c x p r o j     x      Ï>     Ë>     €ïU2    T4âiÖÓ              8 < x w b t o o l _ D e s k t o p _ 2 0 1 5 . v c x p r o j             x      Ï>     Ë>      ğU2    T4âiÖÓ              8 < x w b t o o l _ D e s k t o p _ 2 0 1 5 . v c x p r o j     x      Ï>     Ë>     xğU2    T4âiÖÓ              8 < x w b t o o l _ D e s k t o p _ 2 0 1 5 . v c x p r o j     x      Ï>     Ë>     ğğU2    Š[âiÖÓ €            8 < x w b t o o l _ D e s k t o p _ 2 0 1 5 . v c x p r o j     ˆ      Ğ>     Ë>     hñU2    BÒâiÖÓ               H < x w b t o o l _ D e s k t o p _ 2 0 1 5 . v c x p r o j . f i l t e r s     ˆ      Ğ>     Ë>     ğñU2    BÒâiÖÓ              H < x w b t o o l _ D e s k t o p _ 2 0 1 5 . v c x p r o j . f i l t e r s     ˆ      Ğ>     Ë>     xòU2    BÒâiÖÓ              H < x w b t o o l _ D e s k t o p _ 2 0 1 5 . v c x p r o j . f i l t e r s     ˆ      Ğ>     Ë>      óU2    BÒâiÖÓ              H < x w b t o o l _ D e s k t o p _ 2 0 1 5 . v c x p r o j . f i l t e r s     ˆ      Ğ>     Ë>     ˆóU2    BÒâiÖÓ €            H < x w b t o o l _ D e s k t o p _ 2 0 1 5 . v c x p r o j . f i l t e r s     x      Ñ>     Ë>     ôU2    TãiÖÓ               8 < x w b t o o l _ D e s k t o p _ 2 0 1 7 . v c x p r o j     x      Ñ>     Ë>     ˆôU2    TãiÖÓ              8 < x w b t o o l _ D e s k t o p _ 2 0 1 7 . v c x p r o j     x      Ñ>     Ë>      õU2    ºFãiÖÓ              8 < x w b t o o l _ D e s k t o p _ 2 0 1 7 . v c x p r o j     x      Ñ>     Ë>     xõU2    ºFãiÖÓ              8 < x w b t o o l _ D e s k t o p _ 2 0 1 7 . v c x p r o j     x      Ñ>     Ë>     ğõU2    ºFãiÖÓ €            8 < x w b t o o l _ D e s k t o p _ 2 0 1 7 . v c x p r o j     ˆ      Ò>     Ë>     höU2    ©mãiÖÓ               H < x w b t o o l _ D e s k t o p _ 2 0 1 7 . v c x p r o j . f i l t e r s     ˆ      Ò>     Ë>     ğöU2    Ğ”ãiÖÓ              H < x w b t o o l _ D e s k t o p _ 2 0 1 7 . v c x p r o j . f i l t e r s     ˆ      Ò>     Ë>     x÷U2    Ğ”ãiÖÓ              H < x w b t o o l _ D e s k t o p _ 2 0 1 7 . v c x p r o j . f i l t e r s     ˆ      Ò>     Ë>      øU2    Ğ”ãiÖÓ              H < x w b t o o l _ D e s k t o p _ 2 0 1 7 . v c x p r o j . f i l t e r s     ˆ      Ò>     Ë>     ˆøU2    Ğ”ãiÖÓ €            H < x w b t o o l _ D e s k t o p _ 2 0 1 7 . v c x p r o j . f i l t e r s     P      Ó>     Ø      ùU2    %ããiÖÓ               < p a c k a g e s     P      Ó>     Ø      `ùU2    %ããiÖÓ  €            < p a c k a g e s     P      Ó>     Ø      °ùU2    
äiÖÓ €              < p a c k a g e s     P      Ó>     Ø       úU2    
äiÖÓ € €            < p a c k a g e s     ˆ      Ô>     Ó>     PúU2    ¥XäiÖÓ              F < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . 2 0 1 7 . 1 2 . 1 3 . 1       ˆ      Ô>     Ó>     ØúU2    ¥XäiÖÓ  €           F < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . 2 0 1 7 . 1 2 . 1 3 . 1       ˆ      Ô>     Ó>     `ûU2    àäiÖÓ €             F < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . 2 0 1 7 . 1 2 . 1 3 . 1       ˆ      Ô>     Ó>     èûU2    àäiÖÓ € €           F < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . 2 0 1 7 . 1 2 . 1 3 . 1             Õ>     Ô>     püU2    §äiÖÓ               R < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . 2 0 1 7 . 1 2 . 1 3 . 1 . n u p k g         Õ>     Ô>      ıU2    §äiÖÓ              R < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . 2 0 1 7 . 1 2 . 1 3 . 1 . n u p k g         Õ>     Ô>     ıU2    ƒİçiÖÓ              R < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . 2 0 1 7 . 1 2 . 1 3 . 1 . n u p k g         Õ>     Ô>      şU2    …jÖÓ              R < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . 2 0 1 7 . 1 2 . 1 3 . 1 . n u p k g         Õ>     Ô>     °şU2    J¬jÖÓ €            R < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . 2 0 1 7 . 1 2 . 1 3 . 1 . n u p k g   H      Ö>     Ô>     @ÿU2    sjÖÓ              
 < b u i l d   H      Ö>     Ô>     ˆÿU2    sjÖÓ  €           
 < b u i l d                                                   H      Ö>     Ô>       V2    :)jÖÓ €             
 < b u i l d   H      Ö>     Ô>     H V2    :)jÖÓ € €           
 < b u i l d   h      ×>     Ö>      V2    ›wjÖÓ               , < c o n f i g u r a t i o n s . a u t o p k g h      ×>     Ö>     ø V2    ¾jÖÓ              , < c o n f i g u r a t i o n s . a u t o p k g h      ×>     Ö>     `V2    ¾jÖÓ              , < c o n f i g u r a t i o n s . a u t o p k g h      ×>     Ö>     ÈV2    ¾jÖÓ              , < c o n f i g u r a t i o n s . a u t o p k g h      ×>     Ö>     0V2    ¾jÖÓ €            , < c o n f i g u r a t i o n s . a u t o p k g `      Ø>     Ö>     ˜V2    ];jÖÓ               $ < p u b l i s h e r - i n f o . t x t `      Ø>     Ö>     øV2    ];jÖÓ              $ < p u b l i s h e r - i n f o . t x t `      Ø>     Ö>     XV2    ];jÖÓ              $ < p u b l i s h e r - i n f o . t x t `      Ø>     Ö>     ¸V2    ];jÖÓ              $ < p u b l i s h e r - i n f o . t x t `      Ø>     Ö>     V2    ];jÖÓ €            $ < p u b l i s h e r - i n f o . t x t H      Ù>     Ö>     xV2    Æ‰jÖÓ               < n a t i v e H      Ù>     Ö>     ÀV2    Æ‰jÖÓ  €            < n a t i v e H      Ù>     Ö>     V2    é°jÖÓ €              < n a t i v e H      Ù>     Ö>     PV2    é°jÖÓ € €            < n a t i v e p      Ú>     Ù>     ˜V2    2ÿjÖÓ               0 < d e f a u l t - p r o p e r t i e s u i . x m l     p      Ú>     Ù>     V2    2ÿjÖÓ              0 < d e f a u l t - p r o p e r t i e s u i . x m l     p      Ú>     Ù>     xV2    ëjÖÓ              0 < d e f a u l t - p r o p e r t i e s u i . x m l     p      Ú>     Ù>     èV2    ëjÖÓ              0 < d e f a u l t - p r o p e r t i e s u i . x m l     p      Ú>     Ù>     XV2    ëjÖÓ €            0 < d e f a u l t - p r o p e r t i e s u i . x m l     Ø      Û>     Ù>     ÈV2    º_jÖÓ               ˜ < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 - p r o p e r t i e s u i - e e 3 2 7 c 9 6 - 3 0 d 9 - 4 b 5 8 - 9 e 8 c - 1 d c b 0 3 a c 4 e f a . x m l     Ø      Û>     Ù>      V2    º_jÖÓ              ˜ < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 - p r o p e r t i e s u i - e e 3 2 7 c 9 6 - 3 0 d 9 - 4 b 5 8 - 9 e 8 c - 1 d c b 0 3 a c 4 e f a . x m l     Ø      Û>     Ù>     x	V2    º_jÖÓ              ˜ < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 - p r o p e r t i e s u i - e e 3 2 7 c 9 6 - 3 0 d 9 - 4 b 5 8 - 9 e 8 c - 1 d c b 0 3 a c 4 e f a . x m l     Ø      Û>     Ù>     P
V2    º_jÖÓ              ˜ < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 - p r o p e r t i e s u i - e e 3 2 7 c 9 6 - 3 0 d 9 - 4 b 5 8 - 9 e 8 c - 1 d c b 0 3 a c 4 e f a . x m l     Ø      Û>     Ù>     (V2    º_jÖÓ €            ˜ < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 - p r o p e r t i e s u i - e e 3 2 7 c 9 6 - 3 0 d 9 - 4 b 5 8 - 9 e 8 c - 1 d c b 0 3 a c 4 e f a . x m l     x      Ü>     Ù>      V2    :ÕjÖÓ               < < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . t a r g e t s x      Ü>     Ù>     xV2    aüjÖÓ              < < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . t a r g e t s x      Ü>     Ù>     ğV2    aüjÖÓ              < < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . t a r g e t s x      Ü>     Ù>     hV2    aüjÖÓ              < < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . t a r g e t s x      Ü>     Ù>     àV2    aüjÖÓ €            < < d i r e c t x t k _ d e s k t o p _ 2 0 1 5 . t a r g e t s H      İ>     Ù>     XV2    ¶JjÖÓ               < d o c s     H      İ>     Ù>      V2    ¶JjÖÓ  €            < d o c s     H      İ>     Ù>     èV2    ÙqjÖÓ €              < d o c s     H      İ>     Ù>     0V2    ÙqjÖÓ € €            < d o c s     P      Ş>     İ>     xV2    ™jÖÓ                < R e a d m e . t x t                                                         P      Ş>     İ>      V2    ™jÖÓ               < R e a d m e . t x t P      Ş>     İ>     PV2    2ÀjÖÓ               < R e a d m e . t x t P      Ş>     İ>      V2    2ÀjÖÓ               < R e a d m e . t x t P      Ş>     İ>     ğV2    2ÀjÖÓ €             < R e a d m e . t x t P      ß>     Ù>     @V2    jÖÓ               < i n c l u d e       P      ß>     Ù>     V2    jÖÓ  €            < i n c l u d e       P      ß>     Ù>     àV2    º5jÖÓ €              < i n c l u d e       P      ß>     Ù>     0V2    º5jÖÓ € €            < i n c l u d e       P      à>     ß>     €V2    „jÖÓ                < A u d i o . h       P      à>     ß>     ĞV2    „jÖÓ               < A u d i o . h       P      à>     ß>      V2    „jÖÓ               < A u d i o . h       P      à>     ß>     pV2    „jÖÓ               < A u d i o . h       P      à>     ß>     ÀV2    „jÖÓ €             < A u d i o . h       X      á>     ß>     V2    aÒjÖÓ                < C o m m o n S t a t e s . h X      á>     ß>     hV2    aÒjÖÓ               < C o m m o n S t a t e s . h X      á>     ß>     ÀV2    aÒjÖÓ               < C o m m o n S t a t e s . h X      á>     ß>     V2    ˆùjÖÓ               < C o m m o n S t a t e s . h X      á>     ß>     pV2    ˆùjÖÓ €             < C o m m o n S t a t e s . h `      â>     ß>     ÈV2    º jÖÓ               $ < D D S T e x t u r e L o a d e r . h `      â>     ß>     (V2    º jÖÓ              $ < D D S T e x t u r e L o a d e r . h `      â>     ß>     ˆV2    º jÖÓ              $ < D D S T e x t u r e L o a d e r . h `      â>     ß>     èV2    İGjÖÓ              $ < D D S T e x t u r e L o a d e r . h `      â>     ß>     HV2    İGjÖÓ €            $ < D D S T e x t u r e L o a d e r . h `      ã>     ß>     ¨V2    3–jÖÓ                 < D i r e c t X H e l p e r s . h     `      ã>     ß>     V2    3–jÖÓ                < D i r e c t X H e l p e r s . h     `      ã>     ß>     hV2    3–jÖÓ                < D i r e c t X H e l p e r s . h     `      ã>     ß>     ÈV2    3–jÖÓ                < D i r e c t X H e l p e r s . h     `      ã>     ß>     (V2    3–jÖÓ €              < D i r e c t X H e l p e r s . h     P      ä>     ß>     ˆV2    ˆäjÖÓ                < E f f e c t s . h   P      ä>     ß>     ØV2    ˆäjÖÓ               < E f f e c t s . h   P      ä>     ß>     (V2    ³jÖÓ               < E f f e c t s . h   P      ä>     ß>     xV2    ³jÖÓ               < E f f e c t s . h   P      ä>     ß>     ÈV2    ³jÖÓ €             < E f f e c t s . h   P      å>     ß>     V2    ZjÖÓ                < G a m e P a d . h   P      å>     ß>     hV2    3jÖÓ               < G a m e P a d . h   P      å>     ß>     ¸V2    3jÖÓ               < G a m e P a d . h   P      å>     ß>     V2    3jÖÓ               < G a m e P a d . h   P      å>     ß>     XV2    3jÖÓ €             < G a m e P a d . h   h      æ>     ß>     ¨V2    ’jÖÓ               ( < G e o m e t r i c P r i m i t i v e . h     h      æ>     ß>     V2    ˆ¥jÖÓ              ( < G e o m e t r i c P r i m i t i v e . h     h      æ>     ß>     xV2    ˆ¥jÖÓ              ( < G e o m e t r i c P r i m i t i v e . h     h      æ>     ß>     àV2    ˆ¥jÖÓ              ( < G e o m e t r i c P r i m i t i v e . h     h      æ>     ß>     HV2    ˆ¥jÖÓ €            ( < G e o m e t r i c P r i m i t i v e . h     `      ç>     ß>     °V2    ŞójÖÓ                 < G r a p h i c s M e m o r y . h     `      ç>     ß>     V2    ŞójÖÓ                < G r a p h i c s M e m o r y . h     `      ç>     ß>     pV2    ŞójÖÓ                < G r a p h i c s M e m o r y . h                                                     `      ç>     ß>       V2    ŞójÖÓ                < G r a p h i c s M e m o r y . h     `      ç>     ß>     ` V2    ŞójÖÓ €              < G r a p h i c s M e m o r y . h     P      è>     ß>     À V2    7BjÖÓ                < K e y b o a r d . h P      è>     ß>     !V2    7BjÖÓ               < K e y b o a r d . h P      è>     ß>     `!V2    7BjÖÓ               < K e y b o a r d . h P      è>     ß>     °!V2    7BjÖÓ               < K e y b o a r d . h P      è>     ß>      "V2    ^ijÖÓ €             < K e y b o a r d . h P      é>     ß>     P"V2    ŒjÖÓ                < M o d e l . h     r P      é>     ß>      "V2    ŒjÖÓ               < M o d e l . h     r P      é>     ß>     ğ"V2    ¯·jÖÓ               < M o d e l . h     r P      é>     ß>     @#V2    ¯·jÖÓ               < M o d e l . h     r P      é>     ß>     #V2    ¯·jÖÓ €             < M o d e l . h     r P      ê>     ß>     à#V2    âŞjÖÓ                < M o u s e . h       P      ê>     ß>     0$V2    âŞjÖÓ               < M o u s e . h       P      ê>     ß>     €$V2    âŞjÖÓ               < M o u s e . h       P      ê>     ß>     Ğ$V2    âŞjÖÓ               < M o u s e . h       P      ê>     ß>      %V2    jÖÓ €             < M o u s e . h       X      ë>     ß>     p%V2    3-jÖÓ                < P o s t P r o c e s s . h   X      ë>     ß>     È%V2    3-jÖÓ               < P o s t P r o c e s s . h   X      ë>     ß>      &V2    3-jÖÓ               < P o s t P r o c e s s . h   X      ë>     ß>     x&V2    3-jÖÓ               < P o s t P r o c e s s . h   X      ë>     ß>     Ğ&V2    3-jÖÓ €             < P o s t P r o c e s s . h   `      ì>     ß>     ('V2    ˆ{jÖÓ                 < P r i m i t i v e B a t c h . h     `      ì>     ß>     ˆ'V2    ˆ{jÖÓ                < P r i m i t i v e B a t c h . h     `      ì>     ß>     è'V2    ˆ{jÖÓ                < P r i m i t i v e B a t c h . h     `      ì>     ß>     H(V2    ˆ{jÖÓ                < P r i m i t i v e B a t c h . h     `      ì>     ß>     ¨(V2    ˆ{jÖÓ €              < P r i m i t i v e B a t c h . h     X      í>     ß>     )V2    ŞÉjÖÓ                < S c r e e n G r a b . h     X      í>     ß>     `)V2    ŞÉjÖÓ               < S c r e e n G r a b . h     X      í>     ß>     ¸)V2    ŞÉjÖÓ               < S c r e e n G r a b . h     X      í>     ß>     *V2    ŞÉjÖÓ               < S c r e e n G r a b . h     X      í>     ß>     h*V2    ŞÉjÖÓ €             < S c r e e n G r a b . h     X      î>     ß>     À*V2    ñjÖÓ                < S i m p l e M a t h . h     X      î>     ß>     +V2    3jÖÓ               < S i m p l e M a t h . h     X      î>     ß>     p+V2    3jÖÓ               < S i m p l e M a t h . h     X      î>     ß>     È+V2    3jÖÓ               < S i m p l e M a t h . h     X      î>     ß>      ,V2    3jÖÓ €             < S i m p l e M a t h . h     X      ï>     ß>     x,V2    ^?jÖÓ                < S i m p l e M a t h . i n l X      ï>     ß>     Ğ,V2    ˆfjÖÓ               < S i m p l e M a t h . i n l X      ï>     ß>     (-V2    ˆfjÖÓ               < S i m p l e M a t h . i n l X      ï>     ß>     €-V2    ³jÖÓ               < S i m p l e M a t h . i n l X      ï>     ß>     Ø-V2    ³jÖÓ €             < S i m p l e M a t h . i n l X      ğ>     ß>     0.V2    ÜjÖÓ                < S p r i t e B a t c h . h   X      ğ>     ß>     ˆ.V2    ÜjÖÓ               < S p r i t e B a t c h . h   X      ğ>     ß>     à.V2    ÜjÖÓ               < S p r i t e B a t c h . h   X      ğ>     ß>     8/V2    ÜjÖÓ               < S p r i t e B a t c h . h   X      ğ>     ß>     /V2    ÜjÖÓ €             < S p r i t e B a t c h . h                           X      ñ>     ß>      0V2    ^*jÖÓ                < S p r i t e F o n t . h     X      ñ>     ß>     X0V2    ^*jÖÓ               < S p r i t e F o n t . h     X      ñ>     ß>     °0V2    ^*jÖÓ               < S p r i t e F o n t . h     X      ñ>     ß>     1V2    ^*jÖÓ               < S p r i t e F o n t . h     X      ñ>     ß>     `1V2    ^*jÖÓ €             < S p r i t e F o n t . h     X      ò>     ß>     ¸1V2    ³xjÖÓ                < V e r t e x T y p e s . h   X      ò>     ß>     2V2    ³xjÖÓ               < V e r t e x T y p e s . h   X      ò>     ß>     h2V2    ³xjÖÓ               < V e r t e x T y p e s . h   X      ò>     ß>     À2V2    ³xjÖÓ               < V e r t e x T y p e s . h   X      ò>     ß>     3V2    ³xjÖÓ €             < V e r t e x T y p e s . h   `      ó>     ß>     p3V2    ÇjÖÓ               $ < W I C T e x t u r e L o a d e r . h `      ó>     ß>     Ğ3V2    ÇjÖÓ              $ < W I C T e x t u r e L o a d e r . h `      ó>     ß>     04V2    ÇjÖÓ              $ < W I C T e x t u r e L o a d e r . h `      ó>     ß>     4V2    ÇjÖÓ              $ < W I C T e x t u r e L o a d e r . h `      ó>     ß>     ğ4V2    ÇjÖÓ €            $ < W I C T e x t u r e L o a d e r . h h      ô>     ß>     P5V2    ^jÖÓ               , < X b o x D D S T e x t u r e L o a d e r . h h      ô>     ß>     ¸5V2    ^jÖÓ              , < X b o x D D S T e x t u r e L o a d e r . h h      ô>     ß>      6V2    ^jÖÓ              , < X b o x D D S T e x t u r e L o a d e r . h h      ô>     ß>     ˆ6V2    ^jÖÓ              , < X b o x D D S T e x t u r e L o a d e r . h h      ô>     ß>     ğ6V2    ^jÖÓ €            , < X b o x D D S T e x t u r e L o a d e r . h H      õ>     Ù>     X7V2    ³cjÖÓ               < l i b       H      õ>     Ù>      7V2    ³cjÖÓ  €            < l i b       H      õ>     Ù>     è7V2    9‹jÖÓ €              < l i b       H      õ>     Ù>     08V2    9‹jÖÓ € €            < l i b       H      ö>     õ>     x8V2    	²jÖÓ              
 < W i n 3 2   H      ö>     õ>     À8V2    	²jÖÓ  €           
 < W i n 3 2   H      ö>     õ>     9V2    ’ÙjÖÓ €             
 < W i n 3 2   H      ö>     õ>     P9V2    ’ÙjÖÓ € €           
 < W i n 3 2   H      ÷>     ö>     ˜9V2    b jÖÓ              
 < D e b u g   H      ÷>     ö>     à9V2    b jÖÓ  €           
 < D e b u g   H      ÷>     ö>     (:V2    ‰'jÖÓ €             
 < D e b u g   H      ÷>     ö>     p:V2    ‰'jÖÓ € €           
 < D e b u g   X      ø>     ÷>     ¸:V2    %vjÖÓ                < D i r e c t X T K . l i b   X      ø>     ÷>     ;V2    %vjÖÓ               < D i r e c t X T K . l i b   X      ø>     ÷>     h;V2    Z¬ jÖÓ               < D i r e c t X T K . l i b   X      ø>     ÷>     À;V2    9BjÖÓ               < D i r e c t X T K . l i b   X      ø>     ÷>     <V2    9BjÖÓ €             < D i r e c t X T K . l i b   X      ù>     ÷>     p<V2    ş¶BjÖÓ                < d i r e c t x t k . p d b   X      ù>     ÷>     È<V2    ş¶BjÖÓ               < d i r e c t x t k . p d b   X      ù>     ÷>      =V2    ŠIDjÖÓ               < d i r e c t x t k . p d b   X      ù>     ÷>     x=V2    «LjÖÓ               < d i r e c t x t k . p d b   X      ù>     ÷>     Ğ=V2    «LjÖÓ €             < d i r e c t x t k . p d b   h      ú>     ÷>     (>V2    ÕOjÖÓ               ( < D i r e c t X T K A u d i o D X . l i b     h      ú>     ÷>     >V2    ÕOjÖÓ              ( < D i r e c t X T K A u d i o D X . l i b     h      ú>     ÷>     ø>V2     GOjÖÓ              ( < D i r e c t X T K A u d i o D X . l i b     h      ú>     ÷>     `?V2    ¸{YjÖÓ              ( < D i r e c t X T K A u d i o D X . l i b                                                             h      ú>     ÷>      @V2    ¸{YjÖÓ €            ( < D i r e c t X T K A u d i o D X . l i b     h      û>     ÷>     h@V2    ã¢YjÖÓ               ( < d i r e c t x t k a u d i o d x . p d b     h      û>     ÷>     Ğ@V2    ã¢YjÖÓ              ( < d i r e c t x t k a u d i o d x . p d b     h      û>     ÷>     8AV2    +[jÖÓ              ( < d i r e c t x t k a u d i o d x . p d b     h      û>     ÷>      AV2    Øç`jÖÓ              ( < d i r e c t x t k a u d i o d x . p d b     h      û>     ÷>     BV2    Øç`jÖÓ €            ( < d i r e c t x t k a u d i o d x . p d b     P      ü>     ö>     pBV2    …ÑbjÖÓ               < R e l e a s e       P      ü>     ö>     ÀBV2    …ÑbjÖÓ  €            < R e l e a s e       P      ü>     ö>     CV2    öcjÖÓ €              < R e l e a s e       P      ü>     ö>     `CV2    öcjÖÓ € €            < R e l e a s e       X      ı>     ü>     °CV2    ~‹cjÖÓ                < D i r e c t X T K . l i b   X      ı>     ü>     DV2    ~‹cjÖÓ               < D i r e c t X T K . l i b   X      ı>     ü>     `DV2    –fjÖÓ               < D i r e c t X T K . l i b   X      ı>     ü>     ¸DV2    iÎrjÖÓ               < D i r e c t X T K . l i b   X      ı>     ü>     EV2    iÎrjÖÓ €             < D i r e c t X T K . l i b   X      ş>     ü>     hEV2    íCsjÖÓ                < d i r e c t x t k . p d b   X      ş>     ü>     ÀEV2    íCsjÖÓ               < d i r e c t x t k . p d b   X      ş>     ü>     FV2    ÓítjÖÓ               < d i r e c t x t k . p d b   X      ş>     ü>     pFV2    :jÖÓ               < d i r e c t x t k . p d b   X      ş>     ü>     ÈFV2    :jÖÓ €             < d i r e c t x t k . p d b   h      ÿ>     ü>      GV2    œˆjÖÓ               ( < D i r e c t X T K A u d i o D X . l i b     h      ÿ>     ü>     ˆGV2    œˆjÖÓ              ( < D i r e c t X T K A u d i o D X . l i b     h      ÿ>     ü>     ğGV2    Z7jÖÓ              ( < D i r e c t X T K A u d i o D X . l i b     h      ÿ>     ü>     XHV2    Fï‚jÖÓ              ( < D i r e c t X T K A u d i o D X . l i b     h      ÿ>     ü>     ÀHV2    Fï‚jÖÓ €            ( < D i r e c t X T K A u d i o D X . l i b     h       ?     ü>     (IV2    •:ƒjÖÓ               ( < d i r e c t x t k a u d i o d x . p d b     h       ?     ü>     IV2    •:ƒjÖÓ              ( < d i r e c t x t k a u d i o d x . p d b     h       ?     ü>     øIV2    `ˆƒjÖÓ              ( < d i r e c t x t k a u d i o d x . p d b     h       ?     ü>     `JV2    §‡jÖÓ              ( < d i r e c t x t k a u d i o d x . p d b     h       ?     ü>     ÈJV2    §‡jÖÓ €            ( < d i r e c t x t k a u d i o d x . p d b     H      ?     õ>     0KV2    áõ‡jÖÓ               < x 6 4      H      ?     õ>     xKV2    áõ‡jÖÓ  €            < x 6 4      H      ?     õ>     ÀKV2    áõ‡jÖÓ €              < x 6 4      H      ?     õ>     LV2    ˆjÖÓ € €            < x 6 4      H      ?     ?     PLV2    ;DˆjÖÓ              
 < D e b u g   H      ?     ?     ˜LV2    ;DˆjÖÓ  €           
 < D e b u g   H      ?     ?     àLV2    bkˆjÖÓ €             
 < D e b u g   H      ?     ?     (MV2    ’ˆjÖÓ € €           
 < D e b u g   X      ?     ?     pMV2    PáˆjÖÓ                < D i r e c t X T K . l i b   X      ?     ?     ÈMV2    PáˆjÖÓ               < D i r e c t X T K . l i b   X      ?     ?      NV2    ÔŒjÖÓ               < D i r e c t X T K . l i b   X      ?     ?     xNV2    ›ÏjÖÓ               < D i r e c t X T K . l i b   X      ?     ?     ĞNV2    ›ÏjÖÓ €             < D i r e c t X T K . l i b   X      ?     ?     (OV2    ‡åÑjÖÓ                < d i r e c t x t k . p d b   X      ?     ?     €OV2    ‡åÑjÖÓ               < d i r e c t x t k . p d b                                           X      ?     ?      PV2     -ÖjÖÓ               < d i r e c t x t k . p d b   X      ?     ?     XPV2    é6kÖÓ               < d i r e c t x t k . p d b   X      ?     ?     °PV2    é6kÖÓ €             < d i r e c t x t k . p d b   h      ?     ?     QV2    u8kÖÓ               ( < D i r e c t X T K A u d i o D X . l i b     h      ?     ?     pQV2    u8kÖÓ              ( < D i r e c t X T K A u d i o D X . l i b     h      ?     ?     ØQV2    <kÖÓ              ( < D i r e c t X T K A u d i o D X . l i b     h      ?     ?     @RV2    `!kÖÓ              ( < D i r e c t X T K A u d i o D X . l i b     h      ?     ?     ¨RV2    Ë…!kÖÓ €            ( < D i r e c t X T K A u d i o D X . l i b     h      ?     ?     SV2    ’æ!kÖÓ               ( < d i r e c t x t k a u d i o d x . p d b     h      ?     ?     xSV2    ’æ!kÖÓ              ( < d i r e c t x t k a u d i o d x . p d b     h      ?     ?     àSV2    µ¹%kÖÓ              ( < d i r e c t x t k a u d i o d x . p d b     h      ?     ?     HTV2    f$2kÖÓ              ( < d i r e c t x t k a u d i o d x . p d b     h      ?     ?     °TV2    f$2kÖÓ €            ( < d i r e c t x t k a u d i o d x . p d b     P      ?     ?     UV2    wÁ2kÖÓ               < R e l e a s e       P      ?     ?     hUV2    wÁ2kÖÓ  €            < R e l e a s e       P      ?     ?     ¸UV2    ;ç2kÖÓ €              < R e l e a s e       P      ?     ?     VV2    ;ç2kÖÓ € €            < R e l e a s e       X      ?     ?     XVV2    |3kÖÓ                < D i r e c t X T K . l i b   X      ?     ?     °VV2    |3kÖÓ               < D i r e c t X T K . l i b   X      ?     ?     WV2    ôá7kÖÓ               < D i r e c t X T K . l i b   X      ?     ?     `WV2    JXkÖÓ               < D i r e c t X T K . l i b   X      ?     ?     ¸WV2    JXkÖÓ €             < D i r e c t X T K . l i b   X      	?     ?     XV2    ŸfXkÖÓ                < d i r e c t x t k . p d b   X      	?     ?     hXV2    ŸfXkÖÓ               < d i r e c t x t k . p d b   X      	?     ?     ÀXV2    ?ÿ\kÖÓ               < d i r e c t x t k . p d b   X      	?     ?     YV2    æˆfkÖÓ               < d i r e c t x t k . p d b   X      	?     ?     pYV2    æˆfkÖÓ €             < d i r e c t x t k . p d b   h      
?     ?     ÈYV2    æˆfkÖÓ               ( < D i r e c t X T K A u d i o D X . l i b     h      
?     ?     0ZV2    æˆfkÖÓ              ( < D i r e c t X T K A u d i o D X . l i b     h      
?     ?     ˜ZV2    äìhkÖÓ              ( < D i r e c t X T K A u d i o D X . l i b     h      
?     ?      [V2    óMkkÖÓ              ( < D i r e c t X T K A u d i o D X . l i b     h      
?     ?     h[V2    óMkkÖÓ €            ( < D i r e c t X T K A u d i o D X . l i b     h      ?     ?     Ğ[V2    óMkkÖÓ               ( < d i r e c t x t k a u d i o d x . p d b     h      ?     ?     8\V2    óMkkÖÓ              ( < d i r e c t x t k a u d i o d x . p d b     h      ?     ?      \V2    °mkÖÓ              ( < d i r e c t x t k a u d i o d x . p d b     h      ?     ?     ]V2    g×tkÖÓ              ( < d i r e c t x t k a u d i o d x . p d b     h      ?     ?     p]V2    g×tkÖÓ €            ( < d i r e c t x t k a u d i o d x . p d b     H      ?     Ô>     Ø]V2    g×tkÖÓ              
 < t o o l s   H      ?     Ô>      ^V2    g×tkÖÓ  €           
 < t o o l s   H      ?     Ô>     h^V2    g×tkÖÓ €             
 < t o o l s   H      ?     Ô>     °^V2    g×tkÖÓ € €           
 < t o o l s   H      ?     ?     ø^V2    g×tkÖÓ               < n a t i v e H      ?     ?     @_V2    g×tkÖÓ  €            < n a t i v e H      ?     ?     ˆ_V2    g×tkÖÓ €              < n a t i v e                                                 H      ?     ?      `V2    g×tkÖÓ € €            < n a t i v e `      ?     ?     H`V2    g×tkÖÓ               $ < M a k e S p r i t e F o n t . e x e `      ?     ?     ¨`V2    g×tkÖÓ              $ < M a k e S p r i t e F o n t . e x e `      ?     ?     aV2    Ú9wkÖÓ              $ < M a k e S p r i t e F o n t . e x e `      ?     ?     haV2    Ú9wkÖÓ              $ < M a k e S p r i t e F o n t . e x e `      ?     ?     ÈaV2    Ú9wkÖÓ €            $ < M a k e S p r i t e F o n t . e x e X      ?     ?     (bV2    Ú9wkÖÓ                < X W B T o o l . e x e     \ X      ?     ?     €bV2    Ú9wkÖÓ               < X W B T o o l . e x e     \ X      ?     ?     ØbV2    Ú9wkÖÓ               < X W B T o o l . e x e     \ X      ?     ?     0cV2    Ú9wkÖÓ               < X W B T o o l . e x e     \ X      ?     ?     ˆcV2    Ú9wkÖÓ €             < X W B T o o l . e x e     \ X                   àcV2    ¯>rÖÓ                < $ T x f L o g . b l f     p X                   8dV2    ¯>rÖÓ  €             < $ T x f L o g . b l f     p                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 STRING* obj, int idx) { idx--; while (idx >= 0 && !is_word_boundary_from_right(obj, idx)) idx--; return idx < 0 ? 0 : idx; }
#ifdef __APPLE__    // FIXME: Move setting to IO structure
	static int  is_word_boundary_from_left(STB_TEXTEDIT_STRING* obj, int idx) { return idx > 0 ? (!is_separator(obj->Text[idx - 1]) && is_separator(obj->Text[idx])) : 1; }
	static int  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING* obj, int idx) { idx++; int len = obj->CurLenW; while (idx < len && !is_word_boundary_from_left(obj, idx)) idx++; return idx > len ? len : idx; }
#else
	static int  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING* obj, int idx) { idx++; int len = obj->CurLenW; while (idx < len && !is_word_boundary_from_right(obj, idx)) idx++; return idx > len ? len : idx; }
#endif
#define STB_TEXTEDIT_MOVEWORDLEFT   STB_TEXTEDIT_MOVEWORDLEFT_IMPL    // They need to be #define for stb_textedit.h
#define STB_TEXTEDIT_MOVEWORDRIGHT  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL

	static void STB_TEXTEDIT_DELETECHARS(STB_TEXTEDIT_STRING* obj, int pos, int n)
	{
		ImWchar* dst = obj->Text.Data + pos;

		// We maintain our buffer length in both UTF-8 and wchar formats
		obj->CurLenA -= ImTextCountUtf8BytesFromStr(dst, dst + n);
		obj->CurLenW -= n;

		// Offset remaining text
		const ImWchar* src = obj->Text.Data + pos + n;
		while (ImWchar c = *src++)
			*dst++ = c;
		*dst = '\0';
	}

	static bool STB_TEXTEDIT_INSERTCHARS(STB_TEXTEDIT_STRING* obj, int pos, const ImWchar* new_text, int new_text_len)
	{
		const int text_len = obj->CurLenW;
		IM_ASSERT(pos <= text_len);
		if (new_text_len + text_len + 1 > obj->Text.Size)
			return false;

		const int new_text_len_utf8 = ImTextCountUtf8BytesFromStr(new_text, new_text + new_text_len);
		if (new_text_len_utf8 + obj->CurLenA + 1 > obj->BufSizeA)
			return false;

		ImWchar* text = obj->Text.Data;
		if (pos != text_len)
			memmove(text + pos + new_text_len, text + pos, (size_t)(text_len - pos) * sizeof(ImWchar));
		memcpy(text + pos, new_text, (size_t)new_text_len * sizeof(ImWchar));

		obj->CurLenW += new_text_len;
		obj->CurLenA += new_text_len_utf8;
		obj->Text[obj->CurLenW] = '\0';

		return true;
	}

	// We don't use an enum so we can build even with conflicting symbols (if another user of stb_textedit.h leak their STB_TEXTEDIT_K_* symbols)
#define STB_TEXTEDIT_K_LEFT         0x10000 // keyboard input to move cursor left
#define STB_TEXTEDIT_K_RIGHT        0x10001 // keyboard input to move cursor right
#define STB_TEXTEDIT_K_UP           0x10002 // keyboard input to move cursor up
#define STB_TEXTEDIT_K_DOWN         0x10003 // keyboard input to move cursor down
#define STB_TEXTEDIT_K_LINESTART    0x10004 // keyboard input to move cursor to start of line
#define STB_TEXTEDIT_K_LINEEND      0x10005 // keyboard input to move cursor to end of line
#define STB_TEXTEDIT_K_TEXTSTART    0x10006 // keyboard input to move cursor to start of text
#define STB_TEXTEDIT_K_TEXTEND      0x10007 // keyboard input to move cursor to end of text
#define STB_TEXTEDIT_K_DELETE       0x10008 // keyboard input to delete selection or character under cursor
#define STB_TEXTEDIT_K_BACKSPACE    0x10009 // keyboard input to delete selection or character left of cursor
#define STB_TEXTEDIT_K_UNDO         0x1000A // keyboard input to perform undo
#define STB_TEXTEDIT_K_REDO         0x1000B // keyboard input to perform redo
#define STB_TEXTEDIT_K_WORDLEFT     0x1000C // keyboard input to move cursor left one word
#define STB_TEXTEDIT_K_WORDRIGHT    0x1000D // keyboard input to move cursor right one word
#define STB_TEXTEDIT_K_SHIFT        0x20000

#define STB_TEXTEDIT_IMPLEMENTATION
#include "stb_textedit.h"

}

void ImGuiTextEditState::OnKeyPressed(int key)
{
	stb_textedit_key(this, &StbState, key);
	CursorFollow = true;
	CursorAnimReset();
}

// Public API to manipulate UTF-8 text
// We expose UTF-8 to the user (unlike the STB_TEXTEDIT_* functions which are manipulating wchar)
// FIXME: The existence of this rarely exercised code path is a bit of a nuisance.
void ImGuiTextEditCallbackData::DeleteChars(int pos, int bytes_count)
{
	IM_ASSERT(pos + bytes_count <= BufTextLen);
	char* dst = Buf + pos;
	const char* src = Buf + pos + bytes_count;
	while (char c = *src++)
		*dst++ = c;
	*dst = '\0';

	if (CursorPos + bytes_count >= pos)
		CursorPos -= bytes_count;
	else if (CursorPos >= pos)
		CursorPos = pos;
	SelectionStart = SelectionEnd = CursorPos;
	BufDirty = true;
	BufTextLen -= bytes_count;
}

void ImGuiTextEditCallbackData::InsertChars(int pos, const char* new_text, const char* new_text_end)
{
	const int new_text_len = new_text_end ? (int)(new_text_end - new_text) : (int)strlen(new_text);
	if (new_text_len + BufTextLen + 1 >= BufSize)
		return;

	if (BufTextLen != pos)
		memmove(Buf + pos + new_text_len, Buf + pos, (size_t)(BufTextLen - pos));
	memcpy(Buf + pos, new_text, (size_t)new_text_len * sizeof(char));
	Buf[BufTextLen + new_text_len] = '\0';

	if (CursorPos >= pos)
		CursorPos += new_text_len;
	SelectionStart = SelectionEnd = CursorPos;
	BufDirty = true;
	BufTextLen += new_text_len;
}

// Return false to discard a character.
static bool InputTextFilterCharacter(unsigned int* p_char, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
	unsigned int c = *p_char;

	if (c < 128 && c != ' ' && !isprint((int)(c & 0xFF)))
	{
		bool pass = false;
		pass |= (c == '\n' && (flags & ImGuiInputTextFlags_Multiline));
		pass |= (c == '\t' && (flags & ImGuiInputTextFlags_AllowTabInput));
		if (!pass)
			return false;
	}

	if (c >= 0xE000 && c <= 0xF8FF) // Filter private Unicode range. I don't imagine anybody would want to input them. GLFW on OSX seems to send private characters for special keys like arrow keys.
		return false;

	if (flags & (ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_CharsScientific))
	{
		if (flags & ImGuiInputTextFlags_CharsDecimal)
			if (!(c >= '0' && c <= '9') && (c != '.') && (c != '-') && (c != '+') && (c != '*') && (c != '/'))
				return false;

		if (flags & ImGuiInputTextFlags_CharsScientific)
			if (!(c >= '0' && c <= '9') && (c != '.') && (c != '-') && (c != '+') && (c != '*') && (c != '/') && (c != 'e') && (c != 'E'))
				return false;

		if (flags & ImGuiInputTextFlags_CharsHexadecimal)
			if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f') && !(c >= 'A' && c <= 'F'))
				return false;

		if (flags & ImGuiInputTextFlags_CharsUppercase)
			if (c >= 'a' && c <= 'z')
				*p_char = (c += (unsigned int)('A' - 'a'));

		if (flags & ImGuiInputTextFlags_CharsNoBlank)
			if (ImCharIsSpace(c))
				return false;
	}

	if (flags & ImGuiInputTextFlags_CallbackCharFilter)
	{
		ImGuiTextEditCallbackData callback_data;
		memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
		callback_data.EventFlag = ImGuiInputTextFlags_CallbackCharFilter;
		callback_data.EventChar = (ImWchar)c;
		callback_data.Flags = flags;
		callback_data.UserData = user_data;
		if (callback(&callback_data) != 0)
			return false;
		*p_char = callback_data.EventChar;
		if (!callback_data.EventChar)
			return false;
	}

	return true;
}

// Edit a string of text
// NB: when active, hold on a privately held copy of the text (and apply back to 'buf'). So changing 'buf' while active has no effect.
// FIXME: Rather messy function partly because we are doing UTF8 > u16 > UTF8 conversions on the go to more easily handle stb_textedit calls. Ideally we should stay in UTF-8 all the time. See https://github.com/nothings/stb/issues/188
bool ImGui::InputTextEx(const char* label, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackHistory) && (flags & ImGuiInputTextFlags_Multiline))); // Can't use both together (they both use up/down keys)
	IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackCompletion) && (flags & ImGuiInputTextFlags_AllowTabInput))); // Can't use both together (they both use tab key)

	ImGuiContext& g = *GImGui;
	const ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;

	const bool is_multiline = (flags & ImGuiInputTextFlags_Multiline) != 0;
	const bool is_editable = (flags & ImGuiInputTextFlags_ReadOnly) == 0;
	const bool is_password = (flags & ImGuiInputTextFlags_Password) != 0;
	const bool is_undoable = (flags & ImGuiInputTextFlags_NoUndoRedo) == 0;

	if (is_multiline) // Open group before calling GetID() because groups tracks id created during their spawn
		BeginGroup();
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), (is_multiline ? GetTextLineHeight() * 8.0f : label_size.y) + style.FramePadding.y*2.0f); // Arbitrary default of 8 lines high for multi-line
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? (style.ItemInnerSpacing.x + label_size.x) : 0.0f, 0.0f));

	ImGuiWindow* draw_window = window;
	if (is_multiline)
	{
		ItemAdd(total_bb, id, &frame_bb);
		if (!BeginChildFrame(id, frame_bb.GetSize()))
		{
			EndChildFrame();
			EndGroup();
			return false;
		}
		draw_window = GetCurrentWindow();
		size.x -= draw_window->ScrollbarSizes.x;
	}
	else
	{
		ItemSize(total_bb, style.FramePadding.y);
		if (!ItemAdd(total_bb, id, &frame_bb))
			return false;
	}
	const bool hovered = ItemHoverable(frame_bb, id);
	if (hovered)
		g.MouseCursor = ImGuiMouseCursor_TextInput;

	// Password pushes a temporary font with only a fallback glyph
	if (is_password)
	{
		const ImFontGlyph* glyph = g.Font->FindGlyph('*');
		ImFont* password_font = &g.InputTextPasswordFont;
		password_font->FontSize = g.Font->FontSize;
		password_font->Scale = g.Font->Scale;
		password_font->DisplayOffset = g.Font->DisplayOffset;
		password_font->Ascent = g.Font->Ascent;
		password_font->Descent = g.Font->Descent;
		password_font->ContainerAtlas = g.Font->ContainerAtlas;
		password_font->FallbackGlyph = glyph;
		password_font->FallbackAdvanceX = glyph->AdvanceX;
		IM_ASSERT(password_font->Glyphs.empty() && password_font->IndexAdvanceX.empty() && password_font->IndexLookup.empty());
		PushFont(password_font);
	}

	// NB: we are only allowed to access 'edit_state' if we are the active widget.
	ImGuiTextEditState& edit_state = g.InputTextState;

	const bool focus_requested = FocusableItemRegister(window, id, (flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_AllowTabInput)) == 0);    // Using completion callback disable keyboard tabbing
	const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
	const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

	const bool user_clicked = hovered && io.MouseClicked[0];
	const bool user_scrolled = is_multiline && g.ActiveId == 0 && edit_state.Id == id && g.ActiveIdPreviousFrame == draw_window->GetIDNoKeepAlive("#SCROLLY");
	const bool user_nav_input_start = (g.ActiveId != id) && ((g.NavInputId == id) || (g.NavActivateId == id && g.NavInputSource == ImGuiInputSource_NavKeyboard));

	bool clear_active_id = false;

	bool select_all = (g.ActiveId != id) && ((flags & ImGuiInputTextFlags_AutoSelectAll) != 0 || user_nav_input_start) && (!is_multiline);
	if (focus_requested || user_clicked || user_scrolled || user_nav_input_start)
	{
		if (g.ActiveId != id)
		{
			// Start edition
			// Take a copy of the initial buffer value (both in original UTF-8 format and converted to wchar)
			// From the moment we focused we are ignoring the content of 'buf' (unless we are in read-only mode)
			const int prev_len_w = edit_state.CurLenW;
			edit_state.Text.resize(buf_size + 1);        // wchar count <= UTF-8 count. we use +1 to make sure that .Data isn't NULL so it doesn't crash.
			edit_state.InitialText.resize(buf_size + 1); // UTF-8. we use +1 to make sure that .Data isn't NULL so it doesn't crash.
			ImStrncpy(edit_state.InitialText.Data, buf, edit_state.InitialText.Size);
			const char* buf_end = NULL;
			edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
			edit_state.CurLenA = (int)(buf_end - buf); // We can't get the result from ImFormatString() above because it is not UTF-8 aware. Here we'll cut off malformed UTF-8.
			edit_state.CursorAnimReset();

			// Preserve cursor position and undo/redo stack if we come back to same widget
			// FIXME: We should probably compare the whole buffer to be on the safety side. Comparing buf (utf8) and edit_state.Text (wchar).
			const bool recycle_state = (edit_state.Id == id) && (prev_len_w == edit_state.CurLenW);
			if (recycle_state)
			{
				// Recycle existing cursor/selection/undo stack but clamp position
				// Note a single mouse click will override the cursor/position immediately by calling stb_textedit_click handler.
				edit_state.CursorClamp();
			}
			else
			{
				edit_state.Id = id;
				edit_state.ScrollX = 0.0f;
				stb_textedit_initialize_state(&edit_state.StbState, !is_multiline);
				if (!is_multiline && focus_requested_by_code)
					select_all = true;
			}
			if (flags & ImGuiInputTextFlags_AlwaysInsertMode)
				edit_state.StbState.insert_mode = true;
			if (!is_multiline && (focus_requested_by_tab || (user_clicked && io.KeyCtrl)))
				select_all = true;
		}
		SetActiveID(id, window);
		SetFocusID(id, window);
		FocusWindow(window);
		if (!is_multiline && !(flags & ImGuiInputTextFlags_CallbackHistory))
			g.ActiveIdAllowNavDirFlags |= ((1 << ImGuiDir_Up) | (1 << ImGuiDir_Down));
	}
	else if (io.MouseClicked[0])
	{
		// Release focus when we click outside
		clear_active_id = true;
	}

	bool value_changed = false;
	bool enter_pressed = false;

	if (g.ActiveId == id)
	{
		if (!is_editable && !g.ActiveIdIsJustActivated)
		{
			// When read-only we always use the live data passed to the function
			edit_state.Text.resize(buf_size + 1);
			const char* buf_end = NULL;
			edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
			edit_state.CurLenA = (int)(buf_end - buf);
			edit_state.CursorClamp();
		}

		edit_state.BufSizeA = buf_size;

		// Although we are active we don't prevent mouse from hovering other elements unless we are interacting right now with the widget.
		// Down the line we should have a cleaner library-wide concept of Selected vs Active.
		g.ActiveIdAllowOverlap = !io.MouseDown[0];
		g.WantTextInputNextFrame = 1;

		// Edit in progress
		const float mouse_x = (io.MousePos.x - frame_bb.Min.x - style.FramePadding.x) + edit_state.ScrollX;
		const float mouse_y = (is_multiline ? (io.MousePos.y - draw_window->DC.CursorPos.y - style.FramePadding.y) : (g.FontSize*0.5f));

		const bool osx_double_click_selects_words = io.OptMacOSXBehaviors;      // OS X style: Double click selects by word instead of selecting whole text
		if (select_all || (hovered && !osx_double_click_selects_words && io.MouseDoubleClicked[0]))
		{
			edit_state.SelectAll();
			edit_state.SelectedAllMouseLock = true;
		}
		else if (hovered && osx_double_click_selects_words && io.MouseDoubleClicked[0])
		{
			// Select a word only, OS X style (by simulating keystrokes)
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
		}
		else if (io.MouseClicked[0] && !edit_state.SelectedAllMouseLock)
		{
			if (hovered)
			{
				stb_textedit_click(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
				edit_state.CursorAnimReset();
			}
		}
		else if (io.MouseDown[0] && !edit_state.SelectedAllMouseLock && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
		{
			stb_textedit_drag(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
			edit_state.CursorAnimReset();
			edit_state.CursorFollow = true;
		}
		if (edit_state.SelectedAllMouseLock && !io.MouseDown[0])
			edit_state.SelectedAllMouseLock = false;

		if (io.InputCharacters[0])
		{
			// Process text input (before we check for Return because using some IME will effectively send a Return?)
			// We ignore CTRL inputs, but need to allow ALT+CTRL as some keyboards (e.g. German) use AltGR (which _is_ Alt+Ctrl) to input certain characters.
			if (!(io.KeyCtrl && !io.KeyAlt) && is_editable && !user_nav_input_start)
				for (int n = 0; n < IM_ARRAYSIZE(io.InputCharacters) && io.InputCharacters[n]; n++)
				{
					// Insert character if they pass filtering
					unsigned int c = (unsigned int)io.InputCharacters[n];
					if (InputTextFilterCharacter(&c, flags, callback, user_data))
						edit_state.OnKeyPressed((int)c);
				}

			// Consume characters
			memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));
		}
	}

	bool cancel_edit = false;
	if (g.ActiveId == id && !g.ActiveIdIsJustActivated && !clear_active_id)
	{
		// Handle key-presses
		const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
		const bool is_shortcut_key_only = (io.OptMacOSXBehaviors ? (io.KeySuper && !io.KeyCtrl) : (io.KeyCtrl && !io.KeySuper)) && !io.KeyAlt && !io.KeyShift; // OS X style: Shortcuts using Cmd/Super instead of Ctrl
		const bool is_wordmove_key_down = io.OptMacOSXBehaviors ? io.KeyAlt : io.KeyCtrl;                     // OS X style: Text editing cursor movement using Alt instead of Ctrl
		const bool is_startend_key_down = io.OptMacOSXBehaviors && io.KeySuper && !io.KeyCtrl && !io.KeyAlt;  // OS X style: Line/Text Start and End using Cmd+Arrows instead of Home/End
		const bool is_ctrl_key_only = io.KeyCtrl && !io.KeyShift && !io.KeyAlt && !io.KeySuper;
		const bool is_shift_key_only = io.KeyShift && !io.KeyCtrl && !io.KeyAlt && !io.KeySuper;

		const bool is_cut = ((is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_X)) || (is_shift_key_only && IsKeyPressedMap(ImGuiKey_Delete))) && is_editable && !is_password && (!is_multiline || edit_state.HasSelection());
		const bool is_copy = ((is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_C)) || (is_ctrl_key_only  && IsKeyPressedMap(ImGuiKey_Insert))) && !is_password && (!is_multiline || edit_state.HasSelection());
		const bool is_paste = ((is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_V)) || (is_shift_key_only && IsKeyPressedMap(ImGuiKey_Insert))) && is_editable;

		if (IsKeyPressedMap(ImGuiKey_LeftArrow)) { edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINESTART : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDLEFT : STB_TEXTEDIT_K_LEFT) | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_RightArrow)) { edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINEEND : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDRIGHT : STB_TEXTEDIT_K_RIGHT) | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_UpArrow) && is_multiline) { if (io.KeyCtrl) SetWindowScrollY(draw_window, ImMax(draw_window->Scroll.y - g.FontSize, 0.0f)); else edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTSTART : STB_TEXTEDIT_K_UP) | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_DownArrow) && is_multiline) { if (io.KeyCtrl) SetWindowScrollY(draw_window, ImMin(draw_window->Scroll.y + g.FontSize, GetScrollMaxY())); else edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTEND : STB_TEXTEDIT_K_DOWN) | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_Home)) { edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTSTART | k_mask : STB_TEXTEDIT_K_LINESTART | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_End)) { edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTEND | k_mask : STB_TEXTEDIT_K_LINEEND | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_Delete) && is_editable) { edit_state.OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_Backspace) && is_editable)
		{
			if (!edit_state.HasSelection())
			{
				if (is_wordmove_key_down) edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT | STB_TEXTEDIT_K_SHIFT);
				else if (io.OptMacOSXBehaviors && io.KeySuper && !io.KeyAlt && !io.KeyCtrl) edit_state.OnKeyPressed(STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_SHIFT);
			}
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_Enter))
		{
			bool ctrl_enter_for_new_line = (flags & ImGuiInputTextFlags_CtrlEnterForNewLine) != 0;
			if (!is_multiline || (ctrl_enter_for_new_line && !io.KeyCtrl) || (!ctrl_enter_for_new_line && io.KeyCtrl))
			{
				enter_pressed = clear_active_id = true;
			}
			else if (is_editable)
			{
				unsigned int c = '\n'; // Insert new line
				if (InputTextFilterCharacter(&c, flags, callback, user_data))
					edit_state.OnKeyPressed((int)c);
			}
		}
		else if ((flags & ImGuiInputTextFlags_AllowTabInput) && IsKeyPressedMap(ImGuiKey_Tab) && !io.KeyCtrl && !io.KeyShift && !io.KeyAlt && is_editable)
		{
			unsigned int c = '\t'; // Insert TAB
			if (InputTextFilterCharacter(&c, flags, callback, user_data))
				edit_state.OnKeyPressed((int)c);
		}
		else if (IsKeyPressedMap(ImGuiKey_Escape)) { clear_active_id = cancel_edit = true; }
		else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_Z) && is_editable && is_undoable) { edit_state.OnKeyPressed(STB_TEXTEDIT_K_UNDO); edit_state.ClearSelection(); }
		else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_Y) && is_editable && is_undoable) { edit_state.OnKeyPressed(STB_TEXTEDIT_K_REDO); edit_state.ClearSelection(); }
		else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_A)) { edit_state.SelectAll(); edit_state.CursorFollow = true; }
		else if (is_cut || is_copy)
		{
			// Cut, Copy
			if (io.SetClipboardTextFn)
			{
				const int ib = edit_state.HasSelection() ? ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end) : 0;
				const int ie = edit_state.HasSelection() ? ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end) : edit_state.CurLenW;
				edit_state.TempTextBuffer.resize((ie - ib) * 4 + 1);
				ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data + ib, edit_state.Text.Data + ie);
				SetClipboardText(edit_state.TempTextBuffer.Data);
			}

			if (is_cut)
			{
				if (!edit_state.HasSelection())
					edit_state.SelectAll();
				edit_state.CursorFollow = true;
				stb_textedit_cut(&edit_state, &edit_state.StbState);
			}
		}
		else if (is_paste)
		{
			// Paste
			if (const char* clipboard = GetClipboardText())
			{
				// Filter pasted buffer
				const int clipboard_len = (int)strlen(clipboard);
				ImWchar* clipboard_filtered = (ImWchar*)ImGui::MemAlloc((clipboard_len + 1) * sizeof(ImWchar));
				int clipboard_filtered_len = 0;
				for (const char* s = clipboard; *s; )
				{
					unsigned int c;
					s += ImTextCharFromUtf8(&c, s, NULL);
					if (c == 0)
						break;
					if (c >= 0x10000 || !InputTextFilterCharacter(&c, flags, callback, user_data))
						continue;
					clipboard_filtered[clipboard_filtered_len++] = (ImWchar)c;
				}
				clipboard_filtered[clipboard_filtered_len] = 0;
				if (clipboard_filtered_len > 0) // If everything was filtered, ignore the pasting operation
				{
					stb_textedit_paste(&edit_state, &edit_state.StbState, clipboard_filtered, clipboard_filtered_len);
					edit_state.CursorFollow = true;
				}
				ImGui::MemFree(clipboard_filtered);
			}
		}
	}

	if (g.ActiveId == id)
	{
		if (cancel_edit)
		{
			// Restore initial value
			if (is_editable)
			{
				ImStrncpy(buf, edit_state.InitialText.Data, buf_size);
				value_changed = true;
			}
		}

		// When using 'ImGuiInputTextFlags_EnterReturnsTrue' as a special case we reapply the live buffer back to the input buffer before clearing ActiveId, even though strictly speaking it wasn't modified on this frame.
		// If we didn't do that, code like InputInt() with ImGuiInputTextFlags_EnterReturnsTrue would fail. Also this allows the user to use InputText() with ImGuiInputTextFlags_EnterReturnsTrue without maintaining any user-side storage.
		bool apply_edit_back_to_user_buffer = !cancel_edit || (enter_pressed && (flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0);
		if (apply_edit_back_to_user_buffer)
		{
			// Apply new value immediately - copy modified buffer back
			// Note that as soon as the input box is active, the in-widget value gets priority over any underlying modification of the input buffer
			// FIXME: We actually always render 'buf' when calling DrawList->AddText, making the comment above incorrect.
			// FIXME-OPT: CPU waste to do this every time the widget is active, should mark dirty state from the stb_textedit callbacks.
			if (is_editable)
			{
				edit_state.TempTextBuffer.resize(edit_state.Text.Size * 4);
				ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data, NULL);
			}

			// User callback
			if ((flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackAlways)) != 0)
			{
				IM_ASSERT(callback != NULL);

				// The reason we specify the usage semantic (Completion/History) is that Completion needs to disable keyboard TABBING at the moment.
				ImGuiInputTextFlags event_flag = 0;
				ImGuiKey event_key = ImGuiKey_COUNT;
				if ((flags & ImGuiInputTextFlags_CallbackCompletion) != 0 && IsKeyPressedMap(ImGuiKey_Tab))
				{
					event_flag = ImGuiInputTextFlags_CallbackCompletion;
					event_key = ImGuiKey_Tab;
				}
				else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_UpArrow))
				{
					event_flag = ImGuiInputTextFlags_CallbackHistory;
					event_key = ImGuiKey_UpArrow;
				}
				else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_DownArrow))
				{
					event_flag = ImGuiInputTextFlags_CallbackHistory;
					event_key = ImGuiKey_DownArrow;
				}
				else if (flags & ImGuiInputTextFlags_CallbackAlways)
					event_flag = ImGuiInputTextFlags_CallbackAlways;

				if (event_flag)
				{
					ImGuiTextEditCallbackData callback_data;
					memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
					callback_data.EventFlag = event_flag;
					callback_data.Flags = flags;
					callback_data.UserData = user_data;
					callback_data.ReadOnly = !is_editable;

					callback_data.EventKey = event_key;
					callback_data.Buf = edit_state.TempTextBuffer.Data;
					callback_data.BufTextLen = edit_state.CurLenA;
					callback_data.BufSize = edit_state.BufSizeA;
					callback_data.BufDirty = false;

					// We have to convert from wchar-positions to UTF-8-positions, which can be pretty slow (an incentive to ditch the ImWchar buffer, see https://github.com/nothings/stb/issues/188)
					ImWchar* text = edit_state.Text.Data;
					const int utf8_cursor_pos = callback_data.CursorPos = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.cursor);
					const int utf8_selection_start = callback_data.SelectionStart = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_start);
					const int utf8_selection_end = callback_data.SelectionEnd = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_end);

					// Call user code
					callback(&callback_data);

					// Read back what user may have modified
					IM_ASSERT(callback_data.Buf == edit_state.TempTextBuffer.Data);  // Invalid to modify those fields
					IM_ASSERT(callback_data.BufSize == edit_state.BufSizeA);
					IM_ASSERT(callback_data.Flags == flags);
					if (callback_data.CursorPos != utf8_cursor_pos)            edit_state.StbState.cursor = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.CursorPos);
					if (callback_data.SelectionStart != utf8_selection_start)  edit_state.StbState.select_start = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionStart);
					if (callback_data.SelectionEnd != utf8_selection_end)      edit_state.StbState.select_end = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionEnd);
					if (callback_data.BufDirty)
					{
						IM_ASSERT(callback_data.BufTextLen == (int)strlen(callback_data.Buf)); // You need to maintain BufTextLen if you change the text!
						edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, callback_data.Buf, NULL);
						edit_state.CurLenA = callback_data.BufTextLen;  // Assume correct length and valid UTF-8 from user, saves us an extra strlen()
						edit_state.CursorAnimReset();
					}
				}
			}

			// Copy back to user buffer
			if (is_editable && strcmp(edit_state.TempTextBuffer.Data, buf) != 0)
			{
				ImStrncpy(buf, edit_state.TempTextBuffer.Data, buf_size);
				value_changed = true;
			}
		}
	}

	// Release active ID at the end of the function (so e.g. pressing Return still does a final application of the value)
	if (clear_active_id && g.ActiveId == id)
		ClearActiveID();

	// Render
	// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is set 'buf' might still be the old value. We set buf to NULL to prevent accidental usage from now on.
	const char* buf_display = (g.ActiveId == id && is_editable) ? edit_state.TempTextBuffer.Data : buf; buf = NULL;

	RenderNavHighlight(frame_bb, id);
	if (!is_multiline)
		RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

	const ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
	ImVec2 render_pos = is_multiline ? draw_window->DC.CursorPos : frame_bb.Min + style.FramePadding;
	ImVec2 text_size(0.f, 0.f);
	const bool is_currently_scrolling = (edit_state.Id == id && is_multiline && g.ActiveId == draw_window->GetIDNoKeepAlive("#SCROLLY"));
	if (g.ActiveId == id || is_currently_scrolling)
	{
		edit_state.CursorAnim += io.DeltaTime;

		// This is going to be messy. We need to:
		// - Display the text (this alone can be more easily clipped)
		// - Handle scrolling, highlight selection, display cursor (those all requires some form of 1d->2d cursor position calculation)
		// - Measure text height (for scrollbar)
		// We are attempting to do most of that in **one main pass** to minimize the computation cost (non-negligible for large amount of text) + 2nd pass for selection rendering (we could merge them by an extra refactoring effort)
		// FIXME: This should occur on buf_display but we'd need to maintain cursor/select_start/select_end for UTF-8.
		const ImWchar* text_begin = edit_state.Text.Data;
		ImVec2 cursor_offset, select_start_offset;

		{
			// Count lines + find lines numbers straddling 'cursor' and 'select_start' position.
			const ImWchar* searches_input_ptr[2];
			searches_input_ptr[0] = text_begin + edit_state.StbState.cursor;
			searches_input_ptr[1] = NULL;
			int searches_remaining = 1;
			int searches_result_line_number[2] = { -1, -999 };
			if (edit_state.StbState.select_start != edit_state.StbState.select_end)
			{
				searches_input_ptr[1] = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
				searches_result_line_number[1] = -1;
				searches_remaining++;
			}

			// Iterate all lines to find our line numbers
			// In multi-line mode, we never exit the loop until all lines are counted, so add one extra to the searches_remaining counter.
			searches_remaining += is_multiline ? 1 : 0;
			int line_count = 0;
			for (const ImWchar* s = text_begin; *s != 0; s++)
				if (*s == '\n')
				{
					line_count++;
					if (searches_result_line_number[0] == -1 && s >= searches_input_ptr[0]) { searches_result_line_number[0] = line_count; if (--searches_remaining <= 0) break; }
					if (searches_result_line_number[1] == -1 && s >= searches_input_ptr[1]) { searches_result_line_number[1] = line_count; if (--searches_remaining <= 0) break; }
				}
			line_count++;
			if (searches_result_line_number[0] == -1) searches_result_line_number[0] = line_count;
			if (searches_result_line_number[1] == -1) searches_result_line_number[1] = line_count;

			// Calculate 2d position by finding the beginning of the line and measuring distance
			cursor_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[0], text_begin), searches_input_ptr[0]).x;
			cursor_offset.y = searches_result_line_number[0] * g.FontSize;
			if (searches_result_line_number[1] >= 0)
			{
				select_start_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[1], text_begin), searches_input_ptr[1]).x;
				select_start_offset.y = searches_result_line_number[1] * g.FontSize;
			}

			// Store text height (note that we haven't calculated text width at all, see GitHub issues #383, #1224)
			if (is_multiline)
				text_size = ImVec2(size.x, line_count * g.FontSize);
		}

		// Scroll
		if (edit_state.CursorFollow)
		{
			// Horizontal scroll in chunks of quarter width
			if (!(flags & ImGuiInputTextFlags_NoHorizontalScroll))
			{
				const float scroll_increment_x = size.x * 0.25f;
				if (cursor_offset.x < edit_state.ScrollX)
					edit_state.ScrollX = (float)(int)ImMax(0.0f, cursor_offset.x - scroll_increment_x);
				else if (cursor_offset.x - size.x >= edit_state.ScrollX)
					edit_state.ScrollX = (float)(int)(cursor_offset.x - size.x + scroll_increment_x);
			}
			else
			{
				edit_state.ScrollX = 0.0f;
			}

			// Vertical scroll
			if (is_multiline)
			{
				float scroll_y = draw_window->Scroll.y;
				if (cursor_offset.y - g.FontSize < scroll_y)
					scroll_y = ImMax(0.0f, cursor_offset.y - g.FontSize);
				else if (cursor_offset.y - size.y >= scroll_y)
					scroll_y = cursor_offset.y - size.y;
				draw_window->DC.CursorPos.y += (draw_window->Scroll.y - scroll_y);   // To avoid a frame of lag
				draw_window->Scroll.y = scroll_y;
				render_pos.y = draw_window->DC.CursorPos.y;
			}
		}
		edit_state.CursorFollow = false;
		const ImVec2 render_scroll = ImVec2(edit_state.ScrollX, 0.0f);

		// Draw selection
		if (edit_state.StbState.select_start != edit_state.StbState.select_end)
		{
			const ImWchar* text_selected_begin = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
			const ImWchar* text_selected_end = text_begin + ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end);

			float bg_offy_up = is_multiline ? 0.0f : -1.0f;    // FIXME: those offsets should be part of the style? they don't play so well with multi-line selection.
			float bg_offy_dn = is_multiline ? 0.0f : 2.0f;
			ImU32 bg_color = GetColorU32(ImGuiCol_TextSelectedBg);
			ImVec2 rect_pos = render_pos + select_start_offset - render_scroll;
			for (const ImWchar* p = text_selected_begin; p < text_selected_end; )
			{
				if (rect_pos.y > clip_rect.w + g.FontSize)
					break;
				if (rect_pos.y < clip_rect.y)
				{
					while (p < text_selected_end)
						if (*p++ == '\n')
							break;
				}
				else
				{
					ImVec2 rect_size = InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
					if (rect_size.x <= 0.0f) rect_size.x = (float)(int)(g.Font->GetCharAdvance((unsigned short)' ') * 0.50f); // So we can see selected empty lines
					ImRect rect(rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize), rect_pos + ImVec2(rect_size.x, bg_offy_dn));
					rect.ClipWith(clip_rect);
					if (rect.Overlaps(clip_rect))
						draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
				}
				rect_pos.x = render_pos.x - render_scroll.x;
				rect_pos.y += g.FontSize;
			}
		}

		draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos - render_scroll, GetColorU32(ImGuiCol_Text), buf_display, buf_display + edit_state.CurLenA, 0.0f, is_multiline ? NULL : &clip_rect);

		// Draw blinking cursor
		bool cursor_is_visible = (!g.IO.OptCursorBlink) || (g.InputTextState.CursorAnim <= 0.0f) || fmodf(g.InputTextState.CursorAnim, 1.20f) <= 0.80f;
		ImVec2 cursor_screen_pos = render_pos + cursor_offset - render_scroll;
		ImRect cursor_screen_rect(cursor_screen_pos.x, cursor_screen_pos.y - g.FontSize + 0.5f, cursor_screen_pos.x + 1.0f, cursor_screen_pos.y - 1.5f);
		if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
			draw_window->DrawList->AddLine(cursor_screen_rect.Min, cursor_screen_rect.GetBL(), GetColorU32(ImGuiCol_Text));

		// Notify OS of text input position for advanced IME (-1 x offset so that Windows IME can cover our cursor. Bit of an extra nicety.)
		if (is_editable)
			g.OsImePosRequest = ImVec2(cursor_screen_pos.x - 1, cursor_screen_pos.y - g.FontSize);
	}
	else
	{
		// Render text only
		const char* buf_end = NULL;
		if (is_multiline)
			text_size = ImVec2(size.x, InputTextCalcTextLenAndLineCount(buf_display, &buf_end) * g.FontSize); // We don't need width
		draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display, buf_end, 0.0f, is_multiline ? NULL : &clip_rect);
	}

	if (is_multiline)
	{
		Dummy(text_size + ImVec2(0.0f, g.FontSize)); // Always add room to scroll an extra line
		EndChildFrame();
		EndGroup();
	}

	if (is_password)
		PopFont();

	// Log as text
	if (g.LogEnabled && !is_password)
		LogRenderedText(&render_pos, buf_display, NULL);

	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	if ((flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0)
		return enter_pressed;
	else
		return value_changed;
}

bool ImGui::InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
	IM_ASSERT(!(flags & ImGuiInputTextFlags_Multiline)); // call InputTextMultiline()
	return InputTextEx(label, buf, (int)buf_size, ImVec2(0, 0), flags, callback, user_data);
}

bool ImGui::InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
	return InputTextEx(label, buf, (int)buf_size, size, flags | ImGuiInputTextFlags_Multiline, callback, user_data);
}

// NB: scalar_format here must be a simple "%xx" format string with no prefix/suffix (unlike the Drag/Slider functions "display_format" argument)
bool ImGui::InputScalarEx(const char* label, ImGuiDataType data_type, void* data_ptr, void* step_ptr, void* step_fast_ptr, const char* scalar_format, ImGuiInputTextFlags extra_flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	BeginGroup();
	PushID(label);
	const ImVec2 button_sz = ImVec2(GetFrameHeight(), GetFrameHeight());
	if (step_ptr)
		PushItemWidth(ImMax(1.0f, CalcItemWidth() - (button_sz.x + style.ItemInnerSpacing.x) * 2));

	char buf[64];
	DataTypeFormatString(data_type, data_ptr, scalar_format, buf, IM_ARRAYSIZE(buf));

	bool value_changed = false;
	if ((extra_flags & (ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsScientific)) == 0)
		extra_flags |= ImGuiInputTextFlags_CharsDecimal;
	extra_flags |= ImGuiInputTextFlags_AutoSelectAll;
	if (InputText("", buf, IM_ARRAYSIZE(buf), extra_flags)) // PushId(label) + "" gives us the expected ID from outside point of view
		value_changed = DataTypeApplyOpFromText(buf, GImGui->InputTextState.InitialText.begin(), data_type, data_ptr, scalar_format);

	// Step buttons
	if (step_ptr)
	{
		PopItemWidth();
		SameLine(0, style.ItemInnerSpacing.x);
		if (ButtonEx("-", button_sz, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups))
		{
			DataTypeApplyOp(data_type, '-', data_ptr, data_ptr, g.IO.KeyCtrl && step_fast_ptr ? step_fast_ptr : step_ptr);
			value_changed = true;
		}
		SameLine(0, style.ItemInnerSpacing.x);
		if (ButtonEx("+", button_sz, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups))
		{
			DataTypeApplyOp(data_type, '+', data_ptr, data_ptr, g.IO.KeyCtrl && step_fast_ptr ? step_fast_ptr : step_ptr);
			value_changed = true;
		}
	}
	PopID();

	if (label_size.x > 0)
	{
		SameLine(0, style.ItemInnerSpacing.x);
		RenderText(ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y + style.FramePadding.y), label);
		ItemSize(label_size, style.FramePadding.y);
	}
	EndGroup();

	return value_changed;
}

bool ImGui::InputFloat(const char* label, float* v, float step, float step_fast, int decimal_precision, ImGuiInputTextFlags extra_flags)
{
	extra_flags |= ImGuiInputTextFlags_CharsScientific;
	if (decimal_precision < 0)
	{
		// Ideally we'd have a minimum decimal precision of 1 to visually denote that this is a float, while hiding non-significant digits? %f doesn't have a minimum of 1
		return InputScalarEx(label, ImGuiDataType_Float, (void*)v, (void*)(step > 0.0f ? &step : NULL), (void*)(step_fast > 0.0f ? &step_fast : NULL), "%f", extra_flags);
	}
	else
	{
		char display_format[16];
		ImFormatString(display_format, IM_ARRAYSIZE(display_format), "%%.%df", decimal_precision);
		return InputScalarEx(label, ImGuiDataType_Float, (void*)v, (void*)(step > 0.0f ? &step : NULL), (void*)(step_fast > 0.0f ? &step_fast : NULL), display_format, extra_flags);
	}
}

bool ImGui::InputDouble(const char* label, double* v, double step, double step_fast, const char* display_format, ImGuiInputTextFlags extra_flags)
{
	extra_flags |= ImGuiInputTextFlags_CharsScientific;
	return InputScalarEx(label, ImGuiDataType_Double, (void*)v, (void*)(step > 0.0 ? &step : NULL), (void*)(step_fast > 0.0 ? &step_fast : NULL), display_format, extra_flags);
}

bool ImGui::InputInt(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags extra_flags)
{
	// Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
	const char* scalar_format = (extra_flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
	return InputScalarEx(label, ImGuiDataType_Int, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), scalar_format, extra_flags);
}

bool ImGui::InputFloatN(const char* label, float* v, int components, int decimal_precision, ImGuiInputTextFlags extra_flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	bool value_changed = false;
	BeginGroup();
	PushID(label);
	PushMultiItemsWidths(components);
	for (int i = 0; i < components; i++)
	{
		PushID(i);
		value_changed |= InputFloat("##v", &v[i], 0, 0, decimal_precision, extra_flags);
		SameLine(0, g.Style.ItemInnerSpacing.x);
		PopID();
		PopItemWidth();
	}
	PopID();

	TextUnformatted(label, FindRenderedTextEnd(label));
	EndGroup();

	return value_changed;
}

bool ImGui::InputFloat2(const char* label, float v[2], int decimal_precision, ImGuiInputTextFlags extra_flags)
{
	return InputFloatN(label, v, 2, decimal_precision, extra_flags);
}

bool ImGui::InputFloat3(const char* label, float v[3], int decimal_precision, ImGuiInputTextFlags extra_flags)
{
	return InputFloatN(label, v, 3, decimal_precision, extra_flags);
}

bool ImGui::InputFloat4(const char* label, float v[4], int decimal_precision, ImGuiInputTextFlags extra_flags)
{
	return InputFloatN(label, v, 4, decimal_precision, extra_flags);
}

bool ImGui::InputIntN(const char* label, int* v, int components, ImGuiInputTextFlags extra_flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	bool value_changed = false;
	BeginGroup();
	PushID(label);
	PushMultiItemsWidths(components);
	for (int i = 0; i < components; i++)
	{
		PushID(i);
		value_changed |= InputInt("##v", &v[i], 0, 0, extra_flags);
		SameLine(0, g.Style.ItemInnerSpacing.x);
		PopID();
		PopItemWidth();
	}
	PopID();

	TextUnformatted(label, FindRenderedTextEnd(label));
	EndGroup();

	return value_changed;
}

bool ImGui::InputInt2(const char* label, int v[2], ImGuiInputTextFlags extra_flags)
{
	return InputIntN(label, v, 2, extra_flags);
}

bool ImGui::InputInt3(const char* label, int v[3], ImGuiInputTextFlags extra_flags)
{
	return InputIntN(label, v, 3, extra_flags);
}

bool ImGui::InputInt4(const char* label, int v[4], ImGuiInputTextFlags extra_flags)
{
	return InputIntN(label, v, 4, extra_flags);
}

static float CalcMaxPopupHeightFromItemCount(int items_count)
{
	ImGuiContext& g = *GImGui;
	if (items_count <= 0)
		return FLT_MAX;
	return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

bool ImGui::BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
{
	// Always consume the SetNextWindowSizeConstraint() call in our early return paths
	ImGuiContext& g = *GImGui;
	ImGuiCond backup_next_window_size_constraint = g.NextWindowData.SizeConstraintCond;
	g.NextWindowData.SizeConstraintCond = 0;

	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together

	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : GetFrameHeight();
	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	const float w = (flags & ImGuiComboFlags_NoPreview) ? arrow_size : CalcItemWidth();
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y*2.0f));
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, id, &frame_bb))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(frame_bb, id, &hovered, &held);
	bool popup_open = IsPopupOpen(id);

	const ImRect value_bb(frame_bb.Min, frame_bb.Max - ImVec2(arrow_size, 0.0f));
	const ImU32 frame_col = GetColorU32(hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
	RenderNavHighlight(frame_bb, id);
	if (!(flags & ImGuiComboFlags_NoPreview))
		window->DrawList->AddRectFilled(frame_bb.Min, ImVec2(frame_bb.Max.x - arrow_size, frame_bb.Max.y), frame_col, style.FrameRounding, ImDrawCornerFlags_Left);
	if (!(flags & ImGuiComboFlags_NoArrowButton))
	{
		window->DrawList->AddRectFilled(ImVec2(frame_bb.Max.x - arrow_size, frame_bb.Min.y), frame_bb.Max, GetColorU32((popup_open || hovered) ? ImGuiCol_ButtonHovered : ImGuiCol_Button), style.FrameRounding, (w <= arrow_size) ? ImDrawCornerFlags_All : ImDrawCornerFlags_Right);
		RenderArrow(ImVec2(frame_bb.Max.x - arrow_size + style.FramePadding.y, frame_bb.Min.y + style.FramePadding.y), ImGuiDir_Down);
	}
	RenderFrameBorder(frame_bb.Min, frame_bb.Max, style.FrameRounding);
	if (preview_value != NULL && !(flags & ImGuiComboFlags_NoPreview))
		RenderTextClipped(frame_bb.Min + style.FramePadding, value_bb.Max, preview_value, NULL, NULL, ImVec2(0.0f, 0.0f));
	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	if ((pressed || g.NavActivateId == id) && !popup_open)
	{
		if (window->DC.NavLayerCurrent == 0)
			window->NavLastIds[0] = id;
		OpenPopupEx(id);
		popup_open = true;
	}

	if (!popup_open)
		return false;

	if (backup_next_window_size_constraint)
	{
		g.NextWindowData.SizeConstraintCond = backup_next_window_size_constraint;
		g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
	}
	else
	{
		if ((flags & ImGuiComboFlags_HeightMask_) == 0)
			flags |= ImGuiComboFlags_HeightRegular;
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_));    // Only one
		int popup_max_height_in_items = -1;
		if (flags & ImGuiComboFlags_HeightRegular)     popup_max_height_in_items = 8;
		else if (flags & ImGuiComboFlags_HeightSmall)  popup_max_height_in_items = 4;
		else if (flags & ImGuiComboFlags_HeightLarge)  popup_max_height_in_items = 20;
		SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));
	}

	char name[16];
	ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.CurrentPopupStack.Size); // Recycle windows based on depth

	// Peak into expected window size so we can position it
	if (ImGuiWindow* popup_window = FindWindowByName(name))
		if (popup_window->WasActive)
		{
			ImVec2 size_contents = CalcSizeContents(popup_window);
			ImVec2 size_expected = CalcSizeAfterConstraint(popup_window, CalcSizeAutoFit(popup_window, size_contents));
			if (flags & ImGuiComboFlags_PopupAlignLeft)
				popup_window->AutoPosLastDirection = ImGuiDir_Left;
			ImVec2 pos = FindBestWindowPosForPopup(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, frame_bb, ImGuiPopupPositionPolicy_ComboBox);
			SetNextWindowPos(pos);
		}

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	if (!Begin(name, NULL, window_flags))
	{
		EndPopup();
		IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
		return false;
	}

	// Horizontally align ourselves with the framed text
	if (style.FramePadding.x != style.WindowPadding.x)
		Indent(style.FramePadding.x - style.WindowPadding.x);

	return true;
}

void ImGui::EndCombo()
{
	const ImGuiStyle& style = GImGui->Style;
	if (style.FramePadding.x != style.WindowPadding.x)
		Unindent(style.FramePadding.x - style.WindowPadding.x);
	EndPopup();
}

// Old API, prefer using BeginCombo() nowadays if you can.
bool ImGui::Combo(const char* label, int* current_item, bool(*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
{
	ImGuiContext& g = *GImGui;

	const char* preview_text = NULL;
	if (*current_item >= 0 && *current_item < items_count)
		items_getter(data, *current_item, &preview_text);

	// The old Combo() API exposed "popup_max_height_in_items", however the new more general BeginCombo() API doesn't, so we emulate it here.
	if (popup_max_height_in_items != -1 && !g.NextWindowData.SizeConstraintCond)
	{
		float popup_max_height = CalcMaxPopupHeightFromItemCount(popup_max_height_in_items);
		SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, popup_max_height));
	}

	if (!BeginCombo(label, preview_text, 0))
		return false;

	// Display items
	// FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
	bool value_changed = false;
	for (int i = 0; i < items_count; i++)
	{
		PushID((void*)(intptr_t)i);
		const bool item_selected = (i == *current_item);
		const char* item_text;
		if (!items_getter(data, i, &item_text))
			item_text = "*Unknown item*";
		if (Selectable(item_text, item_selected))
		{
			value_changed = true;
			*current_item = i;
		}
		if (item_selected)
			SetItemDefaultFocus();
		PopID();
	}

	EndCombo();
	return value_changed;
}

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}

static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
{
	// FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
	const char* items_separated_by_zeros = (const char*)data;
	int items_count = 0;
	const char* p = items_separated_by_zeros;
	while (*p)
	{
		if (idx == items_count)
			break;
		p += strlen(p) + 1;
		items_count++;
	}
	if (!*p)
		return false;
	if (out_text)
		*out_text = p;
	return true;
}

// Combo box helper allowing to pass an array of strings.
bool ImGui::Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
{
	const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
	return value_changed;
}

// Combo box helper allowing to pass all items in a single string.
bool ImGui::Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
{
	int items_count = 0;
	const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
	while (*p)
	{
		p += strlen(p) + 1;
		items_count++;
	}
	bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
	return value_changed;
}

// Tip: pass an empty label (e.g. "##dummy") then you can use the space to draw other text or image.
// But you need to make sure the ID is unique, e.g. enclose calls in PushID/PopID.
bool ImGui::Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet) // FIXME-OPT: Avoid if vertically clipped.
		PopClipRect();

	ImGuiID id = window->GetID(label);
	ImVec2 label_size = CalcTextSize(label, NULL, true);
	ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
	ImVec2 pos = window->DC.CursorPos;
	pos.y += window->DC.CurrentLineTextBaseOffset;
	ImRect bb(pos, pos + size);
	ItemSize(bb);

	// Fill horizontal space.
	ImVec2 window_padding = window->WindowPadding;
	float max_x = (flags & ImGuiSelectableFlags_SpanAllColumns) ? GetWindowContentRegionMax().x : GetContentRegionMax().x;
	float w_draw = ImMax(label_size.x, window->Pos.x + max_x - window_padding.x - window->DC.CursorPos.x);
	ImVec2 size_draw((size_arg.x != 0 && !(flags & ImGuiSelectableFlags_DrawFillAvailWidth)) ? size_arg.x : w_draw, size_arg.y != 0.0f ? size_arg.y : size.y);
	ImRect bb_with_spacing(pos, pos + size_draw);
	if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_DrawFillAvailWidth))
		bb_with_spacing.Max.x += window_padding.x;

	// Selectables are tightly packed together, we extend the box to cover spacing between selectable.
	float spacing_L = (float)(int)(style.ItemSpacing.x * 0.5f);
	float spacing_U = (float)(int)(style.ItemSpacing.y * 0.5f);
	float spacing_R = style.ItemSpacing.x - spacing_L;
	float spacing_D = style.ItemSpacing.y - spacing_U;
	bb_with_spacing.Min.x -= spacing_L;
	bb_with_spacing.Min.y -= spacing_U;
	bb_with_spacing.Max.x += spacing_R;
	bb_with_spacing.Max.y += spacing_D;
	if (!ItemAdd(bb_with_spacing, (flags & ImGuiSelectableFlags_Disabled) ? 0 : id))
	{
		if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet)
			PushColumnClipRect();
		return false;
	}

	ImGuiButtonFlags button_flags = 0;
	if (flags & ImGuiSelectableFlags_Menu) button_flags |= ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_NoHoldingActiveID;
	if (flags & ImGuiSelectableFlags_MenuItem) button_flags |= ImGuiButtonFlags_PressedOnRelease;
	if (flags & ImGuiSelectableFlags_Disabled) button_flags |= ImGuiButtonFlags_Disabled;
	if (flags & ImGuiSelectableFlags_AllowDoubleClick) button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
	bool hovered, held;
	bool pressed = ButtonBehavior(bb_with_spacing, id, &hovered, &held, button_flags);
	if (flags & ImGuiSelectableFlags_Disabled)
		selected = false;

	// Hovering selectable with mouse updates NavId accordingly so navigation can be resumed with gamepad/keyboard (this doesn't happen on most widgets)
	if (pressed || hovered)// && (g.IO.MouseDelta.x != 0.0f || g.IO.MouseDelta.y != 0.0f))
		if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerActiveMask)
		{
			g.NavDisableHighlight = true;
			SetNavID(id, window->DC.NavLayerCurrent);
		}

	// Render
	if (hovered || selected)
	{
		const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
		RenderFrame(bb_with_spacing.Min, bb_with_spacing.Max, col, false, 0.0f);
		RenderNavHighlight(bb_with_spacing, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);
	}

	if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet)
	{
		PushColumnClipRect();
		bb_with_spacing.Max.x -= (GetContentRegionMax().x - max_x);
	}

	if (flags & ImGuiSelectableFlags_Disabled) PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
	RenderTextClipped(bb.Min, bb_with_spacing.Max, label, NULL, &label_size, ImVec2(0.0f, 0.0f));
	if (flags & ImGuiSelectableFlags_Disabled) PopStyleColor();

	// Automatically close popups
	if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(window->DC.ItemFlags & ImGuiItemFlags_SelectableDontClosePopup))
		CloseCurrentPopup();
	return pressed;
}

bool ImGui::Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
	if (Selectable(label, *p_selected, flags, size_arg))
	{
		*p_selected = !*p_selected;
		return true;
	}
	return false;
}

// Helper to calculate the size of a listbox and display a label on the right.
// Tip: To have a list filling the entire window width, PushItemWidth(-1) and pass an empty label "##empty"
bool ImGui::ListBoxHeader(const char* label, const ImVec2& size_arg)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiStyle& style = GetStyle();
	const ImGuiID id = GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	// Size default to hold ~7 items. Fractional number of items helps seeing that we can scroll down/up without looking at scrollbar.
	ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), GetTextLineHeightWithSpacing() * 7.4f + style.ItemSpacing.y);
	ImVec2 frame_size = ImVec2(size.x, ImMax(size.y, label_size.y));
	ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
	ImRect bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
	window->DC.LastItemRect = bb; // Forward storage for ListBoxFooter.. dodgy.

	BeginGroup();
	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	BeginChildFrame(id, frame_bb.GetSize());
	return true;
}

bool ImGui::ListBoxHeader(const char* label, int items_count, int height_in_items)
{
	// Size default to hold ~7 items. Fractional number of items helps seeing that we can scroll down/up without looking at scrollbar.
	// However we don't add +0.40f if items_count <= height_in_items. It is slightly dodgy, because it means a dynamic list of items will make the widget resize occasionally when it crosses that size.
	// I am expecting that someone will come and complain about this behavior in a remote future, then we can advise on a better solution.
	if (height_in_items < 0)
		height_in_items = ImMin(items_count, 7);
	float height_in_items_f = height_in_items < items_count ? (height_in_items + 0.40f) : (height_in_items + 0.00f);

	// We include ItemSpacing.y so that a list sized for the exact number of items doesn't make a scrollbar appears. We could also enforce that by passing a flag to BeginChild().
	ImVec2 size;
	size.x = 0.0f;
	size.y = GetTextLineHeightWithSpacing() * height_in_items_f + GetStyle().ItemSpacing.y;
	return ListBoxHeader(label, size);
}

void ImGui::ListBoxFooter()
{
	ImGuiWindow* parent_window = GetCurrentWindow()->ParentWindow;
	const ImRect bb = parent_window->DC.LastItemRect;
	const ImGuiStyle& style = GetStyle();

	EndChildFrame();

	// Redeclare item size so that it includes the label (we have stored the full size in LastItemRect)
	// We call SameLine() to restore DC.CurrentLine* data
	SameLine();
	parent_window->DC.CursorPos = bb.Min;
	ItemSize(bb, style.FramePadding.y);
	EndGroup();
}

bool ImGui::ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_items)
{
	const bool value_changed = ListBox(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_items);
	return value_changed;
}

bool ImGui::ListBox(const char* label, int* current_item, bool(*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items)
{
	if (!ListBoxHeader(label, items_count, height_in_items))
		return false;

	// Assume all items have even height (= 1 line of text). If you need items of different or variable sizes you can create a custom version of ListBox() in your code without using the clipper.
	bool value_changed = false;
	ImGuiListClipper clipper(items_count, GetTextLineHeightWithSpacing()); // We know exactly our line height here so we pass it as a minor optimization, but generally you don't need to.
	while (clipper.Step())
		for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		{
			const bool item_selected = (i == *current_item);
			const char* item_text;
			if (!items_getter(data, i, &item_text))
				item_text = "*Unknown item*";

			PushID(i);
			if (Selectable(item_text, item_selected))
			{
				*current_item = i;
				value_changed = true;
			}
			if (item_selected)
				SetItemDefaultFocus();
			PopID();
		}
	ListBoxFooter();
	return value_changed;
}

bool ImGui::MenuItem(const char* label, const char* shortcut, bool selected, bool enabled)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	ImGuiStyle& style = g.Style;
	ImVec2 pos = window->DC.CursorPos;
	ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImGuiSelectableFlags flags = ImGuiSelectableFlags_MenuItem | (enabled ? 0 : ImGuiSelectableFlags_Disabled);
	bool pressed;
	if (window->DC.LayoutType == ImGuiLayoutType_Horizontal)
	{
		// Mimic the exact layout spacing of BeginMenu() to allow MenuItem() inside a menu bar, which is a little misleading but may be useful
		// Note that in this situation we render neither the shortcut neither the selected tick mark
		float w = label_size.x;
		window->DC.CursorPos.x += (float)(int)(style.ItemSpacing.x * 0.5f);
		PushStyleVar(ImGuiStyleVar_ItemSpacing, style.ItemSpacing * 2.0f);
		pressed = Selectable(label, false, flags, ImVec2(w, 0.0f));
		PopStyleVar();
		window->DC.CursorPos.x += (float)(int)(style.ItemSpacing.x * (-1.0f + 0.5f)); // -1 spacing to compensate the spacing added when Selectable() did a SameLine(). It would also work to call SameLine() ourselves after the PopStyleVar().
	}
	else
	{
		ImVec2 shortcut_size = shortcut ? CalcTextSize(shortcut, NULL) : ImVec2(0.0f, 0.0f);
		float w = window->MenuColumns.DeclColumns(label_size.x, shortcut_size.x, (float)(int)(g.FontSize * 1.20f)); // Feedback for next frame
		float extra_w = ImMax(0.0f, GetContentRegionAvail().x - w);
		pressed = Selectable(label, false, flags | ImGuiSelectableFlags_DrawFillAvailWidth, ImVec2(w, 0.0f));
		if (shortcut_size.x > 0.0f)
		{
			PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
			RenderText(pos + ImVec2(window->MenuColumns.Pos[1] + extra_w, 0.0f), shortcut, NULL, false);
			PopStyleColor();
		}
		if (selected)
			RenderCheckMark(pos + ImVec2(window->MenuColumns.Pos[2] + extra_w + g.FontSize * 0.40f, g.FontSize * 0.134f * 0.5f), GetColorU32(enabled ? ImGuiCol_Text : ImGuiCol_TextDisabled), g.FontSize  * 0.866f);
	}
	return pressed;
}

bool ImGui::MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled)
{
	if (MenuItem(label, shortcut, p_selected ? *p_selected : false, enabled))
	{
		if (p_selected)
			*p_selected = !*p_selected;
		return true;
	}
	return false;
}

bool ImGui::BeginMainMenuBar()
{
	ImGuiContext& g = *GImGui;
	SetNextWindowPos(ImVec2(0.0f, 0.0f));
	SetNextWindowSize(ImVec2(g.IO.DisplaySize.x, g.FontBaseSize + g.Style.FramePadding.y * 2.0f));
	PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));
	if (!Begin("##MainMenuBar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar)
		|| !BeginMenuBar())
	{
		End();
		PopStyleVar(2);
		return false;
	}
	g.CurrentWindow->DC.MenuBarOffsetX += g.Style.DisplaySafeAreaPadding.x;
	return true;
}

void ImGui::EndMainMenuBar()
{
	EndMenuBar();

	// When the user has left the menu layer (typically: closed menus through activation of an item), we restore focus to the previous window
	ImGuiContext& g = *GImGui;
	if (g.CurrentWindow == g.NavWindow && g.NavLayer == 0)
		FocusFrontMostActiveWindow(g.NavWindow);

	End();
	PopStyleVar(2);
}

bool ImGui::BeginMenuBar()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;
	if (!(window->Flags & ImGuiWindowFlags_MenuBar))
		return false;

	IM_ASSERT(!window->DC.MenuBarAppending);
	BeginGroup(); // Save position
	PushID("##menubar");

	// We don't clip with regular window clipping rectangle as it is already set to the area below. However we clip with window full rect.
	// We remove 1 worth of rounding to Max.x to that text in long menus don't tend to display over the lower-right rounded area, which looks particularly glitchy.
	ImRect bar_rect = window->MenuBarRect();
	ImRect clip_rect(ImFloor(bar_rect.Min.x + 0.5f), ImFloor(bar_rect.Min.y + window->WindowBorderSize + 0.5f), ImFloor(ImMax(bar_rect.Min.x, bar_rect.Max.x - window->WindowRounding) + 0.5f), ImFloor(bar_rect.Max.y + 0.5f));
	clip_rect.ClipWith(window->WindowRectClipped);
	PushClipRect(clip_rect.Min, clip_rect.Max, false);

	window->DC.CursorPos = ImVec2(bar_rect.Min.x + window->DC.MenuBarOffsetX, bar_rect.Min.y);// + g.Style.FramePadding.y);
	window->DC.LayoutType = ImGuiLayoutType_Horizontal;
	window->DC.NavLayerCurrent++;
	window->DC.NavLayerCurrentMask <<= 1;
	window->DC.MenuBarAppending = true;
	AlignTextToFramePadding();
	return true;
}

void ImGui::EndMenuBar()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;
	ImGuiContext& g = *GImGui;

	// Nav: When a move request within one of our child menu failed, capture the request to navigate among our siblings.
	if (NavMoveRequestButNoResultYet() && (g.NavMoveDir == ImGuiDir_Left || g.NavMoveDir == ImGuiDir_Right) && (g.NavWindow->Flags & ImGuiWindowFlags_ChildMenu))
	{
		ImGuiWindow* nav_earliest_child = g.NavWindow;
		while (nav_earliest_child->ParentWindow && (nav_earliest_child->ParentWindow->Flags & ImGuiWindowFlags_ChildMenu))
			nav_earliest_child = nav_earliest_child->ParentWindow;
		if (nav_earliest_child->ParentWindow == window && nav_earliest_child->DC.ParentLayoutType == ImGuiLayoutType_Horizontal && g.NavMoveRequestForward == ImGuiNavForward_None)
		{
			// To do so we claim focus back, restore NavId and then process the movement request for yet another frame.
			// This involve a one-frame delay which isn't very problematic in this situation. We could remove it by scoring in advance for multiple window (probably not worth the hassle/cost)
			IM_ASSERT(window->DC.NavLayerActiveMaskNext & 0x02); // Sanity check
			FocusWindow(window);
			SetNavIDWithRectRel(window->NavLastIds[1], 1, window->NavRectRel[1]);
			g.NavLayer = 1;
			g.NavDisableHighlight = true; // Hide highlight for the current frame so we don't see the intermediary selection.
			g.NavMoveRequestForward = ImGuiNavForward_ForwardQueued;
			NavMoveRequestCancel();
		}
	}

	IM_ASSERT(window->Flags & ImGuiWindowFlags_MenuBar);
	IM_ASSERT(window->DC.MenuBarAppending);
	PopClipRect();
	PopID();
	window->DC.MenuBarOffsetX = window->DC.CursorPos.x - window->MenuBarRect().Min.x;
	window->DC.GroupStack.back().AdvanceCursor = false;
	EndGroup();
	window->DC.LayoutType = ImGuiLayoutType_Vertical;
	window->DC.NavLayerCurrent--;
	window->DC.NavLayerCurrentMask >>= 1;
	window->DC.MenuBarAppending = false;
}

bool ImGui::BeginMenu(const char* label, bool enabled)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 label_size = CalcTextSize(label, NULL, true);

	bool pressed;
	bool menu_is_open = IsPopupOpen(id);
	bool menuset_is_open = !(window->Flags & ImGuiWindowFlags_Popup) && (g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].OpenParentId == window->IDStack.back());
	ImGuiWindow* backed_nav_window = g.NavWindow;
	if (menuset_is_open)
		g.NavWindow = window;  // Odd hack to allow hovering across menus of a same menu-set (otherwise we wouldn't be able to hover parent)

	// The reference position stored in popup_pos will be used by Begin() to find a suitable position for the child menu (using FindBestPopupWindowPos).
	ImVec2 popup_pos, pos = window->DC.CursorPos;
	if (window->DC.LayoutType == ImGuiLayoutType_Horizontal)
	{
		// Menu inside an horizontal menu bar
		// Selectable extend their highlight by half ItemSpacing in each direction.
		// For ChildMenu, the popup position will be overwritten by the call to FindBestPopupWindowPos() in Begin()
		popup_pos = ImVec2(pos.x - window->WindowPadding.x, pos.y - style.FramePadding.y + window->MenuBarHeight());
		window->DC.CursorPos.x += (float)(int)(style.ItemSpacing.x * 0.5f);
		PushStyleVar(ImGuiStyleVar_ItemSpacing, style.ItemSpacing * 2.0f);
		float w = label_size.x;
		pressed = Selectable(label, menu_is_open, ImGuiSelectableFlags_Menu | ImGuiSelectableFlags_DontClosePopups | (!enabled ? ImGuiSelectableFlags_Disabled : 0), ImVec2(w, 0.0f));
		PopStyleVar();
		window->DC.CursorPos.x += (float)(int)(style.ItemSpacing.x * (-1.0f + 0.5f)); // -1 spacing to compensate the spacing added when Selectable() did a SameLine(). It would also work to call SameLine() ourselves after the PopStyleVar().
	}
	else
	{
		// Menu inside a menu
		popup_pos = ImVec2(pos.x, pos.y - style.WindowPadding.y);
		float w = window->MenuColumns.DeclColumns(label_size.x, 0.0f, (float)(int)(g.FontSize * 1.20f)); // Feedback to next frame
		float extra_w = ImMax(0.0f, GetContentRegionAvail().x - w);
		pressed = Selectable(label, menu_is_open, ImGuiSelectableFlags_Menu | ImGuiSelectableFlags_DontClosePopups | ImGuiSelectableFlags_DrawFillAvailWidth | (!enabled ? ImGuiSelectableFlags_Disabled : 0), ImVec2(w, 0.0f));
		if (!enabled) PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
		RenderArrow(pos + ImVec2(window->MenuColumns.Pos[2] + extra_w + g.FontSize * 0.30f, 0.0f), ImGuiDir_Right);
		if (!enabled) PopStyleColor();
	}

	const bool hovered = enabled && ItemHoverable(window->DC.LastItemRect, id);
	if (menuset_is_open)
		g.NavWindow = backed_nav_window;

	bool want_open = false, want_close = false;
	if (window->DC.LayoutType == ImGuiLayoutType_Vertical) // (window->Flags & (ImGuiWindowFlags_Popup|ImGuiWindowFlags_ChildMenu))
	{
		// Implement http://bjk5.com/post/44698559168/breaking-down-amazons-mega-dropdown to avoid using timers, so menus feels more reactive.
		bool moving_within_opened_triangle = false;
		if (g.HoveredWindow == window && g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].ParentWindow == window && !(window->Flags & ImGuiWindowFlags_MenuBar))
		{
			if (ImGuiWindow* next_window = g.OpenPopupStack[g.CurrentPopupStack.Size].Window)
			{
				ImRect next_window_rect = next_window->Rect();
				ImVec2 ta = g.IO.MousePos - g.IO.MouseDelta;
				ImVec2 tb = (window->Pos.x < next_window->Pos.x) ? next_window_rect.GetTL() : next_window_rect.GetTR();
				ImVec2 tc = (window->Pos.x < next_window->Pos.x) ? next_window_rect.GetBL() : next_window_rect.GetBR();
				float extra = ImClamp(fabsf(ta.x - tb.x) * 0.30f, 5.0f, 30.0f); // add a bit of extra slack.
				ta.x += (window->Pos.x < next_window->Pos.x) ? -0.5f : +0.5f;   // to avoid numerical issues
				tb.y = ta.y + ImMax((tb.y - extra) - ta.y, -100.0f);            // triangle is maximum 200 high to limit the slope and the bias toward large sub-menus // FIXME: Multiply by fb_scale?
				tc.y = ta.y + ImMin((tc.y + extra) - ta.y, +100.0f);
				moving_within_opened_triangle = ImTriangleContainsPoint(ta, tb, tc, g.IO.MousePos);
				//window->DrawList->PushClipRectFullScreen(); window->DrawList->AddTriangleFilled(ta, tb, tc, moving_within_opened_triangle ? IM_COL32(0,128,0,128) : IM_COL32(128,0,0,128)); window->DrawList->PopClipRect(); // Debug
			}
		}

		want_close = (menu_is_open && !hovered && g.HoveredWindow == window && g.HoveredIdPreviousFrame != 0 && g.HoveredIdPreviousFrame != id && !moving_within_opened_triangle);
		want_open = (!menu_is_open && hovered && !moving_within_opened_triangle) || (!menu_is_open && hovered && pressed);

		if (g.NavActivateId == id)
		{
			want_close = menu_is_open;
			want_open = !menu_is_open;
		}
		if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == ImGuiDir_Right) // Nav-Right to open
		{
			want_open = true;
			NavMoveRequestCancel();
		}
	}
	else
	{
		// Menu bar
		if (menu_is_open && pressed && menuset_is_open) // Click an open menu again to close it
		{
			want_close = true;
			want_open = menu_is_open = false;
		}
		else if (pressed || (hovered && menuset_is_open && !menu_is_open)) // First click to open, then hover to open others
		{
			want_open = true;
		}
		else if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == ImGuiDir_Down) // Nav-Down to open
		{
			want_open = true;
			NavMoveRequestCancel();
		}
	}

	if (!enabled) // explicitly close if an open menu becomes disabled, facilitate users code a lot in pattern such as 'if (BeginMenu("options", has_object)) { ..use object.. }'
		want_close = true;
	if (want_close && IsPopupOpen(id))
		ClosePopupToLevel(g.CurrentPopupStack.Size);

	if (!menu_is_open && want_open && g.OpenPopupStack.Size > g.CurrentPopupStack.Size)
	{
		// Don't recycle same menu level in the same frame, first close the other menu and yield for a frame.
		OpenPopup(label);
		return false;
	}

	menu_is_open |= want_open;
	if (want_open)
		OpenPopup(label);

	if (menu_is_open)
	{
		SetNextWindowPos(popup_pos, ImGuiCond_Always);
		ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ((window->Flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_ChildMenu)) ? ImGuiWindowFlags_ChildMenu | ImGuiWindowFlags_ChildWindow : ImGuiWindowFlags_ChildMenu);
		menu_is_open = BeginPopupEx(id, flags); // menu_is_open can be 'false' when the popup is completely clipped (e.g. zero size display)
	}

	return menu_is_open;
}

void ImGui::EndMenu()
{
	// Nav: When a left move request _within our child menu_ failed, close the menu.
	// A menu doesn't close itself because EndMenuBar() wants the catch the last Left<>Right inputs.
	// However it means that with the current code, a BeginMenu() from outside another menu or a menu-bar won't be closable with the Left direction.
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	if (g.NavWindow && g.NavWindow->ParentWindow == window && g.NavMoveDir == ImGuiDir_Left && NavMoveRequestButNoResultYet() && window->DC.LayoutType == ImGuiLayoutType_Vertical)
	{
		ClosePopupToLevel(g.OpenPopupStack.Size - 1);
		NavMoveRequestCancel();
	}

	EndPopup();
}

// Note: only access 3 floats if ImGuiColorEditFlags_NoAlpha flag is set.
void ImGui::ColorTooltip(const char* text, const float* col, ImGuiColorEditFlags flags)
{
	ImGuiContext& g = *GImGui;

	int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
	BeginTooltipEx(0, true);

	const char* text_end = text ? FindRenderedTextEnd(text, NULL) : text;
	if (text_end > text)
	{
		TextUnformatted(text, text_end);
		Separator();
	}

	ImVec2 sz(g.FontSize * 3 + g.Style.FramePadding.y * 2, g.FontSize * 3 + g.Style.FramePadding.y * 2);
	ColorButton("##preview", ImVec4(col[0], col[1], col[2], col[3]), (flags & (ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf)) | ImGuiColorEditFlags_NoTooltip, sz);
	SameLine();
	if (flags & ImGuiColorEditFlags_NoAlpha)
		Text("#%02X%02X%02X\nR: %d, G: %d, B: %d\n(%.3f, %.3f, %.3f)", cr, cg, cb, cr, cg, cb, col[0], col[1], col[2]);
	else
		Text("#%02X%02X%02X%02X\nR:%d, G:%d, B:%d, A:%d\n(%.3f, %.3f, %.3f, %.3f)", cr, cg, cb, ca, cr, cg, cb, ca, col[0], col[1], col[2], col[3]);
	EndTooltip();
}

static inline ImU32 ImAlphaBlendColor(ImU32 col_a, ImU32 col_b)
{
	float t = ((col_b >> IM_COL32_A_SHIFT) & 0xFF) / 255.f;
	int r = ImLerp((int)(col_a >> IM_COL32_R_SHIFT) & 0xFF, (int)(col_b >> IM_COL32_R_SHIFT) & 0xFF, t);
	int g = ImLerp((int)(col_a >> IM_COL32_G_SHIFT) & 0xFF, (int)(col_b >> IM_COL32_G_SHIFT) & 0xFF, t);
	int b = ImLerp((int)(col_a >> IM_COL32_B_SHIFT) & 0xFF, (int)(col_b >> IM_COL32_B_SHIFT) & 0xFF, t);
	return IM_COL32(r, g, b, 0xFF);
}

// NB: This is rather brittle and will show artifact when rounding this enabled if rounded corners overlap multiple cells. Caller currently responsible for avoiding that.
// I spent a non reasonable amount of time trying to getting this right for ColorButton with rounding+anti-aliasing+ImGuiColorEditFlags_HalfAlphaPreview flag + various grid sizes and offsets, and eventually gave up... probably more reasonable to disable rounding alltogether.
void ImGui::RenderColorRectWithAlphaCheckerboard(ImVec2 p_min, ImVec2 p_max, ImU32 col, float grid_step, ImVec2 grid_off, float rounding, int rounding_corners_flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (((col & IM_COL32_A_MASK) >> IM_COL32_A_SHIFT) < 0xFF)
	{
		ImU32 col_bg1 = GetColorU32(ImAlphaBlendColor(IM_COL32(204, 204, 204, 255), col));
		ImU32 col_bg2 = GetColorU32(ImAlphaBlendColor(IM_COL32(128, 128, 128, 255), col));
		window->DrawList->AddRectFilled(p_min, p_max, col_bg1, rounding, rounding_corners_flags);

		int yi = 0;
		for (float y = p_min.y + grid_off.y; y < p_max.y; y += grid_step, yi++)
		{
			float y1 = ImClamp(y, p_min.y, p_max.y), y2 = ImMin(y + grid_step, p_max.y);
			if (y2 <= y1)
				continue;
			for (float x = p_min.x + grid_off.x + (yi & 1) * grid_step; x < p_max.x; x += grid_step * 2.0f)
			{
				float x1 = ImClamp(x, p_min.x, p_max.x), x2 = ImMin(x + grid_step, p_max.x);
				if (x2 <= x1)
					continue;
				int rounding_corners_flags_cell = 0;
				if (y1 <= p_min.y) { if (x1 <= p_min.x) rounding_corners_flags_cell |= ImDrawCornerFlags_TopLeft; if (x2 >= p_max.x) rounding_corners_flags_cell |= ImDrawCornerFlags_TopRight; }
				if (y2 >= p_max.y) { if (x1 <= p_min.x) rounding_corners_flags_cell |= ImDrawCornerFlags_BotLeft; if (x2 >= p_max.x) rounding_corners_flags_cell |= ImDrawCornerFlags_BotRight; }
				rounding_corners_flags_cell &= rounding_corners_flags;
				window->DrawList->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), col_bg2, rounding_corners_flags_cell ? rounding : 0.0f, rounding_corners_flags_cell);
			}
		}
	}
	else
	{
		window->DrawList->AddRectFilled(p_min, p_max, col, rounding, rounding_corners_flags);
	}
}

void ImGui::SetColorEditOptions(ImGuiColorEditFlags flags)
{
	ImGuiContext& g = *GImGui;
	if ((flags & ImGuiColorEditFlags__InputsMask) == 0)
		flags |= ImGuiColorEditFlags__OptionsDefault & ImGuiColorEditFlags__InputsMask;
	if ((flags & ImGuiColorEditFlags__DataTypeMask) == 0)
		flags |= ImGuiColorEditFlags__OptionsDefault & ImGuiColorEditFlags__DataTypeMask;
	if ((flags & ImGuiColorEditFlags__PickerMask) == 0)
		flags |= ImGuiColorEditFlags__OptionsDefault & ImGuiColorEditFlags__PickerMask;
	IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags__InputsMask)));   // Check only 1 option is selected
	IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags__DataTypeMask))); // Check only 1 option is selected
	IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags__PickerMask)));   // Check only 1 option is selected
	g.ColorEditOptions = flags;
}

// A little colored square. Return true when clicked.
// FIXME: May want to display/ignore the alpha component in the color display? Yet show it in the tooltip.
// 'desc_id' is not called 'label' because we don't display it next to the button, but only in the tooltip.
bool ImGui::ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags, ImVec2 size)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiID id = window->GetID(desc_id);
	float default_size = GetFrameHeight();
	if (size.x == 0.0f)
		size.x = default_size;
	if (size.y == 0.0f)
		size.y = default_size;
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
	ItemSize(bb, (size.y >= default_size) ? g.Style.FramePadding.y : 0.0f);
	if (!ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held);

	if (flags & ImGuiColorEditFlags_NoAlpha)
		flags &= ~(ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf);

	ImVec4 col_without_alpha(col.x, col.y, col.z, 1.0f);
	float grid_step = ImMin(size.x, size.y) / 2.99f;
	float rounding = ImMin(g.Style.FrameRounding, grid_step * 0.5f);
	ImRect bb_inner = bb;
	float off = -0.75f; // The border (using Col_FrameBg) tends to look off when color is near-opaque and rounding is enabled. This offset seemed like a good middle ground to reduce those artifacts.
	bb_inner.Expand(off);
	if ((flags & ImGuiColorEditFlags_AlphaPreviewHalf) && col.w < 1.0f)
	{
		float mid_x = (float)(int)((bb_inner.Min.x + bb_inner.Max.x) * 0.5f + 0.5f);
		RenderColorRectWithAlphaCheckerboard(ImVec2(bb_inner.Min.x + grid_step, bb_inner.Min.y), bb_inner.Max, GetColorU32(col), grid_step, ImVec2(-grid_step + off, off), rounding, ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight);
		window->DrawList->AddRectFilled(bb_inner.Min, ImVec2(mid_x, bb_inner.Max.y), GetColorU32(col_without_alpha), rounding, ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft);
	}
	else
	{
		// Because GetColorU32() multiplies by the global style Alpha and we don't want to display a checkerboard if the source code had no alpha
		ImVec4 col_source = (flags & ImGuiColorEditFlags_AlphaPreview) ? col : col_without_alpha;
		if (col_source.w < 1.0f)
			RenderColorRectWithAlphaCheckerboard(bb_inner.Min, bb_inner.Max, GetColorU32(col_source), grid_step, ImVec2(off, off), rounding);
		else
			window->DrawList->AddRectFilled(bb_inner.Min, bb_inner.Max, GetColorU32(col_source), rounding, ImDrawCornerFlags_All);
	}
	RenderNavHighlight(bb, id);
	if (g.Style.FrameBorderSize > 0.0f)
		RenderFrameBorder(bb.Min, bb.Max, rounding);
	else
		window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), rounding); // Color button are often in need of some sort of border

	// Drag and Drop Source
	if (g.ActiveId == id && BeginDragDropSource()) // NB: The ActiveId test is merely an optional micro-optimization
	{
		if (flags & ImGuiColorEditFlags_NoAlpha)
			SetDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F, &col, sizeof(float) * 3, ImGuiCond_Once);
		else
			SetDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F, &col, sizeof(float) * 4, ImGuiCond_Once);
		ColorButton(desc_id, col, flags);
		SameLine();
		TextUnformatted("Color");
		EndDragDropSource();
		hovered = false;
	}

	// Tooltip
	if (!(flags & ImGuiColorEditFlags_NoTooltip) && hovered)
		ColorTooltip(desc_id, &col.x, flags & (ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf));

	return pressed;
}

bool ImGui::ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags)
{
	return ColorEdit4(label, col, flags | ImGuiColorEditFlags_NoAlpha);
}

void ImGui::ColorEditOptionsPopup(const float* col, ImGuiColorEditFlags flags)
{
	bool allow_opt_inputs = !(flags & ImGuiColorEditFlags__InputsMask);
	bool allow_opt_datatype = !(flags & ImGuiColorEditFlags__DataTypeMask);
	if ((!allow_opt_inputs && !allow_opt_datatype) || !BeginPopup("context"))
		return;
	ImGuiContext& g = *GImGui;
	ImGuiColorEditFlags opts = g.ColorEditOptions;
	if (allow_opt_inputs)
	{
		if (RadioButton("RGB", (opts & ImGuiColorEditFlags_RGB) ? 1 : 0)) opts = (opts & ~ImGuiColorEditFlags__InputsMask) | ImGuiColorEditFlags_RGB;
		if (RadioButton("HSV", (opts & ImGuiColorEditFlags_HSV) ? 1 : 0)) opts = (opts & ~ImGuiColorEditFlags__InputsMask) | ImGuiColorEditFlags_HSV;
		if (RadioButton("HEX", (opts & ImGuiColorEditFlags_HEX) ? 1 : 0)) opts = (opts & ~ImGuiColorEditFlags__InputsMask) | ImGuiColorEditFlags_HEX;
	}
	if (allow_opt_datatype)
	{
		if (allow_opt_inputs) Separator();
		if (RadioButton("0..255", (opts & ImGuiColorEditFlags_Uint8) ? 1 : 0)) opts = (opts & ~ImGuiColorEditFlags__DataTypeMask) | ImGuiColorEditFlags_Uint8;
		if (RadioButton("0.00..1.00", (opts & ImGuiColorEditFlags_Float) ? 1 : 0)) opts = (opts & ~ImGuiColorEditFlags__DataTypeMask) | ImGuiColorEditFlags_Float;
	}

	if (allow_opt_inputs || allow_opt_datatype)
		Separator();
	if (Button("Copy as..", ImVec2(-1, 0)))
		OpenPopup("Copy");
	if (BeginPopup("Copy"))
	{
		int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
		char buf[64];
		ImFormatString(buf, IM_ARRAYSIZE(buf), "(%.3ff, %.3ff, %.3ff, %.3ff)", col[0], col[1], col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : col[3]);
		if (Selectable(buf))
			SetClipboardText(buf);
		ImFormatString(buf, IM_ARRAYSIZE(buf), "(%d,%d,%d,%d)", cr, cg, cb, ca);
		if (Selectable(buf))
			SetClipboardText(buf);
		if (flags & ImGuiColorEditFlags_NoAlpha)
			ImFormatString(buf, IM_ARRAYSIZE(buf), "0x%02X%02X%02X", cr, cg, cb);
		else
			ImFormatString(buf, IM_ARRAYSIZE(buf), "0x%02X%02X%02X%02X", cr, cg, cb, ca);
		if (Selectable(buf))
			SetClipboardText(buf);
		EndPopup();
	}

	g.ColorEditOptions = opts;
	EndPopup();
}

static void ColorPickerOptionsPopup(ImGuiColorEditFlags flags, const float* ref_col)
{
	bool allow_opt_picker = !(flags & ImGuiColorEditFlags__PickerMask);
	bool allow_opt_alpha_bar = !(flags & ImGuiColorEditFlags_NoAlpha) && !(flags & ImGuiColorEditFlags_AlphaBar);
	if ((!allow_opt_picker && !allow_opt_alpha_bar) || !ImGui::BeginPopup("context"))
		return;
	ImGuiContext& g = *GImGui;
	if (allow_opt_picker)
	{
		ImVec2 picker_size(g.FontSize * 8, ImMax(g.FontSize * 8 - (ImGui::GetFrameHeight() + g.Style.ItemInnerSpacing.x), 1.0f)); // FIXME: Picker size copied from main picker function
		ImGui::PushItemWidth(picker_size.x);
		for (int picker_type = 0; picker_type < 2; picker_type++)
		{
			// Draw small/thumbnail version of each picker type (over an invisible button for selection)
			if (picker_type > 0) ImGui::Separator();
			ImGui::PushID(picker_type);
			ImGuiColorEditFlags picker_flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview | (flags & ImGuiColorEditFlags_NoAlpha);
			if (picker_type == 0) picker_flags |= ImGuiColorEditFlags_PickerHueBar;
			if (picker_type == 1) picker_flags |= ImGuiColorEditFlags_PickerHueWheel;
			ImVec2 backup_pos = ImGui::GetCursorScreenPos();
			if (ImGui::Selectable("##selectable", false, 0, picker_size)) // By default, Selectable() is closing popup
				g.ColorEditOptions = (g.ColorEditOptions & ~ImGuiColorEditFlags__PickerMask) | (picker_flags & ImGuiColorEditFlags__PickerMask);
			ImGui::SetCursorScreenPos(backup_pos);
			ImVec4 dummy_ref_col;
			memcpy(&dummy_ref_col.x, ref_col, sizeof(float) * (picker_flags & ImGuiColorEditFlags_NoAlpha ? 3 : 4));
			ImGui::ColorPicker4("##dummypicker", &dummy_ref_col.x, picker_flags);
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	if (allow_opt_alpha_bar)
	{
		if (allow_opt_picker) ImGui::Separator();
		ImGui::CheckboxFlags("Alpha Bar", (unsigned int*)&g.ColorEditOptions, ImGuiColorEditFlags_AlphaBar);
	}
	ImGui::EndPopup();
}

// Edit colors components (each component in 0.0f..1.0f range). 
// See enum ImGuiColorEditFlags_ for available options. e.g. Only access 3 floats if ImGuiColorEditFlags_NoAlpha flag is set.
// With typical options: Left-click on colored square to open color picker. Right-click to open option menu. CTRL-Click over input fields to edit them and TAB to go to next item.
bool ImGui::ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const float square_sz = GetFrameHeight();
	const float w_extra = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
	const float w_items_all = CalcItemWidth() - w_extra;
	const char* label_display_end = FindRenderedTextEnd(label);

	const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
	const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
	const int components = alpha ? 4 : 3;
	const ImGuiColorEditFlags flags_untouched = flags;

	BeginGroup();
	PushID(label);

	// If we're not showing any slider there's no point in doing any HSV conversions
	if (flags & ImGuiColorEditFlags_NoInputs)
		flags = (flags & (~ImGuiColorEditFlags__InputsMask)) | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_NoOptions;

	// Context menu: display and modify options (before defaults are applied)
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		ColorEditOptionsPopup(col, flags);

	// Read stored options
	if (!(flags & ImGuiColorEditFlags__InputsMask))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags__InputsMask);
	if (!(flags & ImGuiColorEditFlags__DataTypeMask))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags__DataTypeMask);
	if (!(flags & ImGuiColorEditFlags__PickerMask))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags__PickerMask);
	flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags__InputsMask | ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__PickerMask));

	// Convert to the formats we need
	float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
	if (flags & ImGuiColorEditFlags_HSV)
		ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
	int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

	bool value_changed = false;
	bool value_changed_as_float = false;

	if ((flags & (ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
	{
		// RGB/HSV 0..255 Sliders
		const float w_item_one = ImMax(1.0f, (float)(int)((w_items_all - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
		const float w_item_last = ImMax(1.0f, (float)(int)(w_items_all - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

		const bool hide_prefix = (w_item_one <= CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
		const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
		const char* fmt_table_int[3][4] =
		{
			{   "%3.0f",   "%3.0f",   "%3.0f",   "%3.0f" }, // Short display
			{ "R:%3.0f", "G:%3.0f", "B:%3.0f", "A:%3.0f" }, // Long display for RGBA
			{ "H:%3.0f", "S:%3.0f", "V:%3.0f", "A:%3.0f" }  // Long display for HSVA
		};
		const char* fmt_table_float[3][4] =
		{
			{   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
			{ "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
			{ "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
		};
		const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_HSV) ? 2 : 1;

		PushItemWidth(w_item_one);
		for (int n = 0; n < components; n++)
		{
			if (n > 0)
				SameLine(0, style.ItemInnerSpacing.x);
			if (n + 1 == components)
				PushItemWidth(w_item_last);
			if (flags & ImGuiColorEditFlags_Float)
				value_changed = value_changed_as_float = value_changed | DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
			else
				value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
			if (!(flags & ImGuiColorEditFlags_NoOptions))
				OpenPopupOnItemClick("context");
		}
		PopItemWidth();
		PopItemWidth();
	}
	else if ((flags & ImGuiColorEditFlags_HEX) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
	{
		// RGB Hexadecimal Input
		char buf[64];
		if (alpha)
			ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
		else
			ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
		PushItemWidth(w_items_all);
		if (InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
		{
			value_changed = true;
			char* p = buf;
			while (*p == '#' || ImCharIsSpace(*p))
				p++;
			i[0] = i[1] = i[2] = i[3] = 0;
			if (alpha)
				sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
			else
				sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			OpenPopupOnItemClick("context");
		PopItemWidth();
	}

	ImGuiWindow* picker_active_window = NULL;
	if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
	{
		if (!(flags & ImGuiColorEditFlags_NoInputs))
			SameLine(0, style.ItemInnerSpacing.x);

		const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
		if (ColorButton("##ColorButton", col_v4, flags))
		{
			if (!(flags & ImGuiColorEditFlags_NoPicker))
			{
				// Store current color and open a picker
				g.ColorPickerRef = col_v4;
				OpenPopup("picker");
				SetNextWindowPos(window->DC.LastItemRect.GetBL() + ImVec2(-1, style.ItemSpacing.y));
			}
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			OpenPopupOnItemClick("context");

		if (BeginPopup("picker"))
		{
			picker_active_window = g.CurrentWindow;
			if (label != label_display_end)
			{
				TextUnformatted(label, label_display_end);
				Separator();
			}
			ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__PickerMask | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
			ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags__InputsMask | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
			PushItemWidth(square_sz * 12.0f); // Use 256 + bar sizes?
			value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);
			PopItemWidth();
			EndPopup();
		}
	}

	if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
	{
		SameLine(0, style.ItemInnerSpacing.x);
		TextUnformatted(label, label_display_end);
	}

	// Convert back
	if (picker_active_window == NULL)
	{
		if (!value_changed_as_float)
			for (int n = 0; n < 4; n++)
				f[n] = i[n] / 255.0f;
		if (flags & ImGuiColorEditFlags_HSV)
			ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
		if (value_changed)
		{
			col[0] = f[0];
			col[1] = f[1];
			col[2] = f[2];
			if (alpha)
				col[3] = f[3];
		}
	}

	PopID();
	EndGroup();

	// Drag and Drop Target
	if ((window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HoveredRect) && BeginDragDropTarget()) // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
	{
		if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
		{
			memcpy((float*)col, payload->Data, sizeof(float) * 3);
			value_changed = true;
		}
		if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
		{
			memcpy((float*)col, payload->Data, sizeof(float) * components);
			value_changed = true;
		}
		EndDragDropTarget();
	}

	// When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
	if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
		window->DC.LastItemId = g.ActiveId;

	return value_changed;
}

bool ImGui::ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags)
{
	float col4[4] = { col[0], col[1], col[2], 1.0f };
	if (!ColorPicker4(label, col4, flags | ImGuiColorEditFlags_NoAlpha))
		return false;
	col[0] = col4[0]; col[1] = col4[1]; col[2] = col4[2];
	return true;
}

// 'pos' is position of the arrow tip. half_sz.x is length from base to tip. half_sz.y is length on each side.
static void RenderArrow(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImU32 col)
{
	switch (direction)
	{
	case ImGuiDir_Left:  draw_list->AddTriangleFilled(ImVec2(pos.x + half_sz.x, pos.y - half_sz.y), ImVec2(pos.x + half_sz.x, pos.y + half_sz.y), pos, col); return;
	case ImGuiDir_Right: draw_list->AddTriangleFilled(ImVec2(pos.x - half_sz.x, pos.y + half_sz.y), ImVec2(pos.x - half_sz.x, pos.y - half_sz.y), pos, col); return;
	case ImGuiDir_Up:    draw_list->AddTriangleFilled(ImVec2(pos.x + half_sz.x, pos.y + half_sz.y), ImVec2(pos.x - half_sz.x, pos.y + half_sz.y), pos, col); return;
	case ImGuiDir_Down:  draw_list->AddTriangleFilled(ImVec2(pos.x - half_sz.x, pos.y - half_sz.y), ImVec2(pos.x + half_sz.x, pos.y - half_sz.y), pos, col); return;
	case ImGuiDir_None: case ImGuiDir_COUNT: break; // Fix warnings
	}
}

static void RenderArrowsForVerticalBar(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, float bar_w)
{
	RenderArrow(draw_list, ImVec2(pos.x + half_sz.x + 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Right, IM_COL32_BLACK);
	RenderArrow(draw_list, ImVec2(pos.x + half_sz.x, pos.y), half_sz, ImGuiDir_Right, IM_COL32_WHITE);
	RenderArrow(draw_list, ImVec2(pos.x + bar_w - half_sz.x - 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Left, IM_COL32_BLACK);
	RenderArrow(draw_list, ImVec2(pos.x + bar_w - half_sz.x, pos.y), half_sz, ImGuiDir_Left, IM_COL32_WHITE);
}

// ColorPicker
// Note: only access 3 floats if ImGuiColorEditFlags_NoAlpha flag is set.
// FIXME: we adjust the big color square height based on item width, which may cause a flickering feedback loop (if automatic height makes a vertical scrollbar appears, affecting automatic width..) 
bool ImGui::ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	ImDrawList* draw_list = window->DrawList;

	ImGuiStyle& style = g.Style;
	ImGuiIO& io = g.IO;

	PushID(label);
	BeginGroup();

	if (!(flags & ImGuiColorEditFlags_NoSidePreview))
		flags |= ImGuiColorEditFlags_NoSmallPreview;

	// Context menu: display and store options.
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		ColorPickerOptionsPopup(flags, col);

	// Read stored options
	if (!(flags & ImGuiColorEditFlags__PickerMask))
		flags |= ((g.ColorEditOptions & ImGuiColorEditFlags__PickerMask) ? g.ColorEditOptions : ImGuiColorEditFlags__OptionsDefault) & ImGuiColorEditFlags__PickerMask;
	IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags__PickerMask))); // Check that only 1 is selected
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);

	// Setup
	int components = (flags & ImGuiColorEditFlags_NoAlpha) ? 3 : 4;
	bool alpha_bar = (flags & ImGuiColorEditFlags_AlphaBar) && !(flags & ImGuiColorEditFlags_NoAlpha);
	ImVec2 picker_pos = window->DC.CursorPos;
	float square_sz = GetFrameHeight();
	float bars_width = square_sz; // Arbitrary smallish width of Hue/Alpha picking bars
	float sv_picker_size = ImMax(bars_width * 1, CalcItemWidth() - (alpha_bar ? 2 : 1) * (bars_width + style.ItemInnerSpacing.x)); // Saturation/Value picking box
	float bar0_pos_x = picker_pos.x + sv_picker_size + style.ItemInnerSpacing.x;
	float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;
	float bars_triangles_half_sz = (float)(int)(bars_width * 0.20f);

	float backup_initial_col[4];
	memcpy(backup_initial_col, col, components * sizeof(float));

	float wheel_thickness = sv_picker_size * 0.08f;
	float wheel_r_outer = sv_picker_size * 0.50f;
	float wheel_r_inner = wheel_r_outer - wheel_thickness;
	ImVec2 wheel_center(picker_pos.x + (sv_picker_size + bars_width)*0.5f, picker_pos.y + sv_picker_size * 0.5f);

	// Note: the triangle is displayed rotated with triangle_pa pointing to Hue, but most coordinates stays unrotated for logic.
	float triangle_r = wheel_r_inner - (int)(sv_picker_size * 0.027f);
	ImVec2 triangle_pa = ImVec2(triangle_r, 0.0f); // Hue point.
	ImVec2 triangle_pb = ImVec2(triangle_r * -0.5f, triangle_r * -0.866025f); // Black point.
	ImVec2 triangle_pc = ImVec2(triangle_r * -0.5f, triangle_r * +0.866025f); // White point.

	float H, S, V;
	ColorConvertRGBtoHSV(col[0], col[1], col[2], H, S, V);

	bool value_changed = false, value_changed_h = false, value_changed_sv = false;

	PushItemFlag(ImGuiItemFlags_NoNav, true);
	if (flags & ImGuiColorEditFlags_PickerHueWheel)
	{
		// Hue wheel + SV triangle logic
		InvisibleButton("hsv", ImVec2(sv_picker_size + style.ItemInnerSpacing.x + bars_width, sv_picker_size));
		if (IsItemActive())
		{
			ImVec2 initial_off = g.IO.MouseClickedPos[0] - wheel_center;
			ImVec2 current_off = g.IO.MousePos - wheel_center;
			float initial_dist2 = ImLengthSqr(initial_off);
			if (initial_dist2 >= (wheel_r_inner - 1)*(wheel_r_inner - 1) && initial_dist2 <= (wheel_r_outer + 1)*(wheel_r_outer + 1))
			{
				// Interactive with Hue wheel
				H = atan2f(current_off.y, current_off.x) / IM_PI * 0.5f;
				if (H < 0.0f)
					H += 1.0f;
				value_changed = value_changed_h = true;
			}
			float cos_hue_angle = cosf(-H * 2.0f * IM_PI);
			float sin_hue_angle = sinf(-H * 2.0f * IM_PI);
			if (ImTriangleContainsPoint(triangle_pa, triangle_pb, triangle_pc, ImRotate(initial_off, cos_hue_angle, sin_hue_angle)))
			{
				// Interacting with SV triangle
				ImVec2 current_off_unrotated = ImRotate(current_off, cos_hue_angle, sin_hue_angle);
				if (!ImTriangleContainsPoint(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated))
					current_off_unrotated = ImTriangleClosestPoint(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated);
				float uu, vv, ww;
				ImTriangleBarycentricCoords(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated, uu, vv, ww);
				V = ImClamp(1.0f - vv, 0.0001f, 1.0f);
				S = ImClamp(uu / V, 0.0001f, 1.0f);
				value_changed = value_changed_sv = true;
			}
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			OpenPopupOnItemClick("context");
	}
	else if (flags & ImGuiColorEditFlags_PickerHueBar)
	{
		// SV rectangle logic
		InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size));
		if (IsItemActive())
		{
			S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
			V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			value_changed = value_changed_sv = true;
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			OpenPopupOnItemClick("context");

		// Hue bar logic
		SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
		InvisibleButton("hue", ImVec2(bars_width, sv_picker_size));
		if (IsItemActive())
		{
			H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			value_changed = value_changed_h = true;
		}
	}

	// Alpha bar logic
	if (alpha_bar)
	{
		SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
		InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size));
		if (IsItemActive())
		{
			col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			value_changed = true;
		}
	}
	PopItemFlag(); // ImGuiItemFlags_NoNav

	if (!(flags & ImGuiColorEditFlags_NoSidePreview))
	{
		SameLine(0, style.ItemInnerSpacing.x);
		BeginGroup();
	}

	if (!(flags & ImGuiColorEditFlags_NoLabel))
	{
		const char* label_display_end = FindRenderedTextEnd(label);
		if (label != label_display_end)
		{
			if ((flags & ImGuiColorEditFlags_NoSidePreview))
				SameLine(0, style.ItemInnerSpacing.x);
			TextUnformatted(label, label_display_end);
		}
	}

	if (!(flags & ImGuiColorEditFlags_NoSidePreview))
	{
		PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
		ImVec4 col_v4(col[0], col[1], col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : col[3]);
		if ((flags & ImGuiColorEditFlags_NoLabel))
			Text("Current");
		ColorButton("##current", col_v4, (flags & (ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip)), ImVec2(square_sz * 3, square_sz * 2));
		if (ref_col != NULL)
		{
			Text("Original");
			ImVec4 ref_col_v4(ref_col[0], ref_col[1], ref_col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : ref_col[3]);
			if (ColorButton("##original", ref_col_v4, (flags & (ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip)), ImVec2(square_sz * 3, square_sz * 2)))
			{
				memcpy(col, ref_col, components * sizeof(float));
				value_changed = true;
			}
		}
		PopItemFlag();
		EndGroup();
	}

	// Convert back color to RGB
	if (value_changed_h || value_changed_sv)
		ColorConvertHSVtoRGB(H >= 1.0f ? H - 10 * 1e-6f : H, S > 0.0f ? S : 10 * 1e-6f, V > 0.0f ? V : 1e-6f, col[0], col[1], col[2]);

	// R,G,B and H,S,V slider color editor
	if ((flags & ImGuiColorEditFlags_NoInputs) == 0)
	{
		PushItemWidth((alpha_bar ? bar1_pos_x : bar0_pos_x) + bars_width - picker_pos.x);
		ImGuiColorEditFlags sub_flags_to_forward = ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf;
		ImGuiColorEditFlags sub_flags = (flags & sub_flags_to_forward) | ImGuiColorEditFlags_NoPicker;
		if (flags & ImGuiColorEditFlags_RGB || (flags & ImGuiColorEditFlags__InputsMask) == 0)
			value_changed |= ColorEdit4("##rgb", col, sub_flags | ImGuiColorEditFlags_RGB);
		if (flags & ImGuiColorEditFlags_HSV || (flags & ImGuiColorEditFlags__InputsMask) == 0)
			value_changed |= ColorEdit4("##hsv", col, sub_flags | ImGuiColorEditFlags_HSV);
		if (flags & ImGuiColorEditFlags_HEX || (flags & ImGuiColorEditFlags__InputsMask) == 0)
			value_changed |= ColorEdit4("##hex", col, sub_flags | ImGuiColorEditFlags_HEX);
		PopItemWidth();
	}

	// Try to cancel hue wrap (after ColorEdit), if any
	if (value_changed)
	{
		float new_H, new_S, new_V;
		ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
		if (new_H <= 0 && H > 0)
		{
			if (new_V <= 0 && V != new_V)
				ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
			else if (new_S <= 0)
				ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
		}
	}

	ImVec4 hue_color_f(1, 1, 1, 1); ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
	ImU32 hue_color32 = ColorConvertFloat4ToU32(hue_color_f);
	ImU32 col32_no_alpha = ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 1.0f));

	const ImU32 hue_colors[6 + 1] = { IM_COL32(255,0,0,255), IM_COL32(255,255,0,255), IM_COL32(0,255,0,255), IM_COL32(0,255,255,255), IM_COL32(0,0,255,255), IM_COL32(255,0,255,255), IM_COL32(255,0,0,255) };
	ImVec2 sv_cursor_pos;

	if (flags & ImGuiColorEditFlags_PickerHueWheel)
	{
		// Render Hue Wheel
		const float aeps = 1.5f / wheel_r_outer; // Half a pixel arc length in radians (2pi cancels out).
		const int segment_per_arc = ImMax(4, (int)wheel_r_outer / 12);
		for (int n = 0; n < 6; n++)
		{
			const float a0 = (n) / 6.0f * 2.0f * IM_PI - aeps;
			const float a1 = (n + 1.0f) / 6.0f * 2.0f * IM_PI + aeps;
			const int vert_start_idx = draw_list->VtxBuffer.Size;
			draw_list->PathArcTo(wheel_center, (wheel_r_inner + wheel_r_outer)*0.5f, a0, a1, segment_per_arc);
			draw_list->PathStroke(IM_COL32_WHITE, false, wheel_thickness);
			const int vert_end_idx = draw_list->VtxBuffer.Size;

			// Paint colors over existing vertices
			ImVec2 gradient_p0(wheel_center.x + cosf(a0) * wheel_r_inner, wheel_center.y + sinf(a0) * wheel_r_inner);
			ImVec2 gradient_p1(wheel_center.x + cosf(a1) * wheel_r_inner, wheel_center.y + sinf(a1) * wheel_r_inner);
			ShadeVertsLinearColorGradientKeepAlpha(draw_list->VtxBuffer.Data + vert_start_idx, draw_list->VtxBuffer.Data + vert_end_idx, gradient_p0, gradient_p1, hue_colors[n], hue_colors[n + 1]);
		}

		// Render Cursor + preview on Hue Wheel
		float cos_hue_angle = cosf(H * 2.0f * IM_PI);
		float sin_hue_angle = sinf(H * 2.0f * IM_PI);
		ImVec2 hue_cursor_pos(wheel_center.x + cos_hue_angle * (wheel_r_inner + wheel_r_outer)*0.5f, wheel_center.y + sin_hue_angle * (wheel_r_inner + wheel_r_outer)*0.5f);
		float hue_cursor_rad = value_changed_h ? wheel_thickness * 0.65f : wheel_thickness * 0.55f;
		int hue_cursor_segments = ImClamp((int)(hue_cursor_rad / 1.4f), 9, 32);
		draw_list->AddCircleFilled(hue_cursor_pos, hue_cursor_rad, hue_color32, hue_cursor_segments);
		draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad + 1, IM_COL32(128, 128, 128, 255), hue_cursor_segments);
		draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad, IM_COL32_WHITE, hue_cursor_segments);

		// Render SV triangle (rotated according to hue)
		ImVec2 tra = wheel_center + ImRotate(triangle_pa, cos_hue_angle, sin_hue_angle);
		ImVec2 trb = wheel_center + ImRotate(triangle_pb, cos_hue_angle, sin_hue_angle);
		ImVec2 trc = wheel_center + ImRotate(triangle_pc, cos_hue_angle, sin_hue_angle);
		ImVec2 uv_white = GetFontTexUvWhitePixel();
		draw_list->PrimReserve(6, 6);
		draw_list->PrimVtx(tra, uv_white, hue_color32);
		draw_list->PrimVtx(trb, uv_white, hue_color32);
		draw_list->PrimVtx(trc, uv_white, IM_COL32_WHITE);
		draw_list->PrimVtx(tra, uv_white, IM_COL32_BLACK_TRANS);
		draw_list->PrimVtx(trb, uv_white, IM_COL32_BLACK);
		draw_list->PrimVtx(trc, uv_white, IM_COL32_BLACK_TRANS);
		draw_list->AddTriangle(tra, trb, trc, IM_COL32(128, 128, 128, 255), 1.5f);
		sv_cursor_pos = ImLerp(ImLerp(trc, tra, ImSaturate(S)), trb, ImSaturate(1 - V));
	}
	else if (flags & ImGuiColorEditFlags_PickerHueBar)
	{
		// Render SV Square
		draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), IM_COL32_WHITE, hue_color32, hue_color32, IM_COL32_WHITE);
		draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), IM_COL32_BLACK_TRANS, IM_COL32_BLACK_TRANS, IM_COL32_BLACK, IM_COL32_BLACK);
		RenderFrameBorder(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), 0.0f);
		sv_cursor_pos.x = ImClamp((float)(int)(picker_pos.x + ImSaturate(S)     * sv_picker_size + 0.5f), picker_pos.x + 2, picker_pos.x + sv_picker_size - 2); // Sneakily prevent the circle to stick out too much
		sv_cursor_pos.y = ImClamp((float)(int)(picker_pos.y + ImSaturate(1 - V) * sv_picker_size + 0.5f), picker_pos.y + 2, picker_pos.y + sv_picker_size - 2);

		// Render Hue Bar
		for (int i = 0; i < 6; ++i)
			draw_list->AddRectFilledMultiColor(ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6)), ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size / 6)), hue_colors[i], hue_colors[i], hue_colors[i + 1], hue_colors[i + 1]);
		float bar0_line_y = (float)(int)(picker_pos.y + H * sv_picker_size + 0.5f);
		RenderFrameBorder(ImVec2(bar0_pos_x, picker_pos.y), ImVec2(bar0_pos_x + bars_width, picker_pos.y + sv_picker_size), 0.0f);
		RenderArrowsForVerticalBar(draw_list, ImVec2(bar0_pos_x - 1, bar0_line_y), ImVec2(bars_triangles_half_sz + 1, bars_triangles_half_sz), bars_width + 2.0f);
	}

	// Render cursor/preview circle (clamp S/V within 0..1 range because floating points colors may lead HSV values to be out of range)
	float sv_cursor_rad = value_changed_sv ? 10.0f : 6.0f;
	draw_list->AddCircleFilled(sv_cursor_pos, sv_cursor_rad, col32_no_alpha, 12);
	draw_list->AddCircle(sv_cursor_pos, sv_cursor_rad + 1, IM_COL32(128, 128, 128, 255), 12);
	draw_list->AddCircle(sv_cursor_pos, sv_cursor_rad, IM_COL32_WHITE, 12);

	// Render alpha bar
	if (alpha_bar)
	{
		float alpha = ImSaturate(col[3]);
		ImRect bar1_bb(bar1_pos_x, picker_pos.y, bar1_pos_x + bars_width, picker_pos.y + sv_picker_size);
		RenderColorRectWithAlphaCheckerboard(bar1_bb.Min, bar1_bb.Max, IM_COL32(0, 0, 0, 0), bar1_bb.GetWidth() / 2.0f, ImVec2(0.0f, 0.0f));
		draw_list->AddRectFilledMultiColor(bar1_bb.Min, bar1_bb.Max, col32_no_alpha, col32_no_alpha, col32_no_alpha & ~IM_COL32_A_MASK, col32_no_alpha & ~IM_COL32_A_MASK);
		float bar1_line_y = (float)(int)(picker_pos.y + (1.0f - alpha) * sv_picker_size + 0.5f);
		RenderFrameBorder(bar1_bb.Min, bar1_bb.Max, 0.0f);
		RenderArrowsForVerticalBar(draw_list, ImVec2(bar1_pos_x - 1, bar1_line_y), ImVec2(bars_triangles_half_sz + 1, bars_triangles_half_sz), bars_width + 2.0f);
	}

	EndGroup();
	PopID();

	return value_changed && memcmp(backup_initial_col, col, components * sizeof(float));
}

// Horizontal separating line.
void ImGui::Separator()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;
	ImGuiContext& g = *GImGui;

	ImGuiSeparatorFlags flags = 0;
	if ((flags & (ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_Vertical)) == 0)
		flags |= (window->DC.LayoutType == ImGuiLayoutType_Horizontal) ? ImGuiSeparatorFlags_Vertical : ImGuiSeparatorFlags_Horizontal;
	IM_ASSERT(ImIsPowerOfTwo((int)(flags & (ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_Vertical))));   // Check that only 1 option is selected
	if (flags & ImGuiSeparatorFlags_Vertical)
	{
		VerticalSeparator();
		return;
	}

	// Horizontal Separator
	if (window->DC.ColumnsSet)
		PopClipRect();

	float x1 = window->Pos.x;
	float x2 = window->Pos.x + window->Size.x;
	if (!window->DC.GroupStack.empty())
		x1 += window->DC.IndentX;

	const ImRect bb(ImVec2(x1, window->DC.CursorPos.y), ImVec2(x2, window->DC.CursorPos.y + 1.0f));
	ItemSize(ImVec2(0.0f, 0.0f)); // NB: we don't provide our width so that it doesn't get feed back into AutoFit, we don't provide height to not alter layout.
	if (!ItemAdd(bb, 0))
	{
		if (window->DC.ColumnsSet)
			PushColumnClipRect();
		return;
	}

	window->DrawList->AddLine(bb.Min, ImVec2(bb.Max.x, bb.Min.y), GetColorU32(ImGuiCol_Separator));

	if (g.LogEnabled)
		LogRenderedText(NULL, IM_NEWLINE "--------------------------------");

	if (window->DC.ColumnsSet)
	{
		PushColumnClipRect();
		window->DC.ColumnsSet->LineMinY = window->DC.CursorPos.y;
	}
}

void ImGui::VerticalSeparator()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;
	ImGuiContext& g = *GImGui;

	float y1 = window->DC.CursorPos.y;
	float y2 = window->DC.CursorPos.y + window->DC.CurrentLineHeight;
	const ImRect bb(ImVec2(window->DC.CursorPos.x, y1), ImVec2(window->DC.CursorPos.x + 1.0f, y2));
	ItemSize(ImVec2(bb.GetWidth(), 0.0f));
	if (!ItemAdd(bb, 0))
		return;

	window->DrawList->AddLine(ImVec2(bb.Min.x, bb.Min.y), ImVec2(bb.Min.x, bb.Max.y), GetColorU32(ImGuiCol_Separator));
	if (g.LogEnabled)
		LogText(" |");
}

bool ImGui::SplitterBehavior(ImGuiID id, const ImRect& bb, ImGuiAxis axis, float* size1, float* size2, float min_size1, float min_size2, float hover_extend)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	const ImGuiItemFlags item_flags_backup = window->DC.ItemFlags;
	window->DC.ItemFlags |= ImGuiItemFlags_NoNav | ImGuiItemFlags_NoNavDefaultFocus;
	bool item_add = ItemAdd(bb, id);
	window->DC.ItemFlags = item_flags_backup;
	if (!item_add)
		return false;

	bool hovered, held;
	ImRect bb_interact = bb;
	bb_interact.Expand(axis == ImGuiAxis_Y ? ImVec2(0.0f, hover_extend) : ImVec2(hover_extend, 0.0f));
	ButtonBehavior(bb_interact, id, &hovered, &held, ImGuiButtonFlags_FlattenChildren | ImGuiButtonFlags_AllowItemOverlap);
	if (g.ActiveId != id)
		SetItemAllowOverlap();

	if (held || (g.HoveredId == id && g.HoveredIdPreviousFrame == id))
		SetMouseCursor(axis == ImGuiAxis_Y ? ImGuiMouseCursor_ResizeNS : ImGuiMouseCursor_ResizeEW);

	ImRect bb_render = bb;
	if (held)
	{
		ImVec2 mouse_delta_2d = g.IO.MousePos - g.ActiveIdClickOffset - bb_interact.Min;
		float mouse_delta = (axis == ImGuiAxis_Y) ? mouse_delta_2d.y : mouse_delta_2d.x;

		// Minimum pane size
		if (mouse_delta < min_size1 - *size1)
			mouse_delta = min_size1 - *size1;
		if (mouse_delta > *size2 - min_size2)
			mouse_delta = *size2 - min_size2;

		// Apply resize
		*size1 += mouse_delta;
		*size2 -= mouse_delta;
		bb_render.Translate((axis == ImGuiAxis_X) ? ImVec2(mouse_delta, 0.0f) : ImVec2(0.0f, mouse_delta));
	}

	// Render
	const ImU32 col = GetColorU32(held ? ImGuiCol_SeparatorActive : hovered ? ImGuiCol_SeparatorHovered : ImGuiCol_Separator);
	window->DrawList->AddRectFilled(bb_render.Min, bb_render.Max, col, g.Style.FrameRounding);

	return held;
}

void ImGui::Spacing()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;
	ItemSize(ImVec2(0, 0));
}

void ImGui::Dummy(const ImVec2& size)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
	ItemSize(bb);
	ItemAdd(bb, 0);
}

bool ImGui::IsRectVisible(const ImVec2& size)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ClipRect.Overlaps(ImRect(window->DC.CursorPos, window->DC.CursorPos + size));
}

bool ImGui::IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ClipRect.Overlaps(ImRect(rect_min, rect_max));
}

// Lock horizontal starting position + capture group bounding box into one "item" (so you can use IsItemHovered() or layout primitives such as SameLine() on whole group, etc.)
void ImGui::BeginGroup()
{
	ImGuiWindow* window = GetCurrentWindow();

	window->DC.GroupStack.resize(window->DC.GroupStack.Size + 1);
	ImGuiGroupData& group_data = window->DC.GroupStack.back();
	group_data.BackupCursorPos = window->DC.CursorPos;
	group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
	group_data.BackupIndentX = window->DC.IndentX;
	group_data.BackupGroupOffsetX = window->DC.GroupOffsetX;
	group_data.BackupCurrentLineHeight = window->DC.CurrentLineHeight;
	group_data.BackupCurrentLineTextBaseOffset = window->DC.CurrentLineTextBaseOffset;
	group_data.BackupLogLinePosY = window->DC.LogLinePosY;
	group_data.BackupActiveIdIsAlive = GImGui->ActiveIdIsAlive;
	group_data.AdvanceCursor = true;

	window->DC.GroupOffsetX = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffsetX;
	window->DC.IndentX = window->DC.GroupOffsetX;
	window->DC.CursorMaxPos = window->DC.CursorPos;
	window->DC.CurrentLineHeight = 0.0f;
	window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;
}

void ImGui::EndGroup()
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();

	IM_ASSERT(!window->DC.GroupStack.empty());    // Mismatched BeginGroup()/EndGroup() calls

	ImGuiGroupData& group_data = window->DC.GroupStack.back();

	ImRect group_bb(group_data.BackupCursorPos, window->DC.CursorMaxPos);
	group_bb.Max = ImMax(group_bb.Min, group_bb.Max);

	window->DC.CursorPos = group_data.BackupCursorPos;
	window->DC.CursorMaxPos = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
	window->DC.CurrentLineHeight = group_data.BackupCurrentLineHeight;
	window->DC.CurrentLineTextBaseOffset = group_data.BackupCurrentLineTextBaseOffset;
	window->DC.IndentX = group_data.BackupIndentX;
	window->DC.GroupOffsetX = group_data.BackupGroupOffsetX;
	window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;

	if (group_data.AdvanceCursor)
	{
		window->DC.CurrentLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, group_data.BackupCurrentLineTextBaseOffset);      // FIXME: Incorrect, we should grab the base offset from the *first line* of the group but it is hard to obtain now.
		ItemSize(group_bb.GetSize(), group_data.BackupCurrentLineTextBaseOffset);
		ItemAdd(group_bb, 0);
	}

	// If the current ActiveId was declared within the boundary of our group, we copy it to LastItemId so IsItemActive() will be functional on the entire group.
	// It would be be neater if we replaced window.DC.LastItemId by e.g. 'bool LastItemIsActive', but if you search for LastItemId you'll notice it is only used in that context.
	const bool active_id_within_group = (!group_data.BackupActiveIdIsAlive && g.ActiveIdIsAlive && g.ActiveId && g.ActiveIdWindow->RootWindow == window->RootWindow);
	if (active_id_within_group)
		window->DC.LastItemId = g.ActiveId;
	window->DC.LastItemRect = group_bb;

	window->DC.GroupStack.pop_back();

	//window->DrawList->AddRect(group_bb.Min, group_bb.Max, IM_COL32(255,0,255,255));   // [Debug]
}

// Gets back to previous line and continue with horizontal layout
//      pos_x == 0      : follow right after previous item
//      pos_x != 0      : align to specified x position (relative to window/group left)
//      spacing_w < 0   : use default spacing if pos_x == 0, no spacing if pos_x != 0
//      spacing_w >= 0  : enforce spacing amount
void ImGui::SameLine(float pos_x, float spacing_w)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	if (pos_x != 0.0f)
	{
		if (spacing_w < 0.0f) spacing_w = 0.0f;
		window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + pos_x + spacing_w + window->DC.GroupOffsetX + window->DC.ColumnsOffsetX;
		window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
	}
	else
	{
		if (spacing_w < 0.0f) spacing_w = g.Style.ItemSpacing.x;
		window->DC.CursorPos.x = window->DC.CursorPosPrevLine.x + spacing_w;
		window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
	}
	window->DC.CurrentLineHeight = window->DC.PrevLineHeight;
	window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
}

void ImGui::NewLine()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiLayoutType backup_layout_type = window->DC.LayoutType;
	window->DC.LayoutType = ImGuiLayoutType_Vertical;
	if (window->DC.CurrentLineHeight > 0.0f)     // In the event that we are on a line with items that is smaller that FontSize high, we will preserve its height.
		ItemSize(ImVec2(0, 0));
	else
		ItemSize(ImVec2(0.0f, g.FontSize));
	window->DC.LayoutType = backup_layout_type;
}

void ImGui::NextColumn()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems || window->DC.ColumnsSet == NULL)
		return;

	ImGuiContext& g = *GImGui;
	PopItemWidth();
	PopClipRect();

	ImGuiColumnsSet* columns = window->DC.ColumnsSet;
	columns->LineMaxY = ImMax(columns->LineMaxY, window->DC.CursorPos.y);
	if (++columns->Current < columns->Count)
	{
		// Columns 1+ cancel out IndentX
		window->DC.ColumnsOffsetX = GetColumnOffset(columns->Current) - window->DC.IndentX + g.Style.ItemSpacing.x;
		window->DrawList->ChannelsSetCurrent(columns->Current);
	}
	else
	{
		window->DC.ColumnsOffsetX = 0.0f;
		window->DrawList->ChannelsSetCurrent(0);
		columns->Current = 0;
		columns->LineMinY = columns->LineMaxY;
	}
	window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);
	window->DC.CursorPos.y = columns->LineMinY;
	window->DC.CurrentLineHeight = 0.0f;
	window->DC.CurrentLineTextBaseOffset = 0.0f;

	PushColumnClipRect();
	PushItemWidth(GetColumnWidth() * 0.65f);  // FIXME: Move on columns setup
}

int ImGui::GetColumnIndex()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.ColumnsSet ? window->DC.ColumnsSet->Current : 0;
}

int ImGui::GetColumnsCount()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.ColumnsSet ? window->DC.ColumnsSet->Count : 1;
}

static float OffsetNormToPixels(const ImGuiColumnsSet* columns, float offset_norm)
{
	return offset_norm * (columns->MaxX - columns->MinX);
}

static float PixelsToOffsetNorm(const ImGuiColumnsSet* columns, float offset)
{
	return offset / (columns->MaxX - columns->MinX);
}

static inline float GetColumnsRectHalfWidth() { return 4.0f; }

static float GetDraggedColumnOffset(ImGuiColumnsSet* columns, int column_index)
{
	// Active (dragged) column always follow mouse. The reason we need this is that dragging a column to the right edge of an auto-resizing
	// window creates a feedback loop because we store normalized positions. So while dragging we enforce absolute positioning.
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	IM_ASSERT(column_index > 0); // We are not supposed to drag column 0.
	IM_ASSERT(g.ActiveId == columns->ID + ImGuiID(column_index));

	float x = g.IO.MousePos.x - g.ActiveIdClickOffset.x + GetColumnsRectHalfWidth() - window->Pos.x;
	x = ImMax(x, ImGui::GetColumnOffset(column_index - 1) + g.Style.ColumnsMinSpacing);
	if ((columns->Flags & ImGuiColumnsFlags_NoPreserveWidths))
		x = ImMin(x, ImGui::GetColumnOffset(column_index + 1) - g.Style.ColumnsMinSpacing);

	return x;
}

float ImGui::GetColumnOffset(int column_index)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	ImGuiColumnsSet* columns = window->DC.ColumnsSet;
	IM_ASSERT(columns != NULL);

	if (column_index < 0)
		column_index = columns->Current;
	IM_ASSERT(column_index < columns->Columns.Size);

	const float t = columns->Columns[column_index].OffsetNorm;
	const float x_offset = ImLerp(columns->MinX, columns->MaxX, t);
	return x_offset;
}

static float GetColumnWidthEx(ImGuiColumnsSet* columns, int column_index, bool before_resize = false)
{
	if (column_index < 0)
		column_index = columns->Current;

	float offset_norm;
	if (before_resize)
		offset_norm = columns->Columns[column_index + 1].OffsetNormBeforeResize - columns->Columns[column_index].OffsetNormBeforeResize;
	else
		offset_norm = columns->Columns[column_index + 1].OffsetNorm - columns->Columns[column_index].OffsetNorm;
	return OffsetNormToPixels(columns, offset_norm);
}

float ImGui::GetColumnWidth(int column_index)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	ImGuiColumnsSet* columns = window->DC.ColumnsSet;
	IM_ASSERT(columns != NULL);

	if (column_index < 0)
		column_index = columns->Current;
	return OffsetNormToPixels(columns, columns->Columns[column_index + 1].OffsetNorm - columns->Columns[column_index].OffsetNorm);
}

void ImGui::SetColumnOffset(int column_index, float offset)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiColumnsSet* columns = window->DC.ColumnsSet;
	IM_ASSERT(columns != NULL);

	if (column_index < 0)
		column_index = columns->Current;
	IM_ASSERT(column_index < columns->Columns.Size);

	const bool preserve_width = !(columns->Flags & ImGuiColumnsFlags_NoPreserveWidths) && (column_index < columns->Count - 1);
	const float width = preserve_width ? GetColumnWidthEx(columns, column_index, columns->IsBeingResized) : 0.0f;

	if (!(columns->Flags & ImGuiColumnsFlags_NoForceWithinWindow))
		offset = ImMin(offset, columns->MaxX - g.Style.ColumnsMinSpacing * (columns->Count - column_index));
	columns->Columns[column_index].OffsetNorm = PixelsToOffsetNorm(columns, offset - columns->MinX);

	if (preserve_width)
		SetColumnOffset(column_index + 1, offset + ImMax(g.Style.ColumnsMinSpacing, width));
}

void ImGui::SetColumnWidth(int column_index, float width)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	ImGuiColumnsSet* columns = window->DC.ColumnsSet;
	IM_ASSERT(columns != NULL);

	if (column_index < 0)
		column_index = columns->Current;
	SetColumnOffset(column_index + 1, GetColumnOffset(column_index) + width);
}

void ImGui::PushColumnClipRect(int column_index)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	ImGuiColumnsSet* columns = window->DC.ColumnsSet;
	if (column_index < 0)
		column_index = columns->Current;

	PushClipRect(columns->Columns[column_index].ClipRect.Min, columns->Columns[column_index].ClipRect.Max, false);
}

static ImGuiColumnsSet* FindOrAddColumnsSet(ImGuiWindow* window, ImGuiID id)
{
	for (int n = 0; n < window->ColumnsStorage.Size; n++)
		if (window->ColumnsStorage[n].ID == id)
			return &window->ColumnsStorage[n];

	window->ColumnsStorage.push_back(ImGuiColumnsSet());
	ImGuiColumnsSet* columns = &window->ColumnsStorage.back();
	columns->ID = id;
	return columns;
}

void ImGui::BeginColumns(const char* str_id, int columns_count, ImGuiColumnsFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();

	IM_ASSERT(columns_count > 1);
	IM_ASSERT(window->DC.ColumnsSet == NULL); // Nested columns are currently not supported

	// Differentiate column ID with an arbitrary prefix for cases where users name their columns set the same as another widget.
	// In addition, when an identifier isn't explicitly provided we include the number of columns in the hash to make it uniquer.
	PushID(0x11223347 + (str_id ? 0 : columns_count));
	ImGuiID id = window->GetID(str_id ? str_id : "columns");
	PopID();

	// Acquire storage for the columns set
	ImGuiColumnsSet* columns = FindOrAddColumnsSet(window, id);
	IM_ASSERT(columns->ID == id);
	columns->Current = 0;
	columns->Count = columns_count;
	columns->Flags = flags;
	window->DC.ColumnsSet = columns;

	// Set state for first column
	const float content_region_width = (window->SizeContentsExplicit.x != 0.0f) ? (window->SizeContentsExplicit.x) : (window->InnerClipRect.Max.x - window->Pos.x);
	columns->MinX = window->DC.IndentX - g.Style.ItemSpacing.x; // Lock our horizontal range
	columns->MaxX = ImMax(content_region_width - window->Scroll.x, columns->MinX + 1.0f);
	columns->StartPosY = window->DC.CursorPos.y;
	columns->StartMaxPosX = window->DC.CursorMaxPos.x;
	columns->LineMinY = columns->LineMaxY = window->DC.CursorPos.y;
	window->DC.ColumnsOffsetX = 0.0f;
	window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);

	// Clear data if columns count changed
	if (columns->Columns.Size != 0 && columns->Columns.Size != columns_count + 1)
		columns->Columns.resize(0);

	// Initialize defaults
	columns->IsFirstFrame = (columns->Columns.Size == 0);
	if (columns->Columns.Size == 0)
	{
		columns->Columns.reserve(columns_count + 1);
		for (int n = 0; n < columns_count + 1; n++)
		{
			ImGuiColumnData column;
			column.OffsetNorm = n / (float)columns_count;
			columns->Columns.push_back(column);
		}
	}

	for (int n = 0; n < columns_count; n++)
	{
		// Compute clipping rectangle
		ImGuiColumnData* column = &columns->Columns[n];
		float clip_x1 = ImFloor(0.5f + window->Pos.x + GetColumnOffset(n) - 1.0f);
		float clip_x2 = ImFloor(0.5f + window->Pos.x + GetColumnOffset(n + 1) - 1.0f);
		column->ClipRect = ImRect(clip_x1, -FLT_MAX, clip_x2, +FLT_MAX);
		column->ClipRect.ClipWith(window->ClipRect);
	}

	window->DrawList->ChannelsSplit(columns->Count);
	PushColumnClipRect();
	PushItemWidth(GetColumnWidth() * 0.65f);
}

void ImGui::EndColumns()
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	ImGuiColumnsSet* columns = window->DC.ColumnsSet;
	IM_ASSERT(columns != NULL);

	PopItemWidth();
	PopClipRect();
	window->DrawList->ChannelsMerge();

	columns->LineMaxY = ImMax(columns->LineMaxY, window->DC.CursorPos.y);
	window->DC.CursorPos.y = columns->LineMaxY;
	if (!(columns->Flags & ImGuiColumnsFlags_GrowParentContentsSize))
		window->DC.CursorMaxPos.x = ImMax(columns->StartMaxPosX, columns->MaxX);  // Restore cursor max pos, as columns don't grow parent

	// Draw columns borders and handle resize
	bool is_being_resized = false;
	if (!(columns->Flags & ImGuiColumnsFlags_NoBorder) && !window->SkipItems)
	{
		const float y1 = columns->StartPosY;
		const float y2 = window->DC.CursorPos.y;
		int dragging_column = -1;
		for (int n = 1; n < columns->Count; n++)
		{
			float x = window->Pos.x + GetColumnOffset(n);
			const ImGuiID column_id = columns->ID + ImGuiID(n);
			const float column_hw = GetColumnsRectHalfWidth(); // Half-width for interaction
			const ImRect column_rect(ImVec2(x - column_hw, y1), ImVec2(x + column_hw, y2));
			KeepAliveID(column_id);
			if (IsClippedEx(column_rect, column_id, false))
				continue;

			bool hovered = false, held = false;
			if (!(columns->Flags & ImGuiColumnsFlags_NoResize))
			{
				ButtonBehavior(column_rect, column_id, &hovered, &held);
				if (hovered || held)
					g.MouseCursor = ImGuiMouseCursor_ResizeEW;
				if (held && !(columns->Columns[n].Flags & ImGuiColumnsFlags_NoResize))
					dragging_column = n;
			}

			// Draw column (we clip the Y boundaries CPU side because very long triangles are mishandled by some GPU drivers.)
			const ImU32 col = GetColorU32(held ? ImGuiCol_SeparatorActive : hovered ? ImGuiCol_SeparatorHovered : ImGuiCol_Separator);
			const float xi = (float)(int)x;
			window->DrawList->AddLine(ImVec2(xi, ImMax(y1 + 1.0f, window->ClipRect.Min.y)), ImVec2(xi, ImMin(y2, window->ClipRect.Max.y)), col);
		}

		// Apply dragging after drawing the column lines, so our rendered lines are in sync with how items were displayed during the frame.
		if (dragging_column != -1)
		{
			if (!columns->IsBeingResized)
				for (int n = 0; n < columns->Count + 1; n++)
					columns->Columns[n].OffsetNormBeforeResize = columns->Columns[n].OffsetNorm;
			columns->IsBeingResized = is_being_resized = true;
			float x = GetDraggedColumnOffset(columns, dragging_column);
			SetColumnOffset(dragging_column, x);
		}
	}
	columns->IsBeingResized = is_being_resized;

	window->DC.ColumnsSet = NULL;
	window->DC.ColumnsOffsetX = 0.0f;
	window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);
}

// [2018-03: This is currently the only public API, while we are working on making BeginColumns/EndColumns user-facing]
void ImGui::Columns(int columns_count, const char* id, bool border)
{
	ImGuiWindow* window = GetCurrentWindow();
	IM_ASSERT(columns_count >= 1);
	if (window->DC.ColumnsSet != NULL && window->DC.ColumnsSet->Count != columns_count)
		EndColumns();

	ImGuiColumnsFlags flags = (border ? 0 : ImGuiColumnsFlags_NoBorder);
	//flags |= ImGuiColumnsFlags_NoPreserveWidths; // NB: Legacy behavior
	if (columns_count != 1)
		BeginColumns(id, columns_count, flags);
}

void ImGui::Indent(float indent_w)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.IndentX += (indent_w != 0.0f) ? indent_w : g.Style.IndentSpacing;
	window->DC.CursorPos.x = window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX;
}

void ImGui::Unindent(float indent_w)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.IndentX -= (indent_w != 0.0f) ? indent_w : g.Style.IndentSpacing;
	window->DC.CursorPos.x = window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX;
}

void ImGui::TreePush(const char* str_id)
{
	ImGuiWindow* window = GetCurrentWindow();
	Indent();
	window->DC.TreeDepth++;
	PushID(str_id ? str_id : "#TreePush");
}

void ImGui::TreePush(const void* ptr_id)
{
	ImGuiWindow* window = GetCurrentWindow();
	Indent();
	window->DC.TreeDepth++;
	PushID(ptr_id ? ptr_id : (const void*)"#TreePush");
}

void ImGui::TreePushRawID(ImGuiID id)
{
	ImGuiWindow* window = GetCurrentWindow();
	Indent();
	window->DC.TreeDepth++;
	window->IDStack.push_back(id);
}

void ImGui::TreePop()
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	Unindent();

	window->DC.TreeDepth--;
	if (g.NavMoveDir == ImGuiDir_Left && g.NavWindow == window && NavMoveRequestButNoResultYet())
		if (g.NavIdIsAlive && (window->DC.TreeDepthMayJumpToParentOnPop & (1 << window->DC.TreeDepth)))
		{
			SetNavID(window->IDStack.back(), g.NavLayer);
			NavMoveRequestCancel();
		}
	window->DC.TreeDepthMayJumpToParentOnPop &= (1 << window->DC.TreeDepth) - 1;

	PopID();
}

void ImGui::Value(const char* prefix, bool b)
{
	Text("%s: %s", prefix, (b ? "true" : "false"));
}

void ImGui::Value(const char* prefix, int v)
{
	Text("%s: %d", prefix, v);
}

void ImGui::Value(const char* prefix, unsigned int v)
{
	Text("%s: %d", prefix, v);
}

void ImGui::Value(const char* prefix, float v, const char* float_format)
{
	if (float_format)
	{
		char fmt[64];
		ImFormatString(fmt, IM_ARRAYSIZE(fmt), "%%s: %s", float_format);
		Text(fmt, prefix, v);
	}
	else
	{
		Text("%s: %.3f", prefix, v);
	}
}

//-----------------------------------------------------------------------------
// DRAG AND DROP
//-----------------------------------------------------------------------------

void ImGui::ClearDragDrop()
{
	ImGuiContext& g = *GImGui;
	g.DragDropActive = false;
	g.DragDropPayload.Clear();
	g.DragDropAcceptIdCurr = g.DragDropAcceptIdPrev = 0;
	g.DragDropAcceptIdCurrRectSurface = FLT_MAX;
	g.DragDropAcceptFrameCount = -1;
}

// Call when current ID is active. 
// When this returns true you need to: a) call SetDragDropPayload() exactly once, b) you may render the payload visual/description, c) call EndDragDropSource()
bool ImGui::BeginDragDropSource(ImGuiDragDropFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	bool source_drag_active = false;
	ImGuiID source_id = 0;
	ImGuiID source_parent_id = 0;
	int mouse_button = 0;
	if (!(flags & ImGuiDragDropFlags_SourceExtern))
	{
		source_id = window->DC.LastItemId;
		if (source_id != 0 && g.ActiveId != source_id) // Early out for most common case
			return false;
		if (g.IO.MouseDown[mouse_button] == false)
			return false;

		if (source_id == 0)
		{
			// If you want to use BeginDragDropSource() on an item with no unique identifier for interaction, such as Text() or Image(), you need to:
			// A) Read the explanation below, B) Use the ImGuiDragDropFlags_SourceAllowNullID flag, C) Swallow your programmer pride.
			if (!(flags & ImGuiDragDropFlags_SourceAllowNullID))
			{
				IM_ASSERT(0);
				return false;
			}

			// Magic fallback (=somehow reprehensible) to handle items with no assigned ID, e.g. Text(), Image()
			// We build a throwaway ID based on current ID stack + relative AABB of items in window. 
			// THE IDENTIFIER WON'T SURVIVE ANY REPOSITIONING OF THE WIDGET, so if your widget moves your dragging operation will be canceled. 
			// We don't need to maintain/call ClearActiveID() as releasing the button will early out this function and trigger !ActiveIdIsAlive.
			bool is_hovered = (window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HoveredRect) != 0;
			if (!is_hovered && (g.ActiveId == 0 || g.ActiveIdWindow != window))
				return false;
			source_id = window->DC.LastItemId = window->GetIDFromRectangle(window->DC.LastItemRect);
			if (is_hovered)
				SetHoveredID(source_id);
			if (is_hovered && g.IO.MouseClicked[mouse_button])
			{
				SetActiveID(source_id, window);
				FocusWindow(window);
			}
			if (g.ActiveId == source_id) // Allow the underlying widget to display/return hovered during the mouse release frame, else we would get a flicker.
				g.ActiveIdAllowOverlap = is_hovered;
		}
		if (g.ActiveId != source_id)
			return false;
		source_parent_id = window->IDStack.back();
		source_drag_active = IsMouseDragging(mouse_button);
	}
	else
	{
		window = NULL;
		source_id = ImHash("#SourceExtern", 0);
		source_drag_active = true;
	}

	if (source_drag_active)
	{
		if (!g.DragDropActive)
		{
			IM_ASSERT(source_id != 0);
			ClearDragDrop();
			ImGuiPayload& payload = g.DragDropPayload;
			payload.SourceId = source_id;
			payload.SourceParentId = source_parent_id;
			g.DragDropActive = true;
			g.DragDropSourceFlags = flags;
			g.DragDropMouseButton = mouse_button;
		}

		if (!(flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
		{
			// FIXME-DRAG
			//SetNextWindowPos(g.IO.MousePos - g.ActiveIdClickOffset - g.Style.WindowPadding);
			//PushStyleVar(ImGuiStyleVar_Alpha, g.Style.Alpha * 0.60f); // This is better but e.g ColorButton with checkboard has issue with transparent colors :(
			SetNextWindowPos(g.IO.MousePos);
			PushStyleColor(ImGuiCol_PopupBg, GetStyleColorVec4(ImGuiCol_PopupBg) * ImVec4(1.0f, 1.0f, 1.0f, 0.6f));
			BeginTooltip();
		}

		if (!(flags & ImGuiDragDropFlags_SourceNoDisableHover) && !(flags & ImGuiDragDropFlags_SourceExtern))
			window->DC.LastItemStatusFlags &= ~ImGuiItemStatusFlags_HoveredRect;

		return true;
	}
	return false;
}

void ImGui::EndDragDropSource()
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(g.DragDropActive);

	if (!(g.DragDropSourceFlags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
	{
		EndTooltip();
		PopStyleColor();
		//PopStyleVar();
	}

	// Discard the drag if have not called SetDragDropPayload()
	if (g.DragDropPayload.DataFrameCount == -1)
		ClearDragDrop();
}

// Use 'cond' to choose to submit payload on drag start or every frame
bool ImGui::SetDragDropPayload(const char* type, const void* data, size_t data_size, ImGuiCond cond)
{
	ImGuiContext& g = *GImGui;
	ImGuiPayload& payload = g.DragDropPayload;
	if (cond == 0)
		cond = ImGuiCond_Always;

	IM_ASSERT(type != NULL);
	IM_ASSERT(strlen(type) < IM_ARRAYSIZE(payload.DataType) && "Payload type can be at most 12 characters long");
	IM_ASSERT((data != NULL && data_size > 0) || (data == NULL && data_size == 0));
	IM_ASSERT(cond == ImGuiCond_Always || cond == ImGuiCond_Once);
	IM_ASSERT(payload.SourceId != 0);                               // Not called between BeginDragDropSource() and EndDragDropSource()

	if (cond == ImGuiCond_Always || payload.DataFrameCount == -1)
	{
		// Copy payload
		ImStrncpy(payload.DataType, type, IM_ARRAYSIZE(payload.DataType));
		g.DragDropPayloadBufHeap.resize(0);
		if (data_size > sizeof(g.DragDropPayloadBufLocal))
		{
			// Store in heap
			g.DragDropPayloadBufHeap.resize((int)data_size);
			payload.Data = g.DragDropPayloadBufHeap.Data;
			memcpy((void*)payload.Data, data, data_size);
		}
		else if (data_size > 0)
		{
			// Store locally
			memset(&g.DragDropPayloadBufLocal, 0, sizeof(g.DragDropPayloadBufLocal));
			payload.Data = g.DragDropPayloadBufLocal;
			memcpy((void*)payload.Data, data, data_size);
		}
		else
		{
			payload.Data = NULL;
		}
		payload.DataSize = (int)data_size;
	}
	payload.DataFrameCount = g.FrameCount;

	return (g.DragDropAcceptFrameCount == g.FrameCount) || (g.DragDropAcceptFrameCount == g.FrameCount - 1);
}

bool ImGui::BeginDragDropTargetCustom(const ImRect& bb, ImGuiID id)
{
	ImGuiContext& g = *GImGui;
	if (!g.DragDropActive)
		return false;

	ImGuiWindow* window = g.CurrentWindow;
	if (g.HoveredWindow == NULL || window->RootWindow != g.HoveredWindow->RootWindow)
		return false;
	IM_ASSERT(id != 0);
	if (!IsMouseHoveringRect(bb.Min, bb.Max) || (id == g.DragDropPayload.SourceId))
		return false;

	g.DragDropTargetRect = bb;
	g.DragDropTargetId = id;
	return true;
}

// We don't use BeginDragDropTargetCustom() and duplicate its code because:
// 1) we use LastItemRectHoveredRect which handles items that pushes a temporarily clip rectangle in their code. Calling BeginDragDropTargetCustom(LastItemRect) would not handle them.
// 2) and it's faster. as this code may be very frequently called, we want to early out as fast as we can.
// Also note how the HoveredWindow test is positioned differently in both functions (in both functions we optimize for the cheapest early out case)
bool ImGui::BeginDragDropTarget()
{
	ImGuiContext& g = *GImGui;
	if (!g.DragDropActive)
		return false;

	ImGuiWindow* window = g.CurrentWindow;
	if (!(window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HoveredRect))
		return false;
	if (g.HoveredWindow == NULL || window->RootWindow != g.HoveredWindow->RootWindow)
		return false;

	const ImRect& display_rect = (window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HasDisplayRect) ? window->DC.LastItemDisplayRect : window->DC.LastItemRect;
	ImGuiID id = window->DC.LastItemId;
	if (id == 0)
		id = window->GetIDFromRectangle(display_rect);
	if (g.DragDropPayload.SourceId == id)
		return false;

	g.DragDropTargetRect = display_rect;
	g.DragDropTargetId = id;
	return true;
}

bool ImGui::IsDragDropPayloadBeingAccepted()
{
	ImGuiContext& g = *GImGui;
	return g.DragDropActive && g.DragDropAcceptIdPrev != 0;
}

const ImGuiPayload* ImGui::AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiPayload& payload = g.DragDropPayload;
	IM_ASSERT(g.DragDropActive);                        // Not called between BeginDragDropTarget() and EndDragDropTarget() ?
	IM_ASSERT(payload.DataFrameCount != -1);            // Forgot to call EndDragDropTarget() ? 
	if (type != NULL && !payload.IsDataType(type))
		return NULL;

	// Accept smallest drag target bounding box, this allows us to nest drag targets conveniently without ordering constraints.
	// NB: We currently accept NULL id as target. However, overlapping targets requires a unique ID to function!
	const bool was_accepted_previously = (g.DragDropAcceptIdPrev == g.DragDropTargetId);
	ImRect r = g.DragDropTargetRect;
	float r_surface = r.GetWidth() * r.GetHeight();
	if (r_surface < g.DragDropAcceptIdCurrRectSurface)
	{
		g.DragDropAcceptIdCurr = g.DragDropTargetId;
		g.DragDropAcceptIdCurrRectSurface = r_surface;
	}

	// Render default drop visuals
	payload.Preview = was_accepted_previously;
	flags |= (g.DragDropSourceFlags & ImGuiDragDropFlags_AcceptNoDrawDefaultRect); // Source can also inhibit the preview (useful for external sources that lives for 1 frame)
	if (!(flags & ImGuiDragDropFlags_AcceptNoDrawDefaultRect) && payload.Preview)
	{
		// FIXME-DRAG: Settle on a proper default visuals for drop target.
		r.Expand(3.5f);
		bool push_clip_rect = !window->ClipRect.Contains(r);
		if (push_clip_rect) window->DrawList->PushClipRectFullScreen();
		window->DrawList->AddRect(r.Min, r.Max, GetColorU32(ImGuiCol_DragDropTarget), 0.0f, ~0, 2.0f);
		if (push_clip_rect) window->DrawList->PopClipRect();
	}

	g.DragDropAcceptFrameCount = g.FrameCount;
	payload.Delivery = was_accepted_previously && !IsMouseDown(g.DragDropMouseButton); // For extern drag sources affecting os window focus, it's easier to just test !IsMouseDown() instead of IsMouseReleased()
	if (!payload.Delivery && !(flags & ImGuiDragDropFlags_AcceptBeforeDelivery))
		return NULL;

	return &payload;
}

// We don't really use/need this now, but added it for the sake of consistency and because we might need it later.
void ImGui::EndDragDropTarget()
{
	ImGuiContext& g = *GImGui; (void)g;
	IM_ASSERT(g.DragDropActive);
}

//-----------------------------------------------------------------------------
// PLATFORM DEPENDENT HELPERS
//-----------------------------------------------------------------------------

#if defined(_WIN32) && !defined(_WINDOWS_) && (!defined(IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS) || !defined(IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS))
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#ifndef __MINGW32__
#include <Windows.h>
#else
#include <windows.h>
#endif
#endif

// Win32 API clipboard implementation
#if defined(_WIN32) && !defined(IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS)

#ifdef _MSC_VER
#pragma comment(lib, "user32")
#endif

static const char* GetClipboardTextFn_DefaultImpl(void*)
{
	static ImVector<char> buf_local;
	buf_local.clear();
	if (!OpenClipboard(NULL))
		return NULL;
	HANDLE wbuf_handle = GetClipboardData(CF_UNICODETEXT);
	if (wbuf_handle == NULL)
	{
		CloseClipboard();
		return NULL;
	}
	if (ImWchar* wbuf_global = (ImWchar*)GlobalLock(wbuf_handle))
	{
		int buf_len = ImTextCountUtf8BytesFromStr(wbuf_global, NULL) + 1;
		buf_local.resize(buf_len);
		ImTextStrToUtf8(buf_local.Data, buf_len, wbuf_global, NULL);
	}
	GlobalUnlock(wbuf_handle);
	CloseClipboard();
	return buf_local.Data;
}

static void SetClipboardTextFn_DefaultImpl(void*, const char* text)
{
	if (!OpenClipboard(NULL))
		return;
	const int wbuf_length = ImTextCountCharsFromUtf8(text, NULL) + 1;
	HGLOBAL wbuf_handle = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)wbuf_length * sizeof(ImWchar));
	if (wbuf_handle == NULL)
	{
		CloseClipboard();
		return;
	}
	ImWchar* wbuf_global = (ImWchar*)GlobalLock(wbuf_handle);
	ImTextStrFromUtf8(wbuf_global, wbuf_length, text, NULL);
	GlobalUnlock(wbuf_handle);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, wbuf_handle);
	CloseClipboard();
}

#else

// Local ImGui-only clipboard implementation, if user hasn't defined better clipboard handlers
static const char* GetClipboardTextFn_DefaultImpl(void*)
{
	ImGuiContext& g = *GImGui;
	return g.PrivateClipboard.empty() ? NULL : g.PrivateClipboard.begin();
}

// Local ImGui-only clipboard implementation, if user hasn't defined better clipboard handlers
static void SetClipboardTextFn_DefaultImpl(void*, const char* text)
{
	ImGuiContext& g = *GImGui;
	g.PrivateClipboard.clear();
	const char* text_end = text + strlen(text);
	g.PrivateClipboard.resize((int)(text_end - text) + 1);
	memcpy(&g.PrivateClipboard[0], text, (size_t)(text_end - text));
	g.PrivateClipboard[(int)(text_end - text)] = 0;
}

#endif

// Win32 API IME support (for Asian languages, etc.)
#if defined(_WIN32) && !defined(__GNUC__) && !defined(IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS)

#include <imm.h>
#ifdef _MSC_VER
#pragma comment(lib, "imm32")
#endif

static void ImeSetInputScreenPosFn_DefaultImpl(int x, int y)
{
	// Notify OS Input Method Editor of text input position
	if (HWND hwnd = (HWND)GImGui->IO.ImeWindowHandle)
		if (HIMC himc = ImmGetContext(hwnd))
		{
			COMPOSITIONFORM cf;
			cf.ptCurrentPos.x = x;
			cf.ptCurrentPos.y = y;
			cf.dwStyle = CFS_FORCE_POSITION;
			ImmSetCompositionWindow(himc, &cf);
		}
}

#else

static void ImeSetInputScreenPosFn_DefaultImpl(int, int) {}

#endif

//-----------------------------------------------------------------------------
// HELP
//-----------------------------------------------------------------------------

void ImGui::ShowMetricsWindow(bool* p_open)
{
	if (ImGui::Begin("ImGui Metrics", p_open))
	{
		ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("%d vertices, %d indices (%d triangles)", ImGui::GetIO().MetricsRenderVertices, ImGui::GetIO().MetricsRenderIndices, ImGui::GetIO().MetricsRenderIndices / 3);
		ImGui::Text("%d allocations", (int)GImAllocatorActiveAllocationsCount);
		static bool show_clip_rects = true;
		ImGui::Checkbox("Show clipping rectangles when hovering draw commands", &show_clip_rects);
		ImGui::Separator();

		struct Funcs
		{
			static void NodeDrawList(ImGuiWindow* window, ImDrawList* draw_list, const char* label)
			{
				bool node_open = ImGui::TreeNode(draw_list, "%s: '%s' %d vtx, %d indices, %d cmds", label, draw_list->_OwnerName ? draw_list->_OwnerName : "", draw_list->VtxBuffer.Size, draw_list->IdxBuffer.Size, draw_list->CmdBuffer.Size);
				if (draw_list == ImGui::GetWindowDrawList())
				{
					ImGui::SameLine();
					ImGui::TextColored(ImColor(255, 100, 100), "CURRENTLY APPENDING"); // Can't display stats for active draw list! (we don't have the data double-buffered)
					if (node_open) ImGui::TreePop();
					return;
				}

				ImDrawList* overlay_draw_list = ImGui::GetOverlayDrawList(); // Render additional visuals into the top-most draw list
				if (window && ImGui::IsItemHovered())
					overlay_draw_list->AddRect(window->Pos, window->Pos + window->Size, IM_COL32(255, 255, 0, 255));
				if (!node_open)
					return;

				int elem_offset = 0;
				for (const ImDrawCmd* pcmd = draw_list->CmdBuffer.begin(); pcmd < draw_list->CmdBuffer.end(); elem_offset += pcmd->ElemCount, pcmd++)
				{
					if (pcmd->UserCallback == NULL && pcmd->ElemCount == 0)
						continue;
					if (pcmd->UserCallback)
					{
						ImGui::BulletText("Callback %p, user_data %p", pcmd->UserCallback, pcmd->UserCallbackData);
						continue;
					}
					ImDrawIdx* idx_buffer = (draw_list->IdxBuffer.Size > 0) ? draw_list->IdxBuffer.Data : NULL;
					bool pcmd_node_open = ImGui::TreeNode((void*)(pcmd - draw_list->CmdBuffer.begin()), "Draw %4d %s vtx, tex 0x%p, clip_rect (%4.0f,%4.0f)-(%4.0f,%4.0f)", pcmd->ElemCount, draw_list->IdxBuffer.Size > 0 ? "indexed" : "non-indexed", pcmd->TextureId, pcmd->ClipRect.x, pcmd->ClipRect.y, pcmd->ClipRect.z, pcmd->ClipRect.w);
					if (show_clip_rects && ImGui::IsItemHovered())
					{
						ImRect clip_rect = pcmd->ClipRect;
						ImRect vtxs_rect;
						for (int i = elem_offset; i < elem_offset + (int)pcmd->ElemCount; i++)
							vtxs_rect.Add(draw_list->VtxBuffer[idx_buffer ? idx_buffer[i] : i].pos);
						clip_rect.Floor(); overlay_draw_list->AddRect(clip_rect.Min, clip_rect.Max, IM_COL32(255, 255, 0, 255));
						vtxs_rect.Floor(); overlay_draw_list->AddRect(vtxs_rect.Min, vtxs_rect.Max, IM_COL32(255, 0, 255, 255));
					}
					if (!pcmd_node_open)
						continue;

					// Display individual triangles/vertices. Hover on to get the corresponding triangle highlighted.
					ImGuiListClipper clipper(pcmd->ElemCount / 3); // Manually coarse clip our print out of individual vertices to save CPU, only items that may be visible.
					while (clipper.Step())
						for (int prim = clipper.DisplayStart, vtx_i = elem_offset + clipper.DisplayStart * 3; prim < clipper.DisplayEnd; prim++)
						{
							char buf[300];
							char *buf_p = buf, *buf_end = buf + IM_ARRAYSIZE(buf);
							ImVec2 triangles_pos[3];
							for (int n = 0; n < 3; n++, vtx_i++)
							{
								ImDrawVert& v = draw_list->VtxBuffer[idx_buffer ? idx_buffer[vtx_i] : vtx_i];
								triangles_pos[n] = v.pos;
								buf_p += ImFormatString(buf_p, (int)(buf_end - buf_p), "%s %04d: pos (%8.2f,%8.2f), uv (%.6f,%.6f), col %08X\n", (n == 0) ? "vtx" : "   ", vtx_i, v.pos.x, v.pos.y, v.uv.x, v.uv.y, v.col);
							}
							ImGui::Selectable(buf, false);
							if (ImGui::IsItemHovered())
							{
								ImDrawListFlags backup_flags = overlay_draw_list->Flags;
								overlay_draw_list->Flags &= ~ImDrawListFlags_AntiAliasedLines; // Disable AA on triangle outlines at is more readable for very large and thin triangles.
								overlay_draw_list->AddPolyline(triangles_pos, 3, IM_COL32(255, 255, 0, 255), true, 1.0f);
								overlay_draw_list->Flags = backup_flags;
							}
						}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			static void NodeWindows(ImVector<ImGuiWindow*>& windows, const char* label)
			{
				if (!ImGui::TreeNode(label, "%s (%d)", label, windows.Size))
					return;
				for (int i = 0; i < windows.Size; i++)
					Funcs::NodeWindow(windows[i], "Window");
				ImGui::TreePop();
			}

			static void NodeWindow(ImGuiWindow* window, const char* label)
			{
				if (!ImGui::TreeNode(window, "%s '%s', %d @ 0x%p", label, window->Name, window->Active || window->WasActive, window))
					return;
				ImGuiWindowFlags flags = window->Flags;
				NodeDrawList(window, window->DrawList, "DrawList");
				ImGui::BulletText("Pos: (%.1f,%.1f), Size: (%.1f,%.1f), SizeContents (%.1f,%.1f)", window->Pos.x, window->Pos.y, window->Size.x, window->Size.y, window->SizeContents.x, window->SizeContents.y);
				ImGui::BulletText("Flags: 0x%08X (%s%s%s%s%s%s..)", flags,
					(flags & ImGuiWindowFlags_ChildWindow) ? "Child " : "", (flags & ImGuiWindowFlags_Tooltip) ? "Tooltip " : "", (flags & ImGuiWindowFlags_Popup) ? "Popup " : "",
					(flags & ImGuiWindowFlags_Modal) ? "Modal " : "", (flags & ImGuiWindowFlags_ChildMenu) ? "ChildMenu " : "", (flags & ImGuiWindowFlags_NoSavedSettings) ? "NoSavedSettings " : "");
				ImGui::BulletText("Scroll: (%.2f/%.2f,%.2f/%.2f)", window->Scroll.x, GetScrollMaxX(window), window->Scroll.y, GetScrollMaxY(window));
				ImGui::BulletText("Active: %d, WriteAccessed: %d", window->Active, window->WriteAccessed);
				ImGui::BulletText("NavLastIds: 0x%08X,0x%08X, NavLayerActiveMask: %X", window->NavLastIds[0], window->NavLastIds[1], window->DC.NavLayerActiveMask);
				ImGui::BulletText("NavLastChildNavWindow: %s", window->NavLastChildNavWindow ? window->NavLastChildNavWindow->Name : "NULL");
				if (window->NavRectRel[0].IsInverted())
					ImGui::BulletText("NavRectRel[0]: (%.1f,%.1f)(%.1f,%.1f)", window->NavRectRel[0].Min.x, window->NavRectRel[0].Min.y, window->NavRectRel[0].Max.x, window->NavRectRel[0].Max.y);
				else
					ImGui::BulletText("NavRectRel[0]: <None>");
				if (window->RootWindow != window) NodeWindow(window->RootWindow, "RootWindow");
				if (window->DC.ChildWindows.Size > 0) NodeWindows(window->DC.ChildWindows, "ChildWindows");
				if (window->ColumnsStorage.Size > 0 && ImGui::TreeNode("Columns", "Columns sets (%d)", window->ColumnsStorage.Size))
				{
					for (int n = 0; n < window->ColumnsStorage.Size; n++)
					{
						const ImGuiColumnsSet* columns = &window->ColumnsStorage[n];
						if (ImGui::TreeNode((void*)(uintptr_t)columns->ID, "Columns Id: 0x%08X, Count: %d, Flags: 0x%04X", columns->ID, columns->Count, columns->Flags))
						{
							ImGui::BulletText("Width: %.1f (MinX: %.1f, MaxX: %.1f)", columns->MaxX - columns->MinX, columns->MinX, columns->MaxX);
							for (int column_n = 0; column_n < columns->Columns.Size; column_n++)
								ImGui::BulletText("Column %02d: OffsetNorm %.3f (= %.1f px)", column_n, columns->Columns[column_n].OffsetNorm, OffsetNormToPixels(columns, columns->Columns[column_n].OffsetNorm));
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
				ImGui::BulletText("Storage: %d bytes", window->StateStorage.Data.Size * (int)sizeof(ImGuiStorage::Pair));
				ImGui::TreePop();
			}
		};

		// Access private state, we are going to display the draw lists from last frame
		ImGuiContext& g = *GImGui;
		Funcs::NodeWindows(g.Windows, "Windows");
		if (ImGui::TreeNode("DrawList", "Active DrawLists (%d)", g.DrawDataBuilder.Layers[0].Size))
		{
			for (int i = 0; i < g.DrawDataBuilder.Layers[0].Size; i++)
				Funcs::NodeDrawList(NULL, g.DrawDataBuilder.Layers[0][i], "DrawList");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Popups", "Open Popups Stack (%d)", g.OpenPopupStack.Size))
		{
			for (int i = 0; i < g.OpenPopupStack.Size; i++)
			{
				ImGuiWindow* window = g.OpenPopupStack[i].Window;
				ImGui::BulletText("PopupID: %08x, Window: '%s'%s%s", g.OpenPopupStack[i].PopupId, window ? window->Name : "NULL", window && (window->Flags & ImGuiWindowFlags_ChildWindow) ? " ChildWindow" : "", window && (window->Flags & ImGuiWindowFlags_ChildMenu) ? " ChildMenu" : "");
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Internal state"))
		{
			const char* input_source_names[] = { "None", "Mouse", "Nav", "NavKeyboard", "NavGamepad" }; IM_ASSERT(IM_ARRAYSIZE(input_source_names) == ImGuiInputSource_COUNT);
			ImGui::Text("HoveredWindow: '%s'", g.HoveredWindow ? g.HoveredWindow->Name : "NULL");
			ImGui::Text("HoveredRootWindow: '%s'", g.HoveredRootWindow ? g.HoveredRootWindow->Name : "NULL");
			ImGui::Text("HoveredId: 0x%08X/0x%08X (%.2f sec)", g.HoveredId, g.HoveredIdPreviousFrame, g.HoveredIdTimer); // Data is "in-flight" so depending on when the Metrics window is called we may see current frame information or not
			ImGui::Text("ActiveId: 0x%08X/0x%08X (%.2f sec), ActiveIdSource: %s", g.ActiveId, g.ActiveIdPreviousFrame, g.ActiveIdTimer, input_source_names[g.ActiveIdSource]);
			ImGui::Text("ActiveIdWindow: '%s'", g.ActiveIdWindow ? g.ActiveIdWindow->Name : "NULL");
			ImGui::Text("MovingWindow: '%s'", g.MovingWindow ? g.MovingWindow->Name : "NULL");
			ImGui::Text("NavWindow: '%s'", g.NavWindow ? g.NavWindow->Name : "NULL");
			ImGui::Text("NavId: 0x%08X, NavLayer: %d", g.NavId, g.NavLayer);
			ImGui::Text("NavInputSource: %s", input_source_names[g.NavInputSource]);
			ImGui::Text("NavActive: %d, NavVisible: %d", g.IO.NavActive, g.IO.NavVisible);
			ImGui::Text("NavActivateId: 0x%08X, NavInputId: 0x%08X", g.NavActivateId, g.NavInputId);
			ImGui::Text("NavDisableHighlight: %d, NavDisableMouseHover: %d", g.NavDisableHighlight, g.NavDisableMouseHover);
			ImGui::Text("DragDrop: %d, SourceId = 0x%08X, Payload \"%s\" (%d bytes)", g.DragDropActive, g.DragDropPayload.SourceId, g.DragDropPayload.DataType, g.DragDropPayload.DataSize);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

//-----------------------------------------------------------------------------

// Include imgui_user.inl at the end of imgui.cpp to access private data/functions that aren't exposed.
// Prefer just including imgui_internal.h from your code rather than using this define. If a declaration is missing from imgui_internal.h add it or request it on the github.
#ifdef IMGUI_INCLUDE_IMGUI_USER_INL
#include "imgui_user.inl"
#endif

//-----------------------------------------------------------------------------
