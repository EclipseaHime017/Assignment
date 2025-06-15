//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
import java.util.Random;

class Matrix {
    int row;
    int col;
    float[] pdata;
}

public class Main {
    public static void main(String[] args) {
        int row1, row2, col1, col2;
        java.util.Scanner scanner = new java.util.Scanner(System.in);
        System.out.println("Please input a number as the row of your first matrix");
        row1 = scanner.nextInt();
        System.out.println("Please input a number as the column of your first matrix");
        col1 = scanner.nextInt();
        System.out.println("Please input a number as the row of your second matrix");
        row2 = scanner.nextInt();
        System.out.println("Please input a number as the column of your second matrix");
        col2 = scanner.nextInt();

        int[] size = {row1, row2, col1, col2, row1, col2};

        if (col1 != row2 || row1 <= 0 || row2 <= 0 || col1 <= 0 || col2 <= 0) {
            System.out.println("Invalid matrix. Please check your columns and rows.");
            return;
        } else {
            Matrix[] matrix = new Matrix[3];
            Matrix matrix1 = new Matrix();
            Matrix matrix2 = new Matrix();
            Matrix matrix3 = new Matrix();
            matrix[0] = matrix1;
            matrix[1] = matrix2;
            matrix[2] = matrix3;

            for (int i = 0; i < 3; i++) {
                matrix[i].row = size[2 * i];
                matrix[i].col = size[2 * i + 1];
                matrix[i].pdata = new float[matrix[i].row * matrix[i].col];
                if (matrix[i].pdata == null)
                    System.out.println("Wrong allocation; Fault Gate 1.");
            }

            geneM(matrix1);
            geneM(matrix2);
            mult(matrix1, matrix2, matrix3);

//            for (int i = 0; i < ptr3.row; i++) {
//                for (int j = 0; j < ptr3.col; j++)
//                    System.out.print(ptr3.pdata[i * ptr3.col + j] + " ");
//                System.out.println();
//            }

        }
    }

    static void geneM(Matrix ptr) {
        Random rand = new Random();
        for (int i = 0; i < ptr.row; i++) {
            for (int j = 0; j < ptr.col; j++) {
                float rand_float = rand.nextFloat();
                ptr.pdata[j + i * ptr.col] = rand_float;
            }
        }
    }

    static void mult(Matrix A, Matrix B, Matrix C) {
        long start, end;
        double time;
        if (A.col != B.row)
            System.out.println("Matrices are invalid! Be careful about that A column equals B row.");
        else {

            start = System.nanoTime();
            for (int i = 0; i < B.col; i++) {
                for (int j = 0; j < A.row; j++) {
                    for (int k = 0; k < A.col; k++) {
                        C.pdata[i + j * C.col] = 0;
                        C.pdata[i + j * B.col] += A.pdata[A.col * j + k] * B.pdata[B.col * k + i];
                    }
                }
            }
            end = System.nanoTime();
            time = (double) (end - start) / 1000000000.0;
            System.out.println("It costs " + time + " second.");
        }
    }
}
