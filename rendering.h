#ifndef TINY_RAYTRACER_RENDERING_H
#define TINY_RAYTRACER_RENDERING_H

#include <fstream>
#include <vector>
#include "geometry.h"

struct Material
{
    explicit Material(const Vec3f &color) : diffuse_color(color) {}
    Material() : diffuse_color() {}
    Vec3f diffuse_color;
};


struct Sphere
{
    Vec3f center;
    float radius;
    Material material;

    Sphere(const Vec3f &c, const float &r, const Material &m) : center(c), radius(r), material(m) {}

    bool ray_intersect(const Vec3f &origin, const Vec3f &directionVec, float &tValue) const
    {
        Vec3f distFromOrigToCenter = center - origin;                     // расстояние от точки начала к центру сферы
        float distProjection       = distFromOrigToCenter * directionVec; // проекция расстояния на ось луча

        // линия проекции (расстояние от центра до луча)
        float distProjectionLine = distFromOrigToCenter * distFromOrigToCenter
                                   - distProjection * distProjection;
        if (distProjectionLine > radius * radius)
        {
            return false; // луч не пересекает сферу, т.к. он направлен выше неё
        }

        // расстояние от первого пересечения до точки падения линии проекции
        float difference = sqrtf(radius * radius - distProjectionLine);

        // Параметрическое уравнение луча: P(t) = O + t*D
        // O - точка начала, D - вектор направления, t - изменяющийся параметр, который задаёт множество точек луча
        float firstIntersectionCoeff  = distProjection - difference; // параметр первого пересечения
        float secondIntersectionCoeff = distProjection + difference; // параметр второго пересечения
        if (firstIntersectionCoeff < 0)
        {
            firstIntersectionCoeff = secondIntersectionCoeff;
        }
        // t = 0 - луч пересекает сферу единожды
        // t > 0 - луч пересекает сферу дважды
        // t < 0 - луч не пересекает сферу
        tValue = firstIntersectionCoeff;
        return firstIntersectionCoeff >= 0;
    }
};

inline bool scene_intersect(const Vec3f &orig, const Vec3f &dir,
                            const std::vector<Sphere> &spheres,
                            Vec3f &hit, Vec3f &N, Material &material)
{
    float spheres_dist = std::numeric_limits<float>::max();
    for (size_t i = 0; i < spheres.size(); i++)
    {
        float dist_i;
        bool hasIntersection = spheres[i].ray_intersect(orig, dir, dist_i);
        if (hasIntersection && dist_i < spheres_dist)
        {
            spheres_dist = dist_i;
            hit          = orig + dir * dist_i;
            N            = (hit - spheres[i].center).normalize();
            material     = spheres[i].material;
        }
    }
    return spheres_dist < 1000;
}


inline Vec3f cast_ray(const Vec3f &orig, const Vec3f &dir, const std::vector<Sphere> &spheres)
{
    Vec3f point, N;
    Material material;

    if (!scene_intersect(orig, dir, spheres, point, N, material))
    {
        return {0.2, 0.7, 0.8}; // background color
    }

    return material.diffuse_color;
}

inline void render(const std::vector<Sphere> &spheres)
{
    constexpr int width         = 1024;
    constexpr int height        = 768;
    constexpr int fieldOfView   = M_PI / 2.;
    constexpr float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    std::vector<Vec3f> framebuffer(width * height);

#pragma omp parallel for
    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            float x = (2 * (i + 0.5) / static_cast<float>(width) - 1)
                      * tan(fieldOfView / 2.) * aspectRatio;
            float y = -(2 * (j + 0.5) / static_cast<float>(height) - 1)
                      * tan(fieldOfView / 2.);
            Vec3f directionVec = Vec3f(x, y, -1).normalize();

            framebuffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), directionVec, spheres);
        }
    }

    std::ofstream ofs;
    ofs.open("../out.ppm", std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height * width; ++i)
    {
        for (size_t j = 0; j < 3; j++)
        {
            // Вещественное значение цвета вгоняется в промежуток от 0 до 1,
            // затем домножается на 255, т.к. формат .ppm хранит каналы цветов в размере 1 байта
            ofs << static_cast<char>(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    ofs.close();
}
#endif //TINY_RAYTRACER_RENDERING_H
