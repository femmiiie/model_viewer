#include "Object/Object.h"
#include "Object/MeshObject.h"

#include "Renderer/Renderer.h"

#include <nfd.h>
#include <nfd_glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


void RenderNode(Object* node, ImGuiTreeNodeFlags node_flags, Object*& selected_node)
{
	std::vector<Object*>& children = node->getChildren_M();
	if (children.empty()) { node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; }
  if (selected_node == node) { node_flags |= ImGuiTreeNodeFlags_Selected; }

	bool opened = ImGui::TreeNodeEx((void*)node, node_flags, node->getCName());
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) { selected_node = node; }

	if (opened && !(node_flags & ImGuiTreeNodeFlags_Leaf))
	{
		for (Object* child : children) { RenderNode(child, node_flags, selected_node); }
		ImGui::TreePop();
	}
}


void ObjectSelector(Renderer& renderer)
{
	static Object* selected_node = nullptr;
  static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow |
                                         ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                         ImGuiTreeNodeFlags_SpanAvailWidth |
                                         ImGuiTreeNodeFlags_DrawLinesFull;


  if (ImGui::Button("+")) {
  	nfdu8char_t *outPath;
    nfdu8filteritem_t filter[1] = { {"OBJ File", "obj"} };
    nfdopendialogu8args_t args = {0};
    args.filterList = filter;
    args.filterCount = 1;
    NFD_GetNativeWindowFromGLFWWindow(renderer.getWindow(), &args.parentWindow);

    nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
    if (result == NFD_OKAY) { renderer.addMesh(outPath, selected_node); }
  }

  if (ImGui::TreeNodeEx("Scene", base_flags))
  {
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) { selected_node = nullptr; }
		for (MeshObject* root : renderer.getRootObject()) { RenderNode(root, base_flags, selected_node); }
		ImGui::TreePop();
	}

}