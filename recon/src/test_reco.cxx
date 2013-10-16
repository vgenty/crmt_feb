#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include "RecoModule.h"

int main()
{

  RecoModule *mm = new RecoModule();
  mm->getfiles("event_list.txt","pins.csv");
  mm->print_fibers_from_pins();

  return 0;
}

