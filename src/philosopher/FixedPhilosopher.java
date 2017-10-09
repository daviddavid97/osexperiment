
/**
 * Created by zxy on 10/9/17.
 */
package philosopher;


import org.junit.jupiter.api.Test;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * 破坏产生死锁的循环条件
 * 使第五个哲学家不按照先获得右边筷子，再获得左边筷子的方式进行
 * @author lecky
 *
 */
public class FixedPhilosopher {

    @Test
    public static void test() throws InterruptedException {
        ExecutorService executor = Executors.newCachedThreadPool();
        int size=5;
        int thinkingTime = 0;
        Chopstick[] chopsticks = new Chopstick[size];
        for(int i=0;i<size;i++)
            chopsticks[i]=new Chopstick(i);
        for(int i=0;i<size-1;i++)
            executor.execute(new Philosopher(chopsticks[i], chopsticks[i+1], i, thinkingTime));
        executor.execute(new Philosopher(chopsticks[0], chopsticks[size-1], size, thinkingTime));//更改第五个哲学家获得筷子的顺序
        Thread.sleep(100*1000);
        executor.shutdownNow();
    }

}
