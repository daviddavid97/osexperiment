package philosopher;

/**
 * Created by zxy on 10/9/17.
 */
public class main {
    public static int times = 0;
    public static void main(String[] args){
        try {
            /*这是会产生死锁的版本,测试用
            DeadlockPhilosopher.test();
            */
            FixedPhilosopher.test();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
