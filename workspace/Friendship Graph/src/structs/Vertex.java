/*---------------------------------------------------------------------------------------------------------------
 * Kaitlin Poskaitis and Thomas Milburn
 * Vertex class
 * - These act as vertices in graph
---------------------------------------------------------------------------------------------------------------*/
package structs;

public class Vertex {
	
	public String name;
	public String school;
	public Node neighbor;
	public int degree;
	
	public Vertex(String name, String school) {
		this.name=name;
		this.school=school;
		this.neighbor=null;
		this.degree=0;
	}
	
	public void addNeighbor(Node neighbor) {
		if(this.neighbor==null)
			this.neighbor=neighbor;
		else {
			Node ptr=this.neighbor;
			while(ptr.next!=null) {
				ptr=ptr.next;
			}
			ptr.next=neighbor;
		}
		this.degree++;
	}

}
