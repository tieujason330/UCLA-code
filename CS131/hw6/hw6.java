/* Name: Jason Tieu

   UID: 304047667

   Others With Whom I Discussed Things:

   Other Resources I Consulted:
piazza.com

http://docs.oracle.com/javase/8/docs/api/

http://docs.oracle.com/javase/7/docs/api/java/lang/System.html

https://docs.oracle.com/javase/8/docs/api/java/util/stream/Stream.html

https://docs.oracle.com/javase/7/docs/api/java/util/Arrays.html

http://stackoverflow.com/questions/24087646/array-stream-not-summing

http://stackoverflow.com/questions/4934412/java-compile-error-reached-end-of-file-while-parsing

http://stackoverflow.com/questions/19788989/error-unreported-exception-filenotfoundexception-must-be-caught-or-declared-to

http://www.developer.com/java/java-7-forkjoin-framework.html

http://docs.oracle.com/javase/7/docs/api/java/lang/Math.html

https://docs.oracle.com/javase/7/docs/api/java/util/concurrent/RecursiveTask.html

https://docs.oracle.com/javase/7/docs/api/java/util/Arrays.html

https://docs.oracle.com/javase/8/docs/api/java/util/stream/IntStream.html

http://docs.oracle.com/javase/7/docs/api/java/util/concurrent/RecursiveAction.html

https://docs.oracle.com/javase/7/docs/api/java/util/concurrent/ForkJoinPool.html

https://docs.oracle.com/javase/tutorial/java/javaOO/methodreferences.html

http://www.java2s.com/Tutorials/Java_Streams/java.util.stream/IntStream/IntStream_range_int_startInclusive_int_endExclusive_example.htm

http://stackoverflow.com/questions/21968918/java-8-nested-loops-with-streams-performance
   
*/

import java.io.*;
//import java.util.stream.*;
import java.util.*;
import java.lang.Object.*;
import java.util.concurrent.RecursiveAction;
import java.util.concurrent.*;

// a marker for code that you need to implement
class ImplementMe extends RuntimeException {}

// an RGB triple
class RGB {
    public int R, G, B;

    RGB(int r, int g, int b) {
	R = r;
	G = g;
	B = b;
    }

    public String toString() { return "(" + R + "," + G + "," + B + ")"; }

	//helper function for negate, negates RGB values
	public RGB negfun(int max) {
		this.R = max - this.R;
		this.G = max - this.G;
		this.B = max - this.B;
		return this;
	}
/*
	//helper function for gaussianBlur2, applies the filter
	public RGB filterfunc(double[][] filter) {
		int rad = filter.length;
		this.R = IntStream.range(0, filter.length).
				 .map(i -> IntStream.range(-rad, rad)
				 .map(n -> IntStream.range(-rad, rad)
				 .map(m -> {
							double fil_val = filter[n+rad][m+rad];
							int i_r = Math.min(Math.max(n+r,0), 
						   }
	}
*/
	//helper function turns RGB into [R,G,B] int array
	public int[] RGBtoArray(RGB x){
		int[] x = new int[3];
		x[0] = x.R;
		x[1] = x.G;
		x[2] = x.B;
		return x;	
	}
}

// code for creating a Gaussian filter
class Gaussian {

    protected static double gaussian(int x, int mu, double sigma) {
	return Math.exp( -(Math.pow((x-mu)/sigma,2.0))/2.0 );
    }

    public static double[][] gaussianFilter(int radius, double sigma) {
	int length = 2 * radius + 1;
	double[] hkernel = new double[length];
	for(int i=0; i < length; i++)
	    hkernel[i] = gaussian(i, radius, sigma);
	double[][] kernel2d = new double[length][length];
	double kernelsum = 0.0;
	for(int i=0; i < length; i++) {
	    for(int j=0; j < length; j++) {
		double elem = hkernel[i] * hkernel[j];
		kernelsum += elem;
		kernel2d[i][j] = elem;
	    }
	}
	for(int i=0; i < length; i++) {
	    for(int j=0; j < length; j++)
		kernel2d[i][j] /= kernelsum;
	}
	return kernel2d;
    }
}

//MirrorTask extends RecursiveAction for fork-join
class MirrorTask extends RecursiveAction {
	protected RGB[] arr;
	protected RGB[] mirr;
	protected int width, low, high;
	
	protected static final int SEQ_CUTOFF = 10000;

	public MirrorTask(RGB[] a, RGB[] b, int Low, int High, int Width) {
		this.arr = a;
		this.mirr = b;
		//System.arraycopy(a,0,arr,0,a.length);
		this.width = Width;
		this.low = Low;
		this.high = High;
	}

