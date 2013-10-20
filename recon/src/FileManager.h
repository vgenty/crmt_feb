#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "Fiber.h"
#include "math.h"
#include "TFile.h"
#include "TTree.h"

class FileManager {
  
private:
  
  TFile *fReconData;
  TTree *fTree;

public:
  FileManager();
  ~FileManager();
  
  void make_tree(std::string file_name, int n_events,
		 std::vector<double>& fAngles);
  
  
};

#endif
