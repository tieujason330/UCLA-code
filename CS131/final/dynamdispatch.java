import java.util.*;

class C {

	void m() { System.out.println("C.m"); }
	void n() { this.m(); }

	void p(C c) { System.out.println("CpC"); }
	void p(D d) { System.out.println("CpD"); }
}

class D extends C {
	void m() { System.out.println("D.m"); }
}

class Main {

	public static void main(String[] args) {
		C c = new C();
		D d = new D();
		/*
		c = d;
		c.m();
		d.m();
		c.p(c);
		d.p(d);
		*/		
		c.m();
		d.m();
		c.n();
		d.n();
		c.p(c);
		c.p(d);	
	}
}
