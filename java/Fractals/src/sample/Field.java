package sample;

import java.util.ArrayList;

public class Field {

    private int columns = 0;
    private int rows = 0;
    private double diam = 2;
    private double scale;

    private double xCentre;
    private double yCentre;

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

    public int getColumns() { return columns; }

    public int getRows() { return rows; }

    public boolean getStatus(int x, int y)  { return getCell(x, y).getStates(); }

    public Cell getCell(int x, int y){ return field.get(x).get(y); }

    public void setDiam(double diam) { this.diam = diam; }

    public void setScale(double scale) { this.scale = scale; }

    public void setCentre(double xCentre, double yCentre) {
        this.xCentre = xCentre;
        this.yCentre = yCentre;
    }

    public void setField(double num, double size)
    {
        double stepX = diam / columns;
        double stepY = diam / rows;
        double xStart = xCentre - ((double)columns / 2 - num * columns / size) * stepX;
        double yStart = yCentre - (double)rows / 2 * stepY;

        for(int i = (int) (num * columns / size), k = 0; i < (num + 1) * columns / size; k++, i++)
            for(int j = 0; j < rows; j++)
                getCell(i, j).setXY(xStart + stepX * k, yStart + stepY * j);
    }

    public void setZoom (int x, int y, boolean isZoomIn)
    {
        setCentre(getCell(x,y).getX(),getCell(x,y).getY());
        if (isZoomIn) {
            scale += 0.1;
            diam /= scale ;
        }
        else {
            diam *= scale;
            if (scale > 1) scale -= 0.1;
        }
        setDiam(diam);
    }


    public void show()
    {
        for(int i = 0; i < columns; i++)
        {
            for(int j = 0; j < rows; j++)
                if(getCell(i, j).getStates())
                    System.out.print(" ");
                else
                    System.out.print("+");
            System.out.println();
        }
    }
}