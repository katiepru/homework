/*---------------------------------------------------------------------------------------------------------------
 * Kaitlin Poskaitis and Thomas Milburn
 * Node class
 * - These point to neighboring vertices in the graph
---------------------------------------------------------------------------------------------------------------*/

package structs;

public class Node<T> {
	
	public T data;
	public Node<T> next;
	
	public Node(T data, Node<T> next) {
		this.data=data;
		this.next=next;
	}

}
