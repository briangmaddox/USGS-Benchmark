/***************************************************************************
                          BenchmarkException.cpp  -  description
                             -------------------
    begin                : Fri Jun 29 2001
    copyright            : (C) 2001 by 
    email                : cbilder@mcpc1159
 ***************************************************************************/

/***************************************************************************
 * This software is free under the terms of the MPL (Mozilla Public License*
 * See www.mozilla.org for details.                                        *
 ***************************************************************************/

#include "BenchmarkException.h"

using namespace USGSBenchmark;

//*******************************************************************
BenchmarkException::BenchmarkException() : err(BENCH_UNKOWN_ERR)
{}

//**********************************************************************
BenchmarkException::BenchmarkException(const std::string & inErrorMesg)
 : err(inErrorMesg)
{}

//********************************************************************
BenchmarkException::BenchmarkException(const char * inErrorMesg) :
 err(inErrorMesg)
{}

//*********************************************************************
BenchmarkException::~BenchmarkException()
{}

//****************************************************************
std::string BenchmarkException::getError() const throw()
{
  return err;
}

//***************************************************************
const char * BenchmarkException::getcstrError() const throw()
{
  return err.c_str();
}

//***************************************************************
void BenchmarkException::setError(std::string inErrorMessage) throw()
{
  err = inErrorMessage;
}
