#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <vector>
#include <map>
#include <functional>

using Callback = std::function<void()>;

class InputManager
{
public:
  void add(int key, Callback& callback) { this->callbacks[key].emplace_back(callback); }
  void keyPress(int key)
  {
    if (this->callbacks.find(key) == this->callbacks.end()) { return; }
    for (Callback& callback : this->callbacks[key]) { callback(); }
  }

private:
  std::map<int , std::vector<Callback>> callbacks;
};

#endif