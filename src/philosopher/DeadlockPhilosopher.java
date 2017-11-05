
/**
 * Created by zxy on 10/9/17.
 */
package philosopher;



import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


/**
 * 会产生死锁的版本
 *
 */
public class DeadlockPhilosopher {
    public static void test() throws InterruptedException {
        ExecutorService executor = Executors.newCachedThreadPool();
        int size=5;
        int thinkingTime=0;
        Chopstick[] chopstick = new Chopstick[size];
        for(int i=0;i<size;i++)
            chopstick[i] = new Chopstick(i);
        for(int i=0;i<size;i++) {
            executor.execute(new Philosopher(chopstick[i], chopstick[(i + 1) % size], i, thinkingTime));
        }
        executor.shutdownNow();
    }

}
