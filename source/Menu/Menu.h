#include "../Object/Object.h"
#include "../Renderer/Renderer.h"

#include <nfd.h>
#include <nfd_glfw3.h>

#include <functional>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Menu
{
  public:
  static void Init(Renderer* renderer);
  static void RenderNode(Object* node, ImGuiTreeNodeFlags node_flags);
  static void ObjectSelector();
  static void ObjectModifier();
  static void MultiSelector(const char* selector_label, std::vector<const char*> labels, std::vector<float*> variables, float size);

  static nfdu8char_t* GetFile();

  private:
  static inline Renderer* renderer;

  static inline Object* selected_node = nullptr;
  static inline ImGuiTreeNodeFlags base_flags = 
    ImGuiTreeNodeFlags_OpenOnArrow       |
    ImGuiTreeNodeFlags_OpenOnDoubleClick |
    ImGuiTreeNodeFlags_SpanAvailWidth    |
    ImGuiTreeNodeFlags_DrawLinesFull;
};