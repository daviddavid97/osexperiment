package philosopher;

/**
 * Created by zxy on 10/9/17.
 */
public class main {
    public static void main(String[] args){
        try {
            FixedPhilosopher.test();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
