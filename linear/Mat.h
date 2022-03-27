//
// Created by Jerry Ye on 2021/12/4.
//

#ifndef CG_MAT_H
#define CG_MAT_H

#include "array"
#include "Vec.h"
#include "string"
#include "sstream"
#include "initializer_list"
#include <numbers>


template<typename T, size_t M, size_t N> requires (M > 0 && N > 0)
class Mat
{
private:
    const size_t m = M;
    const size_t n = N;
    std::array<std::array<T, N>, M> mat;

public:

    Mat();

    Mat(const Mat &other);

    Mat(Mat &&other) noexcept;

    explicit Mat(const std::array<std::array<T, N>, M> &matInput);

    explicit Mat(const T matInput[M][N]);

    explicit Mat(const std::array<T, M * N> &matInput);

    explicit Mat(const T matInput[M * N]);

    Mat(std::initializer_list<T> initList);

    inline std::array<T, N> &operator[](size_t i)
    {
        return mat[i];
    }

    inline const std::array<T, N> &operator[](size_t i) const
    {
        return mat[i];
    }

    /**
     * Mat * Vec
     * @param vec
     * @return
     */
    Vec<T, M> rightMulti(const Vec<T, N> &vec) const;

    [[nodiscard]] std::string toString() const;

    friend std::ostream &operator<<(std::ostream &os, const Mat &matPrinted)
    {
        os << matPrinted.toString();
        return os;
    }

    /**
     * Vec * Mat
     * @param vec
     * @return
     */
    Vec<T, N> leftMulti(const Vec<T, M> &vec) const;


    template<size_t K>
    requires (K > 0)
    Mat<T, M, K> rightMulti(const Mat<T, N, K> &matB) const;


    template<size_t K>
    requires (K > 0)
    Mat<T, K, N> leftMulti(const Mat<T, K, M> &matB) const;

    Mat<T, N, M> transposed() const;

    template<size_t P, size_t Q>
    requires (P < M && Q < N)
    Mat<T, P, Q> subMat(size_t i, size_t j) const;

    Mat<T, M - 1, N - 1> remainMat(size_t i, size_t j) const;

    [[nodiscard]] float determinant() const;

    /**
     * Return the card of the matrix
     * @return
     */
//
    size_t card() { return 0; };


    template<size_t K>
    friend Mat<T, M, K> operator*(const Mat<T, M, N> &mat1, const Mat<T, N, K> &mat2)
    {
        return mat1.rightMulti(mat2);
    };

    friend Vec<T, M> operator*(const Mat<T, M, N> &mat1, const Vec<T, N> &vec)
    {
        return mat1.rightMulti(vec);
    };

    Mat &operator=(const Mat &other);

    Mat &operator=(Mat &&other) noexcept;


};

template<typename T>
class Mat<T, 1, 1>
{
private:
    T value;
public:
    Mat() = default;

    explicit Mat(T value) : value(value) {}

    explicit Mat(T values[1]) : value(values[0]) {}

    explicit Mat(std::array<T, 1> arr) : value(arr[0]) {}

    Mat(std::initializer_list<T> initList) : value(*(initList.begin())) {}

    float determinant() const
    {
        return value;
    }
};

typedef Mat<float, 3, 3> Mat3;
typedef Mat<float, 4, 4> Mat4;


template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Mat<T, M, N>::Mat()
{
    for (auto &row: mat)
    {
        for (auto &i: row)
        {
            i = 0;
        }
    }
}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Mat<T, M, N>::Mat(const std::array<std::array<T, N>, M> &matInput)
{
    for (size_t i = 0; i != M; ++i)
    {
        std::copy(matInput[i].begin(), matInput[i].end(), mat[i].begin());
    }
}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Mat<T, M, N>::Mat(const T matInput[M][N])
{
    for (size_t i = 0; i != M; ++i)
    {
        for (size_t j = 0; j != N; ++j)
        {
            mat[i][j] = matInput[i][j];
        }
    }
}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Mat<T, M, N>::Mat(const std::array<T, M * N> &matInput)
{
    for (auto i = matInput.begin(); i != matInput.end(); ++i)
    {
        auto k = std::distance(matInput.begin(), i);
        if (k == M * N)
        {
            break;
        }
        mat[k / N][k % N] = *i;
    }
}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Mat<T, M, N>::Mat(const T matInput[M * N])
{
    std::array<T, M * N> arrayInput(matInput);
    for (auto i = arrayInput.begin(); i != arrayInput.end(); ++i)
    {
        auto k = std::distance(arrayInput.begin(), i);
        if (k == M * N)
        {
            break;
        }
        mat[k / N][k % N] = *i;
    }
}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Mat<T, M, N>::Mat(std::initializer_list<T> initList)
{
    assert(initList.size() == M * N);
    for (auto i = initList.begin(); i != initList.end(); ++i)
    {
        auto k = std::distance(initList.begin(), i);
        mat[k / N][k % N] = *i;
    }
}


