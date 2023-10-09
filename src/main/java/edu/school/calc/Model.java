package edu.school.calc;

public class Model {
    CppLib cppLib = new CppLib();

    public String validExpression(String exp, String x) {
        return getResult("valid", exp, x);
    }

    public String computeExpression(String exp, String x) {
        return getResult("compute", exp, x).replaceFirst("\\.0*$|(\\.\\d*?)0+$", "$1");
    }

    private String getResult(String mode, String exp, String x) {
        return getNativeResult(mode, exp, x);
    }

    public String getNativeResult(String mode, String exp, String x) {
        return cppLib.getNativeResult(mode, exp, x);
    }
}
