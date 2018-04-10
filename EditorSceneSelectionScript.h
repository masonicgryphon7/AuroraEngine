
#include "Component.h"
#include "Physics.h"
#include "InputHandler.h"



class EditorSceneSelectionScript : public Component
{
public:
	EditorSceneSelectionScript();
	EditorSceneSelectionScript(Camera* editorCamera);

	~EditorSceneSelectionScript();

	void update();

private:
	Camera * editorCamera;
};
