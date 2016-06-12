/***************************************************************************
                          ServerBenchMaster.cpp  -  description
                             -------------------
    begin                : Tue Jul 3 2001
    copyright            : (C) 2001 by 
    email                : cbilder@mcpc1159
 ***************************************************************************/

/***************************************************************************
 * This software is free under the terms of the MPL (Mozilla Public License*
 * See www.mozilla.org for details.                                        *
 ***************************************************************************/

#include "ServerBenchMaster.h"
#include <string.h>

using namespace USGSBenchmark;

//****************************************************************************
ServerBenchMaster::ServerBenchMaster() : PvmBenchmark(SERVERBENCHMASTERTAG),
      childtid(0),
      num_runs(0),
      explicit_names(false),
      number_slaves(0),
      data_size(0),
      mavg(0)
{}

//****************************************************************************
ServerBenchMaster::~ServerBenchMaster()
{
  if (childtid)
    delete childtid;

}

//****************************************************************************
bool ServerBenchMaster::run() throw(BenchmarkException)
{
  int  bufferid(0), len(0), tag(0),    //pvm tags
    temptid(0);
  Time_stat timest;                    //temporary time file
  int counter(0);
  std::list<Time_stat> timelist;       //list of time entries
  char * buffer(0);                    //tempoary buffer

  try
  {
    
    if (!generateConfig())               //get the configuration
      return false;                      //operation canceled
    
    //create the buffer 
    if (!(buffer = new (std::nothrow) char[data_size]))
      throw BenchmarkException
        ("ServerBenchMaster::run-no memory");

    spawn_slaves();                      //spawn the slaves
    
    //broad cast the data_size
    pvm_initsend(PvmDataDefault);
    pvm_pklong(&data_size, 1, 1);
    pvm_mcast(childtid, number_slaves, BENCH_SETUP);
    
    //loop for the number of runs
    for (counter = 0; counter < num_runs; ++counter)
    {
      //get the starting time
      if (!timer.start())
        throw BenchmarkException
          ("ServerBenchMaster::run-could not get start time (kill pvm)");
      
      //get a message
      bufferid = pvm_recv(-1, -1);
      pvm_bufinfo(bufferid, &len, &tag, &temptid);
      
      //check the tag on the message
      if (tag == BENCH_ERR)
        throw BenchmarkException
          ("ServerBenchMaster::run-slave sent error (kill pvm)");
      
      pvm_initsend(PvmDataDefault);
      pvm_pkbyte(buffer, data_size, 1);
      pvm_send(temptid, BENCH_WORK);
      
      //get the ending time
      if (!timer.stop(timest.lapse_tm))
        throw BenchmarkException
          ("ServerBenchMaster::run-could not get stopping time (kill pvm)");
      
      //put the timing in a list
      timelist.push_back(timest);
    }

    //loop through and tell the slave to stop
    for (counter = 0; counter < number_slaves; counter++)
    {
      bufferid = pvm_recv(-1, -1);
      pvm_bufinfo(bufferid, &len, &tag, &temptid);
      
      if (tag == BENCH_ERR)
        throw BenchmarkException
          ("ServerBenchMaster::run-slave sent error on kill (kill pvm)");
      
      pvm_initsend(PvmDataDefault);
      pvm_send(temptid, BENCH_QUIT);
    }
    
    //get my time average
    mavg = average(timelist);
    
    savg.resize(number_slaves);
    //now loop through and ask for the slaves averages
    for (counter = 0; counter < number_slaves; counter++)
    {
      pvm_initsend(PvmDataDefault);
      pvm_send(childtid[counter], BENCH_WORK);
      
      bufferid = pvm_recv(-1, -1);
      pvm_bufinfo(bufferid, &len, &tag, &temptid);
      
      if (tag == BENCH_ERR)
        throw BenchmarkException
         ("ServerBenchMaster::run-slave sent error instead of avg (kill pvm)");
      
      pvm_upkfloat(&savg[counter], 1, 1);
      
      //tell slave to get lost
      pvm_initsend(PvmDataDefault);
      pvm_send(childtid[counter], BENCH_QUIT);
    }
    
    //now that we have results output them
    if (!outputResults())
      throw BenchmarkException
        ("ServerBenchMaster::run-could not ouput the results");
    
    delete [] childtid;
    delete [] buffer;
    childtid = 0;
    
    return true;
  }
  catch(BenchmarkException & err)
  {
    delete [] childtid;
    delete [] buffer;
    childtid = 0;
    throw err;
  }
  catch(...)
  {
    delete [] childtid;
    delete [] buffer;
    childtid = 0;
    throw BenchmarkException
      ("ServerBenchmaster::run-unkown error");
  }
    
}

