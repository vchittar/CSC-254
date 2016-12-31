/*
    Coordinator.java

    Serves to slow down execution of a collection of threads, so that
    behavior is visible on the screen, and to notify all running threads
    when the user wants them to die.

    Michael L. Scott, November 2016, based on code
    originally written in 2002.
 */

// The Coordinator serves to slow down execution, so that behavior is
// visible on the screen, and to notify all running threads when the user
// wants them to die.  DO NOT MODIFY THIS CLASS.
//
class Coordinator {
    private boolean open = true;
        // set to false temporarily when threads are supposed to die.
    private boolean running = true;
        // set to false when threads are supposed to pause.
    private int numThreads = 0;
        // number of active worker threads.  Maintained by register and
        // unregister methods.
    private int pauseIterations = 0;

    // A thread terminates early by throwing itself a KilledException.
    //
    public class KilledException extends Throwable {}

    public synchronized void register() {
        numThreads++;
    }

    public synchronized void unregister() {
        numThreads--;
        notifyAll();
            // so event thread knows to inspect numThreads again
    }

    // Pause or die if so instructed.
    //
    private synchronized void gate()
            throws KilledException {
        if (!open) {
            throw new KilledException();
        }
        while (!running) {
            try {
                wait();
            } catch(InterruptedException e) {};
            if (!open) {
                throw new KilledException();
            }
        }
    }

    // Wait a bit before proceeding through gate.
    //
    public void hesitate() throws KilledException {
        spin(pauseIterations);
        gate();
    }

    // Toggle running.  Resume paused threads if appropriate.
    //
    public synchronized void toggle() {
        running = !running;
        if (running) {
            notifyAll();
        }
    }

    // Spin for specified number of iterations.
    //
    private void spin(long iters) {
        do {
        } while (--iters > 0);
    }

    // Kill all threads using the coordinator.
    //
    public synchronized void stop() {
        running = false;
        open = false;
        notifyAll();
        while (numThreads > 0) {
            try {
                wait();
            } catch(InterruptedException e) {};
        }
        open = true;
        running = true;
    }

    // If positive, count is number of iterations of empty loop to wait
    // in hesitate().  If negative, it's the approximate number of nanoseconds,
    // which we turn into iterations via a brief calibration exercise.
    public Coordinator(int count) {
        if (count > 0) {
            pauseIterations = count;
        } else {
            count = -count;
            long nanos = Integer.MAX_VALUE;
            final int oneMillion = 100000;
            // run multiple trials in case I get preempted during one or more
            for (int i = 0; i < 10; i++) {
                long start = System.nanoTime();
                spin(oneMillion);
                long trial = System.nanoTime() - start;
                if (trial < nanos) {
                    nanos = trial;
                }
            }
            // Now nanos is a good estimate of the number of nanoseconds
            // consumed by spin(oneMillion).
            // Compute the spin argument that would result in a delay of
            // approximately count nanoseconds.
            // NB: oneMillion/nanos = pauseIterations/count
            pauseIterations = (int) ((double)oneMillion / (double)nanos * (double)count);
            System.err.printf("running %d spin iterations per dot\n", pauseIterations);
        }
    }
}
