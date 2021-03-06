#include "path_planner/Map.h"

#include <string.h>
#include <stdlib.h>
#include <sstream> // stringstream

using namespace std;

//Constructor
Map::Map(int w, int h, float res){
  setWidth(w);
  setHeight(h);
  setResolution (res);
  
  //mapLayout is initlialized in default constructor
  mapLayout = (int**) malloc(sizeof( int*)*getHeight());
  for(int i=0;i<getHeight();i++)
  mapLayout[i] = (int*) malloc(sizeof(int)*getWidth());
  
}

//four-argument constructor to initialize map
  Map::Map(int w, int h, float res, int** mapMatrix, int obsSize, float obsRatio)
  {
    setWidth(w);
    setHeight(h);
    setResolution (res);
    setMapLayout(mapMatrix);
    setObstacleSize (obsSize);
    setObstacleRatio(obsRatio);
    //mapLayout is initlialized in  constructor
    mapLayout = (int**) malloc(sizeof( int*)*getHeight());
    for(int i=0;i<getHeight();i++)
    mapLayout[i] = (int*) malloc(sizeof(int)*getWidth());
  }
   
  
  //Copy constructor for the map
  Map::Map (Map* map)
  {
    setWidth(map->getWidth());
    setHeight(map->getHeight());
    setResolution (map->getResolution());
    setMapLayout(map->getMapLayout());
    setObstacleSize (map->getObstacleSize());
    setObstacleRatio( map->getObstacleRatio());
    
    
  }
  
  //Define Mutators
  void Map::setWidth(int w)
  {
   width=((w<0)?0:w);
  }
  
  void Map::setHeight(int h)
  {
    height=((h<0)?0:h);
  }
  
  void Map::setResolution (float res)
  {
    resolution=((res<0)?0:res);
  }
  
  void Map::setMapLayout(int** mapMatrix)
  {
    mapLayout = mapMatrix; 
  }
  void Map::setObstacleSize (int obsSize)
  {
    obstacleSize = obsSize;
  }
  void Map::setObstacleRatio(float obsRatio)
  {
    obstacleRatio = obsRatio;
  }
  
  //Define Accessors
  int Map::getWidth()
  {
    return width;
  }
  
  int Map::getHeight()
  {
    return height;
  }
  
  float Map::getResolution()
  {
      return resolution;
  }
  int** Map::getMapLayout()
  {
      return mapLayout;
  }
  int Map::getObstacleSize()
  {
    return obstacleSize;
  }
  float Map::getObstacleRatio()
  {
    return obstacleRatio;
  }
  
  //calculate the index of the Cell using its row and columun in the matrix
  int Map::getCellIndex(int i,int j)
  {
   return (i*getWidth())+j;
  
  }
  //get the row of the cell in the MapLayout using it index 
  int Map::getCellRowID(int index)
  {
     return index/getWidth();
  }
  //get the columun of the cell in the MapLayout using it index 
  int Map::getCellColID(int index)
  {
    return index%getWidth();
  }

  //read the MapLayout from a file
 void Map::importMapLayout(string path_name, const char* file_name)
{
  int row = 0, col = 0, numrows = 0, numcols = 0, pixel_maxi=0;
  string path_loc= path_name+ file_name; //the location of the file
  ifstream infile(path_loc.c_str());
  stringstream ss;
  string inputLine = "";
  if(!infile) 
  {      
    //file couldn't be opened
    cerr << "[I/O Error] The map file could not be opened. \nCheck file name and/or path" << endl;
    exit(1);
  }
  else
  {
  // First line : version
  getline(infile,inputLine);
  if(inputLine.compare("P2") != 0) cerr << "Version error" << endl;
  else cout << "Version : " << inputLine << endl;

  // Second line : comment
  getline(infile,inputLine);
  cout << "Comment : " << inputLine << endl;
  int indexObstacleRatio=inputLine.find("Obstacle Ratio=", 0);
  int index1= inputLine.find(" ", indexObstacleRatio+10);
  string obstacleRatio=inputLine.substr(indexObstacleRatio+15,index1-(indexObstacleRatio+15));
  setObstacleRatio(atof(obstacleRatio.c_str()));

  int indexObstacleSize=inputLine.find("Obstacle Size=", 0);
  int index2= inputLine.find(" ", indexObstacleSize+10);
  string obstacleSize=inputLine.substr(indexObstacleSize+14,index2-(indexObstacleSize+14));
  setObstacleSize(atoi(obstacleSize.c_str()));
  
  // Continue with a stringstream
  ss << infile.rdbuf();
  // Third line : size
  ss >> numcols >> numrows;
  cout << numcols << " columns and " << numrows << " rows" << endl;
  
  //the forth line
   ss >> pixel_maxi;
   cout << pixel_maxi << endl;
  
  setHeight(numrows);
  setWidth(numcols);
  mapLayout = (int**) malloc(sizeof( int*)*getHeight());
  for(int i=0;i<getHeight();i++)
  mapLayout[i] = (int*) malloc(sizeof(int)*getWidth());

  // Following lines : data
  for(row = 0; row < numrows; ++row)
    for (col = 0; col < numcols; ++col) ss >> getMapLayout()[row][col];

  // Now print the MapLayout to see the result
 for(row = 0; row < numrows; ++row) {
    for(col = 0; col < numcols; ++col) {
     // cout << getMapLayout()[row][col] << " ";
    }
    //cout << endl;
  }
 
  infile.close();
  
  }
}

// write the MapLayout matrix in a file
void Map::exportMapLayout(const char* file_name, int** mapMatrix)
{
   ofstream file (file_name, ios::trunc);  
    file<<"P2";
    file<<endl;
    file<<"#Generated by exportMapLayout in Map.h";
    file<<endl;
    file<<getWidth();
    file<<" ";
    file<<getHeight();
    file<<endl;
    file<<"100";
    file<<endl;
    for(int i=0;i<getHeight();i++)
    {
      for(int j=0;j<getWidth();j++)
      {
	file<<mapMatrix[i][j];
	if(j<getWidth()-1)
	  file<<" ";
      }
      if(i<getHeight()-1)
	file<<endl;
      }
}


    