//****************************************************************************
void ServerBenchMaster::spawn_slaves()  throw (BenchmarkException)
{
  int counter(0);                      //counter for loops
  int numslaves(0);                    //the number of slaves actually spawned
  char * arg[2];                       //for slave argument passing
  char * slavename = 0;                //slave exe name buffer
  char * hostbuf = 0;                  //buffer for hostnames

  try
  {
    //do some checks
    if (number_slaves <= 0)
      throw BenchmarkException
        ("ServerBenchMaster::spawn_slaves number of slaves is wrong");
    
    if(!slave_exe.size())
      throw BenchmarkException
        ("ServerBenchMaster::spawn_slaves slave exe name nonexistant");
    
    if (!(slavename = new (std::nothrow) char[slave_exe.size()+1]))
      throw BenchmarkException
        ("ServerBenchMaster::spawn_slaves could not create slave buffer");
    
    //copy the slave name
    strcpy(slavename, slave_exe.c_str());

    //check the childtid array
    if(childtid)
    {
      delete [] childtid;
      childtid = 0;
    }

    //create the childtid array
    if(!(childtid = new (std::nothrow) int[number_slaves]))
      throw BenchmarkException
        ("ServerBenchMaster::run-unable to create childtid array");
    
    if (slave_args.size())
    {
      if (!(arg[0] = new (std::nothrow) char[slave_args.size()+1]))
        throw BenchmarkException
          ("ServerBenchMaster::run-unable to create slave arguments");
      strcpy(arg[0], slave_args.c_str());
    }
    else
      arg[0] = 0;
    
    arg[1] = 0;
    
    
    if (explicit_names)
    {
      if (name_table.size() != static_cast<unsigned int>(number_slaves))
        throw BenchmarkException
              ("ServerBenchMaster::spawn_slaves name_table is bad");

      for(counter = 0; counter < number_slaves; ++counter)
      {
        if(!(hostbuf = new (std::nothrow) 
             char[name_table[counter].size() + 1]))
          throw BenchmarkException
            ("ServerBenchMaster::spawn_slaves could not create host_buf");
        
        strcpy(hostbuf, name_table[counter].c_str());
        
        //do a spawning loop
        numslaves = pvm_spawn(slavename, arg, PvmTaskHost,
                              hostbuf, 1,
                              &(childtid[counter]));
        if (numslaves != 1)
          throw BenchmarkException
            ("ServerBenchMaster::spawn_slaves unable to spawn explicit");
        
        
        delete [] hostbuf;
        hostbuf = 0;
      }
    }
    else
    {
      //just spawn a bunch of slaves
      numslaves = pvm_spawn(slavename, arg, PvmTaskDefault,
                            0, number_slaves, childtid);
      if (numslaves != number_slaves)
      {
        throw BenchmarkException
          ("ServerBenchMaster::spawn_slaves unable to spawn");
      }
    }
    
    delete  [] slavename;
    delete [] hostbuf;
    delete [] arg[0];

  }
  catch(BenchmarkException & err)
  {
    delete [] childtid;
    childtid = 0;
    delete [] slavename;
    delete [] hostbuf;
    delete [] arg[0];
    throw err;      //rethrow
  }
  catch(...)
  {
    delete [] childtid;
    childtid = 0;
    delete [] slavename;
    delete [] hostbuf;
    delete [] arg[0];
    throw BenchmarkException
      ("ServerBenchMaster::spawn_slaves - unkown error");
  }
    
}





