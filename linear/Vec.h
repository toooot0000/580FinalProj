//
// Created by Jerry Ye on 2021/12/4.
//


#ifndef CG_VEC_H
#define CG_VEC_H

#include <ostream>
#include "string"
#include "initializer_list"
#include "cmath"
#include "sstream"
#include "array"
#include "algorithm"

template<typename T, size_t M, size_t N> requires (M > 0 && N > 0)
class Mat;

template<typename T, size_t N> requires (N > 0)
class Vec
{
protected:
    std::array<T, N> arr;

public:
    /**
     * Constructors
     */
    Vec();

    explicit Vec(const std::array<T, N> &nums);

    Vec(std::initializer_list<T> initList);

    Vec(const Vec &other);

    template<size_t M>
    explicit Vec(const Vec<T, M> &other);

    Vec(Vec &&other)
    noexcept;

    template<typename...Ts, size_t M>
    requires (M < N)
    explicit Vec(const Vec<T, M> &other, Ts...rest);

    template<typename...Ts>
    explicit Vec(Ts...nums);

    Vec &operator=(const Vec &other);

    Vec &operator=(Vec &&other)
    noexcept;

    /**
     * Dot production
     * @param other
     * @return
     */
    T dot(const Vec<T, N> &other) const;

    /**
     * v1 + v2
     * @param other
     * @return
     */
    Vec<T, N> plus(const Vec<T, N> &other) const;

    /**
     * getLength
     * @return
     */
    [[nodiscard]] double length() const;

    /**
     * get normalized
     * @return
     */
    [[nodiscard]] Vec<double, N> normalized() const;

    /*
     * get negative();
     */
    [[nodiscard]] Vec<T, N> negative() const;


    /**
     * Translate
     * @param offset
     */
    void translate(const Vec<T, N> &offset);

    template<typename U>
    void multiple(U k);

    template<typename U>
    Vec<T, N> multipled(U k) const;

    virtual Vec<T, N> cross(const Vec<T, N> &other) const;

    /**
     * get printable string
     * @return
     */
    [[nodiscard]] std::string toString() const;


    Vec &operator+=(const Vec<T, N> &other);

    Vec &operator-=(const Vec<T, N> &other);

    T &operator[](size_t ind);

    T operator[](size_t ind) const;

    template<typename U>
    friend inline Vec operator*(U k, const Vec<T, N> &other)
    {
        return other.multipled(k);
    }


    friend Vec operator+(const Vec<T, N> &l, const Vec<T, N> &other)
    {
        return l.plus(other);
    };

    friend Vec operator-(const Vec<T, N> &l, const Vec<T, N> &other)
    {
        return l.plus(other.negative());
    };

    friend std::ostream &operator<<(std::ostream &os, const Vec &vec)
    {
        os << vec.toString();
        return os;
    }

    inline Mat<T, N, 1> toMat() const
    {
        return Mat<T, N, 1>(arr);
    }


    inline T getX() const
    {
        return arr[0];
    }


    inline T getY() const
    {
        assert(N > 1);
        return arr[1];
    }


    inline T getZ() const
    {
        assert(N > 2);
        return arr[2];
    }

};


/**
     * Constructors
     *
     */
template<typename T, size_t N>
requires (N > 0)
Vec<T, N>::Vec()
{
    for (auto &i: arr)
    {
        i = 0;
    }
}

template<typename T, size_t N>
requires (N > 0)
Vec<T, N>::Vec(const std::array<T, N> &nums)
{
    for (size_t i = 0; i < std::min(std::size(nums), N); i++)
    {
        arr[i] = nums[i];
    }
}

template<typename T, size_t N>
requires (N > 0)
Vec<T, N>::Vec(std::initializer_list<T>
               initList)
{
    for (auto i = initList.begin(); i != initList.end() && std::distance(initList.begin(), i) < N; ++i)
    {
        arr[std::distance(initList.begin(), i)] = *i;
    }
};

template<typename T, size_t N>
requires (N > 0)
Vec<T, N>::Vec(const Vec &other)
{
    std::copy(other.arr.begin(), other.arr.end(), arr.begin());
}

template<typename T, size_t N>
requires (N > 0)
Vec<T, N>::Vec(Vec &&other)
noexcept
{
    arr = std::move(other.arr);
}

template<typename T, size_t N>
requires (N > 0)
Vec<T, N> &Vec<T, N>::operator=(const Vec &other)
{
    std::copy(other.arr.begin(), other.arr.end(), arr.begin());
    return *this;
}

template<typename T, size_t N>
requires (N > 0)
Vec<T, N> &Vec<T, N>::operator=(Vec &&other)
noexcept
{
    arr = std::move(other.arr);
    return *this;
}

/**
 * Dot production
 * @param other
 * @return
 */template<typename T, size_t N>
requires (N > 0)
T Vec<T, N>::dot(const Vec<T, N> &other) const
{
    T ret = 0;
    for (size_t i = 0; i < N; ++i)
    {
        ret += arr[i] * other.arr[i];
    }
    return ret;
}

/**
 * v1 + v2
 * @param other
 * @return
 */
