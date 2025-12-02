//
// Created by elmao on 02.12.2025.
//

#ifndef CGLAB3_MODEL_H
#define CGLAB3_MODEL_H

#include <vector>
#include "geometry.h"

class Model {
private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<int> > faces_;
public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f vert(int i);
    std::vector<int> face(int idx);
};

#endif //CGLAB3_MODEL_H