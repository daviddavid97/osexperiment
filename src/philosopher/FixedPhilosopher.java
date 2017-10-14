package philosopher;

import org.junit.jupiter.api.Test;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by zxy on 10/14/17.
 */
public class FixedPhilosopher {
    @Test
    public static void test() throws InterruptedException {
        ExecutorService executor = Executors.newCachedThreadPool();
        int size=5;
        int thinkingTime=0;
        Chopstick[] chopstick = new Chopstick[size];
        for(int i=0;i<size;i++)
            chopstick[i] = new Chopstick(i);
        //*使用信号量解决问题
        for(int i=0;i<size;i++) {
        }
        executor.shutdownNow();
    }

    }
}
