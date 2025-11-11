#ifndef TINY_RAYTRACER_GEOMETRY_H
#define TINY_RAYTRACER_GEOMETRY_H
#include <cmath>
#include <cassert>
#include <iostream>

template<size_t DIM, typename T>
struct vec
{
private:
    T data_[DIM];

public:
    vec()
    {
        for (size_t i = DIM; i--; data_[i] = T()) {}
    }

    T &operator[](const size_t i)
    {
        assert(i<DIM);
        return data_[i];
    }

    const T &operator[](const size_t i) const
    {
        assert(i<DIM);
        return data_[i];
    }
};

typedef vec<2, float> Vec2f; // 2D вещественный вектор
typedef vec<3, float> Vec3f; // 3D вещественный вектор
typedef vec<3, int> Vec3i;   // 3D целочисленный вектор
typedef vec<4, float> Vec4f; // 4D вектор (для аффинных преобразований)

template<typename T>
struct vec<2, T>
{
    vec() : x(T()), y(T()) {}

    vec(T X, T Y) : x(X), y(Y) {}

    template<class U>
    explicit vec(const vec<2, U> &v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

    T &operator[](const size_t i)
    {
        assert(i<2);
        return i <= 0 ? x : y;
    }

    const T &operator[](const size_t i) const
    {
        assert(i<2);
        return i <= 0 ? x : y;
    }

    T x, y;
};

template<typename T>
struct vec<3, T>
{
    T x, y, z;

    vec() : x(T()), y(T()), z(T()) {}

    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    T &operator[](const size_t i)
    {
        assert(i<3);
        return i <= 0 ? x : (1 == i ? y : z);
    }

    const T &operator[](const size_t i) const
    {
        assert(i<3);
        return i <= 0 ? x : (1 == i ? y : z);
    }

    float norm() { return std::sqrt(x * x + y * y + z * z); }

    vec<3, T> &normalize(T l = 1)
    {
        *this = (*this) * (l / norm());
        return *this;
    }
};


template<typename T>
struct vec<4, T>
{
    T x, y, z, w;

    vec() : x(T()), y(T()), z(T()), w(T()) {}

    vec(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}

    T &operator[](const size_t i)
    {
        assert(i<4);
        return i <= 0 ? x : (1 == i ? y : (2 == i ? z : w));
    }

    const T &operator[](const size_t i) const
    {
        assert(i<4);
        return i <= 0 ? x : (1 == i ? y : (2 == i ? z : w));
    }
};

// Скалярное произведение векторов
template<size_t DIM, typename T>
T operator*(const vec<DIM, T> &lhs, const vec<DIM, T> &rhs)
{
    T ret = T();
    for (size_t i = DIM; i--; ret += lhs[i] * rhs[i]) {}
    return ret;
}

// Сложение векторов
template<size_t DIM, typename T>
vec<DIM, T> operator+(vec<DIM, T> lhs, const vec<DIM, T> &rhs)
{
    for (size_t i = DIM; i--; lhs[i] += rhs[i]) {}
    return lhs;
}

// Вычитание векторов
template<size_t DIM, typename T>
vec<DIM, T> operator-(vec<DIM, T> lhs, const vec<DIM, T> &rhs)
{
    for (size_t i = DIM; i--; lhs[i] -= rhs[i]) {}
    return lhs;
}

// Умножение вектора на число
template<size_t DIM, typename T, typename U>
vec<DIM, T> operator*(const vec<DIM, T> &lhs, const U &rhs)
{
    vec<DIM, T> ret;
    for (size_t i = DIM; i--; ret[i] = lhs[i] * rhs) {}
    return ret;
}

// Противоположный вектор
template<size_t DIM, typename T>
vec<DIM, T> operator-(const vec<DIM, T> &lhs)
{
    return lhs * T(-1);
}

// Векторное произведение
template<typename T>
vec<3, T> cross(vec<3, T> v1, vec<3, T> v2)
{
    return vec<3, T>(v1.y * v2.z - v1.z * v2.y,
                     v1.z * v2.x - v1.x * v2.z,
                     v1.x * v2.y - v1.y * v2.x);
}

// Вывод элементов вектора
template<size_t DIM, typename T>
std::ostream &operator<<(std::ostream &out, const vec<DIM, T> &v)
{
    for (unsigned int i = 0; i < DIM; i++)
    {
        out << v[i] << " ";
    }
    return out;
}
#endif // TINY_RAYTRACER_GEOMETRY_H
