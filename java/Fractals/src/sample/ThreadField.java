package sample;

import sample.Field;

public class ThreadField extends Thread {

    private Field field;
    private int tid;
    private int tsize;

    public ThreadField(Field f, int num, int size)
    {
        field = f;
        tid = num;
        tsize = size;
    }

    @Override
    public void run() {
        int col = field.getColumns() / tsize;
        int row = field.getRows();
        for (int i = tid * col; i < (tid +1) * col ; i++)
            for(int j = 0; j < row; j++)
                field.getCell(i, j).calc();

    }
}
