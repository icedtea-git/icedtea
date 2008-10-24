package sun.applet;


public class RequestQueue {
    PluginCallRequest head = null;
    PluginCallRequest tail = null;
    private int size = 0;

    public void post(PluginCallRequest request) {
    	PluginDebug.debug("Securitymanager=" + System.getSecurityManager());
        if (head == null) {
            head = tail = request;
            tail.setNext(null);
        } else {
            tail.setNext(request);
            tail = request;
            tail.setNext(null);
        }
        
        size++;
    }

    public PluginCallRequest pop() {
        if (head == null)
            return null;

        PluginCallRequest ret = head;
        head = head.getNext();
        ret.setNext(null);

        size--;
        
        return ret;
    }
    
    public int size() {
    	return size;
    }
}