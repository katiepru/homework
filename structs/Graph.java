package structs;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Set;

public class Graph {
	
	public Vertex[] vertices;
	public HashMap indexes;
	
	public Graph() {
	}

	public void build(Vertex[] vertices, HashMap indexes) {
		this.indexes=indexes;
		this.vertices=vertices;
	}
	
	public void buildFromFile(String filename) throws IOException {
		//read the file
		BufferedReader in = new BufferedReader(new FileReader(filename));
		
		//read first line to get size of graph
		int size = Integer.parseInt(in.readLine());
		vertices = new Vertex[size];
		if(vertices.length==0) return;
		indexes = new HashMap(size);
		
		//Population vertex array
		for(int i=0; i<vertices.length; i++) {
			String line = in.readLine();
			if(line.indexOf('|')==-1) {
				vertices[i] = new Vertex(line, "");
			}else {
				vertices[i] = new Vertex(line.substring(0, line.indexOf('|')), line.substring(line.lastIndexOf('|')));
			}
			indexes.put(vertices[i].name, i);
		}
		
		//create nodes for every connection
		String line=in.readLine();
		int i=0;
		while(line!=null) {
			Vertex firstTex = vertices[(Integer) indexes.get(line.substring(0, line.indexOf('|')))];
			Vertex secondTex = vertices[(Integer) indexes.get(line.substring(line.indexOf('|')+1))];
			Node tmp = firstTex.neighbor;
			firstTex.neighbor = new Node(secondTex, tmp);
			tmp = secondTex.neighbor;
			secondTex.neighbor = new Node(firstTex, tmp);
		}
			
		}
	
	public void dfs(Vertex ptr, HashMap<Vertex, Integer> visited){
		//visit each node
		if(visited.get(ptr.name) != null){
			//do something
				return;
			}
			else{
				visited.put(ptr, 1);
				while(ptr.neighbor != null){
					dfs(ptr.neighbor.data, visited);
					ptr = ptr.neighbor.data;
				}
			
			}	
		
		}
	
	public void sameSchool(Vertex ptr, HashMap<Vertex, Integer> visited, String school, Node schoolmates){
		int numStudents = 0;
		//visit each node
			if(visited.get(ptr.name) != null){
				if(ptr.school.compareToIgnoreCase(school)==0){
					numStudents++;
				}
				return;
			}
			else{
				visited.put(ptr, 1);
				while(ptr.neighbor != null){
					dfs(ptr.neighbor.data, visited);
					ptr = ptr.neighbor.data;
				}
			
			}
			int count = 0;
			Vertex[] studentVerticies = new Vertex[numStudents];
			if(visited.get(ptr.name) != null){
				if(ptr.school.compareToIgnoreCase(school)==0){
					studentVerticies[count] = ptr;
					studentHash.put(ptr, studentHash)
				}
				return;
			}
			else{
				visited.put(ptr, 1);
				while(ptr.neighbor != null){
					dfs(ptr.neighbor.data, visited);
					ptr = ptr.neighbor.data;
				}
			
			}	
			Graph subgraph = new Graph(studentVerticies, studentHash);
		
		}
	
	public Node shortestPath(Vertex start, Vertex finish) {
		//implement a breadth first search to find shortest path

		Set<Vertex> visited = new HashSet<Vertex>(this.vertices.length);
		Queue<Vertex> queue = new LinkedList<Vertex>();
		HashMap<Vertex, Node> pathTo = new HashMap<Vertex, Node>(this.vertices.length); 
		
		queue.add(start);
		pathTo.put(start, null);
		
		while(queue.peek()!=null) {
			if(bfs(queue.poll(), finish, visited, queue, pathTo)) break;
		}
		
		return pathTo.get(finish);
	}
	
	private boolean bfs(Vertex curr, Vertex finish, Set visited, Queue<Vertex> queue, HashMap<Vertex, Node> pathTo) {
		if(!visited.contains(curr)) {
			if(curr==finish) {
				return true;
			}else {
				visited.add(curr);
				Node ptr = curr.neighbor;
				while(ptr!=null) {
					pathTo.put(ptr.data, new Node(ptr.data, pathTo.get(curr)));
					queue.add(ptr.data);
				}
			}
		}
		return false;
	}
	
}
