import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;

public class MinCut {	
		public static final int T = 3;
		public static final Scanner scanner = new Scanner(System.in);
		public static void main(String[] args){
			int iTest = scanner.nextInt();
			for(int t = 0; t < iTest; t++){
				//for each test case 
				int n = scanner.nextInt();
				int m = scanner.nextInt();
				Graph g = new Graph(n);
				
				//input for each edges
				for(int i=0;i<m;i++){
					int u = scanner.nextInt();
					int v = scanner.nextInt();
					g.addEdge(u, v);
				}
				
				//find & print minimum cut of graph g
				int minCut = findMinCut(g);
				System.out.println(minCut);
			}
		}
		
		// to find minimum-cut of g by using karger's algorithm, 
		// (1) Assign random weights for each edges - O(m)
		// (2) Find a mst without last edge by using kruskal - O( m log n )
		// (3) count the number of edges that not linked to two group - O(m)
		// repeat (1)~(3) for 'T' times
		// Time Complexity : O( T * ( m + m log n + m ) ) = O(T*m*log n)
		public static int findMinCut(Graph g){
			int minCut = g.getAdjList().size();
			for(int i=0;i<T;i++){
				Random random = new Random();
				
				ArrayList<Graph.Edge> edge = g.getAdjList();
				for(Graph.Edge e : edge){
					e.setWeight(random.nextInt(10000000));
				}
				Kruskal k = Kruskal.getMSTforKarger(g);
				int cuts = 0;	//minimum-cut in this test
				for(Graph.Edge e: edge){
					//if edge e cross two group, e is one of cuts.
					if(k.find(e.u) != k.find(e.v))
						cuts++;
				}
				minCut = Math.min(minCut, cuts);
			}
			return minCut;
		}
}
