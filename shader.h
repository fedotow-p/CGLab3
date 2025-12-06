#ifndef __SHADER_H__
#define __SHADER_H__

#include "geometry.h"
#include "tgaimage.h"
#include "model.h"

struct IShader {
    virtual ~IShader() {}
    virtual Vec4f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(Vec3f bar, TGAColor& color) = 0;
};

class PhongShader : public IShader {
public:
    Matrix uniform_M;
    Matrix uniform_MIT;
    Vec3f uniform_light_dir;
    Vec3f uniform_eye_pos;

    Vec3f varying_normal[3];
    Vec3f varying_pos[3];

    Model* model;

    PhongShader(Model* m) : model(m) {}

    virtual Vec4f vertex(int iface, int nthvert) {
        std::vector<int> face = model->face(iface);
        Vec3f vertex = model->vert(face[nthvert]);
        Vec4f gl_Vertex = uniform_M * Vec4f(vertex.x, vertex.y, vertex.z, 1.0f);

        Vec3f v0 = model->vert(face[0]);
        Vec3f v1 = model->vert(face[1]);
        Vec3f v2 = model->vert(face[2]);
        Vec3f normal = ((v2 - v0) ^ (v1 - v0)).normalize();

        varying_normal[nthvert] = normal;
        varying_pos[nthvert] = vertex;

        return gl_Vertex;
    }

    virtual bool fragment(Vec3f bar, TGAColor& color) {
        Vec3f n = (varying_normal[0] * bar.x +
            varying_normal[1] * bar.y +
            varying_normal[2] * bar.z).normalize();

        Vec3f pos = varying_pos[0] * bar.x + varying_pos[1] * bar.y + varying_pos[2] * bar.z;

        Vec3f light_dir = uniform_light_dir.normalize();
        Vec3f view_dir = (uniform_eye_pos - pos).normalize();
        Vec3f reflect_dir = (n * (n * light_dir * 2.0f) - light_dir).normalize();

        float ambient = 0.1f;
        float diffuse = std::max(0.0f, n * light_dir);
        float specular = std::pow(std::max(0.0f, view_dir * reflect_dir), 32.0f);

        float intensity = ambient + diffuse + specular * 0.5f;
        intensity = std::min(1.0f, intensity);

        unsigned char c = static_cast<unsigned char>(255 * intensity);
        color = TGAColor(c, c, c, 255);

        return false;
    }
};

class TransparentCubeShader : public IShader {
public:
    Matrix uniform_M;
    Vec3f cube_center;
    float cube_size;
    TGAColor cube_color;


    Vec3f varying_pos[3];
    float varying_depth[3];

    TransparentCubeShader(Vec3f center = Vec3f(0,0,0), float size = 1.0f)
        : cube_center(center), cube_size(size)
    {
        // Красный с 50% прозрачностью
        cube_color = TGAColor(255, 0, 0, 128);
    }

    virtual Vec4f vertex(int iface, int nthvert) {
        // 8 вершин куба
        static Vec3f cube_verts[8] = {
            Vec3f(-1, -1, -1), Vec3f(1, -1, -1),
            Vec3f(-1,  1, -1), Vec3f(1,  1, -1),
            Vec3f(-1, -1,  1), Vec3f(1, -1,  1),
            Vec3f(-1,  1,  1), Vec3f(1,  1,  1)
        };

        // 12 граней куба (2 треугольника на каждую грань)
        static int cube_faces[12][3] = {
            {0,2,1}, {1,2,3},  // задняя грань
            {4,5,6}, {5,7,6},  // передняя грань
            {0,4,2}, {2,4,6},  // левая грань
            {1,3,5}, {3,7,5},  // правая грань
            {0,1,4}, {1,5,4},  // нижняя грань
            {2,6,3}, {3,6,7}   // верхняя грань
        };

        // Получаем вершину для текущей грани
        int vidx = cube_faces[iface][nthvert];
        Vec3f vertex = cube_verts[vidx] * cube_size + cube_center;

        varying_pos[nthvert] = vertex;
        Vec4f gl_Vertex = uniform_M * Vec4f(vertex.x, vertex.y, vertex.z, 1.0f);
        varying_depth[nthvert] = gl_Vertex.z;  // Сохраняем глубину

        return gl_Vertex;
    }

    virtual bool fragment(Vec3f bar, TGAColor& color) {
        float frag_depth = varying_depth[0] * bar.x +
                          varying_depth[1] * bar.y +
                          varying_depth[2] * bar.z;

        // float depth_factor = std::max(0.0f, std::min(1.0f, (frag_depth + 1.0f) / 2.0f));
        // unsigned char alpha = static_cast<unsigned char>(128 * (0.5f + 0.5f * depth_factor));

        color = TGAColor(cube_color.r, cube_color.g, cube_color.b, cube_color.a);
        return false;
    }
};

#endif //__SHADER_H__