/* VariableX509TrustManager.java
   Copyright (C) 2009 Red Hat, Inc.

This file is part of IcedTea.

IcedTea is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 2.

IcedTea is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with IcedTea; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version.
*/

package net.sourceforge.jnlp.security;

import java.security.cert.CertPath;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.ArrayList;

import net.sourceforge.jnlp.tools.CertVerifier;

public class HttpsCertVerifier implements CertVerifier {

    private VariableX509TrustManager tm;
    private X509Certificate[] chain;
    private String authType;
    
    public HttpsCertVerifier(VariableX509TrustManager tm, X509Certificate[] chain, String authType) {
        this.tm = tm;
        this.chain = chain;
        this.authType = authType;
    }

    public boolean getAlreadyTrustPublisher() {
        try {
            tm.checkServerTrusted(chain, authType, true);
            return true;
        } catch (CertificateException ce) {
            return false;
        }
    }

    public ArrayList<CertPath> getCerts() {
        
        ArrayList<X509Certificate> list = new ArrayList<X509Certificate>();
        for (int i=0; i < chain.length; i++)
            list.add(chain[i]);

        ArrayList<CertPath> certPaths = new ArrayList<CertPath>();
        
        try {
            certPaths.add(CertificateFactory.getInstance("X.509").generateCertPath(list));
        } catch (CertificateException ce) {
            ce.printStackTrace();
            
            // carry on
        }

        return certPaths; 
    }

    public ArrayList<String> getDetails() {
        // TODO Auto-generated method stub
        return new ArrayList<String>();
    }

    public Certificate getPublisher() {
        // TODO Auto-generated method stub
        return null;
    }

    public Certificate getRoot() {
        // TODO Auto-generated method stub
        return null;
    }

    public boolean getRootInCacerts() {
        // TODO Auto-generated method stub
        return false;
    }

    public boolean hasSigningIssues() {
        // TODO Auto-generated method stub
        return false;
    }

    public boolean noSigningIssues() {
        // TODO Auto-generated method stub
        return false;
    }

}
