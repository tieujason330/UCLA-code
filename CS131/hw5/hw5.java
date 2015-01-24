/* Name: Jason Tieu

   UID: 304047667

   Others With Whom I Discussed Things:

   Other Resources I Consulted:

   https://piazza.com/class/i10rs0q03qt7lj?cid=203

   https://docs.oracle.com/javase/1.5.0/docs/guide/language/foreach.html

   https://docs.oracle.com/javase/7/docs/api/java/util/Stack.html

   https://docs.oracle.com/javase/8/docs/api/java/util/List.html

   http://stackoverflow.com/questions/5766318/converting-double-to-string

   https://docs.oracle.com/javase/7/docs/api/java/util/LinkedList.html

   http://stackoverflow.com/questions/189559/how-do-i-join-two-lists-in-java

   https://docs.oracle.com/javase/tutorial/java/IandI/objectclass.html

   http://www.coderanch.com/t/612460/java-programmer-SCJP/certification/Exception-thrown-body-statement-error

   http://javapapers.com/core-java/lambda-expression-in-java-8/
   
*/

// import lists and other data structures from the Java standard library
import java.util.*;

// a type for arithmetic expressions
interface AExp {
    double eval(); 	                       // Problem 1a
    List<AInstr> compile(); 	               // Problem 1c
}

class Num implements AExp {
    protected double val;

    //Constructor
    Num(double v){
	this.val = v;
    }

    //eval for Numbers just returns val
    public double eval(){
	return this.val;
    }

    //compile for Numbers adds a new Push object to list
    public List<AInstr> compile(){
	List<AInstr> l = new LinkedList<AInstr>();
	l.add(new Push(val));
	return l;
    }
}

class BinOp implements AExp {
    protected AExp left, right;
    protected Op op;

    //Constructor
    BinOp(AExp l, Op oper, AExp r){
	this.left = l;
	this.right = r;
	this.op = oper;	
    }

    //eval for BinOp calls calculate() method on left and right operands
    public double eval(){
	return op.calculate(this.left.eval(), this.right.eval()); 
    }

    //compile adds left operand first, then right operand, and then operator
    public List<AInstr> compile(){
	List<AInstr> l = new LinkedList<AInstr>();
	l.addAll(left.compile());
	l.addAll(right.compile());
	l.add(new Calculate(op));

	return l;
    }
}

// a representation of four arithmetic operators
enum Op {
    PLUS { public double calculate(double a1, double a2) { return a1 + a2; } 
	public String toString() { return "PLUS";} },
    MINUS { public double calculate(double a1, double a2) { return a1 - a2; } 
	public String toString() { return "MINUS";} },
    TIMES { public double calculate(double a1, double a2) { return a1 * a2; } 
	public String toString() { return "TIMES";} },
    DIVIDE { public double calculate(double a1, double a2) { return a1 / a2; } 
	public String toString() { return "DIVIDE";} };

    abstract double calculate(double a1, double a2);
}

// a type for arithmetic instructions
interface AInstr {
    void eval(Stack<Double> s);
}

class Push implements AInstr {
    protected double val;

    //Constructor
    Push(double v){
	this.val = v;
    }

    //returns the double pushed onto the stack
    public void eval(Stack<Double> s){
	s.push(val);
    }

    public String toString(){
	return "Push " + String.valueOf(val);
    }
}

class Calculate implements AInstr {
    protected Op op;

    //Constructor
    Calculate(Op oper){
	this.op = oper;
    }

    //calculates the new value using two operands and pushes value onto stack
    public void eval(Stack<Double> s){
	double d2 = s.pop(); //second operand is 1st val popped off stack
	double d1 = s.pop(); //first operand is 2nd val popped off stack
	double val = op.calculate(d1, d2);
	s.push(val);
    }

    public String toString(){
	return "Calculate " + op.toString();
    }
}

class Instrs {
    protected List<AInstr> instrs;

    public Instrs(List<AInstr> instrs) { this.instrs = instrs; }

    public Stack<Double> s = new Stack<Double>();
    private double final_val;

    // Problem 1b
    public double eval() {
	for(AInstr elem : instrs)
	    elem.eval(s);

	final_val = s.pop();
	return final_val;
    }   
}


class CalcTest {
    public static void main(String[] args) {
	    // a test for Problem 1a
	 AExp aexp =
	     new BinOp(new BinOp(new Num(1.0), Op.PLUS, new Num(2.0)),
	 	      Op.TIMES,
	 	      new Num(3.0));
	 System.out.println("aexp evaluates to " + aexp.eval()); // aexp evaluates to 9.0

	// a test for Problem 1b
	List<AInstr> is = new LinkedList<AInstr>();
	is.add(new Push(1.0));
	is.add(new Push(2.0));
	is.add(new Calculate(Op.PLUS));
	is.add(new Push(3.0));
	is.add(new Calculate(Op.TIMES));
	Instrs instrs = new Instrs(is);
	System.out.println("instrs evaluates to " + instrs.eval());  // instrs evaluates to 9.0

	// a test for Problem 1c
	System.out.println("aexp converts to " + aexp.compile());

    }
}

// a type for dictionaries mapping keys of type K to values of type V
interface Dict<K,V> {
    void put(K k, V v);
    V get(K k) throws NotFoundException;
}

class NotFoundException extends Exception {}


// Problem 2a
class DictImpl2<K,V> implements Dict<K,V> {
    protected Node<K,V> root;

