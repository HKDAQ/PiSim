#include "RBUNetworkSend.h"

RBUNetworkSend::RBUNetworkSend():Tool(){}


bool RBUNetworkSend::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  //setup and connect worker threads

  Isend = new zmq::socket_t(*(m_data->context), ZMQ_PAIR);
  Isend->bind("inproc://main2processor");

  subcat = new zmq::socket_t(*(m_data->context), ZMQ_PAIR);
  subcat->bind("inproc://subcatquit");


  args=new context_args(m_data->context);
  m_variables.Get("TriggerPubPort",args->triggerpubport);
  m_variables.Get("FullDataPort", args->fulldataport);
  m_variables.Get("BufferDeleteTimeOut", args->bufferdeletetimeout);
  m_variables.Get("BufferDeletePeriod", args->bufferdeleteperiod);
  m_variables.Get("SubCatScanPeriod", args->subcatscanperiod);
     
  pthread_create (&thread[0], NULL, RBUNetworkSend::ProcessorThread, args);
  pthread_create (&thread[1], NULL, RBUNetworkSend::TriggerThread, args);
  pthread_create (&thread[2], NULL, RBUNetworkSend::FullDataThread, args);
  pthread_create (&thread[3], NULL, RBUNetworkSend::SubCatThread, args);

  sleep(3);
  return true;
}


bool RBUNetworkSend::Execute(){
  long tt=mtime();
  //  std::cout<<"imrunning , data size is"<<m_data->Data.size()<<std::endl;

  //////////////////////split data send ///////////////////
  
    for(int i=0; i<m_data->Data.size(); i++){ // could send whole vector and leave splitting to processor thread 

    //throw data to worker thread
    
    std::stringstream Datapointer;
    Datapointer<<"D "<<m_data->Data.at(i);

    zmq::message_t message(Datapointer.str().length()+1);
    snprintf ((char *) message.data(), Datapointer.str().length()+1 , "%s" ,Datapointer.str().c_str()) ;
    Isend->send(message);

    //std::cout<<"sending data to processor"<<m_data->Data.at(i)->Time<<std::endl;    
    //throw data to worker thread
    m_data->Data.at(i)=0;
  }

  m_data->Data.clear();

  
  ////////////////////////////////////////////////////////////////

  ///////////////////bunch data send ///////////////////////////
  /*
  std::stringstream Datapointer;
  Datapointer<<"D "<<(m_data->Data);

  zmq::message_t message(Datapointer.str().length()+1);
  snprintf ((char *) message.data(), Datapointer.str().length()+1 , "%\
s" ,Datapointer.str().c_str()) ;
  Isend->send(message);

  //std::cout<<"sending data to processor"<<m_data->Data.at(i)->Time<<std::endl;
  //throw data to worker thread
  m_data->Data=0;
//////////////////////////////////////////////////////

*/
  std::cout<<"e2 "<<mtime()-tt<<std::endl;
  return true;
}


bool RBUNetworkSend::Finalise(){

  zmq::message_t message(5);
  snprintf ((char *) message.data(),5 , "%s" ,"Quit") ;
  Isend->send(message);

  zmq::message_t message2(5);
  snprintf ((char *) message2.data(),5 , "%s" ,"Quit") ;
  subcat->send(message2);

  std::cout<<"main send quit"<<std::endl;
  pthread_join(thread[0], NULL);
  std::cout<<"join 0"<<std::endl;
  pthread_join(thread[1], NULL);
  std::cout<<"join 1"<<std::endl;
  pthread_join(thread[2], NULL);
  std::cout<<"join 2"<<std::endl;
  pthread_join(thread[3], NULL);
  std::cout<<"join 3"<<std::endl;

  delete args;
  args=0;

  delete Isend;
  Isend=0;

  delete subcat;
  subcat=0;

  return true;
}

//worker thread sends time and id then buffers

