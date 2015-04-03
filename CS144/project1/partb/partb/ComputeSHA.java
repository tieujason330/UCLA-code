//Jason Tieu
//ID: 304047667

//package partb;

import java.io.*;
import java.security.*;

public class ComputeSHA {

	public static void main(String args[]) throws Exception
	{
		if (args.length != 1)
		{
			throw new IllegalArgumentException();
		}
		
		byte[] bytes;

		MessageDigest md = MessageDigest.getInstance("SHA-1");
		
		try 
		{	
			FileInputStream fStream = new  FileInputStream(args[0]);

			int numRead;
			byte[] data = new byte[16384];

			while ((numRead = fStream.read(data)) != -1) 
			{
				md.update(data, 0, numRead);
			}
		}
		catch(FileNotFoundException f)
		{
			System.out.println("No such file or directory");
			throw new FileNotFoundException();
		}
		
		bytes = md.digest();
		String hexstr = "";
		  for (int i=0; i < bytes.length; i++) 
		    hexstr += Integer.toString( ( bytes[i] & 0xff ) + 0x100, 16).substring( 1 );
		  
		  System.out.println(hexstr);
	}
}