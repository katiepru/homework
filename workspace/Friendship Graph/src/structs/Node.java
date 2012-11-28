/*---------------------------------------------------------------------------------------------------------------
 * Kaitlin Poskaitis and Thomas Milburn
 * Node class
 * - These point to neighboring vertices in the graph
---------------------------------------------------------------------------------------------------------------*/

package structs;

public class Node {
	
	public Vertex data;
	public Node next;
	
	public Node(Vertex data, Node next) {
		this.data=data;
		this.next=next;
	}

}