template<typename T, size_t N>
requires (N > 0)
Vec<T, N> Vec<T, N>::plus(const Vec<T, N> &other) const
{
    std::array<T, N> newArr{};
    for (size_t i = 0; i < N; ++i)
    {
        newArr[i] = arr[i] + other.arr[i];
    }
    return Vec<T, N>(newArr);
}

/**
 * getLength
 * @return
 */
template<typename T, size_t N>
requires (N > 0)
double Vec<T, N>::length() const
{
    double ret = 0;
    for (auto &i: arr)
    {
        ret += std::pow(i, 2);
    }
    return std::sqrt(ret);
}

/**
 * get normalized
 * @return
 */
template<typename T, size_t N>
requires (N > 0)
[[nodiscard]] Vec<double, N> Vec<T, N>::normalized() const
{
    double len = length();
    std::array<T, N> newArr;
    for (size_t i = 0; i < N; ++i)
    {
        newArr[i] = arr[i] / len;
    }
    return Vec<double, N>(newArr);
}

/*
 * get negative();
 */
template<typename T, size_t N>
requires (N > 0)
[[nodiscard]] Vec<T, N> Vec<T, N>::negative() const
{
    std::array<T, N> newArr{};
    for (size_t i = 0; i < N; ++i)
    {
        newArr[i] = -arr[i];
    }
    return Vec<T, N>(newArr);
}


/**
 * Translate
 * @param offset
 */
template<typename T, size_t N>
requires (N > 0)
void Vec<T, N>::translate(const Vec<T, N> &offset)
{
    for (size_t i = 0; i < N; ++i)
    {
        arr[i] += offset.arr[i];
    }
}

template<typename T, size_t N> requires (N > 0)

template<typename U>
void Vec<T, N>::multiple(U k)
{
    for (auto &i: arr)
    {
        i *= k;
    }
}

template<typename T, size_t N>
requires (N > 0)
Vec<T, N> Vec<T, N>::cross(const Vec<T, N> &other)
const
{
    assert(N == 3);
    T a1 = arr[0], a2 = arr[1], a3 = arr[2];
    T b1 = other[0], b2 = other[1], b3 = other[2];
    return Vec{a2 * b3 - a3 * b2, a3 * b1 - a1 * b3, a1 * b2 - a2 * b1};
};

/**
 * get printable string
 * @return
 */
template<typename T, size_t N>
requires (N > 0)
[[nodiscard]] std::string Vec<T, N>::toString() const
{
    std::stringstream ss;
    ss << "[";
    ss << arr[0];
    for (auto i = std::begin(arr) + 1; i != std::end(arr); ++i)
    {
        ss << ", " << *i;
    }
    ss << "]";
    return ss.str();
}

template<typename T, size_t N>
requires (N > 0)
Vec<T, N> &Vec<T, N>::operator+=(const Vec<T, N> &other)
{
    translate(other);
    return *this;
}

template<typename T, size_t N>
requires (N > 0)
Vec<T, N> &Vec<T, N>::operator-=(const Vec<T, N> &other)
{
    translate(other.negative());
    return *this;
}

template<typename T, size_t N>
requires (N > 0)
T &Vec<T, N>::operator[](size_t ind)
{
    return arr[ind];
}

template<typename T, size_t N>
requires (N > 0)
T Vec<T, N>::operator[](size_t ind) const
{
    return arr[ind];
}

template<typename T, size_t N> requires (N > 0)

template<typename U>
Vec<T, N> Vec<T, N>::multipled(U k) const
{
    Vec<T, N> ret(arr);
    for (int i = 0; i < N; ++i)
    {
        ret[i] = static_cast<T>(ret[i] * k);
    }
    return ret;
}


template<typename T, size_t N> requires (N > 0)

template<size_t M>
Vec<T, N>::Vec(const Vec<T, M> &other)
{
    for (int i = 0; i < (M < N ? M : N); ++i)
    {
        arr[i] = other[i];
    }
}

template<typename T, size_t N> requires (N > 0)

template<typename... Ts>
Vec<T, N>::Vec(Ts...nums)
{
    assert(sizeof...(nums) == N);
    int i = 0;
    int dummy[sizeof...(Ts)] = {(arr[i++] = nums, 0)...};
}


template<typename T, size_t N> requires (N > 0)

template<typename...Ts, size_t M>
requires (M < N)
Vec<T, N>::Vec(const Vec<T, M> &other, Ts...rest)
{
    auto t = sizeof...(rest);
    assert(t + M == N);
    size_t i = 0;
    for (; i < M; i++)
    {
        arr[i] = other[i];
    }
    int dummy[sizeof...(Ts)] = {(arr[i++] = rest, 0)...};
}


template<size_t N>
static Vec<int, N> round(const Vec<double, N> &vec)
{
    Vec<int, N> ret;
    for (int i = 0; i < N; i++)
    {
        ret[i] = static_cast<int>(std::lround(vec[i]));
    }
    return ret;
}

typedef Vec<int, 2> iVec2;
typedef Vec<double, 3> Vec3;
typedef Vec<int, 3> iVec3;
typedef Vec<double, 4> Vec4;

#endif //CG_VEC_H