	//called through ForkJoinPool().invoke()
	public void compute() {
		//RGB[] revArr = new RGB[this.arr.length];
	
		if(this.high - this.low <= SEQ_CUTOFF) {
			for(int i = this.low; i < this.high; i++) {
				int col = i % this.width;
				int mir_col = width - col - 1;
				int position = (i / width) * width + mir_col;
				this.mirr[position] = new RGB(this.arr[i].R, this.arr[i].G, this.arr[i].B);			
			}
			//return revArr;
			return;
		}
		
		int mid = (this.high - this.low) / 2 + this.low;
		MirrorTask m1 = new MirrorTask(this.arr, this.mirr, this.low, mid, this.width);
		MirrorTask m2 = new MirrorTask(this.arr, this.mirr, mid, this.high, this.width);
		m1.fork(); //run m1 in different thread
		m2.compute();
		m1.join();

		//join the two arrays and return
		//return concatArr(rev1, rev2);
	}
	/*
	//concat arrays helper function
	RGB[] concatArr(RGB[] a, RGB[] b) {
		List<RGB> both = new ArrayList<RGB>(a.length + b.length);
		Collections.addAll(both, a);
		Collections.addAll(both, b);
		return both.toArray(new RGB[both.size()]);	
	}
	*/
}

//FilTask extends RecursiveAction for fork-join
class FilTask extends RecursiveAction {
	protected RGB[] arr;
	protected RGB[] fil;
	protected int low, high, width, height;
	protected double[][] filter; 
	
	protected static final int SEQ_CUTOFF = 10000;

	public FilTask(RGB[] a, RGB[] b, double[][] f, int Low, int High, int Width, int Height){
		this.arr = a;
		this.fil = b;
		this.filter = f;
		this.low = Low;
		this.high = High;
		this.width = Width;
		this.height = Height;
	}

	//called through ForkJoinPool().invoke()
	public void compute() {
		int fil_len = filter.length;
		int rad = fil_len / 2;

		if((this.high - this.low) <= SEQ_CUTOFF) {
			for(int i = this.low; i < this.high; i++) {
				int c = i % this.width;
				int r = i / this.width;
				double bR = 0.0, bG = 0.0, bB = 0.0; //need to convert to int after
				for(int n = -rad; n <= rad; n++) {
					for(int m = -rad; m <= rad; m++) {
						double fil_val = this.filter[n+rad][m+rad];
						int i_r = Math.min(Math.max(n + r, 0), this.height - 1);
						int i_c = Math.min(Math.max(m + c, 0), this.width - 1);
						RGB pix_elem = this.arr[i_r * this.width + i_c];
						bR += pix_elem.R * fil_val;
						bG += pix_elem.G * fil_val;
						bB += pix_elem.B * fil_val;
					}
				}
				this.fil[i] = new RGB((int) Math.round(bR)
									,(int) Math.round(bG)
									,(int) Math.round(bB)); 
			}
			return;
		}
		int mid = (this.high - this.low) / 2 + this.low;
		FilTask f1 = new FilTask(this.arr, this.fil, this.filter, this.low, mid, this.width, this.height);
		FilTask f2 = new FilTask(this.arr, this.fil, this.filter, mid, this.high, this.width, this.height);
		f1.fork(); //run m1 in different thread
		f2.compute();
		f1.join();
	}

}
// an object representing a single PPM image
class PPMImage {
    protected int width, height, maxColorVal;
    protected RGB[] pixels;

    PPMImage(int w, int h, int m, RGB[] p) {
	width = w;
	height = h;
	maxColorVal = m;
	pixels = p;
    }

	// parse a PPM file to produce a PPMImage
    public static PPMImage fromFile(String fname) throws FileNotFoundException, IOException {
	FileInputStream is = new FileInputStream(fname);
	BufferedReader br = new BufferedReader(new InputStreamReader(is));
	br.readLine(); // read the P6
	String[] dims = br.readLine().split(" "); // read width and height
	int width = Integer.parseInt(dims[0]);
	int height = Integer.parseInt(dims[1]);
	int max = Integer.parseInt(br.readLine()); // read max color value
	br.close();

	is = new FileInputStream(fname);
	    // skip the first three lines
	int newlines = 0;
	while (newlines < 3) {
	    int b = is.read();
	    if (b == 10)
		newlines++;
	}

	int MASK = 0xff;
	int numpixels = width * height;
	byte[] bytes = new byte[numpixels * 3];
        is.read(bytes);
	RGB[] pixels = new RGB[numpixels];
	for (int i = 0; i < numpixels; i++) {
	    int offset = i * 3;
	    pixels[i] = new RGB(bytes[offset] & MASK, bytes[offset+1] & MASK, bytes[offset+2] & MASK);
	}

	return new PPMImage(width, height, max, pixels);
    }

