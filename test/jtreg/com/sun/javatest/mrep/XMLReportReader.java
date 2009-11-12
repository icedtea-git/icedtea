/*
 * $Id$
 *
 * Copyright 1996-2008 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Sun designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Sun in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 */
package com.sun.javatest.mrep;


import java.io.File;
import java.io.IOException;
import java.text.ParseException;
import java.util.HashMap;
import java.util.Map;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

class XMLReportReader {
    private SAXParser parser;

    private IDHandler handler;

    XMLReportReader() throws SAXException, ParserConfigurationException {
        parser = SAXParserFactory.newInstance().newSAXParser();
        handler = new IDHandler();
    }

    Map readIDs(File file) throws SAXException, IOException {
        parser.parse(file, handler);
        return handler.getMap();
    }

    private static class IDHandler extends DefaultHandler {
        private Map map = new HashMap();

        private long time = 0;

        public void startElement(String namespaceUri, String localName,
                String qualifiedName, Attributes attributes)
                throws SAXException {
            if (qualifiedName.equals(Scheme.TR)) {
                String url = attributes.getValue(Scheme.TR_URL);
                int id = (Integer.valueOf(attributes.getValue(Scheme.TR_WDID)))
                .intValue();
                String status = attributes.getValue(Scheme.TR_STATUS);
                map.put(url, new TestResultDescr(status, id, time));
            }
            if (qualifiedName.equals(Scheme.WD)) {
                Integer id = Integer.valueOf(attributes.getValue(Scheme.WD_ID));
                map.put(id, id);
            }
            if (qualifiedName.equals(Scheme.REPORT)) {
                String dateStr = (String) attributes
                        .getValue(Scheme.REPORT_GENTIME);
                try {
                    time = XMLReportWriter.ISO8601toDate(dateStr).getTime();
                } catch (ParseException e) {
                    throw new SAXException(e);
                }
            }
        }

        public Map getMap() {
            return map;
        }
    }

}
