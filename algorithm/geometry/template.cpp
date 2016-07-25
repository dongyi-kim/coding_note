#include<cmath>
#include<algorithm>
#include<iostream>

using namespace std;

typedef long long coo;				//좌표계 형
const double Pi  = acos(0.0)*2.0;	//PI : 3.14159....
const coo EPS = 1e-9;				//오차 한계

#define ABS(x) 		  ((x)>=0 ? (x) : -(x))
#define equals(a, b)  (ABS(a-b) <= EPS)


/** Point **/
struct point{
    coo x, y;
    point(coo x = 0, coo y = 0) : x(x), y(y) {}
    //comparator
    bool operator < (const point & op) const {
        return equals(x, op.x) ? y < op.y : x < op.x;
    }
    bool operator == (const point& op) const {
        return equals(x , op.x) && equals(y , op.y);
    }
    
    // vector operation
    point operator + (const point & op) const{
        return point(x + op.x, y + op.y);
    }
    point operator - (const point & op) const{
        return point(x - op.x, y - op.y);
    }
    coo operator * (const point & op) const
    {	 //dot product
        return (x*op.x) + (y*op.y);
    }
    coo sqrlen() const {
        return x*x + y*y;
    }
    double length() const {
        return sqrt(sqrlen());
    }
};

inline coo ccw(const point &a, const point &b, const point & c)
{
    return (a.x * b.y + b.x*c.y + c.x * a.y) - (a.y * b.x + b.y*c.x + c.y*a.x);
}

inline point rotate(const point &p, double theta)
{	//점 p를 (0, 0) 기준으로 theta 회전시킨 점의 좌표
    return point(p.x*cos(theta) - p.y*sin(theta), p.x*sin(theta) + p.y*cos(theta));
}

inline point inc_angle(const point &l, const point &c, const point &r)
{	// l->c->r사이의 끼인각
    point da = l - c;
    point db = r - c;
    return acos((l*r) / (l.length() * r.length()));
}

inline point vec_angle(const point &p){
    return inc_angle(p, point(0, 0), point(1, 0));
}


/** line **/
struct line {
    coo a, b, c; // ax + by + c = 0 form
    line(coo a = 0, coo b = 0, coo c = 0) : a(a), b(b), c(c) {}
    line(const point &p, const point &q){
        a = (p.y - q.y);
        b = -(p.x - q.x);
        c = (p.x - q.x) * p.y - (p.y - q.y) * p.x;
    }
    double tangent() const {
        return  (double) -a / b;
    }
};

int checkLL(const line &p, const line &q)
{	//두 직선의 교차검사
    if ( equals( p.a * q.b , p.b * q.a ) && equals( p.b * q.c , p.c * q.b) )
        return 2;
    if ( equals( p.a * q.b , p.b * q.a) )
        return 0;
    return 1; //[0] not cross (parrel), [1] one point, [2] same
}

double distanceLP(const line &l, const point &p){
    return ABS(l.a*p.x + l.b*p.y + l.c) / sqrt(l.a * l.a + l.b * l.b);
}
double distanceLL(const line &p, const line & q){
    if (equals(p.a, 0) )
        return distanceLP(q, point(0, -p.c / p.b));
    else
        return distanceLP(q, point(-p.c / p.a, 0));
}

point getIntersection(const line &p, const line &q)
{	//두 직선의 교차점
    point ret;
    ret.x = (q.b * p.c - p.b * q.c) / (q.a * p.b - q.b * p.a);
    ret.y = (p.a * q.c - q.a * p.c) / (q.a * p.b - q.b * p.a);
    return ret;
}

point getSymmetric(const line &l, const point &p)
{	//점 p를 직선 l에 대해 대칭한 점
    point res;
    res.x = p.x - 2 * l.a*(p.x*l.a + p.y*l.b + l.c) / (l.a*l.a + l.b * l.b);
    res.y = p.y - 2 * l.b*(p.x*l.a + p.y*l.b + l.c) / (l.a*l.a + l.b * l.b);
    return res;
}


/** segment **/
struct segment{
    point a, b;
    segment(const point &a, const point &b) : a(a), b(b) {}
    
    coo sqrlen() const {
        return (a - b).sqrlen();
    }
    coo length() const {
        return (a - b).length();
    }
    
    int dir(const point & c) const{
        if (ccw(a, b, c) > EPS)
            return 1; //반시계
        if (ccw(a, b, c) < -EPS)
            return -1;//시계
        return 0; //평행
    }
    
    int belong(const point & c) const{
        if (c == a || c == b) return 1;
        if (dir(c) == 0 && min(a.x, b.x) <= c.x && c.x <= max(a.x, b.x)
            && min(a.y, b.y) <= c.y && c.y <= max(a.y, b.y))
            return 2;
        return 0; // [0] outside, [1] end-point, [2] inside
    }
};

int checkSS(const segment &p, const segment & q)
{	//두 선분의 교차여부
    if (p.dir(q.a) * p.dir(q.b) + q.dir(p.a)*q.dir(p.b) == -2)
        return 2;
    if (p.belong(q.a) || p.belong(q.b) || q.belong(p.a) || q.belong(p.b))
        return 1;
    return 0; // [0] not cross, [1] end-point, [2] inside
}

//geometric distance
double distanceSP(const segment &s, const point &p)
{	//선분과 점 사이의 거리
    coo A, B, C, t;
    A = (s.a - p).sqrlen();
    B = (s.b - p).sqrlen();
    C = (s.a - s.b).sqrlen();
    if (A < B)	swap(A, B);
    if (A > B + C || C < EPS)	return sqrt(B);
    return ABS(ccw(s.a, s.b, p)) / sqrt(C);
}

double distanceSS(const segment &sa, const segment & sb)
{	//두 선분 사이의 거리 
    if (checkSS(sa,sb) > 0)	return 0.0;
    return min({ distanceSP(sa, sb.a), distanceSP(sa, sb.b), distanceSP(sb, sa.a), distanceSP(sb, sa.b) });
}
