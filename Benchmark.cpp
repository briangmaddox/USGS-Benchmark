/***************************************************************************
                          Benchmark.cpp  -  The base class for our beowulf
                                          benchmarking class.
                             -------------------
    begin                : Thu Jun 28 11:49:19 CDT 2001
    copyright            : (C) 2001 by
    email                : athmer@usgs.gov
    author               : Keith Athmer and Chris Bilderback
 ***************************************************************************/

/***************************************************************************
 * This software is free under the terms of the MPL (Mozilla Public License*
 * See www.mozilla.org for details.                                        *
 ***************************************************************************/

#include "Benchmark.h"

using namespace USGSBenchmark;

//***************************************************************************
Benchmark::Benchmark(const std::string & inid)
{
  id = inid;
}

//***************************************************************************
Benchmark::~Benchmark()
{}

//***************************************************************************
std::string Benchmark::getId() const throw()
{
  return id;
}

//***************************************************************************
float Benchmark::average(const std::list<Time_stat> & time_list)
  const throw(BenchmarkException)
{
  std::list<Time_stat>::const_iterator it;
  float ret(0);

  if (!time_list.size())
    throw BenchmarkException
           ("Benchmark::average-nothing to average!");

  for(it = time_list.begin(); it != time_list.end(); it++)
  {
    ret += (*it).lapse_tm;
  }

  ret = ret/static_cast<float>(time_list.size());
  return ret;
}
