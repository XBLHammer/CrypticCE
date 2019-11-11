#pragma once

#include "stdafx.h"

typedef float vec_t;

typedef struct { int r, g, b, a; }color_t;

struct Vector2 {
	float x, y;

	Vector2() {
		x = y = 0.0f;
	}

	Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	float distance(Vector2 &Vector) {
		return sqrt(distanceEx(Vector));
	}

	float distanceEx(Vector2 &Vector) {
		float _x = this->x - Vector.x, _y = this->y - Vector.y;
		return ((_x * _x) + (_y * _y));
	}
};

struct Vector3 {
	float x, y, z;

	Vector3() {
		x = y = z = 0.0f;
	}

	Vector3(float x, float y, float z) {
		this->x = x; this->y = y; this->z = z;
	}

	bool operator==(Vector3 &Vector) {
		return (this->x == Vector.x && this->y == Vector.y && this->z == Vector.z);
	}

	Vector3& operator+(Vector3 &Vector) {
		return Vector3(this->x + Vector.x, this->y + Vector.y, this->z + Vector.z);
	}

	Vector3& operator-(Vector3 &Vector) {
		return Vector3(this->x - Vector.x, this->y - Vector.y, this->z - Vector.z);
	}

	float distance(Vector3 &Vector) {
		return sqrt(distanceEx(Vector));
	}

	float distanceEx(Vector3 &Vector) {
		float _x = this->x - Vector.x, _y = this->y - Vector.y, _z = this->z - Vector.z;
		return ((_x * _x) + (_y * _y) + (_z * _z));
	}

	float dotProduct(Vector3 &Vector) {
		return (this->x * Vector.x) + (this->y * Vector.y) + (this->z * Vector.z);
	}
};

struct Vector4 {
	float a, b, c, d;

	Vector4() {
		a = b = c = d = 0.0f;
	}

	Vector4(float a, float b, float c, float d) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}
};

#define M_PI       3.14159265358979323846
#define GET_ORIGIN(VecOut,pMatrix) {VecOut.x = pMatrix._41;VecOut.y = pMatrix._42;VecOut.z = pMatrix._43;};
#define GET_X(VecOut,pMatrix) {D3DXVec3Normalize(&VecOut,&D3DXVECTOR3(pMatrix._11,pMatrix._12,pMatrix._13));};
#define GET_Y(VecOut,pMatrix) {D3DXVec3Normalize(&VecOut,&D3DXVECTOR3(pMatrix._21,pMatrix._22,pMatrix._23));};
#define GET_Z(VecOut,pMatrix) {D3DXVec3Normalize(&VecOut,&D3DXVECTOR3(pMatrix._31,pMatrix._32,pMatrix._33));};

#define DotProduct(x,y)			((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define VectorSubtract(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(a,b,c)		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define VectorCopy4(a,b)		((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])
#define	VectorScale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	VectorMA(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))
#define VectorCmp(a,b)			(((a)[0]==(b)[0])&&((a)[1]==(b)[1])&&((a)[2]==(b)[2]))

#define VALID( x ) ( x != NULL && HIWORD( x ) )
#define AngleToShort(X) ((int)((X) * 65536 / 360) & 65535)

typedef float vec_t;

class Vector
{
public:
	// Construction/destruction
	inline Vector(void) { }
	inline Vector(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	inline Vector(double X, double Y, double Z) { x = (float)X; y = (float)Y; z = (float)Z; }
	inline Vector(int X, int Y, int Z) { x = (float)X; y = (float)Y; z = (float)Z; }
	inline Vector(const Vector& v) { x = v.x; y = v.y; z = v.z; }
	inline Vector(float rgfl[3]) { x = rgfl[0]; y = rgfl[1]; z = rgfl[2]; }

	// Operators
	inline Vector operator-(void) const { return Vector(-x, -y, -z); }
	inline int operator==(const Vector& v) const { return x == v.x && y == v.y && z == v.z; }
	inline int operator!=(const Vector& v) const { return !(*this == v); }
	inline Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
	inline Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
	inline Vector operator*(float fl) const { return Vector(x*fl, y*fl, z*fl); }
	inline Vector operator/(float fl) const { return Vector(x / fl, y / fl, z / fl); }

	// Methods
	inline void CopyToArray(float* rgfl) const { rgfl[0] = x, rgfl[1] = y, rgfl[2] = z; }
	inline float Length(void) const { return (float)sqrt(x*x + y*y + z*z); }
	operator float *() { return &x; } // Vectors will now automatically convert to float * when needed
	operator const float *() const { return &x; } // Vectors will now automatically convert to float * when needed
	inline Vector Normalize(void) const
	{
		float flLen = Length();
		if (flLen == 0) return Vector(0, 0, 1); // ????
		flLen = 1 / flLen;
		return Vector(x * flLen, y * flLen, z * flLen);
	}

	inline float Length2D(void) const { return (float)sqrt(x*x + y*y); }

	// Members
	vec_t x, y, z;
};
inline Vector operator*(float fl, const Vector& v) { return v * fl; }
