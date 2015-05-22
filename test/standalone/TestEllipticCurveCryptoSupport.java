/* TestEllipticCurveCryptoSupport -- Check if ECC is available.
   Copyright (C) 2015 Red Hat, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import java.lang.reflect.Field;

import java.security.AlgorithmParameters;
import java.security.KeyFactory;
import java.security.KeyPairGenerator;
import java.security.NoSuchAlgorithmException;
import java.security.Provider;
import java.security.Security;
import java.security.Signature;

import javax.crypto.KeyAgreement;

/**
 * Check whether Elliptic Curve Crypto is available.
 *
 * Based on http://docs.oracle.com/javase/7/docs/technotes/guides/security/SunProviders.html#SunEC
 */
public class TestEllipticCurveCryptoSupport {

  private static final String PKCS11_NAME = "SunPKCS11-NSS";
  private static final String EC_NAME = "SunEC";
  
  public static void main(String[] args) throws Exception {
    boolean possibleProblems = false;
    boolean available = false;
    boolean pkcs11 = false;
    
    if (args.length < 1) {
      System.err.println("TestEllipticCurveCryptoSupport <available=yes|no>");
      System.exit(-1);
    }

    System.err.print("ECC should be available: ");
    if ("yes".equals(args[0])) {
      available = true;
      System.err.println(args[0]);
    } else {
      available = false;
      System.err.println("no");
    }

    // Provider
    Provider provider = Security.getProvider(EC_NAME);
    if (provider == null) {
      System.out.println("No SunEC provider");
    } else {
      System.out.println("SunEC provider is present");
      possibleProblems = true;
    }

    provider = Security.getProvider(PKCS11_NAME);
    if (provider != null)
      {
	pkcs11 = true;
	System.err.println("PKCS11 provider is present; adjusting tests accordingly.");
      }
    else
      System.err.println("PKCS11 provider is present.");
    
    // AlgorithmParameters
    try {
      AlgorithmParameters params = AlgorithmParameters.getInstance("EC");
      System.out.print("EC AlgorithmParameter is present; ");
      if (params.getProvider().getName().equals(PKCS11_NAME))
	{
	  System.out.println("provided by PKCS11");
	}
      else
	{
	  System.out.println("provided by EC provider");
	  possibleProblems = true;
	}
    } catch (NoSuchAlgorithmException e) {
      System.out.println("No EC AlgorithmParameters");
    }
    
    // KeyAgreement
    try {
      KeyAgreement agreement = KeyAgreement.getInstance("ECDH");
      System.out.print("ECDH KeyAgreement is present; ");
      if (agreement.getProvider().getName().equals(PKCS11_NAME))
	{
	  System.out.println("provided by PKCS11");
	}
      else
	{
	  System.out.println("provided by EC provider");
	  possibleProblems = true;
	}
    } catch (NoSuchAlgorithmException e) {
      System.out.println("No ECDH KeyAgreement");
    }
    
    // KeyFactory
    try {
      KeyFactory factory = KeyFactory.getInstance("EC");
      System.out.print("EC KeyFactory is present; ");
      if (factory.getProvider().getName().equals(PKCS11_NAME))
	{
	  System.out.println("provided by PKCS11");
	}
      else
	{
	  System.out.println("provided by EC provider");
	  possibleProblems = true;
	}
    } catch (NoSuchAlgorithmException e) {
      System.out.println("No EC KeyFactory");
    }
    
    // KeyPairGenerator
    try {
      KeyPairGenerator gen = KeyPairGenerator.getInstance("EC");
      System.out.print("EC KeyPairGenerator is present; ");
      if (gen.getProvider().getName().equals(PKCS11_NAME))
	{
	  System.out.println("provided by PKCS11");
	}
      else
	{
	  System.out.println("provided by EC provider");
	  possibleProblems = true;
	}
    } catch (NoSuchAlgorithmException e) {
      System.out.println("No EC KeyFactory");
    }
    
    // Signature
    try {
      Signature sig = Signature.getInstance("NONEwithECDSA");
      System.out.print("EC Signatures are present; ");
      if (sig.getProvider().getName().equals(PKCS11_NAME))
	{
	  System.out.println("provided by PKCS11");
	}
      else
	{
	  System.out.println("provided by EC provider");
	  possibleProblems = true;
	}
    } catch (NoSuchAlgorithmException e) {
      System.out.println("OK: No EC Signatures are present");
    }

    // Full implementation
    try
      {
	Class<?> sunECProvider = Class.forName("sun.security.ec.SunEC");
	System.err.println("sunECProvider class: " + sunECProvider);
	Field f = sunECProvider.getDeclaredField("useFullImplementation");
	f.setAccessible(true);
	boolean implemented = f.getBoolean(null);
	System.err.println("useFullImplementation = " + implemented);
	possibleProblems = implemented;
      }
    catch (ClassNotFoundException e) {
      System.out.println("SunEC provider class not found.");
    }
    catch (NoSuchFieldException e) {
      System.out.println("useFullImplementation field not found in SunEC");
    }
    catch (IllegalAccessException e) {
      System.out.println("Could not access useFullImplementation field");
    }

    System.err.println("SunEC provider available: " + possibleProblems);
    if (available)
      System.exit(possibleProblems ? 0 : 1);
    else
      System.exit(possibleProblems ? 1 : 0);
    
  }    
}