void* RBUNetworkSend::ProcessorThread(void* arg){

  //std::cout<<"debug 1"<<std::endl;
  
  context_args* args= static_cast<context_args*>(arg);
  
  zmq::socket_t Ireceive (*(args->context), ZMQ_PAIR);
  Ireceive.connect("inproc://main2processor");
  
  zmq::socket_t trigsend(*(args->context), ZMQ_PAIR);
  trigsend.bind("inproc://processor2trigger");
  
  zmq::socket_t fulldatasend(*(args->context), ZMQ_PAIR);
  fulldatasend.bind("inproc://processor2fulldata");
  
  bool running=true;

  std::deque<TriggerData*> trigdatabuff;
  std::deque<zmq::message_t*> fulldatamessagebuff;

  zmq::pollitem_t items [] = {
    { Ireceive, 0, ZMQ_POLLIN, 0 },
    { trigsend, 0, ZMQ_POLLOUT, 0 },
    { fulldatasend, 0, ZMQ_POLLOUT, 0},
  };

  //std::cout<<"debug 2"<<std::endl;
  
  while (running){

    zmq::poll(&items[0], 3, -1);

    //std::cout<<"debug 3"<<std::endl;
    if(items[0].revents & ZMQ_POLLIN){
      zmq::message_t* comm= new zmq::message_t();
      Ireceive.recv(comm);
      
      std::istringstream iss(static_cast<char*>(comm->data()));
      std::string arg1="";
      long long unsigned int arg2;
      
      iss>>arg1;
      
      if (arg1=="D"){
	
	iss>>std::hex>>arg2;
	
	//std::cout<<"debug 4"<<std::endl;
	////do processing if still neeed
	
	FEEData *data;
	data=reinterpret_cast<FEEData *>(arg2);
	
	///////////////////////////////
	//for(int i=0; i<data.size();i++){
	  
	  TriggerData* trig=new TriggerData();
	  trig->Time=data->Time;
	  trig->CardID=data->CardID;
	  trig->ChannelID=data->ChannelID;
	  
	  trigdatabuff.push_back(trig);
	  //}
	//////////////////////////////
	
	//std::stringstream tmp;
	//tmp<< "D "<<data->Time<<" "<<data->CardID<<" "<<data->ChannelID;
	//zmq::message_t message(tmp.str().length()+1);
	//snprintf ((char *) message.data(), tmp.str().length()+1 , "%s" ,tmp.str().c_str()) ;
	//std::cout<<"debug 4.0"<<std::endl;
	//trigsend.send(message);
	//std::cout<<"debug 4.1"<<std::endl;
	//fulldatamessagebuff.push_back(comm);
	  fulldatasend.send(*comm);
	
      }
      
      else if(arg1=="Quit")running=false;
      
    }
  
    if((items[1].revents & ZMQ_POLLOUT) && running && trigdatabuff.size()>0){
      
      std::stringstream trigpointer;
      trigpointer<<"D "<<trigdatabuff.at(0);
      
      zmq::message_t message(trigpointer.str().length()+1);
      snprintf((char *) message.data(), trigpointer.str().length()+1 ,"%s" ,trigpointer.str().c_str()) ;
      trigsend.send(message);
      
      //remove element 0
      trigdatabuff.pop_front();
    }
    
    if((items[2].revents & ZMQ_POLLOUT) && running && fulldatamessagebuff.size()>0){
      
      fulldatasend.send(*(fulldatamessagebuff.at(0)));
      
      //remove element 0
      fulldatamessagebuff.pop_front();
    }
    
  }
  
  for (int i=0; i<trigdatabuff.size(); i++){
    delete trigdatabuff.at(i);
    trigdatabuff.at(i)=0;
  }
  trigdatabuff.clear();

  for (int i=0; i<fulldatamessagebuff.size(); i++){
    delete fulldatamessagebuff.at(i);
    fulldatamessagebuff.at(i)=0;
  }
  fulldatamessagebuff.clear();
  

  //std::cout<<"debug 5"<<std::endl; 
  
  zmq::message_t message(5);
  snprintf ((char *) message.data(),5 , "%s" ,"Quit");
  trigsend.send(message);
  snprintf ((char *) message.data(),5 , "%s" ,"Quit");
  fulldatasend.send(message);
  
  //std::cout<<"debug 6"<<std::endl;
  
  pthread_exit(NULL);
  
}


void* RBUNetworkSend::TriggerThread(void* arg){
  
  context_args* args= static_cast<context_args*>(arg);
  
  zmq::socket_t Ireceive (*(args->context), ZMQ_PAIR);
  Ireceive.connect("inproc://processor2trigger");
  
  // zmq::socket_t indexsend(*(args->context), ZMQ_PAIR);
  //indexsend.bind("inproc://trigger2fulldata");

  zmq::socket_t trigsend(*(args->context), ZMQ_PUB);
  std::stringstream tmp;
  tmp<<"tcp://*:"<<args->triggerpubport;
  trigsend.bind(tmp.str().c_str());
  
  bool running=true;

  while(running){
      
      zmq::message_t comm;
      Ireceive.recv(&comm);
      
      std::istringstream iss(static_cast<char*>(comm.data()));
      std::string arg1="";
      long long unsigned int arg2;
      
      iss>>arg1;
      
    if (arg1=="D"){
      
      iss>>std::hex>>arg2;

      //std::cout<<"debug 4"<<std::endl;
      ////do processing if still neeed

      TriggerData *trig;
      trig=reinterpret_cast<TriggerData*>(arg2);
      
      trig->Send(trigsend);
      //iss>>data.Time>>data.CardID>>data.ChannelID;  
      //std::cout<<"sending data "<<trig->Time<<std::endl;
      //    trigsend.send(comm);
      //std::cout<<data.Time<<std::endl;
      //send it
      
    }
    
    else if(arg1=="Quit")running=false;
    
  }
  
  pthread_exit(NULL);
  
}

