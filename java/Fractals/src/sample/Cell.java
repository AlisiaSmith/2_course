package sample;


public class Cell {

    private boolean states = false;
    private double x;
    private double y;

    public Cell () {}

    public void setOn()  {states = true;}
    public void setOff() {states = false;}
    public boolean getStates() {return states;}

    public void setX(double x) { this.x = x; }
    public void setY(double y) { this.y = y; }
    public void setXY(double x, double y)
    {
        setX(x);
        setY(y);
        calc();
    }

    public double getX() { return x; }
    public double getY() { return y; }

    public void calc(double re, double im)
    {
        double rez = re;
        double imz = im;

        int iterations = 0;
        int max = 800;
        while (rez*rez+imz*imz < 4 && iterations < max) {
            double x_new = rez*rez-imz*imz+re;
            imz = 2*rez*imz+im;
            rez = x_new;
            iterations++;
        }
        if (iterations < max)
            setOff();
        else
            setOn();

    }

    public void calc() { calc(x,y);}
}
