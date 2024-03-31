/*=============================================================================
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn
# Last modified: 2015-05-11 00:03
# Filename: PmtProp.h
# Description: 
=============================================================================*/
#ifndef PmtProp_H
#define PmtProp_H
//define PMT properity
#include "TVector3.h"
#include "Identifier/Identifier.h"
#include <vector>
enum Pmttype {
	_PMTNULL, 
	_PMTINCH3, 
	_PMTINCH20, 
	_PMTTT,
}; 
struct PmtProp{
	TVector3 pos;
	double q;
	double fht;
	double res; 
	double trise; 
	double amp; 
	double integ; 
	bool used;
	// Add loc: 1 == CD, 2 == WP, 3 == TT, by wudr
	int loc;
	int value;
	Pmttype type;
        // Hit level
        std::vector<float> hittime;
        std::vector<float> hitq;
};
typedef std::vector<PmtProp> PmtTable; 
#endif
