/*=============================================================================
#
# Author:       Artem Chukanov - chukanov@jinr.ru
# Created:      12.10.2021
# Filename:	SphFunc.h
# Description:
=============================================================================*/
#ifndef SphFunc_h
#define SphFunc_h

class SphFunc
{
 public:

  virtual ~SphFunc() {}

  virtual int GetMaxOrder() const = 0; // get maximum order of spherical function
  virtual int GetLArraySize() const = 0; // get legendre array size
  virtual double GetCnk(int pmtType, int index) const = 0; // get cos coefficient
  virtual double GetSnk(int pmtType, int index) const = 0; // get sin coefficient
};

#endif // SphFunc_h
