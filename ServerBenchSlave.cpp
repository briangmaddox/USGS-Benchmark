/***************************************************************************
                          ServerBenchSlave.cpp  -  description
                             -------------------
    begin                : Thu Jul 12 2001
    copyright            : (C) 2001 by 
    email                : cbilder@mcpc1159
 ***************************************************************************/

/***************************************************************************
 * This software is free under the terms of the MPL (Mozilla Public License*
 * See www.mozilla.org for details.                                        *
 ***************************************************************************/

#include "ServerBenchSlave.h"
#include <fstream>

using namespace USGSBenchmark;

//***************************************************************************
ServerBenchSlave::ServerBenchSlave() : PvmBenchmark(SERVERBENCHSLAVETAG),
mastertid(0)
{}

//***************************************************************************
ServerBenchSlave::~ServerBenchSlave()
{}


//***************************************************************************
bool ServerBenchSlave::run() throw(BenchmarkException)
{
  int  bufferid(0), len(0), tag(0),    //pvm tags
  temptid(0);
  std::list<Time_stat> timelist;       //list of time entries
  char * buf(0);                       //tempoary buffer
  long int buffer_size(0);             //buffer size
  bool done(false);
  float avg(0);                        //average to give to master
  
  try
  {
    //get the parent
    mastertid = pvm_parent();
    
 
    if (mastertid == PvmNoParent)
    {
      throw BenchmarkException
        ("ServerBenchSlave::run-Slave not spawned by master!!");
    }
    
 
    bufferid = pvm_recv(-1, -1);
    pvm_bufinfo(bufferid, &len, &tag, &temptid);
    
    //check the tag
    if (tag != BENCH_SETUP)
    {
      //init the buffer
      pvm_initsend(PvmDataDefault);
      //send error to the master
      pvm_send(mastertid, BENCH_ERR);
      throw BenchmarkException
        ("ServerBenchSlave::run-Slave setup tag is wrong");
    }
    
    //get the buffer size
    pvm_upklong(&buffer_size, 1, 1);
    
    if (!buffer_size)
    {
      //init the buffer
      pvm_initsend(PvmDataDefault);
      //send the error to the master
      pvm_send(mastertid, BENCH_ERR);
      throw BenchmarkException
        ("ServerBenchSlave::run-Slave buffer size is bad");
    }
    
    //create the buffer
    if (!(buf = new (std::nothrow) char [buffer_size]))
    {
      pvm_initsend(PvmDataDefault);
      pvm_send(mastertid, BENCH_ERR);
      throw BenchmarkException
        ("ServerBenchSlave::run-Unable to create the buffer");
    }
    
    //go into the loop until
    while(!done)
    {
      //use the generic send and recv func
      done = !send_recv(timelist, mastertid, 
                        BENCH_WORK, 0, 0, buf, buffer_size);
      
    }
    
    //get the average
    avg = average(timelist);
    
    //get the request for a buffer
    bufferid = pvm_recv(-1, -1);
    pvm_bufinfo(bufferid, &len, &tag, &temptid);
    
    if (tag != BENCH_WORK)
      throw BenchmarkException
        ("ServerBenchSlave::run-invalid tag for average");
    
    //send it
    pvm_initsend(PvmDataDefault);
    pvm_pkfloat(&avg, 1, 1);
    pvm_send(mastertid, BENCH_WORK);
    
    //get the reply
    bufferid = pvm_recv(-1, -1);
    pvm_bufinfo(bufferid, &len, &tag, &temptid);
    
    //delete the buffer
    delete [] buf;
    return true;
  }
  catch(BenchmarkException & err)
  {
    delete [] buf;
    //send a error to the master
    pvm_initsend(PvmDataDefault);
    pvm_send(mastertid, BENCH_ERR);
    throw err;
    }
  catch(...)
  {
    delete [] buf;
    //send a error to the master
    pvm_initsend(PvmDataDefault);
    pvm_send(mastertid, BENCH_ERR);
    throw BenchmarkException("SeverBenchSlave::run -Unkown error");
  }
}