/**
 * Mat * Vec
 * @param vec
 * @return
 */
template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Vec<T, M> Mat<T, M, N>::rightMulti(const Vec<T, N>& vec) const
{
    Vec<T, M> ret;

    for (size_t i = 0; i != M; i++)
    {
        for (size_t j = 0; j != N; j++)
        {
            ret[i] += mat[i][j] * vec[j];
        }
    }

    return ret;
}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
[[nodiscard]] std::string Mat<T, M, N>::toString() const
{
    using namespace std;
    stringstream ss;
    ss << "[\n";
    for (const auto &row: mat)
    {
        ss << "[" << row[0];
        for (size_t i = 1; i < row.size(); ++i)
        {
            ss << ", " << row[i];
        }
        ss << "]\n";
    }
    ss << "]";
    return ss.str();
}

/**
 * Vec * Mat
 * @param vec
 * @return
 */

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Vec<T, N> Mat<T, M, N>::leftMulti(const Vec<T, M> &vec) const
{
    Vec<T, N> ret;

    for (size_t i = 0; i != N; i++)
    {
        for (size_t j = 0; j != M; j++)
        {
            ret[i] += mat[j][i] * vec[j];
        }
    }
    return ret;
}


template<typename T, size_t M, size_t N> requires (M > 0 && N > 0)

template<size_t K>
requires (K > 0)
Mat<T, M, K> Mat<T, M, N>::rightMulti(const Mat<T, N, K> &matB) const
{
    Mat<T, M, K> ret;
    for (size_t i = 0; i != M; ++i)
    {
        for (size_t j = 0; j != K; ++j)
        {
            for (size_t k = 0; k != N; ++k)
            {
                ret[i][j] += mat[i][k] * matB[k][j];
            }
        }
    }
    return ret;
}


template<typename T, size_t M, size_t N> requires (M > 0 && N > 0)

template<size_t K>
requires (K > 0)
Mat<T, K, N> Mat<T, M, N>::leftMulti(const Mat<T, K, M> &matB) const
{
    Mat<T, K, M> ret;
    for (size_t i = 0; i != K; ++i)
    {
        for (size_t j = 0; j != M; ++j)
        {
            for (size_t k = 0; k != N; ++k)
            {
                ret[i][j] += mat[i][k] * matB[k][j];
            }
        }
    }
    return ret;
}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Mat<T, N, M> Mat<T, M, N>::transposed() const
{
    Mat<T, N, M> ret;
    for (size_t i = 0; i != M; ++i)
    {
        for (size_t j = 0; j != N; ++j)
        {
            ret[j][i] = mat[i][j];
        }
    }
    return ret;
}

template<typename T, size_t M, size_t N> requires (M > 0 && N > 0)

template<size_t P, size_t Q>
requires (P < M && Q < N)
Mat<T, P, Q> Mat<T, M, N>::subMat(size_t i, size_t j) const
{
    Mat<T, P, Q> ret;
    for (size_t ii = 0; ii != P; ++ii)
    {
        for (size_t jj = 0; jj != Q; ++jj)
        {
            ret[ii][jj] = mat[i + ii][j + jj];
        }
    }
    return ret;
}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Mat<T, M - 1, N - 1> Mat<T, M, N>::remainMat(size_t i, size_t j) const
{
    std::array<T, (M - 1) * (N - 1)> ret{};
    size_t cur = 0;
    for (size_t ii = 0; ii != M; ++ii)
    {
        for (size_t jj = 0; jj != N; ++jj)
        {
            if (ii != i && jj != j)
            {
                ret[cur++] = mat[ii][jj];
            }
        }
    }
    return Mat<T, M - 1, N - 1>(ret);
}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Mat<T, M, N>::Mat(Mat &&other) noexcept :mat(std::move(other.mat)) {}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Mat<T, M, N>::Mat(const Mat &other)
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            mat[i][j] = other[i][j];
        }
    }
}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Mat<T, M, N> &Mat<T, M, N>::operator=(const Mat &other)
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            mat[i][j] = other[i][j];
        }
    }
    return *this;
}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
Mat<T, M, N> &Mat<T, M, N>::operator=(Mat &&other)
noexcept
{
    mat = std::move(other.mat);
    return *this;
}

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
float Mat<T, M, N>::determinant() const
{
    static_assert(M == N, "Only square matrices have determinant!");
#if M == 2
    return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
#else
    float res = 0;
    for (size_t i = 0; i != M; ++i)
    {
        res += (i & 1 ? -1 : 1) * mat[0][i] * remainMat(0, i).determinant();
    }
    return res;
#endif
};


