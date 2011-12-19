#ifndef ALIB_ADIRIRECTION2D_H
#define ALIB_ADIRIRECTION_H


enum ADirection2d { D_NORTH, D_NORTHEAST, D_EAST, D_SOUTHEAST, D_SOUTH,
  D_SOUTWEST, D_WEST, D_NORTHWEST, D_NONE };


ADirection2d turn(ADirection2d& d, int deg);


#endif // ALIB_ADIRIRECTION_H
