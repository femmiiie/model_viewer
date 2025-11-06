#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>
#include <vector>
#include <variant>

#include <fstream>
#include <sstream>


class Settings
{
  public:
  static void Save(const char* filepath);
  static void Load(const char* filepath);

  struct BoolSetting
  {
    const char* name;
    bool active;
  };

  struct ValSetting
  {
    std::string name;
    float value = 0;
  };

  using Setting = std::variant<BoolSetting*, ValSetting*>;

  struct SettingRef
  {
    std::string_view key;
    Setting setting; 
  };

  static inline BoolSetting ShowGrid = {"Show Grid", false};
  static inline BoolSetting ShowAxes = {"Show Axes", false};
  static inline BoolSetting ShowLightPoints = {"Display Lights as Points", false};

  static inline std::vector<SettingRef> Identifiers = {
    {"ShowGrid", &ShowGrid},
    {"ShowAxes", &ShowAxes},
    {"ShowLightPoints", &ShowLightPoints},
  };
};

#endif