	// write a PPMImage object to a file
    public void toFile(String fname) throws IOException {
	FileOutputStream os = new FileOutputStream(fname);

	String header = "P6\n" + width + " " + height + "\n" + maxColorVal + "\n";
	os.write(header.getBytes());

	int numpixels = width * height;
	byte[] bytes = new byte[numpixels * 3];
	int i = 0;
	for (RGB rgb : pixels) {
	    bytes[i] = (byte) rgb.R;
	    bytes[i+1] = (byte) rgb.G;
	    bytes[i+2] = (byte) rgb.B;
	    i += 3;
	}
	os.write(bytes);
    }

	// implement using Java 8 Streams
    public PPMImage negate() {
	//throw new ImplementMe();
		RGB[] negatedPixels = new RGB[this.pixels.length];
		//copy an array from source array to dest array
		//need to copy to be side-effect-free
		System.arraycopy(this.pixels, 0, negatedPixels, 0, this.pixels.length);
		RGB[] negArray = 
			Arrays.stream(negatedPixels)
			.parallel()
			.map(rgb -> rgb.negfun(this.maxColorVal))
			.toArray(size -> new RGB[size]);
		return new PPMImage(this.width, this.height, this.maxColorVal, negArray);
    } 
    
	// implement using Java's Fork/Join library
    public PPMImage mirrorImage() {
	//throw new ImplementMe();
		RGB[] mirrorPixels = new RGB[this.pixels.length];
		//copy an array from source array to dest array
		//need to copy to be side-effect-free
		//System.arraycopy(this.pixels, 0, mirrorPixels, 0, this.pixels.length);

		int high = mirrorPixels.length;		
		MirrorTask m = new MirrorTask(this.pixels, mirrorPixels, 0, high, this.width);
		//RGB[] mir = m.compute();
		new ForkJoinPool().invoke(m);
		return new PPMImage(this.width, this.height, this.maxColorVal, mirrorPixels);
    }

	// implement using Java's Fork/Join library
    public PPMImage gaussianBlur(int radius, double sigma) {
	//throw new ImplementMe();
		RGB[] gausPixels = new RGB[this.pixels.length];
		//copy an array from source array to dest array
		//need to copy to be side-effect-free
		System.arraycopy(this.pixels, 0, gausPixels, 0, this.pixels.length);

		double[][] filter = Gaussian.gaussianFilter(radius, sigma);
		FilTask f = new FilTask(this.pixels, gausPixels, filter, 0, pixels.length, this.width, this.height);
		new ForkJoinPool().invoke(f);
		return new PPMImage(this.width, this.height, this.maxColorVal, gausPixels);
    }

	// implement using Java 8 Streams
    public PPMImage gaussianBlur2(int radius, double sigma) {
	throw new ImplementMe();
		/*
		RGB[] gausPixels = new RGB[this.pixels.length];
		//copy an array from source array to dest array
		//need to copy to be side-effect-free
		System.arraycopy(this.pixels, 0, gausPixels, 0, this.pixels.length);

		double[][] filter = Gaussian.gaussianFilter(radius, sigma);
		int rad = filter.length / 2;

		RGB[] result = 
					Arrays.stream(gausPixels)
					.parallel()
					.map(rgb -> {	
									Arrays.stream(RGBtoArray(rgb)).
									.parallel()
									.map(elem -> {
													IntStream.range(0, filter.length).
				 									.map(i -> { double bElem = 0.0;
																IntStream.range(-rad, rad)
													.map(n -> IntStream.range(-rad, rad)
				 									.map(m -> {
															double fil_val = filter[n+rad][m+rad];
															int i_r = Math.min(Math.max(n+(i/this.width),0), this.height - 1);
															int i_c = Math.min(Math.max(m+(i%this.width), 0), this.width - 1);
															  })
						  		 							  })
												 })
									
									
								})
					.toArray(size -> new RGB[size]);
	*/
	
    }

	public static void main(String[] args) {
		RGB[] test = new RGB[0];
		PPMImage im = new PPMImage(0,0,0,test);
		try {
			im = PPMImage.fromFile("florence.ppm");
		}catch(FileNotFoundException e) {
			System.out.println("file not found");
		}catch(IOException d){
			System.out.println("d");
		}
		/*
		//negate test
		PPMImage neg = im.negate();
		try {
			neg.toFile("neg.ppm");
		} catch(IOException e) {
			System.out.println("error");
		}
		*/

		/*
		//mirrorImage test
		PPMImage mir = im.mirrorImage();
		try {
			mir.toFile("exmir.ppm");
		} catch(IOException e) {
			System.out.println("error");
		}
		*/

		//gauss1 test
		PPMImage mir = im.gaussianBlur(3, 2.0);
		try {
			mir.toFile("gaus1.ppm");
		} catch(IOException e) {
			System.out.println("error");
		}

		//gauss2 test
		return;
	}
}

/*
class Main {
	
}
*/
