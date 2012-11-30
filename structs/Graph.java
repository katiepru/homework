package structs;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;

public class Graph {
	
	public Vertex[] vertices;
	public HashMap indexes;
	
	public Graph() {
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
			Vertex firstTex = vertices[(int) indexes.get(line.substring(0, line.indexOf('|')))];
			Vertex secondTex = vertices[(int) indexes.get(line.substring(line.indexOf('|')+1))];
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
			
			//visit each node
			if(visited.get(ptr.name) != null){
				if(ptr.school.compareToIgnoreCase(school)==0){
					schoolmates.next = schoolmates;
					schoolmates.data = ptr;
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
		
		}


}
