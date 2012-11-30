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
		graph.build(file);
	}

}