    DictImpl2() {
	//throw new RuntimeException("not implemented"); 
	this.root = new Empty<K,V>();
    }

    public void put(K k, V v) { 
	//throw new RuntimeException("not implemented");
	this.root = this.root.put(k,v);
    }

    public V get(K k) throws NotFoundException { 
	//throw new RuntimeException("not implemented");
	return this.root.get(k);
    }
}

interface Node<K,V> {
    Node<K,V> put(K k, V v);
    V get(K k) throws NotFoundException;
}

class Empty<K,V> implements Node<K,V> {
    Empty() {}

    public Node<K,V> put(K k, V v) {
	//System.out.println("Empty put");
	return new Entry<K,V>(k, v, this);
    }

    public V get(K k) throws NotFoundException {
	throw new NotFoundException();
    }
    
}

class Entry<K,V> implements Node<K,V> {
    protected K k;
    protected V v;
    protected Node<K,V> next;

    Entry(K k, V v, Node<K,V> next) {
	this.k = k;
	this.v = v;
	this.next = next;
    }

    public Node<K,V> put(K k, V v) {
	
	if(this.k.equals(k)) {
	    this.v = v;
	    //System.out.println("if put");
	}
	else {
	    this.next = this.next.put(k, v);
	    //System.out.println("else put");
	}
	return this.next;
    }

    public V get(K k) throws NotFoundException {
	if(this.k.equals(k))
	    return this.v;
	else
	    return this.next.get(k);
    }
}


interface DictFun<A,R> {
    R invoke(A a) throws NotFoundException;
}

// Problem 2b
class DictImpl3<K,V> implements Dict<K,V> {
    protected DictFun<K,V> dFun;

    DictImpl3() { 
	//throw new RuntimeException("not implemented");
	this.dFun = new DictFun<K,V>(){
	    public V invoke(K k) throws NotFoundException{
		throw new NotFoundException();
	    }
	};
    }

    public void put(K k, V v) { 
	//throw new RuntimeException("not implemented"); 
	DictFun<K,V> old_dFun = this.dFun;
	this.dFun = (K invokeK) -> 
	                       {
			       if (k.equals(invokeK)) 
				   return v;
			       else 
				   return old_dFun.invoke(invokeK);
	                       };		     
    }

    public V get(K k) throws NotFoundException { 
	//throw new RuntimeException("not implemented");
	return this.dFun.invoke(k);
    }
}


class Pair<A,B> {
    protected A fst;
    protected B snd;

    Pair(A fst, B snd) { this.fst = fst; this.snd = snd; }

    A fst() { return fst; }
    B snd() { return snd; }
}

// Problem 2c
interface FancyDict<K,V> extends Dict<K,V> {
    void clear();
    boolean containsKey(K k);
    void putAll(List<Pair<K,V>> entries);
}

class FancyDictImpl3<K,V> implements FancyDict<K,V> {
    protected DictFun<K,V> dFun;

    //call clear() to make an 'empty' dFun
    FancyDictImpl3() {
	//dFun = new DictFun<K,V>(){
	//    public V invoke(K k) throws NotFoundException {
	//	throw new NotFoundException();
	//    }
	//	};
	this.clear();
    }

    //clear() should just reset dFun
    public void clear() {
	dFun = new DictFun<K,V>() {
	    public V invoke(K k) throws NotFoundException {
		throw new NotFoundException();
	    }
	};
    }
    
    public void put(K k, V v) {
	DictFun<K,V> old_dFun = this.dFun;
	
	this.dFun = (K invokeK) -> { if(invokeK.equals(k)) return v;
	                             else return old_dFun.invoke(invokeK); 
	                           };
    }

    public V get(K k) throws NotFoundException {
	return this.dFun.invoke(k);
    }
    //if invoke() doesn't throw exception, then contains key
    //else doesn't contain key
    public boolean containsKey(K k) {
	try{
	    this.get(k);
	    return true; //if invoke() doesn't throw exception, then contains key
	}catch(NotFoundException e) {
	    return false;
	}
    }

    public void putAll(List<Pair<K,V>> entries) {
	for(Pair<K,V> pair : entries)
	    this.put(pair.fst(), pair.snd());
    }
}


class DictTest {
    public static void main(String[] args) {

	// a test for Problem 2a
	Dict<String,Integer> dict1 = new DictImpl2<String,Integer>();
	dict1.put("hello", 23);
	dict1.put("bye", 45);
	try {
	    System.out.println("bye maps to " + dict1.get("bye")); // prints 45
	    System.out.println("hi maps to " + dict1.get("hi"));  // throws an exception
	} catch(NotFoundException e) {
	    System.out.println("not found!");  // prints "not found!"
	}

	// a test for Problem 2b
	Dict<String,Integer> dict2 = new DictImpl3<String,Integer>();
	dict2.put("hello", 23);
	dict2.put("bye", 45);
	try {
	    System.out.println("bye maps to " + dict2.get("bye"));  // prints 45
	    System.out.println("hi maps to " + dict2.get("hi"));   // throws an exception
	} catch(NotFoundException e) {
	    System.out.println("not found!");  // prints "not found!"
	}

	// a test for Problem 2c
	FancyDict<String,Integer> dict3 = new FancyDictImpl3<String,Integer>();
	dict3.put("hello", 23);
	dict3.put("bye", 45);
	System.out.println(dict3.containsKey("bye")); // prints true
	dict3.clear();
	System.out.println(dict3.containsKey("bye")); // prints false

    }
}

