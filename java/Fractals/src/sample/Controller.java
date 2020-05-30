package sample;


import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.ColorPicker;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;

import java.util.ArrayList;


public class Controller {

    private double x, y;
    private int widthContext;
    private int heightContext;

    private int fieldsNumber = 4;
    private Field field;

    private boolean ZOOM_IN = true;
    private boolean ZOOM_OUT = false;

    private GraphicsContext context;
    private Color backCol;
    private Color frCol;;

    public Controller() {
        widthContext = 800;
        heightContext = 800;
        backCol = Color.rgb(130,180,130);
        frCol = Color.rgb(100,25,50);
    }

    private void treadsRun()
    {
        ArrayList <ThreadField> tf = new ArrayList<ThreadField>();


        for(int i = 0; i < fieldsNumber; i++)
        {
            tf.add(new ThreadField(field, i, fieldsNumber));
            tf.get(i).start();
        }
        for(int i = 0; i < fieldsNumber; i++)
            try {
                tf.get(i).join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
    }


    private void paint()
    {
        context.setFill(backCol);
        context.fillRect(0,0, widthContext, heightContext);
        context.setFill(frCol);


        for (int i = 0; i < widthContext; i++)
            for (int j = 0; j < heightContext; j++)
                if (field.getStatus(i, j))
                    context.fillRect(i, j, 1, 1);
    }

    @FXML
    private Canvas canvas;

    @FXML
    private ColorPicker fractalColorPicker;

    @FXML
    private ColorPicker backgroungColorPicker;

    @FXML
    void changeBackgroundColor(ActionEvent event) {
        backCol = backgroungColorPicker.getValue();
        paint();
    }

    @FXML
    void changeFractalColor(ActionEvent event) {
        frCol = fractalColorPicker.getValue();
        paint();
    }

    @FXML
    void restart(ActionEvent event) {
        x = -0.5; y = 0;
        field.setScale(1.1);
        field.setField(x, y, 2);

        treadsRun();
        paint();
    }

    @FXML
    void zooming(MouseEvent event) {
        x = event.getX();
        y = event.getY();

        if(event.isControlDown())
            field.resize((int)x,(int)y,ZOOM_OUT);
        else
            field.resize((int)x,(int)y,ZOOM_IN);

        treadsRun();
        paint();
    }


    @FXML
    void initialize() {

        field = new Field(widthContext, heightContext);
        context = canvas.getGraphicsContext2D();
        backgroungColorPicker.setValue(backCol);
        fractalColorPicker.setValue(frCol);

        restart(null);

    }
}