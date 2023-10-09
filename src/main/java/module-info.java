module edu.school.calc {
    requires javafx.fxml;
    requires javafx.controls;

    requires com.dlsc.formsfx;
    requires java.prefs;

    opens edu.school.calc to javafx.fxml;
    exports edu.school.calc to javafx.graphics;
}