/* Copyright (c) 2017-2018, CNRS-LAAS
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef RASTER_TILE_H
#define RASTER_TILE_H

#include <cmath>
#include <fstream>
#include <limits>

#include <opencv2/opencv.hpp>

#include <Vision/FireMapper/Raster_Reader.h>
#include <Vision/FireMapper/sensor_model.h>

using namespace std;

struct Pixel_Data
{
  double x_upleft;
  double y_upleft;
  double z_upleft;

  double x_upright;
  double y_upright;
  double z_upright;

  double x_downleft;
  double y_downleft;
  double z_downleft;

  double x_downright;
  double y_downright;
  double z_downright;

  int col;
  int row;
};


struct Pixel_Range
{
  uint64_t col_left;
  uint64_t col_right;

  uint64_t row_up;
  uint64_t row_down;

  Pixel_Range() {
    // This is an invalid range
    col_left = std::numeric_limits<uint64_t>::max();
    col_right = 0;
    row_up = std::numeric_limits<uint64_t>::max();
    row_down = 0;
  }
};

struct Raster_ALL
{
  int ncols;
  int nrows;
  int noData;
  vector<Pixel_Data> ListeP;
};


class Raster_Tile
{
private:
  Raster_Reader* Map;
  vector<Pixel_Data> Liste_Points;
  cv::Mat fireMap;
  cv::Mat fireMap_bayes;
  cv::Mat occupancy_map;
  cv::Mat fireMap_time;
  bool FireMap_modified;
  // Area of the raster where fire has been mapped into
  Pixel_Range mapped_area;
  sensor_model s_model;

public:
  double no_data;
  //Raster_Tile() = default;

  Raster_Tile(string path);

  bool Test_point(uint64_t x, uint64_t y);

  double get_elevation(uint64_t x, uint64_t y);

  double get_maxheight();

  double get_minheight();

  double get_max_east();

  double get_max_west();

  double get_max_north();

  double get_max_south();

  double get_pixel_width();

  void set_fireMap(uint64_t row, uint64_t col, uchar value, bool use_occupancygrid);

  void set_fireMap_time(uint64_t row, uint64_t col, double value);

  void set_sensor_model(sensor_model sen_mod);

  cv::Mat get_fireMap();

  cv::Mat get_fireMapbayes();

  cv::Mat get_fireMap_time();

  GDALDataset fireMap_time_gdal();

  bool Test_fireMap_Modified();

  void get_DEM_info();


  void ListePoints_Data();

  Raster_ALL get_ListePoints();

  Pixel_Range get_corners(double x,
                          double y);///Calculates four corners of the pixel that will be used to compute with in the raster, given the position of the camera centre.
  Pixel_Range get_Rastercorners(double x_left, double x_right, double y_up,
                                double y_down);///Gets the equivalent pixel position for the coordinates of the corners .

  Pixel_Data All_data(int r, int c);

  void Put_firemap_inGdal(string gdal_result_path);


  static vector<Raster_Tile> get_allMaps(const std::vector<std::string>& files)
  {
    vector<Raster_Tile> Carte;

    for (const auto& f: files){
        Raster_Tile RAST = Raster_Tile(f);
        Carte.emplace_back(RAST);
      }

    return Carte;

  };

  //vector<Raster_Tile*> get_allMaps(string folder);
  //virtual ~Raster_Tile();

};

#endif // RASTER_TILE_H