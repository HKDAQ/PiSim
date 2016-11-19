#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <map>
#include <string>
#include <vector>

#include "TTree.h"

#include "Store.h"
#include "Logging.h"
#include "FEEData.h"
#include "TriggerData.h"
#include "TriggerBunch.h"

#include <zmq.hpp>

class DataModel {


 public:
  
  DataModel();
  TTree* GetTTree(std::string name);
  void AddTTree(std::string name,TTree *tree);
  void DeleteTTree(std::string name);

  Store vars;
  Logging *Log;

  zmq::context_t* context;

  //RBU Data structures
  std::vector<FEEData*> Data;

  //BrokerData structures
  std::vector<TriggerData*> UnSortedTrigData;
  std::map<long, TriggerBunch> TrigData;

  long StartSlicetime;
  bool SNTrigger;
  
 private:


  
  std::map<std::string,TTree*> m_trees; 
  
  
  
};

long mtime(){
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return (long)(tp.tv_sec * 1000000 + tp.tv_usec );
}

#endif