void* RBUNetworkSend::FullDataThread(void* arg){
  
  context_args* args= static_cast<context_args*>(arg);
  
  zmq::socket_t Ireceive (*(args->context), ZMQ_PAIR);
  Ireceive.connect("inproc://processor2fulldata");
  
  zmq::socket_t datadrop (*(args->context), ZMQ_SUB);
  datadrop.connect("inproc://subcat");

  zmq::socket_t Irep (*(args->context), ZMQ_REP);
  std::stringstream tmp;
  tmp<<"tcp://*:"<<args->fulldataport;
  Irep.bind("tcp://*:66666");

  std::map<long, FEEData*> waveforms;
  long lastpreen=0;
  
  zmq::pollitem_t items [] = {
    { Ireceive, 0, ZMQ_POLLIN, 0 },
    { Irep, 0, ZMQ_POLLIN, 0 },
    { datadrop, 0, ZMQ_POLLIN, 0 },
  };

  bool running=true;  

  while(running){

    zmq::poll(&items[0], 3, args->bufferdeleteperiod); //currenly buffer clean run every time data requested

    if (items [0].revents & ZMQ_POLLIN) {
      
      zmq::message_t comm;
      Ireceive.recv(&comm);
      
      std::istringstream iss(static_cast<char*>(comm.data()));
      std::string arg1="";
      long long unsigned int arg2;
      
      iss>>arg1;
      
      if (arg1=="D"){
	iss>>std::hex>>arg2;
	
	////do processing if still neeed
	
	FEEData *data;
	data=reinterpret_cast<FEEData *>(arg2);
	waveforms[data->Time]=data;
	//std::cout<<"data* = "<<data<<" : waveforms.at(i) = "<<waveforms.at(waveforms.size()-1)<<std::endl;
	//std::cout<<"waveforms.size() = "<<waveforms.size()<<std::endl;
	// delete data;
	//      delete waveforms.at(waveforms.size()-1);
      }
      
      else if(arg1=="Quit"){
	
	running=false;
	
	//std::cout<<"waveforms size = "<<waveforms.size()<<std::endl;
	for(std::map<long, FEEData*>::iterator mIter=waveforms.begin(); mIter!=waveforms.end(); ++mIter){
	  
	  delete (mIter->second);
	  //std::cout<<mIter->second->Time<<std::endl;
	  mIter->second=0;
	} 
	
	waveforms.clear();
	
	//std::cout<<"waveforms.size() = "<<waveforms.size()<<std::endl;
      }    
    }

    if ((items [2].revents & ZMQ_POLLIN) && running) {

      //sending data to EBU

      zmq::message_t comm;
      Irep.recv(&comm);

      std::istringstream iss(static_cast<char*>(comm.data()));
      long StartTime=0;
      long EndTime=0;
      iss>>StartTime>>EndTime;
      
      int count=0;
      long lasttime=0;

      for(long time=StartTime;time<=EndTime;time++){
	
	if(waveforms.find(time)!=waveforms.end()){
	  count++;
	  lasttime=time;
	}
	
      }
      
      zmq::message_t sizem(sizeof count);
      
      snprintf ((char *) sizem.data(), sizeof count , "%d" ,count) ;
      if(count!=0) Irep.send(sizem, ZMQ_SNDMORE); //find out what sendmore var is
      else Irep.send(sizem);
      
      for(long time=StartTime;time<=EndTime;time++){
	
        if(waveforms.find(time)!=waveforms.end()){
	  
	  if(time!=lasttime) waveforms[time]->Send(Irep, true);
	  else waveforms[time]->Send(Irep);
	  
	}
	
      }
      
      
    }
    
    if ((items [3].revents & ZMQ_POLLIN) && running) {
      
      //drop data thats no longer needed

      zmq::message_t comm;
      Ireceive.recv(&comm);

      std::istringstream iss(static_cast<char*>(comm.data()));
      long StartTime=0;
      long EndTime=0;
      iss>>StartTime>>EndTime;

      for(long time=StartTime;time<=EndTime;time++){
        
	std::map<long, FEEData*>::iterator it;
        it=waveforms.find(time);
	if(it!=waveforms.end())	waveforms.erase(it);
	
      }

    }
   
     
    // fail safe if havent got drop message to delete old memoryif greate than 50s old
    if(lastpreen<time(NULL)-2){
      lastpreen=(long)time(NULL);
      printf("before waveforms size = %i\n",(int)waveforms.size());
      long currenttime=mtime();
      std::vector<long> deletelist;
      for(std::map<long, FEEData*>::iterator mIter=waveforms.begin();mIter!=waveforms.end();++mIter){
	if (mIter->first<currenttime-args->bufferdeletetimeout) deletelist.push_back(mIter->first);
	//      std::cout<<mIter->first<<std::endl;
      }
      
      for(int i=0;i<deletelist.size();i++){
	//      printf("debug 1\n");
	delete waveforms[deletelist.at(i)];
	// printf("debug 2\n");
	waveforms.erase(deletelist.at(i));
	// printf("debug 3\n");
      }  
      // printf("debug 4\n");
      deletelist.clear();
      // printf("after waveforms size = %i\n",(int)waveforms.size());
    }


  }
  
  pthread_exit(NULL);
}


