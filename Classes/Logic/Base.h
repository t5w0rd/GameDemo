/* 
 * File:   Base.h
 * Author: thunderliu
 *
 * Created on 2013年12月9日, 上午12:42
 */

#ifndef __BASE_H__
#define	__BASE_H__


class CPoint
{
public:
    inline CPoint(): x(0.0f), y(0.0f) {}
    inline CPoint(float x, float y): x(x), y(y) {}
    inline CPoint(const CPoint& rPos): x(rPos.x), y(rPos.y) {}
    
    inline CPoint& operator=(const CPoint& p) { x = p.x; y = p.y; return *this; }
    inline bool operator==(const CPoint& p) const { return (p.x == x) && (p.y == y); }
    inline CPoint operator+(const CPoint& p) const { return CPoint(x + p.x, y + p.y); }
    inline CPoint operator-(const CPoint& p) const { return CPoint(x - p.x, y - p.y); }
    inline CPoint operator*(float f) const { return CPoint(x * f, y * f); }
    inline CPoint operator/(float f) const { return CPoint(x / f, y / f); }
    inline CPoint& operator+=(const CPoint& p) { x += p.x; y += p.y; return *this; }
    inline CPoint& operator-=(const CPoint& p) { x -= p.x; y -= p.y; return *this; }
    inline CPoint& operator*=(float f) { x *= f; y *= f; return *this; }
    inline CPoint& operator/=(float f) { x /= f; y /= f; return *this; }
    
    inline float getDistance(const CPoint& p) const { float fDeltaX = x - p.x; float fDeltaY = y - p.y; return sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY); }
    inline float getAngle() const { return atan2f(y, x); }

public:
    float x;
    float y;
};


class CKeyGen
{
public:
    static int nextKey();
};


#endif	/* __BASE_H__ */

