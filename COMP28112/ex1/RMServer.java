import java.rmi.registry.* ;
import java.rmi.RemoteException ;
import java.rmi.server.UnicastRemoteObject ;

public class RMServer implements RemoteServer {
      private static String serverName;

      public String getId(String s) throws RemoteException {
        System.out.println("A client said: " + s) ;

        if (s.equals("whoRU"))
          return "Server name is " + serverName;
        else
          return "If you ask me nicely, I will tell you who I am";
      }

      public static void main(String[] args) {
         try {
          if (args.length != 1) {
            System.out.println("Need server name for socket server as an argument.");
            System.exit(1);
            }

           serverName = args[0];

           RMServer ms = new RMServer() ;
           RemoteServer stub = (RemoteServer) UnicastRemoteObject.exportObject(ms, 0) ;

           Registry registry = LocateRegistry.getRegistry() ;
           registry.rebind("myIDserver", stub) ;
           System.out.println("Server is running") ;
         } catch (Exception e) {
           System.out.println("Server failed with exception " + e) ;
           System.exit(1) ;
         }
      }
}