/**
 *
 * Return the card of the matrix
 * @return
 */

template<typename T, size_t M, size_t N>
requires (M > 0 && N > 0)
size_t card() { return 0; };


static inline Mat4 makeViewportTrans(int nx, int ny)
{
    auto _nx = static_cast<float>(nx), _ny = static_cast<float>(ny);
    return Mat4({
            _nx / 2.0f, 0, 0, (_nx) / 2.0f,
            0, _ny / 2.0f, 0, (_ny) / 2.0f,
            0, 0, 1, 0,
            0, 0, 0, 1});
}

static inline Mat4 makeOrthographicProjectTrans(float l, float b, float n, float r, float t, float f)
{
    assert(r != l && t != b && f < n);
    return Mat4({2.0f / (r - l), 0, 0,
                 -(r + l) / (r - l), 0, 2.0f / (t - b), 0,
                 -(t + b) / (t - b), 0, 0, 2.0f / (n - f),
                 -(n + f) / (n - f), 0, 0, 0, 1});
}

static inline Mat4 makePerspectiveProjectTrans(float l = -1, float b = -1, float n = -15, float r=1, float t=1, float f=-(float)INT_MAX)
{
    return Mat4({
                        2.0f * n / (r - l), 0, (r + l) / (l - r), 0,
                        0, 2.0f * n / (t - b), (t + b) / (b - t), 0,
                          0, 0, (n + f) / (n - f), 2.0f * n * f / (f - n),
                        0, 0, 1, 0});
}


static inline Mat4 makeCameraTrans(const Vec3 &eye, const Vec3 &gaze, const Vec3 &t)
{
//    assert(t.length() != 0 && gaze.dot(t) == 0);
    Vec3 u, v, w;
    w = gaze.normalized().negative();
    u = w.cross(t).normalized();
    v = w.cross(u).normalized();
    return Mat4({
                u.getX(), u.getY(), u.getZ(), -u.dot(eye),
                v.getX(), v.getY(), v.getZ(), -v.dot(eye),
                w.getX(), w.getY(), w.getZ(), -w.dot(eye),
                0, 0, 0, 1});
}

static inline Mat4 makeTranslateTrans(float x, float y, float z)
{
    return Mat4({
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    });
}

static inline Mat4 makeScaleTrans(float k){
    return Mat4({
        k, 0, 0, 0,
        0, k, 0, 0,
        0, 0, k, 0,
        0, 0, 0, 1
    });
}

static inline Mat4 makeXRotationTrans(float deg){
    using namespace std;
    float rad = deg/180.f * std::numbers::pi_v<float>;
    return {
        1, 0, 0, 0,
        0, cos(rad), -sin(rad), 0,
        0, sin(rad), cos(rad), 0,
        0, 0, 0, 1
    };
}

static inline Mat4 makeYRotationTrans(float deg){
    using namespace std;
    float rad = deg/180.f * std::numbers::pi_v<float>;
    return {
            cos(rad), 0, sin(rad), 0,
            0, 1, 0, 0,
            -sin(rad), 0, cos(rad), 0,
            0, 0, 0, 1
    };
}


static inline Mat4 makeZRotationTrans(float deg){
    using namespace std;
    float rad = deg/180.f * std::numbers::pi_v<float>;
    return {
            cos(rad), -sin(rad), 0, 0,
            sin(rad), cos(rad), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
}

static inline Mat4 makeNormTrans(const Mat4 &ori){
    const auto &row = ori[0];
    float k = std::sqrt(row[0]*row[0] + row[1]*row[1] + row[2]*row[2]);
    return {
        ori[0][0]/k, ori[0][1]/k, ori[0][2]/k, 0,
        ori[1][0]/k, ori[1][1]/k, ori[1][2]/k, 0,
        ori[2][0]/k, ori[2][1]/k, ori[2][2]/k, 0,
        0, 0, 0, 1
    };
}


#endif //CG_MAT_H
