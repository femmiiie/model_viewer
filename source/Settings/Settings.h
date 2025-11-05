#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>

class Settings
{
  struct BoolSetting
  {
    std::string name;
    bool active = false;
  };

  struct ValSetting
  {
    std::string name;
    float value = 0;
  };

  static inline BoolSetting ShowGrid = {"Show Grid"};

};

#endif