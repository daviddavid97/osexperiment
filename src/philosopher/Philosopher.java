
/**
 * Created by zxy on 10/9/17.
 */
package philosopher;

import java.util.Random;

import static philosopher.main.times;

/**
 *
 */
public class Philosopher implements Runnable{
    private Chopstick right ;
    private Chopstick left;
    private int index;
    private int thinkTime;
    public Philosopher(Chopstick right, Chopstick left, int index, int thinkingTime) {
        super();
        this.right = right;
        this.left = left;
        this.index = index;
        this.thinkTime = thinkingTime;
    }

    @Override
    public void run() {
        try {
            while (!Thread.interrupted()) {
                times = times + 1;
                System.out.println(times/5);
                System.out.println(this + " thinking .......");
                thinking();

                int random = new Random().nextInt(2);
                if (random == 0) {
                    System.out.println(this + " start to eat and take right stick");
                    right.take();
                } else {
                    System.out.println(this + " take left stick");
                    left.take();
                }
                System.out.println(this + " eating");
                thinking();//吃饭
                right.drop();
                left.drop();
            }
        } catch (InterruptedException e1) {
            e1.printStackTrace();
        }
    }

    /**
     * 哲学家思考时间，由thinkingTime因子决定
     * @throws InterruptedException
     */
    private void thinking() throws InterruptedException{
        Thread.sleep(thinkTime*1);
    }

    @Override
    public String toString() {
        return "Philosopher [index=" + index + "]";
    }
}
