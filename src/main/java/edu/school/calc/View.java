package edu.school.calc;

import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.stage.Stage;
import java.io.IOException;
import java.util.Objects;
public class View extends javafx.application.Application {
    FXMLLoader fxmlLoader;
    Scene scene;
    @Override
    public void start(Stage stage) throws IOException {
        fxmlLoader = new FXMLLoader(View.class.getResource("view.fxml"));
        scene = new Scene(fxmlLoader.load(), 610, 488);
        Presenter presenter = fxmlLoader.getController();
        presenter.setHotKeys(fxmlLoader, scene);

        try {
            stage.getIcons().add(new Image(Objects.requireNonNull(getClass().getResourceAsStream("icon.png"))));
            scene.getStylesheets().add(Objects.requireNonNull(getClass().getResource("style.css")).toString());
        } catch (Exception e) {
            e.printStackTrace();
        }

        stage.setTitle("Calc");
        stage.setScene(scene);
        stage.setResizable(false);
        stage.show();
    }
    public static void run() {
            launch();
    }
}
