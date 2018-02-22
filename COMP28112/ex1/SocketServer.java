import java.io.* ;
import java.net.* ;

class SocketServer {
/* Sets up a server on port 8181 which you can telnet to.
 * If you send a message, it will reply and then disconnect.
*/

private static String serverName;

public static void main(String [] args) throws IOException {

  if (args.length != 1) {
    System.out.println("Need server name for socket server as an argument.");
    System.exit(1);
    }

  serverName = args[0];

  final int port = 8181 ;
  ServerSocket listener = new ServerSocket(port) ;
  System.out.println("Server is running") ;
  // Listen for clients ....
  while (true) {
    Socket client = listener.accept() ;
    new SessionHandler(client).start() ;
    }
  }
// Function to return the server name.
public static String getServerName() {
  return serverName;
  }
}

class SessionHandler extends Thread {
// an instance created by the server for each client

private BufferedReader in ;
private PrintWriter out ;
private Socket client ;

  SessionHandler(Socket s) {
    client = s ;
    }

  public void run() {
    try {
      in = new BufferedReader(new InputStreamReader(
         client.getInputStream())) ;
      out = new PrintWriter(client.getOutputStream(), true) ;

      String clientMessage = in.readLine();

      System.out.println("A client said: " + clientMessage) ;

      if (clientMessage.equals("whoRU"))
        out.println("Server name is " + SocketServer.getServerName());
      else
        out.println("If you ask me nicely, I will tell you who I am");
    } catch (Exception e) { System.out.println("Server error " + e) ; }
    try { client.close() ; }
    catch (Exception e) {;}
  }
}
