#ifndef PHYSICALSIMULATIONDRIVER_VEC3_H
#define PHYSICALSIMULATIONDRIVER_VEC3_H

#endif //PHYSICALSIMULATIONDRIVER_VEC3_H

template<typename T = float>
class XVector3 {
public:
    using value_type = T;

    inline XVector3() : x(0.0f), y(0.0f), z(0.0f) {}

    inline explicit XVector3(T a) : x(a), y(a), z(a) {}

    inline explicit XVector3(const T *p) : x(p[0]), y(p[1]), z(p[2]) {}

    inline XVector3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {

    }

    inline explicit operator T *() { return &x; }

    inline explicit operator const T *() const { return &x; };

    inline void Set(T x_, T y_, T z_) {
        x = x_;
        y = y_;
        z = z_;
    }

    inline XVector3<T> operator*(T scale) const {
        XVector3<T> r(*this);
        r *= scale;
        return r;
    }

    inline XVector3<T> operator/(T scale) const {
        XVector3<T> r(*this);
        r /= scale;
        return r;
    }

    inline XVector3<T> operator+(const XVector3<T> &v) const {
        XVector3<T> r(*this);
        r += v;
        return r;
    }

    inline XVector3<T> operator-(const XVector3<T> &v) const {
        XVector3<T> r(*this);
        r -= v;
        return r;
    }

    inline XVector3<T> operator/(const XVector3<T> &v) const {
        XVector3<T> r(*this);
        r /= v;
        return r;
    }

    inline XVector3<T> operator*(const XVector3<T> &v) const {
        XVector3<T> r(*this);
        r *= v;
        return r;
    }

    inline XVector3<T> &operator*=(T scale) {
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }

    inline XVector3<T> &operator/=(T scale) {
        T s(1.0f / scale);
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    inline XVector3<T> &operator+=(const XVector3<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    inline XVector3<T> &operator-=(const XVector3<T> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    inline XVector3<T> &operator/=(const XVector3<T> &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    inline XVector3<T> &operator*=(const XVector3<T> &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    inline bool operator!=(const XVector3<T> &v) const { return (x != v.x || y != v.y || z != v.z); }

    inline XVector3<T> operator-() const { return XVector3<T>(-x, -y, -z); }

    T x, y, z;
};

using Vec3 = XVector3<float>;

// lhs scalar scale
template<typename T>
XVector3<T> operator*(T lhs, const XVector3<T> &rhs) {
    XVector3<T> r(rhs);
    r *= lhs;
    return r;
}

template<typename T>
bool operator==(const XVector3<T> &lhs, const XVector3<T> &rhs) {
    return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
}

template<typename T>
typename T::value_type Dot3(const T &v1, const T &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float Dot3(const float *v1, const float *v2) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}


template<typename T>
inline T Dot(const XVector3<T> &v1, const XVector3<T> &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Vec3 Cross(const Vec3 &b, const Vec3 &c) {
    return {b.y * c.z - b.z * c.y,
                b.z * c.x - b.x * c.z,
                b.x * c.y - b.y * c.x};
}

// component wise min max functions
template<typename T>
inline XVector3<T> Max(const XVector3<T> &a, const XVector3<T> &b) {
    return XVector3<T>(Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z));
}

template<typename T>
inline XVector3<T> Min(const XVector3<T> &a, const XVector3<T> &b) {
    return XVector3<T>(Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z));
}