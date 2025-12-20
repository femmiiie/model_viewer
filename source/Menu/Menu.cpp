#include "Menu.h"

#include "../Settings/Settings.h"

void Menu::Init(Renderer* renderer)
{
	Menu::renderer = renderer;
}

nfdu8char_t* Menu::GetOBJFile()
{
	nfdu8char_t* filepath;
	nfdu8filteritem_t filter[1] = {
	    {"OBJ File", "obj"}
  };

	nfdopendialogu8args_t args = {0};
	args.filterList            = filter;
	args.filterCount           = 1;
	NFD_GetNativeWindowFromGLFWWindow(renderer->getWindow(), &args.parentWindow);

	nfdresult_t result = NFD_OpenDialogU8_With(&filepath, &args);

	if (result == NFD_ERROR || result == NFD_CANCEL)
	{
		return nullptr;
	}
	return filepath;
}

void Menu::RenderNode(Object* node, ImGuiTreeNodeFlags node_flags)
{
	std::vector<Object*>& children = node->getChildren_M();
	if (children.empty())
	{
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}
	if (Menu::selected_node == node)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool opened = ImGui::TreeNodeEx((void*)node, node_flags, node->getCName());
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		Menu::selected_node = node;
	}

	if (ImGui::BeginPopupContextItem(node->getName().c_str()))
	{
		ImGui::InputText(" ", node->getName().data(), 32);
		if (ImGui::MenuItem("Delete", "Ctrl + D"))
		{
			if (Menu::selected_node == node)
			{
				Menu::selected_node = nullptr;
			}
			renderer->removeNode(node);
		}

		ImGui::EndPopup();
	}

	if (opened && !(node_flags & ImGuiTreeNodeFlags_Leaf))
	{
		for (Object* child : children)
		{
			Menu::RenderNode(child, node_flags & ~ImGuiTreeNodeFlags_Selected);
		}
		ImGui::TreePop();
	}
}

void Menu::ObjectSelector()
{
	if (ImGui::Button("+"))
	{
		ImGui::OpenPopup("spawn_object");
	}

	if (ImGui::BeginPopup("spawn_object"))
	{
		if (ImGui::Selectable("Object"))
		{
			nfdu8char_t* filepath = Menu::GetOBJFile();
			if (filepath)
			{
				renderer->addMesh(filepath, selected_node);
			}
		}

		if (ImGui::Selectable("Light"))
		{
			renderer->addLight(selected_node);
		}
		if (ImGui::Selectable("Folder"))
		{
			renderer->addFolder(selected_node);
		}
		ImGui::EndPopup();
	}

	if (ImGui::TreeNodeEx("Scene", Menu::base_flags | ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			selected_node = nullptr;
		}
		for (Object* root : renderer->getRootObject())
		{
			RenderNode(root, Menu::base_flags);
		}
		ImGui::TreePop();
	}
}

