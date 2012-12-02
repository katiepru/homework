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
	public HashMap<String, Integer> indexes;
	
	public Graph() {
	}

	public void build(Vertex[] vertices, HashMap<String, Integer> indexes) {
		this.indexes=indexes;
		this.vertices=vertices;
	}
	
	public void buildFromFile(String filename) throws IOException {
		//read the file
		BufferedReader in = new BufferedReader(new FileReader(filename));
		
		//read first line to get size of graph
		int size = Integer.parseInt(in.readLine());
		vertices = new Vertex[size];
		if(vertices.length==0) {
			in.close();
			return;
		}
		indexes = new HashMap<String, Integer>(size);
		
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
		while(line!=null) {
			Vertex firstTex = vertices[(Integer) indexes.get(line.substring(0, line.indexOf('|')))];
			Vertex secondTex = vertices[(Integer) indexes.get(line.substring(line.indexOf('|')+1))];
			Node tmp = firstTex.neighbor;
			firstTex.neighbor = new Node(secondTex, tmp);
			tmp = secondTex.neighbor;
			secondTex.neighbor = new Node(firstTex, tmp);
			line=in.readLine();
		}
		in.close();
			
		}
	
	public void print() {
		for(int i=0; i<vertices.length; i++) {
			System.out.print(vertices[i].name+" -> ");
			Node ptr = vertices[i].neighbor;
			while(ptr!=null) {
				System.out.print(ptr.data.name+" -> ");
				ptr=ptr.next;
			}
			System.out.println("");
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
			//		studentHash.put(ptr, studentHash)
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
		//	Graph subgraph = new Graph(studentVerticies, studentHash);
		
		}
	
	public Node shortestPath(Vertex start, Vertex finish) {
		//implement a breadth first search to find shortest path

		Set<Vertex> visited = new HashSet<Vertex>(this.vertices.length);
		Queue<Vertex> queue = new LinkedList<Vertex>();
		HashMap<Vertex, Node> pathTo = new HashMap<Vertex, Node>(this.vertices.length); 
		
		queue.add(start);
		pathTo.put(start, new Node(start, null));
		
		while(queue.peek()!=null) {
			if(bfs(queue.poll(), finish, visited, queue, pathTo)) break;
		}
		
		return pathTo.get(finish);
	}
	
	private boolean bfs(Vertex curr, Vertex finish, Set<Vertex> visited, Queue<Vertex> queue, HashMap<Vertex, Node> pathTo) {
		if(!visited.contains(curr)) {
			if(curr==finish) {
				return true;
			}else {
				System.out.println("curr= "+curr.name);
				visited.add(curr);
				Node ptr = curr.neighbor;
				while(ptr!=null) {
					System.out.println("ptr= "+ptr.data.name);
					if(visited.contains(ptr.data)) {
						System.out.println("skipping this ptr");
						ptr=ptr.next;
						continue;
					}
					//pathTo.put(ptr.data, new Node(ptr.data, pathTo.get(curr)));
					Node temp = pathTo.get(curr);
					Node ptr2=temp;
					System.out.print("ptr2= "+ptr2.data.name + " -> ");
					while(ptr2.next!=null){
						ptr2=ptr2.next;
						if(!ptr.data.name.equals("bob"))
							System.out.print(ptr2.data.name+" -> ");
					}
					System.out.println("");
					ptr2.next=ptr;
					pathTo.put(ptr.data, temp);
					queue.add(ptr.data);
					ptr=ptr.next;
				}
			}
		}
		return false;
	}
	
}
