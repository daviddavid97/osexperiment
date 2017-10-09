
/**
 * Created by zxy on 10/9/17.
 */
package philosopher;


import org.junit.jupiter.api.Test;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


/**
 * 会产生死锁的版本
 * 5个哲学家，5只筷子，每个哲学家吃饭之前需要先拿到右边的筷子，然后再拿到左边的筷子
 * 之后才能吃饭
 * @author lecky
 *
 */
public class DeadlockPhilosopher {
    @Test
    public static void test() throws InterruptedException {
        ExecutorService executor = Executors.newCachedThreadPool();
        int size=5;
        int thinkingTime=0;
        Chopstick[] chopstick = new Chopstick[size];
        for(int i=0;i<size;i++)
            chopstick[i] = new Chopstick(i);
        for(int i=0;i<size;i++)
            executor.execute(new Philosopher(chopstick[i], chopstick[(i+1)%size], i, thinkingTime));
        Thread.sleep(4*1000);
        executor.shutdownNow();
    }

}
