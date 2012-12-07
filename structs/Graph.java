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
				vertices[i] = new Vertex(line.substring(0, line.indexOf('|')), line.substring(line.lastIndexOf('|')+1));
			}
			indexes.put(vertices[i].name, i);
		}
		
		//create nodes for every connection
		String line=in.readLine();
		while(line!=null) {
			Vertex firstTex = vertices[indexes.get(line.substring(0, line.indexOf('|')))];
			Vertex secondTex = vertices[indexes.get(line.substring(line.indexOf('|')+1))];
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
	
	public int pupilCount(Vertex curr, HashMap<Vertex, Integer> visited, String school, int count){
		
		if(visited.get(curr) == null){
			visited.put(curr, 1);
			if(curr.school.compareToIgnoreCase(school)==0){
				count++;
			}
			Node ptr = curr.neighbor;
			while(ptr != null){
				if((visited.get(ptr.data) == null)){
					count = pupilCount(ptr.data, visited, school, count);
				}
				else{
					ptr = ptr.next;
				}	
			}	
		}
		return count;		
	}

	public Vertex sameSchool(String school){
		HashMap<Vertex, Integer> visited = new HashMap<Vertex, Integer>(vertices.length);
		int pupils = 0;
		for(int i = 0; i<vertices.length; i++){
			pupils+=pupilCount(vertices[i], visited, school, 0);
		}
		Vertex[] subSchool = new Vertex[pupils];
		HashMap<Vertex, Integer> subGraph = new HashMap<Vertex, Integer>(pupils);
		
		for(int i = 0; i<vertices.length; i++){
			Vertex temp = onlySubSchool(vertices[i], school);
			if(temp!=null){
				subGraph.put(temp, 1);
			}
		}		
		return null;
	}
	
	public Vertex onlySubSchool(Vertex curr, String school){
		
		if(curr.school.compareToIgnoreCase(school) == 0){
			
			System.out.println(curr.name);
			for(Node ptr = curr.neighbor; ptr != null; ptr = ptr.next){
				System.out.println(ptr.data.name);
				if(ptr.data.school.compareToIgnoreCase(school) != 0){
					if(ptr.next != null){
						ptr.data = ptr.next.data;
						ptr = ptr.next;
					}
					else{
						ptr = null;
						break;
					}
					
				}
			}
			return curr;
		}
		return null;
		
	}

	public Node shortestPath(Vertex start, Vertex finish) {
		//implement a breadth first search to find shortest path

		Set<Vertex> visited = new HashSet<Vertex>(this.vertices.length);
		Queue<Vertex> queue = new LinkedList<Vertex>();
		HashMap<Vertex, Node> pathTo = new HashMap<Vertex, Node>(this.vertices.length); 
		
		queue.add(start);
		pathTo.put(start, new Node(start, null));
		
		while(queue.peek()!=null) {
			if(bfs(queue.poll(), finish, visited, queue, pathTo)) {
				break;
			}
		}
		
		return pathTo.get(finish);
	}
	
	//This works for most test cases, except sam -> aparna. Will work on it
	private boolean bfs(Vertex curr, Vertex finish, Set<Vertex> visited, Queue<Vertex> queue, HashMap<Vertex, Node> pathTo) {
		if(!visited.contains(curr)) {
			if(curr==finish) {
				return true;
			}else {
				//add curr to visited
				visited.add(curr);
				
				Node ptr = curr.neighbor;
				while(ptr!=null) {
					if(!visited.contains(ptr.data) && pathTo.get(ptr.data)==null) {
						pathTo.put(ptr.data, new Node(ptr.data, pathTo.get(curr)));	
						queue.add(ptr.data);
					}
					ptr=ptr.next;
				}
			}
		}
		return false;
	}

	
	public HashSet<Vertex> connectors() {
		HashMap<Vertex, Integer> dfsNums = new HashMap<Vertex, Integer>(vertices.length);
		HashSet<Vertex> connectors = new HashSet<Vertex>(vertices.length);
		Set<Vertex> visited = new HashSet<Vertex>(vertices.length);
		for(int i=0; i<vertices.length; i++) {
			if(!visited.contains(vertices[i]))
				dfsConnect(vertices[i], true, connectors, 1, dfsNums, visited);
		}
		return connectors;
	}
	
	private int dfsConnect(Vertex curr, boolean start, HashSet<Vertex> connectors, int dfsnum, HashMap<Vertex, Integer> dfsnums, Set<Vertex> visited) {
		visited.add(curr);
		dfsnums.put(curr, dfsnum);
		int back=dfsnum;
		Node ptr = curr.neighbor;
		while(ptr!=null) {
			if(!visited.contains(ptr.data)) {
				int returned = dfsConnect(ptr.data, false, connectors, dfsnum+1, dfsnums, visited);
				if(dfsnum>returned) {
					back=Math.min(back, returned);
				}else {
					if(!start) {
						if(!connectors.contains(curr))
							connectors.add(curr);
					}else {
						start=false;
					}
				}
			}else {
				back=Math.min(back, dfsnums.get(ptr.data));
			}
			ptr=ptr.next;
		}
		return back;
	}
	
}
