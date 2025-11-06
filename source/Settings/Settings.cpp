#include "Settings.h"

void Settings::Save(const char* filepath)
{
  std::ofstream out(filepath);
  if (!out) return;

  for (SettingRef& entry : Identifiers)
  {
    std::visit([&](auto* setting)
    {
      using SettingType = std::decay_t<decltype(*setting)>;
      if constexpr (std::is_same_v<SettingType, BoolSetting>)
      {
        out << entry.key << '=' << (setting->active ? "true" : "false") << '\n';
      }
      else
      {
        out << entry.key << '=' << setting->value << '\n';
      }
    }, entry.setting);
  }
}

void Settings::Load(const char* filepath)
{
  std::ifstream in(filepath);
  if (!in) return;

  std::string line;
  while (std::getline(in, line))
  {
    std::istringstream ss(line);
    std::string key, value;
    if (!std::getline(ss, key, '=')) continue;
    if (!std::getline(ss, value)) continue;

    for (auto& entry : Identifiers)
    {
      if (key != entry.key) continue;

      std::visit([&](auto* setting)
      {
        using SettingType = std::decay_t<decltype(*setting)>;
        if constexpr (std::is_same_v<SettingType, BoolSetting>)
        {
          setting->active = (value == "true" || value == "1");
        }
        else
        {
          try { setting->value = std::stof(value); } catch (...) {}
        }
      }, entry.setting);

      break;
    }
  }
}