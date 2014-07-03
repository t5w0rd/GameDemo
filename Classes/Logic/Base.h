/* 
 * File:   Base.h
 * Author: thunderliu
 *
 * Created on 2013年12月9日, 上午12:42
 */

#ifndef __BASE_H__
#define	__BASE_H__


class CDebugLog
{
public:
    static void Log(const char* pFormat, ...);
};

#define LOG(...) CDebugLog::Log(__VA_ARGS__)

class CPoint
{
public:
    inline CPoint(): x(0.0f), y(0.0f) {}
    inline CPoint(float x, float y): x(x), y(y) {}
    inline CPoint(const CPoint& rPos): x(rPos.x), y(rPos.y) {}
    
    inline CPoint& operator=(const CPoint& p) { x = p.x; y = p.y; return *this; }
    inline bool operator==(const CPoint& p) const { return (p.x == x) && (p.y == y); }
    inline bool operator!=(const CPoint& p) const { return (p.x != x) || (p.y != y); }
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
    inline CPoint getDirectionPoint(float radian, float distance) const { return CPoint(x + cos(-radian) * distance, y + sin(radian) * distance); }
    inline CPoint getForwardPoint(const CPoint& p, float distance) const { float a = (p - *this).getAngle(); return getDirectionPoint(a, distance); }

public:
    float x;
    float y;
};


class CIdGen
{
public:
    static int nextId();
};


#endif	/* __BASE_H__ */

