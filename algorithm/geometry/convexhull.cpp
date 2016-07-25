
namespace convexhull{
    point origin;
    bool comparator(const point & a , const point & b ) {
        return (ccw(origin, a, b) < - EPS);
    }
    
    vector<point> generate(vector<point> &v){
        sort(v.begin(), v.end());
        origin = v[0];
        sort(v.begin() + 1, v.end(), comparator);
        vector<point> h(v.begin(), v.begin() + 2);
        for (int i = 2; i < v.size(); i++){
            int tail = h.size() - 1;
//            while (ccw(h[tail - 1], h[tail], v[i]) >  EPS){ // include point on the segment
            
            while (ccw(h[tail - 1], h[tail], v[i]) > EPS){ // vertex only
                h.pop_back();
                if (!(--tail))	break;
            }
            h.push_back(v[i]);
        }
        if (h.size() < 3)	 h.clear();
        return h;
    }
    
    double getArea(vector<point> &h){
        coo area = 0.0;
        for (int i = 0; i < h.size(); i++){
            int j = ( i + 1 ) % h.size();
            area += h[i].x * h[j].y - h[i].y * h[j].x;
        }
        return ABS(area) / 2.0;
    }
    
    bool contains(vector<point> &h, const point & p){
        for (int i = 0; i < h.size(); i++){
            if (ccw(h[i], h[(i + 1) % h.size()], p) > EPS){     // including points on the segment
            //if (ccw(h[i], h[(i + 1) % h.size()], p) >= -EPS){ // excepting points on the segment
                return false;
            }
            
        }
        return true;
    }
    
    double getMaximumDiameter(vector<point> &h)
    {
        int n = h.size();
        h.push_back(h.front());
        int b;
        coo maxs = 0;
        for (int i = b = 0; i < n; i++){
            while ( (h[i]-h[b+1]).sqrlen() >  (h[i] - h[b]).sqrlen() ) b = (b + 1) % n;
            coo dist = (h[i] - h[b]).sqrlen();
            if (dist >= maxs)
            {   // h[i] 와 h[b]사이의 거리가 최대
                maxs = dist;
            }
        }
        h.pop_back();
        return sqrt(maxs);
    }
}
