import java.util.ArrayList;


public class Graph {
	
	public final ArrayList<Edge> adj;  //Adjacent List for graph
	public final int n;	//number of vertex 
	
	
	//constructor 
	public Graph(int iSize){	
		this.n = iSize;
		adj = new ArrayList<Edge>();
	}
	
	//add edge to adj list 
	public void addEdge(int u,int v){ 
		adj.add(new Edge(u,v));
	}
	public ArrayList<Edge> getAdjList(){
		return this.adj;
	}
	
	public class Edge{
		public final int u, v;	// edge u<->v undirected.
		public int w;			// weigh for edge
		public Edge(int u, int v){
			this.u = u;
			this.v = v;
			this.w = 1;
		}
		void setWeight(int w){
			this.w = w;
		}
		int getWeight(){
			return this.w;
		}
		
		public int compareTo(Edge op){
			return op.w - this.w;
		}
	}
}
