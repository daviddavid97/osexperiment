
/**
 * Created by zxy on 10/9/17.
 */
package philosopher;
/**
 * 满足：
 * 每根筷子同时只能被一个哲学家获得，若有另外一个哲学家请求获得该筷子，则需要等待
 * 哲学家使用完筷子之后就放回并通知其他哲学家使用
 * @author lecky
 *
 */
public class Chopstick {
    private int index;
    private boolean use = false;

    public Chopstick(int index) {
        super();
        this.index = index;
    }

    @Override
    public String toString() {
        return "Chopstick [index=" + index + "]";
    }

    /*
     * 获得筷子
     * 该筷子被获得之后，当有其他哲学家线程来请求获得时，都需要等待
     */
    public synchronized void take() throws InterruptedException{
        while(use)
            wait();
        use =true;
    }

    /*
     * 归还筷子
     * 当持有该筷子的哲学家使用完毕之后，就将其归还，并通知其他在等待该筷子资源的哲学家
     */
    public synchronized void drop(){
        use = false;
        notifyAll();
    }
}
