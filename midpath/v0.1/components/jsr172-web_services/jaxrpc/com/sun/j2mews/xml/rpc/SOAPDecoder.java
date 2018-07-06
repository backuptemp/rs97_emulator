/*
 *  
 * Copyright  1990-2007 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

package com.sun.j2mews.xml.rpc;

import java.io.InputStream;
import java.rmi.MarshalException;
import java.util.Stack;
import java.util.Vector;

import javax.microedition.xml.rpc.ComplexType;
import javax.microedition.xml.rpc.Element;
import javax.microedition.xml.rpc.FaultDetailHandler;
import javax.microedition.xml.rpc.Type;
import javax.xml.namespace.QName;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.rpc.JAXRPCException;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.helpers.DefaultHandler;

public class SOAPDecoder extends DefaultHandler
{
    /**
     * Static internal flag representing a NIL value.
     * This value is converted to a NULL entry in the
     * vectorToArray() function
     */
    private static final Object NIL = new Object();

    /**
     * Parser to decode the xml stream
     */
    private SAXParser parser;

    /**
     * A buffer holding the character data received while decoding
     * a xml element
     */
    private StringBuffer token;

    /**
     * An internal value holding the state of this machine
     * 1 : decoding an Element
     * 2 : decoding a Complex Type
     * 4 : MarshalException occurred
     */
    private int state;

    /**
     * An internal flag, set to mark that the SOAP response
     * contained both an envelope and a body tag
     */
    private int bodyNEnvelope;

    /**
     * An internal flag, set to indicate that the SOAP-ENV:
     * header is being processed
     */
    private boolean processingHeader;

    /**
     * An internal flag, set to true when an element is encountered
     * in the SOAP response which has the attribute of 'nillable=true'
     */
    private boolean isNill;

    /**
     * An internal variable holding the name of the XML element
     * currently being parsed.
     */
    private String eName;

    /**
     * A flag set to true when decoding a SOAP fault
     */
    private boolean faultMode;

    /**
     * A complex type holding the elements of a SOAP fault
     */
    private ComplexType faultCT;

    /**
     * the FaultDetailHandler for custom fault details
     */
    private FaultDetailHandler handler;

    /**
     * Decode fault detail QName
     */
    private QName detailName;

    /**
     * The handler-provided fault 'detail' Element
     */
    private Element handlerDetail;

    /**
     * An internal stack holding the decoded xml values
     */
    private Stack valueStack;

    /**
     * An internal stack holding the type values passed in
     * the decode() method
     */
    private Stack typeStack;

    /**
     * An internal stack holding the state of the machine
     */
    private Stack stateStack;

    /**
     * When an error state occurs, the errorString will have
     * the detail
     */
    private String errString;

    /**
     * SOAP URI
     */
    private static final String SOAP_URI =
        "http://schemas.xmlsoap.org/soap/envelope/";

    /**
     * Constructor.
     */
    public SOAPDecoder() {
        try {
            SAXParserFactory pf = SAXParserFactory.newInstance();
            pf.setNamespaceAware(true);
            pf.setValidating(false);
            parser = pf.newSAXParser();

        } catch (ParserConfigurationException e) {
            throw new RuntimeException("Could not instantiate parser:"
                + e.getMessage());
        } catch (SAXException se) {
            throw new RuntimeException("Could not initialize parser:"
                + se.getMessage());
        }
        token = new StringBuffer();

        valueStack = new Stack();
        typeStack = new Stack();
        stateStack = new Stack();
    }

    /**
     * Decode a SOAP response
     */
    public synchronized Object decode(Type type,
                                      InputStream stream,
                                      String encoding,
                                      long length) throws JAXRPCException
    {
        this.handler = null;
        this.handlerDetail = null;
        this.faultMode = false;
        this.detailName = null;
        this.isNill = false;
        this.bodyNEnvelope = 0;
        this.processingHeader = false;
        clearStacks();

        if (!(type instanceof Element)) {
            // We must start with an Element
            throw new JAXRPCException(
                new MarshalException("Type parameter not instanceof Element"));
        }

        typeStack.push(type);

        try {
            parser.parse(stream, this);
        } catch (RuntimeException re) {
        } catch (SAXParseException spe) {
            stateStack.push(new Integer(4));
            errString = "SAXParseException in response from server";
        } catch (Throwable t) {
            stateStack.push(new Integer(4));
            errString = "Unexpected Exception : " + t.getMessage();
        }

        if (!stateStack.empty()) {
            state = ((Integer)stateStack.pop()).intValue();
        } else if (bodyNEnvelope != 0) {
            state = 4;
            errString = "(1) Missing end tag for Body or Envelope";
        } else {
            state = 0;
        }

        if (state == 4) {
            throw new JAXRPCException(new MarshalException(errString));
        }

        if (valueStack.isEmpty()) {
            Element e = (Element)type;
            if (e.isNillable || e.isOptional) {
                return null;
            } else {
                throw new JAXRPCException(new MarshalException(
                    "Missing return data in response from server"));
            }
        }

        return vectorToArray(valueStack.pop());
    }

    /**
     * Decode an occurrence of a SOAP Fault
     */
    public synchronized Object decodeFault(FaultDetailHandler handler,
                                           InputStream stream,
                                           String encoding,
                                           long length) throws JAXRPCException
    {
        this.handler = handler;
        this.handlerDetail = null;
        this.faultMode = true;
        this.detailName = null;
        this.isNill = false;
        this.bodyNEnvelope = 0;
        this.processingHeader = false;
        clearStacks();

        faultCT = new ComplexType();
        faultCT.elements = new Element[4];
        faultCT.elements[0] = new Element(
            new QName("", "faultcode"),
            Type.STRING);
        faultCT.elements[1] = new Element(
            new QName("", "faultstring"),
            Type.STRING);
        faultCT.elements[2] = new Element(
            new QName("", "faultactor"),
            Type.STRING,
            0,
            1,
            false);
        faultCT.elements[3] = new Element(
            new QName("", "detail"),
            Type.STRING,
            0,
            1,
            false);
        Element faultType = new Element(
            new QName(SOAP_URI, "Fault"),
            faultCT);

        typeStack.push(faultType);

        try {
            parser.parse(stream, this);
        } catch (RuntimeException re) {
        } catch (SAXParseException spe) {
            stateStack.push(new Integer(4));
            errString = "SAXParseException in response from server";
        } catch (Throwable t) {
            t.printStackTrace();
        }

        if (!stateStack.empty()) {
            state = ((Integer)stateStack.pop()).intValue();
        } else if (bodyNEnvelope != 0) {
            state = 4;
            errString = "(2)Missing end tag for Body or Envelope";
        } else {
            state = 0;
        }

        if (state == 4) {
            throw new JAXRPCException(new MarshalException(errString));
        }

        // fault should be a 4 element array
        Object[] fault = (Object[])valueStack.pop();

        token.delete(0, token.length());
        if (fault[0] != null) {
            token.append("\n[Code:   " + ((String)fault[0]) + "] ");
        }
        if (fault[1] != null) {
            token.append("\n[String: " + ((String)fault[1]) + "] ");
        }
        if (fault[2] != null) {
            token.append("\n[Actor:  " + ((String)fault[2]) + "] ");
        }

        if (detailName == null) {
            if (fault[3] != null) {
                token.append("\n[Detail: " + ((String)fault[3]) + "] ");
            }
            return token.toString();
        } else {
            return
                new Object[] {
                    token.toString(),
                    detailName,
                    vectorToArray(fault[3]) };
        }

    }

    /** Clear the state stacks of this machine
     */
    private void clearStacks() {
        stateStack.removeAllElements();
        typeStack.removeAllElements();
        valueStack.removeAllElements();
    }

    /**
     * Given a Type, create its corresponding Object graph.
     */
    private Object graph(ComplexType ct) throws JAXRPCException {
        try {

            Object[] o = new Object[ct.elements.length];
            for (int i = 0; i < ct.elements.length; i++) {
                if (ct.elements[i].isArray) {
                    o[i] = new TypedVector(
                        ct.elements[i].contentType.value,
                        ct.elements[i].isNillable,
                        ct.elements[i].minOccurs, ct.elements[i].maxOccurs,
                        false);
                }
            }
            return o;

        } catch (Throwable t) {
            throw new JAXRPCException("Invalid Type for Output");
        }
    }

    /**
     * Set the state of this "machine" and set up the matching
     * object graphs and stacks
     *
     * @param name the name of the xml parameter which we have
     *        received a startElement event for
     */
    private void startState(String uri, String name) {
        Type top = (Type)typeStack.peek();

        if (top instanceof Element) {
            // we're just starting...
            // we assert that top is Element (checked in invoke())
            Element e = (Element)top;
            validate(e, uri, name);

            if (e.contentType.value < 8) {
                if (!e.isArray) {
                    // We're dealing with a simple type
                    stateStack.push(new Integer(1));
                    // don't do anything with the type stack
                    // or value stack
                } else {
                    // We are handling an array of a simple type elements.
                    // Wrap this array into ComplexType.
                    ComplexType ct = new ComplexType();
                    ct.elements = new Element[1];
                    ct.elements[0] = e;

                    typeStack.push(ct);
                    if (!isNill) {
                        // Push a graph of the complex type onto the value stack
                        Object[] obj = new Object[1];
                        obj[0] = new TypedVector(e.contentType.value,
                            e.isNillable, e.minOccurs, e.maxOccurs, true);
                        valueStack.push(obj);
                    }
                    // Push the state onto the state stack
                    stateStack.push(new Integer(2));
                }
            } else if (e.contentType.value == 8) {
                // We're decoding a complex type.
                // Push the complex type onto the type stack
                typeStack.push(e.contentType);
                if (!isNill) {
                    // Push a graph of the complex type onto the value stack
                    valueStack.push(graph((ComplexType)e.contentType));
                }
                // Push the state onto the state stack
                stateStack.push(new Integer(2));
            } else {
                // We're dealing with an indirected Element?
                // Shouldn't have to do anything. Spec sort of
                // prohibits the content type of an Element to
                // be another Element
            }
        } else if (top instanceof ComplexType) {
            // Whenever we're decoding inside a complex type,
            // its possible that one of the elements will begin
            // a new level of depth - such as an element of a
            // complex type whose "contentType" value is yet another
            // complex type (the nested complex types example).
            ComplexType ct = (ComplexType)top;
            int index = -1;

            for (int i = 0; i < ct.elements.length; i++) {
                if (matchType(ct.elements[i].name, uri, name)) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                // We may be decoding a custom fault detail
                if (faultMode && handler != null
                    && detailName == null && ct == faultCT)
                {
                    detailName = new QName(uri, name);
                    Element e = handler.handleFault(detailName);
                    if (e != null) {
                        handlerDetail = e;
                        if (handlerDetail.contentType.value == 8) {
                            // We've got a nested complex type,
                            // push it on the stack
                            typeStack.push(handlerDetail.contentType);
                            if (!isNill) {
                                // Push a graph of the complex type onto
                                // the value stack
                                valueStack.push(
                                    graph((ComplexType)handlerDetail.contentType));
                                // Push the state onto the state stack
                            }
                            stateStack.push(new Integer(2));
                        } else {
                            // We've got a nested simple type,
                            // push it on the stack for this special case
                            typeStack.push(handlerDetail);
                            // Push the state onto the state stack
                            stateStack.push(new Integer(1));
                        }
                        return;
                    }
                }

                // If we can't find the element in the complex type
                // and it wasn't a detail in a Fault, we bail (or, if
                // it was the detail in a Fault but the fault handler
                // was unable to give us a Type for it)
                if (handlerDetail == null) {
                    // This is an error
                    stateStack.push(new Integer(4));
                    errString = "Invalid Element in Response: " + name;
                    throw new RuntimeException();
                }
            }

            validate (ct.elements[index], uri, name);

            if (ct.elements[index].contentType.value == 8) {
                // We've got a nested complex type, push it on the stack
                typeStack.push(ct.elements[index].contentType);
                if (!isNill) {
                    // Push a graph of the complex type onto the value stack
                    valueStack.push(
                        graph((ComplexType)ct.elements[index].contentType));
                }
                // Push the state onto the state stack
                stateStack.push(new Integer(2));
            }
        }

    }

    /**
     * matchType() returns true if the qname and uri and name match,
     * false otherwise
     */
    private static boolean matchType(QName qname, String uri, String name) {
        return (qname.getNamespaceURI().equals(uri) &&
                qname.getLocalPart().equals(name));
    }

    /**
     * validate() mandates that the qname matches the uri and name,
     * and throws an exception otherwise
     */
    private void validate(Element e, String uri, String name) {
        if (!name.equals(e.name.getLocalPart())) {
            errString = "Invalid Element Name From Server: " + name + ", " +
                        "expected: " + e.name.getLocalPart();
            stateStack.push(new Integer(4));
            throw new RuntimeException();
        } else if (!uri.equals(e.name.getNamespaceURI())) {
            errString = "Invalid Namespace URI From Server: " + uri + ", " +
                        "expected: " + e.name.getNamespaceURI() +
                        " for element: " + name;
            stateStack.push(new Integer(4));
            throw new RuntimeException();
        } else if (isNill && !e.isNillable) {
            errString = "Nillable mismatch from server for: " + name;
            stateStack.push(new Integer(4));
            throw new RuntimeException();
        }
    }

    /**
     * Creates a new object of the given type and initializes it with
     * the value that is default for this type of object.
     *
     * @param type type of object to create
     * @return Object initialized with a default value for this type of object
     * or null if the given type is not a primitive type (for ex., String)
     */
    private static Object getObjWithDefaultValue(int type) {
        Object retObj = null;

        switch (type) {
            case 0: // Type.BOOLEAN
                retObj = (Object) new Boolean(false);
                break;

            case 1: // Type.BYTE
                retObj = (Object) new Byte((byte)0);
                break;

            case 2: // Type.SHORT
                retObj = (Object) new Short((short)0);
                break;

            case 3: // Type.INT
                retObj = (Object) new Integer(0);
                break;

            case 4: // Type.LONG
                retObj = (Object) new Long(0);
                break;

            case 5: // Type.FLOAT
                retObj = (Object) new Float(0.0f);
                break;

            case 6: // Type.DOUBLE
                retObj = (Object) new Double(0.0d);
                break;
        }
        return retObj;
    }

    /**
     * endState is called when an xml 'end' element is encountered,
     * signaling the end of the parsing of that element
     */
    private void endState(String uri, String name) {
        Type top = (Type)typeStack.pop();
        state = ((Integer)stateStack.peek()).intValue();

        switch (state) {
        // assert state can never be 0, as its set in startState
        case 1:
            Element e = (Element)top;

            if (detailName != null &&
                    matchType(detailName, uri, name) &&
                        handlerDetail != null) {
                // If we happen to be done with our custom
                // fault detail
                stateStack.pop();
                if (!isNill) {
                    Object fd = tokenToObject(e.contentType, token.toString());
                    Object[] f = (Object[])valueStack.peek();
                    f[3] = fd;
                }
                return;
            }

            // Make sure our namespaces haven't magically changed
            validate(e, uri, name);

            // We've completed decoding a simple type
            if (!isNill) {
                valueStack.push(tokenToObject(e.contentType, token.toString()));
            }
            stateStack.pop();
            return;

        case 2:
            // We've completed either one of the elements of
            // the complex type, or the complex type itself
            ComplexType ct = (ComplexType)top;
            int index = -1;
            for (int i = 0; i < ct.elements.length; i++) {
                if (matchType(ct.elements[i].name, uri, name)) {
                    index = i;
                    break;
                }
            }

            // This will be the value we've just decoded
            Object ctVal = null;
            if (index == -1) {
                // Either we've completed our complex type, or
                // there's an error

                // We pop the state stack, because we believe to
                // be completed with this complex type
                stateStack.pop();

                if (!stateStack.isEmpty()) {
                    // If the state stack is not empty at this point,
                    // it means this complex type is nested within
                    // another complex type
                    state = ((Integer)stateStack.peek()).intValue();
                    // The state should be 2, if its not, its a
                    // decoding error
                    if (state != 2) {
                        stateStack.push(new Integer(4));
                        errString =
                            "(1):Mismatch between server response and type map";
                        throw new RuntimeException();
                    }
                    // This cast should be safe, because we've verified
                    // from the state stack above that the type on the
                    // type stack is complex
                    top = (Type)typeStack.pop();
                    ct = (ComplexType)top;
                    index = -1;
                    for (int i = 0; i < ct.elements.length; i++) {
                        if (matchType(ct.elements[i].name, uri, name)) {
                            index = i;
                            break;
                        }
                    }

                    if (index == -1) {
                        if (detailName != null &&
                            matchType(detailName, uri, name) &&
                            handlerDetail != null)
                        {
                            // If we happen to be done with our custom
                            // fault detail
                            Object fd = valueStack.pop();
                            Object[] f = (Object[])valueStack.peek();
                            f[3] = fd;
                            typeStack.push(top);
                            return;
                        }
                        stateStack.push(new Integer(4));
                        errString =
                            "(2):Mismatch between server response and type map";
                        throw new RuntimeException();
                    }

                    // We now know we've completed a complex type which is
                    // nested inside another complex type. We pop the completed
                    // inner complex type off the value stack, and set that to
                    // be the 'ctVal'
                    if (!isNill) {
                        ctVal = valueStack.pop();
                    }

                    // We drop thru this inner nested condition, having set
                    // 'top' - the parent of this nested complex type,
                    // 'ct' - the casted version of 'top',
                    // the index of this nested complex type within 'top',
                    // the value of this nested complex type from the value
                    // stack, and the state instance variable

                } else {
                    // the state stack is empty, which means we could be done.
                    // There should be one more type on the type stack, of
                    // type Element, which matches this name/uri
                    top = (Type)typeStack.pop();
                    if (top instanceof Element &&
                            matchType(((Element)top).name, uri, name)) {
                        Element el = (Element)top;

                        if (el.contentType.value == 8 &&
                                !valueStack.isEmpty()) {
                            ct = (ComplexType)(el.contentType);
                            Object[] els = (Object[])valueStack.peek();

                            for (int i = 0; i < ct.elements.length; i++) {
                                if (els[i] == null &&
                                        ct.elements[i].isArray) {
                                    els[i] = getObjWithDefaultValue(
                                        ct.elements[i].contentType.value);
                                }
                            }
                        }

                        return;
                    } else {
                        stateStack.push(new Integer(4));
                        errString =
                            "(3):Mismatch between server response and type map";
                        throw new RuntimeException();
                    }
                }
            }

            // We've completed one of the elements of the
            // complex type we're decoding
            Object[] els = (Object[])valueStack.peek();

            // assert that index must be valid
            validate(ct.elements[index], uri, name);

            if (faultCT != null &&
                    faultCT.elements == ct.elements &&
                        index == 3 &&
                            handlerDetail != null) {
                // This means we have decoded a custom fault detail
                // and the value has already been set in the
                ctVal = els[3];
            } else if (ctVal == null && !isNill) {
                ctVal = tokenToObject(ct.elements[index].contentType,
                                      token.toString());
            }

            if (ct.elements[index].isArray) {
                // This is the case where one of the elements of
                // the complex type is an array of elements, and
                // we've just completed one of those elements
                TypedVector v = null;
                if (els[index] == null) {
                    v = new TypedVector(ct.elements[index].contentType.value,
                                        ct.elements[index].isNillable,
                                        ct.elements[index].minOccurs,
                                        ct.elements[index].maxOccurs,
                                        false);
                    els[index] = v;
                } else {
                    v = (TypedVector)els[index];
                }

                if (ctVal == null) {
                    v.addElement(NIL);
                } else {
                    v.addElement(ctVal);
                }
            } else {
                els[index] = ctVal;
            }

            // before we return, we'll push the parent type back
            // on the type stack to keep goin with this complex type
            typeStack.push(top);

            return;
        }
    }

    /**
     * Convert any occurrence of Vectors in the given Object
     * to Object arrays.
     *
     * @param o the Object to convert. If the Object is an array,
     *          the method is run recursively on each element of
     *          the array.
     * @throws JAXRPCException if the number parameters is incorrect.
     */
    private static Object vectorToArray(Object o) {
        if (o instanceof Object[]) {
            Object arrayEl;
            Object[] set = (Object[])o;

            for (int i = 0; i < set.length; i++) {
                if (set[i] instanceof TypedVector) {
                    TypedVector v = (TypedVector)set[i];
                    int j, k;
                    int len = v.size();

                    if (len < v.minOccurs) {
                        throw new JAXRPCException("Not enough array elements");
                    } else if (v.maxOccurs > 0 && len > v.maxOccurs) {
                        throw new JAXRPCException("Too many array elements");
                    }

                    // If the array is empty, don't include 'null' element
                    // into it (instead create an array without elements).
                    if (len == 1 && v.minOccurs < 1) {
                        if (v.elementAt(0) == NIL) {
                            len--;
                        } else if (v.type == 7) {
                            String s = (String) v.elementAt(0);
                            if (s.length() == 0) {
                                len--;
                            }
                        }
                    }

                    switch (v.type) {
                    case 0: // Type.BOOLEAN
                        if (v.nillable) {
                            Boolean[] tmp = new Boolean[len];
                            for (j = 0; j < tmp.length; j++) {
                                arrayEl = v.elementAt(j);
                                if (arrayEl != NIL) {
                                    tmp[j] = (Boolean)arrayEl;
                                }
                            }
                            set[i] = tmp;
                        } else {
                            boolean tmp[] = new boolean[len];
                            for (k = 0; k < tmp.length; k++) {
                                arrayEl = v.elementAt(k);
                                if (arrayEl != NIL) {
                                    tmp[k] = ((Boolean)arrayEl).booleanValue();
                                }
                            }
                            set[i] = tmp;
                        }
                        break;
                    case 1: // Type.BYTE
                        if (v.nillable) {
                            Byte[] tmp = new Byte[len];
                            for (j = 0; j < tmp.length; j++) {
                                arrayEl = v.elementAt(j);
                                if (arrayEl != NIL) {
                                    tmp[j] = (Byte)arrayEl;
                                }
                            }
                            set[i] = tmp;
                        } else {
                            byte tmp[] = new byte[len];
                            for (k = 0; k < tmp.length; k++) {
                                arrayEl = v.elementAt(k);
                                if (arrayEl != NIL) {
                                    tmp[k] = ((Byte)arrayEl).byteValue();
                                }
                            }
                            set[i] = tmp;
                        }
                        break;
                    case 2: // Type.SHORT
                        if (v.nillable) {
                            Short[] tmp = new Short[len];
                            for (j = 0; j < tmp.length; j++) {
                                arrayEl = v.elementAt(j);
                                if (arrayEl != NIL) {
                                    tmp[j] = (Short)arrayEl;
                                }
                            }
                            set[i] = tmp;
                        } else {
                            short tmp[] = new short[len];
                            for (k = 0; k < tmp.length; k++) {
                                arrayEl = v.elementAt(k);
                                if (arrayEl != NIL) {
                                    tmp[k] = ((Short)arrayEl).shortValue();
                                }
                            }
                            set[i] = tmp;
                        }
                        break;
                    case 3: // Type.INT
                        if (v.nillable) {
                            Integer[] tmp = new Integer[len];
                            for (j = 0; j < tmp.length; j++) {
                                arrayEl = v.elementAt(j);
                                if (arrayEl != NIL) {
                                    tmp[j] = (Integer)arrayEl;
                                }
                            }
                            set[i] = tmp;
                        } else {
                            int tmp[] = new int[len];
                            for (k = 0; k < tmp.length; k++) {
                                arrayEl = v.elementAt(k);
                                if (arrayEl != NIL) {
                                    tmp[k] = ((Integer)arrayEl).intValue();
                                }
                            }
                            set[i] = tmp;
                        }
                        break;
                    case 4: // Type.LONG
                        if (v.nillable) {
                            Long[] tmp = new Long[len];
                            for (j = 0; j < tmp.length; j++) {
                                arrayEl = v.elementAt(j);
                                if (arrayEl != NIL) {
                                    tmp[j] = (Long)arrayEl;
                                }
                            }
                            set[i] = tmp;
                        } else {
                            long[] tmp = new long[len];
                            for (k = 0; k < tmp.length; k++) {
                                arrayEl = v.elementAt(k);
                                if (arrayEl != NIL) {
                                    tmp[k] = ((Long)arrayEl).longValue();
                                }
                            }
                            set[i] = tmp;
                        }
                        break;
                    case 5: // Type.FLOAT
                        if (v.nillable) {
                            Float[] tmp = new Float[len];
                            for (j = 0; j < tmp.length; j++) {
                                arrayEl = v.elementAt(j);
                                if (arrayEl != NIL) {
                                    tmp[j] = (Float)arrayEl;
                                }
                            }
                            set[i] = tmp;
                        } else {
                            float tmp[] = new float[len];
                            for (k = 0; k < tmp.length; k++) {
                                arrayEl = v.elementAt(k);
                                if (arrayEl != NIL) {
                                    tmp[k] = ((Float)arrayEl).floatValue();
                                }
                            }
                            set[i] = tmp;
                        }
                        break;
                    case 6: // Type.DOUBLE
                        if (v.nillable) {
                            Double[] tmp = new Double[len];
                            for (j = 0; j < tmp.length; j++) {
                                arrayEl = v.elementAt(j);
                                if (arrayEl != NIL) {
                                    tmp[j] = (Double)arrayEl;
                                }
                            }
                            set[i] = tmp;
                        } else {
                            double tmp[] = new double[len];
                            for (k = 0; k < tmp.length; k++) {
                                arrayEl = v.elementAt(k);
                                if (arrayEl != NIL) {
                                    tmp[k] = ((Double)arrayEl).doubleValue();
                                }
                            }
                            set[i] = tmp;
                        }
                        break;
                    case 7: // Type.STRING
                        // According to the section 8.2.4 of J2ME Web Services
                        // 1.0 spec, array of String must be encoded into array
                        // of Object each element of which is String.
                        Object[] tmp = new Object[len];
                        for (j = 0; j < tmp.length; j++) {
                            arrayEl = v.elementAt(j);
                            if (arrayEl != NIL) {
                                tmp[j] = arrayEl;
                            }
                        }
                        set[i] = tmp;
                        break;
                    default:
                        Object[] l = new Object[len];
                        v.copyInto(l);
                        set[i] = vectorToArray(l);

                        // Don't create nested arrays if the inner array
                        // is empty.
                        if (set[i] instanceof Object[] &&
                                isArrayEmpty((Object[])set[i])) {
                            set[i] = new Object[0];
                        }
                    } // switch

                    if (v.isArrayWrapper) {
                        // Result is an array of elements of a simple type
                        // so we assert that there is only one element in
                        // the set (otherwise the type would be ComplexType).
                        return (Object) set[0];
                    }
                } else {
                    if (set[i] == NIL) {
                        set[i] = null;
                    } else if (set[i] instanceof Object[]) {
                        set[i] = vectorToArray(set[i]);
                    }
                }
            }
        }

        return o;
    }

    private static boolean isArrayEmpty(Object[] array) {
        for (int i = 0; i < array.length; i++) {
            if (array[i] instanceof Object[]) {
                if (isArrayEmpty((Object[])array[i])) {
                    continue;
                } else {
                    return false;
                }
            }
            if (array[i] == null) {
                continue;
            }
            return false;
        }

        return true;
    }

    private Object tokenToObject(Type type, String token) {
        // Token value "" is valid for String object
        if (token == null || ("".equals(token) && type.value != 7)) {
            return null;
        }

        switch (type.value) {
        case 0: // Type.BOOLEAN
            token = token.toLowerCase();
            if (token.equals("true") || token.equals("1")) {
                return new Boolean(true);
            } else if (token.equals("false") || token.equals("0")) {
                return new Boolean(false);
            } else {
                stateStack.push(new Integer(4));
                errString = "Expected Boolean, received: " + token;
                throw new RuntimeException();
            }
        case 1: // Type.BYTE
            try {
                if (token.startsWith("+")) {
                    return new Byte(Byte.parseByte(
                        token.substring(1)));
                } else {
                    return new Byte(Byte.parseByte(token));
                }
            } catch (NumberFormatException nfe) {
                stateStack.push(new Integer(4));
                errString = "Expected Byte, received: " + token;
                throw new RuntimeException();
            }
        case 2: // Type.SHORT
            try {
                if (token.startsWith("+")) {
                    return new Short(Short.parseShort(
                        token.substring(1)));
                } else {
                    return new Short(Short.parseShort(token));
                }
            } catch (NumberFormatException nfe) {
                stateStack.push(new Integer(4));
                errString = "Expected Short, received: " + token;
                throw new RuntimeException();
            }
        case 3: // Type.INT
            try {
                if (token.startsWith("+")) {
                    return new Integer(Integer.parseInt(
                        token.substring(1)));
                } else {
                    return new Integer(Integer.parseInt(token));
                }
            } catch (NumberFormatException nfe) {
                stateStack.push(new Integer(4));
                errString = "Expected Integer, received: " + token;
                throw new RuntimeException();
            }
        case 4: // Type.LONG
            try {
                if (token.startsWith("+")) {
                    return new Long(Long.parseLong(
                        token.substring(1)));
                } else {
                    return new Long(Long.parseLong(token));
                }
            } catch (NumberFormatException nfe) {
                stateStack.push(new Integer(4));
                errString = "Expected Long, received: " + token;
                throw new RuntimeException();
            }
        case 5: // Type.FLOAT
                try {
                    if (token.startsWith("+")) {
                        return new Float(Float.parseFloat(
                                         token.substring(1)));
                    } else if (token.startsWith("NaN")) {
			return new Float(Float.NaN);
		    } else if (token.startsWith("INF")) {
			return new Float(Float.POSITIVE_INFINITY);
		    } else if (token.startsWith("-INF")) {
			return new Float(Float.NEGATIVE_INFINITY);
		    } else {
                        return new Float(Float.parseFloat(token));
                    }
                } catch (NumberFormatException nfe) {
                    stateStack.push(new Integer(4));
                    errString = "Expected Float, received: " + token;
                    throw new RuntimeException();
                }
        case 6: // Type.DOUBLE
                try {
                    if (token.startsWith("+")) {
                        return new Double(Double.parseDouble(
                                          token.substring(1)));
                    } else if (token.startsWith("NaN")) {
			return new Double(Double.NaN);
		    } else if (token.startsWith("INF")) {
			return new Double(Double.POSITIVE_INFINITY);
		    } else if (token.startsWith("-INF")) {
			return new Double(Double.NEGATIVE_INFINITY);
                    } else {
                        return new Double(Double.parseDouble(token));
                    }
                } catch (NumberFormatException nfe) {
                    stateStack.push(new Integer(4));
                    errString = "Expected Double, received: " + token;
                    throw new RuntimeException();
                }

        case 7: // Type.STRING
            return token;

        default:
            stateStack.push(new Integer(4));
            errString = "Unable to decode type: " +
                type.value + ", for token: " + token;
            throw new RuntimeException();
        }
    }

