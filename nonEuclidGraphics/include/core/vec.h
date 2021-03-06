#pragma once


#include <assert.h>
#include <ostream>
#include <math.h>
#include <initializer_list>

namespace cgcore
{
	template<typename T, size_t N>
	struct vec
	{
		T* const data = new T[N];

		vec() { }
		vec(const T* _data)
		{
			memcpy(data, _data, sizeof(T) * N);
		}
		T& operator [](const size_t n) const { assert(n < N && "Index out of range"); return data[n]; }

		vec(const vec<T, N>& v) {
			memcpy(data, v.data, sizeof(T) * N);
		}

		vec(const std::initializer_list<T> elem_list)
		{
			assert(elem_list.size() <= N && "Initializer list is too long");
			size_t i = 0;
			for (auto it : elem_list)
				data[i++] = it;
		}

		vec(const T v)
		{
			for (T* p = data, *pend = data + N; p != pend; p++)
				*p = v;
		}

		template<size_t N1, size_t N2>
		vec(const vec<T, N1>& v1, const vec<T, N2>& v2)
		{
			static_assert(N1 + N2 == N, "Cannot combine two vecs");
			memcpy(data, v1.data, sizeof(T) * N1);
			memcpy(data + N1, v2.data, sizeof(T) * N2);
		}
		
		template<size_t N1>
		vec(const vec<T, N1>& v1, const T v2)
		{
			static_assert(N1 + 1 == N, "Cannot combine two vec");
			memcpy(data, v1.data, sizeof(T) * N1);
			data[N1] = v2;
		}

		const vec<T, N>& operator=(const vec<T, N>& v)
		{
			memcpy(data, v.data, sizeof(T)*N);
			return *this;
		}

		vec<T, N> operator+(const vec<T, N>& v2) const
		{
			vec<T, N>  v3;
			for (size_t i = 0; i < N; i++)
				v3[i] = data[i] + v2.data[i];
			return v3;
		}
		vec<T, N> operator-(const vec<T, N>& v2) const
		{
			vec<T, N>  v3;
			for (size_t i = 0; i < N; i++)
				v3[i] = data[i] - v2[i];
			return v3;
		}
		vec<T, N> operator*(const T v2) const
		{
			vec<T, N> v3;
			for (size_t i = 0; i < N; i++)
				v3[i] = data[i] * v2;
			return v3;
		}
		vec<T, N> operator/(const T v2) const
		{
			vec<T, N> v3;
			for (size_t i = 0; i < N; i++)
				v3[i] = data[i] / v2;
			return v3;
		}
		vec<T, N> operator*(const vec<T, N>& v2) const
		{
			vec<T, N> v3;
			for (size_t i = 0; i < N; i++)
				v3[i] = data[i] * v2[i];
			return v3;
		}

		void fill(const T v)
		{
			for (T* p = data, *pend = data + N; p != pend; p++)
				*p = v;
		}

		T norm2() const
		{
			T ret = static_cast<T>(0);
			for (size_t i = 0; i < N; i++)
				ret += data[i] * data[i];
			return ret;
		}
		T norm() const
		{
			return sqrt(norm2());
		}
		vec<T, N> normalize() const
		{
			T nrm = norm();
			vec<T, N> ret;
			for (size_t i = 0; i < N; i++)
				ret[i] = data[i] / nrm;
			return ret;
		}

		static const vec<T, 3> cross(vec<T, 3> v1, vec<T, 3>v2)
		{
			return { v1.data[1] * v2.data[2] - v1.data[2] * v2.data[1],
				v1.data[2] * v2.data[0] - v1.data[0] * v2.data[2],
				v1.data[0] * v2.data[1] - v1.data[1] * v2.data[0]
			};
		}

		static const T dot(const vec<T, N>& v1, const vec<T, N>& v2)
		{
			T ret = static_cast<T>(0);
			for (size_t i = 0; i < N; i++)
				ret += v1[i] * v2[i];
			return ret;
		}

		template<typename T2>
		vec<T2, N> cast_to_elemtype() const
		{
			vec<T2, N> ret;
			for (size_t i = 0; i < N; i++)
				ret.data[i] = static_cast<T2>(data[i]);
			return ret;
		}

		~vec() { delete[] data; }

	};
	
	template<typename T, size_t N>
	std::ostream& operator << (std::ostream& ostrm, const vec<T, N>& v)
	{
		for (size_t i = 0; i < N; i++)
			ostrm << v[i] << ", ";
		return ostrm;
	}

	//对vecf3部分成员函数的模板特例化

	template<>
	inline const float vec<float, 3>::dot(const vec<float, 3>& v1, const vec<float, 3>& v2)
	{
		return v1.data[0] * v2.data[0] + v1.data[1] * v2.data[1] + v1.data[2] * v2.data[2];
	}

	template<>
	inline vec<float, 3> vec<float, 3>::operator+(const vec<float, 3>& v2) const
	{
		vec<float, 3> ret;
		ret.data[0] = data[0] + v2.data[0];
		ret.data[1] = data[1] + v2.data[1];
		ret.data[2] = data[2] + v2.data[2];
		return ret;
	}

	template<>
	inline vec<float, 3> vec<float, 3>::operator-(const vec<float, 3>& v2) const
	{
		vec<float, 3> ret;
		ret.data[0] = data[0] - v2.data[0];
		ret.data[1] = data[1] - v2.data[1];
		ret.data[2] = data[2] - v2.data[2];
		return ret;
	}
	template<>
	inline vec<float, 3> vec<float, 3>::operator*(const float v2) const
	{
		vec<float, 3> ret;
		ret.data[0] = data[0] * v2;
		ret.data[1] = data[1] * v2;
		ret.data[2] = data[2] * v2;
		return ret;
	}
	template<>
	inline vec<float, 3> vec<float, 3>::operator/(const float v2) const
	{
		vec<float, 3> ret;
		ret.data[0] = data[0] / v2;
		ret.data[1] = data[1] / v2;
		ret.data[2] = data[2] / v2;
		return ret;
	}
	//对vecf2部分成员函数的模板特例化

	template<>
	inline const float vec<float, 2>::dot(const vec<float, 2>& v1, const vec<float, 2>& v2)
	{
		return v1.data[0] * v2.data[0] + v1.data[1] * v2.data[1];
	}

	template<>
	inline vec<float, 2> vec<float, 2>::operator+(const vec<float, 2>& v2) const
	{
		vec<float, 2> ret;
		ret.data[0] = data[0] + v2.data[0];
		ret.data[1] = data[1] + v2.data[1];
		return ret;
	}

	template<>
	inline vec<float, 2> vec<float, 2>::operator-(const vec<float, 2>& v2) const
	{
		vec<float, 2> ret;
		ret.data[0] = data[0] - v2.data[0];
		ret.data[1] = data[1] - v2.data[1];
		return ret;
	}
	template<>
	inline vec<float, 2> vec<float, 2>::operator*(const float v2) const
	{
		vec<float, 2> ret;
		ret.data[0] = data[0] * v2;
		ret.data[1] = data[1] * v2;
		return ret;
	}
	template<>
	inline vec<float, 2> vec<float, 2>::operator/(const float v2) const
	{
		vec<float, 2> ret;
		ret.data[0] = data[0] / v2;
		ret.data[1] = data[1] / v2;
		return ret;
	}

	//需要的时候再在这里特例化模板以加速

	//...

	typedef vec<float, 2> vecf2;
	typedef vec<float, 3> vecf3;
	typedef vec<float, 4> vecf4;

}

#include <core/vec.inl>