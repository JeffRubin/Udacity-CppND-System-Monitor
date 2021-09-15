#include "config.h"
#include "format.h"
#include "ncurses_display.h"
#include "process.h"
#include "system.h"

#include <chrono>
#include <thread>

char user_input = ' ';

void GetUserInput()
{
  while (true) {
    user_input = getch();
    std::this_thread::sleep_for(std::chrono::milliseconds(USER_INPUT_DELAY));
  }
}

int main() {
  std::thread t(GetUserInput);

  System system;

  std::chrono::steady_clock::time_point last_update_time, current_time;
  last_update_time = std::chrono::steady_clock::now() - std::chrono::milliseconds(UPDATE_INTERVAL);

  while(true){
    current_time = std::chrono::steady_clock::now();
    int delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_update_time).count();
    if(delta_time > UPDATE_INTERVAL) {
      last_update_time = std::chrono::steady_clock::now();
      system.SetSortBy(user_input);
      system.Update();
      NCursesDisplay::Display(system);
    }
  }  
}
