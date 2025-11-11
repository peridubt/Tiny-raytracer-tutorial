#define USE_MATH_DEFINES
#include <fstream>
#include <vector>
#include "geometry.h"

void render()
{
    constexpr int width  = 1024;
    constexpr int height = 768;
    std::vector<Vec3f> framebuffer(width * height);

    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            framebuffer[i + j * width] = Vec3f(j / static_cast<float>(height),
                                               i / static_cast<float>(width), 0);
        }
    }

    std::ofstream ofs;
    ofs.open("../out.ppm", std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height * width; ++i)
    {
        for (size_t j = 0; j < 3; j++)
        {
            ofs << static_cast<char>(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    ofs.close();
}

int main()
{
    render();

    return 0;
}