void Menu::MultiSelector(const char* selector_label, std::vector<const char*> labels,
                         std::vector<float*> variables, float padding = 0)
{
	const ImGuiStyle& style   = ImGui::GetStyle();
	const size_t item_count   = std::min(labels.size(), variables.size());
	const float header_height = ImGui::CalcTextSize(selector_label).y + style.ItemSpacing.y * 2.0f;
	const float row_height    = ImGui::GetFrameHeight();
	const float rows_height =
	    item_count > 0 ? (row_height * item_count) + style.ItemSpacing.y * (item_count - 1) : 0.0f;
	const float extra_height = padding > 0.0f ? padding * row_height : 0.0f;

	float widget_height = header_height + rows_height + extra_height + style.WindowPadding.y * 2.0f;
	float avail_width   = ImGui::GetContentRegionAvail().x;

	ImGui::BeginChild(selector_label, ImVec2(avail_width, widget_height), false,
	                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::SeparatorText(selector_label);

	float indent = avail_width * 0.05f;
	ImGui::Indent(indent);
	ImGui::PushItemWidth(avail_width * 0.45f);

	for (size_t i = 0; i < item_count; ++i)
	{
		if (ImGui::DragScalar(labels[i], ImGuiDataType_Float, variables[i], 0.02f, nullptr, nullptr,
		                      "%.2f"))
		{
			Menu::selected_node->update();
			Menu::renderer->regenerateLightUBO();
		}
	}

	ImGui::PopItemWidth();
	ImGui::Unindent(indent);

	ImGui::EndChild();
}

void Menu::ObjectModifier()
{
	if (!Menu::selected_node)
	{
		return;
	}

	glm::vec3& position = selected_node->getPosition_M();

	Menu::MultiSelector("Location", {"X", "Y", "Z"}, {&position.x, &position.y, &position.z});

	if (auto light = dynamic_cast<LightObject*>(selected_node))
	{
		glm::vec4& color = light->getColor();

		float& power    = light->getPower();
		float& diffuse  = light->getDiffuse();
		float& specular = light->getSpecular();
		float& ambient  = light->getAmbient();

		Menu::MultiSelector("Color", {}, {});
		ImGuiColorEditFlags flags = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview;
		if (ImGui::ColorPicker4(" ", glm::value_ptr(color), flags))
		{
			light->update();
			Menu::renderer->regenerateLightUBO();
		}

		Menu::MultiSelector("Power", {" "}, {&power});

		Menu::MultiSelector("Lighting Properties", {"Diffuse", "Specular", "Ambient"},
		                    {&diffuse, &specular, &ambient});

		int& type            = light->getType();
		glm::vec3& direction = light->getDirection();
		float& cutoff        = light->getCutoff();

		const char* types[3] = {"Point", "Directional", "Spotlight"};

		if (ImGui::BeginCombo("Light Type", types[type]))
		{
			for (int i = 0; i < 3; i++)
			{
				bool is_selected = type == i;
				if (ImGui::Selectable(types[i], is_selected))
				{
					type = i;
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (type > 0)
		{
			Menu::MultiSelector("Light Direction", {"X", "Y", "Z"},
			                    {&direction.x, &direction.y, &direction.z});
		}

		if (type == 2)
		{
			Menu::MultiSelector("Spotlight Cutoff", {" "}, {&cutoff});
		}
	}

	else if (auto mesh = dynamic_cast<MeshObject*>(selected_node))
	{
		glm::vec3& rotation = mesh->getRotation_M();
		glm::vec3& scaling  = mesh->getScaling_M();

		Menu::MultiSelector("Rotation", {"X", "Y", "Z"}, {&rotation.x, &rotation.y, &rotation.z});

		Menu::MultiSelector("Scaling", {"X", "Y", "Z"}, {&scaling.x, &scaling.y, &scaling.z});
	}
}

void Menu::Settings()
{
	static ImVec4 color(0.0f, 0.0f, 0.2f, 0.0f);
	ImGuiColorEditFlags flags = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview;
	if (ImGui::ColorPicker4("BG Color", (float*)&color, flags))
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}

	ImGui::Checkbox(Settings::ShowGrid.name, &Settings::ShowGrid.active);
	if (Settings::ShowGrid.active)
	{
		GridObject& grid = Menu::renderer->getGridObject_M();
		if (ImGui::DragInt("Grid Size", &grid.getGridSize_M(), 1.0f, 1) ||
		    ImGui::DragInt("Grid Spacing", &grid.getGridSpacing_M(), 1.0f, 1))
		{
			grid.generateGrid(Settings::ShowAxes.active);
		}
	}

	ImGui::Checkbox(Settings::ShowAxes.name, &Settings::ShowAxes.active);
	ImGui::Checkbox(Settings::ShowLightPoints.name, &Settings::ShowLightPoints.active);

	Camera* camera = Menu::renderer->getCamera();

	glm::vec3 pos = camera->getPosCAR();
	float vec[3]  = {pos.x, pos.y, pos.z};
	if (ImGui::DragFloat3("Camera Position", vec, 0.1f))
	{
		ImGui::SetTooltip("For manually adjusting the camera position to set coordinates");
		Menu::renderer->getCamera()->setPosCAR({vec[0], vec[1], vec[2]});
	}

	Settings::ProjectionType& type = camera->getProjectionType_M();
	const char* types[] = {"Perspective", "Orthographic"};
	if (ImGui::BeginCombo("Light Type", types[type]))
	{
		for (int i = 0; i < 2; i++)
		{
			bool is_selected = type == i;
			if (ImGui::Selectable(types[i], is_selected))
			{
				type = static_cast<Settings::ProjectionType>(i);
				camera->deferUpdate();
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::DragFloat("Camera FOV", &camera->getFOV_M()) ||
	    ImGui::DragFloat("Aspect Ratio", &camera->getAspect_M()) ||
	    ImGui::DragFloat("Near Clip", &camera->getNearClip_M()) ||
	    ImGui::DragFloat("Far Clip", &camera->getFarClip_M()) )
	{
		camera->deferUpdate();
	}

}