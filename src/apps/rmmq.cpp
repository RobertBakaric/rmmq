/*
 * rmmq.cpp
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
#include<fstream>
#include<vector>
#include<string>

#include "boost/variant.hpp"
#include <boost/program_options.hpp>

#include<rmmq.hpp>


namespace po = boost::program_options;
using namespace std;



void PrintAcknowledgements(){

cout << "**************************************************************************************"<<"\n";
cout <<"                         rmmq - Range minimum/maximum query                                "<<"\n";
cout <<"                                          by                                           "<<"\n";
cout <<"                                    Robert Bakaric                                     "<<"\n\n";
cout <<"CONTACT:                                                                               "<<"\n";
cout <<" Code written and maintained by Robert Bakaric,                                        "<<"\n";
cout <<" email: rbakaric@irb.hr , bakaric@evolbio.mpg.de                                       "<<"\n\n";
cout <<"ACKNOWLEDGEMENT:                                                                       "<<"\n";
cout <<" http://www.topcoder.com/tc?module=Static&d1=tutorials&d2=lowestCommonAncestor         "<<"\n\n";
cout <<"LICENSE:                                                                               "<<"\n";
cout <<" The program is distributed under the GNU General Public License. You should have      "<< "\n";
cout <<" received a copy of the licence together  with this software. If not, see              "<< "\n";
cout <<" http://www.gnu.org/licenses/                                                          "<< "\n";
cout <<"***************************************************************************************"<< "\n\n\n";
}


template <typename INT, typename CHARA>
po::variables_map SetOptions(INT& argc, CHARA& argv){

    try {
        int opt;
        string version = "1.01";
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("version,v", "print version information")
            ("input-file,i", po::value< string >(), "input file")
        ;

        po::positional_options_description p;
        p.add("input-file,i", -1);
        
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
                  options(desc).positional(p).run(), vm);
        po::notify(vm);
    
        if (vm.count("help")) {
            cout << "Usage: ./program [options]\n\n";
            PrintAcknowledgements();
            cout << desc;
            exit(0);
        }else if (vm.count("version")) {
            cout << "Program version:  " << version << "\n";
            exit(0);
        }
        if (!vm.count("input-file")){
            cout << "Input file is not defined \n";
            exit(0);
        }
        return vm;    
    }
    catch(std::exception& e)
    {
        cout << e.what() << "\n";
        exit(0);
    }    
}

void ReadRange(const string & file, vector<int>& vec){
  
  fstream fs;
  fs.open (file.c_str(), ios::in);
  if ( !fs.is_open()){
    cerr << "Cannot open file: "<<file <<"! Line:" << __LINE__<< endl;
    exit(-1);
  }

  int i = 0; 
  while( !fs.eof()) {
    if(vec.size() == i)
       vec.resize((vec.size()+1)*2);
    fs >> vec[i++];
    
  }  
 
  vec.resize(i-1);
  fs.close();
}




int main (int argc, char** argv){
  
  
  
  po::variables_map arg;
  arg = SetOptions(argc,argv);

  string file =  arg["input-file"].as<string>();

   vector<int> vec;
   
   ReadRange(file, vec);
/*Print array */
   cout << "Positions:";
   for(int i = 0; i< vec.size(); i++){
      cout << "\t[" << i+1 <<"]";
   }
   cout << endl;
   cout << "Values:   ";  
   for(int i = 0; i< vec.size(); i++){
      cout << "\t" << vec[i] ;
   }
   cout << endl;
/* Build ST */
   RMMQ<int> rmmq(vec);

/* Process Queries */
  int in = 0;
  int x=0, y=0;
  
  while(1){
    cout << "Note: Ctrl-c to quit\n";
    cout << "Start: ";
    cin >> x;
    cout << "Stop: ";
    cin >> y;
    
    if(x<=vec.size() && y<=vec.size() && x>=0 && y>=0){ 
      cout << "\nmin("<< x<<","<<y<<"): val(" << rmmq.MinVal(x-1,y-1) <<") pos(" << rmmq.MinPos(x-1,y-1)+1 << ")"<< endl;
      cout << "max("<< x<<","<<y<<"): val(" << rmmq.MaxVal(x-1,y-1) <<") pos(" << rmmq.MaxPos(x-1,y-1)+1 << ")\n"<< endl;
    }else
      cout << "\nValues are outside the given interval\n";
  }
      
  
  return 0;
}
