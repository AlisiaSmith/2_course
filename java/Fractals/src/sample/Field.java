package sample;



import java.util.ArrayList;

public class Field {

    private int columns = 0;
    private int rows = 0;
    private double diam;
    private double scale;

    private ArrayList<ArrayList<Cell>> field;

    public Field (int col, int row) {
        columns = col;
        rows = row;
        scale = 1.1;

        field = new ArrayList<>();
        for(int i = 0; i < columns; i++)
        {
            field.add(new ArrayList<>());
            for(int j = 0; j < rows; j++)
                field.get(i).add(new Cell());
        }

    }

    public void setField(double x, double y, double d)
    {
        diam = d;
        double stepX = diam / columns;
        double stepY = diam / rows;
        double xStart = x - (double)columns / 2 * stepX;
        double yStart = y - (double)rows / 2 * stepY;

        for(int i = 0; i < columns; i++)
            for(int j = 0; j < rows; j++)
                field.get(i).get(j).setXY(xStart + stepX * i, yStart + stepY * j);
    }

    public void resize (int x, int y, boolean isZoomIn)
    {
        if (isZoomIn) {
            scale += 0.1;
            diam /= scale ;
        }
        else {
            diam *= scale;
            if(scale > 1) scale -= 0.1;
        }

        System.out.println(diam);
        setField(field.get(x).get(y).getX(), field.get(x).get(y).getY(), diam);
    }

    public boolean getStatus(int x, int y)  { return field.get(x).get(y).getStates(); }

    public void setScale(double scale) { this.scale = scale; }

    public void show()
    {
        for(int i = 0; i < columns; i++)
        {
            for(int j = 0; j < rows; j++)
                if(field.get(i).get(j).getStates())
                    System.out.print(" ");
                else
                    System.out.print("+");
             System.out.println();
        }
    }
}

