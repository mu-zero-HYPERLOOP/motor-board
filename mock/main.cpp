#include "canzero/canzero.h"
#include "defaults.h"
#include "fsm/fsm.h"
#include <unistd.h>


int main() {
  canzero_init();
  can_defaults();
  fsm_init();

  fsm_next(); //exit init state!

  while(true) {
    canzero_can0_poll();
    canzero_can1_poll();

    fsm_next();

    canzero_update_continue(canzero_get_time());

    usleep(1);
 }


}
