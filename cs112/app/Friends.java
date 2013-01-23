package app;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import structs.Graph;
import structs.Node;
import structs.Vertex;

public class Friends {
	
	public static void main(String[] args) throws IOException {
		System.out.println("Enter graph file name");
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		String file;
		try {
			file = in.readLine();
		} catch (IOException e) {
			System.out.println("IOException - aborting");
			return;
		}
		
		Graph graph = new Graph();
		graph.buildFromFile(file);
		
		boolean terminate=false;
		while(!terminate) {
			System.out.println("Select an option:");
			System.out.println("\t1. Students at a School");
			System.out.println("\t2. Shortest Intro Chain");
			System.out.println("\t3. Cliques at a School");
			System.out.println("\t4. Connectors");
			System.out.println("\t5. Quit");
			
			int option = Integer.parseInt(in.readLine());
			if(option<0 || option>5) {
				System.out.println("Bad input");
				continue;
			}
			switch(option) {
			case(0):	graph.print();
						break;
			case(1):	sameSchool(graph);
						break;
			case(2):	shortestPath(graph);
						break;
			case(3):	cliques(graph);
						break;
			case(4):	connectors(graph);
						break;
			case(5):	terminate=true;
						break;
			default:	continue;
			}
		}
	}
	
	public static void shortestPath(Graph graph) throws IOException {
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Who is the starting person?");
		String startName = in.readLine();
		System.out.println("Who is the destination?");
		String endName = in.readLine();
		
		Vertex start = graph.vertices.get(graph.indexes.get(startName));
		Vertex end = graph.vertices.get(graph.indexes.get(endName));
		Node<Vertex> ptr = graph.shortestPath(end, start);
		
		//print chain
		if(ptr==null)
			System.out.println("No path found");
		else {
			while(ptr.next!=null) {
				System.out.print(ptr.data.name+" -> ");
				ptr=ptr.next;
			}
			System.out.println(ptr.data.name);
		}
	}
	
	public static void connectors(Graph graph) {
		HashSet nodes = graph.connectors();
		Iterator<Vertex> i = nodes.iterator();
		while(i.hasNext()) {
			Vertex v = i.next();
			System.out.print(v.name+" -> ");
		}
	}
	
	public static void sameSchool(Graph graph) throws IOException {
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("What is the name of the school?");
		String school = in.readLine();
		Graph subgraph = graph.sameSchool(school);
		subgraph.print();
		if(subgraph.vertices.size()==0)
			System.out.println("No one found");
	}
	
	public static void cliques(Graph graph) throws IOException {
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("What is the name of the school?");
		String school = in.readLine();
		Node<Graph> graphs = graph.cliques(school);
		int i=1;
		while(graphs!=null) {
			System.out.println("Clique "+i+":");
			graphs.data.print();
			System.out.println("");
			i++;
			graphs=graphs.next;
		}
	}

}