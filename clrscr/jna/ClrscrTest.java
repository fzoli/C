import com.sun.jna.Native;

public class ClrscrTest {
    
    private interface Clrscr extends com.sun.jna.Library {
        Clrscr INSTANCE = (Clrscr) Native.loadLibrary("clrscr", Clrscr.class);
        void clrscr();
    }
    
    public static void main(String[] args) {
        System.out.println("Before clearing.");
        Clrscr.INSTANCE.clrscr();
        System.out.println("After clearing.");
    }
    
}
