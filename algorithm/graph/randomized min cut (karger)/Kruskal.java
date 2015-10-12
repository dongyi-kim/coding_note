
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;




public class Kruskal {
	
	
	private final ArrayList<Graph.Edge> edge;
	private final int n;
	private final int[] rank;
	private final ArrayList<Graph.Edge> cutEdge;
	
	//constructor
	private Kruskal(Graph g){
		this.n = g.n;
		this.edge = g.getAdjList();
		this.rank = new int[n];
		for(int i=0;i<n;i++)
			this.rank[i] = i;
		cutEdge = new ArrayList<Graph.Edge>();
	}
	
	private void generateWithoutLast(){
		//sort all of edges order by their weight
		Collections.sort(edge, new Comparator<Graph.Edge>(){
			@Override
			public int compare(Graph.Edge e1,Graph.Edge e2){
				return e1.w - e2.w;
			}
		});
		
		int cnt = 0;
		for(Graph.Edge e : edge){
			if(find(e.u) == find(e.v))	
				continue;
			
			//union u <-> v
			union(e.u, e.v);
			
			//for divide two group, skip for last edge of mst
			if(++cnt == n-2)	
				break;
		}
	}
	
	//disjoint-set operation
	public int find(int vertex){
		if(rank[vertex] != vertex)
			rank[vertex]= find(rank[vertex]);
		return rank[vertex];
	}
	public void union(int v1, int v2){
		rank[find(v1)] = find(v2);
	}
	
	
	public static Kruskal getMSTforKarger(Graph g){
		Kruskal k = new Kruskal(g);
		k.generateWithoutLast();
		return k;
	}
	

}