void* RBUNetworkSend::SubCatThread(void* arg){

  context_args* args= static_cast<context_args*>(arg);

  zmq::socket_t Ireceive (*(args->context), ZMQ_PAIR);
  Ireceive.connect("inproc://subcatquit");

  zmq::socket_t xsubcat (*(args->context), ZMQ_XPUB);
  xsubcat.connect("inproc://subcat");

  zmq::socket_t findservices (*(args->context), ZMQ_DEALER);
  findservices.connect("inproc://ServiceDiscovery");

  std::vector<zmq::socket_t*> xsubs; 
  //zmq::socket_t xubcat (*(args->context), ZMQ_XSUB);
    //xpubcat.connect("inproc://subcat");
  //  std::vector<double> v;
  //double* a = &v[0];
    //std::vector<zmq::pollitem_t> items;
  /*
typedef struct
{
  void //*socket//;
  int //fd//;
  short //events//;
  short //revents//;
} zmq_pollitem_t;
  */

  long lastscan;
  
  std::vector<zmq::pollitem_t> items;
  std::vector<std::string>xsubid;
  zmq::pollitem_t item0;
  item0.socket=Ireceive;
  item0.fd=0;
  item0.events=ZMQ_POLLIN;
  item0.revents=0;
  items.push_back(item0);
  
  //  zmq::pollitem_t items [] = {
  //  { Ireceive, 0, ZMQ_POLLIN, 0 },
  //};

  bool running= true;
  
  while (running){
    
    zmq::poll(&items[0], items.size(), args->subcatscanperiod);
    
    if (items[0].revents & ZMQ_POLLIN) running=false;
   
    //scan the pubs and repeate on sub
    for(int nsubs=1; nsubs<items.size(); nsubs++){

      if(items[nsubs].revents & ZMQ_POLLIN && running){
	zmq::message_t comm;
        xsubs.at(nsubs-1)->recv(&comm);
	xsubcat.send(comm);
      }

    }
    

    //scan for new pubs
    if(lastscan < ((long)time(NULL)) +120){

      lastscan=(long)time(NULL);
      
      zmq::message_t message(6);
      snprintf ((char *) message.data(),6 , "%s" ,"All 0");
      findservices.send(message);   
      
      zmq::message_t comm;
      findservices.recv(&comm);
      std::istringstream iss(static_cast<char*>(comm.data()));
      int size=0;
      iss>>size;
      
      xsubs.clear(); //more affective to keep old connections
      items.resize(1);
      xsubid.clear();

      for (int i=0; i<<size; i++){
	zmq::message_t comm;
	findservices.recv(&comm);
	std::istringstream iss(static_cast<char*>(comm.data()));
	
	Store tmp;
	tmp.JsonPaser(iss.str());
	std::string uuid;
	std::string ip;
	std::string port;
	std::string service;
	tmp.Get("uuid",uuid);
	tmp.Get("ip",ip);
	tmp.Get("remote_port",port);
	tmp.Get("msg_value",service);
	
	if(service == "Broker"){
	  
	  zmq::socket_t* sock = new  zmq::socket_t(*(args->context), ZMQ_XSUB);
	  std::stringstream connection;
	  connection<<"tcp://"<<ip<<":"<<port;
	  sock->connect(connection.str().c_str());
	  xsubs.push_back(sock);
	  zmq::pollitem_t item;
	  item.socket=sock;
	  item.fd=0;
	  item.events=ZMQ_POLLIN;
	  item.revents=0;
	  items.push_back(item);
	  xsubid.push_back(uuid);
	  
	}	
      }
    }
      
      
  }
  
  
  for (int i=0; i< xsubs.size(); i++){
    
    delete xsubs.at(i);
    xsubs.at(i)=0;
  }
  
  xsubs.clear();
  xsubid.clear();
  items.clear();
  
  pthread_exit(NULL);
  
}

