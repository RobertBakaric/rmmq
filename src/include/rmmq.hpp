/*
 * rmmq.hpp
 * 
 * Copyright 2015 Robert Bakaric <rbakaric@irb.hr>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<algorithm>

using namespace std;

template<typename TInt>
class ST{
  protected:
  
  vector<vector<TInt>> MinSTable;
  vector<vector<TInt>> MaxSTable;  
  
  public:

/* Constructor */
  ST();
  ST(const vector<TInt>& rvec);  

/* Destructor */
  ~ST();


/* Builder */
  void MakeST(const vector<TInt>& rvec);
  
  void make(const vector<TInt>& rvec);
  void destroy();

/* Getter -- not finished */ 
  TInt Get(const TInt x, const TInt y);
  
};


/* Constructor */
template <typename TInt>
ST<TInt>::ST(){}

template <typename TInt>
ST<TInt>::ST(const vector<TInt>& rvec){
  MakeST(rvec);
}

template <typename TInt>
void ST<TInt>::make(const vector<TInt>& rvec){
  MakeST(rvec);
}


/* Destructor */
template <typename TInt>
ST<TInt>::~ST(){
  vector<vector<TInt>>().swap(MinSTable);
  vector<vector<TInt>>().swap(MaxSTable);
}

template <typename TInt>
void ST<TInt>::destroy(){
  vector<vector<TInt>>().swap(MinSTable);
  vector<vector<TInt>>().swap(MaxSTable);
}



/* Functions Make ST */
template <typename TInt>
void ST<TInt>::MakeST(const vector<TInt>& rvec){

  TInt x = log2(rvec.size()) + 1;
  TInt p = 0;

  vector<TInt> vec(x);

  MinSTable.resize(rvec.size()+1, vec);
  MaxSTable.resize(rvec.size()+1, vec);

  for(TInt i = 0; i < rvec.size(); i++){
    MinSTable[i][0] = i;
    MaxSTable[i][0] = i;
  }
  
  for(TInt i = 1; i< x; i++){
    p=(1 << (i - 1));
    for(TInt j=0; j+(1<<i)-1 < rvec.size(); j++){
      MinSTable[j][i] = (rvec[MinSTable[j][i-1]] < rvec[MinSTable[j+p][i-1]]) ? (MinSTable[j][i-1]) : (MinSTable[j+p][i-1]);
      MaxSTable[j][i] = (rvec[MaxSTable[j][i-1]] > rvec[MaxSTable[j+p][i-1]]) ? (MaxSTable[j][i-1]) : (MaxSTable[j+p][i-1]);
    }
  }
}





template<typename TInt>
class RMMQ : protected ST<TInt>{
  
  vector <TInt> range;
/* Functions */
  
  public:
/* Constructor */
  RMMQ();
  RMMQ(const vector <TInt>& vec);

/* Destructor */
  ~RMMQ();

/* Queries */

  TInt MinVal(const TInt, const TInt);
  TInt MaxVal(const TInt, const TInt);
  TInt MinPos(const TInt, const TInt);
  TInt MaxPos(const TInt, const TInt); 

  void make(const vector<TInt>& vec);
  void destroy();
};

/* Constructors */

template <typename TInt>
RMMQ<TInt>::RMMQ():ST<TInt>(){}

template <typename TInt>
RMMQ<TInt>::RMMQ(const vector<TInt>& vec) : range(vec), ST<TInt> (vec){}

/* Destructor */
template <typename TInt>
RMMQ<TInt>::~RMMQ(){
  vector<TInt>().swap(range);
}


template <typename TInt>
void RMMQ<TInt>::destroy(){
  vector<TInt>().swap(range);
  ST<TInt>::destroy();
}


/* Functions */
template <typename TInt>
void RMMQ<TInt>::make(const vector<TInt>& vec){
 range = vec;
 ST<TInt>::make(vec);
}

/* Queries */
template <typename TInt>
TInt RMMQ<TInt>::MinVal(const TInt x, const TInt y){
 
  TInt mn = min(x,y);
  TInt mx = max(x,y);

  TInt o = log2(mx - mn + 1);
  TInt p = (1<<o);
  return (range[this->MinSTable[mn][o]] < range[this->MinSTable[mx - p + 1][o]]) ? (range[this->MinSTable[mn][o]]) : (range[this->MinSTable[mx - p + 1][o]]);
}

  


template <typename TInt>
TInt RMMQ<TInt>::MaxVal(const TInt x, const TInt y){

  TInt mn = min(x,y);
  TInt mx = max(x,y);

  TInt o = log2(mx - mn + 1);
  TInt p = (1<<o);
  return (range[this->MaxSTable[mn][o]] > range[this->MaxSTable[mx - p + 1][o]]) ? (range[this->MaxSTable[mn][o]]) : (range[this->MaxSTable[mx - p + 1][o]]);
}


template <typename TInt>
TInt RMMQ<TInt>::MinPos(const TInt x, const TInt y){
 
  TInt mn = min(x,y);
  TInt mx = max(x,y);
  
  TInt o = log2(mx - mn + 1);
  TInt p = (1<<o);
  return (range[this->MinSTable[mn][o]] < range[this->MinSTable[mx - p + 1][o]]) ? (this->MinSTable[mn][o]) : (this->MinSTable[mx - p + 1][o]);
}




template <typename TInt>
TInt RMMQ<TInt>::MaxPos(const TInt x, const TInt y){
  
  TInt mn = min(x,y);
  TInt mx = max(x,y);

  TInt o = log2(mx - mn + 1);
  TInt p = (1<<o);
  return (range[this->MaxSTable[mn][o]] > range[this->MaxSTable[mx - p + 1][o]]) ? (this->MaxSTable[mn][o]) : (this->MaxSTable[mx - p + 1][o]);
}

