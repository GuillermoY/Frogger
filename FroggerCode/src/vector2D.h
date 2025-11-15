#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>

/**
 * Vector bidimensional genérico.
 */
template<typename T = float>
class Vector2D
{
	T x, y;

public:
	Vector2D(T x, T y) : x(x), y(y) { }
	Vector2D() : Vector2D(0, 0) { }

	// Coordenadas del vector
	const T& getX() const { return x; }
	const T& getY() const { return y; }

	// Operadores
	Vector2D operator+(const Vector2D& otro) const {
		return {x + otro.x, y + otro.y};
	}
	Vector2D operator-(const Vector2D& otro) const {
		return { x - otro.x, y - otro.y };
	}
	Vector2D operator*(double d) const {
		Vector2D r;
		r.x = x * d;
		r.y = y * d;
		return r;
	} 
	double operator*(const Vector2D& d) const {
		return d.x * x + d.y * y;
	}

	// Operadores de entrada/salida
	friend std::ostream& operator<<(std::ostream& out, const Vector2D& v) {
		return out << '{' << v.x << ", " << v.y << '}';
	}
};
using Point2D = Vector2D<>;
#endif // VECTOR2D_H
