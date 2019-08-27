package org.lsst.ccs.daq.ims;

import org.junit.After;
import org.junit.Assume;
import static org.junit.Assume.assumeNotNull;
import org.junit.Before;
import org.junit.Test;

/**
 *
 * @author tonyj
 */
public class AssumeTest {


    @Before
    public void setUp() throws DAQException {
        assumeNotNull(new Object());
    }

    @After
    public void tearDown() {
    }

    @Test
    public void test1() {
        System.out.println("Running test1 as expected");
    }

    @Test
    public void test2() {
        Assume.assumeFalse(true);
        System.out.println("Running test2... This should not have happened");
    }

}
