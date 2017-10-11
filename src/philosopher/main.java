package philosopher;

/**
 * Created by zxy on 10/9/17.
 */
public class main {
    public static int times = 0;
    public static void main(String[] args){
        try {
            DeadlockPhilosopher.test();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
