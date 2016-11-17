#include "BrokerTrigSorter.h"

BrokerTrigSorter::BrokerTrigSorter():Tool(){}


bool BrokerTrigSorter::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  lasttimeslice=(long)time(NULL);
  return true;
}


bool BrokerTrigSorter::Execute(){

  long currenttime=(long)time(NULL);
  if(currenttime > lasttimeslice-20){
    
    for(;lasttimeslice<=currenttime+20;lasttimeslice++){
     
      m_data->TrigData[lasttimeslice].StartTime=lasttimeslice;
      m_data->TrigData[lasttimeslice].EndTime=lasttimeslice+2;
      
    }
  }
  
  for(int i=0; i<m_data->UnSortedTrigData.size(); i++){
    /*    
    for(std::map<int,TriggerBunch>::iterator mIter=m_data->TrigData.begin(); mIter!=m_data->TrigData.end();++mIter){
      
      if(m_data->UnSortedTrigData.at(i)->Time>= mIter->second.StartTime && m_data->UnSortedTrigData.at(i)->Time<= mIter->second.EndTime) mIter->second.Data.push_back(m_data->UnSortedTrigData.at(i));
      
    
    
      //      std::cout<<"ID = "<<mIter->first<<" : starttime = "<<mIter->second.StartTime<<" : endtime = "<<mIter->second.EndTime<<" : number of entries = "<<mIter->second.Data.size()<<std::endl;
    
    }
    */

    for(int time=m_data->UnSortedTrigData.at(i)->Time-2 ; time<=m_data->UnSortedTrigData.at(i)->Time;time++){

      m_data->TrigData[time].Data.push_back(m_data->UnSortedTrigData.at(i));
    }

  }

  m_data->UnSortedTrigData.clear();

  for(std::map<long,TriggerBunch>::iterator mIter=m_data->TrigData.begin(); mIter!=m_data->\
	TrigData.end();++mIter){
    std::cout<<"ID = "<<mIter->first<<" : starttime = "<<mIter->second.StartTime<<\
      " : endtime = "<<mIter->second.EndTime<<" : number of entries = "<<mIter->second.Data.size()\
	     <<std::endl;
  }
  std::cout<<"done"<<std::endl;
  return true;
}


bool BrokerTrigSorter::Finalise(){

  return true;
}
