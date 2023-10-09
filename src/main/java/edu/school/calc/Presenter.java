package edu.school.calc;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.input.KeyCode;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Pane;
import javafx.scene.layout.Region;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import java.io.IOException;

public class Presenter {
    Model model = new Model();
    Graph graph = new Graph(model);

    @FXML private TextField currentFocusedinputTextField;
    @FXML private TextField tittleXField;
    @FXML private TextField tittleExp;
    @FXML private TextField tittleExpGraph;
    @FXML private Label labelResultDisplay;

    @FXML private TabPane tabPane;
    @FXML private Pane graphChartPane;

    @FXML private Spinner<Double> spinnerFrom;
    @FXML private Spinner<Double> spinnerTo;

    @FXML private ListView<String> listViewHistory;
    @FXML private Label currentSelectedElementInHistoryList;

    @FXML
    public void initialize() {
        currentFocusedinputTextField=tittleExp;
        spinnerInit(spinnerFrom, -10);
        spinnerInit(spinnerTo, 10);
        setTabChangeListener();
        initHistoryListView();
    }

    void setHotKeys(FXMLLoader fxmlLoader, Scene scene) {
        scene.setOnKeyPressed(key -> {
            var code = key.getCode();
            if (code == KeyCode.EQUALS && key.isShiftDown()) code = KeyCode.PLUS;
            if (code == KeyCode.DIGIT0 && key.isShiftDown()) code = KeyCode.CLOSE_BRACKET;
            if (code == KeyCode.DIGIT6 && key.isShiftDown()) code = KeyCode.POWER;
            if (code == KeyCode.DIGIT8 && key.isShiftDown()) code = KeyCode.ASTERISK;
            if (code == KeyCode.DIGIT9 && key.isShiftDown()) code = KeyCode.OPEN_BRACKET;
            if (code == KeyCode.ENTER) code = KeyCode.EQUALS;
            if (key.getText().equals("%")) code = KeyCode.M; // mod

            if (key.getText().equals("c")) code = KeyCode.F1; // cos
            if (code == KeyCode.C && key.isShiftDown()) code = KeyCode.F2; // acos
            if (key.getText().equals("t")) code = KeyCode.F3; // tan
            if (code == KeyCode.T && key.isShiftDown()) code = KeyCode.F4; // atan
            if (key.getText().equals("s")) code = KeyCode.F5; // sin
            if (code == KeyCode.S && key.isShiftDown()) code = KeyCode.F6; // asin
            if (key.getText().equals("q")) code = KeyCode.F7; // sqrt
            if (key.getText().equals("l")) code = KeyCode.F8; // ln
            if (code == KeyCode.L && key.isShiftDown()) code = KeyCode.F9; // log


            switch (code) {
                case DIGIT0, DIGIT1, DIGIT2, DIGIT3, DIGIT4, DIGIT5,
                        DIGIT6, DIGIT7, DIGIT8, DIGIT9, PERIOD, EQUALS,
                        PLUS, MINUS, ASTERISK, SLASH, POWER, M, X, OPEN_BRACKET,
                        CLOSE_BRACKET, ESCAPE, BACK_SPACE, F1, F2, F3, F4, F5, F6, F7, F8, F9
                        -> ((Button) fxmlLoader.getNamespace().get(code.toString())).fire();
                default -> System.err.println("key -> " + code + " not set");
            }

        });
    }

