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
		BufferedReader in = new BufferedReader(new FileReader(filename));
	}

}
