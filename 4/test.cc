#include <time.h>
#include <iostream>


int main(){
  time_t tick_start = time(NULL);

  while (1) {
    if (time(NULL) - tick_start > 1){
      tick_start = time(NULL);
      std::cout << "Tock" << std::endl;
    }
  }

  return 1;
}
