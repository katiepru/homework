package structs;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Graph {
	
	public Vertex[] vertices;
	
	public Graph() {
	}
	
	public void build(String filename) throws IOException {
		//read the file
		BufferedReader in = new BufferedReader(new FileReader(filename));
		
		//read first line to get size of graph
		vertices = new Vertex[Integer.parseInt(in.readLine())];
		if(vertices.length==0) return;
		
		//Population vertex array
		for(int i=0; i<vertices.length; i++) {
			String line = in.readLine();
			if(line.indexOf('|')==-1) {
				vertices[i] = new Vertex(line, "");
			}else {
				vertices[i] = new Vertex(line.substring(0, line.indexOf('|')), line.substring(line.lastIndexOf('|')));
			}
		}
		
		//create nodes for every connection
		String line=in.readLine();
		int i=0;
		while(line!=null) {
			if(line.substring(0,line.indexOf('|')).equals(vertices[i].name)) {
				for(int j=0; j<vertices.length; j++) {
					if(line.substring(line.indexOf('|')).equals(vertices[j].name)
						
				}
			}
		}
	}
	

}
