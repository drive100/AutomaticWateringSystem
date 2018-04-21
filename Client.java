import java.net.*;
import java.io.*;
import java.util.Scanner;

public class Client
{
    // initialize socket and input output streams
    private Socket socket            = null;
    private DataInputStream  input   = null;
    private DataOutputStream out     = null;
    private BufferedReader d = null;
    private int time = 0;
    // constructor to put ip address and port
    public Client(String address, int port)
    {
        // establish a connection
        // string to read message from input
        String line = "";
        Scanner keyboard = new Scanner(System.in);
        String stringKeyboard = "";
        // keep reading until "Over" is input
        try
        {
            socket = new Socket(address, port);
            System.out.println("Connected");

            // takes input from terminal
            //input  = new DataInputStream(socket.getInputStream());

            // sends output to the socket
            out    = new DataOutputStream(socket.getOutputStream());
            d = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        }
        catch(UnknownHostException u)
        {
            System.out.println(u);
        }
        catch(IOException i)
        {
            System.out.println(i);
        }
        while (!line.equals("eww"))
        {

            try
            {   
                System.out.println(d.readLine());
                if (time == 10)
                    out.writeBytes("10");
                else if(time == 20)
                {
                    out.writeBytes("11");
                    time = 0;
                }
                time++;
            }
            catch(IOException i)
            {
                System.out.println(i);
            }
        }

        // close the connection
        try
        {
            d.close();
            out.close();
            socket.close();
        }
        catch(IOException i)
        {
            System.out.println(i);
        }
    }

    public static void main(String args[])
    {
        Client client = new Client("10.1.99.179", 8133);
    }
}