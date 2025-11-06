#include "Menu.h"


void Menu::Init(Renderer* renderer)
{
  Menu::renderer = renderer;
}


nfdu8char_t* Menu::GetFile()
{
  nfdu8char_t *filepath;
  nfdu8filteritem_t filter[1] = { {"OBJ File", "obj"} };
  
  nfdopendialogu8args_t args = {0};
  args.filterList = filter;
  args.filterCount = 1;
  NFD_GetNativeWindowFromGLFWWindow(renderer->getWindow(), &args.parentWindow);
  
  nfdresult_t result = NFD_OpenDialogU8_With(&filepath, &args);

  if (result == NFD_ERROR) { return NULL; }
  return filepath;
}


void Menu::RenderNode(Object* node, ImGuiTreeNodeFlags node_flags)
{
	std::vector<Object*>& children = node->getChildren_M();
	if (children.empty()) { node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; }
  if (Menu::selected_node == node) { node_flags |= ImGuiTreeNodeFlags_Selected; }

	bool opened = ImGui::TreeNodeEx((void*)node, node_flags, node->getCName());
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) { Menu::selected_node = node; }

	if (opened && !(node_flags & ImGuiTreeNodeFlags_Leaf))
	{
		for (Object* child : children) { RenderNode(child, node_flags & ~ImGuiTreeNodeFlags_Selected); }
		ImGui::TreePop();
	}
}


void Menu::ObjectSelector()
{
  if (ImGui::Button("+")) { ImGui::OpenPopup("spawn_object"); }
    
  if (ImGui::BeginPopup("spawn_object"))
  {
    if (ImGui::Selectable("Object"))
    {
      nfdu8char_t* filepath = GetFile();
      if (filepath) { renderer->addMesh(filepath, selected_node); }
    }
  
    if (ImGui::Selectable("Light")) { renderer->addLight(selected_node); }
    ImGui::EndPopup();
  }

  if (ImGui::TreeNodeEx("Scene", Menu::base_flags | ImGuiTreeNodeFlags_DefaultOpen))
  {
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) { selected_node = nullptr; }
		for (Object* root : renderer->getRootObject()) { RenderNode(root, Menu::base_flags); }
		ImGui::TreePop();
	}
}


void Menu::MultiSelector(
  const char* selector_label, 
  std::vector<const char*> labels, 
  std::vector<float*> variables, 
  float size
)
{
    float line_height = ImGui::GetTextLineHeightWithSpacing();

    float avail_width = ImGui::GetContentRegionAvail().x;
    ImGui::BeginChild(selector_label, ImVec2(0, line_height * size), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

    std::string sep_label = std::string("separator_") + selector_label;
    ImGui::BeginChild(sep_label.c_str(), ImVec2(avail_width * 0.8, 30));
    ImGui::SeparatorText(selector_label);
    ImGui::EndChild();

    ImGui::Indent(avail_width * 0.05f);
    ImGui::PushItemWidth(avail_width * 0.45f);

    ImGui::BeginGroup();
    for (int i = 0; i < labels.size(); i++)
    {
      if (ImGui::DragScalar(labels[i], ImGuiDataType_Float, variables[i], 0.02f, NULL, NULL, "%.2f"))
      {
        Menu::selected_node->update();
        Menu::renderer->regenerateLightUBO();
      }
    }
    ImGui::EndGroup();

    ImGui::PopItemWidth();
    ImGui::Unindent(avail_width * 0.05f); 

    ImGui::EndChild();
}


void Menu::ObjectModifier()
{
  if (!Menu::selected_node) { return; }

  glm::vec3& position = selected_node->getPosition_M();

  Menu::MultiSelector(
    "Location",
    {"X", "Y", "Z"},
    {&position.x, &position.y, &position.z},
    7
  );

  if (auto light = dynamic_cast<LightObject*>(selected_node))
  {
    glm::vec4& color = light->getColor();

    float& power = light->getPower();
    float& diffuse = light->getDiffuse();
    float& specular = light->getSpecular();
    float& ambient = light->getAmbient();

    MultiSelector("Color", {}, {}, 3);
    ImGuiColorEditFlags flags = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview;
    if (ImGui::ColorPicker4(" ", glm::value_ptr(color), flags))
      {
        light->update();
        Menu::renderer->regenerateLightUBO();
      }

    Menu::MultiSelector(
      "Power",
      {" "},
      {&power},
      4.5
    );

    Menu::MultiSelector(
      "Lighting Properties",
      {"Diffuse", "Specular", "Ambient"},
      {&diffuse, &specular, &ambient},
      7
    );
  }

  else if (auto mesh = dynamic_cast<MeshObject*>(selected_node))
  {
    glm::vec3& rotation = mesh->getRotation_M();
    glm::vec3& scaling = mesh->getScaling_M();

    Menu::MultiSelector(
      "Rotation",
      {"X", "Y", "Z"},
      {&rotation.x, &rotation.y, &rotation.z},
      7
    );

    Menu::MultiSelector(
      "Scaling",
      {"X", "Y", "Z"},
      {&scaling.x, &scaling.y, &scaling.z},
      7
    );
  }
}