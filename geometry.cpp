#include "geometry.h"

// Определения специализаций
template <> template <>
Vec3<int>::Vec3(const Vec3<float> &v) : x(int(v.x+.5f)), y(int(v.y+.5f)), z(int(v.z+.5f)) {}

template <> template <>
Vec3<float>::Vec3(const Vec3<int> &v) : x(v.x), y(v.y), z(v.z) {}

// Общее определение шаблонного конструктора преобразования
template <class t> template <class u>
Vec3<t>::Vec3(const Vec3<u> &v) : x(v.x), y(v.y), z(v.z) {}