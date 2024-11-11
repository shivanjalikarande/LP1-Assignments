import java.util.Vector;
import java.util.concurrent.Semaphore;

class Container {
    Vector<Integer> buffer = new Vector<>(100);
    static final int BUFFER_SIZE = 100;

    // Semaphores for synchronization
    static Semaphore con = new Semaphore(0); // Counts filled slots
    static Semaphore prod = new Semaphore(1); // Mutex for buffer access
    static Semaphore limit = new Semaphore(BUFFER_SIZE); // Counts empty slots

    // Producer method
    void put(int item) {
        try {
            limit.acquire(); // Wait for an empty slot
            prod.acquire(); // Lock buffer for exclusive access

            buffer.add(item); // Produce item
            System.out.println("Producer produced item " + item);
            System.out.println("No. of items produced: " + buffer.size());
            System.out.println("Remaining production capacity: " + limit.availablePermits());
            System.out.println();

            prod.release(); // Unlock buffer
            con.release(); // Signal a filled slot is available
        } catch (Exception e) {
            System.out.println("Error: " + e);
        }
    }

    // Consumer method
    void get() {
        try {
            con.acquire(); // Wait for a filled slot
            prod.acquire(); // Lock buffer for exclusive access

            if (buffer.size() > 0) {
                int consumedItem = buffer.remove(0); // Consume one item
                System.out.println("Consumer consumed item " + consumedItem);
                System.out.println("Items left in buffer: " + buffer.size());
            }
            System.out.println();

            prod.release(); // Unlock buffer
            limit.release(); // Signal an empty slot is available
        } catch (Exception e) {
            System.out.println("Error: " + e);
        }
    }
}

class Producer implements Runnable {
    Container container;
    Thread t;

    Producer(Container c) {
        this.container = c;
        t = new Thread(this, "Producer");
        t.start();
    }

    public void run() {
        try {
            for (int i = 0; i < 100; i++) {
                container.put(i);
                Thread.sleep(5); // Simulate production delay
            }
        } catch (InterruptedException e) {
            System.out.println("Error: " + e);
        }
    }
}

class Consumer implements Runnable {
    Container container;
    Thread t;

    Consumer(Container c) {
        this.container = c;
        t = new Thread(this, "Consumer");
        t.start();
    }

    public void run() {
        try {
            for (int i = 0; i < 100; i++) {
                container.get();
                Thread.sleep(30); // Simulate consumption delay
            }
        } catch (InterruptedException e) {
            System.out.println("Error: " + e);
        }
    }
}

class Producer_Consumer {
    public static void main(String[] args) {
        Container container = new Container();
        Producer producer = new Producer(container);
        Consumer consumer = new Consumer(container);
    }
}
