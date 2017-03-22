#include <cstdio>
//
const int MaxN = 123;
const int MaxM = 123;
const int MaxK = 15;
//
int N, M, K;
int Field[MaxN][MaxM];
bool Water[MaxN][MaxM];
int Memo[MaxN][MaxM][MaxK];

//
inline int Min(int a, int b){ return a < b ? a : b; }
inline int ABS(int a){ return a<0 ? -a : a; }
void Init(){
   for (int i = 0; i<MaxN; i++){
      for (int j = 0; j < MaxM; j++){
         for (int k = 0; k < MaxK; k++){
            Memo[i][j][k] = -1;
         }
      }
   }
}
// 
int dp(int r, int c, int need_Water){
   if (r > N || c > M) return 1e8;
   if (r == N && c == M){
      if (Water[r][c] == true && need_Water > 0){
         //도착점에 물병이 있다면
         need_Water--;
      }
      if (need_Water == 0) return 0;
      else return 1e8;
   }
   int &ret = Memo[r][c][need_Water];
   if (ret != -1) return ret;
   ret=   1e8;

   int distr = ABS(Field[r][c] - Field[r+1][c]); //세로로 이동할 때 고도차
   int distc = ABS(Field[r][c] - Field[r][c+1]); //가로로 이동할 떄 고도차
   if (Water[r][c] == true){
      //필요한 물이 남았으면 들고 가던지 그냥 가던지
      if (need_Water > 0){
         ret = Min(ret, dp(r + 1, c, need_Water - 1) + distr);
         ret = Min(ret, dp(r + 1, c, need_Water) + distr);
         ret = Min(ret, dp(r , c+1, need_Water - 1) + distc);
         ret = Min(ret, dp(r , c+1, need_Water) + distc);
      }
      else if (need_Water == 0){ // 물 다 찾으니 그냥 이동
         ret = Min(ret, dp(r + 1, c, need_Water) + distr);
         ret = Min(ret, dp(r, c + 1, need_Water) + distc);
      }
   }
   else { // 물이 없으면 그냥 가기
      ret = Min(ret, dp(r + 1, c, need_Water) + distr);
      ret = Min(ret, dp(r, c + 1, need_Water) + distc);
   }
   return ret;
}

//
void test_case(){
   Init();
   scanf("%d %d %d", &M, &N, &K);
   for (int r = 0; r <= N; r++){
      for (int c = 0; c <= M; c++){
         int height;
         scanf("%d", &height);
         if (height < 0){
            Water[r][c] = true;
            height = -height;
         }
         Field[r][c] = height;
      }
   }
   
   int ans = dp(0, 0, K); // k~10까지 다해봐야 생각해봤으니 안해도 됨. 어차피 줏거나 안줍거나 다 고려하므로
   printf("%d\n", ans);
   return;
}
//
int main(){
   int ncase;
   scanf("%d", &ncase);
   for (int icase = 0; icase < ncase; icase++){
      printf("Case #%d\n", icase + 1);
      test_case();
   }

}
