package app;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;

import structs.Graph;

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
			if(option<1 || option>5) {
				System.out.println("Bad input");
				continue;
			}
		}
	}

}