/***************** XML Parser *********************/

    public void startElement(String uri,
                             String localName,
                             String name,
                             Attributes attrs)
    {
        eName = localName;
        if (eName == null || eName.length() == 0) {
            eName = name;
        }

        // simple test to see if we've made it inside the body yet
        if (eName.toLowerCase().equals("envelope") ||
            eName.toLowerCase().equals("body"))
        {
            if (!uri.equals(SOAP_URI)) {
                errString = "Invalid URI From Server: " + uri + ", " +
                            "expected: " + SOAP_URI;
                stateStack.push(new Integer(4));
                throw new RuntimeException();
            }
            bodyNEnvelope++;
            return;
        }

        // the header element may appear as the first child of the
        // the envelope element.
        if (bodyNEnvelope == 1 &&
            uri.equals(SOAP_URI) &&
            eName.toLowerCase().equals("header"))

        {
            processingHeader = true;
            return;
        }

        // while processing the header element we can ignore anything
        // that does not have mustUnderstand = "1", if it does then
        // refuse to process the message. We understand no headers...
        if (processingHeader = true) {
            String mustUnderstand = attrs.getValue(SOAP_URI, "mustUnderstand");
            if (mustUnderstand != null && mustUnderstand.equals("1")) {
                errString = "Unsupported header element with mustUnderstand";
                stateStack.push(new Integer(4));
                throw new RuntimeException();
            }
        }

        if (bodyNEnvelope != 2) {
            errString = "Missing SOAP Body or Envelope";
            stateStack.push(new Integer(4));
            throw new RuntimeException();
        }

        if (isNill) {
            // This means that an element was marked as being
            // nill, but instead has some element inside it!
            errString = "Nillable element contains value: " + eName;
            stateStack.push(new Integer(4));
            throw new RuntimeException();
        }

        String attr = attrs.getValue(
            "http://www.w3.org/2001/XMLSchema-instance", "nil");
        isNill = (attr != null && (
            attr.toLowerCase().equals("true") ||
            attr.equals("1")));

        token.delete(0, token.length());
        startState(uri, eName);
    }

    public void endElement(String uri, String localName, String name) {
        eName = localName;
        if (eName == null || eName.length() == 0) {
            eName = name;
        }

        if (eName.toLowerCase().equals("envelope") ||
            eName.toLowerCase().equals("body"))
        {
            if (!uri.equals(SOAP_URI)) {
                errString = "Invalid URI From Server: " + uri + ", " +
                            "expected: " + SOAP_URI;
                stateStack.push(new Integer(4));
                throw new RuntimeException();
            }
            bodyNEnvelope--;
            return;
        }

        endState(uri, eName);
        isNill = false;

        if (bodyNEnvelope == 1 &&
            uri.equals(SOAP_URI) &&
            eName.toLowerCase().equals("header"))
        {
            processingHeader = false;
            return;
        }
    }

    /**
     * Overrides the method of the base class.
     */
    public void characters(char[] chars, int start, int len) {
        // Assert token != null
        token.append(chars, start, len);
    }
}

/**
 * A special holder class which holds an array of
 * values of a certain type while decoding
 */
class TypedVector extends Vector {
    public int type;
    public boolean nillable;
    public int minOccurs;
    public int maxOccurs;
    public boolean isArrayWrapper;

    TypedVector(int _type, boolean _nillable, int _minOccurs, int _maxOccurs,
                boolean wrapper) {
        super();
        type = _type;
        nillable = _nillable;
        minOccurs = _minOccurs;
        maxOccurs = _maxOccurs;
        isArrayWrapper = wrapper;
    }
}
