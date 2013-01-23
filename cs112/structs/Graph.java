package structs;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Set;

public class Graph {
	
	public ArrayList<Vertex> vertices;
	public HashMap<String, Integer> indexes;
	
	public Graph() {
		vertices = new ArrayList<Vertex>();
		indexes = new HashMap<String, Integer>();
	}

	public void build(ArrayList<Vertex> vertices, HashMap<String, Integer> indexes) {
		this.indexes=indexes;
		this.vertices=vertices;
	}
	
	public void buildFromFile(String filename) throws IOException {
		//read the file
		BufferedReader in = new BufferedReader(new FileReader(filename));
		
		//read first line to get size of graph
		int size = Integer.parseInt(in.readLine());
		vertices = new ArrayList<Vertex>(size);
		if(size==0) {
			in.close();
			return;
		}
		indexes = new HashMap<String, Integer>(size);
		
		//Population vertex array
		for(int i=0; i<size; i++) {
			String line = in.readLine();
			if(line.indexOf('|')==-1) {
				vertices.add(i, new Vertex(line, ""));
			}else {
				vertices.add(i, new Vertex(line.substring(0, line.indexOf('|')), line.substring(line.lastIndexOf('|')+1)));
			}
			indexes.put(vertices.get(i).name, i);
		}
		
		//create nodes for every connection
		String line=in.readLine();
		while(line!=null) {
			Vertex firstTex = vertices.get((int) indexes.get(line.substring(0, line.indexOf('|'))));
			Vertex secondTex = vertices.get((Integer) indexes.get(line.substring(line.indexOf('|')+1)));
			Node<Vertex> tmp = firstTex.neighbor;
			firstTex.neighbor = new Node<Vertex>(secondTex, tmp);
			tmp = secondTex.neighbor;
			secondTex.neighbor = new Node<Vertex>(firstTex, tmp);
			line=in.readLine();
		}
		in.close();
			
		}
	
	public void addVertex(Vertex v) {
		vertices.add(v);
		indexes.put(v.name, vertices.indexOf(v));
	}
	
	public void print() {
		for(int i=0; i<vertices.size(); i++) {
			System.out.print(vertices.get(i).name+" -> ");
			Node<Vertex> ptr = vertices.get(i).neighbor;
			while(ptr!=null) {
				System.out.print(ptr.data.name+" -> ");
				ptr=ptr.next;
			}
			System.out.println("");
		}
	}

	
	public Graph sameSchool(String school) {
		
		Set<Vertex> visited = new HashSet<Vertex>(vertices.size());
		Graph graph = new Graph();
		
		for(int i=0; i<vertices.size(); i++) {
			if(!visited.contains(vertices.get(i)))
				dfsSchool(vertices.get(i), graph, school, visited);
		}
		
		return graph;
	}
	
	public Node<Graph> cliques(String school) {
		Set<Vertex> visited = new HashSet<Vertex>(vertices.size());
		Node<Graph> graphs = new Node<Graph>(null, null);
		for(int i=0; i<vertices.size(); i++) {
			if(!visited.contains(vertices.get(i))) {
				Graph graph = new Graph();
				dfsSchool(vertices.get(i), graph, school, visited);
				if(graph.vertices.size()!=0) {
					if(graphs.data==null) {
						graphs.data=graph;
					}else {
						graphs = new Node<Graph>(graph, graphs);
					}
				}
			}
		}
		
		return graphs;
	}
	
	private boolean dfsSchool(Vertex curr, Graph schoolMembers, String school, Set<Vertex> visited){
		//add curr to visited
		visited.add(curr);
		if(curr.school.equalsIgnoreCase(school)) {
			Vertex tmp = new Vertex(curr.name, curr.school);
			schoolMembers.addVertex(tmp);
			Node<Vertex> ptr = curr.neighbor;
			while(ptr!=null) {
				if(!visited.contains(ptr.data)) {
					boolean isGood = dfsSchool(ptr.data, schoolMembers, school, visited);
					if(isGood) 
						tmp.neighbor=new Node(ptr.data, tmp.neighbor);	
				}else {
					try {
						tmp.neighbor=new Node(schoolMembers.vertices.get(schoolMembers.indexes.get(ptr.data.name)), tmp.neighbor);
					}catch(NullPointerException e) {
						//Not in sameSchool...no problem
					}
				}
				ptr=ptr.next;
			}
			
			return true;
		}else {
			return false;
		}
			
	}
	
	public Node<Vertex> shortestPath(Vertex start, Vertex finish) {
		//implement a breadth first search to find shortest path

		Set<Vertex> visited = new HashSet<Vertex>(this.vertices.size());
		Queue<Vertex> queue = new LinkedList<Vertex>();
		HashMap<Vertex, Node<Vertex>> pathTo = new HashMap<Vertex, Node<Vertex>>(this.vertices.size()); 
		
		queue.add(start);
		pathTo.put(start, new Node<Vertex>(start, null));
		
		while(queue.peek()!=null) {
			if(bfs(queue.poll(), finish, visited, queue, pathTo)) {
				break;
			}
		}
		
		return pathTo.get(finish);
	}
	
	//This works for most test cases, except sam -> aparna. Will work on it
	private boolean bfs(Vertex curr, Vertex finish, Set<Vertex> visited, Queue<Vertex> queue, HashMap<Vertex, Node<Vertex>> pathTo) {
		if(!visited.contains(curr)) {
			if(curr==finish) {
				return true;
			}else {
				//add curr to visited
				visited.add(curr);
				
				Node<Vertex> ptr = curr.neighbor;
				while(ptr!=null) {
					if(!visited.contains(ptr.data) && pathTo.get(ptr.data)==null) {
						pathTo.put(ptr.data, new Node<Vertex>(ptr.data, pathTo.get(curr)));	
						queue.add(ptr.data);
					}
					ptr=ptr.next;
				}
			}
		}
		return false;
	}

	
	public HashSet connectors() {
		HashMap<Vertex, Integer> dfsNums = new HashMap<Vertex, Integer>(vertices.size());
		HashSet connectors = new HashSet(vertices.size());
		Set<Vertex> visited = new HashSet<Vertex>(vertices.size());
		for(int i=0; i<vertices.size(); i++) {
			if(!visited.contains(vertices.get(i)))
				dfsConnect(vertices.get(i), true, connectors, 1, dfsNums, visited);
		}
		return connectors;
	}
	
	private int dfsConnect(Vertex curr, boolean start, HashSet connectors, int dfsnum, HashMap<Vertex, Integer> dfsnums, Set<Vertex> visited) {
		visited.add(curr);
		dfsnums.put(curr, dfsnum);
		int back=dfsnum;
		Node<Vertex> ptr = curr.neighbor;
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
