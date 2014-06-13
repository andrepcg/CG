#ifndef ESTRUTURAS
#define ESTRUTURAS

enum MissionEstado
{
	WAITING_FOR_MISSION_COMPLETE,
	MISSION_COMPLETED,
	DELAY_BEFORE_MISSION,
	CREATING_MISSION
};

enum MissionBonus
{
	NONE,
	JUMP_INCREASE,
	//DAMAGE_TEMP,
	//SPEED_TEMP,
	DAMAGE_INCREASE,
	SPEED_INCREASE,
	//HEALTH_TEMP,
	HEALTH_INCREASE,
	JUMP_DROP_EXPLOSION,
	FORCE_FIELD

};

struct Bonus
{
	MissionBonus b;
	float amount;
};

struct XY
{
	float x, y;
	XY(float X, float Y) : x(X), y(Y) { }
	XY() : x(0), y(0) { }
	XY operator+(const XY& v2) const {
		return(XY(x + v2.x, y + v2.y));
	}
};

struct XYZ
{
	float x, y, z;
	XYZ(float X, float Y, float Z) : x(X), y(Y), z(Z) { }
	XYZ() : x(0), y(0), z(0) { }
	XYZ operator+(const XYZ& v2) const {
		return(XYZ(x + v2.x, y + v2.y, z + v2.z));
	}
	inline XY getXZ() { return XY(x,z); }
};

struct vec2f
{
	float x, y;
	vec2f(float X, float Y) : x(X), y(Y) { }
	vec2f() : x(0), y(0) { }
	XY operator*(const XY& v2) const {
		return(XY(x * v2.x, y * v2.y));
	}
};


struct RGBf
{
	float r, g, b;
	RGBf(float R, float G, float B) : r(R), g(G), b(B) { }
	RGBf() : r(0), g(0), b(0) { }
};

struct Circle
{
	float x, y, radius;
	Circle() : x(0), y(0), radius(0) { }
	Circle(float X, float Y, float R) : x(X), y(Y), radius(R) { }
	inline bool Contains(XY &p) { return (((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y)) <= (radius * radius)); }
};

struct Rect
{
	int x, y, w, h;
	Rect() : x(0), y(0), w(0), h(0) { }
	Rect(int X, int Y, int W, int H) : x(X), y(Y), w(W), h(H) { }

	inline bool Contains(XY& p) { return (p.x >= x && p.y >= y && p.x < x + w && p.y < y + h); }
	inline bool Intersects(Rect& r) { return !(r.x >(x + w) || (r.x + r.w) < x || r.y > (y + h) || (r.y + r.h) < y); }
	inline bool Intersects(Circle& c) { return !(c.x >(x + w) || (c.x + c.radius) < x || c.y > (y + h) || (c.y + c.radius) < y); }
	inline Rect Resize(int f) { return Rect(x + w / (f * 2), y + h / (f * 2), w / f, h / f); }
};



#endif