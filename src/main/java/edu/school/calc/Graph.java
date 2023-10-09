package edu.school.calc;

import javafx.fxml.FXML;
import javafx.scene.chart.XYChart;
import java.io.IOException;

import static java.lang.Double.parseDouble;

public class Graph {
    Model model;
    public Graph(Model model_) { model = model_; }

    Boolean IsEqualDouble(double x, double y) { return Math.abs(x - y) <= 1e-6; }
    Boolean LessOrEqualDouble(double x, double y) { return IsEqualDouble(x, y) || (x <= y); }

    @FXML
    public XYChart.Series<Number, Number> getPoints(double start, double end, double step, String exp) {
        var series = new XYChart.Series<Number, Number>();
        for (double i = start; LessOrEqualDouble(i, end); i += step) {
            String resultString = model.computeExpression(exp, Double.toString(i));
            Number resultNumber = parseDouble(resultString);

            if (!Double.isNaN(resultNumber.doubleValue()) ) {
                series.getData().add(new XYChart.Data<>(i, resultNumber) );
            }
        }

        return series;
    }

}
