package org.lsst.ccs.daq.imageapi.decoder;

/**
 *
 * @author tonyj
 */
public class CodeGen {

    public static void main(String[] args) {
        int bits = 0;
        for (int i = 0;; i++) {
            System.out.printf("d%d = input.get();\n", i%2);
            if (bits > 0) {
                System.out.printf("destination.write((d%d|d%d<<%d)& 0x2ffff);\n", (i - 1)%2, i%2, bits);
                System.out.printf("d%d >>= %d;\n", i%2, 18 - bits);
                bits -= 18;
            }
            bits += 32;
            while (bits >= 18) {
                System.out.printf("destination.write(d%d & 0x2ffff);\n", i%2);
                bits -= 18;
                System.out.printf("d%d >>= 18;\n", i%2);
            }
            if (bits == 0) {
                break;
            }
        }
    }
}
