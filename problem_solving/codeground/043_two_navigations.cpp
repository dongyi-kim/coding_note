/* 이 코드가 도움이 되었다면 페이스북 페이지 좋아요를 눌러주세요! */
/* https://www.facebook.com/CodingMonster/ */
#include<algorithm>
#include<vector>
#include<queue>
#include<cstdio>
using namespace std;

struct edge{
    int from;   //출발점
    int to;     //도착점
    int index;  //간선의 번호
}; //edge  정보, weight는 외부에서 저장하므로 index만 있으면 됩니다

const int MAX_E = 50000 + 1;
const int MAX_N = 10000 + 1;

long long wa[MAX_E]; //wa[i] := i번 간선의 아이폰의 가중치
long long wb[MAX_E]; //wb[i] := i번 간선의 안드로이드 가중치
long long wc[MAX_E]; //wc[i] := i번 간선을 사용할 시 듣게되는 경보음의 횟수

long long djk[MAX_N];
vector<edge> adj[MAX_N]; //인접리스트, 입력과 역방향임에 주의

struct state
{   //다익스트라를 통하여 조사하게 될 각 상태들을 하나의 구조체 값으로 표현하고자 선언한다
    int from;   //현재 상태로 오기 직전의 노드 번호
    int now;    //현재 상태에 서 있는 노드 번호
    long long total; //현재까지 소비된 총 비용 (지나온 간선의 가중치의 합)
    int last_edge;   //(from -> now)로 이동할 때 사용한 edge의 번호
    bool operator < (const state & op) const{
        //상태의 우선순위
        //다익스트라는 비용이 적은 간선일수록 우선순위가 높다
        return total > op.total;
    }
};

vector<state> back[MAX_N]; //back[i] := i노드에 최단 비용으로 도달한 상태의 집합
void dijkstra(int start, int n, long long* weight)
{   //start로 부터 weight[]의 가중치를 이용하여 djk 배열을 구합니다
    
    //각 노드에 대하여 초기화
    for(int i = 0 ; i < n ; i++)
    {
        djk[i] = 1e9;   //djk[i]는 무한대로
        back[i].clear();//초기화
    }
    
    //초기 상태
    state init;
    init.from = -1;
    init.last_edge = - 1;
    init.total = 0;
    init.now = start;
    
    priority_queue<state> q;
    q.push(init);
    
    while(!q.empty())
    {
        state cur = q.top();
        q.pop();
        
        if(djk[cur.now] < cur.total)
        {   //이미 cur.now 노드에 대해 더 짧은 거리를 찾았다면, 이 상태는 필요없다
            continue;
        }
        else if(djk[cur.now] == cur.total)
        {
            //최단 거리로 cur.now에 도달한 방법이 여러가지 일 경우
            //그 상태들을 모두 저장합니다
            back[cur.now].push_back(cur);
            continue;
        }
        
        //처음으로 cur.now에 대한 상태를 조회한 경우
        //최단 경로이다.
        djk[cur.now] = cur.total;
        back[cur.now].clear();
        back[cur.now].push_back(cur);   //cur.now번 노드에 도달하기 위하여 cur이라는 상태를 거쳤다
        
        for(int i = 0 ; i < adj[cur.now].size() ; i++)
        {   //cur.now로 부터 나가는 모든 간선 e에 대하여
            edge e = adj[cur.now][i];
            
            //cur.now에서 간선 e를 사용하여 이동한 다음 상태를 생성한다
            state next;
            next.now = e.to;
            next.from = e.from;
            next.last_edge = e.index;
            next.total = cur.total + weight[e.index];
            q.push(next);
        }
    }
}

void track(int start, int n)
{   //출발점부터 각 노드에 대한 최단 경로에 이용된 edge들을 조회한다
    for(int now = 0; now < n; now ++)
    {
        if(now == start)
            continue;
        
        for(int i = 0; i < back[now].size(); i++)
        {
            state s = back[now][i];
            wc[s.last_edge] --; //출발점부터 이 노드(now)에 대한 최단 경로로 사용된 edge는 경보음을 울리지 않는다.
        }
    }
}
void test_case()
{
    int n, m;
    scanf("%d %d", &n, &m);
    for(int i = 0 ; i < n ; i++)
    {
        adj[i].clear();
        
    }
    for(int i= 0 ; i < m ; i++)
    {
        int a, b, p, q;
        scanf("%d %d %d %d", &a, &b, &p, &q);
        a--;
        b--;
    
        //반대 방향으로 최단 경로를 계산할 것이므로
        //a, b를 뒤집는다
        swap(a,b);
        
        
        //edge정보 생성
        edge u;
        u.index = i;
        u.from = a;
        u.to = b;
        wa[i] = p;
        wb[i] = q;
        
        //adj-list에 추가
        adj[a].push_back(u);
        
        //모든 edge는 처음에 경보음 두 번을 유발한다고 초기화한다
        wc[i] = 2;
    }
    
    //아이폰 기준으로 n-1번 노드부터 모든 노드 까지의 최단 경로를 구한다
    dijkstra(n-1, n, wa);
    //그리고 최단 경로에 사용된 edge들에 대해 경보음 유발 횟수를 감소시킨다
    track(n-1, n);
    
    //안드로이드 폰도 마찬가지
    dijkstra(n-1, n, wb);
    track(n-1, n);
    
    //이렇게 갱신된 wc 가중치를 기준으로 다익스트라를 수행한다
    //즉, 각 간선의 가중치가 경보음 유발 횟수로 대체된 체로 다익스트라를 수행한다.
    dijkstra(n-1, n, wc);
    
    //방향이 반대므로, N번 노드에서 1번 노드로 향하는 최단 거리를 출력한다.
    printf("%lld\n", djk[0]);
}

int main(){
    setbuf(stdout, NULL);
    int t = 0 ;
    scanf("%d", &t);
    for(int i = 0; i < t ; i++)
    {
        printf("#testcase%d\n", i+1);
        test_case();
    }
    return 0;
    
    
}