    private void showAlert(String msg) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION, msg, ButtonType.OK);
        alert.setTitle("Error");
        alert.getDialogPane().setMinHeight(Region.USE_PREF_SIZE);
        alert.show();
    }

    @FXML
    protected void handleButtonDrawGraphic() throws IOException {
        graphChartPane.getChildren().clear();

        var exp = tittleExpGraph.getText().toLowerCase();
        double start = spinnerFrom.getValue();
        double end = spinnerTo.getValue();

        if ( (end <= start) ) {
            showAlert("Please, set correct range");
            return;
        }

        if (exp.isEmpty() || !model.validExpression(exp, "").equals("OK")) {
            showAlert("Cannot take result");
            return;
        }

        double range = Math.sqrt(Math.pow(start - end, 2));
        double step = range/200;

        NumberAxis xAxis = new NumberAxis(start, end, step);
        xAxis.setLabel("x");
        xAxis.setAutoRanging(true);

        NumberAxis yAxis = new NumberAxis(start, end, step);
        yAxis.setLabel("y");
        yAxis.setAutoRanging(true);

        LineChart<Number, Number> myChart = new LineChart<>(xAxis, yAxis);
        myChart.setLegendVisible(false);

        myChart.getData().add(graph.getPoints(start, end, step, exp));

        myChart.setPrefSize(graphChartPane.getMaxWidth(), graphChartPane.getHeight());
        myChart.setMaxWidth(graphChartPane.getMaxWidth());
        myChart.setMaxHeight(graphChartPane.getHeight());

        graphChartPane.getChildren().add(myChart);
    }

    private void updateHistoryListView() {
        try {
            listViewHistory.getItems().clear();
            listViewHistory.getItems().addAll(History.getAllRecords());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void addRecordToHistory(String res) {
        History.addRecord(res);
        updateHistoryListView();
    }

    @FXML
    private void handleClearButton() {
        try {
            History.clear();
            currentSelectedElementInHistoryList.setText("");
            listViewHistory.getItems().clear();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    private void initHistoryListView() {
        updateHistoryListView();

        listViewHistory.getSelectionModel().selectedItemProperty().addListener((observableValue, s, t1) -> {
            if(listViewHistory.getSelectionModel().getSelectedItem() != null) {
                var withoutDate = listViewHistory.getSelectionModel().getSelectedItem().split(": ")[1];
                if (withoutDate != null) {
                    currentSelectedElementInHistoryList.setText(withoutDate);
                    tittleExp.setText(withoutDate);
                }
            }
        });
    }


    public void spinnerInit(Spinner<Double> spinner, double initValue) {
        var valueFactory = new SpinnerValueFactory.DoubleSpinnerValueFactory(-10e6, 10e6, initValue, 1);
        spinner.setValueFactory(valueFactory);
        spinner.setOnScroll(event -> {
            if (event.getDeltaY() < 0) {
                spinner.decrement();
            } else if (event.getDeltaY() > 0) {
                spinner.increment();
            }
        });
    }

    protected void setTabChangeListener() {
        tabPane.getSelectionModel().selectedItemProperty().addListener((ov, t, t1) -> focusResetOnCurrentTab());
    }

    @FXML
    protected void handleButtonReset() {
        uiResetOnCurrentTab();
    }

    public void uiResetOnCurrentTab() {
        if(tabPane.getSelectionModel().getSelectedIndex() == 0) {
            labelResultDisplay.setText("0");
            tittleXField.clear();
            tittleExp.clear();
        } else if(tabPane.getSelectionModel().getSelectedIndex() == 1) {
            tittleExpGraph.clear();
            graphChartPane.getChildren().clear();
        } else if(tabPane.getSelectionModel().getSelectedIndex() == 2) {
            currentSelectedElementInHistoryList.setText("");
        }
    }

    public void focusResetOnCurrentTab() {
        if(tabPane.getSelectionModel().getSelectedIndex() == 0) {
            currentFocusedinputTextField=tittleExp;
            tittleExp.requestFocus();
        } else if(tabPane.getSelectionModel().getSelectedIndex() == 1) {
            currentFocusedinputTextField=tittleExpGraph;
            tittleExpGraph.requestFocus();
        } else if(tabPane.getSelectionModel().getSelectedIndex() == 2) {
            listViewHistory.getSelectionModel().clearSelection();
        }
    }

    @FXML
    protected void handleButtonEqual() {
        String exp = tittleExp.getText();
        String x = tittleXField.getText();

        if(exp.toLowerCase().contains("x") && x.isEmpty()) {
            labelResultDisplay.setText("please, set x");
            tittleXField.requestFocus();
            return;
        }
        try {
            if (!model.validExpression(exp, x).equals("OK")) {
                labelResultDisplay.setText("error");
                return;
            }
            String res = model.computeExpression(exp, x);
            if(res.isEmpty()) labelResultDisplay.setText("error");
            else {
                labelResultDisplay.setText(res);

                String replaced = exp.replace("x", x);
                addRecordToHistory(replaced);
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    @FXML
    protected void handleInputButtons(ActionEvent event) {
        Button source = (Button) event.getSource();

        currentFocusedinputTextField.setText(currentFocusedinputTextField.getText()+source.getText());
    }

    @FXML
    protected void handleDeleteButton() {
        if(!currentFocusedinputTextField.getText().isEmpty())
            currentFocusedinputTextField.setText(
                    currentFocusedinputTextField.getText().replaceFirst(".$","")
            );
    }

    @FXML
    protected void handleChangeInputField(MouseEvent event) {
        currentFocusedinputTextField=(TextField) event.getSource();
    }


}