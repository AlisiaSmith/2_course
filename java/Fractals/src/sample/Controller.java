package sample;


import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.ColorPicker;
import javafx.scene.paint.Color;
import sample.Field;


public class Controller {

    private Field field;

    private double x, y;
    private int widthContext;
    private int heightContext;

    private boolean ZOOM_IN = true;
    private boolean ZOOM_OUT = false;

    private GraphicsContext context;
    private Color backCol;
    private Color frCol;;

    public Controller() {
        widthContext = 800;
        heightContext = 800;
        backCol = Color.rgb(128,179,128);
        frCol = Color.rgb(102,26,51);
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
    private Button restartButton;

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
        backgroungColorPicker.setValue(backCol);
        fractalColorPicker.setValue(frCol);
        paint();
    }




    @FXML
    void initialize() {

        field = new Field(widthContext, heightContext);
        context = canvas.getGraphicsContext2D();
        restart(null);

        canvas.setOnMouseClicked(event -> {
            x = event.getX();
            y = event.getY();

            if(event.isControlDown())
                field.resize((int)x,(int)y,ZOOM_OUT);
            else
                field.resize((int)x,(int)y,ZOOM_IN);

            paint();
        });



    }
}
